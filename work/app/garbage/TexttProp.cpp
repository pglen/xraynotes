// TexttProp.cpp : implementation file
//

#include "stdafx.h"
#include "xraynotes.h"
#include "TexttProp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextProp dialog


CTextProp::CTextProp(CWnd* pParent /*=NULL*/)
	: CDialog(CTextProp::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTextProp)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTextProp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTextProp)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTextProp, CDialog)
	//{{AFX_MSG_MAP(CTextProp)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextProp message handlers
