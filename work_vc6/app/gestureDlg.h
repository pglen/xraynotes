
/* =====[ gestureDlg.h ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the gestureDlg.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  10/22/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#if !defined(AFX_MAGDLG_H__8D067BA6_3FC7_4AB2_AD61_681CE9FEDFB7__INCLUDED_)
#define AFX_MAGDLG_H__8D067BA6_3FC7_4AB2_AD61_681CE9FEDFB7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "xfile.h"

/////////////////////////////////////////////////////////////////////////////
// CGestureDlg dialog

class CGestureDlg : public CGrayDlg

{
	typedef	struct _coms
		{
		char	*str;
		void   (*func)(void*);
		} coms;

	static coms	varcoms[];

	typedef struct _tdata
		{	
		CString		str;
		int			item;
		CWordArray	arr;
		} tdata;

public:

	// Construction
	CGestureDlg(CWnd* pParent = NULL);   // standard constructor
	~CGestureDlg();

	// Funcs
	void	PlayGesture();
	void	ExecGesture(CString &str);
	int		LoadSave(int loadflag, CString &fname);

	// Data
	int		push;

private:

	// Funcs
	void	DumpGestures();
	void	DumpAngleArray();
	void	DumpMouArr();

	void	Train();
	void	WriteNL(CxCFile *fp);
	void	LoadGestures(CString &fname);
	void	SaveGestures(CString &fname);
	void	ExecGesture2(CString &str);
	void	InvalDrawing();
	int		Fuzzy(CWordArray *arr);
	void	DoMagic();
	int		isTrainChecked();
	void	ShowProg(const char * str);
	void	SynLButtonUp(int timeout);

	// Data
	CWordArray	xarr, yarr, normalarr;	
	CPtrArray	traindata;
	CString currfile;

	HCURSOR pencil;

	// Flags and such
	int		changed, created, loaded;
	int		in_draw, busy;
	int		old_x, old_y;
	int		dark, more;
	int		drag, drag_x, drag_y;
	int		addto;

	CPoint	lastpoint;
	UINT	lastflags;
	int		trigger;
	int		lbutton;

protected:

// Dialog Data
	//{{AFX_DATA(CGestureDlg)
	enum { IDD = IDD_DIALOG7 };
	CLabel	m_com;
	CxTargetButton	m_b9;
	CxTargetButton	m_b8;
	CxTargetButton	m_b7;
	CxTargetButton	m_b6;
	CxTargetButton	m_b4;
	CxTargetButton	m_b3;
	CxTargetButton	m_b10;
	CxTargetButton	m_b2;
	CxTargetButton	m_butt1;
	CLabel	m_prog;
	CListBox	m_list;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGestureDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGestureDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual void OnOK();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnButton6();
	afx_msg void OnButton4();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnDblclkList1();
	afx_msg void OnButton7();
	afx_msg void OnButton8();
	afx_msg void OnButton9();
	afx_msg void OnDestroy();
	afx_msg void OnButton10();
	afx_msg void OnRadio1();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAGDLG_H__8D067BA6_3FC7_4AB2_AD61_681CE9FEDFB7__INCLUDED_)

