
/* =====[ shades.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the shades.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  3/22/2007  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#include "stdafx.h"
#include "winuser.h"

#include <malloc.h>
#include <direct.h>
#include <stdio.h>
#include <io.h>
#include <stdlib.h>
#include <signal.h>
#include <setjmp.h>

#include "xraynotes.h"
#include "shades.h"
#include "shadesDlg.h"
#include "Splash.h"

#include "commline.h"
#include "support.h"
#include "msgbox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CSplash	  splash;

/////////////////////////////////////////////////////////////////////////////
// The one and only CShadesApp object

CShadesApp theApp;
	
CFont	cust_font;

///////////////////////////////////////////////////////////////////////////
// The one and only application object

CShadesDlg dlg;

///////////////////////////////////////////////////////////////////////////
// Global DATA

jmp_buf main_loop;
jmp_buf idle_loop;

CCommLine cline;

POINT	appmouse, appmouseclick;
HWND	appwnd;

int	gl_resamp_rough = CxImage::IM_NEAREST_NEIGHBOUR;
int	gl_resamp_fine = CxImage::IM_BILINEAR;

int	gl_animated = true;

int	xnt_clip = 0;
int	xnt_multi = 0;
int	xnt_bin = 0;

///////////////////////////////////////////////////////////////////////////
// Global Strings

CString appdata;
CString approot;
CString dataroot;
CString tmproot;
CString intialcwd;
CString shared_data;

CString lastseries;
CString lastdicom;

CString	appdirname("XrayNotes");
CString strConfig = "Remember";

/*!
	\mainpage

	\section Introduction

	This document describes the XRAYNOTES source code files. Links to the most important classes
	are provided below.

	\section qq Hot Keys
	
		ref keystrokes "Keystrokes and Hotkeys"

	\section qqq Important Classes

		CShadesApp

	\section qqqq Support Classes

		CxPtrArr MxPad

  	\section qqqqq PYTHON API

*/

//- \page index XrayNotes Documentation
//- This document describes the XRAYNOTES source code files.<p>
//- The following links help navigate the XrayNotes default
//- keystroke assignemnts.<p>
//- <li><linkhere altkeys Alt Keystroke Assignments>
//- <li><linkhere ctrlkeys Ctrl Keystroke Assignments>
//- <li><linkhere ctrlkeys Ctrl-Alt Keystroke Assignments>

int		SetupException()

{
	int ret;

	P2N("Setting up Exception\r\n");

	if(ret = setjmp(main_loop))
		{
		//support.YieldToWinEx();
		P2N("retried after violation acivity\r\n");
		
		CString str; str.Format("Command restart. Exception: %d", ret);

		xMsgBox("This operation attemted an illegal memory acess, restarting command. \r\n"
			"If this condition persist, please consult the program manual and/or your XrayNotes representative. \r\n", 
			str);

		//return;

		Sleep(1000);
		}

	return(0);
}

void _cdecl sigfunc( int sig)

{
	P2N("****** Signal %d\r\n", sig);
	
	// Re-arm signal
	//signal( sig, sigfunc);
	//jmp_buf  save;
	//memcpy(save, main_loop, sizeof(jmp_buf));
	//longjmp(save, sig);	
	
	//longjmp(main_loop, sig);

}

lpfnSetLayeredWindowAttributes g_pSetLayeredWindowAttributes = NULL;

/////////////////////////////////////////////////////////////////////////////
// CShadesApp

BEGIN_MESSAGE_MAP(CShadesApp, CWinApp)
	//{{AFX_MSG_MAP(CShadesApp)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_HELP, CShadesApp::WinHelp)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShadesApp construction

CShadesApp::CShadesApp()

{
	inmodal		= 0;
	signal_init = false;
	forcesave	= false;
	albino		= false;

	dlgbackcol		= RGB(100,100,100);

	// If one wants to use notepad as output
	//mxpad.SetAppname("Notepad");

	// If one wants to use it in release mode
	//mxpad.force = true;

	mxpad.Detect();
	
	C2N();

	//support.CheckPoint();
}

CShadesApp::~CShadesApp()

{
	//P2N("CShadesApp::~CShadesApp()\r\n");

	DestroyAcceleratorTable(xntaccel);

	//support.CheckPoint();
	//support.HeapDump("Exit");
	//support.ShowLeaks();
}

/////////////////////////////////////////////////////////////////////////////
// CShadesApp initialization

BOOL CShadesApp::InitInstance()

{
	WSADATA sock;

	if (!AfxSocketInit(&sock))
		{
		MBOX("IDP_SOCKETS_INIT_FAILED");
		return FALSE;
		}

#ifdef _DEBUG
	//afxTraceFlags |= 0x2;
#endif

	if(albino)
		dlgbackcol		= RGB(225, 255, 255);
	
	intialcwd = support.GetCurrentDir();
	support.GetAppRoot(approot);
	support.GetDataRoot(dataroot);
	support.GetAppData(appdata);
	support.GetSharedData(shared_data);
	support.GetTmpDir(tmproot);

	//SetupException();

	// Install signal handlers
	signal( SIGTERM, sigfunc);	
	signal( SIGINT, sigfunc);
	signal( SIGABRT, sigfunc);
	signal( SIGSEGV, sigfunc);
	signal( SIGBREAK, sigfunc);
	signal( SIGFPE, sigfunc);
	signal( SIGILL, sigfunc);

	// divide by null exception
	
	CTime ct = CTime::GetCurrentTime();
	CString datestr2 = ct.Format("%A, %B %d, %Y - %I:%M %p");

	//P2N("Inited socket subsystem %s\r\n", sock.szDescription);
	//P2N("Started %s\r\n", approot);
	
	char *aa;
	char longname[_MAX_PATH + 1];

	DWORD rr = GetFullPathName(
		tmproot,		// pointer to name of file to find path for
		MAX_PATH,		//tmproot.GetLength(); // size, in characters, of path buffer
		longname,		// pointer to path buffer
		&aa   // pointer to filename in path
		);

	OSVERSIONINFO ver;
	ver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&ver);

	P2N("\r\nApp started on Win %d.%d (%s) at %s\r\n\r\n", 
					ver.dwMajorVersion, ver.dwMinorVersion, ver.szCSDVersion, datestr2);
	P2N("aproot:        '%s'\r\n"
		"dataroot:      '%s'\r\n"
        "appdata:       '%s' \r\n"
		"tmproot:       '%s' \r\n"
		"shared_data:   '%s'\r\n\r\n", 

				approot, dataroot, 
				appdata, tmproot, shared_data);

	AfxEnableControlContainer();

	SetDialogBkColor(dlgbackcol);
	SetRegistryKey(_T("XrayNotes"));

	// Register clipboard formats
	xnt_clip  = RegisterClipboardFormat("XNT");
	xnt_multi = RegisterClipboardFormat("XNT_MUL");
	xnt_bin   = RegisterClipboardFormat("XNT_BIN");

	// Create font system
	CString	mainfont("MS Sans Serif");
	CString	docfont("Arial");

	//CString  strConfig = "DefaultFont";
	//CString fontname = 	GetProfileString(strConfig, "Font", docfont);
	
	//////////////////////////////////////////////////////////////////////
	// Load font defined in setup:

	CString  strConfig = "DefaultFont";

	CString fontname = 	GetProfileString(strConfig, "Font", docfont);
	int size    =  		GetProfileInt(strConfig,  "FontSize", -13);
	int weight  =  		GetProfileInt(strConfig, "FontWeight", 0);
	int italic  =  		GetProfileInt(strConfig, "FontItalic", 0);
	int	cset	=  		GetProfileInt(strConfig, "FontCharSet", ANSI_CHARSET);
	int	pitch	=  		GetProfileInt(strConfig, "FontPitch", FIXED_PITCH);
		
	if(!cust_font.CreateFont(size, 0, 0, 0, weight, italic, 0, 0,
				cset, 0, 0, 0, pitch, fontname))	
		{
		MBOX("Cannot set custom font, defaulting to system font.");
		}
	
	//P2N("Application data at %s\r\n", appdata);

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	HMODULE hUser32 = GetModuleHandle(_T("USER32.DLL"));
	g_pSetLayeredWindowAttributes = (lpfnSetLayeredWindowAttributes)
                        GetProcAddress(hUser32, "SetLayeredWindowAttributes");

	CTime ct2 = CTime::GetCurrentTime();
	CString datestr = ct2.Format("%A, %B %d, %Y - %I:%M %p");
	
	//P2N("Started Shades App at %s\r\n", datestr);

	support.HeapCheck("Initial", false);

	//////////////////////////////////////////////////////////////////////
	// Load accelerator

	LoadAccelFromMenus();

	m_pMainWnd = &dlg;

	// Does no work
	//if(GetKeyState(VK_CONTROL) & 0x80)
	//	dlg.resetpositions = true;

	// Instructions from command line 
	cline.ParseComandLine(AfxGetApp()->m_lpCmdLine);
	cline.InterpretComandLine();

	//////////////////////////////////////////////////////////////////////
	// Load remembered values

	//dlg.lastfilename = 	GetProfileString(strConfig, "LastFileName", "");
	//lastseries		= 	GetProfileString(strConfig, "LastSeries", "");

	//AfxGetMainWnd();

	//P2N("Loaded last file name %s\r\n", dlg.lastfilename);

	jmp_buf dlg_jump;
	int ret = setjmp(dlg_jump);

	//CException 

	//try
	{
	int nResponse;
	
	if(!ret)
		{
		nResponse = dlg.DoModal();	
		}
	else
		{
		P2N("Retry dialog \r\n");
		
		dlg.EndDialog(IDOK, 1);
		//dlg.DestroyWindow();

		nResponse = dlg.DoModal();
	
		//while(true)
		//	{
		//	if(dlg.ContinueModal() == false)
		//		break;
		//	dlg.RunModalLoop();
		//	//support.YieldToWinEx();
		//	}	
		}	

	if (nResponse == IDOK)
		{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
		}
	else if (nResponse == IDCANCEL)
		{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
		}
	else
		{
		//P2N("Dialog dismissed with error\r\n");
		}

	}
	//catch( ... )
	//	{
	//	P2N("Main Dialog Exception occured\r\n");
	//	
	//	xMsgBox("The XRAYOTES application has encountered an internal error. \r\n"
	//		"Your current data has been saved to the '_dump' subdirectory\r\n", "Segment Violation");
	//	
	//	//goto again;
	//	//longjmp(dlg_jump, 1);
	//	}
	//

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.

	return FALSE;
}


int CShadesApp::ExitInstance() 

{
	support.HeapCheck("At exit", false);

	CTime ct = CTime::GetCurrentTime();
	CString datestr = ct.Format("%A, %B %d, %Y - %I:%M %p");

	P2N("Exiting App at %s\r\n", datestr);

	return CWinApp::ExitInstance();
}

CDocument* CShadesApp::OpenDocumentFile(LPCTSTR lpszFileName) 

{
	P2N("Opendocumentfile\r\n");
	return CWinApp::OpenDocumentFile(lpszFileName);
}

//////////////////////////////////////////////////////////////////////////
// Performace counter routines

static	LARGE_INTEGER Frequency, PerformanceCount;
static	int	init_perf = 0;

int		mark_perf()

{
	if(!init_perf)
		{
		init_perf = true;
		QueryPerformanceFrequency(&Frequency);

		//P2N("freq=%d - %d\r\n", Frequency.HighPart, Frequency.LowPart);
		}

	QueryPerformanceCounter(&PerformanceCount);
	//P2N("count = %d - %d\r\n",	PerformanceCount.HighPart, PerformanceCount.LowPart);

	return(0);
}

/////////////////////////////////////////////////////////////////////////////
// Get number of milliseconds since last call to mark

int		query_perf()

{
	LARGE_INTEGER lastcount;
	LARGE_INTEGER lastcount2;

	QueryPerformanceCounter(&lastcount);
	QueryPerformanceCounter(&lastcount2);

	int elapsed = ((lastcount.LowPart - PerformanceCount.LowPart) * 1000) / Frequency.LowPart;

	// Just to check latency (can be ignored in msec range)
	//int elapsed2 = ((lastcount2.LowPart - PerformanceCount.LowPart) * 1000) / Frequency.LowPart;
	//P2N("Time elapsed: %d msec elapsed2: %d msec\r\n", elapsed, elapsed2); 

	return(elapsed);
}

//include "xPtrArr.h"
//include "persistdlg.h"

///////////////////////////////////////////////////////////////////////////////
BOOL CShadesApp::SaveAllModified() 

{
	int ret = true;
	//P2N("CShadesApp::SaveAllModified()\r\n");
				
	CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);
	ASSERT(dlg->magic == SHADE_MAGIC);
	
	// Close modal dialogs
	for (int loopm = 0; loopm < PersistDlg::modals.GetSize(); loopm++)
		{
		if(PersistDlg::modals[loopm])
			P2N("dumping modal %p %s\r\n", PersistDlg::modals[loopm],
				((PersistDlg*)PersistDlg::modals[loopm])->windowname);
		}
	
	// Iterate save cycle
	int ndoc = dlg->docarr.GetSize();
	int retry = 0;

	for(int loop = 0; loop < ndoc; loop++)	
		{
		CXrayDoc *doc = (CXrayDoc*)dlg->docarr[loop];
	
		// Stop all pending processing
		if(doc->thhandle)
			{
			//P2N("CShadesApp::SaveAllModified - Aborted processing\r\n");
			doc->img->SetEscape(-1);	
			doc->img->Copy(*doc->org);
			//P2N("Aborted DSP processing\r\n");
			support.YieldToWinEx();
			}
		
		if(doc->changed)
			{
			if(!forcesave)
				{
				// So this will become a parent to the msgbox
				dlg->SetFocus(); support.YieldToWinEx();

				CString str; str.Format("Save changes to: %s ", ((CXrayDoc*)dlg->docarr[loop])->filename);
				int mret = xMsgBox(str, "Save File?", MBTYPE_YESNOSTOP, true);

				switch (mret)
					{
					case true:
						P2N("Saving %s\r\n", doc->filename);
						if(doc->img->Save(doc->filename, FindImageType(doc->filename)) == 0)
							MBOXP("Cannot Save Image %s", doc->filename);
						break;

					case false:
						P2N("Save anwered NO\r\n");
						break;

					case -1:
						P2N("Save aborted\r\n");
						ret = false; goto endd;
						break;
					}
				}
			else
				{
				P2N("Shutdown Save Forced on %s\r\n", doc->filename);
				doc->img->Save(doc->filename, FindImageType(doc->filename));
				}
			//return true;
			}
		}	
	
endd:
	//P2N("CShadesApp::SaveAllModified End.\r\n");
	//return CWinApp::SaveAllModified();

	return ret;
}

BOOL CShadesApp::OnIdle(LONG lCount) 

{
	P2N("Calling Idle\r\n");

	if(!signal_init)
		{
		P2N("Setting up Exception\r\n");
		signal_init = true;
		
		int ret;
		if(ret = setjmp(main_loop))
			{
			//support.YieldToWinEx();
			P2N("retried after violation acivity\r\n");
			
			CString str; str.Format("Command restart. Exception: %d", ret);

			xMsgBox("This operation attemted an illegal memory acess, restarting command. \r\n"
				"If this condition persist, please consult the program manual and/or your XrayNotes representative. \r\n", 
				str);

			//return;

			Sleep(1000);
			}
		}
	return CWinApp::OnIdle(lCount);
}

int CShadesApp::Run() 

{
	P2N("Calling CShadesApp::Run\r\n");	
	return CWinApp::Run();
}

LRESULT CShadesApp::ProcessWndProcException(CException* e, const MSG* pMsg) 

{
	P2N("Exception occured\r\n");	

	CString str;

	char emsg[_MAX_PATH + 1];

	e->GetErrorMessage(emsg, _MAX_PATH);

	str.Format(
		"Invalid operation attempted:\r\n"
		"Description: '%s'\r\n"
		"Message: '%s'\r\n"
		"Please make note of the last operation and the error description,\r\n"
				"as it may be instrumental in troubleshooting your particular error situation.\r\n"
					"\r\nCurrent operation is aborted, continuing program execution.\r\n",
						emsg, mxpad.num2msg(pMsg->message));

						//, file , line);

	//"Example: Divide by Zero or NULL Pointer Dereference in\r\n"
			
	xMsgBox(str, "Invalid Operation Attempted");

	//return CWinApp::ProcessWndProcException(e, pMsg);

	return 0;
}

void	ReportException(const char *file, int line)

{
	CString str;

	str.Format(
		"An invalid operation was attempted in \r\n'%s' Line number: %d \r\n"
			"Please make note of the file name / line number. "
				" It may be instrumental in troubleshooting the error.\r\n"
					"\r\nCurrent operation is aborted, continuing program execution.\r\n", 
						file , line);

	//"Example: Divide by Zero or NULL Pointer Dereference in\r\n"
			
	xMsgBox(str, "Invalid Operation Attempted");

}

//////////////////////////////////////////////////////////////////////////
//
// Function name:    FindImageType
// Description:      Return CXIMAGE filetype from filename
// Return type:      int
// Argument:         const CString& name full fle name
//
//////////////////////////////////////////////////////////////////////////

int		FindImageType(const CString& name)

{
	// Get extension
	CString drive, dir, fname, ext;

	support.SplitPath(name, drive, dir, fname, ext);

	ext.MakeLower();

    int type = -1;

    if (ext == ".bmp")                   type = CXIMAGE_FORMAT_BMP;
#if CXIMAGE_SUPPORT_JPG
    else if (ext==".jpg"||ext==".jpeg")   type = CXIMAGE_FORMAT_JPG;
#endif
#if CXIMAGE_SUPPORT_GIF
    else if (ext == ".gif")              type = CXIMAGE_FORMAT_GIF;
#endif
#if CXIMAGE_SUPPORT_PNG
    else if (ext == ".png")              type = CXIMAGE_FORMAT_PNG;
#endif
#if CXIMAGE_SUPPORT_MNG
    else if (ext==".mng"||ext==".jng")    type = CXIMAGE_FORMAT_MNG;
#endif
#if CXIMAGE_SUPPORT_ICO
    else if (ext == ".ico")              type = CXIMAGE_FORMAT_ICO;
#endif
#if CXIMAGE_SUPPORT_TIF
    else if (ext==".tiff"||ext==".tif")   type = CXIMAGE_FORMAT_TIF;
#endif
#if CXIMAGE_SUPPORT_TGA
    else if (ext==".tga")                type = CXIMAGE_FORMAT_TGA;
#endif
#if CXIMAGE_SUPPORT_PCX
    else if (ext==".pcx")                type = CXIMAGE_FORMAT_PCX;
#endif
#if CXIMAGE_SUPPORT_WBMP
    else if (ext==".wbmp")               type = CXIMAGE_FORMAT_WBMP;
#endif
#if CXIMAGE_SUPPORT_WMF
    else if (ext==".wmf"||ext==".emf")    type = CXIMAGE_FORMAT_WMF;
#endif
#if CXIMAGE_SUPPORT_J2K
    else if (ext==".j2k"||ext==".jp2")    type = CXIMAGE_FORMAT_J2K;
#endif
#if CXIMAGE_SUPPORT_JBG
    else if (ext==".jbg")                type = CXIMAGE_FORMAT_JBG;
#endif
#if CXIMAGE_SUPPORT_JP2
    else if (ext==".jp2"||ext==".j2k")    type = CXIMAGE_FORMAT_JP2;
#endif
#if CXIMAGE_SUPPORT_JPC
    else if (ext==".jpc"||ext==".j2c")    type = CXIMAGE_FORMAT_JPC;
#endif
#if CXIMAGE_SUPPORT_PGX
    else if (ext==".pgx")                type = CXIMAGE_FORMAT_PGX;
#endif
#if CXIMAGE_SUPPORT_RAS
    else if (ext==".ras")                type = CXIMAGE_FORMAT_RAS;
#endif
#if CXIMAGE_SUPPORT_PNM
    else if (ext==".pnm"||ext==".pgm"||ext==".ppm") type = CXIMAGE_FORMAT_PNM;
#endif

    //else if (ext==".xnt")                type = CXIMAGE_FORMAT_XNOTES;
    //else if (ext==".xt")                 type = CXIMAGE_FORMAT_XNOTES;

    else type = CXIMAGE_FORMAT_UNKNOWN;

    //P2N("Findtype ret=%d\r\n", type);

    return type;
}

void CShadesApp::WinHelp(DWORD dwData, UINT nCmd) 
{
	//Help();		
	//CWinApp::WinHelp(dwData, nCmd);
}

//////////////////////////////////////////////////////////////////////////

void	CShadesApp::Help(const char *str)

{
	// Execute winhelp
	CString str2(approot), path("umanual\\"), file("index.html");

	if(str != NULL && str[0] != '\0')
		{
		file = str;
		}

#ifdef _DEBUG
	str2 = "..\\install\\" + path  + file;
#else
	str2 =  approot + path + file;
#endif

	//P2N("Called Winhelp %s\r\n", str2);

	ShellExecute(NULL, "open", str2, NULL, NULL, SW_SHOWMAXIMIZED);
}

// Capture messages from the application for global statistics and control

BOOL CShadesApp::PreTranslateMessage(MSG* pMsg) 

{
#if 1
	if(pMsg->message == WM_KEYDOWN)
		{
		appwnd = GetActiveWindow();
		//P2N("CShadesApp::PreTranslateMessage WM_KEYDOWN %c (%d) win=%x\r\n", 
		//						pMsg->wParam, pMsg->wParam, appwnd);
		}

	if(pMsg->message == WM_SYSKEYDOWN)
		{
		appwnd = GetActiveWindow();

		static DWORD oldkey;

		if(pMsg->wParam != oldkey)
			{
			oldkey = pMsg->wParam;
			//P2N("CShadesApp::PreTranslateMessage WM_SYSKEYDOWN %c (%d) win=%x\r\n", 
			//					pMsg->wParam, pMsg->wParam, appwnd);
			}
		}

	if(pMsg->message == WM_SYSKEYUP)
		{
		appwnd = GetActiveWindow();

		static DWORD oldkey;

		if(pMsg->wParam != oldkey)
			{
			oldkey = pMsg->wParam;
			//P2N("CShadesApp::PreTranslateMessage WM_SYSKEYUP %c (%d) win=%x\r\n", 
			//					pMsg->wParam, pMsg->wParam, appwnd);
			}
		}

	if(pMsg->message == WM_LBUTTONDOWN)
		{
		// Save last mouse click details:
		appwnd = GetActiveWindow();
		appmouseclick.x = LOWORD(pMsg->lParam);  
		appmouseclick.y = HIWORD(pMsg->lParam);  

		::ClientToScreen(pMsg->hwnd, &appmouseclick);

		//P2N("CShadesApp::PreTranslateMessage WM_LBUTTONDOWN %x x=%d y=%d win=%x\r\n", 
		//						pMsg->wParam, appmouseclick.x,  appmouseclick.y, appwnd);
		}

	/////////////////////////////////////////////////////////////////////////////

	if(pMsg->message == WM_MOUSEMOVE)
		{
		// Save last mouse move details:
		appwnd = GetActiveWindow();
		appmouse.x = LOWORD(pMsg->lParam);  
		appmouse.y = HIWORD(pMsg->lParam);  

		::ClientToScreen(pMsg->hwnd, &appmouse);

		//P2N("CShadesApp::PreTranslateMessage WM_MOUSEMOVE %x x=%d y=%d win=%x\r\n", 
		//						pMsg->wParam, appmouse.x,  appmouse.y, appwnd);
		}

#endif

	int ret = CWinApp::PreTranslateMessage(pMsg);

	// Do Hotkeys AFTER the pretranslation took place

	if(!inmodal)
		{
		if(TranslateAccelerator(GetActiveWindow(), xntaccel, pMsg))
			return true;
		}

	return ret;
}

void CShadesApp::LoadAccelFromMenu(int mid, CPtrArray *parr)

{
	CXrayM *xxmenu = new CXrayM;	
	xxmenu->LoadMenu(mid, 1);		

	for(int loop = 0; loop < xxmenu->arr.GetSize(); loop++)
		{
		CXrayMitem *item = (CXrayMitem*)xxmenu->arr[loop];

		if(item->command && item->key[0] != '\0')
			{
			CString ll, rr, tmp(item->key);

			tmp.Replace("(", ""); tmp.Replace(")", "");
			int idx = tmp.ReverseFind('-');

			if(idx >= 0)
				{
				ll = tmp.Left(idx);
				rr = tmp.Mid(idx + 1);
				}

			// Process entries
			rr.TrimLeft(); rr.TrimRight();	ll.MakeUpper();
			
			ACCEL	*acc = new ACCEL;
			memset(acc, 0, sizeof(ACCEL));

			// Fill in accelerator commands
			acc->cmd = item->command;
			acc->fVirt = FVIRTKEY; //FNOINVERT;
		
			if(ll.Find("ALT") >= 0)
				acc->fVirt |= FALT;

			if(ll.Find("CTRL") >= 0)
				acc->fVirt |= FCONTROL;

			if(ll.Find("SHIFT") >=0)
				acc->fVirt |= FSHIFT;

			acc->key = TransKey(rr);
	
			//P2N("Menu command: %d virt=0x%-2x key: '%s' '%s' '%s' %d\r\n", 
			//				item->command, acc->fVirt, tmp, ll, rr, acc->key);

			// Add only if unique:
			int found = false;
			for(int loop4 = parr->GetSize() - 1; loop4 >= 0;  loop4--)
				{
				ACCEL *aitem = (ACCEL*) (*parr)[loop4];

				if((aitem->fVirt == acc->fVirt) && (aitem->key == acc->key))
					{
					found = true;
					//if(item->cmd != acc->cmd)
						//{
						//P2N("Duplicate accel on  CMD %d (%x) %s %c -- '%s'\r\n", 
						//		aitem->cmd, acc->cmd, 
						//			TranslateAccel(aitem->fVirt), aitem->key,
						//				item->str);
						//}
					}
				}

			if(!found)
				parr->Add(acc);
			else
				delete acc;
			}
		}
	delete xxmenu;
}

//////////////////////////////////////////////////////////////////////////

void CShadesApp::LoadAccelFromMenus( )

{
	//P2N("CShadesApp::LoadAccelFromMenus()\r\n");

	return;

	HACCEL tmpaccel	= LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_ACCELERATOR)); 

	CPtrArray accarr, orgarr, sumarray;

	// Grab from resource
	int alen =  CopyAcceleratorTable(tmpaccel, NULL, 0);
	ACCEL *dptr = new ACCEL[alen]; ASSERT(dptr);
	CopyAcceleratorTable(tmpaccel, dptr, alen);
	
	//P2N("Get old table:\r\n");
	for(int loop44 = 0; loop44 < alen; loop44++)
		{
		ACCEL *item = (ACCEL*) &dptr[loop44];
		orgarr.Add(item);

		//P2N("command=%d (%x) flag=%x %s %s %s key=%c \r\n", 
		//				item->cmd, item->cmd, item->fVirt, 
		//				item->fVirt & FALT ? "ALT":"",
		//				item->fVirt & FCONTROL ? "CONTROL":"",
		//				item->fVirt & FSHIFT ? "SHIFT":"",
		//				item->key);
		}
	//P2N("end\r\n");
	sumarray.Append(orgarr);

	LoadAccelFromMenu(IDR_MENU1, &accarr);
	LoadAccelFromMenu(IDR_MENU2, &accarr);
	LoadAccelFromMenu(IDR_MENU3, &accarr);
	LoadAccelFromMenu(IDR_MENU4, &accarr);
	LoadAccelFromMenu(IDR_MENU5, &accarr);
	LoadAccelFromMenu(IDR_MENU6, &accarr);
	LoadAccelFromMenu(IDR_ADDR_MENU, &accarr);

	// Create real accel table malloc
	//ACCEL *torg, *tptr; 
	//torg = tptr = (ACCEL *)malloc(accarr.GetSize() * sizeof(ACCEL));
	//for(int loop4 = 0; loop4 < accarr.GetSize(); loop4++)
	//	memcpy(tptr++, accarr[loop4], sizeof(ACCEL));
	//D2N(torg, 128);

	sumarray.Append(accarr);

	// Create real accel table with new
	ACCEL *aptr = new ACCEL[sumarray.GetSize()];
	for(int loop4 = 0; loop4 < sumarray.GetSize(); loop4++)
		aptr[loop4] = *((ACCEL*) sumarray[loop4]);
	
	//D2N(aptr, accarr.GetSize() * sizeof(ACCEL));

	for(int loop5 = 0; loop5 < sumarray.GetSize(); loop5++)
		{
		ACCEL *item = (ACCEL*) &aptr[loop5];
	
		//P2N("command=%d (%x) flag=%d %s %s %s key=%c \r\n", 
		//				item->cmd, item->cmd, item->fVirt, 
		//				item->fVirt & FALT ? "ALT":"",
		//				item->fVirt & FCONTROL ? "CONTROL":"",
		//				item->fVirt & FSHIFT ? "SHIFT":"",
		//				item->key);
		}	

	xntaccel = CreateAcceleratorTable(aptr, sumarray.GetSize());
	
	for(int loop2 = accarr.GetSize() - 1; loop2 >= 0;  loop2--)
		{
		delete (ACCEL*)accarr[loop2];
		}

	delete aptr;
	delete dptr;
}

CShadesDlg *GetMainDlgWin()

{
	CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);

	ASSERT(dlg);
	ASSERT(dlg->magic == SHADE_MAGIC);

	return dlg;
}

CString CShadesApp::TranslateAccel(int val)

{
	CString str;

	str += val & FALT ? "ALT" : "";
	str += val & FCONTROL ? "CONTROL" : "";
	str += val & FSHIFT ? "SHIFT" : "";

	return str;
}

int CShadesApp::TransKey(CString &str)

{
	int ret = 0;

	// Most likely a letter or a number 
	if(str.GetLength() == 1)
		ret = (int)str[0];
	else if(str.Find("F1") >= 0)
		ret = VK_F1;
	else if(str.Find("F2") >= 0)
		ret = VK_F2;
	else if(str.Find("F3") >= 0)
		ret = VK_F3;
	else if(str.Find("F4") >= 0)
		ret = VK_F4;
	else if(str.Find("F5") >= 0)
		ret = VK_F5;
	else if(str.Find("F6") >= 0)
		ret = VK_F6;
	else if(str.Find("F7") >= 0)
		ret = VK_F7;
	else if(str.Find("F8") >= 0)
		ret = VK_F8;
	else if(str.Find("F9") >= 0)
		ret = VK_F9;
	else if(str.Find("F10") >= 0)
		ret = VK_F10;
	else if(str.Find("F11") >= 0)
		ret = VK_F11;
	else if(str.Find("F12") >= 0)
		ret = VK_F12;

	//P2N("Translated key: '%s'\r\n", mxpad.vkey2keystr(ret));

	return ret;
}


//////////////////////////////////////////////////////////////////////////
// Pop up a message box pertaining into the last current window
// for centering

int		xMsgBox(const char *str, const char *title, int yesno, int defa) 

{
	CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);
	ASSERT(dlg->magic == SHADE_MAGIC);

	CWnd *oldw = dlg->FromHandle(GetForegroundWindow());

	//P2N("Forground window %p %p\r\n", GetForegroundWindow(), oldw);
	
	CMsgBox mb(oldw);

	if(title == NULL || *title == '\0')
		mb.m_header = "XrayNotes MessageBox";
	else
		mb.m_header = title;

	mb.m_text = str;
	mb.yesno = yesno;
	mb.idret = defa;

	return mb.DoModal();
}



#include "Status.h"

extern CStatus     *statusdlg;

//////////////////////////////////////////////////////////////////////////
// Show status window

void    ShowStatus(const char *str, int prog, int tme)

{
   // HWND wnd =  ::GetFocus();
    
	//support.YieldToWinEx();
	HideStatus(); 

    statusdlg->showprog = prog;
	statusdlg->Show(str, tme);
    
	//support.YieldToWinEx(statusdlg->m_hWnd);
	support.YieldToWinEx();

	//::SetFocus(wnd);
}

void	ShowStatusP(const char *Format, ...)

{
	CString str; 
	char *buff = str.GetBuffer(_MAX_PATH);
    va_list ArgList; va_start(ArgList, Format);
    _vsnprintf(buff, _MAX_PATH, Format, ArgList);
	str.ReleaseBuffer();
    ShowStatus(str);
}

void        HideStatus()

{
    //HWND wnd =  ::GetFocus();
    statusdlg->Hide();
    //::SetFocus(wnd);
    support.YieldToWinEx(statusdlg->m_hWnd);
}

//////////////////////////////////////////////////////////////////////////

static char    asztmp[512];

int     AfxPrintf(const char *Format, ...)

{
    char    *asc = NULL;
    va_list ArgList;

    va_start(ArgList, Format);
    _vsnprintf(asztmp, sizeof(asztmp), Format, ArgList);

    MBOX(asztmp);

    return(0);
}

#include "about.h"

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
    //{{AFX_DATA_INIT(CAboutDlg)
    //}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAboutDlg)
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
    //{{AFX_MSG_MAP(CAboutDlg)
        // No message handlers
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


