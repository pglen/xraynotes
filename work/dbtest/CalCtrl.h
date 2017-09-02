#if !defined(AFX_CALCTRL_H__D098AE53_82C5_4B65_93D9_AC122DD6FC3E__INCLUDED_)
#define AFX_CALCTRL_H__D098AE53_82C5_4B65_93D9_AC122DD6FC3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CalCtrl.h : header file
//

#define  WM_CALCHANGE		(WM_USER + 3)
#define  WM_CALCDBLCLICK	(WM_USER + 4)

/////////////////////////////////////////////////////////////////////////////
// CCalCtrl window

class CCalCtrl : public CWnd
{
// Construction
public:
	
	CCalCtrl();
	
	COleDateTime date, today, selday;

	CFont font;
	int pad, head, nav;
	int inited;
	int ldrag, rdrag;
	CPoint oldpoint;

	COLORREF bg, fg;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	void Disable();
	int HitTest(CPoint point);
	void SetToday();
	void GetDate(COleDateTime &od);
	void SetDate(COleDateTime &nd, int nopost = false);
	virtual ~CCalCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCalCtrl)
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

#endif // !defined(AFX_CALCTRL_H__D098AE53_82C5_4B65_93D9_AC122DD6FC3E__INCLUDED_)
