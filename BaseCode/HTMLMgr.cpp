#include "StdAfx.h"

#include <afxinet.h>
#include "BaseFunc.h"

#include "HTMLMgr.h"

CHTMLMgr::CHTMLMgr(void)
{
}

CHTMLMgr::~CHTMLMgr(void)
{
}

bool CHTMLMgr::GetHtmlData( const char *pszUrl, std::vector<std::string> &vecStringData )
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

	//~~~~~~~~~~~
	// 将网页内容的源代码读至编辑框
	CString myData;
	//~~~~~~~~~~~

	myHttpFile = (CHttpFile *)mySession.OpenURL(pszUrl);

	if (NULL == myHttpFile) {
		LogInfoIn("连接失败");
		return false;
	}

	LogInfoIn("连接成功，读取中...");

	while (myHttpFile->ReadString(myData)) {
		vecStringData.push_back(myData.GetBuffer(0));
		//		vecStringData.push_back(Big2GB(myData.GetBuffer(0)));
	}

	LogInfoIn("读取完成");

	myHttpFile->Close();
	mySession.Close();
	return true;
}
