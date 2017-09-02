
/* =====[ xyzDlg.h ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the xyzDlg.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  11/26/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#if !defined(AFX_XYZDLG_H__8F735788_9752_4E2E_855A_6718E37C4856__INCLUDED_)
#define AFX_XYZDLG_H__8F735788_9752_4E2E_855A_6718E37C4856__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// xyzDlg.h : header file
//

#include "label.h"
#include "transslider.h"
#include "GrayDlg.h"
#include "2dCtl.h"

/////////////////////////////////////////////////////////////////////////////
// CxyzDlg dialog

class CxyzDlg : public CGrayDlg

{
// Construction
public:
	void Update3D(int xx, int yy);
		
	CxyzDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CxyzDlg)
	enum { IDD = IDD_DIALOG27 };
	C2dCtl	m_2d;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CxyzDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CxyzDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XYZDLG_H__8F735788_9752_4E2E_855A_6718E37C4856__INCLUDED_)
