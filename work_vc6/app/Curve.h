#if !defined(AFX_CURVE_H__896B4C56_532C_458C_851E_2BDC0D5994C0__INCLUDED_)
#define AFX_CURVE_H__896B4C56_532C_458C_851E_2BDC0D5994C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Curve.h : header file
//

#include "BaseWnd.h"

#define DISTCNT 20
#define AVGSPAN 20

/////////////////////////////////////////////////////////////////////////////
// CCurve window

class CCurve : public CBaseWnd

{
// Construction
public:
	
	CCurve();

	long		hg[256], hg2[256];
	int			maxhist, curbusy;

	int		low, high, lowlim, highlim;

	int curdrag, match, inmm, count;
	POINT *arr, *xarr, *yarr ;

	HCURSOR grab;

	int mousedist;
	void CCurve::OnInitControl();
	void CCurve::GetTipText(CString &str);

	CXrayDoc *olddoc;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCurve)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	void ResetImg();
	void SetHist();
	void GetHist();
	void SmoothCurve(POINT *zarr = NULL, int inval = true, int span = 0, int low = -1, int high = -1);
	void ResetCurve();
	virtual ~CCurve();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCurve)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CURVE_H__896B4C56_532C_458C_851E_2BDC0D5994C0__INCLUDED_)
