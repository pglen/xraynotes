// dbtest.h : main header file for the DBTEST application
//

#if !defined(AFX_DBTEST_H__F3ADFFE5_3C90_4EE0_8F93_E50F90707D6C__INCLUDED_)
#define AFX_DBTEST_H__F3ADFFE5_3C90_4EE0_8F93_E50F90707D6C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#define RED        RGB(127,  0,  0)
#define GREEN      RGB(  0,127,  0)
#define BLUE       RGB(  0,  0,127)
#define LIGHTRED   RGB(255,  0,  0)
#define LIGHTGREEN RGB(  0,255,  0)
#define LIGHTBLUE  RGB(  0,  0,255)
#define BLACK      RGB(  0,  0,  0)
#define WHITE      RGB(255,255,255)
#define GRAY       RGB(192,192,192)

#include "editex.h"

typedef struct _fields
		{
		CString name;
		CString value;
		CString value2;
		CString type;
		int		checked;
		CString	*var;
		CString	var2;
		CEditEx *ctrl;
		int		tipid;
		int		olddis;
		} fields;

typedef struct _flags
		{
		CString name;
		int		value;
		int		*var;
		CButton *ctrl;
		int		tipid;
		} flags;


extern 	CString droot;
extern 	CString wdroot;
extern 	CString approot;

//void	GetAppRoot(CString &approot);

#include "resource.h"		// main symbols
#include "msgbox.h"

/////////////////////////////////////////////////////////////////////////////
// CDbtestApp:
// See dbtest.cpp for the implementation of this class
//

class CDbtestApp : public CWinApp
{
public:
	CDbtestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDbtestApp)
	public:
	virtual BOOL InitInstance();
	virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDbtestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DBTEST_H__F3ADFFE5_3C90_4EE0_8F93_E50F90707D6C__INCLUDED_)
