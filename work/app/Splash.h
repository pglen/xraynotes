
/* =====[ Splash.h ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the Splash.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  12/25/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#if !defined(AFX_SPLASH_H__3787EC70_C900_4BB7_B860_90F124B9762F__INCLUDED_)
#define AFX_SPLASH_H__3787EC70_C900_4BB7_B860_90F124B9762F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Splash.h : header file
//

//include "Label.h"

/////////////////////////////////////////////////////////////////////////////
/// CSplash Dialog

/// Responsible for showing the initial splash screen for the XrayNotes 
///  Application.

class CSplash : public CDialog

{
// Construction
public:
	void	Show();
	void	Hide();

	int		elapsed_time, alpha, m_anim;

	CSplash(CWnd* pParent = NULL);   // standard constructor

	//CxImage image;

// Dialog Data
	//{{AFX_DATA(CSplash)
	enum { IDD = IDD_SPLASH };
	CLabel	m_demo;
	//}}AFX_DATA

	int		addon;
	//int		m_nCount;			

	CxImg	image, image2;

	CBitmap	m_bitmap;				// Splash Bitmap
	int		m_nWidth, m_nHeight;	// Splash Bitmap's width & height

	CBitmap m_bitmap2;
	CBitmap m_bitmap3;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSplash)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSplash)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPLASH_H__3787EC70_C900_4BB7_B860_90F124B9762F__INCLUDED_)
