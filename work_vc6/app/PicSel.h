
/* =====[ PicSel.h ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the PicSel.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  12/10/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#if !defined(AFX_PICSEL_H__8D67F2C4_FB89_40F0_80F1_445D474FFAF7__INCLUDED_)
#define AFX_PICSEL_H__8D67F2C4_FB89_40F0_80F1_445D474FFAF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PicSel.h : header file
//

#include "Layer.h"
#include "lines.h"
#include "GrayDlg.h"
#include "transslider.h"

#define PIC_APPLY_NONE		0
#define PIC_APPLY_BG		1
#define PIC_APPLY_FG		2

#define MAXLABELS 500

// This is to create correct colors even if overshoot passed to it
#define RGBLIM(rr,gg,bb) RGB(MIN(rr,255),MIN(gg,255),MIN(bb,255))


/////////////////////////////////////////////////////////////////////////////
// CPicSel dialog

class CPicSel : public CGrayDlg
{
// Construction
public:

	void	UpdateUI(int updscr);
	void	CreatePics();
	void	ShowCol(RECT &rc, int &loop1, int &cnt);
	void	OnCancel();

	int		old_col, old_thick;
	int		left;
	void	LiveUpdate();

	~CPicSel();
	CPicSel(CWnd* pParent = NULL);   // standard constructor

	CLayer	*lay;
	//int		*liveptr;

	int		slider;
	int		m_showalt;
	int		m_lines;
	int		uppd;
	
// Dialog Data
	//{{AFX_DATA(CPicSel)
	enum { IDD = IDD_PICSEL };
	CxTargetButton	m_b1;
	CLabel	m_curr2;
	CTransSlider	m_slider3;
	CTransSlider	m_slider2;
	CTransSlider	m_slider1;
	CLabel	m_curr;
	CxTargetButton	m_ok;
	CxTargetButton	m_cancel;
	CString	m_r;
	CString	m_g;
	CString	m_b;
	//}}AFX_DATA

	COLORREF	m_color, *liveptr;

	RECT		ra[MAXLABELS];
	COLORREF	cr[MAXLABELS];

	//Clines		*pLine[4];
	//RECT		la[4];

	//CString		m_title;

	int			labcount;

	CLabel		*labels[MAXLABELS];
	CString		m_prompt;
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPicSel)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPicSel)
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	virtual void OnOK();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnCheck2();
	afx_msg void OnCheck5();
	afx_msg void OnCheck4();
	afx_msg void OnButton1();
	afx_msg void OnKillfocusEdit1();
	afx_msg void OnKillfocusEdit2();
	afx_msg void OnKillfocusEdit3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PICSEL_H__8D67F2C4_FB89_40F0_80F1_445D474FFAF7__INCLUDED_)
