
/* =====[ 2dCtl.h ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the 2dCtl.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  11/25/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// 2dCtl.h: interface for the C2dCtl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_2DCTL_H__81201376_C8A1_4019_8C9A_1FC9397A0727__INCLUDED_)
#define AFX_2DCTL_H__81201376_C8A1_4019_8C9A_1FC9397A0727__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define	CTRL2D_START	WM_USER+1
#define	CTRL2D_MOVE		WM_USER+2
#define	CTRL2D_END		WM_USER+3

#include "BaseWnd.h"

class C2dCtl : public CBaseWnd  
{
public:
	C2dCtl();
	virtual ~C2dCtl();
	
	// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(C2dCtl)
	//}}AFX_VIRTUAL

// Implementation
public:

	void SetPos(int xxx, int yyy);
	void OnInitControl();

	void	OnGetTipText(CString &str);


protected:

	RECT	bull, sbull;
	int		bullw;

	void	DoMove(const POINT *pnp);
	void	DoMove(int xxx, int yyy);

	void	CenterRect(int xx, int yy,  RECT *rc);
	void	CenterRect(POINT *pt, RECT *rc);

private:
	int		capped;
	int		xx, yy;	

	// Generated message map functions
protected:
	//{{AFX_MSG(C2dCtl)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_2DCTL_H__81201376_C8A1_4019_8C9A_1FC9397A0727__INCLUDED_)
