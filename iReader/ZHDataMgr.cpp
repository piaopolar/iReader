#include "StdAfx.h"

#include <boost/regex.hpp>
#include "../BaseCode/HTMLMgr.h"

#include "ZHDataMgr.h"

// ============================================================================
// ==============================================================================

CZHDataMgr::CZHDataMgr(void)
{
}

// ============================================================================
// ==============================================================================
CZHDataMgr::~CZHDataMgr(void)
{
}

// ============================================================================
// ==============================================================================
std::vector<CZHDataMgr::BOOK_INFO> CZHDataMgr::GetBookInfo(const char *pszKeyWord,
														   int nType	// KEYWORD_TYPE_BOOKNAME
																															   ///
													   )
{
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	std::vector<CZHDataMgr::BOOK_INFO> vecBookInfo;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	if (NULL == pszKeyWord) {
		return vecBookInfo;
	}

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	char szUrlSearchResult[MAX_STRING];
	std::string strType = "all";
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	// http://search.zongheng.com/search/bookName/keyword/1.html
	switch (nType) {
	case KEYWORD_TYPE_BOOKNAME: strType = "bookName"; break;
	}

	_snprintf(szUrlSearchResult, sizeof(szUrlSearchResult),
			  "http://search.zongheng.com/search/%s/%s/1.html", strType.c_str(),
			  pszKeyWord);

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	std::vector<std::string> vecSearchResult;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	if (!CHTMLMgr::GetInstance().GetHtmlData(szUrlSearchResult, vecSearchResult
		)) {
		return vecBookInfo;
	}

	std::vector < std::string >::const_iterator it(vecSearchResult.begin());
	for (; it != vecSearchResult.end(); ++it) {
		if (strstr(it->c_str(), "<div id=\"booksearch\"")) {
			break;
		}
	}

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~
	int nInfoCount = 0;
	CZHDataMgr::BOOK_INFO infoBook;
	boost::cmatch what;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~

	boost::regex expBookInfo("(.*?)<a href=\"http://book.zongheng.com/book/(.*?)\"(.*?)>(.*?)</a>");
	boost::regex expAuthor("(.*?)作者：<a href=\"http://home.zongheng.com/show/userInfo/(.*?)>(.*?)</a>(.*?)");

	for (; it != vecSearchResult.end(); ++it) {

		//~~~~~~~~~~~~~~~~~~~~~~
		std::string strLine = *it;
		//~~~~~~~~~~~~~~~~~~~~~~

		ReplaceStdString(strLine, "<span class=\"fred\">", "");
		ReplaceStdString(strLine, "</span>", "");
		ReplaceStdString(strLine, "<em>", "");
		ReplaceStdString(strLine, "</em>", "");
		if (boost::regex_match(strLine.c_str(), what, expBookInfo)) {
			infoBook.m_strBookName = what[4].str();
			infoBook.m_strBookUrl = "http://book.zongheng.com/book/";
			infoBook.m_strBookUrl += what[2].str();
			++nInfoCount;
		}

		if (boost::regex_match(strLine.c_str(), what, expAuthor)) {
			infoBook.m_strAuthor = what[3].str();
			++nInfoCount;
		}

		if (nInfoCount >= 2) {
			nInfoCount = 0;
			vecBookInfo.push_back(infoBook);

			LogInfoIn("读取到书籍信息\n%s\n%s\n%s\n",
					  infoBook.m_strAuthor.c_str(),
					  infoBook.m_strBookName.c_str(),
					  infoBook.m_strBookUrl.c_str());
			break;
		}
	}

	return vecBookInfo;
}
