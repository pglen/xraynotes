
/* =====[ mxpad.h ]=========================================================

   Description:

            Interface for Marx debug pad.

      REV     DATE     BY           DESCRIPTION
      ----  --------  ----------    --------------------------------------
      0.00  03/09/99  Peter Glen    Initial version.

   ======================================================================= */

#ifndef mxpad_defined
#define mxpad_defined

#ifdef  EXPORT
#define EXTERN
#else
#define EXTERN extern
#endif

/* -------- Defines: ----------------------------------------------------- */

#define P2N   PrintToNotepad
#define T2N   ToNotepad
#define D2N   DumpToNotepad

/* -------- Macros: ------------------------------------------------------ */


/* -------- Protos ------------------------------------------------------- */


/* -------- Declarations: ------------------------------------------------ */


/* -------- Definitions: ------------------------------------------------- */

#undef  EXTERN
#undef  EXPORT

//
// Just like printf, but prints to debug pad
//

int     PrintToNotepadW(const char *Format, ...);
int     PrintToNotepad(const char *Format, ...);


//
// Just like hexdump, but prints to debug pad
//

int     DumpToNotepad(const char *mem, int len);

//
// Just like print, but prints to debug pad
//

int     ToNotepadW(LPWSTR   *str);
int     ToNotepad(char  *str);

//
// convert numeric message number to readable message
//

char    *num2msg(int msg);

//
// convert numeric error number to readable message
//

char    *num2err(int msg);


//
// Return TRUE if message is non-client
//

int     is_nonclient(int msg);

//
// Return TRUE if message is async junc (mouse move etc ...)
//

int     is_junk(int msg);

#endif

/* EOF */