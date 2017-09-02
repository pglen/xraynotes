
/* =====[ xConfig.h ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the xConfig.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  11/19/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#if !defined(AFX_XCONFIG_H__1617ACFB_1294_49E9_AE47_C37A8FB319CD__INCLUDED_)
#define AFX_XCONFIG_H__1617ACFB_1294_49E9_AE47_C37A8FB319CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// xConfig.h : header file
//

#include "label.h"
#include "transslider.h"
#include "GrayDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CxConfig dialog

class CxConfig : public CGrayDlg
{
// Construction
public:
	int CxConfig::unSelCombo(CString &str, CWnd *combo);
	int CxConfig::SelCombo(int resamp, CWnd *combo) ;
	CxConfig(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CxConfig)
	enum { IDD = IDD_DIALOG25 };
	CxTargetButton	m_can;
	CxTargetButton	m_ok;
	CxTargetButton	m_b3;
	CxTargetButton	m_b2;
	CxTargetButton	m_b1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CxConfig)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CxConfig)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnSelchangeCombo2();
	afx_msg void OnButton1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XCONFIG_H__1617ACFB_1294_49E9_AE47_C37A8FB319CD__INCLUDED_)
