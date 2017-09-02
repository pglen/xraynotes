
/* =====[ CxPad.h ]=========================================================

   Description:

            Interface for MX debug pad.

      REV     DATE     BY           DESCRIPTION
      ----  --------  ----------    --------------------------------------
      0.00  03/09/99  Peter Glen    Initial version.
	  0.00  11/08/05  Peter Glen    Network Version
	  0.00  11/08/05  Peter Glen    Async printf

   ======================================================================= */

#ifndef CxPad_defined
#define CxPad_defined

#include "xCircular.h"

//
// Just like printf, but prints to debug pad
//

class CxPad

{

public:

	int		remote;
	int		local;
	int		handle;
	int		endthread;

	CxCircular circ;

	CxPad();
	~CxPad();

	void SetLocal(int flag);
	void SetRemote(int flag);
	
	DWORD   UnicodeToAnsi(LPWSTR pszW, LPSTR* ppszA);
	DWORD   AnsiToUnicode(LPCSTR pszA, LPWSTR* ppszW);

	//
	// Just like hexdump, but prints to debug pad
	int     dump(const void *mem, int len);
	int     dumpmx(const char *mem, int len);

	//
	// Just like print, but prints to debug pad
	int     printf(const char *Format, ...);
	int     aprintf(const char *Format, ...);

	//
	// Convert numeric message number to readable message
	char    *num2msg(int msg);

	//
	// Convert numeric error number to readable message
	char    *num2err(int msg);

	//
	// Return TRUE if message is non-client
	int     is_nonclient(int msg);

	//
	// Return TRUE if message is async junc (mouse move etc ...)
	int     is_junk(int msg);

protected:
	


	int     ToNotepad(char  *str);

	SOCKADDR_IN     dest_sin;                 /* Local socket - internet style */
	SOCKET sock2;

	int nd, ndlen;

	int connected;
	int resolved; 

	SOCKET		sock;
	SOCKADDR_IN local_sin;  /* Local socket - internet style */
	SOCKADDR    acc_sin;    /* Accept socket address - internet style */
	int			acc_sin_len;        /* Accept socket address length */

	int Resolve();
	int Connect();

	char    *szOutString;
	char    *aszOutString;
	char	*dump_str;

};

extern CxPad	cxpad;

/* -------- Defines: ----------------------------------------------------- */

#define P2N		cxpad.printf
#define D2N		cxpad.dump
#define AP2N	cxpad.aprintf

#define P2NLOCAL(flag)	cxpad.local=flag;
#define P2NREMOTE(flag)	cxpad.remote=flag;

#endif

