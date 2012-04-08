#include "StdAfx.h"

#include "../BaseCode/HTMLMgr.h"

#include "ZHDataMgr.h"

CZHDataMgr::CZHDataMgr(void)
{
}

CZHDataMgr::~CZHDataMgr(void)
{
}

std::vector<std::string> CZHDataMgr::GetBookUrl( const char* pszKeyWord, int nType /*= KEYWORD_TYPE_BOOKNAME*/ )
{
	std::vector<std::string> vecBookUrl;
	if (NULL == pszKeyWord) {
		return vecBookUrl;
	}

	char szUrlSearchResult[MAX_STRING];
	std::string strType = "all";
	// http://search.zongheng.com/search/bookName/keyword/1.html
	switch (nType) {
	case KEYWORD_TYPE_BOOKNAME:
		strType = "bookName";
		break;
	}

	_snprintf(szUrlSearchResult, sizeof(szUrlSearchResult), "http://search.zongheng.com/search/%s/%s/1.html", strType.c_str(), pszKeyWord);
	std::string urlUTF8;
	AnsiToUTF8(szUrlSearchResult, urlUTF8);
	std::vector<std::string> vecSearchResult;
	if (!CHTMLMgr::GetInstance().GetHtmlData(urlUTF8.c_str(), vecSearchResult)) {
		return vecBookUrl;
	}
	
	std::vector<std::string>::const_iterator it(vecSearchResult.begin());
	for (; it != vecSearchResult.end(); ++it) {
		if (strstr(it->c_str(), "<div id=\"booksearch\"")) {
			break;
		}
	}

	return vecBookUrl;
}
