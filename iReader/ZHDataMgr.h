#pragma once

#include <string>
#include <vector>

#include "../BaseCode/Singleton.h"


class CZHDataMgr : public Singleton<CZHDataMgr>
{
public:
	enum
	{
		KEYWORD_TYPE_BOOKNAME,
	};
public:
	CZHDataMgr(void);
	~CZHDataMgr(void);

public:
	std::vector<std::string> GetBookUrl(const char* pszKeyWord, int nType = KEYWORD_TYPE_BOOKNAME);
};
