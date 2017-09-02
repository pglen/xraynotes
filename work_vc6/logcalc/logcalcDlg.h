// logcalcDlg.h : header file
//

#if !defined(AFX_LOGCALCDLG_H__CFFDE07B_51F3_43DE_B815_1846C7C4F9B1__INCLUDED_)
#define AFX_LOGCALCDLG_H__CFFDE07B_51F3_43DE_B815_1846C7C4F9B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CLogcalcDlg dialog

class CLogcalcDlg : public CDialog
{
// Construction
public:
	CLogcalcDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CLogcalcDlg)
	enum { IDD = IDD_LOGCALC_DIALOG };
	CString	m_cnt;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogcalcDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CLogcalcDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGCALCDLG_H__CFFDE07B_51F3_43DE_B815_1846C7C4F9B1__INCLUDED_)
