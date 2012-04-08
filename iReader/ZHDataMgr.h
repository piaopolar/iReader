#pragma once
#include <string>
#include <vector>

#include "../BaseCode/Singleton.h"

class CZHDataMgr : public Singleton<CZHDataMgr>
{
public:
	struct BOOK_INFO
	{
		std::string m_strBookName;
		std::string m_strBookUrl;
		std::string m_strAuthor;
	};
public:
	enum { KEYWORD_TYPE_BOOKNAME, };
public:
	CZHDataMgr(void);
	~ CZHDataMgr(void);
public:
	std::vector<CZHDataMgr::BOOK_INFO> GetBookInfo(const char *pszKeyWord,
												   int nType = KEYWORD_TYPE_BOOKNAME);
};
