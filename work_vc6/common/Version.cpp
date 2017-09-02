// Version.cpp: implementation of the CVersion class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Version.h"

#include "../common/mxpad.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVersion::CVersion()
{
	dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(this);

	memset(&osvx, 0 , sizeof(OSVERSIONINFOEX));
	osvx.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	GetVersionEx((OSVERSIONINFO*)&osvx);
}

CVersion::~CVersion()
{

}

CVersion::Dump()

{

P2N(
    "DWORD dwOSVersionInfoSize;   %d\r\n"
    "DWORD dwMajorVersion;        %d\r\n"
    "DWORD dwMinorVersion;        %d\r\n"
    "DWORD dwBuildNumber;         %d\r\n"
    "DWORD dwPlatformId;          %d\r\n"
    "TCHAR szCSDVersion[ 128 ];   '%s'\r\n",

	    dwOSVersionInfoSize, 
	    dwMajorVersion, 
	    dwMinorVersion, 
	    dwBuildNumber, 
	    dwPlatformId, 
	    szCSDVersion); 	
}

CVersion::DumpEx()

{
   	P2N(
   	"DWORD dwOSVersionInfoSize;    %d\r\n"
    "DWORD dwMajorVersion;         %d\r\n"
    "DWORD dwMinorVersion;         %d\r\n"
    "DWORD dwBuildNumber;          %d\r\n"
    "DWORD dwPlatformId;           %d\r\n"
    "TCHAR szCSDVersion[ 128 ];    '%s'\r\n"
    "WORD wServicePackMajor;       %d\r\n"
    "WORD wServicePackMinor;       %d\r\n",
    //"WORD wReserved[2];            %s\r\n",

	osvx.dwOSVersionInfoSize,
	osvx.dwMajorVersion,
	osvx.dwMinorVersion,
	osvx.dwBuildNumber,
	osvx.dwPlatformId,
	osvx.szCSDVersion,

	osvx.wServicePackMajor,
	osvx.wServicePackMinor
	//osvx.wReserved
	);
}

#if 0

	OSVERSIONINFO osv; 
	memset(&osv, 0 , sizeof(OSVERSIONINFO));
	osv.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osv);

    
	P2N("==========================\r\n");

	OSVERSIONINFOEX osvx; 
	memset(&osvx, 0 , sizeof(OSVERSIONINFOEX));
	osvx.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	GetVersionEx((OSVERSIONINFO*)&osvx);

	   	P2N(
   	"DWORD dwOSVersionInfoSize;    %d\r\n"
    "DWORD dwMajorVersion;         %d\r\n"
    "DWORD dwMinorVersion;         %d\r\n"
    "DWORD dwBuildNumber;          %d\r\n"
    "DWORD dwPlatformId;           %d\r\n"
    "TCHAR szCSDVersion[ 128 ];    '%s'\r\n"
    "WORD wServicePackMajor;       %d\r\n"
    "WORD wServicePackMinor;       %d\r\n",
    //"WORD wReserved[2];            %s\r\n",

   	 osvx.dwOSVersionInfoSize,
     osvx.dwMajorVersion,
     osvx.dwMinorVersion,
     osvx.dwBuildNumber,
     osvx.dwPlatformId,
     osvx.szCSDVersion,

	 osvx.wServicePackMajor,
	 osvx.wServicePackMinor

    //osvx.wReserved
	);

#endif

//////////////////////////////////////////////////////////////////////////

#if 0

	NONCLIENTMETRICS ncm;
	ncm.cbSize = sizeof(NONCLIENTMETRICS);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, &ncm, 0);

	    P2N(
    "UINT    cbSize;			   %d\r\n"
    "int     iBorderWidth;         %d\r\n"
    "int     iScrollWidth;         %d\r\n"
    "int     iScrollHeight;        %d\r\n"
    "int     iCaptionWidth;        %d\r\n"
    "int     iCaptionHeight;       %d\r\n"
    "LOGFONT lfCaptionFont;        %d\r\n"
    "int     iSmCaptionWidth;      %d\r\n"
    "int     iSmCaptionHeight;     %d\r\n"
    "LOGFONT lfSmCaptionFont;      %d\r\n"
    "int     iMenuWidth;           %d\r\n"
    "int     iMenuHeight;          %d\r\n"
    "LOGFONT lfMenuFont;           %d\r\n"
    "LOGFONT lfStatusFont;         %d\r\n"
    "LOGFONT lfMessageFont;        %d\r\n",
    ncm.cbSize,
    ncm.iBorderWidth,
    ncm.iScrollWidth, 
    ncm.iScrollHeight, 
    ncm.iCaptionWidth, 
    ncm.iCaptionHeight,
    ncm.lfCaptionFont, 
    ncm.iSmCaptionWidth,
    ncm.iSmCaptionHeight,
    ncm.lfSmCaptionFont, 
    ncm.iMenuWidth, 
    ncm.iMenuHeight, 
    ncm.lfMenuFont, 
    ncm.lfStatusFont, 
    ncm.lfMessageFont); 
 
	//m_lf.lfHeight -= 2;

	//m_font.CreateFontIndirect(&ncm.lfMessageFont);
	//m_font.CreateFontIndirect(&ncm.lfStatusFont);
	//m_font.CreateFontIndirect(&ncm.lfMenuFont);

#endif
