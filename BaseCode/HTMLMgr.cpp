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
	CInternetSession mySession(NULL, 0);	// �����Ự
	CHttpFile *myHttpFile = NULL;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	LogInfoIn("���ӵ�Url %s...", pszUrl);

	//~~~~~~~~~~~
	// ����ҳ���ݵ�Դ��������༭��
	CString myData;
	//~~~~~~~~~~~

	myHttpFile = (CHttpFile *)mySession.OpenURL(pszUrl);

	if (NULL == myHttpFile) {
		LogInfoIn("����ʧ��");
		return false;
	}

	LogInfoIn("���ӳɹ�����ȡ��...");

	while (myHttpFile->ReadString(myData)) {
		vecStringData.push_back(myData.GetBuffer(0));
		//		vecStringData.push_back(Big2GB(myData.GetBuffer(0)));
	}

	LogInfoIn("��ȡ���");

	myHttpFile->Close();
	mySession.Close();
	return true;
}
