#if !defined(AFX_CHEVRON_H__682F74CD_6BD1_4464_BF13_F15D274D781D__INCLUDED_)
#define AFX_CHEVRON_H__682F74CD_6BD1_4464_BF13_F15D274D781D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Chevron.h : header file
//

#include	"Flyout.h"
#include	"Label.h"
#include	"PersistDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CChevron dialog

class CChevron : public PersistDlg
{

public:
	void CloseFlyOut();
	void MouseHover();
	void MouseLeave();
	void MouseEnter();
	void InitVars();

	void Move(int want_x, int want_y);
	void OnCancel();
	void OnOK();
	void OpenFlyOut();
	void SetTransparency(int tran);
	void Show();

	CPopup	tip;
	CPoint	mouse;
	CPoint	capmouse;

	CChevron(CWnd* pParent = NULL);   // standard constructor
	CChevron::CChevron(UINT nIDTemplate, CWnd* pParent = NULL);
	
	void CChevron::SetDark(int flag);
	
	CString tiptext;
	// Dialog Data
	//{{AFX_DATA(CChevron)
	enum { IDD = IDD_DIALOG1 };
	CLabel	m_prompt;
	//}}AFX_DATA

	int		drag, drag_x, drag_y;
	int		push;
	int		dark, entered, kick;
	int		darkval, lightval;		
	int		capped, fired;

	COLORREF	bgcolor;

	CFlyout *fo;
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChevron)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CChevron)
	virtual BOOL OnInitDialog();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHEVRON_H__682F74CD_6BD1_4464_BF13_F15D274D781D__INCLUDED_)
