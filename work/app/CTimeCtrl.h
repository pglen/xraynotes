#if !defined(AFX_CALCTRL_H__D098AE53_82C5_4B65_93D9_AC122DD6FC3__INCLUDED_)
#define AFX_CALCTRL_H__D098AE53_82C5_4B65_93D9_AC122DD6FC3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CalCtrl.h : header file

#include "lcd7.h"
//

//define  WM_CALCHANGE		(WM_USER + 1)
//define  WM_CALCDBLCLICK	(WM_USER + 2)

/////////////////////////////////////////////////////////////////////////////
// CTimeCtrl window

class CTimeCtrl : public CWnd
{
// Construction
public:

	CTimeCtrl();
	
	Clcd7	hour1, hour2;
	Clcd7	minute1, minute2;

	COLORREF bg, fg;

	virtual BOOL PreTranslateMessage(MSG* pMsg);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTimeCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	
	void Disable();
	void SetToday();
	
	void GetTime(COleDateTime &od);
	void SetTime(COleDateTime &nd);

	virtual ~CTimeCtrl();

protected:
	CStatic	dot1, dot2;

	CFont font;

	int pad, head, nav;
	int inited;
	int ldrag, rdrag;

	CPoint oldpoint;


	void InitialPaint();
	int	 HitTest(CPoint point);

	// Generated message map functions
protected:
	//{{AFX_MSG(CTimeCtrl)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALCTRL_H__D098AE53_82C5_4B65_93D9_AC122DD6FC3__INCLUDED_)
