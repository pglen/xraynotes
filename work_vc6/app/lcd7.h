
/* =====[ lcd7.h ]========================================== 
                                                                             
   Description:     The dbtest project, implementation of the lcd7.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  1/15/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#if !defined(AFX_LCD7_H__2D19C590_DD9E_4371_9827_4E02E4826C02__INCLUDED_)
#define AFX_LCD7_H__2D19C590_DD9E_4371_9827_4E02E4826C02__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// lcd7.h : header file
//

#define WM_OVERFLOW			(WM_USER + 6)
#define WM_UNDERFLOW		(WM_USER + 7)
#define	WM_TCHANGE			(WM_USER + 8)

/////////////////////////////////////////////////////////////////////////////
// Clcd7 window

class Clcd7 : public CWnd
{
// Construction
public:
	int		nomessage;


	Clcd7();
	int  gap, width;
	int num, oldnum;
	int low, high;
	int firstpaint;
	CPoint oldpoint;
	bool seg1, seg2, seg3, seg4, seg5, seg6, seg7;
	
	COLORREF fg;
	COLORREF bg;

// Attributes
public:

// Operations
public:
	void SetFgColor(COLORREF col);
	void SetBgColor(COLORREF col);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Clcd7)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetMinMax(int minn, int maxx);
	virtual ~Clcd7();

	// Generated message map functions
protected:
	//{{AFX_MSG(Clcd7)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LCD7_H__2D19C590_DD9E_4371_9827_4E02E4826C02__INCLUDED_)
