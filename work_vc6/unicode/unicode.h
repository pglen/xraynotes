// unicode.h : main header file for the UNICODE application
//

#if !defined(AFX_UNICODE_H__78D1C713_B7FC_42CC_A261_1434810453EF__INCLUDED_)
#define AFX_UNICODE_H__78D1C713_B7FC_42CC_A261_1434810453EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CUnicodeApp:
// See unicode.cpp for the implementation of this class
//

class CUnicodeApp : public CWinApp
{
public:
	CUnicodeApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUnicodeApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CUnicodeApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UNICODE_H__78D1C713_B7FC_42CC_A261_1434810453EF__INCLUDED_)
