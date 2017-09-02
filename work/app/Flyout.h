#if !defined(AFX_FLYOUT_H__46AD0C9F_7B64_4FBE_B69B_ECF50F2F22B3__INCLUDED_)
#define AFX_FLYOUT_H__46AD0C9F_7B64_4FBE_B69B_ECF50F2F22B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Flyout.h : header file
//

//include "BCMenu.h"        // BCMenu class declaration

#include "flag.h"
#include "popup.h"


/////////////////////////////////////////////////////////////////////////////
// CFlyout dialog

class CFlyout : public CDialog

{

typedef struct _flyact

{
	void   (CShadesFunc::*func)(void*);
	char	*str;

} flyact;

// Construction
public:
	void CFlyout::MouseHover();
	void CFlyout::MouseLeave();
	void CFlyout::MouseEnter();

	void Hide();
	void Show();
	CFlyout(CWnd* pParent = NULL);   // standard constructor

	CShadesFunc *backlink;

	int		bitmap;
// Dialog Data
	//{{AFX_DATA(CFlyout)
	enum { IDD = IDD_DIALOG2 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	RECT	lastrc;

	int		xdrag, ydrag;
	int		want_x, want_y;
	int		dark;
	int		prev_button;
	int		capped, fired;
	int		currbutt, maxbutton, buttonw;

	CFlag	entered;

	CBitmap m_bitmap;				//  Flyout Bitmap
	int		m_nWidth, m_nHeight;	//  Bitmap's width & height
	RECT	closerect;
	void	*parent;

	flyact	arr_l[12];
	flyact	arr_r[12];

	CPopup	tip;
	
	CPoint	mouse;
	CPoint	capmouse;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFlyout)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFlyout)
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLYOUT_H__46AD0C9F_7B64_4FBE_B69B_ECF50F2F22B3__INCLUDED_)
