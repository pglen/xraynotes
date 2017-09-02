// ftext.h : main header file for the FTEXT application
//

#if !defined(AFX_FTEXT_H__7EB1CF1B_57BD_4969_8FF3_4F01B3A7DC67__INCLUDED_)
#define AFX_FTEXT_H__7EB1CF1B_57BD_4969_8FF3_4F01B3A7DC67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CFtextApp:
// See ftext.cpp for the implementation of this class
//

class CFtextApp : public CWinApp
{
public:
	CFtextApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFtextApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CFtextApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FTEXT_H__7EB1CF1B_57BD_4969_8FF3_4F01B3A7DC67__INCLUDED_)
