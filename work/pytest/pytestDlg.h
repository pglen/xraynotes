// pytestDlg.h : header file
//

#if !defined(AFX_PYTESTDLG_H__63582B64_BB28_451A_8ADE_6FCDD1A4EBE2__INCLUDED_)
#define AFX_PYTESTDLG_H__63582B64_BB28_451A_8ADE_6FCDD1A4EBE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPytestDlg dialog

class CPytestDlg : public CDialog
{
// Construction
public:

	HINSTANCE phand;

	CPytestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CPytestDlg)
	enum { IDD = IDD_PYTEST_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPytestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CPytestDlg)
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

#endif // !defined(AFX_PYTESTDLG_H__63582B64_BB28_451A_8ADE_6FCDD1A4EBE2__INCLUDED_)
