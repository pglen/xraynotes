// uniq.h : main header file for the UNIQ application
//

#if !defined(AFX_UNIQ_H__4FE6DA53_DAE7_4B53_AEBD_CFC438463D68__INCLUDED_)
#define AFX_UNIQ_H__4FE6DA53_DAE7_4B53_AEBD_CFC438463D68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CUniqApp:
// See uniq.cpp for the implementation of this class
//

class CUniqApp : public CWinApp
{
public:
	CUniqApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUniqApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CUniqApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UNIQ_H__4FE6DA53_DAE7_4B53_AEBD_CFC438463D68__INCLUDED_)
