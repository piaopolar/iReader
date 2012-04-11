#include "StdAfx.h"

#include <boost/regex.hpp>

#include "../BaseCode/HTMLMgr.h"

#include "BookSkyMgr.h"

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
std::vector<CBookSkyMgr::BOOK_INFO> CBookSkyMgr::GetBookInfo(const char *pszKeyWord,
															 int nType)
{
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	std::vector<CBookSkyMgr::BOOK_INFO> vecBookInfo;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	if (NULL == pszKeyWord) {
		return vecBookInfo;
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

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	std::vector<std::string> vecSearchResult;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	if (!CHTMLMgr::GetInstance().GetHtmlData(szUrlSearchResult, vecSearchResult
		)) {
		return vecBookInfo;
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

	//~~~~~~~~~~~~~~~~~~~~~~~
	bool bHaveNextPage = false;
	//~~~~~~~~~~~~~~~~~~~~~~~

	for (; it != vecSearchResult.end(); ++it) {

		//~~~~~~~~~~~~~~~~~~~~~~
		std::string strLine = *it;
		//~~~~~~~~~~~~~~~~~~~~~~

		if (boost::regex_match(strLine.c_str(), what, expNextPage)) {
			bHaveNextPage = true;
			LogInfoIn("下一页搜索页链接 %s", what[2].str().c_str());
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
				vecBookInfo.push_back(infoBook);
			}
			break;
		default:
			break;
		}
	}

	return vecBookInfo;
}
