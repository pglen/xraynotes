
/* =====[ ShapeProp.h ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the ShapeProp.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  1/29/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#if !defined(AFX_SHAPEPROP_H__A8FB7F7C_B908_4E36_84FB_90C81449BA1F__INCLUDED_)
#define AFX_SHAPEPROP_H__A8FB7F7C_B908_4E36_84FB_90C81449BA1F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShapeProp.h : header file
//
#include "GrayDlg.h"  

/////////////////////////////////////////////////////////////////////////////
// CShapeProp dialog

class CShapeProp : public CGrayDlg
{
// Construction
public:
	void SaveColors();
	void LoadColors();

	CLayer *lay;
	void	Apply(int offset, int val1 = 0, int val2 = 0);

	CShapeProp(CWnd* pParent = NULL);   // standard constructor

	void	OnOK();
	void	OnCancel();

// Dialog Data
	//{{AFX_DATA(CShapeProp)
	enum { IDD = IDD_DIALOG32 };
	CxTargetButton	m_but5;
	CLabel	m_l4;
	CLabel	m_l3;
	CLabel	m_b4;
	CxTargetButton	m_but4;
	CLabel	m_b3;
	CxTargetButton	m_ok;
	CxTargetButton	m_can;
	CxTargetButton	m_but3;
	CxTargetButton	m_but2;
	CLabel	m_b2;
	CLabel	m_b1;
	CLabel	m_l2;
	CLabel	m_l1;
	CString	m_ed5;
	CString	m_ed3;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShapeProp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CShapeProp)
	afx_msg void OnRadio13();
	afx_msg void OnRadio14();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnRadio12();
	afx_msg void OnRadio6();
	afx_msg void OnRadio11();
	afx_msg void OnRadio10();
	afx_msg void OnRadio9();
	afx_msg void OnRadio7();
	afx_msg void OnRadio8();
	virtual BOOL OnInitDialog();
	afx_msg void OnRadio18();
	afx_msg void OnRadio15();
	afx_msg void OnRadio16();
	afx_msg void OnRadio17();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnRadio19();
	afx_msg void OnMrul1();
	afx_msg void OnMrub1();
	afx_msg void OnMrub2();
	afx_msg void OnMrub3();
	afx_msg void OnMrub4();
	afx_msg void OnMrul2();
	afx_msg void OnMrul3();
	afx_msg void OnMrul4();
	afx_msg void OnButton5();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHAPEPROP_H__A8FB7F7C_B908_4E36_84FB_90C81449BA1F__INCLUDED_)
