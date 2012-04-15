// iReaderDlg.cpp : ʵ���ļ�
#include "stdafx.h"

#include "../BaseCode/BaseFunc.h"

#include "iReader.h"
#include "iReaderDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ============================================================================
//    ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
// ============================================================================
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// �Ի�������
	enum { IDD = IDD_ABOUTBOX };
protected:
	virtual void DoDataExchange(CDataExchange *pDX);	// DDX/DDV ֧��

// ----------------------------------------------------------------------------
//    ʵ��
// ----------------------------------------------------------------------------
protected:
	DECLARE_MESSAGE_MAP()
};

// ============================================================================
// ==============================================================================

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

// ============================================================================
// ==============================================================================
void CAboutDlg::DoDataExchange(CDataExchange *pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// ============================================================================
//    CiReaderDlg �Ի���
// ============================================================================
CiReaderDlg::CiReaderDlg(CWnd *pParent /* NULL */ ) : CDialog(CiReaderDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nBookIndex = 0;
	m_nSiteIndex = 0;
	m_nChapterIndex = 0;
	m_nSearchPageIndex = 0;
}

// ============================================================================
// ==============================================================================
void CiReaderDlg::DoDataExchange(CDataExchange *pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDT_LOGINFO, m_edtLog);
	DDX_Control(pDX, IDC_EDT_KEYWORD, m_edtKeyWord);
	DDX_Control(pDX, IDC_EDT_INDEX, m_edtIndex);
	DDX_Control(pDX, IDC_EDT_RESULT, m_edtResult);
}

BEGIN_MESSAGE_MAP(CiReaderDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_SEARCH, &CiReaderDlg::OnBnClickedBtnSearch)
	ON_BN_CLICKED(IDC_BTN_SHOW, &CiReaderDlg::OnBnClickedBtnShow)
	ON_BN_CLICKED(IDC_BTN_CLEAR, &CiReaderDlg::OnBnClickedBtnClear)
END_MESSAGE_MAP()

// ============================================================================
//    CiReaderDlg ��Ϣ�������
// ============================================================================
BOOL CiReaderDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С� IDM_ABOUTBOX
	// ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	CMenu *pSysMenu = GetSystemMenu(FALSE);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	if (pSysMenu != NULL) {

		//~~~~~~~~~~~~~~~~~
		CString strAboutMenu;
		//~~~~~~~~~~~~~~~~~

		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty()) {
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	// ִ�д˲�?
	SetIcon(m_hIcon, TRUE);		// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);	// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	SetLogEdit(&m_edtLog);
	((CButton *)GetDlgItem(IDC_RADIO_BOOK))->SetCheck(TRUE);	// ѡ��
	((CButton *)GetDlgItem(IDC_RADIO_BOOKS))->SetCheck(TRUE);	// ѡ��

	return TRUE;	// ���ǽ��������õ��ؼ������򷵻� TRUE
}

// ============================================================================
// ==============================================================================
void CiReaderDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX) {

		//~~~~~~~~~~~~~~~
		CAboutDlg dlgAbout;
		//~~~~~~~~~~~~~~~

		dlgAbout.DoModal();
	} else {
		CDialog::OnSysCommand(nID, lParam);
	}
}

// ============================================================================
//    �����Ի��������С����ť������Ҫ����Ĵ���
//    �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó��� �⽫�ɿ���Զ���ɡ�
// ============================================================================
void CiReaderDlg::OnPaint()
{
	if (IsIconic()) {

		//~~~~~~~~~~~~~~
		CPaintDC dc(this);	// ���ڻ��Ƶ��豸������
		//~~~~~~~~~~~~~~

		SendMessage(WM_ICONERASEBKGND,
					reinterpret_cast<WPARAM> (dc.GetSafeHdc()), 0);

		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

		GetClientRect(&rect);

		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	} else {
		CDialog::OnPaint();
	}
}

// ============================================================================
//    ���û��϶���С������ʱϵͳ���ô˺���ȡ�ù�� ��ʾ��
// ============================================================================
HCURSOR CiReaderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// ============================================================================
// ==============================================================================
void CiReaderDlg::OnBnClickedBtnSearch()
{
	//~~~~~~~~~~~~~~~~~~~~~~~
	char szKeyWord[MAX_STRING];
	//~~~~~~~~~~~~~~~~~~~~~~~

	m_edtKeyWord.GetWindowText(szKeyWord, sizeof(szKeyWord));
	m_infoBookSet = CBookSkyMgr::GetInstance().GetBookInfo(szKeyWord,
														   ((CButton *)GetDlgItem(IDC_RADIO_AUTHOR))->GetCheck()
																			   ? CBookSkyMgr::KEYWORD_TYPE_AUTHOR : CBookSkyMgr::
																			   KEYWORD_TYPE_BOOKNAME);
}

// ============================================================================
// ==============================================================================
void CiReaderDlg::OnBnClickedBtnShow()
{
	//~~~~~~~~~~~~~~~~~~~~~
	char szIndex[MAX_STRING];
	//~~~~~~~~~~~~~~~~~~~~~

	m_edtIndex.GetWindowText(szIndex, sizeof(szIndex));

	//~~~~~~~~~~~
	int nIndex = 0;
	//~~~~~~~~~~~

	if (EOF != sscanf(szIndex, "%d", &nIndex)) {
		switch (this->GetMode()) {
		case MODE_BOOKS:	m_nSearchPageIndex = nIndex; break;
		case MODE_SITES:	m_nBookIndex = nIndex; break;
		case MODE_CHAPTERS: m_nSiteIndex = nIndex; break;
		case MODE_TEXT:		m_nChapterIndex = nIndex; break;
		}
	}

	this->ShowResult();
}

// ============================================================================
// ==============================================================================
int CiReaderDlg::GetMode(void)
{
	//~~~~~~~~~~~~~~~~~~~
	int nMode = MODE_BOOKS;
	//~~~~~~~~~~~~~~~~~~~

	if (((CButton *)GetDlgItem(IDC_RADIO_SITE))->GetCheck()) {
		nMode = MODE_SITES;
	} else if (((CButton *)GetDlgItem(IDC_RADIO_CHAPTER))->GetCheck()) {
		nMode = MODE_CHAPTERS;
	} else if (((CButton *)GetDlgItem(IDC_RADIO_TEXT))->GetCheck()) {
		nMode = MODE_TEXT;
	}

	return nMode;
}

// ============================================================================
// ==============================================================================
void CiReaderDlg::ShowResult(void)
{
	m_edtResult.SetWindowText("");

	switch (this->GetMode()) {
	case MODE_BOOKS:	this->ShowResult_BookSet(); break;
	case MODE_SITES:	this->ShowResult_SiteSet(); break;
	case MODE_CHAPTERS: this->ShowResult_ChapterSet(); break;
	case MODE_TEXT:		this->ShowResult_Text(); break;
	default:			break;
	}
}

// ============================================================================
// ==============================================================================
void CiReaderDlg::ShowResult_BookSet(void)
{
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	std::string strBookSet;
	std::vector<CBookSkyMgr::BOOK_INFO>::const_iterator it = m_infoBookSet.
		m_vecBookInfo.begin();
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	for (; it != m_infoBookSet.m_vecBookInfo.end(); ++it) {
		strBookSet += it->GetBasicInfoDumpStr();
	}

	this->OutputResult(strBookSet.c_str());
}

// ============================================================================
// ==============================================================================
void CiReaderDlg::ShowResult_SiteSet(void)
{
	if (m_nBookIndex < 0 || m_nBookIndex >= m_infoBookSet.m_vecBookInfo.size()) {
		LogInfoIn("invalid Book Index");
		return;
	}

	CBookSkyMgr::BOOK_INFO & rInfoBook = m_infoBookSet.m_vecBookInfo.at(m_nBookIndex);
	this->OutputResult(rInfoBook.GetSiteInfoDumpStr().c_str());
}

// ============================================================================
// ==============================================================================
void CiReaderDlg::ShowResult_ChapterSet(void)
{
	if (m_nBookIndex < 0 || m_nBookIndex >= m_infoBookSet.m_vecBookInfo.size()) {
		LogInfoIn("invalid Book Index");
		return;
	}

	CBookSkyMgr::BOOK_INFO & rInfoBook = m_infoBookSet.m_vecBookInfo.at(m_nBookIndex);
	this->OutputResult(rInfoBook.GetChapterDumpStr(m_nSiteIndex).c_str());
}

// ============================================================================
// ==============================================================================
void CiReaderDlg::ShowResult_Text(void)
{
	if (m_nBookIndex < 0 || m_nBookIndex >= m_infoBookSet.m_vecBookInfo.size()) {
		LogInfoIn("invalid Book Index");
		return;
	}

	CBookSkyMgr::BOOK_INFO & rInfoBook = m_infoBookSet.m_vecBookInfo.at(m_nBookIndex);
	this->OutputResult(rInfoBook.GetTextDumpStr(m_nSiteIndex, m_nChapterIndex).c_str());
}

// ============================================================================
// ==============================================================================
void CiReaderDlg::OutputResult(const char *pszFormat, ...)
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
	cstrData = strLine.c_str();

	m_edtResult.SetWindowText(cstrData.GetBuffer(0));
	m_edtResult.UpdateWindow();
	m_edtResult.LineScroll(m_edtResult.GetLineCount());
}

// ============================================================================
// ==============================================================================
void CiReaderDlg::OnBnClickedBtnClear()
{
	m_edtLog.SetWindowText("");
}
