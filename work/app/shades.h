
/* =====[ shades.h ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the shades.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  9/17/2007  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// shades.h : main header file for the SHADES application
//

#if !defined(AFX_SHADES_H__10CE72D1_1678_4810_8D52_1EA02C2176E0__INCLUDED_)
#define AFX_SHADES_H__10CE72D1_1678_4810_8D52_1EA02C2176E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#define MYNEW(type)  (new type; ASSERT(type);)

#include "resource.h"		// main symbols
#include "setjmp.h"

//////////////////////////////////////////////////////////////////////////
// Project specific defines

#define		MBOX		xMsgBox
#define		MBOXP		AfxPrintf

#define		DEMO_DAYS	30
#define		STAMP_MAX	800

#define		SHADE_MAGIC 0x1234567
#define		PACK_MAGIC 0x12345678

#define INITBLEND(aa, bb)							\
		aa.BlendOp	= AC_SRC_OVER;					\
		aa.BlendFlags = 0;							\
		aa.SourceConstantAlpha = min(bb, 255);		\
		aa.AlphaFormat = 0;							\

//////////////////////////////////////////////////////////////////////////
// Helper routines

extern	int		query_perf();
extern	int		mark_perf();

//extern	int		heapcheck(const char *str, int showok = false);

extern  int		xMsgBox(const char *str, const char *title = "", int yesno = false, int defa = true);

//extern	BOOL	GetSpecialFolder(UINT SpecialFolder, CString &SpecialFolderString);

extern	CString GetDir(const CString& name);

//extern	int		ParseComandLine(const char *cmdline, CString *arr, int lim);
//extern  int		InterpretComandLine(CString *arr, int lim);
//extern	void	OpenFileCb(void *ptr);
//extern	void	CloseFileCb(void *ptr);

extern	void	ShowStatus(const char *str, int prog = 0, int tme = 2000);
extern	void	ShowStatusP(const char *Format, ...);
extern	void	HideStatus();

//extern	CString GetFileNameExt(const CString& name);

//extern	void	BrightCb(void *ptr);

extern	void	ReportException(const char *file, int line);
extern	int		FindImageType(const CString& ext);

//void	SplitPath(const char *full,  CString &drive,
//					CString &dir, CString &fname, CString &ext);

//////////////////////////////////////////////////////////////////////////
// Global includes

#include "..\\CxImage\\ximage.h"

#include "xTargetButton.h"
#include "XrayDoc.h"
#include "tooldef.h"
#include "commline.h"
#include "support.h"
#include "MagWnd.h"

//////////////////////////////////////////////////////////////////////////
// Global Data

extern  CCommLine cline;
extern	POINT	appmouse;
extern	POINT	appmouseclick;
extern	HWND	appwnd;

extern	int	gl_resamp_rough;
extern  int	gl_resamp_fine;
extern	int	gl_animated;

extern	int	xnt_clip;
extern	int	xnt_multi;
extern	int	xnt_bin;

//////////////////////////////////////////////////////////////////////////
// Global Strings

#define	PAPER_ASPECT_RATIO		(11.0/8.0)
#define	MIN_SCREEN_WIDTH		1280

extern	CString dataroot;
extern	CString intialcwd;
extern	CString approot;
extern  CString appdata;
extern	CString tmproot;
extern	CString shared_data;
extern  CString	appdirname;

extern	CString lastseries;
extern	CString strConfig;

extern	CFont	cust_font;

//////////////////////////////////////////////////////////////////////////
// Global Messages

#define		WM_DOCCHANGE			(WM_USER + 2000)

// Thread related 
//extern int		threadhandle;
void	RunWorkerThread(void *lpParam);
void	RunPreviewThread(void *lpParam);

extern	CMagWnd		magwin;

#define	STATE_WIDTH		0
#define	STATE_HEIGHT	1
#define	STATE_FULL		2		
#define	STATE_VARI		3
#define	STATE_FINE		4
#define	STATE_MUL		5

extern jmp_buf main_loop;

/////////////////////////////////////////////////////////////////////////////
// Shortcut codes:
//
//   Note: because the macros reference specific members, the code stays
//			relatively type safe
//


#define	GETMAINDIALOG(ppp)									\
		(ppp = ((CShadesDlg* )AfxGetApp()->m_pMainWnd));	\
		ASSERT(ppp); ASSERT(ppp->magic == SHADE_MAGIC);     \

#define GETCURRDOC(ddoc, dlg)								\
		ASSERT(dlg); ASSERT(dlg->magic == SHADE_MAGIC);     \
		ddoc = (CXrayDoc*)dlg->GetCurrDoc();				\

// Sorthand for the macros:

#define	GMD		GETMAINDIALOG
#define	GCD		GETCURRDOC
#define	GCDR	GETCURRDOCRET

#define	RET_IF_NODOC_MSG(ddd, sss)							\
	if(ddd->current < 0)									\
		{													\
		ShowStatusP(										\
		"Cannot %s without a document loaded.", sss);		\
		return;												\
		}													\

#define GETCURRDOCRET(ddoc, dlg)							\
		ASSERT(dlg); ASSERT(dlg->magic == SHADE_MAGIC);     \
		ddoc = (CXrayDoc*)dlg->GetCurrDoc(); 				\
		if(ddoc == NULL) return;							\
		ASSERT(ddoc->magic == DOC_MAGIC);					\

/////////////////////////////////////////////////////////////////////////////
// CShadesApp: see shades.cpp for the implementation of this class
//

class CShadesApp : public CWinApp

{
public:
	int TransKey(CString &str);
	CString TranslateAccel(int val);
	void LoadAccelFromMenu(int mid, CPtrArray *parr);
	void LoadAccelFromMenus();
	CShadesApp();
	~CShadesApp();

	void	Help(const char *str = "");

	HACCEL	xntaccel;

	int signal_init;
	int	inmodal;
	int forcesave;
	int dlgbackcol;
	int albino;
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShadesApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
	virtual BOOL SaveAllModified();
	virtual BOOL OnIdle(LONG lCount);
	virtual int Run();
	virtual LRESULT ProcessWndProcException(CException* e, const MSG* pMsg);
	virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CShadesApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHADES_H__10CE72D1_1678_4810_8D52_1EA02C2176E0__INCLUDED_)
