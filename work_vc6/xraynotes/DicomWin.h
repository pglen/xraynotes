
/* =====[ DicomWin.h ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the DicomWin.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  11/13/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#if !defined(AFX_DICOMWIN_H__AE24A0D6_6A07_4F6E_B932_8B903019C9A1__INCLUDED_)
#define AFX_DICOMWIN_H__AE24A0D6_6A07_4F6E_B932_8B903019C9A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// DicomWin.h : header file
//

#include "label.h"
#include "transslider.h"
#include "GrayDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CDicomWin dialog

class CDicomWin : public CGrayDlg

{
// Construction
public:
	
	void	UpdateSlider(int curr);
	void	Play(int rev);
	void	SetNumViews(int num);
	void	Show();

	void	DoCristal3D(int rev);
	//void	Do3Dp(int rev = false);

	void	Do3Dpix(int xx, int yy, int rev, CxImg *img);
	//void	Do3Dpix2(int xx, int yy, int rev, CxImg *img);

	void	SetCurrFrame(int pos, int update = true);
	void	UpdateCursor();
	void	Hide();
	void	UpdateNum();

	double factor, diff;
	CPtrArray docs, imgs;

	RGBQUAD bg; 
	CxImg	mask;

	int		views, 	in_play;
	int		stop, delay;
	int		created;
	int		drag, drag_x, drag_y;
	void	*parent;
	int		maxframe, oldcurr;

	CDicomWin(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDicomWin)
	enum { IDD = IDD_DIALOG13 };
	CxTargetButton	m_b20;
	CxTargetButton	m_b13;
	CxTargetButton	m_b22;
	CxTargetButton	m_b5;
	CxTargetButton	m_b21;
	CxTargetButton	m_b23;
	CxTargetButton	m_b24;
	CxTargetButton	m_b15;
	CxTargetButton	m_b14;
	CxTargetButton	m_b12;
	CxTargetButton	m_b11;
	CxTargetButton	m_b16;
	CxTargetButton	m_b26;
	CxTargetButton	m_b10;
	CxTargetButton	m_b25;
	CxTargetButton	m_b9;
	CxTargetButton	m_b7;
	CxTargetButton	m_b35;
	CxTargetButton	m_b6;
	CxTargetButton	m_play;
	CxTargetButton	m_stop;
	CLabel	m_lab3;
	CLabel			m_lab5;
	CTransSlider	m_frame;
	CLabel	m_lab4;
	CTransSlider	m_curr;
	CxTargetButton	m_b2;
	CxTargetButton	m_b1;
	CLabel	m_lab2;
	CLabel	m_lab1;
	CxTargetButton	m_ok;
	CxTargetButton	m_can;
	CTransSlider	m_width;
	CTransSlider	m_lev;
	//}}AFX_DATA

	//CSliderCtrl	m_width;
	//CSliderCtrl	m_lev;
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDicomWin)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDicomWin)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnDestroy();
	virtual void OnOK();
	afx_msg void OnButton6();
	afx_msg void OnButton7();
	virtual void OnCancel();
	afx_msg void OnButton9();
	afx_msg void OnButton10();
	afx_msg void OnButton11();
	afx_msg void OnButton16();
	afx_msg void OnButton12();
	afx_msg void OnButton13();
	afx_msg void OnButton14();
	afx_msg void OnButton15();
	afx_msg void OnButton20();
	afx_msg void OnButton5();
	afx_msg void OnButton23();
	afx_msg void OnButton24();
	afx_msg void OnButton21();
	afx_msg void OnButton22();
	afx_msg void OnDicomMovezordertowardsthefirst();
	afx_msg void OnButton25();
	afx_msg void OnButton26();
	afx_msg void OnButton35();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DICOMWIN_H__AE24A0D6_6A07_4F6E_B932_8B903019C9A1__INCLUDED_)
