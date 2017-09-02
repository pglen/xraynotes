// xcal.h : main header file for the XCAL application
//

#if !defined(AFX_XCAL_H__F93112AC_4017_47D3_9109_9E00021922CE__INCLUDED_)
#define AFX_XCAL_H__F93112AC_4017_47D3_9109_9E00021922CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CXcalApp:
// See xcal.cpp for the implementation of this class
//

class CXcalApp : public CWinApp
{
public:
	CXcalApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXcalApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CXcalApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XCAL_H__F93112AC_4017_47D3_9109_9E00021922CE__INCLUDED_)
