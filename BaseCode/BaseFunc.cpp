#include "stdafx.h"

#include "BaseFunc.h"

CEdit *s_pEditLog;

// ============================================================================
// ==============================================================================

void FormatPath(std::string &strPath)
{
	if (strPath.empty()) {
		return;
	}

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	char cLast = strPath.at(strPath.length() - 1);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	switch (cLast) {
	case '\\':
	case '/':
		break;
	default:
		strPath += "/";
		break;
	}
}

// ============================================================================
// ==============================================================================
void TrimRight(char *pszStr)
{
	if (NULL == pszStr) {
		return;
	}

	//~~~~~~~~~~~~~~~~~~~~~~
	int nLen = strlen(pszStr);
	char *pLast;
	//~~~~~~~~~~~~~~~~~~~~~~

	for (pLast = pszStr + nLen - 1; pszStr >= pszStr; --pLast) {

		//~~~~~~~~~~~~~~
		bool bRid = false;
		//~~~~~~~~~~~~~~

		switch (*pLast) {
		case '\n':
		case '\r':
		case '\t':
		case ' ':
			bRid = true;
			break;
		default:
			break;
		}

		if (!bRid) {
			break;
		}
	}

	*(pLast + 1) = 0;
}

// ============================================================================
// ==============================================================================
void LogInfoIn(const char *pszFormat, ...)
{
	//~~~~~~~~~~~~~~~~~~~~
	static CString cstrData;
	std::string strLine;
	va_list args;
	int len;
	char *buffer;
	//~~~~~~~~~~~~~~~~~~~~

	va_start(args, pszFormat);
	len = _vscprintf(pszFormat, args) + 1;	// _vscprintf doesn't count

	// terminating '\0'
	buffer = static_cast<char *>(malloc(len * sizeof(char)));
	vsprintf_s(buffer, len, pszFormat, args);

	strLine = buffer;
	ReplaceStdString(strLine, "\n", "\r\n");
	free(buffer);

	strLine += "\r\n";
	cstrData += strLine.c_str();

	if (NULL == s_pEditLog) {
		return;
	}

	s_pEditLog->SetWindowText(cstrData.GetBuffer(0));
	s_pEditLog->UpdateWindow();
	s_pEditLog->LineScroll(s_pEditLog->GetLineCount());
}

// ============================================================================
// ==============================================================================
void LogFile(const char *pszFormat, ...)
{
	//~~~~~~~~~~~~~~~~~~~~
	static CString cstrData;
	std::string strLine;
	va_list args;
	int len;
	char *buffer;
	//~~~~~~~~~~~~~~~~~~~~

	va_start(args, pszFormat);
	len = _vscprintf(pszFormat, args) + 1;	// _vscprintf doesn't count

	// terminating '\0'
	buffer = static_cast<char *>(malloc(len * sizeof(char)));
	vsprintf_s(buffer, len, pszFormat, args);

	strLine = buffer;
	free(buffer);

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	char szLogFile[_MAX_PATH] = { 0 };
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	GetPrivateProfileString("GlobalSet", "LogFile", "", szLogFile,
							sizeof(szLogFile), CONFIG_INI);

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	FILE *pFile = fopen(szLogFile, "w+");
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	if (NULL == pFile) {
		return;
	}

	fprintf(pFile, "%s\n", strLine.c_str());
	fclose(pFile);
}

// ============================================================================
// ==============================================================================
std::string MyTrim(char sz[])
{
	//~~~~~~~~~~~~~~
	CString cstr = sz;
	//~~~~~~~~~~~~~~

	cstr.Trim();
	strcpy(sz, cstr.GetBuffer(0));
	return sz;
}

// ============================================================================
// ==============================================================================
std::string MyTrim(std::string &str)
{
	//~~~~~~~~~~~~~~~~~~~~~~~
	CString cstr = str.c_str();
	//~~~~~~~~~~~~~~~~~~~~~~~

	cstr.Trim();
	str = cstr.GetBuffer(0);
	return str;
}

// ============================================================================
// ==============================================================================
void SetLogEdit(CEdit *pEditLog)
{
	s_pEditLog = pEditLog;
}

// ============================================================================
// ==============================================================================
std::string UTF8ToAnsi(const std::string &strIn, std::string &strOut)
{
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	WCHAR *strSrc = NULL;
	TCHAR *szRes = NULL;
	int i = MultiByteToWideChar(CP_UTF8, 0, strIn.c_str(), -1, NULL, 0);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	strSrc = new WCHAR[i + 1];
	MultiByteToWideChar(CP_UTF8, 0, strIn.c_str(), -1, strSrc, i);

	i = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL, NULL);

	szRes = new TCHAR[i + 1];
	WideCharToMultiByte(CP_ACP, 0, strSrc, -1, szRes, i, NULL, NULL);

	strOut = szRes;

	delete[] strSrc;
	delete[] szRes;

	return strOut;
}

// ============================================================================
// ==============================================================================
std::string AnsiToUTF8(const std::string &strIn, std::string &strOut)
{
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	WCHAR *strSrc = NULL;
	TCHAR *szRes = NULL;
	int len = MultiByteToWideChar(CP_ACP, 0, (LPCTSTR) strIn.c_str(), -1, NULL,
								  0);
	unsigned short *wszUtf8 = new unsigned short[len + 1];
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	memset(wszUtf8, 0, len * 2 + 2);
	MultiByteToWideChar(CP_ACP, 0, (LPCTSTR) strIn.c_str(), -1, (LPWSTR) wszUtf8,
						len);

	len = WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR) wszUtf8, -1, NULL, 0, NULL,
							  NULL);

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	char *szUtf8 = new char[len + 1];
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	memset(szUtf8, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR) wszUtf8, -1, szUtf8, len, NULL,
						NULL);

	strOut = szUtf8;

	delete[] szUtf8;
	delete[] wszUtf8;

	return strOut;
}

// ============================================================================
// ==============================================================================
void ReplaceStdString(std::string &str,
					  const std::string &strSrc,
					  const std::string &strDest)
{
	//~~~~~~~~~~~~~~~~~~~~~~~
	CString cstr = str.c_str();
	//~~~~~~~~~~~~~~~~~~~~~~~

	cstr.Replace(strSrc.c_str(), strDest.c_str());

	str = cstr.GetBuffer(0);
}
