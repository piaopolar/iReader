// iReaderDlg.h : 头文件
#pragma once
#include "afxwin.h"

#include "BookSkyMgr.h"

// ============================================================================
//    CiReaderDlg 对话框
// ============================================================================
class CiReaderDlg : public CDialog
{
public:
	CBookSkyMgr::BOOK_SET_INFO m_infoBookSet;

	int m_nSearchPageIndex;
	int m_nSiteIndex;
	int m_nBookIndex;
	int m_nChapterIndex;
public:
	enum { MODE_BOOKS, MODE_SITES, MODE_CHAPTERS, MODE_TEXT, };
private:
	int GetMode(void);
	void OutputResult(const char *pszFormat, ...);
	void ShowResult(void);
	void ShowResult_BookSet(void);
	void ShowResult_SiteSet(void);
	void ShowResult_ChapterSet(void);
	void ShowResult_Text(void);

// ----------------------------------------------------------------------------
//    构造
// ----------------------------------------------------------------------------
public:
	CiReaderDlg(CWnd *pParent = NULL);					// 标准构造函数

	// 对话框数据
	enum { IDD = IDD_IREADER_DIALOG };
protected:
	virtual void DoDataExchange(CDataExchange *pDX);	// DDX/DDV 支持

// ----------------------------------------------------------------------------
//    实现
// ----------------------------------------------------------------------------
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edtLog;
	CEdit m_edtKeyWord;
	afx_msg void OnBnClickedBtnSearch();
	CEdit m_edtIndex;
	afx_msg void OnBnClickedBtnShow();
	CEdit m_edtResult;
	afx_msg void OnBnClickedBtnClear();
};
