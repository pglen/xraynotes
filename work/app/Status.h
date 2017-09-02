#if !defined(AFX_STATUS_H__C9A8DB29_E7F7_4AC8_8A86_700EA69B0C69__INCLUDED_)
#define AFX_STATUS_H__C9A8DB29_E7F7_4AC8_8A86_700EA69B0C69__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Status.h : header file
//

#include "label.h"
#include "transprog.h"

/////////////////////////////////////////////////////////////////////////////
// CStatus dialog

class CStatus : public CDialog

{
// Construction
public:
	void CenterStatus(CWnd *wnd);
	void	Show(const char *str, int tme = 2000);
	
	CSize textsize;

	CStatus(CWnd* pParent = NULL);   // standard constructor

	void	Show();
	void	Hide();
	//int		created;
	int		showprog;
	//int		pos;
	void	*parent;
	int		timer;

	LOGFONT		m_lf;
	CFont		m_font;
	
	HBITMAP		m_hBitmap;
    DWORD		m_dwWidth;			// Width of bitmap
	DWORD		m_dwHeight;			// Height of bitmap

// Dialog Data
	//{{AFX_DATA(CStatus)
	enum { IDD = IDD_DIALOG9 };
	CTransProg	m_prog;
	CLabel	m_text2;
	CLabel	m_text;
	//}}AFX_DATA

	int		want_x, want_y;
	CString text;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStatus)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStatus)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATUS_H__C9A8DB29_E7F7_4AC8_8A86_700EA69B0C69__INCLUDED_)
