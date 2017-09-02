// serials.h : main header file for the SERIALS application
//

#if !defined(AFX_SERIALS_H__92AF08CF_83C9_40F5_9BAB_130295E2AA31__INCLUDED_)
#define AFX_SERIALS_H__92AF08CF_83C9_40F5_9BAB_130295E2AA31__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSerialsApp:
// See serials.cpp for the implementation of this class
//

class CSerialsApp : public CWinApp
{
public:
	CSerialsApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSerialsApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSerialsApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERIALS_H__92AF08CF_83C9_40F5_9BAB_130295E2AA31__INCLUDED_)
