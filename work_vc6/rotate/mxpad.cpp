
/* =====[ mxpad.c ]=========================================================

    MAIN description:

        Access library for debug pad. Converted to a class to allow access 
		from MFC library.

    Revisions:

      REV     DATE     BY           DESCRIPTION
      ----  --------  ----------    --------------------------------------
      0.00  03/09/99  Peter Glen    Initial version.
	  0.00  03/20/99  Peter Glen    MFC
	  0.00  08/24/05  Peter Glen    Class conversion
	  0.00  22/22/22  Peter Glen    Remote deployment
	  0.00  22/22/22  Peter Glen    Fixed overflow and defines
	
   ======================================================================= */

/* -------- System includes:  -------------------------------------------- */

#include "stdafx.h"
#include <windows.h>
#include <windowsx.h>
#include <lm.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <dos.h>
#include <time.h>
#include <string.h>
#include <process.h>

#include "mxpad.h"

#define APPNAME "MxPad"
#define mxMBOX(aa)  MessageBox(NULL, aa, "", MB_OK)

#define	DUMPLEN	32000
#define	OUTLEN	4096

//define	OUTLEN	32		// for testing

#define CHUNK 16

// Declare one instance, visible globally
class MxPad mxpad;

/////////////////////////////////////////////////////////////////////
#ifdef AfxGetApp

void	MxPad::RunMxThread(void *lpParam)

{
	MxPad  *ptr = (MxPad *)lpParam;
	CString str;

	// Wait till main window is UP
	while (true)
		{
		if(AfxGetApp()->m_pMainWnd)
			if(AfxGetApp()->m_pMainWnd->m_hWnd)
				if(IsWindow(AfxGetApp()->m_pMainWnd->m_hWnd))
					break;

		Sleep(333);
		}

	while(true)
		{
		//P2N("Looping MxThread\r\n");
		
		str = "";

		while(true)
			{
			char chh;

			if(!ptr->circ.Get(&chh))
				{
				P2N(str);
				break;
				}
			str += chh;
			}
	
        //YieldToWinEx();

		if(ptr->endthread)
			break;

		// Ease up on the processor ...
        Sleep(333);

		if(ptr->endthread)
			break;
		}

	//P2N("Ended circular buffer Thread\r\n");

	 _endthread();
 
    return;
}

#endif

HWND    MxPad::child;


BOOL    CALLBACK MxPad::EnumChildProc(HWND  hwnd, LPARAM  lParam)

{
    child = hwnd;
    return(0);
}

HWND    MxPad::GetChild(HWND  hwndParent)

{
	child = NULL;

    EnumChildWindows(
                hwndParent,         // handle of parent window
                EnumChildProc,      // address of callback function
                0                   // application-defined value
               );

    return(child);
}

SOCKADDR_IN MxPad::rec_sin2;

/*
 * Only one of these will run, no access control needed.
 */

DWORD	MxPad::AcceptThreadProc( DWORD *ptp )

{
    char	nd  = 1;
	int		ndlen  = sizeof(char);

    SOCKADDR_IN rec_sin;

    int     rec_sin_len = sizeof(rec_sin2);
    int     ret_val;
    char *str;
    SOCKET sock2, sock3;

    //mxMBOX("Hello thread %d");

    // --------------------------------------------------------------
    // Listen for server answer back:

    sock2 = socket( AF_INET, SOCK_STREAM, 0);
    //printf("After sock2.\n");

    //ret_val = setsockopt (sock2, SOL_SOCKET, SO_DONTLINGER, &nd, sizeof(int));
    //printf("After ling sockoption %d\n", ret_val);

    ret_val = getsockopt (sock2, SOL_SOCKET, SO_DONTLINGER, &nd, &ndlen);
    
	//printf("After ling getsockoption %d ---  %d\n", ret_val, nd);

    rec_sin.sin_addr.s_addr = INADDR_ANY;
    rec_sin.sin_port = htons(2001);
    rec_sin.sin_family = AF_INET;

    if (bind( sock2, (struct sockaddr FAR *) &rec_sin, sizeof(rec_sin))
        == SOCKET_ERROR)
        {
        //sprintf(szBuff, "%d is the error", WSAGetLastError());

        mxMBOX("Cannot Bind2 socket\n");
        //exit(0);
        }

    //printf("After bind2.\n");

    if (listen( sock2, 1) < 0)
        {
        mxMBOX("Cannot listen on socket2\n");
        //exit(0);
        }

    //printf("After listen2.\n");

    rec_sin_len = sizeof(rec_sin2);

    sock3 = accept( sock2,(struct sockaddr FAR *) &rec_sin2,
            (int FAR *) &rec_sin_len );

    str = inet_ntoa( *((struct in_addr*) &rec_sin2.sin_addr.s_addr));
    //mxMBOX(str);

    closesocket(sock3); closesocket(sock2);

    //("End thread.\n");

    //fflush(stdout);

    // Release crap

    *ptp = 1;
    return(0); 
}

/// Clear Debug Pad 

void     MxPad::ClearNotepad()

{
	HWND    chld;

	 if((chld = GetChild(FindWindow(appname, NULL)) ))
		{
		Edit_SetSel(chld, 0, -1); 
        Edit_ReplaceSel(chld, "");
		}
}
//////////////////////////////////////////////////////////////////////////

MxPad::~MxPad()

{
	//P2N("MxPad::~MxPad Freeing mem\r\n");

	// Stop the async thread, let it breathe
	endthread = true;	Sleep(100);

	if(dump_str) 
		free(dump_str);

	if(szOutString) 
		free(szOutString);

	if(aszOutString) 
		free(aszOutString);
}

//////////////////////////////////////////////////////////////////////////

MxPad::MxPad()

{
	nd = 1;

	strcpy(appname,  TEXT(APPNAME));

	dump_str = szOutString = NULL;

	dump_str = (char *) malloc(DUMPLEN);
	//memset(dump_str, 'a', DUMPLEN);

	szOutString = (char *) malloc(OUTLEN);
	//memset(szOutString, 'a', OUTLEN);

	aszOutString = (char *) malloc(OUTLEN);
	//memset(szOutString, 'a', OUTLEN);
	//ASSERT(aszOutString);

	ndlen = sizeof(int);
	connected = false;
	resolved = false;
	local = true;
	remote = false;
	trace = false;

	handle = 0;
	endthread = false;

	force = false;
}

//////////////////////////////////////////////////////////////////////////
/// \brief Just like print, but prints to debug pad

int     MxPad::printf(const char *Format, ...)

{
	#ifndef _DEBUG
		if(!force)
			return (0);
	#endif

#ifdef		AfxGetApp

	// Start thread if none
	if(!handle)
		{
		handle = _beginthread(RunMxThread, 0, this);
		}

#endif

	memset(szOutString, 0, OUTLEN);

	int len;
	try
		{
		va_list ArgList; va_start(ArgList, Format);

		len = _vsnprintf(szOutString, OUTLEN - 1, Format, ArgList);

		// Terminate if overflowed
		if(len < 0)	
			{
			szOutString[OUTLEN - 1] = '\0';
			ToNotepad("\r\nMxpad encountered a buffer overflow.\r\n");
			}

		ToNotepad(szOutString);
		}

	catch(...)
		{
		ToNotepad("\r\n"
				  "----------------------------------------------------------------------------\r\n");
		ToNotepad("Exception caught on P2N. Possible mismatching of FORMAT str vs. FORMAT arg\r\n");
		ToNotepad("Here is what we got so far: (between [])\r\n[");
		
		ToNotepad(szOutString);
		
		ToNotepad("]\r\nException End");
		ToNotepad("\r\n"
			  	  "----------------------------------------------------------------------------\r\n");
		ToNotepad("\r\n");
		}

	//if(len < 0)	
	//	{
	//	ToNotepad("\r\nWarning: Output string was cut to prevent buffer overflow.\r\n");
	//	}

    return(0);
}

///////////////////////////////////////////////////////////////////////////
/// \brief Just like hexdump, but prints to debug pad

int     MxPad::dump(const void *mem, int len)

{
    char    *dump_ptr = dump_str;
    char    *ptr = (char *)mem;
    int     slen = 0;
	int		chunk = CHUNK;

	#ifndef _DEBUG
		if(!force)
			return (0);
	#endif

    if (!mem)
        {
        ToNotepad("DumpMX: NULL pointer\r\n"); 
		return(0);
        }

	if(!len)
		{
        ToNotepad("DumpMX: Buffer with zero length.\r\n"); 
		return(0);
        }

    while(TRUE)
        {
        int aa;

        if(len <= 0)                                  /* end of buffer */
            break;

		 if(slen >=  DUMPLEN - 96)				/* end of local buffer */
			{
			slen += sprintf(dump_str + slen, "Warning: dump string was cut to prevent dump buffer overflow.\r\n");
            break;
			}

        slen += sprintf(dump_str + slen, "  %08p  -  ", ptr);

        for(aa = 0; aa < chunk; aa++)
			{
			if(aa == 4)
				slen += sprintf(dump_str + slen, " ");

			if(aa == 8)
				slen += sprintf(dump_str + slen, "  ");

			if(aa == 12)
				slen += sprintf(dump_str + slen, " ");

            slen += sprintf(dump_str + slen, "%02x ", *(ptr+aa) & 0xff);

			}

		if(chunk < CHUNK)
			{
	        for(aa = 0; aa < CHUNK - chunk; aa++)
				slen += sprintf(dump_str + slen, "   ");
			}

        slen += sprintf(dump_str + slen, "  -  ");

        for(aa = 0; aa < chunk; aa++)
            {
            char chh = *(ptr+aa);

            if(!isprint((int)chh))                 /* convert to ASC if not prt */
                chh = '.';

			if(chh == '\b')
				chh = '.';

            slen += sprintf(dump_str + slen, "%c", chh);
            }

		// Do Second line
		//slen += sprintf(dump_str + slen, "\r\n            ");

		//for(aa = 0; aa < chunk/2; aa++)
          //  {
            //WORD ww = * (((WORD*)ptr) + aa);
//
//			slen += sprintf(dump_str + slen, "  %04x", ww & 0xffff);
//			}

		if(chunk < CHUNK)
			{
	        for(aa = 0; aa < CHUNK - chunk; aa++)
				slen += sprintf(dump_str + slen, " ");
			}

        slen += sprintf(dump_str + slen, "  - \r\n");
        
		
        len -= chunk; ptr += chunk;

		if(len < chunk)
			chunk = len;
        }

    ToNotepad(dump_str);

    return(0);
}

/// Set local output flag

void	MxPad::SetLocal(int flag)

{
	local = flag;
}

/// Set remote output flag

void	MxPad::SetRemote(int flag)

{
	remote = flag;
}

// --------------------------------------------------------------
// Return true if local agent is found

/// \brief   Return true if local Mxpad agent is found.
/// 
/// \details This function will test if the local agent has started. 
/// That way the app can elect to test ONCE and never to try again.
/// \n  
/// Return type:      int  
/// \n  

int     MxPad::IsLocalAvail()

{
	int ret = 0;
	ret = (int)FindWindow(appname, NULL);
	return ret;
}

/// Return true if remote agent is found
	
int     MxPad::IsRemoteAvail()

{
	#ifdef _DEBUG
		return 0;
	#endif

	return Resolve();
}

//////////////////////////////////////////////////////////////////////////
/// Set Trace flag. Will output into C++ debug window instead. 
/// Returns old trace flag.

int		MxPad::SetTrace(int flag)

{
	int old = trace;
	trace = flag;
	return old;
}

//////////////////////////////////////////////////////////////////////////


int     MxPad::ToNotepad(char  *str)

{
	#ifndef _DEBUG
		if(!force)
			return (0);
	#endif

	if(local)
		{
		HWND    chld;

		if((chld = GetChild(FindWindow(appname, NULL)) ))
			Edit_ReplaceSel(chld, str);
		}

	if(trace)
		{
#ifdef TRACE
		TRACE(str);
#endif
		}

	if(remote)
		{
		if(!resolved)
			Resolve();

		if(!connected)
			Connect();

		int len = strlen(str);
		if(send(sock2, str, len, 0) < 0)
			{
			MessageBox(NULL, "Cannot send", "", MB_OK);
			}
		}
    return(0);
}

// --------------------------------------------------------------
//
/// \brief   UnicodeToAnsi converts the Unicode string pszW to 
/// an ANSI string. 
/// 
/// \details Returns the ANSI string through ppszA. Space for the
/// converted string is allocated by this function. (care: free) 
/// \n \n 
/// Return type:      DWORD  
/// \n \n 
/// \param pszW Arg Type: LPWSTR Pointer to the UNICODE str to convert from
/// \param ppszA Arg Type: LPSTR* ptr to a buffer toi receve results

DWORD   MxPad::UnicodeToAnsi(LPWSTR pszW, LPSTR* ppszA)

{
    ULONG cbAnsi, cCharacters;
    DWORD dwError;

    //Free old instance:
    if(*ppszA)
        free(*ppszA);

    // If input is null then just return the same.
    if (pszW == NULL)
        {
        *ppszA = NULL;
        return NOERROR;
        }
    cCharacters = wcslen(pszW)+1;

    // Determine number of bytes to be allocated for ANSI string. An
    // ANSI string can have at most 2 bytes per character (for Double
    // Byte Character Strings.)

    cbAnsi = cCharacters*2;

    // Use of the OLE allocator is not required because the resultant
    // ANSI  string will never be passed to another COM component. You
    // can use your own allocator.

    *ppszA = (LPSTR) malloc(cbAnsi);

    if (NULL == *ppszA)
        return TRUE;

    // Convert to ANSI.

    if (0 == WideCharToMultiByte(CP_ACP, 0, pszW, cCharacters, *ppszA,
    cbAnsi, NULL, NULL))
        {
        dwError = GetLastError();
        free(*ppszA);
        *ppszA = NULL;
        return (dwError);
        }
    return NOERROR;
}

// --------------------------------------------------------------
//
/// \brief   AnsiToUnicode converts the ANSI string pszA to a 
/// Unicode string. 
/// 
/// \details AnsiToUnicode returns the Unicode string through ppszW. 
/// Space for the  the converted string is allocated by this 
/// function. (care: free)
/// \n  
/// Return type:      DWORD  
/// \n  
/// \param pszA Arg Type: LPCSTR 
/// \param ppszW Arg Type: LPWSTR* 

DWORD   MxPad::AnsiToUnicode(LPCSTR pszA, LPWSTR* ppszW)

{
    ULONG cCharacters;
    DWORD dwError;

    // If input is null then just return the same.
    if (NULL == pszA)
        {
        *ppszW = NULL; return NOERROR;
        }

    // Determine number of wide characters to be allocated for the
    // Unicode string.

    cCharacters =  strlen(pszA)+1;

    // Use of the OLE allocator is required if the resultant Unicode
    // string will be passed to another COM component and if that
    // component will free it. Otherwise you can use your own allocator.

    *ppszW = (LPWSTR) malloc(cCharacters*2);

    if (NULL == *ppszW)
        return (TRUE);

    // Covert to Unicode.
    if (0 == MultiByteToWideChar(CP_ACP, 0, pszA, cCharacters,
    *ppszW, cCharacters))
        {
        dwError = GetLastError();
        free(*ppszW);
        *ppszW = NULL;
        return (dwError);
        }
    return NOERROR;
}

// --------------------------------------------------------------
//
/// \brief   Convert numeric message number to text 
/// 
/// \details DescHere 
/// \n  
/// Return type:      char  
/// \n  
/// \param msg Arg Type: int 

char    *MxPad::num2msg(int msg)

{
	static	char buff[32];

    char *str = "WM_UNKNOWN";

    switch(msg)
        {
        case WM_NULL              : str = "WM_NULL"; break;
        case WM_CREATE            : str = "WM_CREATE"; break;
        case WM_DESTROY           : str = "WM_DESTROY"; break;
        case WM_MOVE              : str = "WM_MOVE"; break;
        case WM_SIZE              : str = "WM_SIZE"; break;
        case WM_ACTIVATE          : str = "WM_ACTIVATE"; break;
        case WM_SETFOCUS          : str = "WM_SETFOCUS"; break;
        case WM_KILLFOCUS         : str = "WM_KILLFOCUS"; break;
        case WM_ENABLE            : str = "WM_ENABLE"; break;
        case WM_SETREDRAW         : str = "WM_SETREDRAW"; break;
        case WM_SETTEXT           : str = "WM_SETTEXT"; break;
        case WM_GETTEXT           : str = "WM_GETTEXT"; break;
        case WM_GETTEXTLENGTH     : str = "WM_GETTEXTLENGTH"; break;
        case WM_PAINT             : str = "WM_PAINT"; break;
        case WM_CLOSE             : str = "WM_CLOSE"; break;
        case WM_QUERYENDSESSION   : str = "WM_QUERYENDSESSION"; break;
        case WM_QUIT              : str = "WM_QUIT"; break;
        case WM_QUERYOPEN         : str = "WM_QUERYOPEN"; break;
        case WM_ERASEBKGND        : str = "WM_ERASEBKGND"; break;
        case WM_SYSCOLORCHANGE    : str = "WM_SYSCOLORCHANGE"; break;
        case WM_ENDSESSION        : str = "WM_ENDSESSION"; break;
        case WM_SHOWWINDOW        : str = "WM_SHOWWINDOW"; break;
        case WM_WININICHANGE      : str = "WM_WININICHANGE"; break;
        case WM_DEVMODECHANGE     : str = "WM_DEVMODECHANGE"; break;
        case WM_ACTIVATEAPP       : str = "WM_ACTIVATEAPP"; break;
        case WM_FONTCHANGE        : str = "WM_FONTCHANGE"; break;
        case WM_TIMECHANGE        : str = "WM_TIMECHANGE"; break;
        case WM_CANCELMODE        : str = "WM_CANCELMODE"; break;
        case WM_SETCURSOR         : str = "WM_SETCURSOR"; break;
        case WM_MOUSEACTIVATE     : str = "WM_MOUSEACTIVATE"; break;
        case WM_CHILDACTIVATE     : str = "WM_CHILDACTIVATE"; break;
        case WM_QUEUESYNC         : str = "WM_QUEUESYNC"; break;
        case WM_GETMINMAXINFO     : str = "WM_GETMINMAXINFO"; break;
        case WM_PAINTICON         : str = "WM_PAINTICON"; break;
        case WM_ICONERASEBKGND    : str = "WM_ICONERASEBKGND"; break;
        case WM_NEXTDLGCTL        : str = "WM_NEXTDLGCTL"; break;
        case WM_SPOOLERSTATUS     : str = "WM_SPOOLERSTATUS"; break;
        case WM_DRAWITEM          : str = "WM_DRAWITEM"; break;
        case WM_MEASUREITEM       : str = "WM_MEASUREITEM"; break;
        case WM_DELETEITEM        : str = "WM_DELETEITEM"; break;
        case WM_VKEYTOITEM        : str = "WM_VKEYTOITEM"; break;
        case WM_CHARTOITEM        : str = "WM_CHARTOITEM"; break;
        case WM_SETFONT           : str = "WM_SETFONT"; break;
        case WM_GETFONT           : str = "WM_GETFONT"; break;
        case WM_SETHOTKEY         : str = "WM_SETHOTKEY"; break;
        case WM_GETHOTKEY         : str = "WM_GETHOTKEY"; break;
        case WM_QUERYDRAGICON     : str = "WM_QUERYDRAGICON"; break;
        case WM_COMPAREITEM       : str = "WM_COMPAREITEM"; break;
        case WM_COMPACTING        : str = "WM_COMPACTING"; break;
        case WM_COMMNOTIFY        : str = "WM_COMMNOTIFY"; break;
        case WM_WINDOWPOSCHANGING : str = "WM_WINDOWPOSCHANGING"; break;
        case WM_WINDOWPOSCHANGED  : str = "WM_WINDOWPOSCHANGED"; break;
        case WM_POWER             : str = "WM_POWER"; break;
        case WM_COPYDATA          : str = "WM_COPYDATA"; break;
        case WM_CANCELJOURNAL     : str = "WM_CANCELJOURNAL"; break;
        case WM_NCCREATE          : str = "WM_NCCREATE"; break;
        case WM_NCDESTROY         : str = "WM_NCDESTROY"; break;
        case WM_NCCALCSIZE        : str = "WM_NCCALCSIZE"; break;
        case WM_NCHITTEST         : str = "WM_NCHITTEST"; break;
        case WM_NCPAINT           : str = "WM_NCPAINT"; break;
        case WM_NCACTIVATE        : str = "WM_NCACTIVATE"; break;
        case WM_GETDLGCODE        : str = "WM_GETDLGCODE"; break;
        case WM_NCMOUSEMOVE       : str = "WM_NCMOUSEMOVE"; break;
        case WM_NCLBUTTONDOWN     : str = "WM_NCLBUTTONDOWN"; break;
        case WM_NCLBUTTONUP       : str = "WM_NCLBUTTONUP"; break;
        case WM_NCLBUTTONDBLCLK   : str = "WM_NCLBUTTONDBLCLK"; break;
        case WM_NCRBUTTONDOWN     : str = "WM_NCRBUTTONDOWN"; break;
        case WM_NCRBUTTONUP       : str = "WM_NCRBUTTONUP"; break;
        case WM_NCRBUTTONDBLCLK   : str = "WM_NCRBUTTONDBLCLK"; break;
        case WM_NCMBUTTONDOWN     : str = "WM_NCMBUTTONDOWN"; break;
        case WM_NCMBUTTONUP       : str = "WM_NCMBUTTONUP"; break;
        case WM_NCMBUTTONDBLCLK   : str = "WM_NCMBUTTONDBLCLK"; break;
        //case WM_KEYFIRST          : str = "WM_KEYFIRST"; break;
        case WM_KEYDOWN           : str = "WM_KEYDOWN"; break;
        case WM_KEYUP             : str = "WM_KEYUP"; break;
        case WM_CHAR              : str = "WM_CHAR"; break;
        case WM_DEADCHAR          : str = "WM_DEADCHAR"; break;
        case WM_SYSKEYDOWN        : str = "WM_SYSKEYDOWN"; break;
        case WM_SYSKEYUP          : str = "WM_SYSKEYUP"; break;
        case WM_SYSCHAR           : str = "WM_SYSCHAR"; break;
        case WM_SYSDEADCHAR       : str = "WM_SYSDEADCHAR"; break;
        case WM_KEYLAST           : str = "WM_KEYLAST"; break;
        case WM_INITDIALOG        : str = "WM_INITDIALOG"; break;
        case WM_COMMAND           : str = "WM_COMMAND"; break;
        case WM_SYSCOMMAND        : str = "WM_SYSCOMMAND"; break;
        case WM_TIMER             : str = "WM_TIMER"; break;
        case WM_HSCROLL           : str = "WM_HSCROLL"; break;
        case WM_VSCROLL           : str = "WM_VSCROLL"; break;
        case WM_INITMENU          : str = "WM_INITMENU"; break;
        case WM_INITMENUPOPUP     : str = "WM_INITMENUPOPUP"; break;
        case WM_MENUSELECT        : str = "WM_MENUSELECT"; break;
        case WM_MENUCHAR          : str = "WM_MENUCHAR"; break;
        case WM_ENTERIDLE         : str = "WM_ENTERIDLE"; break;
        case WM_CTLCOLORMSGBOX    : str = "WM_CTLCOLORMSGBOX"; break;
        case WM_CTLCOLOREDIT      : str = "WM_CTLCOLOREDIT"; break;
        case WM_CTLCOLORLISTBOX   : str = "WM_CTLCOLORLISTBOX"; break;
        case WM_CTLCOLORBTN       : str = "WM_CTLCOLORBTN"; break;
        case WM_CTLCOLORDLG       : str = "WM_CTLCOLORDLG"; break;
        case WM_CTLCOLORSCROLLBAR : str = "WM_CTLCOLORSCROLLBAR"; break;
        case WM_CTLCOLORSTATIC    : str = "WM_CTLCOLORSTATIC"; break;
        //case WM_MOUSEFIRST        : str = "WM_MOUSEFIRST"; break;
        case WM_MOUSEMOVE         : str = "WM_MOUSEMOVE"; break;
        case WM_LBUTTONDOWN       : str = "WM_LBUTTONDOWN"; break;
        case WM_LBUTTONUP         : str = "WM_LBUTTONUP"; break;
        case WM_LBUTTONDBLCLK     : str = "WM_LBUTTONDBLCLK"; break;
        case WM_RBUTTONDOWN       : str = "WM_RBUTTONDOWN"; break;
        case WM_RBUTTONUP         : str = "WM_RBUTTONUP"; break;
        case WM_RBUTTONDBLCLK     : str = "WM_RBUTTONDBLCLK"; break;
        case WM_MBUTTONDOWN       : str = "WM_MBUTTONDOWN"; break;
        case WM_MBUTTONUP         : str = "WM_MBUTTONUP"; break;
        case WM_MBUTTONDBLCLK     : str = "WM_MBUTTONDBLCLK"; break;
        //case WM_MOUSELAST         : str = "WM_MOUSELAST"; break;
        case WM_PARENTNOTIFY      : str = "WM_PARENTNOTIFY"; break;
        case WM_ENTERMENULOOP     : str = "WM_ENTERMENULOOP"; break;
        case WM_EXITMENULOOP      : str = "WM_EXITMENULOOP"; break;
        case WM_MDICREATE         : str = "WM_MDICREATE"; break;
        case WM_MDIDESTROY        : str = "WM_MDIDESTROY"; break;
        case WM_MDIACTIVATE       : str = "WM_MDIACTIVATE"; break;
        case WM_MDIRESTORE        : str = "WM_MDIRESTORE"; break;
        case WM_MDINEXT           : str = "WM_MDINEXT"; break;
        case WM_MDIMAXIMIZE       : str = "WM_MDIMAXIMIZE"; break;
        case WM_MDITILE           : str = "WM_MDITILE"; break;
        case WM_MDICASCADE        : str = "WM_MDICASCADE"; break;
        case WM_MDIICONARRANGE    : str = "WM_MDIICONARRANGE"; break;
        case WM_MDIGETACTIVE      : str = "WM_MDIGETACTIVE"; break;
        case WM_MDISETMENU        : str = "WM_MDISETMENU"; break;
        case WM_DROPFILES         : str = "WM_DROPFILES"; break;
        case WM_MDIREFRESHMENU    : str = "WM_MDIREFRESHMENU"; break;
        case WM_CUT               : str = "WM_CUT"; break;
        case WM_COPY              : str = "WM_COPY"; break;
        case WM_PASTE             : str = "WM_PASTE"; break;
        case WM_CLEAR             : str = "WM_CLEAR"; break;
        case WM_UNDO              : str = "WM_UNDO"; break;
        case WM_RENDERFORMAT      : str = "WM_RENDERFORMAT"; break;
        case WM_RENDERALLFORMATS  : str = "WM_RENDERALLFORMATS"; break;
        case WM_DESTROYCLIPBOARD  : str = "WM_DESTROYCLIPBOARD"; break;
        case WM_DRAWCLIPBOARD     : str = "WM_DRAWCLIPBOARD"; break;
        case WM_PAINTCLIPBOARD    : str = "WM_PAINTCLIPBOARD"; break;
        case WM_VSCROLLCLIPBOARD  : str = "WM_VSCROLLCLIPBOARD"; break;
        case WM_SIZECLIPBOARD     : str = "WM_SIZECLIPBOARD"; break;
        case WM_ASKCBFORMATNAME   : str = "WM_ASKCBFORMATNAME"; break;
        case WM_CHANGECBCHAIN     : str = "WM_CHANGECBCHAIN"; break;
        case WM_HSCROLLCLIPBOARD  : str = "WM_HSCROLLCLIPBOARD"; break;
        case WM_QUERYNEWPALETTE   : str = "WM_QUERYNEWPALETTE"; break;
        case WM_PALETTEISCHANGING : str = "WM_PALETTEISCHANGING"; break;
        case WM_PALETTECHANGED    : str = "WM_PALETTECHANGED"; break;
        case WM_HOTKEY            : str = "WM_HOTKEY"; break;
        case WM_PENWINFIRST       : str = "WM_PENWINFIRST"; break;
        case WM_PENWINLAST        : str = "WM_PENWINLAST"; break;
        case WM_USER              : str = "WM_USER"; break;

		default:
			sprintf(buff, "WM_UNKOWN %d", msg);
			str = buff;
			break;
        }

    return(str);
}

//////////////////////////////////////////////////////////////////////////
// Non client messages are identified:
//

int     MxPad::is_nonclient(int msg)

{
    int     ret_val = FALSE;

    switch(msg)
        {
        case WM_NCCREATE:
        case WM_NCDESTROY:
        case WM_NCCALCSIZE:
        case WM_NCHITTEST:
        case WM_NCPAINT:
        case WM_NCACTIVATE:
        case WM_NCMOUSEMOVE:
        case WM_NCLBUTTONDOWN:
        case WM_NCLBUTTONUP:
        case WM_NCLBUTTONDBLCLK:
        case WM_NCRBUTTONDOWN:
        case WM_NCRBUTTONUP:
        case WM_NCRBUTTONDBLCLK:
        case WM_NCMBUTTONDOWN:
        case WM_NCMBUTTONUP:
        case WM_NCMBUTTONDBLCLK:
            ret_val = TRUE;
            break;
        }
    return(ret_val);
}

int     MxPad::is_junk(int msg)

{
    int     ret_val = FALSE;

    switch(msg)
        {

        case WM_MOUSEMOVE:
        case WM_SETCURSOR:
        case WM_TIMER:
            ret_val = TRUE;
            break;
        }
    return(ret_val);
}

// --------------------------------------------------------------
//
/// \brief   Convert a numeric error to text 
/// 
/// \n  
/// Return type:      char  
/// \n  
/// \param msg Arg Type: int -- error number to convert from

char    *MxPad::num2err(int msg)

{
    char *str = "ERROR_UNKNOWN";

    switch(msg)
        {
        case ERROR_SUCCESS                      : str = "ERROR_SUCCESS";
        case ERROR_INVALID_FUNCTION             : str = "ERROR_INVALID_FUNCTION";
        case ERROR_FILE_NOT_FOUND               : str = "ERROR_FILE_NOT_FOUND";
        case ERROR_PATH_NOT_FOUND               : str = "ERROR_PATH_NOT_FOUND";
        case ERROR_TOO_MANY_OPEN_FILES          : str = "ERROR_TOO_MANY_OPEN_FILES";
        case ERROR_ACCESS_DENIED                : str = "ERROR_ACCESS_DENIED";
        case ERROR_INVALID_HANDLE               : str = "ERROR_INVALID_HANDLE";
        case ERROR_ARENA_TRASHED                : str = "ERROR_ARENA_TRASHED";
        case ERROR_NOT_ENOUGH_MEMORY            : str = "ERROR_NOT_ENOUGH_MEMORY";
        case ERROR_INVALID_BLOCK                : str = "ERROR_INVALID_BLOCK";
        case ERROR_BAD_ENVIRONMENT              : str = "ERROR_BAD_ENVIRONMENT";
        case ERROR_BAD_FORMAT                   : str = "ERROR_BAD_FORMAT";
        case ERROR_INVALID_ACCESS               : str = "ERROR_INVALID_ACCESS";
        case ERROR_INVALID_DATA                 : str = "ERROR_INVALID_DATA";
        case ERROR_OUTOFMEMORY                  : str = "ERROR_OUTOFMEMORY";
        case ERROR_INVALID_DRIVE                : str = "ERROR_INVALID_DRIVE";
        case ERROR_CURRENT_DIRECTORY            : str = "ERROR_CURRENT_DIRECTORY";
        case ERROR_NOT_SAME_DEVICE              : str = "ERROR_NOT_SAME_DEVICE";
        case ERROR_NO_MORE_FILES                : str = "ERROR_NO_MORE_FILES";
        case ERROR_WRITE_PROTECT                : str = "ERROR_WRITE_PROTECT";
        case ERROR_BAD_UNIT                     : str = "ERROR_BAD_UNIT";
        case ERROR_NOT_READY                    : str = "ERROR_NOT_READY";
        case ERROR_BAD_COMMAND                  : str = "ERROR_BAD_COMMAND";
        case ERROR_CRC                          : str = "ERROR_CRC";
        case ERROR_BAD_LENGTH                   : str = "ERROR_BAD_LENGTH";
        case ERROR_SEEK                         : str = "ERROR_SEEK";
        case ERROR_NOT_DOS_DISK                 : str = "ERROR_NOT_DOS_DISK";
        case ERROR_SECTOR_NOT_FOUND             : str = "ERROR_SECTOR_NOT_FOUND";
        case ERROR_OUT_OF_PAPER                 : str = "ERROR_OUT_OF_PAPER";
        case ERROR_WRITE_FAULT                  : str = "ERROR_WRITE_FAULT";
        case ERROR_READ_FAULT                   : str = "ERROR_READ_FAULT";
        case ERROR_GEN_FAILURE                  : str = "ERROR_GEN_FAILURE";
        case ERROR_SHARING_VIOLATION            : str = "ERROR_SHARING_VIOLATION";
        case ERROR_LOCK_VIOLATION               : str = "ERROR_LOCK_VIOLATION";
        case ERROR_WRONG_DISK                   : str = "ERROR_WRONG_DISK";
        case ERROR_SHARING_BUFFER_EXCEEDED      : str = "ERROR_SHARING_BUFFER_EXCEEDED";
        case ERROR_HANDLE_EOF                   : str = "ERROR_HANDLE_EOF";
        case ERROR_HANDLE_DISK_FULL             : str = "ERROR_HANDLE_DISK_FULL";
        case ERROR_NOT_SUPPORTED                : str = "ERROR_NOT_SUPPORTED";
        case ERROR_REM_NOT_LIST                 : str = "ERROR_REM_NOT_LIST";
        case ERROR_DUP_NAME                     : str = "ERROR_DUP_NAME";
        case ERROR_BAD_NETPATH                  : str = "ERROR_BAD_NETPATH";
        case ERROR_NETWORK_BUSY                 : str = "ERROR_NETWORK_BUSY";
        case ERROR_DEV_NOT_EXIST                : str = "ERROR_DEV_NOT_EXIST";
        case ERROR_TOO_MANY_CMDS                : str = "ERROR_TOO_MANY_CMDS";
        case ERROR_ADAP_HDW_ERR                 : str = "ERROR_ADAP_HDW_ERR";
        case ERROR_BAD_NET_RESP                 : str = "ERROR_BAD_NET_RESP";
        case ERROR_UNEXP_NET_ERR                : str = "ERROR_UNEXP_NET_ERR";
        case ERROR_BAD_REM_ADAP                 : str = "ERROR_BAD_REM_ADAP";
        case ERROR_PRINTQ_FULL                  : str = "ERROR_PRINTQ_FULL";
        case ERROR_NO_SPOOL_SPACE               : str = "ERROR_NO_SPOOL_SPACE";
        case ERROR_PRINT_CANCELLED              : str = "ERROR_PRINT_CANCELLED";
        case ERROR_NETNAME_DELETED              : str = "ERROR_NETNAME_DELETED";
        case ERROR_NETWORK_ACCESS_DENIED        : str = "ERROR_NETWORK_ACCESS_DENIED";
        case ERROR_BAD_DEV_TYPE                 : str = "ERROR_BAD_DEV_TYPE";
        case ERROR_BAD_NET_NAME                 : str = "ERROR_BAD_NET_NAME";
        case ERROR_TOO_MANY_NAMES               : str = "ERROR_TOO_MANY_NAMES";
        case ERROR_TOO_MANY_SESS                : str = "ERROR_TOO_MANY_SESS";
        case ERROR_SHARING_PAUSED               : str = "ERROR_SHARING_PAUSED";
        case ERROR_REQ_NOT_ACCEP                : str = "ERROR_REQ_NOT_ACCEP";
        case ERROR_REDIR_PAUSED                 : str = "ERROR_REDIR_PAUSED";
        case ERROR_FILE_EXISTS                  : str = "ERROR_FILE_EXISTS";
        case ERROR_CANNOT_MAKE                  : str = "ERROR_CANNOT_MAKE";
        case ERROR_FAIL_I24                     : str = "ERROR_FAIL_I24";
        case ERROR_OUT_OF_STRUCTURES            : str = "ERROR_OUT_OF_STRUCTURES";
        case ERROR_ALREADY_ASSIGNED             : str = "ERROR_ALREADY_ASSIGNED";
        case ERROR_INVALID_PASSWORD             : str = "ERROR_INVALID_PASSWORD";
        case ERROR_INVALID_PARAMETER            : str = "ERROR_INVALID_PARAMETER";
        case ERROR_NET_WRITE_FAULT              : str = "ERROR_NET_WRITE_FAULT";
        case ERROR_NO_PROC_SLOTS                : str = "ERROR_NO_PROC_SLOTS";
        case ERROR_TOO_MANY_SEMAPHORES          : str = "ERROR_TOO_MANY_SEMAPHORES";
        case ERROR_EXCL_SEM_ALREADY_OWNED       : str = "ERROR_EXCL_SEM_ALREADY_OWNED";
        case ERROR_SEM_IS_SET                   : str = "ERROR_SEM_IS_SET";
        case ERROR_TOO_MANY_SEM_REQUESTS        : str = "ERROR_TOO_MANY_SEM_REQUESTS";
        case ERROR_INVALID_AT_INTERRUPT_TIME    : str = "ERROR_INVALID_AT_INTERRUPT_TIME";
        case ERROR_SEM_OWNER_DIED               : str = "ERROR_SEM_OWNER_DIED";
        case ERROR_SEM_USER_LIMIT               : str = "ERROR_SEM_USER_LIMIT";
        case ERROR_DISK_CHANGE                  : str = "ERROR_DISK_CHANGE";
        case ERROR_DRIVE_LOCKED                 : str = "ERROR_DRIVE_LOCKED";
        case ERROR_BROKEN_PIPE                  : str = "ERROR_BROKEN_PIPE";
        case ERROR_OPEN_FAILED                  : str = "ERROR_OPEN_FAILED";
        case ERROR_BUFFER_OVERFLOW              : str = "ERROR_BUFFER_OVERFLOW";
        case ERROR_DISK_FULL                    : str = "ERROR_DISK_FULL";
        case ERROR_NO_MORE_SEARCH_HANDLES       : str = "ERROR_NO_MORE_SEARCH_HANDLES";
        case ERROR_INVALID_TARGET_HANDLE        : str = "ERROR_INVALID_TARGET_HANDLE";
        case ERROR_INVALID_CATEGORY             : str = "ERROR_INVALID_CATEGORY";
        case ERROR_INVALID_VERIFY_SWITCH        : str = "ERROR_INVALID_VERIFY_SWITCH";
        case ERROR_BAD_DRIVER_LEVEL             : str = "ERROR_BAD_DRIVER_LEVEL";
        case ERROR_CALL_NOT_IMPLEMENTED         : str = "ERROR_CALL_NOT_IMPLEMENTED";
        case ERROR_SEM_TIMEOUT                  : str = "ERROR_SEM_TIMEOUT";
        case ERROR_INSUFFICIENT_BUFFER          : str = "ERROR_INSUFFICIENT_BUFFER";
        case ERROR_INVALID_NAME                 : str = "ERROR_INVALID_NAME";
        case ERROR_INVALID_LEVEL                : str = "ERROR_INVALID_LEVEL";
        case ERROR_NO_VOLUME_LABEL              : str = "ERROR_NO_VOLUME_LABEL";
        case ERROR_MOD_NOT_FOUND                : str = "ERROR_MOD_NOT_FOUND";
        case ERROR_PROC_NOT_FOUND               : str = "ERROR_PROC_NOT_FOUND";
        case ERROR_WAIT_NO_CHILDREN             : str = "ERROR_WAIT_NO_CHILDREN";
        case ERROR_CHILD_NOT_COMPLETE           : str = "ERROR_CHILD_NOT_COMPLETE";
        case ERROR_DIRECT_ACCESS_HANDLE         : str = "ERROR_DIRECT_ACCESS_HANDLE";
        case ERROR_NEGATIVE_SEEK                : str = "ERROR_NEGATIVE_SEEK";
        case ERROR_SEEK_ON_DEVICE               : str = "ERROR_SEEK_ON_DEVICE";
        case ERROR_IS_JOIN_TARGET               : str = "ERROR_IS_JOIN_TARGET";
        case ERROR_IS_JOINED                    : str = "ERROR_IS_JOINED";
        case ERROR_IS_SUBSTED                   : str = "ERROR_IS_SUBSTED";
        case ERROR_NOT_JOINED                   : str = "ERROR_NOT_JOINED";
        case ERROR_NOT_SUBSTED                  : str = "ERROR_NOT_SUBSTED";
        case ERROR_JOIN_TO_JOIN                 : str = "ERROR_JOIN_TO_JOIN";
        case ERROR_SUBST_TO_SUBST               : str = "ERROR_SUBST_TO_SUBST";
        case ERROR_JOIN_TO_SUBST                : str = "ERROR_JOIN_TO_SUBST";
        case ERROR_SUBST_TO_JOIN                : str = "ERROR_SUBST_TO_JOIN";
        case ERROR_BUSY_DRIVE                   : str = "ERROR_BUSY_DRIVE";
        case ERROR_SAME_DRIVE                   : str = "ERROR_SAME_DRIVE";
        case ERROR_DIR_NOT_ROOT                 : str = "ERROR_DIR_NOT_ROOT";
        case ERROR_DIR_NOT_EMPTY                : str = "ERROR_DIR_NOT_EMPTY";
        case ERROR_IS_SUBST_PATH                : str = "ERROR_IS_SUBST_PATH";
        case ERROR_IS_JOIN_PATH                 : str = "ERROR_IS_JOIN_PATH";
        case ERROR_PATH_BUSY                    : str = "ERROR_PATH_BUSY";
        case ERROR_IS_SUBST_TARGET              : str = "ERROR_IS_SUBST_TARGET";
        case ERROR_SYSTEM_TRACE                 : str = "ERROR_SYSTEM_TRACE";
        case ERROR_INVALID_EVENT_COUNT          : str = "ERROR_INVALID_EVENT_COUNT";
        case ERROR_TOO_MANY_MUXWAITERS          : str = "ERROR_TOO_MANY_MUXWAITERS";
        case ERROR_INVALID_LIST_FORMAT          : str = "ERROR_INVALID_LIST_FORMAT";
        case ERROR_LABEL_TOO_LONG               : str = "ERROR_LABEL_TOO_LONG";
        case ERROR_TOO_MANY_TCBS                : str = "ERROR_TOO_MANY_TCBS";
        case ERROR_SIGNAL_REFUSED               : str = "ERROR_SIGNAL_REFUSED";
        case ERROR_DISCARDED                    : str = "ERROR_DISCARDED";
        case ERROR_NOT_LOCKED                   : str = "ERROR_NOT_LOCKED";
        case ERROR_BAD_THREADID_ADDR            : str = "ERROR_BAD_THREADID_ADDR";
        case ERROR_BAD_ARGUMENTS                : str = "ERROR_BAD_ARGUMENTS";
        case ERROR_BAD_PATHNAME                 : str = "ERROR_BAD_PATHNAME";
        case ERROR_SIGNAL_PENDING               : str = "ERROR_SIGNAL_PENDING";
        case ERROR_MAX_THRDS_REACHED            : str = "ERROR_MAX_THRDS_REACHED";
        case ERROR_LOCK_FAILED                  : str = "ERROR_LOCK_FAILED";
        case ERROR_BUSY                         : str = "ERROR_BUSY";
        case ERROR_CANCEL_VIOLATION             : str = "ERROR_CANCEL_VIOLATION";
        case ERROR_ATOMIC_LOCKS_NOT_SUPPORTED   : str = "ERROR_ATOMIC_LOCKS_NOT_SUPPORTED";
        case ERROR_INVALID_SEGMENT_NUMBER       : str = "ERROR_INVALID_SEGMENT_NUMBER";
        case ERROR_INVALID_ORDINAL              : str = "ERROR_INVALID_ORDINAL";
        case ERROR_ALREADY_EXISTS               : str = "ERROR_ALREADY_EXISTS";
        case ERROR_INVALID_FLAG_NUMBER          : str = "ERROR_INVALID_FLAG_NUMBER";
        case ERROR_SEM_NOT_FOUND                : str = "ERROR_SEM_NOT_FOUND";
        case ERROR_INVALID_STARTING_CODESEG     : str = "ERROR_INVALID_STARTING_CODESEG";
        case ERROR_INVALID_STACKSEG             : str = "ERROR_INVALID_STACKSEG";
        case ERROR_INVALID_MODULETYPE           : str = "ERROR_INVALID_MODULETYPE";
        case ERROR_INVALID_EXE_SIGNATURE        : str = "ERROR_INVALID_EXE_SIGNATURE";
        case ERROR_EXE_MARKED_INVALID           : str = "ERROR_EXE_MARKED_INVALID";
        case ERROR_BAD_EXE_FORMAT               : str = "ERROR_BAD_EXE_FORMAT";
        case ERROR_ITERATED_DATA_EXCEEDS_64k    : str = "ERROR_ITERATED_DATA_EXCEEDS_64k";
        case ERROR_INVALID_MINALLOCSIZE         : str = "ERROR_INVALID_MINALLOCSIZE";
        case ERROR_DYNLINK_FROM_INVALID_RING    : str = "ERROR_DYNLINK_FROM_INVALID_RING";
        case ERROR_IOPL_NOT_ENABLED             : str = "ERROR_IOPL_NOT_ENABLED";
        case ERROR_INVALID_SEGDPL               : str = "ERROR_INVALID_SEGDPL";
        case ERROR_AUTODATASEG_EXCEEDS_64k      : str = "ERROR_AUTODATASEG_EXCEEDS_64k";
        case ERROR_RING2SEG_MUST_BE_MOVABLE     : str = "ERROR_RING2SEG_MUST_BE_MOVABLE";
        case ERROR_RELOC_CHAIN_XEEDS_SEGLIM     : str = "ERROR_RELOC_CHAIN_XEEDS_SEGLIM";
        case ERROR_INFLOOP_IN_RELOC_CHAIN       : str = "ERROR_INFLOOP_IN_RELOC_CHAIN";
        case ERROR_ENVVAR_NOT_FOUND             : str = "ERROR_ENVVAR_NOT_FOUND";
        case ERROR_NO_SIGNAL_SENT               : str = "ERROR_NO_SIGNAL_SENT";
        case ERROR_FILENAME_EXCED_RANGE         : str = "ERROR_FILENAME_EXCED_RANGE";
        case ERROR_RING2_STACK_IN_USE           : str = "ERROR_RING2_STACK_IN_USE";
        case ERROR_META_EXPANSION_TOO_LONG      : str = "ERROR_META_EXPANSION_TOO_LONG";
        case ERROR_INVALID_SIGNAL_NUMBER        : str = "ERROR_INVALID_SIGNAL_NUMBER";
        case ERROR_THREAD_1_INACTIVE            : str = "ERROR_THREAD_1_INACTIVE";
        case ERROR_LOCKED                       : str = "ERROR_LOCKED";
        case ERROR_TOO_MANY_MODULES             : str = "ERROR_TOO_MANY_MODULES";
        case ERROR_NESTING_NOT_ALLOWED          : str = "ERROR_NESTING_NOT_ALLOWED";
        case ERROR_BAD_PIPE                     : str = "ERROR_BAD_PIPE";
        case ERROR_PIPE_BUSY                    : str = "ERROR_PIPE_BUSY";
        case ERROR_NO_DATA                      : str = "ERROR_NO_DATA";
        case ERROR_PIPE_NOT_CONNECTED           : str = "ERROR_PIPE_NOT_CONNECTED";
        case ERROR_MORE_DATA                    : str = "ERROR_MORE_DATA";
        case ERROR_VC_DISCONNECTED              : str = "ERROR_VC_DISCONNECTED";
        case ERROR_INVALID_EA_NAME              : str = "ERROR_INVALID_EA_NAME";
        case ERROR_EA_LIST_INCONSISTENT         : str = "ERROR_EA_LIST_INCONSISTENT";
        case ERROR_NO_MORE_ITEMS                : str = "ERROR_NO_MORE_ITEMS";
        case ERROR_CANNOT_COPY                  : str = "ERROR_CANNOT_COPY";
        case ERROR_DIRECTORY                    : str = "ERROR_DIRECTORY";
        case ERROR_EAS_DIDNT_FIT                : str = "ERROR_EAS_DIDNT_FIT";
        case ERROR_EA_FILE_CORRUPT              : str = "ERROR_EA_FILE_CORRUPT";
        case ERROR_EA_TABLE_FULL                : str = "ERROR_EA_TABLE_FULL";
        case ERROR_INVALID_EA_HANDLE            : str = "ERROR_INVALID_EA_HANDLE";
        case ERROR_EAS_NOT_SUPPORTED            : str = "ERROR_EAS_NOT_SUPPORTED";
        case ERROR_NOT_OWNER                    : str = "ERROR_NOT_OWNER";
        case ERROR_TOO_MANY_POSTS               : str = "ERROR_TOO_MANY_POSTS";
        case ERROR_PARTIAL_COPY                 : str = "ERROR_PARTIAL_COPY";
        case ERROR_MR_MID_NOT_FOUND             : str = "ERROR_MR_MID_NOT_FOUND";
        case ERROR_INVALID_ADDRESS              : str = "ERROR_INVALID_ADDRESS";
        case ERROR_ARITHMETIC_OVERFLOW          : str = "ERROR_ARITHMETIC_OVERFLOW";
        case ERROR_PIPE_CONNECTED               : str = "ERROR_PIPE_CONNECTED";
        case ERROR_PIPE_LISTENING               : str = "ERROR_PIPE_LISTENING";
        case ERROR_EA_ACCESS_DENIED             : str = "ERROR_EA_ACCESS_DENIED";
        case ERROR_OPERATION_ABORTED            : str = "ERROR_OPERATION_ABORTED";
        case ERROR_IO_INCOMPLETE                : str = "ERROR_IO_INCOMPLETE";
        case ERROR_IO_PENDING                   : str = "ERROR_IO_PENDING";
        case ERROR_NOACCESS                     : str = "ERROR_NOACCESS";
        case ERROR_SWAPERROR                    : str = "ERROR_SWAPERROR";
        case ERROR_STACK_OVERFLOW               : str = "ERROR_STACK_OVERFLOW";
        case ERROR_INVALID_MESSAGE              : str = "ERROR_INVALID_MESSAGE";
        case ERROR_CAN_NOT_COMPLETE             : str = "ERROR_CAN_NOT_COMPLETE";
        case ERROR_INVALID_FLAGS                : str = "ERROR_INVALID_FLAGS";
        case ERROR_UNRECOGNIZED_VOLUME          : str = "ERROR_UNRECOGNIZED_VOLUME";
        case ERROR_FILE_INVALID                 : str = "ERROR_FILE_INVALID";
        case ERROR_FULLSCREEN_MODE              : str = "ERROR_FULLSCREEN_MODE";
        case ERROR_NO_TOKEN                     : str = "ERROR_NO_TOKEN";
        case ERROR_BADDB                        : str = "ERROR_BADDB";
        case ERROR_BADKEY                       : str = "ERROR_BADKEY";
        case ERROR_CANTOPEN                     : str = "ERROR_CANTOPEN";
        case ERROR_CANTREAD                     : str = "ERROR_CANTREAD";
        case ERROR_CANTWRITE                    : str = "ERROR_CANTWRITE";
        case ERROR_REGISTRY_RECOVERED           : str = "ERROR_REGISTRY_RECOVERED";
        case ERROR_REGISTRY_CORRUPT             : str = "ERROR_REGISTRY_CORRUPT";
        case ERROR_REGISTRY_IO_FAILED           : str = "ERROR_REGISTRY_IO_FAILED";
        case ERROR_NOT_REGISTRY_FILE            : str = "ERROR_NOT_REGISTRY_FILE";
        case ERROR_KEY_DELETED                  : str = "ERROR_KEY_DELETED";
        case ERROR_NO_LOG_SPACE                 : str = "ERROR_NO_LOG_SPACE";
        case ERROR_KEY_HAS_CHILDREN             : str = "ERROR_KEY_HAS_CHILDREN";
        case ERROR_CHILD_MUST_BE_VOLATILE       : str = "ERROR_CHILD_MUST_BE_VOLATILE";
        case ERROR_NOTIFY_ENUM_DIR              : str = "ERROR_NOTIFY_ENUM_DIR";
        case ERROR_DEPENDENT_SERVICES_RUNNING   : str = "ERROR_DEPENDENT_SERVICES_RUNNING";
        case ERROR_INVALID_SERVICE_CONTROL      : str = "ERROR_INVALID_SERVICE_CONTROL";
        case ERROR_SERVICE_REQUEST_TIMEOUT      : str = "ERROR_SERVICE_REQUEST_TIMEOUT";
        case ERROR_SERVICE_NO_THREAD            : str = "ERROR_SERVICE_NO_THREAD";
        case ERROR_SERVICE_DATABASE_LOCKED      : str = "ERROR_SERVICE_DATABASE_LOCKED";
        case ERROR_SERVICE_ALREADY_RUNNING      : str = "ERROR_SERVICE_ALREADY_RUNNING";
        case ERROR_INVALID_SERVICE_ACCOUNT      : str = "ERROR_INVALID_SERVICE_ACCOUNT";
        case ERROR_SERVICE_DISABLED             : str = "ERROR_SERVICE_DISABLED";
        case ERROR_CIRCULAR_DEPENDENCY          : str = "ERROR_CIRCULAR_DEPENDENCY";
        case ERROR_SERVICE_DOES_NOT_EXIST       : str = "ERROR_SERVICE_DOES_NOT_EXIST";
        case ERROR_SERVICE_CANNOT_ACCEPT_CTRL   : str = "ERROR_SERVICE_CANNOT_ACCEPT_CTRL";
        case ERROR_SERVICE_NOT_ACTIVE           : str = "ERROR_SERVICE_NOT_ACTIVE";
        case ERROR_EXCEPTION_IN_SERVICE         : str = "ERROR_EXCEPTION_IN_SERVICE";
        case ERROR_DATABASE_DOES_NOT_EXIST      : str = "ERROR_DATABASE_DOES_NOT_EXIST";
        case ERROR_SERVICE_SPECIFIC_ERROR       : str = "ERROR_SERVICE_SPECIFIC_ERROR";
        case ERROR_PROCESS_ABORTED              : str = "ERROR_PROCESS_ABORTED";
        case ERROR_SERVICE_DEPENDENCY_FAIL      : str = "ERROR_SERVICE_DEPENDENCY_FAIL";
        case ERROR_SERVICE_LOGON_FAILED         : str = "ERROR_SERVICE_LOGON_FAILED";
        case ERROR_SERVICE_START_HANG           : str = "ERROR_SERVICE_START_HANG";
        case ERROR_INVALID_SERVICE_LOCK         : str = "ERROR_INVALID_SERVICE_LOCK";
        case ERROR_SERVICE_MARKED_FOR_DELETE    : str = "ERROR_SERVICE_MARKED_FOR_DELETE";
        case ERROR_SERVICE_EXISTS               : str = "ERROR_SERVICE_EXISTS";
        case ERROR_ALREADY_RUNNING_LKG          : str = "ERROR_ALREADY_RUNNING_LKG";
        case ERROR_SERVICE_DEPENDENCY_DELETED   : str = "ERROR_SERVICE_DEPENDENCY_DELETED";
        case ERROR_BOOT_ALREADY_ACCEPTED        : str = "ERROR_BOOT_ALREADY_ACCEPTED";
        case ERROR_SERVICE_NEVER_STARTED        : str = "ERROR_SERVICE_NEVER_STARTED";
        case ERROR_DUPLICATE_SERVICE_NAME       : str = "ERROR_DUPLICATE_SERVICE_NAME";
        case ERROR_END_OF_MEDIA                 : str = "ERROR_END_OF_MEDIA";
        case ERROR_FILEMARK_DETECTED            : str = "ERROR_FILEMARK_DETECTED";
        case ERROR_BEGINNING_OF_MEDIA           : str = "ERROR_BEGINNING_OF_MEDIA";
        case ERROR_SETMARK_DETECTED             : str = "ERROR_SETMARK_DETECTED";
        case ERROR_NO_DATA_DETECTED             : str = "ERROR_NO_DATA_DETECTED";
        case ERROR_PARTITION_FAILURE            : str = "ERROR_PARTITION_FAILURE";
        case ERROR_INVALID_BLOCK_LENGTH         : str = "ERROR_INVALID_BLOCK_LENGTH";
        case ERROR_DEVICE_NOT_PARTITIONED       : str = "ERROR_DEVICE_NOT_PARTITIONED";
        case ERROR_UNABLE_TO_LOCK_MEDIA         : str = "ERROR_UNABLE_TO_LOCK_MEDIA";
        case ERROR_UNABLE_TO_UNLOAD_MEDIA       : str = "ERROR_UNABLE_TO_UNLOAD_MEDIA";
        case ERROR_MEDIA_CHANGED                : str = "ERROR_MEDIA_CHANGED";
        case ERROR_BUS_RESET                    : str = "ERROR_BUS_RESET";
        case ERROR_NO_MEDIA_IN_DRIVE            : str = "ERROR_NO_MEDIA_IN_DRIVE";
        case ERROR_NO_UNICODE_TRANSLATION       : str = "ERROR_NO_UNICODE_TRANSLATION";
        case ERROR_DLL_INIT_FAILED              : str = "ERROR_DLL_INIT_FAILED";
        case ERROR_SHUTDOWN_IN_PROGRESS         : str = "ERROR_SHUTDOWN_IN_PROGRESS";
        case ERROR_NO_SHUTDOWN_IN_PROGRESS      : str = "ERROR_NO_SHUTDOWN_IN_PROGRESS";
        case ERROR_IO_DEVICE                    : str = "ERROR_IO_DEVICE";
        case ERROR_SERIAL_NO_DEVICE             : str = "ERROR_SERIAL_NO_DEVICE";
        case ERROR_IRQ_BUSY                     : str = "ERROR_IRQ_BUSY";
        case ERROR_MORE_WRITES                  : str = "ERROR_MORE_WRITES";
        case ERROR_COUNTER_TIMEOUT              : str = "ERROR_COUNTER_TIMEOUT";
        case ERROR_FLOPPY_ID_MARK_NOT_FOUND     : str = "ERROR_FLOPPY_ID_MARK_NOT_FOUND";
        case ERROR_FLOPPY_WRONG_CYLINDER        : str = "ERROR_FLOPPY_WRONG_CYLINDER";
        case ERROR_FLOPPY_UNKNOWN_ERROR         : str = "ERROR_FLOPPY_UNKNOWN_ERROR";
        case ERROR_FLOPPY_BAD_REGISTERS         : str = "ERROR_FLOPPY_BAD_REGISTERS";
        case ERROR_DISK_RECALIBRATE_FAILED      : str = "ERROR_DISK_RECALIBRATE_FAILED";
        case ERROR_DISK_OPERATION_FAILED        : str = "ERROR_DISK_OPERATION_FAILED";
        case ERROR_DISK_RESET_FAILED            : str = "ERROR_DISK_RESET_FAILED";
        case ERROR_EOM_OVERFLOW                 : str = "ERROR_EOM_OVERFLOW";
        case ERROR_NOT_ENOUGH_SERVER_MEMORY     : str = "ERROR_NOT_ENOUGH_SERVER_MEMORY";
        case ERROR_POSSIBLE_DEADLOCK            : str = "ERROR_POSSIBLE_DEADLOCK";
        case ERROR_MAPPED_ALIGNMENT             : str = "ERROR_MAPPED_ALIGNMENT";
        case ERROR_BAD_USERNAME                 : str = "ERROR_BAD_USERNAME";
        case ERROR_NOT_CONNECTED                : str = "ERROR_NOT_CONNECTED";
        case ERROR_OPEN_FILES                   : str = "ERROR_OPEN_FILES";
        case ERROR_ACTIVE_CONNECTIONS           : str = "ERROR_ACTIVE_CONNECTIONS";
        case ERROR_DEVICE_IN_USE                : str = "ERROR_DEVICE_IN_USE";
        case ERROR_BAD_DEVICE                   : str = "ERROR_BAD_DEVICE";
        case ERROR_CONNECTION_UNAVAIL           : str = "ERROR_CONNECTION_UNAVAIL";
        case ERROR_DEVICE_ALREADY_REMEMBERED    : str = "ERROR_DEVICE_ALREADY_REMEMBERED";
        case ERROR_NO_NET_OR_BAD_PATH           : str = "ERROR_NO_NET_OR_BAD_PATH";
        case ERROR_BAD_PROVIDER                 : str = "ERROR_BAD_PROVIDER";
        case ERROR_CANNOT_OPEN_PROFILE          : str = "ERROR_CANNOT_OPEN_PROFILE";
        case ERROR_BAD_PROFILE                  : str = "ERROR_BAD_PROFILE";
        case ERROR_NOT_CONTAINER                : str = "ERROR_NOT_CONTAINER";
        case ERROR_EXTENDED_ERROR               : str = "ERROR_EXTENDED_ERROR";
        case ERROR_INVALID_GROUPNAME            : str = "ERROR_INVALID_GROUPNAME";
        case ERROR_INVALID_COMPUTERNAME         : str = "ERROR_INVALID_COMPUTERNAME";
        case ERROR_INVALID_EVENTNAME            : str = "ERROR_INVALID_EVENTNAME";
        case ERROR_INVALID_DOMAINNAME           : str = "ERROR_INVALID_DOMAINNAME";
        case ERROR_INVALID_SERVICENAME          : str = "ERROR_INVALID_SERVICENAME";
        case ERROR_INVALID_NETNAME              : str = "ERROR_INVALID_NETNAME";
        case ERROR_INVALID_SHARENAME            : str = "ERROR_INVALID_SHARENAME";
        case ERROR_INVALID_PASSWORDNAME         : str = "ERROR_INVALID_PASSWORDNAME";
        case ERROR_INVALID_MESSAGENAME          : str = "ERROR_INVALID_MESSAGENAME";
        case ERROR_INVALID_MESSAGEDEST          : str = "ERROR_INVALID_MESSAGEDEST";
        case ERROR_DUP_DOMAINNAME               : str = "ERROR_DUP_DOMAINNAME";
        case ERROR_NO_NETWORK                   : str = "ERROR_NO_NETWORK";
        case ERROR_CANCELLED                    : str = "ERROR_CANCELLED";
        case ERROR_USER_MAPPED_FILE             : str = "ERROR_USER_MAPPED_FILE";
        case ERROR_CONNECTION_REFUSED           : str = "ERROR_CONNECTION_REFUSED";
        case ERROR_GRACEFUL_DISCONNECT          : str = "ERROR_GRACEFUL_DISCONNECT";
        case ERROR_ADDRESS_ALREADY_ASSOCIATED   : str = "ERROR_ADDRESS_ALREADY_ASSOCIATED";
        case ERROR_ADDRESS_NOT_ASSOCIATED       : str = "ERROR_ADDRESS_NOT_ASSOCIATED";
        case ERROR_CONNECTION_INVALID           : str = "ERROR_CONNECTION_INVALID";
        case ERROR_CONNECTION_ACTIVE            : str = "ERROR_CONNECTION_ACTIVE";
        case ERROR_NETWORK_UNREACHABLE          : str = "ERROR_NETWORK_UNREACHABLE";
        case ERROR_HOST_UNREACHABLE             : str = "ERROR_HOST_UNREACHABLE";
        case ERROR_PROTOCOL_UNREACHABLE         : str = "ERROR_PROTOCOL_UNREACHABLE";
        case ERROR_PORT_UNREACHABLE             : str = "ERROR_PORT_UNREACHABLE";
        case ERROR_REQUEST_ABORTED              : str = "ERROR_REQUEST_ABORTED";
        case ERROR_CONNECTION_ABORTED           : str = "ERROR_CONNECTION_ABORTED";
        case ERROR_RETRY                        : str = "ERROR_RETRY";
        case ERROR_CONNECTION_COUNT_LIMIT       : str = "ERROR_CONNECTION_COUNT_LIMIT";
        case ERROR_LOGIN_TIME_RESTRICTION       : str = "ERROR_LOGIN_TIME_RESTRICTION";
        case ERROR_LOGIN_WKSTA_RESTRICTION      : str = "ERROR_LOGIN_WKSTA_RESTRICTION";
        case ERROR_INCORRECT_ADDRESS            : str = "ERROR_INCORRECT_ADDRESS";
        case ERROR_ALREADY_REGISTERED           : str = "ERROR_ALREADY_REGISTERED";
        case ERROR_SERVICE_NOT_FOUND            : str = "ERROR_SERVICE_NOT_FOUND";
        case ERROR_NOT_ALL_ASSIGNED             : str = "ERROR_NOT_ALL_ASSIGNED";
        case ERROR_SOME_NOT_MAPPED              : str = "ERROR_SOME_NOT_MAPPED";
        case ERROR_NO_QUOTAS_FOR_ACCOUNT        : str = "ERROR_NO_QUOTAS_FOR_ACCOUNT";
        case ERROR_LOCAL_USER_SESSION_KEY       : str = "ERROR_LOCAL_USER_SESSION_KEY";
        case ERROR_NULL_LM_PASSWORD             : str = "ERROR_NULL_LM_PASSWORD";
        case ERROR_UNKNOWN_REVISION             : str = "ERROR_UNKNOWN_REVISION";
        case ERROR_REVISION_MISMATCH            : str = "ERROR_REVISION_MISMATCH";
        case ERROR_INVALID_OWNER                : str = "ERROR_INVALID_OWNER";
        case ERROR_INVALID_PRIMARY_GROUP        : str = "ERROR_INVALID_PRIMARY_GROUP";
        case ERROR_NO_IMPERSONATION_TOKEN       : str = "ERROR_NO_IMPERSONATION_TOKEN";
        case ERROR_CANT_DISABLE_MANDATORY       : str = "ERROR_CANT_DISABLE_MANDATORY";
        case ERROR_NO_LOGON_SERVERS             : str = "ERROR_NO_LOGON_SERVERS";
        case ERROR_NO_SUCH_LOGON_SESSION        : str = "ERROR_NO_SUCH_LOGON_SESSION";
        case ERROR_NO_SUCH_PRIVILEGE            : str = "ERROR_NO_SUCH_PRIVILEGE";
        case ERROR_PRIVILEGE_NOT_HELD           : str = "ERROR_PRIVILEGE_NOT_HELD";
        case ERROR_INVALID_ACCOUNT_NAME         : str = "ERROR_INVALID_ACCOUNT_NAME";
        case ERROR_USER_EXISTS                  : str = "ERROR_USER_EXISTS";
        case ERROR_NO_SUCH_USER                 : str = "ERROR_NO_SUCH_USER";
        case ERROR_GROUP_EXISTS                 : str = "ERROR_GROUP_EXISTS";
        case ERROR_NO_SUCH_GROUP                : str = "ERROR_NO_SUCH_GROUP";
        case ERROR_MEMBER_IN_GROUP              : str = "ERROR_MEMBER_IN_GROUP";
        case ERROR_MEMBER_NOT_IN_GROUP          : str = "ERROR_MEMBER_NOT_IN_GROUP";
        case ERROR_LAST_ADMIN                   : str = "ERROR_LAST_ADMIN";
        case ERROR_WRONG_PASSWORD               : str = "ERROR_WRONG_PASSWORD";
        case ERROR_ILL_FORMED_PASSWORD          : str = "ERROR_ILL_FORMED_PASSWORD";
        case ERROR_PASSWORD_RESTRICTION         : str = "ERROR_PASSWORD_RESTRICTION";
        case ERROR_LOGON_FAILURE                : str = "ERROR_LOGON_FAILURE";
        case ERROR_ACCOUNT_RESTRICTION          : str = "ERROR_ACCOUNT_RESTRICTION";
        case ERROR_INVALID_LOGON_HOURS          : str = "ERROR_INVALID_LOGON_HOURS";
        case ERROR_INVALID_WORKSTATION          : str = "ERROR_INVALID_WORKSTATION";
        case ERROR_PASSWORD_EXPIRED             : str = "ERROR_PASSWORD_EXPIRED";
        case ERROR_ACCOUNT_DISABLED             : str = "ERROR_ACCOUNT_DISABLED";
        case ERROR_NONE_MAPPED                  : str = "ERROR_NONE_MAPPED";
        case ERROR_TOO_MANY_LUIDS_REQUESTED     : str = "ERROR_TOO_MANY_LUIDS_REQUESTED";
        case ERROR_LUIDS_EXHAUSTED              : str = "ERROR_LUIDS_EXHAUSTED";
        case ERROR_INVALID_SUB_AUTHORITY        : str = "ERROR_INVALID_SUB_AUTHORITY";
        case ERROR_INVALID_ACL                  : str = "ERROR_INVALID_ACL";
        case ERROR_INVALID_SID                  : str = "ERROR_INVALID_SID";
        case ERROR_INVALID_SECURITY_DESCR       : str = "ERROR_INVALID_SECURITY_DESCR";
        case ERROR_BAD_INHERITANCE_ACL          : str = "ERROR_BAD_INHERITANCE_ACL";
        case ERROR_SERVER_DISABLED              : str = "ERROR_SERVER_DISABLED";
        case ERROR_SERVER_NOT_DISABLED          : str = "ERROR_SERVER_NOT_DISABLED";
        case ERROR_INVALID_ID_AUTHORITY         : str = "ERROR_INVALID_ID_AUTHORITY";
        case ERROR_ALLOTTED_SPACE_EXCEEDED      : str = "ERROR_ALLOTTED_SPACE_EXCEEDED";
        case ERROR_INVALID_GROUP_ATTRIBUTES     : str = "ERROR_INVALID_GROUP_ATTRIBUTES";
        case ERROR_BAD_IMPERSONATION_LEVEL      : str = "ERROR_BAD_IMPERSONATION_LEVEL";
        case ERROR_CANT_OPEN_ANONYMOUS          : str = "ERROR_CANT_OPEN_ANONYMOUS";
        case ERROR_BAD_VALIDATION_CLASS         : str = "ERROR_BAD_VALIDATION_CLASS";
        case ERROR_BAD_TOKEN_TYPE               : str = "ERROR_BAD_TOKEN_TYPE";
        case ERROR_NO_SECURITY_ON_OBJECT        : str = "ERROR_NO_SECURITY_ON_OBJECT";
        case ERROR_CANT_ACCESS_DOMAIN_INFO      : str = "ERROR_CANT_ACCESS_DOMAIN_INFO";
        case ERROR_INVALID_SERVER_STATE         : str = "ERROR_INVALID_SERVER_STATE";
        case ERROR_INVALID_DOMAIN_STATE         : str = "ERROR_INVALID_DOMAIN_STATE";
        case ERROR_INVALID_DOMAIN_ROLE          : str = "ERROR_INVALID_DOMAIN_ROLE";
        case ERROR_NO_SUCH_DOMAIN               : str = "ERROR_NO_SUCH_DOMAIN";
        case ERROR_DOMAIN_EXISTS                : str = "ERROR_DOMAIN_EXISTS";
        case ERROR_DOMAIN_LIMIT_EXCEEDED        : str = "ERROR_DOMAIN_LIMIT_EXCEEDED";
        case ERROR_INTERNAL_DB_CORRUPTION       : str = "ERROR_INTERNAL_DB_CORRUPTION";
        case ERROR_INTERNAL_ERROR               : str = "ERROR_INTERNAL_ERROR";
        case ERROR_GENERIC_NOT_MAPPED           : str = "ERROR_GENERIC_NOT_MAPPED";
        case ERROR_BAD_DESCRIPTOR_FORMAT        : str = "ERROR_BAD_DESCRIPTOR_FORMAT";
        case ERROR_NOT_LOGON_PROCESS            : str = "ERROR_NOT_LOGON_PROCESS";
        case ERROR_LOGON_SESSION_EXISTS         : str = "ERROR_LOGON_SESSION_EXISTS";
        case ERROR_NO_SUCH_PACKAGE              : str = "ERROR_NO_SUCH_PACKAGE";
        case ERROR_BAD_LOGON_SESSION_STATE      : str = "ERROR_BAD_LOGON_SESSION_STATE";
        case ERROR_LOGON_SESSION_COLLISION      : str = "ERROR_LOGON_SESSION_COLLISION";
        case ERROR_INVALID_LOGON_TYPE           : str = "ERROR_INVALID_LOGON_TYPE";
        case ERROR_CANNOT_IMPERSONATE           : str = "ERROR_CANNOT_IMPERSONATE";
        case ERROR_RXACT_INVALID_STATE          : str = "ERROR_RXACT_INVALID_STATE";
        case ERROR_RXACT_COMMIT_FAILURE         : str = "ERROR_RXACT_COMMIT_FAILURE";
        case ERROR_SPECIAL_ACCOUNT              : str = "ERROR_SPECIAL_ACCOUNT";
        case ERROR_SPECIAL_GROUP                : str = "ERROR_SPECIAL_GROUP";
        case ERROR_SPECIAL_USER                 : str = "ERROR_SPECIAL_USER";
        case ERROR_MEMBERS_PRIMARY_GROUP        : str = "ERROR_MEMBERS_PRIMARY_GROUP";
        case ERROR_TOKEN_ALREADY_IN_USE         : str = "ERROR_TOKEN_ALREADY_IN_USE";
        case ERROR_NO_SUCH_ALIAS                : str = "ERROR_NO_SUCH_ALIAS";
        case ERROR_MEMBER_NOT_IN_ALIAS          : str = "ERROR_MEMBER_NOT_IN_ALIAS";
        case ERROR_MEMBER_IN_ALIAS              : str = "ERROR_MEMBER_IN_ALIAS";
        case ERROR_ALIAS_EXISTS                 : str = "ERROR_ALIAS_EXISTS";
        case ERROR_LOGON_NOT_GRANTED            : str = "ERROR_LOGON_NOT_GRANTED";
        case ERROR_TOO_MANY_SECRETS             : str = "ERROR_TOO_MANY_SECRETS";
        case ERROR_SECRET_TOO_LONG              : str = "ERROR_SECRET_TOO_LONG";
        case ERROR_INTERNAL_DB_ERROR            : str = "ERROR_INTERNAL_DB_ERROR";
        case ERROR_TOO_MANY_CONTEXT_IDS         : str = "ERROR_TOO_MANY_CONTEXT_IDS";
        case ERROR_LOGON_TYPE_NOT_GRANTED       : str = "ERROR_LOGON_TYPE_NOT_GRANTED";
        case ERROR_NO_SUCH_MEMBER               : str = "ERROR_NO_SUCH_MEMBER";
        case ERROR_INVALID_MEMBER               : str = "ERROR_INVALID_MEMBER";
        case ERROR_TOO_MANY_SIDS                : str = "ERROR_TOO_MANY_SIDS";
        case ERROR_NO_INHERITANCE               : str = "ERROR_NO_INHERITANCE";
        case ERROR_FILE_CORRUPT                 : str = "ERROR_FILE_CORRUPT";
        case ERROR_DISK_CORRUPT                 : str = "ERROR_DISK_CORRUPT";
        case ERROR_NO_USER_SESSION_KEY          : str = "ERROR_NO_USER_SESSION_KEY";
        case ERROR_INVALID_WINDOW_HANDLE        : str = "ERROR_INVALID_WINDOW_HANDLE";
        case ERROR_INVALID_MENU_HANDLE          : str = "ERROR_INVALID_MENU_HANDLE";
        case ERROR_INVALID_CURSOR_HANDLE        : str = "ERROR_INVALID_CURSOR_HANDLE";
        case ERROR_INVALID_ACCEL_HANDLE         : str = "ERROR_INVALID_ACCEL_HANDLE";
        case ERROR_INVALID_HOOK_HANDLE          : str = "ERROR_INVALID_HOOK_HANDLE";
        case ERROR_INVALID_DWP_HANDLE           : str = "ERROR_INVALID_DWP_HANDLE";
        case ERROR_TLW_WITH_WSCHILD             : str = "ERROR_TLW_WITH_WSCHILD";
        case ERROR_CANNOT_FIND_WND_CLASS        : str = "ERROR_CANNOT_FIND_WND_CLASS";
        case ERROR_WINDOW_OF_OTHER_THREAD       : str = "ERROR_WINDOW_OF_OTHER_THREAD";
        case ERROR_HOTKEY_ALREADY_REGISTERED    : str = "ERROR_HOTKEY_ALREADY_REGISTERED";
        case ERROR_CLASS_ALREADY_EXISTS         : str = "ERROR_CLASS_ALREADY_EXISTS";
        case ERROR_CLASS_DOES_NOT_EXIST         : str = "ERROR_CLASS_DOES_NOT_EXIST";
        case ERROR_CLASS_HAS_WINDOWS            : str = "ERROR_CLASS_HAS_WINDOWS";
        case ERROR_INVALID_INDEX                : str = "ERROR_INVALID_INDEX";
        case ERROR_INVALID_ICON_HANDLE          : str = "ERROR_INVALID_ICON_HANDLE";
        case ERROR_PRIVATE_DIALOG_INDEX         : str = "ERROR_PRIVATE_DIALOG_INDEX";
        case ERROR_LISTBOX_ID_NOT_FOUND         : str = "ERROR_LISTBOX_ID_NOT_FOUND";
        case ERROR_NO_WILDCARD_CHARACTERS       : str = "ERROR_NO_WILDCARD_CHARACTERS";
        case ERROR_CLIPBOARD_NOT_OPEN           : str = "ERROR_CLIPBOARD_NOT_OPEN";
        case ERROR_HOTKEY_NOT_REGISTERED        : str = "ERROR_HOTKEY_NOT_REGISTERED";
        case ERROR_WINDOW_NOT_DIALOG            : str = "ERROR_WINDOW_NOT_DIALOG";
        case ERROR_CONTROL_ID_NOT_FOUND         : str = "ERROR_CONTROL_ID_NOT_FOUND";
        case ERROR_INVALID_COMBOBOX_MESSAGE     : str = "ERROR_INVALID_COMBOBOX_MESSAGE";
        case ERROR_WINDOW_NOT_COMBOBOX          : str = "ERROR_WINDOW_NOT_COMBOBOX";
        case ERROR_INVALID_EDIT_HEIGHT          : str = "ERROR_INVALID_EDIT_HEIGHT";
        case ERROR_DC_NOT_FOUND                 : str = "ERROR_DC_NOT_FOUND";
        case ERROR_INVALID_HOOK_FILTER          : str = "ERROR_INVALID_HOOK_FILTER";
        case ERROR_INVALID_FILTER_PROC          : str = "ERROR_INVALID_FILTER_PROC";
        case ERROR_HOOK_NEEDS_HMOD              : str = "ERROR_HOOK_NEEDS_HMOD";
        case ERROR_GLOBAL_ONLY_HOOK             : str = "ERROR_GLOBAL_ONLY_HOOK";
        case ERROR_JOURNAL_HOOK_SET             : str = "ERROR_JOURNAL_HOOK_SET";
        case ERROR_HOOK_NOT_INSTALLED           : str = "ERROR_HOOK_NOT_INSTALLED";
        case ERROR_INVALID_LB_MESSAGE           : str = "ERROR_INVALID_LB_MESSAGE";
        case ERROR_SETCOUNT_ON_BAD_LB           : str = "ERROR_SETCOUNT_ON_BAD_LB";
        case ERROR_LB_WITHOUT_TABSTOPS          : str = "ERROR_LB_WITHOUT_TABSTOPS";
        case ERROR_CHILD_WINDOW_MENU            : str = "ERROR_CHILD_WINDOW_MENU";
        case ERROR_NO_SYSTEM_MENU               : str = "ERROR_NO_SYSTEM_MENU";
        case ERROR_INVALID_MSGBOX_STYLE         : str = "ERROR_INVALID_MSGBOX_STYLE";
        case ERROR_INVALID_SPI_VALUE            : str = "ERROR_INVALID_SPI_VALUE";
        case ERROR_SCREEN_ALREADY_LOCKED        : str = "ERROR_SCREEN_ALREADY_LOCKED";
        case ERROR_HWNDS_HAVE_DIFF_PARENT       : str = "ERROR_HWNDS_HAVE_DIFF_PARENT";
        case ERROR_NOT_CHILD_WINDOW             : str = "ERROR_NOT_CHILD_WINDOW";
        case ERROR_INVALID_GW_COMMAND           : str = "ERROR_INVALID_GW_COMMAND";
        case ERROR_INVALID_THREAD_ID            : str = "ERROR_INVALID_THREAD_ID";
        case ERROR_NON_MDICHILD_WINDOW          : str = "ERROR_NON_MDICHILD_WINDOW";
        case ERROR_POPUP_ALREADY_ACTIVE         : str = "ERROR_POPUP_ALREADY_ACTIVE";
        case ERROR_NO_SCROLLBARS                : str = "ERROR_NO_SCROLLBARS";
        case ERROR_INVALID_SCROLLBAR_RANGE      : str = "ERROR_INVALID_SCROLLBAR_RANGE";
        case ERROR_INVALID_SHOWWIN_COMMAND      : str = "ERROR_INVALID_SHOWWIN_COMMAND";
        case ERROR_EVENTLOG_FILE_CORRUPT        : str = "ERROR_EVENTLOG_FILE_CORRUPT";
        case ERROR_EVENTLOG_CANT_START          : str = "ERROR_EVENTLOG_CANT_START";
        case ERROR_LOG_FILE_FULL                : str = "ERROR_LOG_FILE_FULL";
        case ERROR_EVENTLOG_FILE_CHANGED        : str = "ERROR_EVENTLOG_FILE_CHANGED";
        case ERROR_INVALID_USER_BUFFER          : str = "ERROR_INVALID_USER_BUFFER";
        case ERROR_UNRECOGNIZED_MEDIA           : str = "ERROR_UNRECOGNIZED_MEDIA";
        case ERROR_NO_TRUST_LSA_SECRET          : str = "ERROR_NO_TRUST_LSA_SECRET";
        case ERROR_NO_TRUST_SAM_ACCOUNT         : str = "ERROR_NO_TRUST_SAM_ACCOUNT";
        case ERROR_TRUSTED_DOMAIN_FAILURE       : str = "ERROR_TRUSTED_DOMAIN_FAILURE";
        case ERROR_NETLOGON_NOT_STARTED         : str = "ERROR_NETLOGON_NOT_STARTED";
        case ERROR_ACCOUNT_EXPIRED              : str = "ERROR_ACCOUNT_EXPIRED";
        case ERROR_UNKNOWN_PORT                 : str = "ERROR_UNKNOWN_PORT";
        case ERROR_UNKNOWN_PRINTER_DRIVER       : str = "ERROR_UNKNOWN_PRINTER_DRIVER";
        case ERROR_UNKNOWN_PRINTPROCESSOR       : str = "ERROR_UNKNOWN_PRINTPROCESSOR";
        case ERROR_INVALID_SEPARATOR_FILE       : str = "ERROR_INVALID_SEPARATOR_FILE";
        case ERROR_INVALID_PRIORITY             : str = "ERROR_INVALID_PRIORITY";
        case ERROR_INVALID_PRINTER_NAME         : str = "ERROR_INVALID_PRINTER_NAME";
        case ERROR_PRINTER_ALREADY_EXISTS       : str = "ERROR_PRINTER_ALREADY_EXISTS";
        case ERROR_INVALID_PRINTER_COMMAND      : str = "ERROR_INVALID_PRINTER_COMMAND";
        case ERROR_INVALID_DATATYPE             : str = "ERROR_INVALID_DATATYPE";
        case ERROR_INVALID_ENVIRONMENT          : str = "ERROR_INVALID_ENVIRONMENT";
        case ERROR_DOMAIN_TRUST_INCONSISTENT    : str = "ERROR_DOMAIN_TRUST_INCONSISTENT";
        case ERROR_SERVER_HAS_OPEN_HANDLES      : str = "ERROR_SERVER_HAS_OPEN_HANDLES";
        case ERROR_RESOURCE_DATA_NOT_FOUND      : str = "ERROR_RESOURCE_DATA_NOT_FOUND";
        case ERROR_RESOURCE_TYPE_NOT_FOUND      : str = "ERROR_RESOURCE_TYPE_NOT_FOUND";
        case ERROR_RESOURCE_NAME_NOT_FOUND      : str = "ERROR_RESOURCE_NAME_NOT_FOUND";
        case ERROR_RESOURCE_LANG_NOT_FOUND      : str = "ERROR_RESOURCE_LANG_NOT_FOUND";
        case ERROR_NOT_ENOUGH_QUOTA             : str = "ERROR_NOT_ENOUGH_QUOTA";
        case ERROR_INVALID_TIME                 : str = "ERROR_INVALID_TIME";
        case ERROR_INVALID_FORM_NAME            : str = "ERROR_INVALID_FORM_NAME";
        case ERROR_INVALID_FORM_SIZE            : str = "ERROR_INVALID_FORM_SIZE";
        case ERROR_ALREADY_WAITING              : str = "ERROR_ALREADY_WAITING";
        case ERROR_PRINTER_DELETED              : str = "ERROR_PRINTER_DELETED";
        case ERROR_INVALID_PRINTER_STATE        : str = "ERROR_INVALID_PRINTER_STATE";
        case ERROR_PASSWORD_MUST_CHANGE         : str = "ERROR_PASSWORD_MUST_CHANGE";
        case ERROR_ACCOUNT_LOCKED_OUT           : str = "ERROR_ACCOUNT_LOCKED_OUT";
        case ERROR_NO_BROWSER_SERVERS_FOUND     : str = "ERROR_NO_BROWSER_SERVERS_FOUND";
        case ERROR_INVALID_PIXEL_FORMAT         : str = "ERROR_INVALID_PIXEL_FORMAT";
        case ERROR_BAD_DRIVER                   : str = "ERROR_BAD_DRIVER";
        case ERROR_INVALID_WINDOW_STYLE         : str = "ERROR_INVALID_WINDOW_STYLE";
        case ERROR_METAFILE_NOT_SUPPORTED       : str = "ERROR_METAFILE_NOT_SUPPORTED";
        case ERROR_TRANSFORM_NOT_SUPPORTED      : str = "ERROR_TRANSFORM_NOT_SUPPORTED";
        case ERROR_CLIPPING_NOT_SUPPORTED       : str = "ERROR_CLIPPING_NOT_SUPPORTED";
        case ERROR_UNKNOWN_PRINT_MONITOR        : str = "ERROR_UNKNOWN_PRINT_MONITOR";
        case ERROR_PRINTER_DRIVER_IN_USE        : str = "ERROR_PRINTER_DRIVER_IN_USE";
        case ERROR_SPOOL_FILE_NOT_FOUND         : str = "ERROR_SPOOL_FILE_NOT_FOUND";
        case ERROR_SPL_NO_STARTDOC              : str = "ERROR_SPL_NO_STARTDOC";
        case ERROR_SPL_NO_ADDJOB                : str = "ERROR_SPL_NO_ADDJOB";
        case ERROR_WINS_INTERNAL                : str = "ERROR_WINS_INTERNAL";
        case ERROR_CAN_NOT_DEL_LOCAL_WINS       : str = "ERROR_CAN_NOT_DEL_LOCAL_WINS";
        case ERROR_STATIC_INIT                  : str = "ERROR_STATIC_INIT";
        case ERROR_INC_BACKUP                   : str = "ERROR_INC_BACKUP";
        case ERROR_FULL_BACKUP                  : str = "ERROR_FULL_BACKUP";
        case ERROR_REC_NON_EXISTENT             : str = "ERROR_REC_NON_EXISTENT";
        case ERROR_RPL_NOT_ALLOWED              : str = "ERROR_RPL_NOT_ALLOWED";
        }
    return(str);
}

//////////////////////////////////////////////////////////////////////////

int MxPad::Resolve()

{
	char	bc  = 1;
	int		bclen  = sizeof(char);
	int     res2 = 0;
	unsigned long     res;
	unsigned int     tme;
	
	#ifndef _DEBUG
		return 0;
	#endif

	sock = socket( AF_INET, SOCK_DGRAM, 0);
	//printf("After sock %d\n",sock);

	int ret_val = setsockopt (sock, SOL_SOCKET, SO_BROADCAST, &bc, sizeof(int));

	//TRACE("After sockoption %d\n", ret_val);

	//ret_val = getsockopt (sock, SOL_SOCKET, SO_BROADCAST, &bc, &bclen);
	//printf("After getsockoption %d ---  %d\n", ret_val, bc);

	local_sin.sin_addr.s_addr = INADDR_BROADCAST;
	local_sin.sin_port = htons(2000);
	local_sin.sin_family = AF_INET;
	acc_sin_len = sizeof(local_sin);

	ret_val = sendto(sock, "mxpad", 6, 0, (struct sockaddr *)&local_sin, acc_sin_len);
	//printf("After sendto %d \n", ret_val);

	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) AcceptThreadProc,
														   &res2, 0, &res);
	//printf("Created: %x\n", res);

	tme = GetTickCount() + 1000;

	while(1)
		{
		MSG  msg;

		if(tme < GetTickCount())		// Timeout
			break;

		if(res2)                        // Done
			break;

		// Let windows do its thing ...

		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
			TranslateMessage(&msg), DispatchMessage(&msg);
			}
		}

	if(res2)
		{
		// Address in rec_sin2
		resolved = true;
		}

	return resolved;
}


MxPad::Connect()

{
	SOCKADDR_IN     dest_sin;                 /* Local socket - internet style */

	#ifndef _DEBUG
		return 0;
	#endif


	int nd = 1, ndlen = sizeof(int);

	sock2 = socket( AF_INET, SOCK_STREAM, 0);

	if(sock2 == INVALID_SOCKET)
		{
		MessageBox(NULL, "no socket", "", MB_OK);
		}

	dest_sin.sin_family = AF_INET;
	dest_sin.sin_port = htons(2002);

	//dest_sin.sin_addr.s_addr = inet_addr("127.0.0.1");
	dest_sin.sin_addr.s_addr = rec_sin2.sin_addr.s_addr;
	
	if (connect( sock2, (PSOCKADDR) &dest_sin, sizeof( dest_sin)) < 0)
		{
		#ifdef PG_DEBUG
		//PrintToNotepad("SER32: Cannot connect back.\r\n");
		//mxMBOX("cannot connect", MB_OK);
		TRACE("cannot connect");
		#endif

		return 0;
		}
	connected = true;

	return connected;
}

//////////////////////////////////////////////////////////////////////////
/// \brief The Async version of printf. 
/// Will output to temporary buffer so it is really fast. 
/// Can be used to debug real time code, as it
/// writes to memory, and has no IO.\n
/// It is also useful to debug constructors of global variables, as the 
/// CRT is not operational on global construct time. (go figure)

int     MxPad::aprintf(const char *Format, ...)

{
	#ifndef _DEBUG
		if(!force)
			return (0);
	#endif

	// Start thread if none
	//if(!handle)
	//	{
	//	handle = _beginthread(RunMxThread, 0, this);
	//	}

    va_list ArgList;
    va_start(ArgList, Format);

    int len = _vsnprintf(aszOutString, OUTLEN - 1, Format, ArgList);

	// Terminate if overflowed
	if(len < 0)	
		{
		aszOutString[OUTLEN - 1] = '\0';
		}

	for(int loop = 0; loop < len; loop++)
		{
		if(!circ.Add(aszOutString[loop]))
			{
			break;
			}
		}
    
	if(len < 0)	
		{
		ToNotepad("\r\nWarning: Output string was cut to prevent buffer overflow.\r\n");
		}

    return(0);
}

//////////////////////////////////////////////////////////////////////////

MxPad::CxCircular::CxCircular()

{
	size = CIRC_DEF_SIZE;
	InitVars();
}

MxPad::CxCircular::CxCircular(int bufsize)

{
	size = bufsize;
	InitVars();
}

MxPad::CxCircular::InitVars()

{
	head = tail = 0;

	buff = (char *)malloc(size);
	//ASSERT(buff);

	return 0;
}

MxPad::CxCircular::~CxCircular()

{
	if(buff) 
		free (buff);
}

//////////////////////////////////////////////////////////////////////////
//
//  ---------------------------------
//	    ^tail        ^head
//
//  ---------------------------------
//	           ^head       ^tail 
//

/// Internal: Add to circular buffer

int MxPad::CxCircular::Add(char chh)

{
	// Full?
	if (head == tail - 1)
		return false;
	
	// Full? - special case
	if((tail == 0) && (head == size - 1))
		return false;

	buff[head] = chh;

	head++;

	if(head >= size)
		head = 0;

	return true;
}

//////////////////////////////////////////////////////////////////////////

/// Internal: read char from circular buffer

int MxPad::CxCircular::Get(char *chh)

{
	// Empty?
	if (head == tail)
		return false;

	*chh = 	buff[tail];

	tail++;

	if(tail >= size)
		tail = 0;

	return true;
}

#ifdef CWnd

/// Helper: dump window styles as textual information

void MxPad::DumpStyles(CWnd *ptr)

{
	P2N("Window Styles:\r\n");
	DWORD style = ::GetWindowLong(ptr->m_hWnd, GWL_STYLE);

	if(style & WS_BORDER   )
		P2N("WS_BORDER   ");
	if(style & WS_CAPTION   )
		P2N("WS_CAPTION   ");
	if(style & WS_CHILD   )
		P2N("WS_CHILD   ");
	if(style & WS_CLIPCHILDREN   )
		P2N("WS_CLIPCHILDREN   ");
	if(style & WS_CLIPSIBLINGS   )
		P2N("WS_CLIPSIBLINGS   ");
	if(style & WS_DISABLED   )
		P2N("WS_DISABLED   ");
	if(style & WS_DLGFRAME   )
		P2N("WS_DLGFRAME   ");
	if(style & WS_GROUP   )
		P2N("WS_GROUP   ");
	if(style & WS_HSCROLL   )
		P2N("WS_HSCROLL   ");
	if(style & WS_MAXIMIZE   )
		P2N("WS_MAXIMIZE   ");
	if(style & WS_MAXIMIZEBOX   )
		P2N("WS_MAXIMIZEBOX   ");
	if(style & WS_MINIMIZE   )
		P2N("WS_MINIMIZE   ");
	if(style & WS_MINIMIZEBOX   )
		P2N("WS_MINIMIZEBOX   ");
	if(style & WS_OVERLAPPED   )
		P2N("WS_OVERLAPPED   ");
	if(style & WS_OVERLAPPEDWINDOW   )
		P2N("WS_OVERLAPPEDWINDOW   ");
	if(style & WS_POPUP   )
		P2N("WS_POPUP   ");
	if(style & WS_POPUPWINDOW   )
		P2N("WS_POPUPWINDOW   ");
	if(style & WS_SYSMENU   )
		P2N("WS_SYSMENU   ");
	if(style & WS_TABSTOP   )
		P2N("WS_TABSTOP   ");
	if(style & WS_THICKFRAME   )
		P2N("WS_THICKFRAME   ");
	if(style & WS_VISIBLE   )
		P2N("WS_VISIBLE   ");
	if(style & WS_VSCROLL   )
		P2N("WS_VSCROLL   ");

	P2N("\r\n");
}

/// Helper: Dump extended styles as textual information

void MxPad::DumpEdS(CWnd *ptr)

{
	// CEdit

	P2N("Edit Window Styles:\r\n");
	DWORD style = ::GetWindowLong(ptr->m_hWnd, GWL_STYLE);

	if(style & ES_AUTOHSCROLL   )
		P2N("  ES_AUTOHSCROLL  ");
	if(style & ES_AUTOVSCROLL   )
		P2N("  ES_AUTOVSCROLL  ");
	if(style & ES_CENTER   )
		P2N("  ES_CENTER  ");
	if(style & ES_LEFT   )
		P2N("  ES_LEFT  ");
	if(style & ES_LOWERCASE   )
		P2N("  ES_LOWERCASE  ");
	if(style & ES_MULTILINE   )
		P2N("  ES_MULTILINE  ");
	if(style & ES_NOHIDESEL   )
		P2N("  ES_NOHIDESEL  ");
	if(style & ES_OEMCONVERT   )
		P2N("  ES_OEMCONVERT  ");
	if(style & ES_PASSWORD   )
		P2N("  ES_PASSWORD  ");
	if(style & ES_RIGHT   )
		P2N("  ES_RIGHT  ");
	if(style & ES_UPPERCASE   )
		P2N("  ES_UPPERCASE  ");
	if(style & ES_READONLY   )
		P2N("  ES_READONLY  ");
	if(style & ES_WANTRETURN   )
		P2N("  ES_WANTRETURN  ");

	P2N("\r\n");

}

/// Helper: Dump static styles as textual information

void MxPad::DumpSS(CWnd *ptr)

{
	// CStatic 

	P2N("Static Window Styles:\r\n");
	DWORD style = ::GetWindowLong(ptr->m_hWnd, GWL_STYLE);

	if(style & SS_BLACKFRAME   )
		P2N("  SS_BLACKFRAME   ");
	if(style & SS_BLACKRECT   )
		P2N("  SS_BLACKRECT   ");
	if(style & SS_CENTER   )
		P2N("  SS_CENTER   ");
	if(style & SS_GRAYFRAME   )
		P2N("  SS_GRAYFRAME   ");
	if(style & SS_GRAYRECT   )
		P2N("  SS_GRAYRECT   ");
	if(style & SS_ICON   )
		P2N("  SS_ICON   ");
	if(style & SS_LEFT   )
		P2N("  SS_LEFT   ");
	if(style & SS_LEFTNOWORDWRAP   )
		P2N("  SS_LEFTNOWORDWRAP   ");
	if(style & SS_NOPREFIX   )
		P2N("  SS_NOPREFIX   ");
	if(style & SS_RIGHT   )
		P2N("  SS_RIGHT   ");
	if(style & SS_SIMPLE   )
		P2N("  SS_SIMPLE   ");
	if(style & SS_USERITEM   )
		P2N("  SS_USERITEM   ");
	if(style & SS_WHITEFRAME   )
		P2N("  SS_WHITEFRAME   ");
	if(style & SS_WHITERECT   )
		P2N("  SS_WHITERECT   ");

	
	P2N("\r\n");
}

/// Helper: Dump extended styles as textual information

void MxPad::DumpExStyles(CWnd *ptr)

{
	P2N("Extended Window Styles:\r\n");
	DWORD style = ::GetWindowLong(ptr->m_hWnd, GWL_EXSTYLE);

	if(style & WS_EX_ACCEPTFILES   )
		P2N("  WS_EX_ACCEPTFILES  ");
	if(style & WS_EX_CLIENTEDGE   )
		P2N("WS_EX_CLIENTEDGE  ");
	if(style & WS_EX_CONTEXTHELP   )
		P2N("WS_EX_CONTEXTHELP  ");
	if(style & WS_EX_CONTROLPARENT   )
		P2N("WS_EX_CONTROLPARENT  ");
	if(style & WS_EX_DLGMODALFRAME   )
		P2N("WS_EX_DLGMODALFRAME  ");
	if(style & WS_EX_LEFT   )
		P2N("WS_EX_LEFT  ");
	if(style & WS_EX_LEFTSCROLLBAR   )
		P2N("WS_EX_LEFTSCROLLBAR  ");
	if(style & WS_EX_LTRREADING   )
		P2N("WS_EX_LTRREADING  ");
	if(style & WS_EX_MDICHILD   )
		P2N("WS_EX_MDICHILD  ");
	if(style & WS_EX_NOPARENTNOTIFY   )
		P2N("WS_EX_NOPARENTNOTIFY  ");
	if(style & WS_EX_OVERLAPPEDWINDOW   )
		P2N("WS_EX_OVERLAPPEDWINDOW  ");
	if(style & WS_EX_PALETTEWINDOW   )
		P2N("WS_EX_PALETTEWINDOW  ");
	if(style & WS_EX_RIGHT   )
		P2N("WS_EX_RIGHT  ");
	if(style & WS_EX_RIGHTSCROLLBAR   )
		P2N("WS_EX_RIGHTSCROLLBAR  ");
	if(style & WS_EX_RTLREADING   )
		P2N("WS_EX_RTLREADING  ");
	if(style & WS_EX_STATICEDGE   )
		P2N("WS_EX_STATICEDGE  ");
	if(style & WS_EX_TOOLWINDOW   )
		P2N("WS_EX_TOOLWINDOW  ");
	if(style & WS_EX_TOPMOST   )
		P2N("WS_EX_TOPMOST  ");
	if(style & WS_EX_TRANSPARENT   )
		P2N("WS_EX_TRANSPARENT  ");
	if(style & WS_EX_WINDOWEDGE   )
		P2N("WS_EX_WINDOWEDGE  ");

	P2N("\r\n");
}

#endif

/// Helper: convert keystroke integer to textual 

char	*MxPad::vkey2keystr(int code)

{
	static 	char xx[5];
	char *str = "none";
	
	//P2N("MxPad::vkey2keystr code = %d\r\n", code);

	if(isupper(code))
		{	
		xx[0]  = code;	xx[1]  = 0;	
		str = xx;
		}
	else switch(code)
		{
		case VK_LBUTTON: str= "VK_LBUTTON"; break;
		case VK_RBUTTON: str= "VK_RBUTTON"; break;
		case VK_CANCEL: str= "VK_CANCEL"; break;
		case VK_MBUTTON: str= "VK_MBUTTON"; break;
		case VK_BACK: str= "VK_BACK"; break;
		case VK_TAB: str= "VK_TAB"; break;
		case VK_CLEAR: str= "VK_CLEAR"; break;
		case VK_RETURN: str= "VK_RETURN"; break;
		case VK_SHIFT: str= "VK_SHIFT"; break;
		case VK_CONTROL: str= "VK_CONTROL"; break;
		case VK_MENU: str= "VK_MENU"; break;
		case VK_PAUSE: str= "VK_PAUSE"; break;
		case VK_CAPITAL: str= "VK_CAPITAL"; break;
		case VK_KANA: str= "VK_KANA"; break;
		case VK_JUNJA: str= "VK_JUNJA"; break;
		case VK_FINAL: str= "VK_FINAL"; break;
		case VK_HANJA: str= "VK_HANJA"; break;
		case VK_ESCAPE: str= "VK_ESCAPE"; break;
		case VK_CONVERT: str= "VK_CONVERT"; break;
		case VK_NONCONVERT: str= "VK_NONCONVERT"; break;
		case VK_ACCEPT: str= "VK_ACCEPT"; break;
		case VK_MODECHANGE: str= "VK_MODECHANGE"; break;
		case VK_SPACE: str= "VK_SPACE"; break;
		case VK_PRIOR: str= "VK_PRIOR"; break;
		case VK_NEXT: str= "VK_NEXT"; break;
		case VK_END: str= "VK_END"; break;
		case VK_HOME: str= "VK_HOME"; break;
		case VK_LEFT: str= "VK_LEFT"; break;
		case VK_UP: str= "VK_UP"; break;
		case VK_RIGHT: str= "VK_RIGHT"; break;
		case VK_DOWN: str= "VK_DOWN"; break;
		case VK_SELECT: str= "VK_SELECT"; break;
		case VK_PRINT: str= "VK_PRINT"; break;
		case VK_EXECUTE: str= "VK_EXECUTE"; break;
		case VK_SNAPSHOT: str= "VK_SNAPSHOT"; break;
		case VK_INSERT: str= "VK_INSERT"; break;
		case VK_DELETE: str= "VK_DELETE"; break;
		case VK_HELP: str= "VK_HELP"; break;
		case VK_LWIN: str= "VK_LWIN"; break;
		case VK_RWIN: str= "VK_RWIN"; break;
		case VK_APPS: str= "VK_APPS"; break;
		case VK_NUMPAD0: str= "VK_NUMPAD0"; break;
		case VK_NUMPAD1: str= "VK_NUMPAD1"; break;
		case VK_NUMPAD2: str= "VK_NUMPAD2"; break;
		case VK_NUMPAD3: str= "VK_NUMPAD3"; break;
		case VK_NUMPAD4: str= "VK_NUMPAD4"; break;
		case VK_NUMPAD5: str= "VK_NUMPAD5"; break;
		case VK_NUMPAD6: str= "VK_NUMPAD6"; break;
		case VK_NUMPAD7: str= "VK_NUMPAD7"; break;
		case VK_NUMPAD8: str= "VK_NUMPAD8"; break;
		case VK_NUMPAD9: str= "VK_NUMPAD9"; break;
		case VK_MULTIPLY: str= "VK_MULTIPLY"; break;
		case VK_ADD: str= "VK_ADD"; break;
		case VK_SEPARATOR: str= "VK_SEPARATOR"; break;
		case VK_SUBTRACT: str= "VK_SUBTRACT"; break;
		case VK_DECIMAL: str= "VK_DECIMAL"; break;
		case VK_DIVIDE: str= "VK_DIVIDE"; break;
		case VK_F1:	str="VK_F1"; break; 
		case VK_F2:	str="VK_F2"; break; 
		case VK_F3:	str="VK_F3"; break; 
		case VK_F4:	str="VK_F4"; break; 
		case VK_F5:	str="VK_F5"; break; 
		case VK_F6:	str="VK_F6"; break; 
		case VK_F7:	str="VK_F7"; break; 
		case VK_F8:	str="VK_F8"; break; 
		case VK_F9:	str="VK_F9"; break; 
		case VK_F10:	str="VK_F10"; break; 
		case VK_F11:	str="VK_F11"; break; 
		case VK_F12:	str="VK_F12"; break; 
		case VK_F13:	str="VK_F13"; break; 
		case VK_F14:	str="VK_F14"; break; 
		case VK_F15:	str="VK_F15"; break; 
		case VK_F16:	str="VK_F16"; break; 
		case VK_F17:	str="VK_F17"; break; 
		case VK_F18:	str="VK_F18"; break; 
		case VK_F19:	str="VK_F19"; break; 
		case VK_F20:	str="VK_F20"; break; 
		case VK_F21:	str="VK_F21"; break; 
		case VK_F22:	str="VK_F22"; break; 
		case VK_F23:	str="VK_F23"; break; 
		case VK_F24:	str="VK_F24"; break; 
		case VK_NUMLOCK:	str="VK_NUMLOCK"; break; 
		case VK_SCROLL:	str="VK_SCROLL"; break; 
		case VK_LSHIFT:	str="VK_LSHIFT"; break; 
		case VK_RSHIFT:	str="VK_RSHIFT"; break; 
		case VK_LCONTROL:	str="VK_LCONTROL"; break; 
		case VK_RCONTROL:	str="VK_RCONTROL"; break; 
		case VK_LMENU:	str="VK_LMENU"; break; 
		case VK_RMENU:	str="VK_RMENU"; break; 
		case VK_PROCESSKEY:	str="VK_PROCESSKEY"; break; 
		case VK_ATTN:	str="VK_ATTN"; break; 
		case VK_CRSEL:	str="VK_CRSEL"; break; 
		case VK_EXSEL:	str="VK_EXSEL"; break; 
		case VK_EREOF:	str="VK_EREOF"; break; 
		case VK_PLAY:	str="VK_PLAY"; break; 
		case VK_ZOOM:	str="VK_ZOOM"; break; 
		case VK_NONAME:	str="VK_NONAME"; break; 
		case VK_PA1:	str="VK_PA1"; break; 
		case VK_OEM_CLEAR:	str="VK_OEM_CLEAR"; break; 

		default: str = "no key";
		}
	return str;
}

/// Sets the output application name as returned by FindWindow().
/// Can be used to direct the debug output to any application that had the Edit
/// control at its core. (NotePad for example)
/// Make sure you know what you are doing, as it makes for a confused user
/// seeing programming data in notepad, (seemingly) coming from nowhere.
/// \n\n Must call this before any other function as the found window is cached.

void	MxPad::SetAppname(const char *str)

{
	strcpy(appname,  str);
}

///	Set up mxpad to autodetect conditions present

void	MxPad::Detect()

{
	#ifndef _DEBUG
		if(!force)
			return;
	#endif

	// Set up mxpad to autodetect
	int got = false;
	if(IsLocalAvail())
		{
		got++;
		//CString str; str.Format("Local Agent is running.\r\n");
		//TRACE(str);
		mxpad.SetLocal(true);
		}
	if(IsRemoteAvail())
		{
		got++;
		//CString str; str.Format("Remote Agent is running.\r\n");
		//TRACE(str);
		mxpad.SetRemote(true);
		}
	if(!got)
		{
#ifdef CString
		CString str; str.Format(
			"No debug agent is running, routing to TRACE instead.\r\n");
		TRACE(str);
	
#endif

		mxpad.SetTrace(true);
		}
	else
		{

#ifdef CString
		CString str; str.Format(
			"Started to log to MxPad debug agent.\r\n\r\n");
		TRACE(str);
#endif
		}
}


char *MxPad::werr2str(int err)

{
	char *str = "unknown error";

	switch (err)
		{
		case ERROR_SUCCESS : str = 	"The operation completed successfully.  "; break;
		case ERROR_INVALID_FUNCTION : str = 	"Incorrect function.  "; break;
		case ERROR_FILE_NOT_FOUND : str = 	"The system cannot find the file specified.  "; break;
		case ERROR_PATH_NOT_FOUND : str = 	"The system cannot find the path specified.  "; break;
		case ERROR_TOO_MANY_OPEN_FILES : str = 	"The system cannot open the file.  "; break;
		case ERROR_ACCESS_DENIED : str = 	"Access is denied.  "; break;
		case ERROR_INVALID_HANDLE : str = 	"The handle is invalid.  "; break;
		case ERROR_ARENA_TRASHED : str = 	"The storage control blocks were destroyed.  "; break;
		case ERROR_NOT_ENOUGH_MEMORY : str = 	"Not enough storage is available to process this command.  "; break;
		case ERROR_INVALID_BLOCK : str = 	"The storage control block address is invalid.  "; break;
		case ERROR_BAD_ENVIRONMENT : str = 	"The environment is incorrect.  "; break;
		case ERROR_BAD_FORMAT : str = 	"An attempt was made to load a program with an incorrect format.  "; break;
		case ERROR_INVALID_ACCESS : str = 	"The access code is invalid.  "; break;
		case ERROR_INVALID_DATA : str = 	"The data is invalid.  "; break;
		case ERROR_OUTOFMEMORY : str = 	"Not enough storage is available to complete this operation.  "; break;
		case ERROR_INVALID_DRIVE : str = 	"The system cannot find the drive specified.  "; break;
		case ERROR_CURRENT_DIRECTORY : str = 	"The directory cannot be removed.  "; break;
		case ERROR_NOT_SAME_DEVICE : str = 	"The system cannot move the file to a different disk drive.  "; break;
		case ERROR_NO_MORE_FILES : str = 	"There are no more files.  "; break;
		case ERROR_WRITE_PROTECT : str = 	"The media is write protected.  "; break;
		case ERROR_BAD_UNIT : str = 	"The system cannot find the device specified.  "; break;
		case ERROR_NOT_READY : str = 	"The device is not ready.  "; break;
		case ERROR_BAD_COMMAND : str = 	"The device does not recognize the command.  "; break;
		case ERROR_CRC : str = 	"Data error (cyclic redundancy check).  "; break;
		case ERROR_BAD_LENGTH : str = 	"The program issued a command but the command length is incorrect.  "; break;
		case ERROR_SEEK : str = 	"The drive cannot locate a specific area or track on the disk.  "; break;
		case ERROR_NOT_DOS_DISK : str = 	"The specified disk or diskette cannot be accessed.  "; break;
		case ERROR_SECTOR_NOT_FOUND : str = 	"The drive cannot find the sector requested.  "; break;
		case ERROR_OUT_OF_PAPER : str = 	"The printer is out of paper.  "; break;
		case ERROR_WRITE_FAULT : str = 	"The system cannot write to the specified device.  "; break;
		case ERROR_READ_FAULT : str = 	"The system cannot read from the specified device.  "; break;
		case ERROR_GEN_FAILURE : str = 	"A device attached to the system is not functioning.  "; break;
		case ERROR_SHARING_VIOLATION : str = 	"The process cannot access the file because it is being used by another process.  "; break;
		case ERROR_LOCK_VIOLATION : str = 	"The process cannot access the file because another process has locked a portion of the file.  "; break;
		case ERROR_WRONG_DISK : str = 	"The wrong diskette is in the drive. Insert %2 (Volume Serial Number: str =  %3) into drive %1.  "; break;
		case ERROR_SHARING_BUFFER_EXCEEDED : str = 	"Too many files opened for sharing.  "; break;
		case ERROR_HANDLE_EOF : str = 	"Reached the end of the file.  "; break;
		case ERROR_HANDLE_DISK_FULL : str = 	"The disk is full.  "; break;
		case ERROR_NOT_SUPPORTED : str = 	"The network request is not supported.  "; break;
		case ERROR_REM_NOT_LIST : str = 	"The remote computer is not available.  "; break;
		case ERROR_DUP_NAME : str = 	"A duplicate name exists on the network.  "; break;
		case ERROR_BAD_NETPATH : str = 	"The network path was not found.  "; break;
		case ERROR_NETWORK_BUSY : str = 	"The network is busy.  "; break;
		case ERROR_DEV_NOT_EXIST : str = 	"The specified network resource or device is no longer available.  "; break;
		case ERROR_TOO_MANY_CMDS : str = 	"The network BIOS command limit has been reached.  "; break;
		case ERROR_ADAP_HDW_ERR : str = 	"A network adapter hardware error occurred.  "; break;
		case ERROR_BAD_NET_RESP : str = 	"The specified server cannot perform the requested operation.  "; break;
		case ERROR_UNEXP_NET_ERR : str = 	"An unexpected network error occurred.  "; break;
		case ERROR_BAD_REM_ADAP : str = 	"The remote adapter is not compatible.  "; break;
		case ERROR_PRINTQ_FULL : str = 	"The printer queue is full.  "; break;
		case ERROR_NO_SPOOL_SPACE : str = 	"Space to store the file waiting to be printed is not available on the server.  "; break;
		case ERROR_PRINT_CANCELLED : str = 	"Your file waiting to be printed was deleted.  "; break;
		case ERROR_NETNAME_DELETED : str = 	"The specified network name is no longer available.  "; break;
		case ERROR_NETWORK_ACCESS_DENIED : str = 	"Network access is denied.  "; break;
		case ERROR_BAD_DEV_TYPE : str = 	"The network resource type is not correct.  "; break;
		case ERROR_BAD_NET_NAME : str = 	"The network name cannot be found.  "; break;
		case ERROR_TOO_MANY_NAMES : str = 	"The name limit for the local computer network adapter card was exceeded.  "; break;
		case ERROR_TOO_MANY_SESS : str = 	"The network BIOS session limit was exceeded.  "; break;
		case ERROR_SHARING_PAUSED : str = 	"The remote server has been paused or is in the process of being started.  "; break;
		case ERROR_REQ_NOT_ACCEP : str = 	"No more connections can be made to this remote computer at this time because there are already as many connections as the computer can accept.  "; break;
		case ERROR_REDIR_PAUSED : str = 	"The specified printer or disk device has been paused.  "; break;
		case ERROR_FILE_EXISTS : str = 	"The file exists.  "; break;
		case ERROR_CANNOT_MAKE : str = 	"The directory or file cannot be created.  "; break;
		case ERROR_FAIL_I24 : str = 	"Fail on INT 24.  "; break;
		case ERROR_OUT_OF_STRUCTURES : str = 	"Storage to process this request is not available.  "; break;
		case ERROR_ALREADY_ASSIGNED : str = 	"The local device name is already in use.  "; break;
		case ERROR_INVALID_PASSWORD : str = 	"The specified network password is not correct.  "; break;
		case ERROR_INVALID_PARAMETER : str = 	"The parameter is incorrect.  "; break;
		case ERROR_NET_WRITE_FAULT : str = 	"A write fault occurred on the network.  "; break;
		case ERROR_NO_PROC_SLOTS : str = 	"The system cannot start another process at this time.  "; break;
		case ERROR_TOO_MANY_SEMAPHORES : str = 	"Cannot create another system semaphore.  "; break;
		case ERROR_EXCL_SEM_ALREADY_OWNED : str = 	"The exclusive semaphore is owned by another process.  "; break;
		case ERROR_SEM_IS_SET : str = 	"The semaphore is set and cannot be closed.  "; break;
		case ERROR_TOO_MANY_SEM_REQUESTS : str = 	"The semaphore cannot be set again.  "; break;
		case ERROR_INVALID_AT_INTERRUPT_TIME : str = 	"Cannot request exclusive semaphores at interrupt time.  "; break;
		case ERROR_SEM_OWNER_DIED : str = 	"The previous ownership of this semaphore has ended.  "; break;
		case ERROR_SEM_USER_LIMIT : str = 	"Insert the diskette for drive %1.  "; break;
		case ERROR_DISK_CHANGE : str = 	"The program stopped because an alternate diskette was not inserted.  "; break;
		case ERROR_DRIVE_LOCKED : str = 	"The disk is in use or locked by another process.  "; break;
		case ERROR_BROKEN_PIPE : str = 	"The pipe has been ended.  "; break;
		case ERROR_OPEN_FAILED : str = 	"The system cannot open the device or file specified.  "; break;
		case ERROR_BUFFER_OVERFLOW : str = 	"The file name is too long.  "; break;
		case ERROR_DISK_FULL : str = 	"There is not enough space on the disk.  "; break;
		case ERROR_NO_MORE_SEARCH_HANDLES : str = 	"No more internal file identifiers available.  "; break;
		case ERROR_INVALID_TARGET_HANDLE : str = 	"The target internal file identifier is incorrect.  "; break;
		case ERROR_INVALID_CATEGORY : str = 	"The IOCTL call made by the application program is not correct.  "; break;
		case ERROR_INVALID_VERIFY_SWITCH : str = 	"The verify-on-write switch parameter value is not correct.  "; break;
		case ERROR_BAD_DRIVER_LEVEL : str = 	"The system does not support the command requested.  "; break;
		case ERROR_CALL_NOT_IMPLEMENTED : str = 	"This function is not supported on this system.  "; break;
		case ERROR_SEM_TIMEOUT : str = 	"The semaphore timeout period has expired.  "; break;
		case ERROR_INSUFFICIENT_BUFFER : str = 	"The data area passed to a system call is too small.  "; break;
		case ERROR_INVALID_NAME : str = 	"The filename, directory name, or volume label syntax is incorrect.  "; break;
		case ERROR_INVALID_LEVEL : str = 	"The system call level is not correct.  "; break;
		case ERROR_NO_VOLUME_LABEL : str = 	"The disk has no volume label.  "; break;
		case ERROR_MOD_NOT_FOUND : str = 	"The specified module could not be found.  "; break;
		case ERROR_PROC_NOT_FOUND : str = 	"The specified procedure could not be found.  "; break;
		case ERROR_WAIT_NO_CHILDREN : str = 	"There are no child processes to wait for.  "; break;
		case ERROR_CHILD_NOT_COMPLETE : str = 	"The %1 application cannot be run in Win32 mode.  "; break;
		case ERROR_DIRECT_ACCESS_HANDLE : str = 	"Attempt to use a file handle to an open disk partition for an operation other than raw disk I/O.  "; break;
		case ERROR_NEGATIVE_SEEK : str = 	"An attempt was made to move the file pointer before the beginning of the file.  "; break;
		case ERROR_SEEK_ON_DEVICE : str = 	"The file pointer cannot be set on the specified device or file.  "; break;
		case ERROR_IS_JOIN_TARGET : str = 	"A JOIN or SUBST command cannot be used for a drive that contains previously joined drives.  "; break;
		case ERROR_IS_JOINED : str = 	"An attempt was made to use a JOIN or SUBST command on a drive that has already been joined.  "; break;
		case ERROR_IS_SUBSTED : str = 	"An attempt was made to use a JOIN or SUBST command on a drive that has already been substituted.  "; break;
		case ERROR_NOT_JOINED : str = 	"The system tried to delete the JOIN of a drive that is not joined.  "; break;
		case ERROR_NOT_SUBSTED : str = 	"The system tried to delete the substitution of a drive that is not substituted.  "; break;
		case ERROR_JOIN_TO_JOIN : str = 	"The system tried to join a drive to a directory on a joined drive.  "; break;
		case ERROR_SUBST_TO_SUBST : str = 	"The system tried to substitute a drive to a directory on a substituted drive.  "; break;
		case ERROR_JOIN_TO_SUBST : str = 	"The system tried to join a drive to a directory on a substituted drive.  "; break;
		case ERROR_SUBST_TO_JOIN : str = 	"The system tried to SUBST a drive to a directory on a joined drive.  "; break;
		case ERROR_BUSY_DRIVE : str = 	"The system cannot perform a JOIN or SUBST at this time.  "; break;
		case ERROR_SAME_DRIVE : str = 	"The system cannot join or substitute a drive to or for a directory on the same drive.  "; break;
		case ERROR_DIR_NOT_ROOT : str = 	"The directory is not a subdirectory of the root directory.  "; break;
		case ERROR_DIR_NOT_EMPTY : str = 	"The directory is not empty.  "; break;
		case ERROR_IS_SUBST_PATH : str = 	"The path specified is being used in a substitute.  "; break;
		case ERROR_IS_JOIN_PATH : str = 	"Not enough resources are available to process this command.  "; break;
		case ERROR_PATH_BUSY : str = 	"The path specified cannot be used at this time.  "; break;
		case ERROR_IS_SUBST_TARGET : str = 	"An attempt was made to join or substitute a drive for which a directory on the drive is the target of a previous substitute.  "; break;
		case ERROR_SYSTEM_TRACE : str = 	"System trace information was not specified in your CONFIG.SYS file, or tracing is disallowed.  "; break;
		case ERROR_INVALID_EVENT_COUNT : str = 	"The number of specified semaphore events for DosMuxSemWait is not correct.  "; break;
		case ERROR_TOO_MANY_MUXWAITERS : str = 	"DosMuxSemWait did not execute; too many semaphores are already set.  "; break;
		case ERROR_INVALID_LIST_FORMAT : str = 	"The DosMuxSemWait list is not correct.  "; break;
		case ERROR_LABEL_TOO_LONG : str = 	"The volume label you entered exceeds the label character limit of the target file system.  "; break;
		case ERROR_TOO_MANY_TCBS : str = 	"Cannot create another thread.  "; break;
		case ERROR_SIGNAL_REFUSED : str = 	"The recipient process has refused the signal.  "; break;
		case ERROR_DISCARDED : str = 	"The segment is already discarded and cannot be locked.  "; break;
		case ERROR_NOT_LOCKED : str = 	"The segment is already unlocked.  "; break;
		case ERROR_BAD_THREADID_ADDR : str = 	"The address for the thread ID is not correct.  "; break;
		case ERROR_BAD_ARGUMENTS : str = 	"The argument string passed to DosExecPgm is not correct.  "; break;
		case ERROR_BAD_PATHNAME : str = 	"The specified path is invalid.  "; break;
		case ERROR_SIGNAL_PENDING : str = 	"A signal is already pending.  "; break;
		case ERROR_MAX_THRDS_REACHED : str = 	"No more threads can be created in the system.  "; break;
		case ERROR_LOCK_FAILED : str = 	"Unable to lock a region of a file.  "; break;
		case ERROR_BUSY : str = 	"The requested resource is in use.  "; break;
		case ERROR_CANCEL_VIOLATION : str = 	"A lock request was not outstanding for the supplied cancel region.  "; break;
		case ERROR_ATOMIC_LOCKS_NOT_SUPPORTED : str = 	"The file system does not support atomic changes to the lock type.  "; break;
		case ERROR_INVALID_SEGMENT_NUMBER : str = 	"The system detected a segment number that was not correct.  "; break;
		case ERROR_INVALID_ORDINAL : str = 	"The operating system cannot run %1.  "; break;
		case ERROR_ALREADY_EXISTS : str = 	"Cannot create a file when that file already exists.  "; break;
		case ERROR_INVALID_FLAG_NUMBER : str = 	"The flag passed is not correct.  "; break;
		case ERROR_SEM_NOT_FOUND : str = 	"The specified system semaphore name was not found.  "; break;
		case ERROR_INVALID_STARTING_CODESEG : str = 	"The operating system cannot run %1.  "; break;
		case ERROR_INVALID_STACKSEG : str = 	"The operating system cannot run %1.  "; break;
		case ERROR_INVALID_MODULETYPE : str = 	"The operating system cannot run %1.  "; break;
		case ERROR_INVALID_EXE_SIGNATURE : str = 	"Cannot run %1 in Win32 mode.  "; break;
		case ERROR_EXE_MARKED_INVALID : str = 	"The operating system cannot run %1.  "; break;
		case ERROR_BAD_EXE_FORMAT : str = 	"is not a valid Win32 application.  "; break;
		case ERROR_ITERATED_DATA_EXCEEDS_64k : str = 	"The operating system cannot run %1.  "; break;
		case ERROR_INVALID_MINALLOCSIZE : str = 	"The operating system cannot run %1.  "; break;
		case ERROR_DYNLINK_FROM_INVALID_RING : str = 	"The operating system cannot run this application program.  "; break;
		case ERROR_IOPL_NOT_ENABLED : str = 	"The operating system is not presently configured to run this application.  "; break;
		case ERROR_INVALID_SEGDPL : str = 	"The operating system cannot run %1.  "; break;
		case ERROR_AUTODATASEG_EXCEEDS_64k : str = 	"The operating system cannot run this application program.  "; break;
		case ERROR_RING2SEG_MUST_BE_MOVABLE : str = 	"The code segment cannot be greater than or equal to 64K.  "; break;
		case ERROR_RELOC_CHAIN_XEEDS_SEGLIM : str = 	"The operating system cannot run %1.  "; break;
		case ERROR_INFLOOP_IN_RELOC_CHAIN : str = 	"The operating system cannot run %1.  "; break;
		case ERROR_ENVVAR_NOT_FOUND : str = 	"The system could not find the environment option that was entered.  "; break;
		case ERROR_NO_SIGNAL_SENT : str = 	"No process in the command subtree has a signal handler.  "; break;
		case ERROR_FILENAME_EXCED_RANGE : str = 	"The filename or extension is too long.  "; break;
		case ERROR_RING2_STACK_IN_USE : str = 	"The ring 2 stack is in use.  "; break;
		case ERROR_META_EXPANSION_TOO_LONG : str = 	"The global filename characters, * or ?, are entered incorrectly or too many global filename characters are specified.  "; break;
		case ERROR_INVALID_SIGNAL_NUMBER : str = 	"The signal being posted is not correct.  "; break;
		case ERROR_THREAD_1_INACTIVE : str = 	"The signal handler cannot be set.  "; break;
		case ERROR_LOCKED : str = 	"The segment is locked and cannot be reallocated.  "; break;
		case ERROR_TOO_MANY_MODULES : str = 	"Too many dynamic-link modules are attached to this program or dynamic-link module.  "; break;
		case ERROR_NESTING_NOT_ALLOWED : str = 	"Can't nest calls to LoadModule.  "; break;
		case ERROR_EXE_MACHINE_TYPE_MISMATCH : str = 	"The image file %1 is valid, but is for a machine type other than the current machine.  "; break;
		case ERROR_BAD_PIPE : str = 	"The pipe state is invalid.  "; break;
		case ERROR_PIPE_BUSY : str = 	"All pipe instances are busy.  "; break;
		case ERROR_NO_DATA : str = 	"The pipe is being closed.  "; break;
		case ERROR_PIPE_NOT_CONNECTED : str = 	"No process is on the other end of the pipe.  "; break;
		case ERROR_MORE_DATA : str = 	"More data is available.  "; break;
		case ERROR_VC_DISCONNECTED : str = 	"The session was canceled.  "; break;
		case ERROR_INVALID_EA_NAME : str = 	"The specified extended attribute name was invalid.  "; break;
		case ERROR_EA_LIST_INCONSISTENT : str = 	"The extended attributes are inconsistent.  "; break;
		case ERROR_NO_MORE_ITEMS : str = 	"No more data is available.  "; break;
		case ERROR_CANNOT_COPY : str = 	"The copy functions cannot be used.  "; break;
		case ERROR_DIRECTORY : str = 	"The directory name is invalid.  "; break;
		case ERROR_EAS_DIDNT_FIT : str = 	"The extended attributes did not fit in the buffer.  "; break;
		case ERROR_EA_FILE_CORRUPT : str = 	"The extended attribute file on the mounted file system is corrupt.  "; break;
		case ERROR_EA_TABLE_FULL : str = 	"The extended attribute table file is full.  "; break;
		case ERROR_INVALID_EA_HANDLE : str = 	"The specified extended attribute handle is invalid.  "; break;
		case ERROR_EAS_NOT_SUPPORTED : str = 	"The mounted file system does not support extended attributes.  "; break;
		case ERROR_NOT_OWNER : str = 	"Attempt to release mutex not owned by caller.  "; break;
		case ERROR_TOO_MANY_POSTS : str = 	"Too many posts were made to a semaphore.  "; break;
		case ERROR_PARTIAL_COPY : str = 	"Only part of a ReadProcessMemoty or WriteProcessMemory request was completed.  "; break;
		case ERROR_OPLOCK_NOT_GRANTED : str = 	"The oplock request is denied.  "; break;
		case ERROR_INVALID_OPLOCK_PROTOCOL : str = 	"An invalid oplock acknowledgment was received by the system.  "; break;
		case ERROR_MR_MID_NOT_FOUND : str = 	"The system cannot find message text for message number 0x%1 in the message file for %2.  "; break;
		case ERROR_INVALID_ADDRESS : str = 	"Attempt to access invalid address.  "; break;
		case ERROR_ARITHMETIC_OVERFLOW : str = 	"Arithmetic result exceeded 32 bits.  "; break;
		case ERROR_PIPE_CONNECTED : str = 	"There is a process on other end of the pipe.  "; break;
		case ERROR_PIPE_LISTENING : str = 	"Waiting for a process to open the other end of the pipe.  "; break;
		case ERROR_EA_ACCESS_DENIED : str = 	"Access to the extended attribute was denied.  "; break;
		case ERROR_OPERATION_ABORTED : str = 	"The I/O operation has been aborted because of either a thread exit or an application request.  "; break;
		case ERROR_IO_INCOMPLETE : str = 	"Overlapped I/O event is not in a signaled state.  "; break;
		case ERROR_IO_PENDING : str = 	"Overlapped I/O operation is in progress.  "; break;
		case ERROR_NOACCESS : str = 	"Invalid access to memory location.  "; break;
		case ERROR_SWAPERROR : str = 	"Error performing inpage operation.  "; break;
		case ERROR_STACK_OVERFLOW : str = 	"Recursion too deep; the stack overflowed.  "; break;
		case ERROR_INVALID_MESSAGE : str = 	"The window cannot act on the sent message.  "; break;
		case ERROR_CAN_NOT_COMPLETE : str = 	"Cannot complete this function.  "; break;
		case ERROR_INVALID_FLAGS : str = 	"Invalid flags.  "; break;
		case ERROR_UNRECOGNIZED_VOLUME : str = 	"The volume does not contain a recognized file system. Please make sure that all required file system drivers are loaded and that the volume is not corrupted.  "; break;
		case ERROR_FILE_INVALID : str = 	"The volume for a file has been externally altered so that the opened file is no longer valid.  "; break;
		case ERROR_FULLSCREEN_MODE : str = 	"The requested operation cannot be performed in full-screen mode.  "; break;
		case ERROR_NO_TOKEN : str = 	"An attempt was made to reference a token that does not exist.  "; break;
		case ERROR_BADDB : str = 	"The configuration registry database is corrupt.  "; break;
		case ERROR_BADKEY : str = 	"The configuration registry key is invalid.  "; break;
		case ERROR_CANTOPEN : str = 	"The configuration registry key could not be opened.  "; break;
		case ERROR_CANTREAD : str = 	"The configuration registry key could not be read.  "; break;
		case ERROR_CANTWRITE : str = 	"The configuration registry key could not be written.  "; break;
		case ERROR_REGISTRY_RECOVERED : str = 	"One of the files in the registry database had to be recovered by use of a log or alternate copy. The recovery was successful.  "; break;
		case ERROR_REGISTRY_CORRUPT : str = 	"The registry is corrupted. The structure of one of the files that contains registry data is corrupted, or the system's image of the file in memory is corrupted, or the file could not be recovered because the alternate copy or log was absent or corrupted.  "; break;
		case ERROR_REGISTRY_IO_FAILED : str = 	"An I/O operation initiated by the registry failed unrecoverably. The registry could not read in, or write out, or flush, one of the files that contain the system's image of the registry.  "; break;
		case ERROR_NOT_REGISTRY_FILE : str = 	"The system has attempted to load or restore a file into the registry, but the specified file is not in a registry file format.  "; break;
		case ERROR_KEY_DELETED : str = 	"Illegal operation attempted on a registry key that has been marked for deletion.  "; break;
		case ERROR_NO_LOG_SPACE : str = 	"System could not allocate the required space in a registry log.  "; break;
		case ERROR_KEY_HAS_CHILDREN : str = 	"Cannot create a symbolic link in a registry key that already has subkeys or values.  "; break;
		case ERROR_CHILD_MUST_BE_VOLATILE : str = 	"Cannot create a stable subkey under a volatile parent key.  "; break;
		case ERROR_NOTIFY_ENUM_DIR : str = 	"A notify change request is being completed and the information is not being returned in the caller's buffer. The caller now needs to enumerate the files to find the changes.  "; break;
		case ERROR_DEPENDENT_SERVICES_RUNNING : str = 	"A stop control has been sent to a service that other running services are dependent on.  "; break;
		case ERROR_INVALID_SERVICE_CONTROL : str = 	"The requested control is not valid for this service.  "; break;
		case ERROR_SERVICE_REQUEST_TIMEOUT : str = 	"The service did not respond to the start or control request in a timely fashion.  "; break;
		case ERROR_SERVICE_NO_THREAD : str = 	"A thread could not be created for the service.  "; break;
		case ERROR_SERVICE_DATABASE_LOCKED : str = 	"The service database is locked.  "; break;
		case ERROR_SERVICE_ALREADY_RUNNING : str = 	"An instance of the service is already running.  "; break;
		case ERROR_INVALID_SERVICE_ACCOUNT : str = 	"The account name is invalid or does not exist.  "; break;
		case ERROR_SERVICE_DISABLED : str = 	"The service cannot be started, either because it is disabled or because it has no enabled devices associated with it.  "; break;
		case ERROR_CIRCULAR_DEPENDENCY : str = 	"Circular service dependency was specified.  "; break;
		case ERROR_SERVICE_DOES_NOT_EXIST : str = 	"The specified service does not exist as an installed service.  "; break;
		case ERROR_SERVICE_CANNOT_ACCEPT_CTRL : str = 	"The service cannot accept control messages at this time.  "; break;
		case ERROR_SERVICE_NOT_ACTIVE : str = 	"The service has not been started.  "; break;
		case ERROR_FAILED_SERVICE_CONTROLLER_CONNECT : str = 	"The service process could not connect to the service controller.  "; break;
		case ERROR_EXCEPTION_IN_SERVICE : str = 	"An exception occurred in the service when handling the control request.  "; break;
		case ERROR_DATABASE_DOES_NOT_EXIST : str = 	"The database specified does not exist.  "; break;
		case ERROR_SERVICE_SPECIFIC_ERROR : str = 	"The service has returned a service-specific error code.  "; break;
		case ERROR_PROCESS_ABORTED : str = 	"The process terminated unexpectedly.  "; break;
		case ERROR_SERVICE_DEPENDENCY_FAIL : str = 	"The dependency service or group failed to start.  "; break;
		case ERROR_SERVICE_LOGON_FAILED : str = 	"The service did not start due to a logon failure.  "; break;
		case ERROR_SERVICE_START_HANG : str = 	"After starting, the service hung in a start-pending state.  "; break;
		case ERROR_INVALID_SERVICE_LOCK : str = 	"The specified service database lock is invalid.  "; break;
		case ERROR_SERVICE_MARKED_FOR_DELETE : str = 	"The specified service has been marked for deletion.  "; break;
		case ERROR_SERVICE_EXISTS : str = 	"The specified service already exists.  "; break;
		case ERROR_ALREADY_RUNNING_LKG : str = 	"The system is currently running with the last-known-good configuration.  "; break;
		case ERROR_SERVICE_DEPENDENCY_DELETED : str = 	"The dependency service does not exist or has been marked for deletion.  "; break;
		case ERROR_BOOT_ALREADY_ACCEPTED : str = 	"The current boot has already been accepted for use as the last-known-good control set.  "; break;
		case ERROR_SERVICE_NEVER_STARTED : str = 	"No attempts to start the service have been made since the last boot.  "; break;
		case ERROR_DUPLICATE_SERVICE_NAME : str = 	"The name is already in use as either a service name or a service display name.  "; break;
		case ERROR_DIFFERENT_SERVICE_ACCOUNT : str = 	"The account specified for this service is different from the account specified for other services running in the same process.  "; break;
		case ERROR_CANNOT_DETECT_DRIVER_FAILURE : str = 	"Failure actions can only be set for Win32 services, not for drivers.  "; break;
		case ERROR_CANNOT_DETECT_PROCESS_ABORT : str = 	"This service runs in the same process as the service control manager. Therefore, the service control manager cannot take action if this service's process terminates unexpectedly.  "; break;
		case ERROR_NO_RECOVERY_PROGRAM : str = 	"No recovery program has been configured for this service.  "; break;
		case ERROR_END_OF_MEDIA : str = 	"The physical end of the tape has been reached.  "; break;
		case ERROR_FILEMARK_DETECTED : str = 	"A tape access reached a filemark.  "; break;
		case ERROR_BEGINNING_OF_MEDIA : str = 	"The beginning of the tape or a partition was encountered.  "; break;
		case ERROR_SETMARK_DETECTED : str = 	"A tape access reached the end of a set of files.  "; break;
		case ERROR_NO_DATA_DETECTED : str = 	"No more data is on the tape.  "; break;
		case ERROR_PARTITION_FAILURE : str = 	"Tape could not be partitioned.  "; break;
		case ERROR_INVALID_BLOCK_LENGTH : str = 	"When accessing a new tape of a multivolume partition, the current blocksize is incorrect.  "; break;
		case ERROR_DEVICE_NOT_PARTITIONED : str = 	"Tape partition information could not be found when loading a tape.  "; break;
		case ERROR_UNABLE_TO_LOCK_MEDIA : str = 	"Unable to lock the media eject mechanism.  "; break;
		case ERROR_UNABLE_TO_UNLOAD_MEDIA : str = 	"Unable to unload the media.  "; break;
		case ERROR_MEDIA_CHANGED : str = 	"The media in the drive may have changed.  "; break;
		case ERROR_BUS_RESET : str = 	"The I/O bus was reset.  "; break;
		case ERROR_NO_MEDIA_IN_DRIVE : str = 	"No media in drive.  "; break;
		case ERROR_NO_UNICODE_TRANSLATION : str = 	"No mapping for the Unicode character exists in the target multi-byte code page.  "; break;
		case ERROR_DLL_INIT_FAILED : str = 	"A dynamic link library (DLL) initialization routine failed.  "; break;
		case ERROR_SHUTDOWN_IN_PROGRESS : str = 	"A system shutdown is in progress.  "; break;
		case ERROR_NO_SHUTDOWN_IN_PROGRESS : str = 	"Unable to abort the system shutdown because no shutdown was in progress.  "; break;
		case ERROR_IO_DEVICE : str = 	"The request could not be performed because of an I/O device error.  "; break;
		case ERROR_SERIAL_NO_DEVICE : str = 	"No serial device was successfully initialized. The serial driver will unload.  "; break;
		case ERROR_IRQ_BUSY : str = 	"Unable to open a device that was sharing an interrupt request (IRQ) with other devices. At least one other device that uses that IRQ was already opened.  "; break;
		case ERROR_MORE_WRITES : str = 	"A serial I/O operation was completed by another write to the serial port. The IOCTL_SERIAL_XOFF_COUNTER reached zero.)  "; break;
		case ERROR_COUNTER_TIMEOUT : str = 	"A serial I/O operation completed because the timeout period expired. The IOCTL_SERIAL_XOFF_COUNTER did not reach zero.)  "; break;
		case ERROR_FLOPPY_ID_MARK_NOT_FOUND : str = 	"No ID address mark was found on the floppy disk.  "; break;
		case ERROR_FLOPPY_WRONG_CYLINDER : str = 	"Mismatch between the floppy disk sector ID field and the floppy disk controller track address.  "; break;
		case ERROR_FLOPPY_UNKNOWN_ERROR : str = 	"The floppy disk controller reported an error that is not recognized by the floppy disk driver.  "; break;
		case ERROR_FLOPPY_BAD_REGISTERS : str = 	"The floppy disk controller returned inconsistent results in its registers.  "; break;
		case ERROR_DISK_RECALIBRATE_FAILED : str = 	"While accessing the hard disk, a recalibrate operation failed, even after retries.  "; break;
		case ERROR_DISK_OPERATION_FAILED : str = 	"While accessing the hard disk, a disk operation failed even after retries.  "; break;
		case ERROR_DISK_RESET_FAILED : str = 	"While accessing the hard disk, a disk controller reset was needed, but even that failed.  "; break;
		case ERROR_EOM_OVERFLOW : str = 	"Physical end of tape encountered.  "; break;
		case ERROR_NOT_ENOUGH_SERVER_MEMORY : str = 	"Not enough server storage is available to process this command.  "; break;
		case ERROR_POSSIBLE_DEADLOCK : str = 	"A potential deadlock condition has been detected.  "; break;
		case ERROR_MAPPED_ALIGNMENT : str = 	"The base address or the file offset specified does not have the proper alignment.  "; break;
		case ERROR_SET_POWER_STATE_VETOED : str = 	"An attempt to change the system power state was vetoed by another application or driver.  "; break;
		case ERROR_SET_POWER_STATE_FAILED : str = 	"The system BIOS failed an attempt to change the system power state.  "; break;
		case ERROR_TOO_MANY_LINKS : str = 	"An attempt was made to create more links on a file than the file system supports.  "; break;
		case ERROR_OLD_WIN_VERSION : str = 	"The specified program requires a newer version of Windows.  "; break;
		case ERROR_APP_WRONG_OS : str = 	"The specified program is not a Windows or MS-DOS program.  "; break;
		case ERROR_SINGLE_INSTANCE_APP : str = 	"Cannot start more than one instance of the specified program.  "; break;
		case ERROR_RMODE_APP : str = 	"The specified program was written for an earlier version of Windows.  "; break;
		case ERROR_INVALID_DLL : str = 	"One of the library files needed to run this application is damaged.  "; break;
		case ERROR_NO_ASSOCIATION : str = 	"No application is associated with the specified file for this operation.  "; break;
		case ERROR_DDE_FAIL : str = 	"An error occurred in sending the command to the application.  "; break;
		case ERROR_DLL_NOT_FOUND : str = 	"One of the library files needed to run this application cannot be found.  "; break;
		case ERROR_NO_MORE_USER_HANDLES : str = 	"The current process has used all of its system allowance of handles for Window Manager objects.  "; break;
		case ERROR_MESSAGE_SYNC_ONLY : str = 	"The message can be used only with synchronous operations.  "; break;
		case ERROR_SOURCE_ELEMENT_EMPTY : str = 	"The indicated source element has no media.  "; break;
		case ERROR_DESTINATION_ELEMENT_FULL : str = 	"The indicated destination element already contains media.  "; break;
		case ERROR_ILLEGAL_ELEMENT_ADDRESS : str = 	"The indicated element does not exist.  "; break;
		case ERROR_MAGAZINE_NOT_PRESENT : str = 	"The indicated element is part of a magazine that is not present.  "; break;
		case ERROR_DEVICE_REINITIALIZATION_NEEDED : str = 	"The indicated device requires reinitialization due to hardware errors.  "; break;
		case ERROR_DEVICE_REQUIRES_CLEANING : str = 	"The device has indicated that cleaning is required before further operations are attempted.  "; break;
		case ERROR_DEVICE_DOOR_OPEN : str = 	"The device has indicated that its door is open.  "; break;
		case ERROR_DEVICE_NOT_CONNECTED : str = 	"The device is not connected.  "; break;
		case ERROR_NOT_FOUND : str = 	"Element not found.  "; break;
		case ERROR_NO_MATCH : str = 	"There was no match for the specified key in the index.  "; break;
		case ERROR_SET_NOT_FOUND : str = 	"The property set specified does not exist on the object.  "; break;
		case ERROR_POINT_NOT_FOUND : str = 	"The point passed to GetMouseMovePoints is not in the buffer.  "; break;
		case ERROR_NO_TRACKING_SERVICE : str = 	"The tracking (workstation) service is not running.  "; break;
		case ERROR_NO_VOLUME_ID : str = 	"The Volume ID could not be found.  "; break;
		case ERROR_BAD_DEVICE : str = 	"The specified device name is invalid.  "; break;
		case ERROR_CONNECTION_UNAVAIL : str = 	"The device is not currently connected but it is a remembered connection.  "; break;
		case ERROR_DEVICE_ALREADY_REMEMBERED : str = 	"An attempt was made to remember a device that had previously been remembered.  "; break;
		case ERROR_NO_NET_OR_BAD_PATH : str = 	"No network provider accepted the given network path.  "; break;
		case ERROR_BAD_PROVIDER : str = 	"The specified network provider name is invalid.  "; break;
		case ERROR_CANNOT_OPEN_PROFILE : str = 	"Unable to open the network connection profile.  "; break;
		case ERROR_BAD_PROFILE : str = 	"The network connection profile is corrupted.  "; break;
		case ERROR_NOT_CONTAINER : str = 	"Cannot enumerate a noncontainer.  "; break;
		case ERROR_EXTENDED_ERROR : str = 	"An extended error has occurred.  "; break;
		case ERROR_INVALID_GROUPNAME : str = 	"The format of the specified group name is invalid.  "; break;
		case ERROR_INVALID_COMPUTERNAME : str = 	"The format of the specified computer name is invalid.  "; break;
		case ERROR_INVALID_EVENTNAME : str = 	"The format of the specified event name is invalid.  "; break;
		case ERROR_INVALID_DOMAINNAME : str = 	"The format of the specified domain name is invalid.  "; break;
		case ERROR_INVALID_SERVICENAME : str = 	"The format of the specified service name is invalid.  "; break;
		case ERROR_INVALID_NETNAME : str = 	"The format of the specified network name is invalid.  "; break;
		case ERROR_INVALID_SHARENAME : str = 	"The format of the specified share name is invalid.  "; break;
		case ERROR_INVALID_PASSWORDNAME : str = 	"The format of the specified password is invalid.  "; break;
		case ERROR_INVALID_MESSAGENAME : str = 	"The format of the specified message name is invalid.  "; break;
		case ERROR_INVALID_MESSAGEDEST : str = 	"The format of the specified message destination is invalid.  "; break;
		case ERROR_SESSION_CREDENTIAL_CONFLICT : str = 	"The credentials supplied conflict with an existing set of credentials.  "; break;
		case ERROR_REMOTE_SESSION_LIMIT_EXCEEDED : str = 	"An attempt was made to establish a session to a network server, but there are already too many sessions established to that server.  "; break;
		case ERROR_DUP_DOMAINNAME : str = 	"The workgroup or domain name is already in use by another computer on the network.  "; break;
		case ERROR_NO_NETWORK : str = 	"The network is not present or not started.  "; break;
		case ERROR_CANCELLED : str = 	"The operation was canceled by the user.  "; break;
		case ERROR_USER_MAPPED_FILE : str = 	"The requested operation cannot be performed on a file with a user-mapped section open.  "; break;
		case ERROR_CONNECTION_REFUSED : str = 	"The remote system refused the network connection.  "; break;
		case ERROR_GRACEFUL_DISCONNECT : str = 	"The network connection was gracefully closed.  "; break;
		case ERROR_ADDRESS_ALREADY_ASSOCIATED : str = 	"The network transport endpoint already has an address associated with it.  "; break;
		case ERROR_ADDRESS_NOT_ASSOCIATED : str = 	"An address has not yet been associated with the network endpoint.  "; break;
		case ERROR_CONNECTION_INVALID : str = 	"An operation was attempted on a nonexistent network connection.  "; break;
		case ERROR_CONNECTION_ACTIVE : str = 	"An invalid operation was attempted on an active network connection.  "; break;
		case ERROR_NETWORK_UNREACHABLE : str = 	"The remote network is not reachable by the transport.  "; break;
		case ERROR_HOST_UNREACHABLE : str = 	"The remote system is not reachable by the transport.  "; break;
		case ERROR_PROTOCOL_UNREACHABLE : str = 	"The remote system does not support the transport protocol.  "; break;
		case ERROR_PORT_UNREACHABLE : str = 	"No service is operating at the destination network endpoint on the remote system.  "; break;
		case ERROR_REQUEST_ABORTED : str = 	"The request was aborted.  "; break;
		case ERROR_CONNECTION_ABORTED : str = 	"The network connection was aborted by the local system.  "; break;
		case ERROR_RETRY : str = 	"The operation could not be completed. A retry should be performed.  "; break;
		case ERROR_CONNECTION_COUNT_LIMIT : str = 	"A connection to the server could not be made because the limit on the number of concurrent connections for this account has been reached.  "; break;
		case ERROR_LOGIN_TIME_RESTRICTION : str = 	"Attempting to log in during an unauthorized time of day for this account.  "; break;
		case ERROR_LOGIN_WKSTA_RESTRICTION : str = 	"The account is not authorized to log in from this station.  "; break;
		case ERROR_INCORRECT_ADDRESS : str = 	"The network address could not be used for the operation requested.  "; break;
		case ERROR_ALREADY_REGISTERED : str = 	"The service is already registered.  "; break;
		case ERROR_SERVICE_NOT_FOUND : str = 	"The specified service does not exist.  "; break;
		case ERROR_NOT_AUTHENTICATED : str = 	"The operation being requested was not performed because the user has not been authenticated.  "; break;
		case ERROR_NOT_LOGGED_ON : str = 	"The operation being requested was not performed because the user has not logged on to the network. The specified service does not exist.  "; break;
		case ERROR_CONTINUE : str = 	"Continue with work in progress.  "; break;
		case ERROR_ALREADY_INITIALIZED : str = 	"An attempt was made to perform an initialization operation when initialization has already been completed.  "; break;
		case ERROR_NO_MORE_DEVICES : str = 	"No more local devices.  "; break;
		case ERROR_NO_SUCH_SITE : str = 	"The specified site does not exist.  "; break;
		case ERROR_DOMAIN_CONTROLLER_EXISTS : str = 	"A domain controller with the specified name already exists.  "; break;
		case ERROR_DS_NOT_INSTALLED : str = 	"An error occurred while installing the Windows NT directory service. Please view the event log for more information.  "; break;
		case ERROR_NOT_ALL_ASSIGNED : str = 	"Not all privileges referenced are assigned to the caller.  "; break;
		case ERROR_SOME_NOT_MAPPED : str = 	"Some mapping between account names and security IDs was not done.  "; break;
		case ERROR_NO_QUOTAS_FOR_ACCOUNT : str = 	"No system quota limits are specifically set for this account.  "; break;
		case ERROR_LOCAL_USER_SESSION_KEY : str = 	"No encryption key is available. A well-known encryption key was returned.  "; break;
		case ERROR_NULL_LM_PASSWORD : str = 	"The Windows NT password is too complex to be converted to a LAN Manager password. The LAN Manager password returned is a NULL string.  "; break;
		case ERROR_UNKNOWN_REVISION : str = 	"The revision level is unknown.  "; break;
		case ERROR_REVISION_MISMATCH : str = 	"Indicates two revision levels are incompatible.  "; break;
		case ERROR_INVALID_OWNER : str = 	"This security ID may not be assigned as the owner of this object.  "; break;
		case ERROR_INVALID_PRIMARY_GROUP : str = 	"This security ID may not be assigned as the primary group of an object.  "; break;
		case ERROR_NO_IMPERSONATION_TOKEN : str = 	"An attempt has been made to operate on an impersonation token by a thread that is not currently impersonating a client.  "; break;
		case ERROR_CANT_DISABLE_MANDATORY : str = 	"The group may not be disabled.  "; break;
		case ERROR_NO_LOGON_SERVERS : str = 	"There are currently no logon servers available to service the logon request.  "; break;
		case ERROR_NO_SUCH_LOGON_SESSION : str = 	"A specified logon session does not exist. It may already have been terminated.  "; break;
		case ERROR_NO_SUCH_PRIVILEGE : str = 	"A specified privilege does not exist.  "; break;
		case ERROR_PRIVILEGE_NOT_HELD : str = 	"A required privilege is not held by the client.  "; break;
		case ERROR_INVALID_ACCOUNT_NAME : str = 	"The name provided is not a properly formed account name.  "; break;
		case ERROR_USER_EXISTS : str = 	"The specified user already exists.  "; break;
		case ERROR_NO_SUCH_USER : str = 	"The specified user does not exist.  "; break;
		case ERROR_GROUP_EXISTS : str = 	"The specified group already exists.  "; break;
		case ERROR_NO_SUCH_GROUP : str = 	"The specified group does not exist.  "; break;
		case ERROR_MEMBER_IN_GROUP : str = 	"Either the specified user account is already a member of the specified group, or the specified group cannot be deleted because it contains a member.  "; break;
		case ERROR_MEMBER_NOT_IN_GROUP : str = 	"The specified user account is not a member of the specified group account.  "; break;
		case ERROR_LAST_ADMIN : str = 	"The last remaining administration account cannot be disabled or deleted.  "; break;
		case ERROR_WRONG_PASSWORD : str = 	"Unable to update the password. The value provided as the current password is incorrect.  "; break;
		case ERROR_ILL_FORMED_PASSWORD : str = 	"Unable to update the password. The value provided for the new password contains values that are not allowed in passwords.  "; break;
		case ERROR_PASSWORD_RESTRICTION : str = 	"Unable to update the password because a password update rule has been violated.  "; break;
		case ERROR_LOGON_FAILURE : str = 	"Logon failure: str =  unknown user name or bad password.  "; break;
		case ERROR_ACCOUNT_RESTRICTION : str = 	"Logon failure: str =  user account restriction.  "; break;
		case ERROR_INVALID_LOGON_HOURS : str = 	"Logon failure: str =  account logon time restriction violation.  "; break;
		case ERROR_INVALID_WORKSTATION : str = 	"Logon failure: str =  user not allowed to log on to this computer.  "; break;
		case ERROR_PASSWORD_EXPIRED : str = 	"Logon failure: str =  the specified account password has expired.  "; break;
		case ERROR_ACCOUNT_DISABLED : str = 	"Logon failure: str =  account currently disabled.  "; break;
		case ERROR_NONE_MAPPED : str = 	"No mapping between account names and security IDs was done.  "; break;
		case ERROR_TOO_MANY_LUIDS_REQUESTED : str = 	"Too many local user identifiers (LUIDs) were requested at one time.  "; break;
		case ERROR_LUIDS_EXHAUSTED : str = 	"No more local user identifiers (LUIDs) are available.  "; break;
		case ERROR_INVALID_SUB_AUTHORITY : str = 	"The subauthority part of a security ID is invalid for this particular use.  "; break;
		case ERROR_INVALID_ACL : str = 	"The access control list (ACL) structure is invalid.  "; break;
		case ERROR_INVALID_SID : str = 	"The security ID structure is invalid.  "; break;
		case ERROR_INVALID_SECURITY_DESCR : str = 	"The security descriptor structure is invalid.  "; break;
		case ERROR_BAD_INHERITANCE_ACL : str = 	"The inherited access control list (ACL) or access control entry (ACE) could not be built.  "; break;
		case ERROR_SERVER_DISABLED : str = 	"The server is currently disabled.  "; break;
		case ERROR_SERVER_NOT_DISABLED : str = 	"The server is currently enabled.  "; break;
		case ERROR_INVALID_ID_AUTHORITY : str = 	"The value provided was an invalid value for an identifier authority.  "; break;
		case ERROR_ALLOTTED_SPACE_EXCEEDED : str = 	"No more memory is available for security information updates.  "; break;
		case ERROR_INVALID_GROUP_ATTRIBUTES : str = 	"The specified attributes are invalid, or incompatible with the attributes for the group as a whole.  "; break;
		case ERROR_BAD_IMPERSONATION_LEVEL : str = 	"Either a required impersonation level was not provided, or the provided impersonation level is invalid.  "; break;
		case ERROR_CANT_OPEN_ANONYMOUS : str = 	"Cannot open an anonymous level security token.  "; break;
		case ERROR_BAD_VALIDATION_CLASS : str = 	"The validation information class requested was invalid.  "; break;
		case ERROR_BAD_TOKEN_TYPE : str = 	"The type of the token is inappropriate for its attempted use.  "; break;
		case ERROR_NO_SECURITY_ON_OBJECT : str = 	"Unable to perform a security operation on an object that has no associated security.  "; break;
		case ERROR_CANT_ACCESS_DOMAIN_INFO : str = 	"Indicates a Windows NT Server could not be contacted or that objects within the domain are protected such that necessary information could not be retrieved.  "; break;
		case ERROR_INVALID_SERVER_STATE : str = 	"The security account manager (SAM) or local security authority (LSA) server was in the wrong state to perform the security operation.  "; break;
		case ERROR_INVALID_DOMAIN_STATE : str = 	"The domain was in the wrong state to perform the security operation.  "; break;
		case ERROR_INVALID_DOMAIN_ROLE : str = 	"This operation is only allowed for the Primary Domain Controller of the domain.  "; break;
		case ERROR_NO_SUCH_DOMAIN : str = 	"The specified domain did not exist.  "; break;
		case ERROR_DOMAIN_EXISTS : str = 	"The specified domain already exists.  "; break;
		case ERROR_DOMAIN_LIMIT_EXCEEDED : str = 	"An attempt was made to exceed the limit on the number of domains per server.  "; break;
		case ERROR_INTERNAL_DB_CORRUPTION : str = 	"Unable to complete the requested operation because of either a catastrophic media failure or a data structure corruption on the disk.  "; break;
		case ERROR_INTERNAL_ERROR : str = 	"The security account database contains an internal inconsistency.  "; break;
		case ERROR_GENERIC_NOT_MAPPED : str = 	"Generic access types were contained in an access mask which should already be mapped to nongeneric types.  "; break;
		case ERROR_BAD_DESCRIPTOR_FORMAT : str = 	"A security descriptor is not in the right format (absolute or self-relative).  "; break;
		case ERROR_NOT_LOGON_PROCESS : str = 	"The requested action is restricted for use by logon processes only. The calling process has not registered as a logon process.  "; break;
		case ERROR_LOGON_SESSION_EXISTS : str = 	"Cannot start a new logon session with an ID that is already in use.  "; break;
		case ERROR_NO_SUCH_PACKAGE : str = 	"A specified authentication package is unknown.  "; break;
		case ERROR_BAD_LOGON_SESSION_STATE : str = 	"The logon session is not in a state that is consistent with the requested operation.  "; break;
		case ERROR_LOGON_SESSION_COLLISION : str = 	"The logon session ID is already in use.  "; break;
		case ERROR_INVALID_LOGON_TYPE : str = 	"A logon request contained an invalid logon type value.  "; break;
		case ERROR_CANNOT_IMPERSONATE : str = 	"Unable to impersonate using a named pipe until data has been read from that pipe.  "; break;
		case ERROR_RXACT_INVALID_STATE : str = 	"The transaction state of a registry subtree is incompatible with the requested operation.  "; break;
		case ERROR_RXACT_COMMIT_FAILURE : str = 	"An internal security database corruption has been encountered.  "; break;
		case ERROR_SPECIAL_ACCOUNT : str = 	"Cannot perform this operation on built-in accounts.  "; break;
		case ERROR_SPECIAL_GROUP : str = 	"Cannot perform this operation on this built-in special group.  "; break;
		case ERROR_SPECIAL_USER : str = 	"Cannot perform this operation on this built-in special user.  "; break;
		case ERROR_MEMBERS_PRIMARY_GROUP : str = 	"The user cannot be removed from a group because the group is currently the user's primary group.  "; break;
		case ERROR_TOKEN_ALREADY_IN_USE : str = 	"The token is already in use as a primary token.  "; break;
		case ERROR_NO_SUCH_ALIAS : str = 	"The specified local group does not exist.  "; break;
		case ERROR_MEMBER_NOT_IN_ALIAS : str = 	"The specified account name is not a member of the local group.  "; break;
		case ERROR_MEMBER_IN_ALIAS : str = 	"The specified account name is already a member of the local group.  "; break;
		case ERROR_ALIAS_EXISTS : str = 	"The specified local group already exists.  "; break;
		case ERROR_LOGON_NOT_GRANTED : str = 	"Logon failure: str =  the user has not been granted the requested logon type at this computer.  "; break;
		case ERROR_TOO_MANY_SECRETS : str = 	"The maximum number of secrets that may be stored in a single system has been exceeded.  "; break;
		case ERROR_SECRET_TOO_LONG : str = 	"The length of a secret exceeds the maximum length allowed.  "; break;
		case ERROR_INTERNAL_DB_ERROR : str = 	"The local security authority database contains an internal inconsistency.  "; break;
		case ERROR_TOO_MANY_CONTEXT_IDS : str = 	"During a logon attempt, the user's security context accumulated too many security IDs.  "; break;
		case ERROR_LOGON_TYPE_NOT_GRANTED : str = 	"Logon failure: str =  the user has not been granted the requested logon type at this computer.  "; break;
		case ERROR_NT_CROSS_ENCRYPTION_REQUIRED : str = 	"A cross-encrypted password is necessary to change a user password.  "; break;
		case ERROR_NO_SUCH_MEMBER : str = 	"A new member could not be added to a local group because the member does not exist.  "; break;
		case ERROR_INVALID_MEMBER : str = 	"A new member could not be added to a local group because the member has the wrong account type.  "; break;
		case ERROR_TOO_MANY_SIDS : str = 	"Too many security IDs have been specified.  "; break;
		case ERROR_LM_CROSS_ENCRYPTION_REQUIRED : str = 	"A cross-encrypted password is necessary to change this user password.  "; break;
		case ERROR_NO_INHERITANCE : str = 	"Indicates an ACL contains no inheritable components.  "; break;
		case ERROR_FILE_CORRUPT : str = 	"The file or directory is corrupted and unreadable.  "; break;
		case ERROR_DISK_CORRUPT : str = 	"The disk structure is corrupted and unreadable.  "; break;
		case ERROR_NO_USER_SESSION_KEY : str = 	"There is no user session key for the specified logon session.  "; break;
		case ERROR_LICENSE_QUOTA_EXCEEDED : str = 	"The service being accessed is licensed for a particular number of connections. No more connections can be made to the service at this time because there are already as many connections as the service can accept.  "; break;
		case ERROR_INVALID_WINDOW_HANDLE : str = 	"Invalid window handle.  "; break;
		case ERROR_INVALID_MENU_HANDLE : str = 	"Invalid menu handle.  "; break;
		case ERROR_INVALID_CURSOR_HANDLE : str = 	"Invalid cursor handle.  "; break;
		case ERROR_INVALID_ACCEL_HANDLE : str = 	"Invalid accelerator table handle.  "; break;
		case ERROR_INVALID_HOOK_HANDLE : str = 	"Invalid hook handle.  "; break;
		case ERROR_INVALID_DWP_HANDLE : str = 	"Invalid handle to a multiple-window position structure.  "; break;
		case ERROR_TLW_WITH_WSCHILD : str = 	"Cannot create a top-level child window.  "; break;
		case ERROR_CANNOT_FIND_WND_CLASS : str = 	"Cannot find window class.  "; break;
		case ERROR_WINDOW_OF_OTHER_THREAD : str = 	"Invalid window; it belongs to other thread.  "; break;
		case ERROR_HOTKEY_ALREADY_REGISTERED : str = 	"Hot key is already registered.  "; break;
		case ERROR_CLASS_ALREADY_EXISTS : str = 	"Class already exists.  "; break;
		case ERROR_CLASS_DOES_NOT_EXIST : str = 	"Class does not exist.  "; break;
		case ERROR_CLASS_HAS_WINDOWS : str = 	"Class still has open windows.  "; break;
		case ERROR_INVALID_INDEX : str = 	"Invalid index.  "; break;
		case ERROR_INVALID_ICON_HANDLE : str = 	"Invalid icon handle.  "; break;
		case ERROR_PRIVATE_DIALOG_INDEX : str = 	"Using private DIALOG window words.  "; break;
		case ERROR_LISTBOX_ID_NOT_FOUND : str = 	"The list box identifier was not found.  "; break;
		case ERROR_NO_WILDCARD_CHARACTERS : str = 	"No wildcards were found.  "; break;
		case ERROR_CLIPBOARD_NOT_OPEN : str = 	"Thread does not have a clipboard open.  "; break;
		case ERROR_HOTKEY_NOT_REGISTERED : str = 	"Hot key is not registered.  "; break;
		case ERROR_WINDOW_NOT_DIALOG : str = 	"The window is not a valid dialog window.  "; break;
		case ERROR_CONTROL_ID_NOT_FOUND : str = 	"Control ID not found.  "; break;
		case ERROR_INVALID_COMBOBOX_MESSAGE : str = 	"Invalid message for a combo box because it does not have an edit control.  "; break;
		case ERROR_WINDOW_NOT_COMBOBOX : str = 	"The window is not a combo box.  "; break;
		case ERROR_INVALID_EDIT_HEIGHT : str = 	"Height must be less than 256.  "; break;
		case ERROR_DC_NOT_FOUND : str = 	"Invalid device context (DC) handle.  "; break;
		case ERROR_INVALID_HOOK_FILTER : str = 	"Invalid hook procedure type.  "; break;
		case ERROR_INVALID_FILTER_PROC : str = 	"Invalid hook procedure.  "; break;
		case ERROR_HOOK_NEEDS_HMOD : str = 	"Cannot set nonlocal hook without a module handle.  "; break;
		case ERROR_GLOBAL_ONLY_HOOK : str = 	"This hook procedure can only be set globally.  "; break;
		case ERROR_JOURNAL_HOOK_SET : str = 	"The journal hook procedure is already installed.  "; break;
		case ERROR_HOOK_NOT_INSTALLED : str = 	"The hook procedure is not installed.  "; break;
		case ERROR_INVALID_LB_MESSAGE : str = 	"Invalid message for single-selection list box.  "; break;
		case ERROR_SETCOUNT_ON_BAD_LB : str = 	"LB_SETCOUNT sent to non-lazy list box.  "; break;
		case ERROR_LB_WITHOUT_TABSTOPS : str = 	"This list box does not support tab stops.  "; break;
		case ERROR_DESTROY_OBJECT_OF_OTHER_THREAD : str = 	"Cannot destroy object created by another thread.  "; break;
		case ERROR_CHILD_WINDOW_MENU : str = 	"Child windows cannot have menus.  "; break;
		case ERROR_NO_SYSTEM_MENU : str = 	"The window does not have a system menu.  "; break;
		case ERROR_INVALID_MSGBOX_STYLE : str = 	"Invalid message box style.  "; break;
		case ERROR_INVALID_SPI_VALUE : str = 	"Invalid system-wide (SPI_*) parameter.  "; break;
		case ERROR_SCREEN_ALREADY_LOCKED : str = 	"Screen already locked.  "; break;
		case ERROR_HWNDS_HAVE_DIFF_PARENT : str = 	"All handles to windows in a multiple-window position structure must have the same parent.  "; break;
		case ERROR_NOT_CHILD_WINDOW : str = 	"The window is not a child window.  "; break;
		case ERROR_INVALID_GW_COMMAND : str = 	"Invalid GW_* command.  "; break;
		case ERROR_INVALID_THREAD_ID : str = 	"Invalid thread identifier.  "; break;
		case ERROR_NON_MDICHILD_WINDOW : str = 	"Cannot process a message from a window that is not a multiple document interface (MDI) window.  "; break;
		case ERROR_POPUP_ALREADY_ACTIVE : str = 	"Popup menu already active.  "; break;
		case ERROR_NO_SCROLLBARS : str = 	"The window does not have scroll bars.  "; break;
		case ERROR_INVALID_SCROLLBAR_RANGE : str = 	"Scroll bar range cannot be greater than 0x7FFF.  "; break;
		case ERROR_INVALID_SHOWWIN_COMMAND : str = 	"Cannot show or remove the window in the way specified.  "; break;
		case ERROR_NO_SYSTEM_RESOURCES : str = 	"Insufficient system resources exist to complete the requested service.  "; break;
		case ERROR_NONPAGED_SYSTEM_RESOURCES : str = 	"Insufficient system resources exist to complete the requested service.  "; break;
		case ERROR_PAGED_SYSTEM_RESOURCES : str = 	"Insufficient system resources exist to complete the requested service.  "; break;
		case ERROR_WORKING_SET_QUOTA : str = 	"Insufficient quota to complete the requested service.  "; break;
		case ERROR_PAGEFILE_QUOTA : str = 	"Insufficient quota to complete the requested service.  "; break;
		case ERROR_COMMITMENT_LIMIT : str = 	"The paging file is too small for this operation to complete.  "; break;
		case ERROR_MENU_ITEM_NOT_FOUND : str = 	"A menu item was not found.  "; break;
		case ERROR_INVALID_KEYBOARD_HANDLE : str = 	"Invalid keyboard layout handle.  "; break;
		case ERROR_HOOK_TYPE_NOT_ALLOWED : str = 	"Hook type not allowed.  "; break;
		case ERROR_REQUIRES_INTERACTIVE_WINDOWSTATION : str = 	"This operation requires an interactive window station.  "; break;
		case ERROR_TIMEOUT : str = 	"This operation returned because the timeout period expired.  "; break;
		case ERROR_INVALID_MONITOR_HANDLE : str = 	"Invalid monitor handle.  "; break;
		case ERROR_EVENTLOG_FILE_CORRUPT : str = 	"The event log file is corrupted.  "; break;
		case ERROR_EVENTLOG_CANT_START : str = 	"No event log file could be opened, so the event logging service did not start.  "; break;
		case ERROR_LOG_FILE_FULL : str = 	"The event log file is full.  "; break;
		case ERROR_EVENTLOG_FILE_CHANGED : str = 	"The event log file has changed between read operations.  "; break;
		//case ERROR_INSTALL_SERVICE : str = 	"Failure accessing install service.  "; break;
		case ERROR_INSTALL_USEREXIT : str = 	"The user canceled the installation.  "; break;
		case ERROR_INSTALL_FAILURE : str = 	"Fatal error during installation.  "; break;
		case ERROR_INSTALL_SUSPEND : str = 	"Installation suspended, incomplete.  "; break;
		case ERROR_UNKNOWN_PRODUCT : str = 	"Product code not registered.  "; break;
		case ERROR_UNKNOWN_FEATURE : str = 	"Feature ID not registered.  "; break;
		case ERROR_UNKNOWN_COMPONENT : str = 	"Component ID not registered.  "; break;
		case ERROR_UNKNOWN_PROPERTY : str = 	"Unknown property.  "; break;
		case ERROR_INVALID_HANDLE_STATE : str = 	"Handle is in an invalid state.  "; break;
		case ERROR_BAD_CONFIGURATION : str = 	"Configuration data corrupt.  "; break;
		case ERROR_INDEX_ABSENT : str = 	"Language not available.  "; break;
		case ERROR_INSTALL_SOURCE_ABSENT : str = 	"Install source unavailable.  "; break;
		//case ERROR_BAD_DATABASE_VERSION : str = 	"Database version unsupported.  "; break;
		case ERROR_PRODUCT_UNINSTALLED : str = 	"Product is uninstalled.  "; break;
		case ERROR_BAD_QUERY_SYNTAX : str = 	"SQL query syntax invalid or unsupported.  "; break;
		case ERROR_INVALID_FIELD : str = 	"Record field does not exist.  "; break;
		case RPC_S_INVALID_STRING_BINDING : str = 	"The string binding is invalid.  "; break;
		case RPC_S_WRONG_KIND_OF_BINDING : str = 	"The binding handle is not the correct type.  "; break;
		case RPC_S_INVALID_BINDING : str = 	"The binding handle is invalid.  "; break;
		case RPC_S_PROTSEQ_NOT_SUPPORTED : str = 	"The RPC protocol sequence is not supported.  "; break;
		case RPC_S_INVALID_RPC_PROTSEQ : str = 	"The RPC protocol sequence is invalid.  "; break;
		case RPC_S_INVALID_STRING_UUID : str = 	"The string universal unique identifier (UUID) is invalid.  "; break;
		case RPC_S_INVALID_ENDPOINT_FORMAT : str = 	"The endpoint format is invalid.  "; break;
		case RPC_S_INVALID_NET_ADDR : str = 	"The network address is invalid.  "; break;
		case RPC_S_NO_ENDPOINT_FOUND : str = 	"No endpoint was found.  "; break;
		case RPC_S_INVALID_TIMEOUT : str = 	"The timeout value is invalid.  "; break;
		case RPC_S_OBJECT_NOT_FOUND : str = 	"The object universal unique identifier (UUID) was not found.  "; break;
		case RPC_S_ALREADY_REGISTERED : str = 	"The object universal unique identifier (UUID) has already been registered.  "; break;
		case RPC_S_TYPE_ALREADY_REGISTERED : str = 	"The type universal unique identifier (UUID) has already been registered.  "; break;
		case RPC_S_ALREADY_LISTENING : str = 	"The RPC server is already listening.  "; break;
		case RPC_S_NO_PROTSEQS_REGISTERED : str = 	"No protocol sequences have been registered.  "; break;
		case RPC_S_NOT_LISTENING : str = 	"The RPC server is not listening.  "; break;
		case RPC_S_UNKNOWN_MGR_TYPE : str = 	"The manager type is unknown.  "; break;
		case RPC_S_UNKNOWN_IF : str = 	"The interface is unknown.  "; break;
		case RPC_S_NO_BINDINGS : str = 	"There are no bindings.  "; break;
		case RPC_S_NO_PROTSEQS : str = 	"There are no protocol sequences.  "; break;
		case RPC_S_CANT_CREATE_ENDPOINT : str = 	"The endpoint cannot be created.  "; break;
		case RPC_S_OUT_OF_RESOURCES : str = 	"Not enough resources are available to complete this operation.  "; break;
		case RPC_S_SERVER_UNAVAILABLE : str = 	"The RPC server is unavailable.  "; break;
		case RPC_S_SERVER_TOO_BUSY : str = 	"The RPC server is too busy to complete this operation.  "; break;
		case RPC_S_INVALID_NETWORK_OPTIONS : str = 	"The network options are invalid.  "; break;
		case RPC_S_NO_CALL_ACTIVE : str = 	"There are no remote procedure calls active on this thread.  "; break;
		case RPC_S_CALL_FAILED : str = 	"The remote procedure call failed.  "; break;
		case RPC_S_CALL_FAILED_DNE : str = 	"The remote procedure call failed and did not execute.  "; break;
		case RPC_S_PROTOCOL_ERROR : str = 	"A remote procedure call (RPC) protocol error occurred.  "; break;
		case RPC_S_UNSUPPORTED_TRANS_SYN : str = 	"The transfer syntax is not supported by the RPC server.  "; break;
		case RPC_S_UNSUPPORTED_TYPE : str = 	"The universal unique identifier (UUID) type is not supported.  "; break;
		case RPC_S_INVALID_TAG : str = 	"The tag is invalid.  "; break;
		case RPC_S_INVALID_BOUND : str = 	"The array bounds are invalid.  "; break;
		case RPC_S_NO_ENTRY_NAME : str = 	"The binding does not contain an entry name.  "; break;
		case RPC_S_INVALID_NAME_SYNTAX : str = 	"The name syntax is invalid.  "; break;
		case RPC_S_UNSUPPORTED_NAME_SYNTAX : str = 	"The name syntax is not supported.  "; break;
		case RPC_S_UUID_NO_ADDRESS : str = 	"No network address is available to use to construct a universal unique identifier (UUID).  "; break;
		case RPC_S_DUPLICATE_ENDPOINT : str = 	"The endpoint is a duplicate.  "; break;
		case RPC_S_UNKNOWN_AUTHN_TYPE : str = 	"The authentication type is unknown.  "; break;
		case RPC_S_MAX_CALLS_TOO_SMALL : str = 	"The maximum number of calls is too small.  "; break;
		case RPC_S_STRING_TOO_LONG : str = 	"The string is too long.  "; break;
		case RPC_S_PROTSEQ_NOT_FOUND : str = 	"The RPC protocol sequence was not found.  "; break;
		case RPC_S_PROCNUM_OUT_OF_RANGE : str = 	"The procedure number is out of range.  "; break;
		case RPC_S_BINDING_HAS_NO_AUTH : str = 	"The binding does not contain any authentication information.  "; break;
		case RPC_S_UNKNOWN_AUTHN_SERVICE : str = 	"The authentication service is unknown.  "; break;
		case RPC_S_UNKNOWN_AUTHN_LEVEL : str = 	"The authentication level is unknown.  "; break;
		case RPC_S_INVALID_AUTH_IDENTITY : str = 	"The security context is invalid.  "; break;
		case RPC_S_UNKNOWN_AUTHZ_SERVICE : str = 	"The authorization service is unknown.  "; break;
		case EPT_S_INVALID_ENTRY : str = 	"The entry is invalid.  "; break;
		case EPT_S_CANT_PERFORM_OP : str = 	"The server endpoint cannot perform the operation.  "; break;
		case EPT_S_NOT_REGISTERED : str = 	"There are no more endpoints available from the endpoint mapper.  "; break;
		case RPC_S_NOTHING_TO_EXPORT : str = 	"No interfaces have been exported.  "; break;
		case RPC_S_INCOMPLETE_NAME : str = 	"The entry name is incomplete.  "; break;
		case RPC_S_INVALID_VERS_OPTION : str = 	"The version option is invalid.  "; break;
		case RPC_S_NO_MORE_MEMBERS : str = 	"There are no more members.  "; break;
		case RPC_S_NOT_ALL_OBJS_UNEXPORTED : str = 	"There is nothing to unexport.  "; break;
		case RPC_S_INTERFACE_NOT_FOUND : str = 	"The interface was not found.  "; break;
		case RPC_S_ENTRY_ALREADY_EXISTS : str = 	"The entry already exists.  "; break;
		case RPC_S_ENTRY_NOT_FOUND : str = 	"The entry is not found.  "; break;
		case RPC_S_NAME_SERVICE_UNAVAILABLE : str = 	"The name service is unavailable.  "; break;
		case RPC_S_INVALID_NAF_ID : str = 	"The network address family is invalid.  "; break;
		case RPC_S_CANNOT_SUPPORT : str = 	"The requested operation is not supported.  "; break;
		case RPC_S_NO_CONTEXT_AVAILABLE : str = 	"No security context is available to allow impersonation.  "; break;
		case RPC_S_INTERNAL_ERROR : str = 	"An internal error occurred in a remote procedure call (RPC).  "; break;
		case RPC_S_ZERO_DIVIDE : str = 	"The RPC server attempted an integer division by zero.  "; break;
		case RPC_S_ADDRESS_ERROR : str = 	"An addressing error occurred in the RPC server.  "; break;
		case RPC_S_FP_DIV_ZERO : str = 	"A floating-point operation at the RPC server caused a division by zero.  "; break;
		case RPC_S_FP_UNDERFLOW : str = 	"A floating-point underflow occurred at the RPC server.  "; break;
		case RPC_S_FP_OVERFLOW : str = 	"A floating-point overflow occurred at the RPC server.  "; break;
		case RPC_X_NO_MORE_ENTRIES : str = 	"The list of RPC servers available for the binding of auto handles has been exhausted.  "; break;
		case RPC_X_SS_CHAR_TRANS_OPEN_FAIL : str = 	"Unable to open the character translation table file.  "; break;
		case RPC_X_SS_CHAR_TRANS_SHORT_FILE : str = 	"The file containing the character translation table has fewer than bytes.  "; break;
		case RPC_X_SS_IN_NULL_CONTEXT : str = 	"A null context handle was passed from the client to the host during a remote procedure call.  "; break;
		case RPC_X_SS_CONTEXT_DAMAGED : str = 	"The context handle changed during a remote procedure call.  "; break;
		case RPC_X_SS_HANDLES_MISMATCH : str = 	"The binding handles passed to a remote procedure call do not match.  "; break;
		case RPC_X_SS_CANNOT_GET_CALL_HANDLE : str = 	"The stub is unable to get the remote procedure call handle.  "; break;
		case RPC_X_NULL_REF_POINTER : str = 	"A null reference pointer was passed to the stub.  "; break;
		case RPC_X_ENUM_VALUE_OUT_OF_RANGE : str = 	"The enumeration value is out of range.  "; break;
		case RPC_X_BYTE_COUNT_TOO_SMALL : str = 	"The byte count is too small.  "; break;
		case RPC_X_BAD_STUB_DATA : str = 	"The stub received bad data.  "; break;
		case ERROR_INVALID_USER_BUFFER : str = 	"The supplied user buffer is not valid for the requested operation.  "; break;
		case ERROR_UNRECOGNIZED_MEDIA : str = 	"The disk media is not recognized. It may not be formatted.  "; break;
		case ERROR_NO_TRUST_LSA_SECRET : str = 	"The workstation does not have a trust secret.  "; break;
		case ERROR_NO_TRUST_SAM_ACCOUNT : str = 	"The SAM database on the Windows NT Server does not have a computer account for this workstation trust relationship.  "; break;
		case ERROR_TRUSTED_DOMAIN_FAILURE : str = 	"The trust relationship between the primary domain and the trusted domain failed.  "; break;
		case ERROR_TRUSTED_RELATIONSHIP_FAILURE : str = 	"The trust relationship between this workstation and the primary domain failed.  "; break;
		case ERROR_TRUST_FAILURE : str = 	"The network logon failed.  "; break;
		case RPC_S_CALL_IN_PROGRESS : str = 	"A remote procedure call is already in progress for this thread.  "; break;
		case ERROR_NETLOGON_NOT_STARTED : str = 	"An attempt was made to logon, but the network logon service was not started.  "; break;
		case ERROR_ACCOUNT_EXPIRED : str = 	"The user's account has expired.  "; break;
		case ERROR_REDIRECTOR_HAS_OPEN_HANDLES : str = 	"The redirector is in use and cannot be unloaded.  "; break;
		case ERROR_PRINTER_DRIVER_ALREADY_INSTALLED : str = 	"The specified printer driver is already installed.  "; break;
		case ERROR_UNKNOWN_PORT : str = 	"The specified port is unknown.  "; break;
		case ERROR_UNKNOWN_PRINTER_DRIVER : str = 	"The printer driver is unknown.  "; break;
		case ERROR_UNKNOWN_PRINTPROCESSOR : str = 	"The print processor is unknown.  "; break;
		case ERROR_INVALID_SEPARATOR_FILE : str = 	"The specified separator file is invalid.  "; break;
		case ERROR_INVALID_PRIORITY : str = 	"The specified priority is invalid.  "; break;
		case ERROR_INVALID_PRINTER_NAME : str = 	"The printer name is invalid.  "; break;
		case ERROR_PRINTER_ALREADY_EXISTS : str = 	"The printer already exists.  "; break;
		case ERROR_INVALID_PRINTER_COMMAND : str = 	"The printer command is invalid.  "; break;
		case ERROR_INVALID_DATATYPE : str = 	"The specified datatype is invalid.  "; break;
		case ERROR_INVALID_ENVIRONMENT : str = 	"The environment specified is invalid.  "; break;
		case RPC_S_NO_MORE_BINDINGS : str = 	"There are no more bindings.  "; break;
		case ERROR_NOLOGON_INTERDOMAIN_TRUST_ACCOUNT : str = 	"The account used is an interdomain trust account. Use your global user account or local user account to access this server.  "; break;
		case ERROR_NOLOGON_WORKSTATION_TRUST_ACCOUNT : str = 	"The account used is a computer account. Use your global user account or local user account to access this server.  "; break;
		case ERROR_NOLOGON_SERVER_TRUST_ACCOUNT : str = 	"The account used is a server trust account. Use your global user account or local user account to access this server.  "; break;
		case ERROR_DOMAIN_TRUST_INCONSISTENT : str = 	"The name or security ID (SID) of the domain specified is inconsistent with the trust information for that domain.  "; break;
		case ERROR_SERVER_HAS_OPEN_HANDLES : str = 	"The server is in use and cannot be unloaded.  "; break;
		case ERROR_RESOURCE_DATA_NOT_FOUND : str = 	"The specified image file did not contain a resource section.  "; break;
		case ERROR_RESOURCE_TYPE_NOT_FOUND : str = 	"The specified resource type cannot be found in the image file.  "; break;
		case ERROR_RESOURCE_NAME_NOT_FOUND : str = 	"The specified resource name cannot be found in the image file.  "; break;
		case ERROR_RESOURCE_LANG_NOT_FOUND : str = 	"The specified resource language ID cannot be found in the image file.  "; break;
		case ERROR_NOT_ENOUGH_QUOTA : str = 	"Not enough quota is available to process this command.  "; break;
		case RPC_S_NO_INTERFACES : str = 	"No interfaces have been registered.  "; break;
		case RPC_S_CALL_CANCELLED : str = 	"The remote procedure call was cancelled.  "; break;
		case RPC_S_BINDING_INCOMPLETE : str = 	"The binding handle does not contain all required information.  "; break;
		case RPC_S_COMM_FAILURE : str = 	"A communications failure occurred during a remote procedure call.  "; break;
		case RPC_S_UNSUPPORTED_AUTHN_LEVEL : str = 	"The requested authentication level is not supported.  "; break;
		case RPC_S_NO_PRINC_NAME : str = 	"No principal name registered.  "; break;
		case RPC_S_NOT_RPC_ERROR : str = 	"The error specified is not a valid Windows RPC error code.  "; break;
		case RPC_S_UUID_LOCAL_ONLY : str = 	"A UUID that is valid only on this computer has been allocated.  "; break;
		case RPC_S_SEC_PKG_ERROR : str = 	"A security package specific error occurred.  "; break;
		case RPC_S_NOT_CANCELLED : str = 	"Thread is not canceled.  "; break;
		case RPC_X_INVALID_ES_ACTION : str = 	"Invalid operation on the encoding/decoding handle.  "; break;
		case RPC_X_WRONG_ES_VERSION : str = 	"Incompatible version of the serializing package.  "; break;
		case RPC_X_WRONG_STUB_VERSION : str = 	"Incompatible version of the RPC stub.  "; break;
		case RPC_X_INVALID_PIPE_OBJECT : str = 	"The RPC pipe object is invalid or corrupted.  "; break;
		case RPC_X_WRONG_PIPE_ORDER : str = 	"An invalid operation was attempted on an RPC pipe object.  "; break;
		case RPC_X_WRONG_PIPE_VERSION : str = 	"Unsupported RPC pipe version.  "; break;
		case RPC_S_GROUP_MEMBER_NOT_FOUND : str = 	"The group member was not found.  "; break;
		case EPT_S_CANT_CREATE : str = 	"The endpoint mapper database entry could not be created.  "; break;
		case RPC_S_INVALID_OBJECT : str = 	"The object universal unique identifier (UUID) is the nil UUID.  "; break;
		case ERROR_INVALID_TIME : str = 	"The specified time is invalid.  "; break;
		case ERROR_INVALID_FORM_NAME : str = 	"The specified form name is invalid.  "; break;
		case ERROR_INVALID_FORM_SIZE : str = 	"The specified form size is invalid.  "; break;
		case ERROR_ALREADY_WAITING : str = 	"The specified printer handle is already being waited on  "; break;
		case ERROR_PRINTER_DELETED : str = 	"The specified printer has been deleted.  "; break;
		case ERROR_INVALID_PRINTER_STATE : str = 	"The state of the printer is invalid.  "; break;
		case ERROR_PASSWORD_MUST_CHANGE : str = 	"The user must change his password before he logs on the first time.  "; break;
		case ERROR_DOMAIN_CONTROLLER_NOT_FOUND : str = 	"Could not find the domain controller for this domain.  "; break;
		case ERROR_ACCOUNT_LOCKED_OUT : str = 	"The referenced account is currently locked out and may not be logged on to.  "; break;
		case OR_INVALID_OXID : str = 	"The object exporter specified was not found.  "; break;
		case OR_INVALID_OID : str = 	"The object specified was not found.  "; break;
		case OR_INVALID_SET : str = 	"The object resolver set specified was not found.  "; break;
		case RPC_S_SEND_INCOMPLETE : str = 	"Some data remains to be sent in the request buffer.  "; break;
		case RPC_S_INVALID_ASYNC_HANDLE : str = 	"Invalid asynchronous remote procedure call handle.  "; break;
		case RPC_S_INVALID_ASYNC_CALL : str = 	"Invalid asynchronous RPC call handle for this operation.  "; break;
		case RPC_X_PIPE_CLOSED : str = 	"The RPC pipe object has already been closed.  "; break;
		case RPC_X_PIPE_DISCIPLINE_ERROR : str = 	"The RPC call completed before all pipes were processed.  "; break;
		case RPC_X_PIPE_EMPTY : str = 	"No more data is available from the RPC pipe.  "; break;
		case ERROR_NO_SITENAME : str = 	"No site name is available for this machine.  "; break;
		case ERROR_CANT_ACCESS_FILE : str = 	"The file can not be accessed by the system.  "; break;
		case ERROR_CANT_RESOLVE_FILENAME : str = 	"The name of the file cannot be resolved by the system.  "; break;
		case ERROR_DS_MEMBERSHIP_EVALUATED_LOCALLY : str = 	"The directory service evaluated group memberships locally.  "; break;
		case ERROR_DS_NO_ATTRIBUTE_OR_VALUE : str = 	"The specified directory service attribute or value does not exist.  "; break;
		case ERROR_DS_INVALID_ATTRIBUTE_SYNTAX : str = 	"The attribute syntax specified to the directory service is invalid.  "; break;
		case ERROR_DS_ATTRIBUTE_TYPE_UNDEFINED : str = 	"The attribute type specified to the directory service is not defined.  "; break;
		case ERROR_DS_ATTRIBUTE_OR_VALUE_EXISTS : str = 	"The specified directory service attribute or value already exists.  "; break;
		case ERROR_DS_BUSY : str = 	"The directory service is busy.  "; break;
		case ERROR_DS_UNAVAILABLE : str = 	"The directory service is unavailable.  "; break;
		case ERROR_DS_NO_RIDS_ALLOCATED : str = 	"The directory service was unable to allocate a relative identifier.  "; break;
		case ERROR_DS_NO_MORE_RIDS : str = 	"The directory service has exhausted the pool of relative identifiers.  "; break;
		case ERROR_DS_INCORRECT_ROLE_OWNER : str = 	"The requested operation could not be performed because the directory service is not the master for that type of operation.  "; break;
		case ERROR_DS_RIDMGR_INIT_ERROR : str = 	"The directory service was unable to initialize the subsystem that allocates relative identifiers.  "; break;
		case ERROR_DS_OBJ_CLASS_VIOLATION : str = 	"The requested operation did not satisfy one or more constraints associated with the class of the object.  "; break;
		case ERROR_DS_CANT_ON_NON_LEAF : str = 	"The directory service can perform the requested operation only on a leaf object.  "; break;
		case ERROR_DS_CANT_ON_RDN : str = 	"The directory service cannot perform the requested operation on the RDN attribute of an object.  "; break;
		case ERROR_DS_CANT_MOD_OBJ_CLASS : str = 	"The directory service detected an attempt to modify the object class of an object.  "; break;
		case ERROR_DS_CROSS_DOM_MOVE_ERROR : str = 	"The requested cross domain move operation could not be performed.  "; break;
		case ERROR_DS_GC_NOT_AVAILABLE : str = 	"Unable to contact the global catalog server.  "; break;
		case ERROR_INVALID_PIXEL_FORMAT : str = 	"The pixel format is invalid.  "; break;
		case ERROR_BAD_DRIVER : str = 	"The specified driver is invalid.  "; break;
		case ERROR_INVALID_WINDOW_STYLE : str = 	"The window style or class attribute is invalid for this operation.  "; break;
		case ERROR_METAFILE_NOT_SUPPORTED : str = 	"The requested metafile operation is not supported.  "; break;
		case ERROR_TRANSFORM_NOT_SUPPORTED : str = 	"The requested transformation operation is not supported.  "; break;
		case ERROR_CLIPPING_NOT_SUPPORTED : str = 	"The requested clipping operation is not supported.  "; break;
		case ERROR_CONNECTED_OTHER_PASSWORD : str = 	"The network connection was made successfully, but the user had to be prompted for a password other than the one originally specified.  "; break;
		case ERROR_BAD_USERNAME : str = 	"The specified username is invalid.  "; break;
		case ERROR_NOT_CONNECTED : str = 	"This network connection does not exist.  "; break;
		case ERROR_INVALID_CMM : str = 	"The specified color management module is invalid.  "; break;
		case ERROR_INVALID_PROFILE : str = 	"The specified color profile is invalid.  "; break;
		case ERROR_TAG_NOT_FOUND : str = 	"The specified tag was not found.  "; break;
		case ERROR_TAG_NOT_PRESENT : str = 	"A required tag is not present.  "; break;
		case ERROR_DUPLICATE_TAG : str = 	"The specified tag is already present.  "; break;
		case ERROR_PROFILE_NOT_ASSOCIATED_WITH_DEVICE : str = 	"The specified color profile is not associated with any device.  "; break;
		case ERROR_PROFILE_NOT_FOUND : str = 	"The specified color profile was not found.  "; break;
		case ERROR_INVALID_COLORSPACE : str = 	"The specified color space is invalid.  "; break;
		case ERROR_ICM_NOT_ENABLED : str = 	"Image Color Management is not enabled.  "; break;
		case ERROR_DELETING_ICM_XFORM : str = 	"There was an error while deleting the color transform.  "; break;
		case ERROR_INVALID_TRANSFORM : str = 	"The specified color transform is invalid.  "; break;
		case ERROR_OPEN_FILES : str = 	"This network connection has files open or requests pending.  "; break;
		case ERROR_ACTIVE_CONNECTIONS : str = 	"Active connections still exist.  "; break;
		case ERROR_DEVICE_IN_USE : str = 	"The device is in use by an active process and cannot be disconnected.  "; break;
		case ERROR_UNKNOWN_PRINT_MONITOR : str = 	"The specified print monitor is unknown.  "; break;
		case ERROR_PRINTER_DRIVER_IN_USE : str = 	"The specified printer driver is currently in use.  "; break;
		case ERROR_SPOOL_FILE_NOT_FOUND : str = 	"The spool file was not found.  "; break;
		case ERROR_SPL_NO_STARTDOC : str = 	"A StartDocPrinter call was not issued.  "; break;
		case ERROR_SPL_NO_ADDJOB : str = 	"An AddJob call was not issued.  "; break;
		case ERROR_PRINT_PROCESSOR_ALREADY_INSTALLED : str = 	"The specified print processor has already been installed.  "; break;
		case ERROR_PRINT_MONITOR_ALREADY_INSTALLED : str = 	"The specified print monitor has already been installed.  "; break;
		case ERROR_INVALID_PRINT_MONITOR : str = 	"The specified print monitor does not have the required functions.  "; break;
		case ERROR_PRINT_MONITOR_IN_USE : str = 	"The specified print monitor is currently in use.  "; break;
		case ERROR_PRINTER_HAS_JOBS_QUEUED : str = 	"The requested operation is not allowed when there are jobs queued to the printer.  "; break;
		case ERROR_SUCCESS_REBOOT_REQUIRED : str = 	"The requested operation is successful. Changes will not be effective until the system is rebooted.  "; break;
		case ERROR_SUCCESS_RESTART_REQUIRED : str = 	"The requested operation is successful. Changes will not be effective until the service is restarted.  "; break;
		case ERROR_WINS_INTERNAL : str = 	"WINS encountered an error while processing the command.  "; break;
		case ERROR_CAN_NOT_DEL_LOCAL_WINS : str = 	"The local WINS can not be deleted.  "; break;
		case ERROR_STATIC_INIT : str = 	"The importation from the file failed.  "; break;
		case ERROR_INC_BACKUP : str = 	"The backup failed. Was a full backup done before?  "; break;
		case ERROR_FULL_BACKUP : str = 	"The backup failed. Check the directory to which you are backing the database.  "; break;
		case ERROR_REC_NON_EXISTENT : str = 	"The name does not exist in the WINS database.  "; break;
		case ERROR_RPL_NOT_ALLOWED : str = 	"Replication with a nonconfigured partner is not allowed.  "; break;
		case ERROR_DHCP_ADDRESS_CONFLICT : str = 	"The DHCP client has obtained an IP address that is already in use on the network. The local interface will be disabled until the DHCP client can obtain a new address.  "; break;
		case ERROR_WMI_GUID_NOT_FOUND : str = 	"The GUID passed was not recognized as valid by a WMI data provider.  "; break;
		case ERROR_WMI_INSTANCE_NOT_FOUND : str = 	"The instance name passed was not recognized as valid by a WMI data provider.  "; break;
		case ERROR_WMI_ITEMID_NOT_FOUND : str = 	"The data item ID passed was not recognized as valid by a WMI data provider.  "; break;
		case ERROR_WMI_TRY_AGAIN : str = 	"The WMI request could not be completed and should be retried.  "; break;
		case ERROR_WMI_DP_NOT_FOUND : str = 	"The WMI data provider could not be located.  "; break;
		case ERROR_WMI_UNRESOLVED_INSTANCE_REF : str = 	"The WMI data provider references an instance set that has not been registered.  "; break;
		case ERROR_WMI_ALREADY_ENABLED : str = 	"The WMI data block or event notification has already been enabled.  "; break;
		case ERROR_WMI_GUID_DISCONNECTED : str = 	"The WMI data block is no longer available.  "; break;
		case ERROR_WMI_SERVER_UNAVAILABLE : str = 	"The WMI data service is not available.  "; break;
		case ERROR_WMI_DP_FAILED : str = 	"The WMI data provider failed to carry out the request.  "; break;
		case ERROR_WMI_INVALID_MOF : str = 	"The WMI MOF information is not valid.  "; break;
		case ERROR_WMI_INVALID_REGINFO : str = 	"The WMI registration information is not valid.  "; break;
		case ERROR_INVALID_MEDIA : str = 	"The media identifier does not represent a valid medium.  "; break;
		case ERROR_INVALID_LIBRARY : str = 	"The library identifier does not represent a valid library.  "; break;
		case ERROR_INVALID_MEDIA_POOL : str = 	"The media pool identifier does not represent a valid media pool.  "; break;
		case ERROR_DRIVE_MEDIA_MISMATCH : str = 	"The drive and medium are not compatible or exist in different libraries.  "; break;
		case ERROR_MEDIA_OFFLINE : str = 	"The medium currently exists in an offline library and must be online to perform this operation.  "; break;
		case ERROR_LIBRARY_OFFLINE : str = 	"The operation cannot be performed on an offline library.  "; break;
		case ERROR_EMPTY : str = 	"The library, drive, or media pool is empty.  "; break;
		case ERROR_NOT_EMPTY : str = 	"The library, drive, or media pool must be empty to perform this operation.  "; break;
		case ERROR_MEDIA_UNAVAILABLE : str = 	"No media is currently available in this media pool or library.  "; break;
		case ERROR_RESOURCE_DISABLED : str = 	"A resource required for this operation is disabled.  "; break;
		case ERROR_INVALID_CLEANER : str = 	"The media identifier does not represent a valid cleaner.  "; break;
		case ERROR_UNABLE_TO_CLEAN : str = 	"The drive cannot be cleaned or does not support cleaning.  "; break;
		case ERROR_OBJECT_NOT_FOUND : str = 	"The object identifier does not represent a valid object.  "; break;
		case ERROR_DATABASE_FAILURE : str = 	"Unable to read from or write to the database.  "; break;
		case ERROR_DATABASE_FULL : str = 	"The database is full.  "; break;
		case ERROR_MEDIA_INCOMPATIBLE : str = 	"The medium is not compatible with the device or media pool.  "; break;
		case ERROR_RESOURCE_NOT_PRESENT : str = 	"The resource required for this operation does not exist.  "; break;
		case ERROR_INVALID_OPERATION : str = 	"The operation identifier is not valid.  "; break;
		case ERROR_MEDIA_NOT_AVAILABLE : str = 	"The media is not mounted or ready for use.  "; break;
		case ERROR_DEVICE_NOT_AVAILABLE : str = 	"The device is not ready for use.  "; break;
		case ERROR_REQUEST_REFUSED : str = 	"The operator or administrator has refused the request.  "; break;
		case ERROR_FILE_OFFLINE : str = 	"The remote storage service was not able to recall the file.  "; break;
		case ERROR_REMOTE_STORAGE_NOT_ACTIVE : str = 	"The remote storage service is not operational at this time.  "; break;
		case ERROR_REMOTE_STORAGE_MEDIA_ERROR : str = 	"The remote storage service encountered a media error.  "; break;
		case ERROR_NOT_A_REPARSE_POINT : str = 	"The file or directory is not a reparse point.  "; break;
		case ERROR_REPARSE_ATTRIBUTE_CONFLICT : str = 	"The reparse point attribute cannot be set because it conflicts with an existing attribute.  "; break;
		case ERROR_DEPENDENT_RESOURCE_EXISTS : str = 	"The cluster resource cannot be moved to another group because other resources are dependent on it.  "; break;
		case ERROR_DEPENDENCY_NOT_FOUND : str = 	"The cluster resource dependency cannot be found.  "; break;
		case ERROR_DEPENDENCY_ALREADY_EXISTS : str = 	"The cluster resource cannot be made dependent on the specified resource because it is already dependent.  "; break;
		case ERROR_RESOURCE_NOT_ONLINE : str = 	"The cluster resource is not online.  "; break;
		case ERROR_HOST_NODE_NOT_AVAILABLE : str = 	"A cluster node is not available for this operation.  "; break;
		case ERROR_RESOURCE_NOT_AVAILABLE : str = 	"The cluster resource is not available.  "; break;
		case ERROR_RESOURCE_NOT_FOUND : str = 	"The cluster resource could not be found.  "; break;
		case ERROR_SHUTDOWN_CLUSTER : str = 	"The cluster is being shut down.  "; break;
		case ERROR_CANT_EVICT_ACTIVE_NODE : str = 	"A cluster node cannot be evicted from the cluster while it is online.  "; break;
		case ERROR_OBJECT_ALREADY_EXISTS : str = 	"The object already exists.  "; break;
		case ERROR_OBJECT_IN_LIST : str = 	"The object is already in the list.  "; break;
		case ERROR_GROUP_NOT_AVAILABLE : str = 	"The cluster group is not available for any new requests.  "; break;
		case ERROR_GROUP_NOT_FOUND : str = 	"The cluster group could not be found.  "; break;
		case ERROR_GROUP_NOT_ONLINE : str = 	"The operation could not be completed because the cluster group is not online.  "; break;
		case ERROR_HOST_NODE_NOT_RESOURCE_OWNER : str = 	"The cluster node is not the owner of the resource.  "; break;
		case ERROR_HOST_NODE_NOT_GROUP_OWNER : str = 	"The cluster node is not the owner of the group.  "; break;
		case ERROR_RESMON_CREATE_FAILED : str = 	"The cluster resource could not be created in the specified resource monitor.  "; break;
		case ERROR_RESMON_ONLINE_FAILED : str = 	"The cluster resource could not be brought online by the resource monitor.  "; break;
		case ERROR_RESOURCE_ONLINE : str = 	"The operation could not be completed because the cluster resource is online.  "; break;
		case ERROR_QUORUM_RESOURCE : str = 	"The cluster resource could not be deleted or brought offline because it is the quorum resource.  "; break;
		case ERROR_NOT_QUORUM_CAPABLE : str = 	"The cluster could not make the specified resource a quorum resource because it is not capable of being a quorum resource.  "; break;
		case ERROR_CLUSTER_SHUTTING_DOWN : str = 	"The cluster software is shutting down.  "; break;
		case ERROR_INVALID_STATE : str = 	"The group or resource is not in the correct state to perform the requested operation.  "; break;
		case ERROR_RESOURCE_PROPERTIES_STORED : str = 	"The properties were stored but not all changes will take effect until the next time the resource is brought online.  "; break;
		case ERROR_NOT_QUORUM_CLASS : str = 	"The cluster could not make the specified resource a quorum resource because it does not belong to a shared storage class.  "; break;
		case ERROR_CORE_RESOURCE : str = 	"The cluster resource could not be deleted since it is a core resource.  "; break;
		case ERROR_QUORUM_RESOURCE_ONLINE_FAILED : str = 	"The quorum resource failed to come online.  "; break;
		case ERROR_QUORUMLOG_OPEN_FAILED : str = 	"The quorum log could not be created or mounted successfully.  "; break;
		case ERROR_CLUSTERLOG_CORRUPT : str = 	"The cluster log is corrupt.  "; break;
		case ERROR_CLUSTERLOG_RECORD_EXCEEDS_MAXSIZE : str = 	"The record could not be written to the cluster log since it exceeds the maximum size.  "; break;
		case ERROR_CLUSTERLOG_EXCEEDS_MAXSIZE : str = 	"The cluster log exceeds its maximum size.  "; break;
		case ERROR_CLUSTERLOG_CHKPOINT_NOT_FOUND : str = 	"No checkpoint record was found in the cluster log.  "; break;
		case ERROR_CLUSTERLOG_NOT_ENOUGH_SPACE : str = 	"The minimum required disk space needed for logging is not available.  "; break;
		case ERROR_ENCRYPTION_FAILED : str = 	"The specified file could not be encrypted.  "; break;
		case ERROR_DECRYPTION_FAILED : str = 	"The specified file could not be decrypted.  "; break;
		case ERROR_FILE_ENCRYPTED : str = 	"The specified file is encrypted and the user does not have the ability to decrypt it.  "; break;
		case ERROR_NO_RECOVERY_POLICY : str = 	"There is no encryption recovery policy configured for this system.  "; break;
		case ERROR_NO_EFS : str = 	"The required encryption driver is not loaded for this system.  "; break;
		case ERROR_WRONG_EFS : str = 	"The file was encrypted with a different encryption driver than is currently loaded.  "; break;
		case ERROR_NO_USER_KEYS : str = 	"There are no EFS keys defined for the user.  "; break;
		case ERROR_FILE_NOT_ENCRYPTED : str = 	"The specified file is not encrypted.  "; break;
		case ERROR_NOT_EXPORT_FORMAT : str = 	"The specified file is not in the defined EFS export format.  "; break;
		case ERROR_NO_BROWSER_SERVERS_FOUND : str = 	"The list of servers for this workgroup is not currently available  "; break;
	}
	return str;
}


