#if !defined(AFX_CURVEDLG_H__B74812E0_A63F_405E_92AC_A2053D37D62C__INCLUDED_)
#define AFX_CURVEDLG_H__B74812E0_A63F_405E_92AC_A2053D37D62C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CurveDlg.h : header file
//

#include "GrayDlg.h"
#include "triangle.h"
#include "Curve.h"
#include "xTargetButton.h"

/////////////////////////////////////////////////////////////////////////////
// CCurveDlg dialog

class CCurveDlg : public CGrayDlg
{

// Construction
public:

	CCurveDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCurveDlg)
	enum { IDD = IDD_DIALOG23 };
	CxTargetButton	m_b3;
	CxTargetButton	m_ok;
	CxTargetButton	m_can;
	CxTargetButton	m_b2;
	CxTargetButton	m_b1;
	CTriangle	m_tri2;
	CCurve		m_cur;
	CTriangle	m_tri;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCurveDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCurveDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnButton4();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CURVEDLG_H__B74812E0_A63F_405E_92AC_A2053D37D62C__INCLUDED_)
