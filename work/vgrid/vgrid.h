// vgrid.h : main header file for the VGRID application
//

#if !defined(AFX_VGRID_H__A9B3DEC8_1963_4674_B6C1_81BDA27F4E01__INCLUDED_)
#define AFX_VGRID_H__A9B3DEC8_1963_4674_B6C1_81BDA27F4E01__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#define WS_EX_LAYERED 0x00080000 
#define LWA_COLORKEY 1 // Use color as the transparency color.
#define LWA_ALPHA    2 // Use bAlpha to determine the opacity of the layer

//  Function pointer for lyering API in User32.dll
typedef BOOL (WINAPI *lpfnSetLayeredWindowAttributes)
            (HWND hWnd, COLORREF cr, BYTE bAlpha, DWORD dwFlags);

extern	lpfnSetLayeredWindowAttributes g_pSetLayeredWindowAttributes ;


#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CVgridApp:
// See vgrid.cpp for the implementation of this class
//

class CVgridApp : public CWinApp
{
public:
	CVgridApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVgridApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CVgridApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VGRID_H__A9B3DEC8_1963_4674_B6C1_81BDA27F4E01__INCLUDED_)
