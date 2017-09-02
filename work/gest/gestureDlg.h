#if !defined(AFX_MAGDLG_H__8D067BA6_3FC7_4AB2_AD61_681CE9FEDFB7__INCLUDED_)
#define AFX_MAGDLG_H__8D067BA6_3FC7_4AB2_AD61_681CE9FEDFB7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MagDlg.h : header file
//

#include "gest.h"
#include "label.h"
#include "GrayWin.h"
#include "xtargetbutton.h"

/////////////////////////////////////////////////////////////////////////////
// CGestureDlg dialog

class CGestureDlg : public CGrayDlg

{
	typedef struct _tdata
		{	
		CString		str;
		int			item;
		CDWordArray	arr;
		} tdata;

	typedef struct _stroke
		{	
		short	dist;
		short	angle;
		} stroke;

	int newtrain;

// Construction
public:
	int		RecFuzzy(CDWordArray *arr) ;
	void	RecBuildArr(CDWordArray *arr);
	void	Recognize();

	void	PlayGesture();
	void	DumpGestures();
	
	CGestureDlg(CWnd* pParent = NULL);   // standard constructor
	~CGestureDlg();

	void	Move(int want_x, int want_y);
	void	ExecGesture(CString &str);
	void	ExecGesture2(CString &str);

	int		LoadSave(int loadflag, CString &fname);
	void	InvalDrawing();
	void	Show();

	int		more;
	int		push;
	int		changed;
	int		in_draw;
	int		want_x, want_y;
	int		created;
	int		dark;
	int		drag, drag_x, drag_y;
	//void	*parent;
	int		old_x, old_y;

	clock_t		lastmdown;
	clock_t		lastmup;

	CDWordArray	xarr, yarr, marr;
	CDWordArray	normalarr;
	
	CPtrArray	traindata;

// Dialog Data
	//{{AFX_DATA(CGestureDlg)
	enum { IDD = IDD_GESTDIALOG };
	CxTargetButton	m_b7;
	CxTargetButton	m_b15;
	CEdit	m_edit;
	CxTargetButton	m_butt1;	
	CxTargetButton	m_Add	    ;
	CxTargetButton	m_Delete	;	
	CxTargetButton	m_Load		;	
	CxTargetButton	m_Save		;	
	CxTargetButton	m_More 		;	
	CxTargetButton	m_Dump		;	
	CxTargetButton	m_Play		;	
	CxTargetButton	m_Place		;	
	CxTargetButton	m_ClearLast	;	
	CxTargetButton	m_Copy		;	
	CxTargetButton	m_Paste		;	
	CxTargetButton	m_Clear2		;	
	CxTargetButton	m_Recall	;	
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
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSysChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
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
	afx_msg void OnButton11();
	afx_msg void OnButton15();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAGDLG_H__8D067BA6_3FC7_4AB2_AD61_681CE9FEDFB7__INCLUDED_)
