// uniqDlg.h : header file
//

#if !defined(AFX_UNIQDLG_H__8A9FD63C_9C62_4EFE_A0EA_10130BF74DD1__INCLUDED_)
#define AFX_UNIQDLG_H__8A9FD63C_9C62_4EFE_A0EA_10130BF74DD1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UniqID.h"


/////////////////////////////////////////////////////////////////////////////
// CUniqDlg dialog

class CUniqDlg : public CDialog
{
// Construction
public:
	CPtrArray arr;

	void OnCancel();
	void OnOK();

	void CUniqDlg::Progress(CUniqID *id, int len);

	int stop;

	CUniqDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CUniqDlg)
	enum { IDD = IDD_UNIQ_DIALOG };
	CString	m_str;
	CString	m_str2;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUniqDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CUniqDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UNIQDLG_H__8A9FD63C_9C62_4EFE_A0EA_10130BF74DD1__INCLUDED_)
