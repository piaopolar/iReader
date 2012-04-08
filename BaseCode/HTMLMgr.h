#pragma once

#include <string>
#include <vector>

#include "Singleton.h"

class CHTMLMgr : public Singleton<CHTMLMgr>
{
public:
	CHTMLMgr(void);
	~CHTMLMgr(void);
	
	bool GetHtmlData(const char *pszUrl, std::vector<std::string> &vecStringData);
};
