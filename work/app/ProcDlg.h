#if !defined(AFX_PROCDLG_H__95CF1C15_F2C9_4A92_879F_E52B3D90323B__INCLUDED_)
#define AFX_PROCDLG_H__95CF1C15_F2C9_4A92_879F_E52B3D90323B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProcDlg.h : header file
//

#include "GrayDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CProcDlg dialog

class CProcDlg : public CGrayDlg
{

public:
	void UpdateCursor();

	int		changed;
	int		org_changed;

	void	RunWorkerFunc(int comm);
	void	Show();

	void	*parent;

	void MyPaint();
	//CxImage org;

	CProcDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProcDlg)
	enum { IDD = IDD_DIALOG10 };
	CxTargetButton m_OK;
	CxTargetButton m_Cancel;
	CxTargetButton m_Soften;
	CxTargetButton m_Sharpen;
	CxTargetButton m_Reset;
	CxTargetButton m_Erode;
	CxTargetButton m_Emboss;
	CxTargetButton m_Log;
	CxTargetButton m_Root;
	CxTargetButton m_Reverse;
	CxTargetButton m_Punch;
	CxTargetButton m_Equalize;
	CxTargetButton m_Normalize;
	CxTargetButton m_Stretch;
	CxTargetButton m_Pinch;
	CxTargetButton m_GrayScale;
	CxTargetButton m_Dilate;
	CxTargetButton m_Contour;
	CxTargetButton m_Jitter;
	CxTargetButton m_GrayScale2;
	CxTargetButton m_Edge;
	CxTargetButton m_Gamma;
//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProcDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProcDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButton2();
	afx_msg void OnButton1();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	afx_msg void OnButton8();
	afx_msg void OnButton6();
	afx_msg void OnButton7();
	afx_msg void OnButton10();
	afx_msg void OnButton11();
	afx_msg void OnButton12();
	afx_msg void OnButton13();
	afx_msg void OnButton14();
	afx_msg void OnButton3();
	virtual void OnCancel();
	afx_msg void OnButton15();
	afx_msg void OnButton16();
	afx_msg void OnButton17();
	afx_msg void OnButton18();
	virtual void OnOK();
	afx_msg void OnButton9();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnButton21();
	afx_msg void OnButton19();
	afx_msg void OnButton20();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROCDLG_H__95CF1C15_F2C9_4A92_879F_E52B3D90323B__INCLUDED_)
