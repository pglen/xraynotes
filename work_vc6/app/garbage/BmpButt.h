#if !defined(AFX_BMPBUTT_H__546523E6_3C92_4855_A0D9_85C28A85FE84__INCLUDED_)
#define AFX_BMPBUTT_H__546523E6_3C92_4855_A0D9_85C28A85FE84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BmpButt.h : header file
//

#include "popup.h"

/////////////////////////////////////////////////////////////////////////////
// CBmpButt window

class CBmpButt : public CWnd
{
// Construction
public:

	int		gotbmp, bitmap;
	int		firstpaint;
	int		buttdown, id;
	int		capped, fired;
		
	CString text, tooltip;

	CBmpButt();

	CPoint	mouse, capmouse;
	CPopup	tip;

	
protected:

	CBitmap m_bitmap;				// Splash Bitmap

	int		m_nWidth, m_nHeight;	// Splash Bitmap's width & height
	void	FirstPaint();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBmpButt)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	int disabled;
	virtual ~CBmpButt();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBmpButt)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BMPBUTT_H__546523E6_3C92_4855_A0D9_85C28A85FE84__INCLUDED_)
