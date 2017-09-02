#if !defined(AFX_XSCROLLBAR_H__6365DB1A_90AE_423F_8215_6CD21F36A803__INCLUDED_)
#define AFX_XSCROLLBAR_H__6365DB1A_90AE_423F_8215_6CD21F36A803__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// xScrollBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CxScrollBar window

class CxScrollBar : public CScrollBar
{
// Construction
public:
	CxScrollBar();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CxScrollBar)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CxScrollBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CxScrollBar)
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XSCROLLBAR_H__6365DB1A_90AE_423F_8215_6CD21F36A803__INCLUDED_)
