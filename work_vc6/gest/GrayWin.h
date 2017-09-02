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
	
	CString m_title;
	BOOL OnInitDialog();
	int		nstate;

	CFont	m_font, m_fontb;
	CRect	closerect, titlerect;

	//CGrayDlg();
	CGrayDlg(CWnd* pParent =NULL);
	CGrayDlg(UINT nIDTemplate, CWnd* pParentWnd);
	
	void OnLButtonDown(UINT nFlags, CPoint point);
	virtual ~CGrayDlg();

	void	OnPaint(); 
	
	// Dialog Data
	//{{AFX_DATA(CGrayDlg)
	enum { IDD = 1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// Overrides

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGrayDlg)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

	//}}AFX_VIRTUAL

	//{{AFX_MSG(CGrayDlg)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_GRAYWIN_H__BE3CE0FD_4B2D_4BAC_A21F_E674E50B9921__INCLUDED_)
