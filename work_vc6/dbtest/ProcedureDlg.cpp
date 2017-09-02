// ProcedureDlg.cpp : implementation file
//

#include "stdafx.h"
#include "dbtest.h"
#include "ProcedureDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProcedureDlg dialog


CProcedureDlg::CProcedureDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProcedureDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProcedureDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CProcedureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProcedureDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProcedureDlg, CDialog)
	//{{AFX_MSG_MAP(CProcedureDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProcedureDlg message handlers
