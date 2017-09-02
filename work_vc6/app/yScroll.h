#if !defined(AFX_XSCROLL_H__6B6FAEC4_E371_4116_8416_FE49BA415145__INCLUDED_)
#define AFX_XSCROLL_H__6B6FAEC4_E371_4116_8416_FE49BA415145__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// xScroll.h : header file
//

#include "GrayDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CyScroll dialog

class CyScroll : public CGrayDlg

{
// Construction
public:
	void MoveWidth(int want_x, int want_y, int want_w);

	BOOL ExtMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	CyScroll(CWnd* pParent = NULL);   // standard constructor

	CWnd	*old_focus;
	int		push;
	void	*parent;

	//void CyScroll::Create()

// Dialog Data
	//{{AFX_DATA(CyScroll)
	enum { IDD = IDD_DIALOG6 };
	CScrollBar	m_scroll;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CyScroll)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CyScroll)
	virtual BOOL OnInitDialog();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) ;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XSCROLL_H__6B6FAEC4_E371_4116_8416_FE49BA415145__INCLUDED_)
