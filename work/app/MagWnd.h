#if !defined(AFX_MAGWND_H__C08E8027_73A7_497D_B607_0926F3E860AB__INCLUDED_)
#define AFX_MAGWND_H__C08E8027_73A7_497D_B607_0926F3E860AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MagWnd.h : header file
//

#include "TransSlider.h"
#include "graydlg.h"


/////////////////////////////////////////////////////////////////////////////
// CMagWnd dialog

class CMagWnd : public 	CGrayDlg
{
// Construction
public:
	void Norm();
	void Sharpen();
	void ChangeMag(int mm = 2);
	void SimMove(int fine = 1,  int outside = false);

	//int lastcmd;

	// Data:
	WINDOWPOS oldpos;
	POINT	lastmouse, currmouse;
	int		mag, moving, invalid, lastmove;
	int		showtext, fired, scantime;

	CPtrArray	controls;

	CxImg img;

	CMagWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMagWnd)
	enum { IDD = IDD_DIALOG24 };
	CxTargetButton	m_b6;
	CxTargetButton	m_b5;
	CxTargetButton	m_b4;
	CxTargetButton	m_b3;
	CxTargetButton	m_b2;
	CxTargetButton	m_b1;
	CxTargetButton	m_b7;
	CxTargetButton	m_b8;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMagWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMagWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	afx_msg void OnButton6();
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnButton7();
	afx_msg void OnButton8();
	afx_msg void OnButton9();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMagrclickMagnificationfactor2();
	afx_msg void OnMagrclickMagnificationfactor3();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMagrclickMagnificationfactor4();
	afx_msg void OnMagrclickMagnificationfactor6();
	afx_msg void OnMagrclickMagnificationfactor8();
	afx_msg void OnMagrclickNormalizemagview();
	afx_msg void OnMagrclickSharpenmagview();
	afx_msg void OnMagrclickMagnificationfactor1();
	afx_msg void OnMagrclickShowhidemagviewbuttons();
	afx_msg void OnMagrclickHidemagviewwindow();
	afx_msg void OnMagrclickCapturemagview();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAGWND_H__C08E8027_73A7_497D_B607_0926F3E860AB__INCLUDED_)
