#if !defined(AFX_XWND_H__F1F19D90_466B_4D4D_AF9D_7A4CC28C0A8E__INCLUDED_)
#define AFX_XWND_H__F1F19D90_466B_4D4D_AF9D_7A4CC28C0A8E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// xWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CxWnd window

class CxWnd : public CWnd
{
// Construction
public:
	int firstpaint;

	CxWnd();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CxWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CxWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CxWnd)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XWND_H__F1F19D90_466B_4D4D_AF9D_7A4CC28C0A8E__INCLUDED_)
