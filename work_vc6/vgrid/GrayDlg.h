
/* =====[ GrayWin.h ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the GrayWin.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  10/29/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// GrayWin.h: interface for the CGrayDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAYWIN_H__BE3CE0FD_4B2D_4BAC_A21F_E674E50B9921__INCLUDED_)
#define AFX_GRAYWIN_H__BE3CE0FD_4B2D_4BAC_A21F_E674E50B9921__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PersistDlg.h"

class CGrayDlg : public PersistDlg  

{

public:

	void CGrayDlg::SetWindowText(LPCTSTR lpszString);

	void	Show(int center = false);
	int		IsDlgKey(MSG *pMsg);
	
	CGrayDlg(CWnd* pParent =NULL);
	CGrayDlg(UINT nIDTemplate, CWnd* pParentWnd);
	
	virtual ~CGrayDlg();

	// Data
	CString m_title;

	int		dheadcol, headcol;
	//int		destroyed;
	int		titleheight;
	int		noheader, noclosebox;

protected:

	virtual BOOL	OnInitDialog();
	void	InitVars();
	
	int		nstate;
	
	CFont	m_font, m_fontb;

	CRect	closerect, titlerect;
	CRect	closerect2, titlerect2;
	
	//void	OnLButtonDown(UINT nFlags, CPoint point);	
	//void	OnPaint(); 

	// Dialog Data
	//{{AFX_DATA(CGrayDlg)
	enum { IDD = 0 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// Overrides

	// ClassWizard generated virtual function overrides

	//{{AFX_VIRTUAL(CGrayDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CGrayDlg)
	afx_msg virtual void OnPaint();
	afx_msg virtual void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg virtual void OnDestroy();
	afx_msg virtual void OnSize(UINT nType, int cx, int cy);
	afx_msg virtual void OnSetFocus(CWnd* pOldWnd);
	afx_msg virtual void OnKillFocus(CWnd* pNewWnd);
	afx_msg virtual void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg virtual void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_GRAYWIN_H__BE3CE0FD_4B2D_4BAC_A21F_E674E50B9921__INCLUDED_)
