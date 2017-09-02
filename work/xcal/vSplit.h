#if !defined(AFX_VSPLIT_H__B6BFC093_81AD_4888_8821_414FB7A8F080__INCLUDED_)
#define AFX_VSPLIT_H__B6BFC093_81AD_4888_8821_414FB7A8F080__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// vSplit.h : header file
//

#include "xwnd.h"

/////////////////////////////////////////////////////////////////////////////
// CvSplit window

class CvSplit : public CxWnd
{

// Construction
public:

	int in_border;
	RECT rc2;
	int firstpaint;
	int	split;				// Percent point of split (50% = middle)

	CvSplit();
	CWnd *wnd1, *wnd2;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CvSplit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CvSplit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CvSplit)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VSPLIT_H__B6BFC093_81AD_4888_8821_414FB7A8F080__INCLUDED_)
