// dbtest.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "dbtest.h"
#include "io.h"

#include "dbtestDlg.h"
#include "mxpad.h"
#include "support.h"
#include "sqlite3.h"
#include "DBMain.h"
#include "AddrEntry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString droot("c:\\temp\\");
CString wdroot("c:\\temp\\");
CString approot("c:\\temp\\");

/////////////////////////////////////////////////////////////////////////////
// CDbtestApp

BEGIN_MESSAGE_MAP(CDbtestApp, CWinApp)
	//{{AFX_MSG_MAP(CDbtestApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDbtestApp construction

CDbtestApp::CDbtestApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CDbtestApp object

CDbtestApp theApp;

lpfnSetLayeredWindowAttributes m_pSetLayeredWindowAttributes = NULL;


/////////////////////////////////////////////////////////////////////////////
// CDbtestApp initialization

BOOL CDbtestApp::InitInstance()

{
	C2N();
	
	// Here we import the function from USER32.DLL
	HMODULE hUser32 = GetModuleHandle(_T("USER32.DLL"));
	m_pSetLayeredWindowAttributes = 
                       (lpfnSetLayeredWindowAttributes)GetProcAddress(hUser32, 
                       "SetLayeredWindowAttributes");

	if (!AfxSocketInit())
		{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
		}

	AfxInitRichEdit();


	SetRegistryKey("DBTEST");
	srand(time(NULL));

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	//////////////////////////////////////////////////////////////////////

	P2N("Start application dbtest sql version %s\r\n", sqlite3_libversion());
	TRACE("Start application dbtest sql version %s\r\n", sqlite3_libversion());

	support.GetAppRoot(approot);
	P2N("Application code root: '%s' access=%d\r\n", approot, _access(approot, 2));

	support.GetAppDataDir(wdroot);
	P2N("Application data root: '%s' access=%d\r\n", wdroot, _access(wdroot, 2));

	//CString shareddir;
	support.GetSharedDocDir(droot);
	P2N("Shared doc root: '%s' access=%d\r\n", droot, _access(droot, 2));

	//////////////////////////////////////////////////////////////////////

	SetDialogBkColor(RGB(100,100,100));

	//CDbtestDlg dlg;
	CDBMain  dlg;
	//CAddrEntry dlg;
	
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
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

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////

void CDbtestApp::WinHelp(DWORD dwData, UINT nCmd) 
{
	support.Help();	

	//CWinApp::WinHelp(dwData, nCmd);
}
