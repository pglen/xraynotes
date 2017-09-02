
/* =====[ multimon.h ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the multimon.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  4/6/2007  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

//
// if we are building on Win95/NT4 headers we need to declare this stuff ourselves
//
#ifndef SM_CMONITORS

#define SM_XVIRTUALSCREEN       76
#define SM_YVIRTUALSCREEN       77
#define SM_CXVIRTUALSCREEN      78
#define SM_CYVIRTUALSCREEN      79
#define SM_CMONITORS            80
#define SM_SAMEDISPLAYFORMAT    81

DECLARE_HANDLE(HMONITOR);

#define MONITOR_DEFAULTTONULL       0x00000000
#define MONITOR_DEFAULTTOPRIMARY    0x00000001
#define MONITOR_DEFAULTTONEAREST    0x00000002

#define MONITORINFOF_PRIMARY        0x00000001

#define MONITOR_DEFAULTTONULL       0x00000000
#define MONITOR_DEFAULTTOPRIMARY    0x00000001
#define MONITOR_DEFAULTTONEAREST    0x00000002

WINUSERAPI
HMONITOR
WINAPI
MonitorFromPoint(
    IN POINT pt,
    IN DWORD dwFlags);

WINUSERAPI
HMONITOR
WINAPI
MonitorFromRect(
    IN LPCRECT lprc,
    IN DWORD dwFlags);

WINUSERAPI
HMONITOR
WINAPI
MonitorFromWindow( IN HWND hwnd, IN DWORD dwFlags);

#define MONITORINFOF_PRIMARY        0x00000001


typedef struct tagMONITORINFO
{
    DWORD   cbSize;
    RECT    rcMonitor;
    RECT    rcWork;
    DWORD   dwFlags;
} MONITORINFO, *LPMONITORINFO;

#define CCHDEVICENAME 32

#ifdef __cplusplus
typedef struct tagMONITORINFOEX : public tagMONITORINFO
{
  TCHAR       szDevice[CCHDEVICENAME];
} MONITORINFOEX, *LPMONITORINFOEX;
#else
typedef struct
{
  MONITORINFO;
  TCHAR       szDevice[CCHDEVICENAME];
} MONITORINFOEX, *LPMONITORINFOEX;
#endif

typedef BOOL (CALLBACK* MONITORENUMPROC)(HMONITOR, HDC, LPRECT, LPARAM);

#endif  // SM_CMONITORS
