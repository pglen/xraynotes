
/* =====[ PersistDlg.h ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the PersistDlg.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  11/7/2007  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#if !defined(AFX_PERSISTDLG_H__3B62E52E_E649_4715_9316_A96327899DD1__INCLUDED_)
#define AFX_PERSISTDLG_H__3B62E52E_E649_4715_9316_A96327899DD1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PersistDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// PersistDlg dialog

class PersistDlg : public CDialog

{
public:

	int		bAlpha;
	int		showstate;
	int		pre_showstate;
	int		xidd;
	int		pre_want_x, pre_want_y;
	int		pre_want_w, pre_want_h;
	int		posload;
	int		rdown;
	int		noheader;

// Construction
public:
	void Hide();
	void Show();

	PersistDlg(CWnd* pParent = NULL);   // standard constructor
	PersistDlg(UINT nIDTemplate, CWnd* pParentWnd = NULL);

// Dialog Data
	//{{AFX_DATA(PersistDlg)
	enum { IDD = 1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PersistDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	void InitVars();

	// Generated message map functions
	//{{AFX_MSG(PersistDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnClose();
	afx_msg void OnPaint();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PERSISTDLG_H__3B62E52E_E649_4715_9316_A96327899DD1__INCLUDED_)
