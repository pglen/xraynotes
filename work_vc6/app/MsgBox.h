
/* =====[ MsgBox.h ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the MsgBox.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  11/24/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#if !defined(AFX_MSGBOX_H__97DB95B2_93EC_41F6_AB21_F4C3C3D336AA__INCLUDED_)
#define AFX_MSGBOX_H__97DB95B2_93EC_41F6_AB21_F4C3C3D336AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MsgBox.h : header file
//

#include "GrayDlg.h"

#define MBTYPE_YESOK		0
#define MBTYPE_YESNO		1
#define MBTYPE_YESNOSTOP	2

#define MBOXYES		1
#define MBOXNO		0
#define MBOXSTOP	-1

#define DEFYES		1
#define DEFNO		2
#define DEFSTOP		3


#define AUTOCLOSE 11	// Number of seconds in close
#define AUTOWARN  3		// Number of seconds before timeout warning

/////////////////////////////////////////////////////////////////////////////
// CMsgBox dialog

class CMsgBox : public CGrayDlg

{
	// Construction
public:

	void OnCancel();
	void OnOK();

	CSize titlesss, textsss;

	CString m_timer;

	int		timeout;
	int		yesno;
	int		idret;

	int		tocount;

	CSize GetTextSize(const char *str, CFont *font);

	CFont	m_font, m_fontb;
	CString m_text;
	CString m_header;

	CMsgBox(CWnd* pParent = NULL);   // standard constructor

	//CRect closerect;

	int			drag, drag_x, drag_y;
	CBitmap		m_bitmap;				// Bitmap
	int			m_nWidth, m_nHeight;	// Bitmap's width & height
	
// Dialog Data
	//{{AFX_DATA(CMsgBox)
	enum { IDD = IDD_MBOX };
	CxTargetButton	m_stop;
	CxTargetButton	m_no;
	CxTargetButton	m_yes;
	CxTargetButton	m_ok;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMsgBox)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual int DoModal();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMsgBox)
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnOk2();
	afx_msg void OnOk3();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnOk4();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSGBOX_H__97DB95B2_93EC_41F6_AB21_F4C3C3D336AA__INCLUDED_)
