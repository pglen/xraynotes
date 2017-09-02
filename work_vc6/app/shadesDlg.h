
/* =====[ shadesDlg.h ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the shadesDlg.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  10/27/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// shadesDlg.h : header file
//

#if !defined(AFX_SHADESDLG_H__5494EEAA_64A7_4D88_9C5B_5B81FF1B9061__INCLUDED_)
#define AFX_SHADESDLG_H__5494EEAA_64A7_4D88_9C5B_5B81FF1B9061__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

int     AfxPrintf(const char *Format, ...);

#include "FileDialogST.h"
#include "Flag.h"
#include "XrayM.h" 

#define MAXDOCS	32

/////////////////////////////////////////////////////////////////////////////
// PLUGIN related 

#define BIT_0 (1<<0)
#define BIT_1 (1<<1)
#define BIT_2 (1<<2)
#define BIT_3 (1<<3)
#define BIT_4 (1<<4)
#define BIT_5 (1<<5)
#define BIT_6 (1<<6)
#define BIT_7 (1<<7)

#define		PLUG_MAGIC	0x11223344


//////////////////////////////////////////////////////////////////////////
// Messages used internally by xraynotes

#define     WM_APP_OPEN				(WM_USER + 1)
#define     WM_CLEAR_RECENT			(WM_USER + 1000)
#define		WM_OPEN_RECENT			(WM_USER + 1001)

#define		WM_CLEAR_RECENT_DICOM	(WM_USER + 1030)
#define		WM_OPEN_RECENT_DICOM	(WM_USER + 1031)

#define		WM_CLEAR_RECENT_DICOM_S	(WM_USER + 1060)
#define		WM_OPEN_RECENT_DICOM_S	(WM_USER + 1061)


//////////////////////////////////////////////////////////////////////////

#define MAXRECENT 16

// Read: pointer to a function returning string
typedef void	(*pf_rv)();
typedef char *	(*pf_rs)();
typedef int 	(*pf_ri)();
typedef int 	(*pf_ri_piii)(void *, int, int, int);

typedef struct _plugin

{
	int			magic;
	int			comid;
	int			running;
	char		Name[_MAX_PATH];	
	HMODULE		hm;

	// Functions in the DLL

	pf_rv		Stop;
	pf_rs		Message;
	pf_rs		Description;
	pf_ri		DLLVersion;
	pf_ri		APIVersion;
	pf_ri_piii	Process;
	
}	Plugin;

#include "ShadesHelp.h"
#include "ShadesFunc.h"
#include "ShadesSel.h"

/////////////////////////////////////////////////////////////////////////////
// CShadesDlg dialog

class CShadesDlg : public CDialog

{
	typedef struct _showarr

{
	int			renx, reny, xpos, ypos;
	int			frame;
	//CXrayDoc	*doc;
	CxImg		*img;
	
} showarr;

	int		keylock;
	CFont	m_LogoFont, m_Font;

	CString sLogoString;
	showarr *saptr;

public:

	int		movetresh;
	int		EraseBkgnd(CDC* pDC);
	void	PaintLogo(CDC *pDC);
	void	DumpSap();
	void	InitFunctions();

	friend class CShadesHelp;
	friend class CShadesSel;
	friend class CShadesFunc;

	CShadesHelp  *helper;
	CShadesFunc	 *functs;
	CShadesSel	 *sels;

	CXrayDoc* GetCurrDoc();

	int		plugin_running;
	void	SuppWindows(int show);
	void	ReRender(CXrayDoc *doc, int show);
	void	InvalidateSurface();
	void	UpdateCursor();
	void	CloseAllDialogs();
	void	BusyCursor(int bus = true);
	//void	TmpRestorePanels();
	//void	TmpClosePanels();

	//void	CalcSelRect(RECT *rc);
//	void	SelToRender(int toimg = false);
//	void	SelMove(CPoint point);
//	void	MarkMove(CPoint point);
//	void	MarkLdown(CPoint point);
//	void	SelLdown(CPoint point);

	void	RectMove(CPoint point);

	int		showrender;
	int		resetpositions;

	void	Idle();
	void	DrawFreeHandSel(CDC *pDC);
	void	DrawImage(CDC *pDC);

	int		xpos, ypos, renx, reny;
	int		windowmove, moire, busy;

	//int		drawstate;

	//void	MarkToRender(int toimg  = false);


	void	SelDoc(int idx);
	int		PromptFileName(const char *str, 	CFileDialogST &dlgFile);
	void	Exception();
	void	EndDialog(int ret, int noprompt = false);
	void	GetSelRect(RECT *rc);
	
	HCURSOR  targcircle;
	HCURSOR  pencil;
	HCURSOR  selectcur;
	HCURSOR  markcur;
	HCURSOR  waitcur;
	HCURSOR  arrowcur;
	HCURSOR	 rectcur;
	HCURSOR	 circcur;
	HCURSOR	 textcur;
	HCURSOR	 linecur;
	HCURSOR	 imgcur;

	CPoint	old_point; 
	CPoint	startp, endp;
	CPoint	currmouse; 
	
	//////////////////////////////////////////////////////////////////////////
	// Array to hold the list of documents
	CPtrArray	docarr;	

	//////////////////////////////////////////////////////////////////////////
	// Plugin related
	CPtrArray	plugarr;

	/// Array to hold the marker
	CDWordArray	 xmarkarr, ymarkarr;

	// 
	CRect	selrect, updaterect;
	
	//CString lastfilename;

	int		old_x, old_y;
	int		magic;
	int		current;					// Current document index
	int		autoadvance, autodir;		// Auto show
	int		hscroll, vscroll;			// Current scroll value
	int		rbdown, lbdown;				// Button flags

	int		minmax;

	// Document navigation
	
	void	DrawGrid(CDC *pDC);
	void	OnOK();
	void	OnCancel();

	int		handlehit, handlesel;
	int		drag, drag_x, drag_y;
	
	CShadesDlg(CWnd* pParent = NULL);	// standard constructor
	~CShadesDlg();

// Dialog Data
	//{{AFX_DATA(CShadesDlg)
	enum { IDD = IDD_SHADES_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShadesDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	CBitmap  m_bitmap;			// Splash Bitmap
	int m_nWidth, m_nHeight;	// Splash Bitmap's width & height
	int	created;

// Implementation
protected:

	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CShadesDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnQueryEndSession();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnClose();
	afx_msg void OnOpenOpendicomfile();
	afx_msg void OnOpenOpendicomseries();
	afx_msg void OnOpenDebugdumphandles();
	afx_msg void OnOpenOpenpythondialog();
	afx_msg void OnOpenOpenimagefile();
	afx_msg void OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnOpenNewimage();
	afx_msg void OnFileNew( );
	afx_msg void OnFileOpen( );
	afx_msg void OnFilePrint( );
	afx_msg void OnRclicknodocExitwithoutpromptforcesave();
	afx_msg void OnOpenOpenlastdicomfile();
	afx_msg void OnOpenOpenlastdicomseries();
	afx_msg void OnCloseClosecurrentdefault();
	afx_msg void OnRclicktextProperties();
	afx_msg void OnRclicktextChangebackgroundcolor();
	afx_msg void OnRclicktextChangeforegroundcolor();
	afx_msg void OnRclicktextSendtoback();
	afx_msg void OnRclicktextSendtofront();
	afx_msg void OnRclicktextSendbackwardonezorder();
	afx_msg void OnRclicktextSendforwardone();
	afx_msg void OnRclicktextAddnewtextdefault();
	afx_msg void OnRclicktextAddnewgraytext();
	afx_msg void OnRclicktextDumptextobjects();
	afx_msg void OnRclicktextAlignAligntotheleft();
	afx_msg void OnRclicktextAlignAligntotheright();
	afx_msg void OnRclicktextAlignAlightothetop();
	afx_msg void OnRclicktextAlignAligntothebottom();
	afx_msg void OnRclicktextAlignHorizontalcenter();
	afx_msg void OnRclicktextAlignDistributehorizontally();
	afx_msg void OnRclicktextAlignDistributevertically();
	afx_msg void OnRclicktextAlignEnsurevisible();
	afx_msg void OnRclicknodInserttextobject();
	afx_msg void OnRclicklineLineproperties();
	afx_msg void OnExitaltAnother();
	afx_msg void OnExitaltDumpdocuments();
	afx_msg void OnRclicknodocClosecurrentdocument();
	afx_msg void OnRclicknodEnsureallobjectsarevisible();
	afx_msg void OnRclicknodInsertstraightline();
	afx_msg void OnRclicknodInsertrectangleobject();
	afx_msg void OnRclicktextDeleteobject();
	afx_msg void OnRclicknodDrawfreehandlines();
	afx_msg void OnRclicknodDrawinsertcircle();
	afx_msg void OnRclicknodInsertimageobject();
	afx_msg void OnRclicktextChangeobjecttransparency();
	afx_msg void OnActivateApp(BOOL bActive, HTASK hTask);
	afx_msg void OnRclicktextRotateobject();
	afx_msg void OnRclicknodocShowonscreenkeyboard();
	afx_msg void OnRclicktextMoveobject();
	afx_msg void OnRclicktextResize();
	afx_msg void OnRclicknodocToggleminmax();
	afx_msg void OnMove(int x, int y);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHADESDLG_H__5494EEAA_64A7_4D88_9C5B_5B81FF1B9061__INCLUDED_)

