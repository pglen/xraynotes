// HShield.cpp : implementation file
//

#include "stdafx.h"
#include "dbtest.h"
#include "HShield.h"
#include "mxpad.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHShield dialog

int		CHShield::disable = 0;


CHShield::CHShield(CWnd* pParent /*=NULL*/)
	: CDialog(CHShield::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHShield)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CHShield::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHShield)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHShield, CDialog)
	//{{AFX_MSG_MAP(CHShield)
	ON_BN_CLICKED(IDC_HIP, OnHip)
	ON_BN_CLICKED(IDC_CHECK_HIP_1, OnCheckHip1)
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_CHECK_HIP_2, OnCheckHip2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHShield message handlers

void CHShield::OnHip() 
{
	EndDialog(IDOK);
}

void CHShield::OnCheckHip1() 

{	
	CString str("showhippa"), str2("flag");;

	if( ((CButton*)GetDlgItem(IDC_CHECK_HIP_1))->GetCheck())
		{
		//P2N("Checked\r\n");
		AfxGetApp()->WriteProfileString(str, str2, "1");
		}
	else
		{
		//P2N("UnChecked\r\n");
		AfxGetApp()->WriteProfileString(str, str2, "0");
		}
}

void CHShield::OnLButtonUp(UINT nFlags, CPoint point) 
{
	EndDialog(IDOK);
	CDialog::OnLButtonUp(nFlags, point);
}


int CHShield::DoModal() 

{
	int ret = 0;
	CString str("showhippa"), str2("flag");;

	CString num = AfxGetApp()->GetProfileString(str, str2, "");

	//P2N("Registry for HIPAA %s, disable flag=%d\r\n", num, disable);

	if(atoi(num) == 0 && CHShield::disable == false)
		ret = CDialog::DoModal();
	else
		ret = IDOK;
		
	return ret;
}


void CHShield::OnCheckHip2() 

{
	//P2N("Check hip2 HIPAA\r\n");

	if( ((CButton*)GetDlgItem(IDC_CHECK_HIP_2))->GetCheck())
		{
		CHShield::disable = true;
		}
	else
		{
		CHShield::disable = false;
		}
}
