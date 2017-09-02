#if !defined(AFX_MAGDLG_H__8D067BA6_3FC7_4AB2_AD61_681CE9FEDFB6__INCLUDED_)
#define AFX_MAGDLG_H__8D067BA6_3FC7_4AB2_AD61_681CE9FEDFB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// MagDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMagDlg dialog

class CMagDlg : public CGrayDlg

{
// Construction
public:
	
	void UpdateDoc();
	CMagDlg(CWnd* pParent = NULL);   // standard constructor

	int		push;
	int		dark;
	int		drag, drag_x, drag_y;
	void	*parent;

	int		old;

// Dialog Data
	//{{AFX_DATA(CMagDlg)
	enum { IDD = IDD_DIALOG4 };
	CLabel	m_mag;
	CSliderCtrl	m_slide;
	CxTargetButton m_ToWidth ;
	CxTargetButton m_ToHeight;
	CxTargetButton m_ToFull  ;  
	CxTargetButton m_Lt	  ;  
	CxTargetButton m_Gt	  ;  
	CxTargetButton m_AutoL	  ;
	CxTargetButton m_AutoR	  ;
	CxTargetButton m_Fine	  ;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMagDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMagDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnSysChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnButton5();
	afx_msg void OnButton4();
	afx_msg void OnButton6();
	afx_msg void OnButton7();
	afx_msg void OnReleasedcaptureSlider1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnButton9();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAGDLG_H__8D067BA6_3FC7_4AB2_AD61_681CE9FEDFB6__INCLUDED_)
