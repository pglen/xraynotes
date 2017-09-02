#if !defined(AFX_POPUP_H__6CDEC48B_CA86_4255_9FAF_568533840B37__INCLUDED_)
#define AFX_POPUP_H__6CDEC48B_CA86_4255_9FAF_568533840B37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Popup.h : header file
//
//include "label1.h"


/////////////////////////////////////////////////////////////////////////////
// CPopup dialog

class CPopup : public CEdit

{

// Construction
public:

	void	Hide();
	void	Show();
	void	Move(CPoint screen);
	void	SetText(const char *str);
	void	Create();
	void	Create(CWnd *pp);
	
	void	Show(const char *str, CPoint screen);

	CPopup();   // standard constructor

	int			padding;
	COLORREF	bgcolor;

protected:

	CWnd	*parent;
	int		created, first;
	void	PopAutoSize();

	CString		str;
	CFont		m_font;
	LOGFONT		m_lf;

// Dialog Data
	//{{AFX_DATA(CPopup)
	//enum { IDD = IDD_TIP_DLG };
	//CLabel	m_str;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPopup)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPopup)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	//afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POPUP_H__6CDEC48B_CA86_4255_9FAF_568533840B37__INCLUDED_)
