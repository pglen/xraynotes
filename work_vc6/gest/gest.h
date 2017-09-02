// gest.h : main header file for the GEST application
//

#if !defined(AFX_GEST_H__28D3D0FF_FE0A_4A0C_B9D8_77F0C182D471__INCLUDED_)
#define AFX_GEST_H__28D3D0FF_FE0A_4A0C_B9D8_77F0C182D471__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

extern CString	appdirname;


/////////////////////////////////////////////////////////////////////////////
// CGestApp:
// See gest.cpp for the implementation of this class
//

class CGestApp : public CWinApp
{
public:
	CGestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CGestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GEST_H__28D3D0FF_FE0A_4A0C_B9D8_77F0C182D471__INCLUDED_)
