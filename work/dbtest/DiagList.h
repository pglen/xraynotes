#if !defined(AFX_DIAGLIST_H__C69ADBE5_4F47_41AC_AA69_0E9D730CCBAB__INCLUDED_)
#define AFX_DIAGLIST_H__C69ADBE5_4F47_41AC_AA69_0E9D730CCBAB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiagList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDiagList dialog

class CDiagList : public CDialog
{
// Construction
public:
	void init_array();
	CDiagList(CWnd* pParent = NULL);   // standard constructor

	fields	farr[60];			// Make sure it will hold all of constructor's additions
	int		farrsize;

	int		UpdateDB();
	void	LoadData(int getid = -1);

	int rows;

	CString tid, dbfile;

// Dialog Data
	//{{AFX_DATA(CDiagList)
	enum { IDD = IDD_DIAG_DIALOG };
	CString	m_diag;
	CString	m_diagd;
	CString	m_diag1;
	CString	m_diag1d;
	CString	m_diag1n;
	CString	m_diag2;
	CString	m_diag2d;
	CString	m_diag2n;
	CString	m_diag3;
	CString	m_diag3d;
	CString	m_diag3n;
	CString	m_diagn;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiagList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDiagList)
	afx_msg void OnDlButton2();
	virtual BOOL OnInitDialog();
	afx_msg void OnDlButton23();
	afx_msg void OnDlButton30();
	afx_msg void OnDlButton24();
	virtual void OnOK();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIAGLIST_H__C69ADBE5_4F47_41AC_AA69_0E9D730CCBAB__INCLUDED_)
