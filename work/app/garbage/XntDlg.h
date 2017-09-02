
/* =====[ XntDlg.h ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the XntDlg.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  10/29/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#if !defined(AFX_XNTDLG_H__78B686C6_81D7_4ED3_AC9A_BC525D543A54__INCLUDED_)
#define AFX_XNTDLG_H__78B686C6_81D7_4ED3_AC9A_BC525D543A54__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XntDlg.h : header file
//

#include "GrayDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CXntDlg dialog

class CXntDlg : public CGrayDlg

{
	int		xidd;
	void	InitVars();
	
public:

	void	Show(int center = false);
	void	Hide();

	CXntDlg(CWnd* pParent = NULL);   // standard constructor
	CXntDlg::CXntDlg(UINT nIDTemplate, CWnd* pParentWnd);

// Dialog Data
	//{{AFX_DATA(CXntDlg)
	enum { IDD = 1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXntDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CXntDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XNTDLG_H__78B686C6_81D7_4ED3_AC9A_BC525D543A54__INCLUDED_)
