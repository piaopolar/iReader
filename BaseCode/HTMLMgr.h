#pragma once

class CHTMLMgr
{
public:
	CHTMLMgr(void);
	~CHTMLMgr(void);
	
	bool GetHtmlData(const char *pszUrl, std::vector<std::string> &vecStringData);
};
