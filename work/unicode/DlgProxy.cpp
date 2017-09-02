// DlgProxy.cpp : implementation file
//

#include "stdafx.h"
#include "unicode.h"
#include "DlgProxy.h"
#include "unicodeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUnicodeDlgAutoProxy

IMPLEMENT_DYNCREATE(CUnicodeDlgAutoProxy, CCmdTarget)

CUnicodeDlgAutoProxy::CUnicodeDlgAutoProxy()
{
	EnableAutomation();
	
	// To keep the application running as long as an automation 
	//	object is active, the constructor calls AfxOleLockApp.
	AfxOleLockApp();

	// Get access to the dialog through the application's
	//  main window pointer.  Set the proxy's internal pointer
	//  to point to the dialog, and set the dialog's back pointer to
	//  this proxy.
	ASSERT (AfxGetApp()->m_pMainWnd != NULL);
	ASSERT_VALID (AfxGetApp()->m_pMainWnd);
	ASSERT_KINDOF(CUnicodeDlg, AfxGetApp()->m_pMainWnd);
	m_pDialog = (CUnicodeDlg*) AfxGetApp()->m_pMainWnd;
	m_pDialog->m_pAutoProxy = this;
}

CUnicodeDlgAutoProxy::~CUnicodeDlgAutoProxy()
{
	// To terminate the application when all objects created with
	// 	with automation, the destructor calls AfxOleUnlockApp.
	//  Among other things, this will destroy the main dialog
	if (m_pDialog != NULL)
		m_pDialog->m_pAutoProxy = NULL;
	AfxOleUnlockApp();
}

void CUnicodeDlgAutoProxy::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CUnicodeDlgAutoProxy, CCmdTarget)
	//{{AFX_MSG_MAP(CUnicodeDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CUnicodeDlgAutoProxy, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CUnicodeDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IUnicode to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {7411CBB8-79EA-4936-9A64-B825ECA1B9D8}
static const IID IID_IUnicode =
{ 0x7411cbb8, 0x79ea, 0x4936, { 0x9a, 0x64, 0xb8, 0x25, 0xec, 0xa1, 0xb9, 0xd8 } };

BEGIN_INTERFACE_MAP(CUnicodeDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CUnicodeDlgAutoProxy, IID_IUnicode, Dispatch)
END_INTERFACE_MAP()

// The IMPLEMENT_OLECREATE2 macro is defined in StdAfx.h of this project
// {5B279D39-2FFA-445D-A957-762AC7045E5A}
IMPLEMENT_OLECREATE2(CUnicodeDlgAutoProxy, "Unicode.Application", 0x5b279d39, 0x2ffa, 0x445d, 0xa9, 0x57, 0x76, 0x2a, 0xc7, 0x4, 0x5e, 0x5a)

/////////////////////////////////////////////////////////////////////////////
// CUnicodeDlgAutoProxy message handlers
