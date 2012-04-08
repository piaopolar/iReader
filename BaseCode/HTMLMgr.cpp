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
	CInternetSession mySession(NULL, 0);	// �����Ự
	CHttpFile *myHttpFile = NULL;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	LogInfoIn("���ӵ�Url %s...", pszUrl);

	//~~~~~~~~~~~~~~~~~~~
	std::string strUrlUTF8;
	//~~~~~~~~~~~~~~~~~~~

	AnsiToUTF8(pszUrl, strUrlUTF8);

	//~~~~~~~~~~~
	// ����ҳ���ݵ�Դ��������༭��
	CString myData;
	//~~~~~~~~~~~

	myHttpFile = (CHttpFile *)mySession.OpenURL(strUrlUTF8.c_str());

	if (NULL == myHttpFile) {
		LogInfoIn("����ʧ��");
		return false;
	}

	LogInfoIn("���ӳɹ�����ȡ��...");

	while (myHttpFile->ReadString(myData)) {

		//~~~~~~~~~~~~~~~~
		std::string strANSI;
		//~~~~~~~~~~~~~~~~

		UTF8ToAnsi(myData.GetBuffer(0), strANSI);
		vecStringData.push_back(strANSI);
	}

	LogInfoIn("��ȡ���");

	myHttpFile->Close();
	mySession.Close();
	return true;
}
