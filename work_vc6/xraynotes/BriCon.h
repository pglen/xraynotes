#if !defined(AFX_CBriCon_H__825A2088_D017_4E6C_8E93_248AA398F9A6__INCLUDED_)
#define AFX_CBriCon_H__825A2088_D017_4E6C_8E93_248AA398F9A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CBriCon.h : header file
//

#include "xTargetButton.h"
#include "TransStatic.h"
#include "TransSlider.h"
#include "GrayDlg.h"

#define	RUN_BRIGHT	0
#define	RUN_CONT	1
#define	RUN_COLOR	2
#define	RUN_SHARP	3

/////////////////////////////////////////////////////////////////////////////
// CBriCon dialog

class CBriCon : public CGrayDlg

{
// Construction
public:
	void UpdateCursor();
	void Show();

	~CBriCon();
	CBriCon(CWnd* pParent = NULL);   // standard constructor

	int		org_changed;
	int		UpdateParam();
	int		bri_level;
	int		old_pos;
	int		run_as;
	int		old_pos1, old_pos2; 

	void	RunWorkerFunc(int comm);

	void	*parent;

	HCURSOR m_busy;
	
// Dialog Data
	//{{AFX_DATA(CBriCon)
	enum { IDD = IDD_BRIGHT };
	CxTargetButton	m_ok;
	CxTargetButton	m_cancel;
	CxTargetButton	m_b1;
	CxTargetButton	m_b5;
	CxTargetButton	m_b4;
	CxTargetButton	m_b3;
	CxTargetButton	m_b2;
	
	CxTargetButton	m_b6;
	CxTargetButton	m_b7;
	CxTargetButton	m_b8;
	CxTargetButton	m_b9;

	CTransSlider	m_slide1;
	CTransSlider	m_slide2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBriCon)
	public:
	virtual int DoModal();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBriCon)
	virtual BOOL OnInitDialog();
	afx_msg void OnReleasedcaptureSlider1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	virtual void OnCancel();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	virtual void OnOK();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnReleasedcaptureSlider2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButton8();
	afx_msg void OnButton7();
	afx_msg void OnButton9();
	afx_msg void OnButton6();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CBriCon_H__825A2088_D017_4E6C_8E93_248AA398F9A6__INCLUDED_)
