
/* =====[ DBMain.h ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the DBMain.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  10/28/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#if !defined(AFX_DBMAIN_H__4F078312_8AB9_45F5_8F3B_3B9E69A6C218__INCLUDED_)
#define AFX_DBMAIN_H__4F078312_8AB9_45F5_8F3B_3B9E69A6C218__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DBMain.h : header file
//

#include "GrayDlg.h"
#include "BmpButt.h"

/////////////////////////////////////////////////////////////////////////////
// CDBMain dialog

class CDBMain : public CGrayDlg

{
// Construction
public:

	void	DecorateButtons();
	void	UpdateCheck();
	
	CBmpButt	*arr[6];
	int		drag, drag_x, drag_y;

	CDBMain(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDBMain)
	enum { IDD = IDD_DIALOG21 };
	CLabel	m_pat;
	CBmpButt	m_bmp6;
	CBmpButt	m_bmp4;
	CBmpButt	m_bmp5;
	CBmpButt	m_bmp3;
	CBmpButt	m_bmp2;
	CBmpButt	m_bmp1;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDBMain)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDBMain)
	afx_msg void OnButton1();
	afx_msg void OnButton3();
	virtual BOOL OnInitDialog();
	afx_msg void OnBmp1();
	afx_msg void OnBmp2();
	afx_msg void OnBmp4();
	afx_msg void OnBmp5();
	afx_msg void OnBmp6();
	afx_msg void OnBmp3();
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnCheck1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DBMAIN_H__4F078312_8AB9_45F5_8F3B_3B9E69A6C218__INCLUDED_)
