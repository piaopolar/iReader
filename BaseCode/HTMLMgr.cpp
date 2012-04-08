#include "StdAfx.h"

#include <afxinet.h>
#include "BaseFunc.h"

#include "HTMLMgr.h"

// ============================================================================
// ==============================================================================

CHTMLMgr::CHTMLMgr(void)
{
}

// ============================================================================
// ==============================================================================
CHTMLMgr::~CHTMLMgr(void)
{
}

// ============================================================================
// ==============================================================================
bool CHTMLMgr::GetHtmlData(const char *pszUrl,
						   std::vector<std::string> &vecStringData)
{
	if (NULL == pszUrl) {
		return false;
	}

	vecStringData.clear();

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	CInternetSession mySession(NULL, 0);	// 建立会话
	CHttpFile *myHttpFile = NULL;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	LogInfoIn("连接到Url %s...", pszUrl);

	//~~~~~~~~~~~~~~~~~~~
	std::string strUrlUTF8;
	//~~~~~~~~~~~~~~~~~~~

	AnsiToUTF8(pszUrl, strUrlUTF8);

	//~~~~~~~~~~~
	// 将网页内容的源代码读至编辑框
	CString myData;
	//~~~~~~~~~~~

	myHttpFile = (CHttpFile *)mySession.OpenURL(strUrlUTF8.c_str());

	if (NULL == myHttpFile) {
		LogInfoIn("连接失败");
		return false;
	}

	LogInfoIn("连接成功，读取中...");

	while (myHttpFile->ReadString(myData)) {

		//~~~~~~~~~~~~~~~~
		std::string strANSI;
		//~~~~~~~~~~~~~~~~

		UTF8ToAnsi(myData.GetBuffer(0), strANSI);
		vecStringData.push_back(strANSI);
	}

	LogInfoIn("读取完成");

	myHttpFile->Close();
	mySession.Close();
	return true;
}
