// ftext.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ftext.h"
#include "ftextDlg.h"
#include "../common/mxpad.h"
#include "../common/support.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFtextApp

BEGIN_MESSAGE_MAP(CFtextApp, CWinApp)
	//{{AFX_MSG_MAP(CFtextApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFtextApp construction

CFtextApp::CFtextApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CFtextApp object

CFtextApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CFtextApp initialization

#include "ExcRep.h"

BOOL CFtextApp::InitInstance()
{
	if (!AfxSocketInit())
		{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
		}

	//C2N();

	AfxEnableControlContainer();

	SetRegistryKey(_T("XrayNotes"));

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CFtextDlg *dlg = new CFtextDlg;
	
//again:
	m_pMainWnd = dlg;
	int nResponse;
	//try
		{
		nResponse = dlg->DoModal();
		}
	/*catch(...)
		{
			P2N("Exception on CFtextApp::InitInstance\r\n");
			// Re - create it 

			CExcRep rep; int ret = rep.DoModal();
			//AfxMessageBox("Error encountered, restarting Application, press OK to restart");

			if(ret >= 0)
				{
				dlg->EndDialog(IDOK);support.YieldToWinEx();
				delete dlg;	support.YieldToWinEx();	
				dlg = new CFtextDlg; support.YieldToWinEx();
				goto again;
				}
		}
*/
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

	delete dlg;
	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CFtextApp::ExitInstance() 

{	
	CWinApp::ExitInstance();
	return 0;
}
