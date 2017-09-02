// DCMConverter.h : main header file for the DCMCONVERTER application
//

#if !defined(AFX_DCMCONVERTER_H__DC8B33F8_6EF7_4AF5_8C17_B0FBF43652F9__INCLUDED_)
#define AFX_DCMCONVERTER_H__DC8B33F8_6EF7_4AF5_8C17_B0FBF43652F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDCMConverterApp:
// See DCMConverter.cpp for the implementation of this class
//

class CDCMConverterApp : public CWinApp
{
public:
	CDCMConverterApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDCMConverterApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDCMConverterApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DCMCONVERTER_H__DC8B33F8_6EF7_4AF5_8C17_B0FBF43652F9__INCLUDED_)
