#include "StdAfx.h"

#include <boost/regex.hpp>

#include "../BaseCode/HTMLMgr.h"

#include "BookSkyMgr.h"

// ============================================================================
// ==============================================================================

CBookSkyMgr::CHAPTER_INFO::CHAPTER_INFO()
{
	m_nNo = -1;
	m_bTextInit = false;
}

// ============================================================================
// ==============================================================================
void CBookSkyMgr::CHAPTER_INFO::InitTextInfo(void)
{
	if (m_bTextInit) {
		return;
	}

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	std::vector<std::string> vecHtmlData;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	if (!CHTMLMgr::GetInstance().GetHtmlData(m_strUrl.c_str(), vecHtmlData)) {
		return;
	}

	std::vector < std::string >::const_iterator it(vecHtmlData.begin());
	boost::regex regWaterMark("<span class=\"watermark\">(.*?)</span>");

	m_strText = "";

	for (; it != vecHtmlData.end(); ++it) {

		//~~~~~~~~~~~~~~~~~~~~~~
		std::string strLine = *it;
		//~~~~~~~~~~~~~~~~~~~~~~

		if (strstr(strLine.c_str(), "<span class=\"watermark\">")) {
			ReplaceStdString(strLine, "<p>", "\n");
			ReplaceStdString(strLine, "</p>", "\n");
			m_strText += boost::regex_replace(strLine, regWaterMark, "");
		}
	}
}

// ============================================================================
// ==============================================================================
std::string CBookSkyMgr::CHAPTER_INFO::GetTextDumpStr(void)
{
	if (!m_bTextInit) {
		this->InitTextInfo();
	}

	return m_strText;
}

// ============================================================================
// ==============================================================================
CBookSkyMgr::BOOK_INFO::BOOK_INFO()
{
	m_bSiteInfoInit = false;
}

// ============================================================================
// ==============================================================================
std::string CBookSkyMgr::BOOK_INFO::GetBasicInfoDumpStr(void) const
{
	//~~~~~~~~~~~~~~~~
	std::string strDump;
	//~~~~~~~~~~~~~~~~

	strDump += std::string("\nBookName:") + m_strBookName;
	strDump += std::string("\nBookUrl:") + m_strBookUrl;
	strDump += std::string("\nPicUrl:") + m_strPicUrl;
	strDump += std::string("\nAuthor:") + m_strAuthor;
	strDump += std::string("\nAuthorUrl:") + m_strAuthorUrl;
	strDump += std::string("\nDesc:") + m_strDesc;
	strDump += std::string("\nLastUpdate:") + m_strLastUpdate;
	strDump += std::string("\nLastUpdateUrl:") + m_strLastUpdateUrl;
	strDump += "\n";
	return strDump;
}

// ============================================================================
// ==============================================================================
void CBookSkyMgr::BOOK_INFO::InitSiteInfo(void)
{
	if (m_bSiteInfoInit) {
		return;
	}

	m_bSiteInfoInit = true;
	m_vecSiteInfo = CBookSkyMgr::GetInstance().GetSiteInfo(m_strBookUrl.c_str());
}

std::string CBookSkyMgr::BOOK_INFO::GetSiteInfoDumpStr (void) {
	this->InitSiteInfo();

	std::string strDump;
	std::vector<SITE_INFO>::const_iterator it(m_vecSiteInfo.begin());
	for (; it != m_vecSiteInfo.end(); ++it) {
		strDump += std::string("\nSiteName:") + it->m_strSiteName;
		strDump += std::string("\nSiteUrl: :") + it->m_strSiteUrl;
		strDump += "\n";
	}
	return strDump;
}

// ============================================================================
// ==============================================================================
void CBookSkyMgr::BOOK_INFO::InitChapterInfo(int nSite)
{
	this->InitSiteInfo();

	if (0 <= nSite && nSite < m_vecSiteInfo.size()) {
		m_vecSiteInfo.at(nSite).InitChapterInfo();
	}
}

// ============================================================================
// ==============================================================================
std::string CBookSkyMgr::BOOK_INFO::GetChapterDumpStr(int nSite)
{
	if (!m_bSiteInfoInit) {
		this->InitSiteInfo();
	}

	if (0 <= nSite && nSite < m_vecSiteInfo.size()) {
		return m_vecSiteInfo.at(nSite).GetChapterDumpStr();
	}

	return "";
}

// ============================================================================
// ==============================================================================
std::string CBookSkyMgr::BOOK_INFO::GetTextDumpStr(int nSite, int nChapter)
{
	if (!m_bSiteInfoInit) {
		this->InitSiteInfo();
	}

	if (0 <= nSite && nSite < m_vecSiteInfo.size()) {
		return m_vecSiteInfo.at(nSite).GetTextDumpStr(nChapter);
	}
}

// ============================================================================
// ==============================================================================
CBookSkyMgr::CBookSkyMgr(void)
{
}

// ============================================================================
// ==============================================================================
CBookSkyMgr::~CBookSkyMgr(void)
{
}

// ============================================================================
// ==============================================================================
CBookSkyMgr::BOOK_SET_INFO CBookSkyMgr::GetBookInfo(const char *pszKeyWord,
													int nType)
{
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	CBookSkyMgr::BOOK_SET_INFO infoBookSet;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	if (NULL == pszKeyWord) {
		return infoBookSet;
	}

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	char szUrlSearchResult[MAX_STRING];
	std::string strType = "ForBookName";
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	switch (nType) {
	case KEYWORD_TYPE_AUTHOR:	strType = "ForAuthorName"; break;
	default:					break;
	}

	_snprintf(szUrlSearchResult, sizeof(szUrlSearchResult),
			  "http://www.booksky.org/Search.aspx?BookName=%s&SearchType=%s", pszKeyWord,
			  strType.c_str());

	//~~~~~~~~~~~~~~~~~~~
	SEARCH_INFO infoSearch;
	//~~~~~~~~~~~~~~~~~~~

	infoSearch.m_strKeyWord = pszKeyWord;
	infoSearch.m_nPage = 0;
	infoSearch.m_nType = nType;
	m_mapUrl2SearchInfo[szUrlSearchResult] = infoSearch;

	return this->GetBookInfo(szUrlSearchResult);
}

// ============================================================================
// ==============================================================================
CBookSkyMgr::BOOK_SET_INFO CBookSkyMgr::GetBookInfo(const char *pszSearchUrl)
{
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	CBookSkyMgr::BOOK_SET_INFO infoBookSet;
	std::vector<std::string> vecSearchResult;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	if (!CHTMLMgr::GetInstance().GetHtmlData(pszSearchUrl, vecSearchResult)) {
		return infoBookSet;
	}

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	int nInfoStep = 0;
	CBookSkyMgr::BOOK_INFO infoBook;
	boost::cmatch what;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	boost::regex expBookInfo("(.*?)书名：<a href=\"(.*?)\"><b>(.*?)</b></a>　作者：<a href=\"(.*?)\">(.*?)</a>(.*?)");
	boost::regex expPicUrl("(.*?)<td width=\"([0-9]+)\" valign=\"top\"><img src=\"(.*?)\" width=\"([0-9]+)\" height=\"([0-9]+)\"></td>(.*?)");
	boost::regex expDesc("(.*?)<td valign=\"top\"><br>(.*?)<br> <br><br> </td>(.*?)");
	boost::regex expUpdate("(.*?)最近更新：<a class=\"buy\" href=\"(.*?)\">(.*?)</a>(.*?)<br><br></td>(.*?)");
	boost::regex expNextPage("(.*?)<td bgcolor=\"#EEEEEE\" align=\"right\" colspan=\"2\"><a href='(.*?)'>更多..</a></td>(.*?)");

	std::vector < std::string >::const_iterator it(vecSearchResult.begin());

	for (; it != vecSearchResult.end(); ++it) {

		//~~~~~~~~~~~~~~~~~~~~~~
		std::string strLine = *it;
		//~~~~~~~~~~~~~~~~~~~~~~

		if (boost::regex_match(strLine.c_str(), what, expNextPage)) {
			infoBookSet.m_strNextPage = what[2].str();

			//~~~~~~~~~~~~~~~~~~~
			SEARCH_INFO infoSearch;
			//~~~~~~~~~~~~~~~~~~~

			if (this->GetUrlSearchInfo(pszSearchUrl, infoSearch)) {
				++infoSearch.m_nPage;
				m_mapUrl2SearchInfo[what[2].str()] = infoSearch;
			}
			break;
		}

		if (boost::regex_match(strLine.c_str(), what, expBookInfo)) {
			infoBook = CBookSkyMgr::BOOK_INFO();
			infoBook.m_strBookUrl = what[2].str();
			infoBook.m_strBookName = what[3].str();
			infoBook.m_strAuthorUrl = what[4].str();
			infoBook.m_strAuthor = what[5].str();
			nInfoStep = 1;
		}

		switch (nInfoStep) {
		case 1:
			if (boost::regex_match(strLine.c_str(), what, expPicUrl)) {
				infoBook.m_strPicUrl = what[3].str();
				++nInfoStep;
			}
			break;
		case 2:
			// 有可能没简介
			if (boost::regex_match(strLine.c_str(), what, expDesc)) {
				infoBook.m_strDesc = what[2].str();
				ReplaceStdString(infoBook.m_strDesc, "<br>", "\n");
			}

			if (boost::regex_match(strLine.c_str(), what, expUpdate)) {
				infoBook.m_strLastUpdateUrl = what[2].str();
				infoBook.m_strLastUpdate = what[3].str();
				infoBookSet.m_vecBookInfo.push_back(infoBook);
			}
			break;
		default:
			break;
		}
	}

	return infoBookSet;
}

// ============================================================================
// ==============================================================================
bool CBookSkyMgr::GetUrlSearchInfo(const char *pszUrl,
								   OUT SEARCH_INFO &rInfo) const
{
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	std::map<std::string, SEARCH_INFO>::const_iterator it = m_mapUrl2SearchInfo.
		find(pszUrl);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	if (it == m_mapUrl2SearchInfo.end()) {
		return false;
	}

	rInfo = it->second;
	return true;
}

// ============================================================================
// ==============================================================================
std::vector<CBookSkyMgr::SITE_INFO> CBookSkyMgr::GetSiteInfo(const char *pszBookUrl)
{
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	std::vector<CBookSkyMgr::SITE_INFO> vecInfoSite;
	std::vector<std::string> vecHtmlData;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	if (!CHTMLMgr::GetInstance().GetHtmlData(pszBookUrl, vecHtmlData)) {
		return vecInfoSite;
	}

	std::vector < std::string >::const_iterator it(vecHtmlData.begin());

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	std::vector<std::vector<std::string> > vecRegexResult;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	for (; it != vecHtmlData.end(); ++it) {

		//~~~~~~~~~~~~~~~~~~~~~~
		std::string strLine = *it;
		//~~~~~~~~~~~~~~~~~~~~~~

		if (strstr(strLine.c_str(), "slist")) {
			RegexSplitMul(strLine, "slist\\(([0-9]+),\"(.*?)\",([0-9]+)\\);",
						  vecRegexResult);
			break;
		}
	}

	std::vector < std::vector < std::string > >::const_iterator itResult(vecRegexResult.begin());
	for (; itResult != vecRegexResult.end(); ++itResult) {

		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		CBookSkyMgr::SITE_INFO infoSite;
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~

		infoSite.m_strSiteUrl = std::string("http://www.booksky.org/ReadDirectory.aspx?DirectoryID=") + itResult->at(1);
		infoSite.m_strSiteName = itResult->at(2);
		vecInfoSite.push_back(infoSite);
	}

	return vecInfoSite;
}

// ============================================================================
// ==============================================================================
CBookSkyMgr::SITE_INFO::SITE_INFO()
{
	m_bChapterInfoInit = false;
}

// ============================================================================
// ==============================================================================
void CBookSkyMgr::SITE_INFO::InitChapterInfo(void)
{
	if (m_bChapterInfoInit) {
		return;
	}

	m_bChapterInfoInit = true;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	std::vector<std::string> vecHtmlData;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	if (!CHTMLMgr::GetInstance().GetHtmlData(m_strSiteUrl.c_str(), vecHtmlData)) {
		return;
	}

	std::vector < std::string >::const_iterator it(vecHtmlData.begin());

	//~~~~~~~~~~~~~~~
	boost::cmatch what;
	//~~~~~~~~~~~~~~~

	boost::regex expRealUrl("(.*?)<frame name=\"(.*?)\" src=\"(.*?)\">");

	//~~~~~~~~~~~~~~~~~~~
	std::string strRealUrl;
	//~~~~~~~~~~~~~~~~~~~

	for (; it != vecHtmlData.end(); ++it) {

		//~~~~~~~~~~~~~~~~~~~~~~
		std::string strLine = *it;
		//~~~~~~~~~~~~~~~~~~~~~~

		if (boost::regex_match(strLine.c_str(), what, expRealUrl)) {
			strRealUrl = what[3].str();
		}
	}

	if (!CHTMLMgr::GetInstance().GetHtmlData(strRealUrl.c_str(), vecHtmlData)) {
		return;
	}

	boost::regex expChapter("(.*?)<a href=\"(.*?)\" title=\"最后更新时间:(.*?)  字数:(.*?)\">");

	//~~~~~~~~~~~~~~~~~~~~~
	int nNo = 0;
	CHAPTER_INFO infoChapter;
	//~~~~~~~~~~~~~~~~~~~~~

	for (it = vecHtmlData.begin(); it != vecHtmlData.end(); ++it) {

		//~~~~~~~~~~~~~~~~~~~~~~
		std::string strLine = *it;
		//~~~~~~~~~~~~~~~~~~~~~~

		if (boost::regex_match(strLine.c_str(), what, expChapter)) {
			infoChapter.m_nNo = ++nNo;
			infoChapter.m_strUrl = what[2].str();
			infoChapter.m_strLastUpdate = what[3].str();
			infoChapter.m_strExtra = std::string("字数:") + what[4].str();
			continue;
		}

		if (infoChapter.m_nNo >= 0) {
			infoChapter.m_strName = MyTrim(strLine);
			m_vecChapterInfo.push_back(infoChapter);
			infoChapter = CHAPTER_INFO();
		}
	}
}

std::string CBookSkyMgr::SITE_INFO::GetChapterDumpStr (void) {
	if (!m_bChapterInfoInit) {
		this->InitChapterInfo();
	}
	std::string strDump;
	std::vector<CHAPTER_INFO>::const_iterator it(m_vecChapterInfo.begin());
	for (; it != m_vecChapterInfo.end(); ++it) {
		strDump += std::string("\nChapterName:") + it->m_strName;
		strDump += std::string("\nChapterUrl: :") + it->m_strUrl;
		strDump += std::string("\nChapterLastUpdate: ") + it->m_strLastUpdate;
		strDump += std::string("\nChapterExtra: :") + it->m_strExtra;
		strDump += "\n";
	}
	return strDump;
}

// ============================================================================
// ==============================================================================
std::string CBookSkyMgr::SITE_INFO::GetTextDumpStr(int nChapterIndex)
{
	if (!m_bChapterInfoInit) {
		InitChapterInfo();
	}

	if (0 <= nChapterIndex && nChapterIndex < m_vecChapterInfo.size()) {
		return m_vecChapterInfo.at(nChapterIndex).GetTextDumpStr();
	}

	return "";
}
