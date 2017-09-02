#if !defined(AFX_LINEPROP_H__F92A4146_3B9F_4FA6_B5C5_AF344346A89D__INCLUDED_)
#define AFX_LINEPROP_H__F92A4146_3B9F_4FA6_B5C5_AF344346A89D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// lineProp.h : header file
//
#include "layer.h"

/////////////////////////////////////////////////////////////////////////////
// ClineProp dialog

class ClineProp : public CGrayDlg
{
// Construction
public:
	void OnCancel();

	CLaySline *lay;

	void Apply(int offset, int val1 = 0, int val2 = 0);

	ClineProp(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(ClineProp)
	enum { IDD = IDD_DIALOG30 };
	CxTargetButton	m_b5;
	CxTargetButton	m_b4;
	CxTargetButton	m_can;
	CxTargetButton	m_ok;
	CString	m_ed3;
	CString	m_ed5;
	CString	m_ed6;
	CString	m_ed7;
	CString	m_ed11;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ClineProp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ClineProp)
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
	afx_msg void OnCheck1();
	afx_msg void OnCheck4();
	afx_msg void OnCheck5();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LINEPROP_H__F92A4146_3B9F_4FA6_B5C5_AF344346A89D__INCLUDED_)
