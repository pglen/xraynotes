#if !defined(AFX_TESTDLG_H__4BB25914_0607_4615_B1B6_6DA57F72DB9B__INCLUDED_)
#define AFX_TESTDLG_H__4BB25914_0607_4615_B1B6_6DA57F72DB9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TestDlg.h : header file
//

#include "Triangle.h"
#include "PGedit.h"
#include "piclist.h"
#include "GrayDlg.h"  


/////////////////////////////////////////////////////////////////////////////
// CTestDlg dialog

class CTestDlg : public CGrayDlg
{
// Construction
public:
	CTestDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTestDlg)
	enum { IDD = IDD_DIALOG3 };
	CListBox	m_list;
	CPicList	m_tlist;
	CPGedit	m_ed;
	CLabel	m_l8;
	CLabel	m_l7;
	CLabel	m_l5;
	CLabel	m_l4;
	CLabel	m_l3;
	CLabel	m_l2;
	CLabel	m_l1;
	CTriangle		m_tri;
	CxTargetButton	m_ok;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTestDlg)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	virtual BOOL OnInitDialog();
	afx_msg void OnButton1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTDLG_H__4BB25914_0607_4615_B1B6_6DA57F72DB9B__INCLUDED_)
