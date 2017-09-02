#if !defined(AFX_FLYOUT_H__46AD0C9F_7B64_4FBE_B69B_ECF50F2F22B3__INCLUDED_)
#define AFX_FLYOUT_H__46AD0C9F_7B64_4FBE_B69B_ECF50F2F22B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Flyout.h : header file
//

//include "BCMenu.h"        // BCMenu class declaration

#include "mxpad.h"
#include "flag.h"
#include "misc.h"
#include "popup.h"
#include "PersistDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CToolStrip dialog


//class CToolStrip : public CDialog

class CToolStrip : public PersistDlg

{

typedef struct _toolstripact

{
	void   (*func)(void*);
	char	*str;

} toolstripact;

// Construction
public:
	void xCreate();
	void CToolStrip::MouseHover();
	void CToolStrip::MouseLeave();
	void CToolStrip::MouseEnter();

	void Hide();
	void Show();
	
	CToolStrip(CWnd* pParent = NULL);   // standard constructor
	~CToolStrip();

	int		num_buttons;

	HWND 	boddy;

protected:

	int		in_close, old_in_close;

	RECT	rclose, rhead;

	int		bitmap;

// Dialog Data
	//{{AFX_DATA(CToolStrip)
	enum { IDD = IDD_DIALOG2 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	RECT	lastrc;

	int		dragging, prev_button;
	int		xdrag, ydrag, want_x, want_y;
	int		capped, fired;
	int		currbutt, buttonw;
	int		lbuttdown;
	int		xgap, ygap, lheader;

	CFlag	entered;

	CBitmap m_bitmap;				//  Flyout Bitmap
	int		m_nWidth, m_nHeight;	//  Bitmap's width & height
	RECT	closerect;
	void	*parent;

	toolstripact	*arr_l;
	toolstripact	*arr_r;

	CPopup	tip;
	
	CPoint	mouse;
	CPoint	capmouse;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolStrip)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CToolStrip)
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLYOUT_H__46AD0C9F_7B64_4FBE_B69B_ECF50F2F22B3__INCLUDED_)
