// client.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "client.h"
#include "clientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClientApp

BEGIN_MESSAGE_MAP(CClientApp, CWinApp)
	//{{AFX_MSG_MAP(CClientApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClientApp construction

CClientApp::CClientApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CClientApp object

CClientApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CClientApp initialization

BOOL CClientApp::InitInstance()
{
	WSADATA wd;

	if (!AfxSocketInit(&wd))
		{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
		}

	P2N("Inited socket %d.%d %s %s\r\n", wd.wHighVersion / 256, wd.wHighVersion % 256, 
									wd.szDescription, wd.szSystemStatus);

	AfxEnableControlContainer();

	SetDialogBkColor(RGB(128,128,128));

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	//Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	//Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CClientDlg dlg;
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

typedef struct _ERROR_STRUCT 
	{
	int err;
	char *str;
	}	ERROR_STRUCT ;


struct _ERROR_STRUCT errlist[] = 

{     
	{WSAEINTR,           "WSAEINTR - Interrupted"},     
	{WSAEBADF, 			"WSAEBADF - Bad file number"},                
	{WSAEFAULT,   		"WSAEFAULT - Bad address"},               
	{WSAEINVAL,          "WSAEINVAL - Invalid argument"},               
	{WSAEMFILE,          "WSAEMFILE - Too many open files"},            
/* 
 *    Windows Sockets definitions of regular Berkeley error constants 
 */      
	{WSAEWOULDBLOCK,   	"WSAEWOULDBLOCK - Socket marked as non-blocking"},     
	{WSAEINPROGRESS,     "WSAEINPROGRESS - Blocking call in progress"},     
	{WSAEALREADY,        "WSAEALREADY - Command already completed"},     
	{WSAENOTSOCK,        "WSAENOTSOCK - Descriptor is not a socket"},     
	{WSAEDESTADDRREQ,    "WSAEDESTADDRREQ - Destination address required"},       
	{WSAEMSGSIZE,        "WSAEMSGSIZE - Data size too large"},          
	{WSAEPROTOTYPE,      "WSAEPROTOTYPE - Protocol is of wrong type for this socket"},        
	{WSAENOPROTOOPT,     "WSAENOPROTOOPT - Protocol option not supported for this socket type"},       
	{WSAEPROTONOSUPPORT, "WSAEPROTONOSUPPORT - Protocol is not supported"},     
	{WSAESOCKTNOSUPPORT, "WSAESOCKTNOSUPPORT - Socket type not supported by this address family"},     
	{WSAEOPNOTSUPP,      "WSAEOPNOTSUPP - Option not supported"},        
	{WSAEPFNOSUPPORT,    "WSAEPFNOSUPPORT - "},      
	{WSAEAFNOSUPPORT,    "WSAEAFNOSUPPORT - Address family not supported by this protocol"},     
	{WSAEADDRINUSE,      "WSAEADDRINUSE - Address is in use"},        
	{WSAEADDRNOTAVAIL,   "WSAEADDRNOTAVAIL - Address not available from local machine"},     
	{WSAENETDOWN,        "WSAENETDOWN - Network subsystem is down"},          
	{WSAENETUNREACH,     "WSAENETUNREACH - Network cannot be reached"},       
	{WSAENETRESET,       "WSAENETRESET - Connection has been dropped"},         
	{WSAECONNABORTED,    "WSAECONNABORTED - "},      
	{WSAECONNRESET,      "WSAECONNRESET - "},        
	{WSAENOBUFS,         "WSAENOBUFS - No buffer space available"},           
	{WSAEISCONN,         "WSAEISCONN - Socket is already connected"},           
	{WSAENOTCONN,        "WSAENOTCONN - Socket is not connected"},          
	{WSAESHUTDOWN,       "WSAESHUTDOWN - Socket has been shut down"},         
	{WSAETOOMANYREFS,    "WSAETOOMANYREFS - "},      
	{WSAETIMEDOUT,       "WSAETIMEDOUT - Command timed out"},         
	{WSAECONNREFUSED,    "WSAECONNREFUSED - Connection refused"},      
	{WSAELOOP,           "WSAELOOP - "},             
	{WSAENAMETOOLONG,    "WSAENAMETOOLONG - "},      
	{WSAEHOSTDOWN,       "WSAEHOSTDOWN - "},         
	{WSAEHOSTUNREACH,    "WSAEHOSTUNREACH - "},      
	{WSAENOTEMPTY,       "WSAENOTEMPTY - "},         
	{WSAEPROCLIM,        "WSAEPROCLIM - "},          
	{WSAEUSERS,          "WSAEUSERS - "},            
	{WSAEDQUOT,          "WSAEDQUOT - "},            
	{WSAESTALE,          "WSAESTALE - "},            
	{WSAEREMOTE,         "WSAEREMOTE - "},                                
/* 
 *    Extended Windows Sockets error constant definitions 
 */      
	{WSASYSNOTREADY,     "WSASYSNOTREADY - Network subsystem not ready"},       
	{WSAVERNOTSUPPORTED, "WSAVERNOTSUPPORTED - Version not supported"},     
	{WSANOTINITIALISED,  "WSANOTINITIALISED - WSAStartup() has not been successfully called"},  
/* 
 *    Other error constants. 
 */       
	{WSAHOST_NOT_FOUND,  "WSAHOST_NOT_FOUND - Host not found"},     
	{WSATRY_AGAIN,       "WSATRY_AGAIN - Host not found or SERVERFAIL"},     
	{WSANO_RECOVERY,     "WSANO_RECOVERY - Non-recoverable error"},     
	{WSANO_DATA,         "WSANO_DATA - (or WSANO_ADDRESS) - No data record of requested type"},     


{-1, NULL} 

}; 

//////////////////////////////////////////////////////////////////////////

char *sockerr2str(int err)

{
	// No error
	if(err == 0)
		return("no error");

	int idx = 0;
	
	// search for matching error code
	while(true)
		{
		// Terminal condition?
		if(errlist[idx].err == -1)
			return("unknown error");

		if(errlist[idx].err == err)
			return(errlist[idx].str);

		idx++;
		}

	// Not reached
	return "unkown error";
}

