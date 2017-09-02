
/* =====[ TextProp.h ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the TextProp.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  12/9/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#if !defined(AFX_TEXTTPROP_H__A287E6AC_B722_4363_8428_91FA3DEE6D30__INCLUDED_)
#define AFX_TEXTTPROP_H__A287E6AC_B722_4363_8428_91FA3DEE6D30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// TexttProp.h : header file
//

#include "editex.h"
#include "PGedit.h"
#include "transslider.h"

/////////////////////////////////////////////////////////////////////////////
// CTextProp dialog

class CTextProp : public CGrayDlg

{
	int in_timer;

// Construction
public:
	void Apply(int offset, int val1 = 0, int val2 = 0);

	COLORREF	ColorLookup(int idx);
	void		OnCancel();
	void		OnOK();

	CLayText *lay;
	CTextProp(CWnd* pParent = NULL);   // standard constructor

	CString		orgm, orgt;

	//COLORREF	orgb, orgf;

	void		OnSlide1(int wParam, int lParam);
	void		OnSlide2(int wParam, int lParam);
	void		OnSlide3(int wParam, int lParam);
	void		OnSlide4(int wParam, int lParam);

// Dialog Data
	//{{AFX_DATA(CTextProp)
	enum { IDD = IDD_DIALOG28 };
	CTransSlider	m_slide6;
	CxTargetButton	m_ok;
	CxTargetButton	m_can;
	CxTargetButton	m_b9;
	CxTargetButton	m_b8;
	CxTargetButton	m_b2;
	CxTargetButton	m_b1;
	CTransSlider	m_slide4;
	CTransSlider	m_slide3;
	CTransSlider	m_slide2;
	CTransSlider	m_slide1;
	CPGedit	m_edit2;
	CPGedit	m_edit1;
	CString	m_main;
	CString	m_tip;
	CString	m_ed5;
	CString	m_ed6;
	CString	m_ed7;
	CString	m_ed11;
	CString	m_ed12;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTextProp)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTextProp)
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButton2();
	afx_msg void OnButton1();
	afx_msg void OnButton8();
	afx_msg void OnChangeEdit1();
	afx_msg void OnButton9();
	afx_msg void OnCheck1();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	afx_msg void OnCheck4();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTTPROP_H__A287E6AC_B722_4363_8428_91FA3DEE6D30__INCLUDED_)
