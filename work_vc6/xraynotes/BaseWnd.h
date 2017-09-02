
/* =====[ BaseWnd.h ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the BaseWnd.h   
					Baseclass to derive controls from
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  11/25/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#if !defined(AFX_BASEWND_H__0E1C41DF_EFE3_4C52_908E_0BCAA98DCF58__INCLUDED_)
#define AFX_BASEWND_H__0E1C41DF_EFE3_4C52_908E_0BCAA98DCF58__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// BaseWnd.h : header file
//

#include "popup.h"
#include "Flag.h"

//define  XCTRL_CLASSNAME "XrayNotesClass"

/////////////////////////////////////////////////////////////////////////////
// CBaseWnd window

class CBaseWnd : public CWnd

{
	DECLARE_DYNCREATE(CBaseWnd)

// Construction
public:

	CBaseWnd();
	CPopup	tip;

	CPoint	lastmousepoint;
	
private:

	int		capped, fired;
	int		xdrag, ydrag;

	int		firstpaint;
	CFlag	entered;

	CPoint	capmouse;

// Attributes
public:

	
// Operations

public:

	void InvalidateParentRect(const RECT *rc);
	void InvalidateParent();
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBaseWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

	virtual void OnMouseHover();
	virtual void OnMouseLeave();
	virtual void OnMouseEnter();
	virtual void OnGetTipText(CString &str);
	virtual void OnInitControl();
	virtual	void OnFirstPaint();

	virtual ~CBaseWnd();


	// Implementation
public:
	virtual int KeyEvent(UINT event, UINT nChar, UINT nFlags);
	int hovertime;
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CBaseWnd)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG

	afx_msg virtual void OnSysChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg virtual void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BASEWND_H__0E1C41DF_EFE3_4C52_908E_0BCAA98DCF58__INCLUDED_)
