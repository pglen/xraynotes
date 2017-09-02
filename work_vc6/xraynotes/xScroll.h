
/* =====[ xScroll.h ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the xScroll.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  11/26/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#if !defined(AFX_XSCROLL_H__6B6FAEC4_E371_4116_8416_FE49BA415144__INCLUDED_)
#define AFX_XSCROLL_H__6B6FAEC4_E371_4116_8416_FE49BA415144__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// xScroll.h : header file
//

#include "GrayDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CxScroll dialog

class CxScroll : public CGrayDlg

{
// Construction
public:

	CPoint	ncold;

	int		push, sizing;
	
	CxScroll(CWnd* pParent = NULL);   // standard constructor

	CWnd	*old_focus;
	void	*parent;

// Dialog Data
	//{{AFX_DATA(CxScroll)
	enum { IDD = IDD_DIALOG5 };
	CScrollBar	m_scroll;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CxScroll)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CxScroll)
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) ;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XSCROLL_H__6B6FAEC4_E371_4116_8416_FE49BA415144__INCLUDED_)
