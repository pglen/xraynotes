// MedReg.cpp : implementation file
//

#include "stdafx.h"
#include "xraynotes.h"
#include "MedReg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMedReg dialog


CMedReg::CMedReg(CWnd* pParent /*=NULL*/)
	: CDialog(CMedReg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMedReg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMedReg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMedReg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMedReg, CDialog)
	//{{AFX_MSG_MAP(CMedReg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMedReg message handlers
