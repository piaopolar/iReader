#pragma once
#include "../BaseCode/Singleton.h"

#include <string>
#include <vector>

class CBookSkyMgr : public Singleton<CBookSkyMgr>
{
public:
	struct BOOK_INFO
	{
		std::string m_strBookName;
		std::string m_strBookUrl;
		std::string m_strPicUrl;
		std::string m_strAuthor;
		std::string m_strAuthorUrl;
		std::string m_strDesc;
		std::string m_strLastUpdate;
		std::string m_strLastUpdateUrl;
	};

	enum { KEYWORD_TYPE_BOOKNAME, KEYWORD_TYPE_AUTHOR, };
public:
	CBookSkyMgr(void);
	~ CBookSkyMgr(void);
public:
	std::vector<CBookSkyMgr::BOOK_INFO> GetBookInfo(const char *pszKeyWord,
												   int nType);
};
