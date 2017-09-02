
/* =====[ MxPad.h ]=========================================================

   Description:

            Interface for MX debug pad.

      REV     DATE     BY           DESCRIPTION
      ----  --------  ----------    --------------------------------------
      0.00  03/09/99  Peter Glen    Initial version.
	  0.00  11/08/05  Peter Glen    Network Version
	  0.00  xx/08/08  Peter Glen    Force addedd
	  0.00  10/06/08  Peter Glen    Ifdef for socket functions

   ======================================================================= */

#ifndef mxpad_defined
#define mxpad_defined

// User settable items:

//////////////////////////////////////////////////////////////////////////
// Define this if you want remote functionality
// If we include it in a DLL, we elected to not define wsock functions 
// for simpliocity

//define USESOCK

//////////////////////////////////////////////////////////////////////////

#ifdef USESOCK
#include "winsock2.h"
#endif

//
// Just like printf, but prints to debug pad
//

class MxPad

{

public:

	int		remote;
	int		local;

	MxPad();
	~MxPad();

	SetLocal(int flag);
	SetRemote(int flag);
	
	DWORD   UnicodeToAnsi(LPWSTR pszW, LPSTR* ppszA);
	DWORD   AnsiToUnicode(LPCSTR pszA, LPWSTR* ppszW);

	//
	// Just like hexdump, but prints to debug pad
	int     dump(const void *mem, int len);
	int     dumpmx(const char *mem, int len);

	//
	// Just like print, but prints to debug pad
	int     printf(const char *Format, ...);
	
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


#ifdef USESOCK

	SOCKADDR_IN     dest_sin;                 /* Local socket - internet style */
	SOCKET sock2;

	int nd, ndlen;

	int connected, resolved; 

	SOCKET		sock;
	SOCKADDR_IN local_sin;  /* Local socket - internet style */
	SOCKADDR    acc_sin;    /* Accept socket address - internet style */
	int			acc_sin_len;        /* Accept socket address length */

	int Resolve();
	Connect();

#endif //USESOCK


};

// Define the ext here

extern MxPad	mxpad;

/* -------- Defines for convenience: ---------------------------------- */

#define P2N   mxpad.printf
#define D2N   mxpad.dump

#define p2n   mxpad.printf


#define P2NLOCAL(flag)	mxpad.local=flag;
#define P2NREMOTE(flag)	mxpad.remote=flag;

#endif

