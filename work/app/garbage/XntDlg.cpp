// XntDlg.cpp : implementation file
//

#include "stdafx.h"
#include "xraynotes.h"
#include "XntDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXntDlg dialog

CXntDlg::CXntDlg(CWnd* pParent /*=NULL*/)
	: CGrayDlg(CXntDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CXntDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	InitVars();
}

CXntDlg::CXntDlg(UINT nIDTemplate, CWnd* pParentWnd)
	: CGrayDlg(nIDTemplate, pParentWnd)

{
	xidd	= 	nIDTemplate;
	InitVars();
}

void CXntDlg::DoDataExchange(CDataExchange* pDX)
{
	CGrayDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXntDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CXntDlg, CGrayDlg)
	//{{AFX_MSG_MAP(CXntDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXntDlg message handlers

void CXntDlg::Show(int center)

{
	P2N("CXntDlg::Show %p '%s' \r\n", m_title);

	//xCreate();

	CGrayDlg::Show(center);
}

void CXntDlg::Hide() 

{
	P2N("CXntDlg::Hide %s\r\n", m_title);

	if(!::IsWindow(m_hWnd))
		return;

	CGrayDlg::Hide();
}

void CXntDlg::InitVars()
{
	//created = 0;
}
