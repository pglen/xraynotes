
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

static  HWND    GetChild(HWND  hwndParent);
static  HWND    child;

#include "mxpad.h"

#define APPNAME "MxPad"
#define MBOX(aa)  MessageBox(NULL, aa, "", MB_OK)

#define	DUMPLEN	32000
#define	OUTLEN	4096


#define CHUNK 16

class MxPad mxpad;


static	char    *szOutString;
static  char	*dump_str;

BOOL    CALLBACK EnumChildProc(HWND  hwnd, LPARAM  lParam)

{
    child = hwnd;
    return(0);
}

static  HWND    GetChild(HWND  hwndParent)

{
    EnumChildWindows(
                hwndParent,         // handle of parent window
                EnumChildProc,      // address of callback function
                0                   // application-defined value
               );

    return(child);
}

SOCKADDR_IN rec_sin2;

/*
 * Only one of these will run, no access control needed.
 */

DWORD AcceptThreadProc( DWORD *ptp )

{
    char	nd  = 1;
	int		ndlen  = sizeof(char);

    SOCKADDR_IN rec_sin;

    int     rec_sin_len = sizeof(rec_sin2);
    int     ret_val;
    char *str;
    SOCKET sock2, sock3;

    //MBOX("Hello thread %d");

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

        MBOX("Cannot Bind2 socket\n");
        //exit(0);
        }

    //printf("After bind2.\n");

    if (listen( sock2, 1) < 0)
        {
        MBOX("Cannot listen on socket2\n");
        //exit(0);
        }

    //printf("After listen2.\n");

    rec_sin_len = sizeof(rec_sin2);

    sock3 = accept( sock2,(struct sockaddr FAR *) &rec_sin2,
            (int FAR *) &rec_sin_len );

    str = inet_ntoa( *((struct in_addr*) &rec_sin2.sin_addr.s_addr));
    //MBOX(str);

    closesocket(sock3); closesocket(sock2);

    //("End thread.\n");

    //fflush(stdout);

    // Release crap

    *ptp = 1;
    return(0); 
}

//////////////////////////////////////////////////////////////////////////

MxPad::~MxPad()

{
	//P2N("Freeing mem\r\n");

	if(dump_str) free(dump_str);
	if(szOutString) free(szOutString);
}

//////////////////////////////////////////////////////////////////////////

MxPad::MxPad()

{
	nd = 1;

	dump_str = szOutString = NULL;

	dump_str = (char *) malloc(DUMPLEN);
	//memset(dump_str, 'a', DUMPLEN);

	szOutString = (char *) malloc(OUTLEN);
	//memset(szOutString, 'a', OUTLEN);

	ndlen = sizeof(int);
	connected = false;
	resolved = false;
	local = true;
	remote = false;
}

//////////////////////////////////////////////////////////////////////////

int     MxPad::printf(const char *Format, ...)

{
	#ifndef _DEBUG
		return (0);
	#endif

    va_list ArgList;

    va_start(ArgList, Format);

    int len = _vsnprintf(szOutString, OUTLEN - 1, Format, ArgList);

	// Terminate if overflowed
	if(len < 0)	
		{
		szOutString[OUTLEN - 1] = '\0';
		}

    ToNotepad(szOutString);

	if(len < 0)	
		{
		ToNotepad("\r\nWarning: Output string was cut to prevent buffer overflow.\r\n");
		}

    return(0);
}

///////////////////////////////////////////////////////////////////////////


int     MxPad::dump(const void *mem, int len)

{
    char    *dump_ptr = dump_str;
    char    *ptr = (char *)mem;
    int     slen = 0;
	int		chunk = CHUNK;

	#ifndef _DEBUG
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
            slen += sprintf(dump_str + slen, "%02x ", *(ptr+aa) & 0xff);

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

MxPad::SetLocal(int flag)

{
	local = flag;
}

MxPad::SetRemote(int flag)

{
	remote = flag;
}

#if 0

//////////////////////////////////////////////////////////////////////////

int     MxPad::dumpmx(const char *dptr, int len)

{
    char    *dump_ptr = dump_str;
    char    *ptr = (char*)dptr;
    int     slen = 0;

#ifndef _DEBUG
	return (0);
#endif

    if (!dptr)
        {
        ToNotepad("NULL"); return(0);
        }

    while(TRUE)
        {
        int aa;

        if(len <= 0)                                  /* end of buffer */
            break;

        if(slen >=  DUMPLEN - 96)        /* end of local buffer */
			{
			slen += sprintf(dump_str + slen, "\r\nDump string cut to prevent buffer overflow\r\n");
            break;
			}

        slen += sprintf(dump_str + slen, "  -  ");

        for(aa = 0; aa < 8; aa++)
            slen += sprintf(dump_str + slen, "%02x ", *(ptr+aa) & 0xff);

        slen += sprintf(dump_str + slen, "  -  ");

        for(aa = 0; aa < 8; aa++)
            {
            char chh = *(ptr+aa);

            if(!isprint((int)chh))                 /* convert to USC if not prt */
                chh = '.';

            slen += sprintf(dump_str + slen, "%c", chh);
            }
        slen += sprintf(dump_str + slen, "  -  ");
        slen += sprintf(dump_str + slen, "\r\n");

        len -= 8; ptr += 8;
        }
    ToNotepad(dump_str);
    return(0);
}

#endif

int     MxPad::Clear()

{

		if(local)
		{
		HWND    chld;

		if((chld = GetChild(FindWindow(TEXT(APPNAME), NULL)) ))
			{
			SendMessage(chld, EM_SETSEL, 0, -1);
			Edit_ReplaceSel(chld, "");
			}
		}
	return 0;

}
//////////////////////////////////////////////////////////////////////////

int     MxPad::ToNotepad(char  *str)

{

	#ifndef _DEBUG
	return (0);
	#endif

	if(local)
		{
		HWND    chld;

		if((chld = GetChild(FindWindow(TEXT(APPNAME), NULL)) ))
			Edit_ReplaceSel(chld, str);
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

/*
* UnicodeToAnsi converts the Unicode string pszW to an ANSI string
* and returns the ANSI string through ppszA. Space for the
* the converted string is allocated by UnicodeToAnsi.
*/

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

/*
 * AnsiToUnicode converts the ANSI string pszA to a Unicode string
 * and returns the Unicode string through ppszW. Space for the
 * the converted string is allocated by AnsiToUnicode.
 */

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

	tme = GetTickCount() + 3000;

	while(1)
		{
		MSG  msg;

		if(tme < GetTickCount())            // Timeout
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
		AfxMessageBox("cannot connect", MB_OK);
		#endif

		return 0;
		}
	connected = true;

	return connected;
}
