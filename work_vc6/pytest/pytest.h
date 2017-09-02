// pytest.h : main header file for the PYTEST application
//

#if !defined(AFX_PYTEST_H__D866AE48_FE09_446A_B39A_973F015A27B2__INCLUDED_)
#define AFX_PYTEST_H__D866AE48_FE09_446A_B39A_973F015A27B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPytestApp:
// See pytest.cpp for the implementation of this class
//

class CPytestApp : public CWinApp
{
public:
	CPytestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPytestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPytestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PYTEST_H__D866AE48_FE09_446A_B39A_973F015A27B2__INCLUDED_)
