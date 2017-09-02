
/* =====[ MxPad.h ]=========================================================

   Description:

            Interface for MX debug pad.

      REV     DATE     BY           DESCRIPTION
      ----  --------  ----------    --------------------------------------
      0.00  03/09/99  Peter Glen    Initial version.
	  0.00  11/08/05  Peter Glen    Network Version

   ======================================================================= */

#ifndef mxpad_defined
#define mxpad_defined

#define CIRC_DEF_SIZE	4096

// Just like printf, but prints to debug pad
//

#include "winsock.h"


class MxPad

{
	static  HWND    child;
	static	SOCKADDR_IN rec_sin2;
	
	static  HWND    GetChild(HWND  hwndParent);
	static	BOOL	CALLBACK EnumChildProc(HWND  hwnd, LPARAM  lParam);

	static	DWORD	AcceptThreadProc(DWORD *ptp);
	static	void	RunMxThread(void *lpParam);


public:
	
	// Set this to true if you want release mode debugging
	int force;		
	
class CxCircular  

{

public:

	int Add(char chh);
	int Get(char *chh);

	CxCircular();
	CxCircular(int bufsize);

	virtual ~CxCircular();

protected:

	int head, tail, size;
	char	*buff;

	InitVars();
};

protected:

	int		remote, local, trace, handle;
	char    *szOutString, *aszOutString;
	char	*dump_str;

public:

	MxPad();
	~MxPad();

	int		endthread;
	CxCircular circ;

	SetLocal(int flag);
	SetRemote(int flag);
	SetTrace(int flag);

	void	ClearNotepad();

	//
	// See if any agent is running (local, remote)
	//

	int IsLocalAvail();
	int IsRemoteAvail();

	// Dump WINDOW Styles
	void DumpExStyles(CWnd *ptr);
	void DumpStyles(CWnd *ptr);	
	void DumpSS(CWnd *ptr);
	void DumpEdS(CWnd *ptr);

	// Handle/Convert UNICODE strings
	DWORD   UnicodeToAnsi(LPWSTR pszW, LPSTR* ppszA);
	DWORD   AnsiToUnicode(LPCSTR pszA, LPWSTR* ppszW);

	// Just like hexdump, but prints to debug pad
	int     dump(const void *mem, int len);
	int     dumpmx(const char *mem, int len);

	// Just like print, but prints to debug pad
	int     printf(const char *Format, ...);

	// Async version
	int     aprintf(const char *Format, ...);

	// Convert numeric message number to readable message
	char    *num2msg(int msg);

	// Convert numeric error number to readable message
	char    *num2err(int msg);

	// Return TRUE if message is a non-client message
	int     is_nonclient(int msg);

	// Return TRUE if message is async junc (mouse move etc ...)
	int     is_junk(int msg);

protected:

	int     ToNotepad(char  *str);

	// Connectivity related 
	SOCKADDR_IN     dest_sin, local_sin;		
	SOCKET			sock, sock2;
	int				acc_sin_len;    /* Accept socket address length */
	SOCKADDR		acc_sin;		/* Accept socket address */
	
	int nd, ndlen, connected, resolved; 

	int			Resolve();
	int			Connect();
};

extern MxPad	mxpad;

// Defines for simplification:

#define P2N		mxpad.printf
#define D2N		mxpad.dump
#define C2N		mxpad.ClearNotepad

// Asyncronous notepad output:

#define AP2N	mxpad.aprintf

// Flag macros:

#define P2NLOCAL(flag)	mxpad.local=flag;
#define P2NREMOTE(flag)	mxpad.remote=flag;

#endif // mxpad_defined

// EOF

