
/* =====[ ClipBoard.cpp ]========================================== 
                                                                             
   Description:     The ftext project, implementation of the ClipBoard.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  7/7/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// ClipBoard.cpp: implementation of the CClipBoard class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ftext.h"
#include "mxpad.h"
#include "ClipBoard.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CClipBoard clip;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CClipBoard::CClipBoard()
{

}

CClipBoard::~CClipBoard()
{

}

//////////////////////////////////////////////////////////////////////////

int	CClipBoard::PutText(const char *str)

{
	P2N("Copy to clip '%s'\r\n", str);

	if(*str == 0)
		return 0;

	if (::OpenClipboard(AfxGetApp()->m_pMainWnd->GetSafeHwnd()))
		{
		if(::EmptyClipboard())
			{
			int len = strlen(str);
			HANDLE  txthandle = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, len + 1);

			char *txtptr = (char *)GlobalLock(txthandle); ASSERT(txtptr);

			strncpy(txtptr, str, len);
			txtptr[len] = '\0';

			::SetClipboardData(CF_TEXT, txthandle);
			}
		CloseClipboard();
		}

	return 0;
}

//////////////////////////////////////////////////////////////////////////

int	CClipBoard::GetText(CString &str)

{
	//P2N("CClipBoard::GetText\r\n");

	if (OpenClipboard(AfxGetApp()->m_pMainWnd->GetSafeHwnd()))
		{	
		HANDLE htext = GetClipboardData(CF_TEXT);
		if (htext)
			{
			LPVOID ptr = GlobalLock(htext);	ASSERT(ptr);

			//P2N("Got clipboard text='%s'\r\n", ptr);

			str = (char *)ptr;
			GlobalUnlock(htext);
			}
		CloseClipboard();
		}
	return 0;
}
