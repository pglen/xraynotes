// HShield.cpp : implementation file
//

#include "stdafx.h"
#include "xraynotes.h"
#include "HShield.h"
#include "mxpad.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString CHShield::strhipaa("ShowHippa");
CString CHShield::strflag("ShowFlag");;

/////////////////////////////////////////////////////////////////////////////
// CHShield dialog

//int		CHShield::disable = 0;


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
	ON_WM_KEYDOWN()
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
	CString str;
	if( ((CButton*)GetDlgItem(IDC_CHECK_HIP_1))->GetCheck())
		{
		//P2N("Checked\r\n");
		str = "1";
		}
	else
		{
		//P2N("UnChecked\r\n");
		str = "0";
		}
	AfxGetApp()->WriteProfileString(strhipaa, strflag, str);
}

void CHShield::OnLButtonUp(UINT nFlags, CPoint point) 
{
	EndDialog(IDOK);
	CDialog::OnLButtonUp(nFlags, point);
}


int CHShield::DoModal() 

{
	int ret = 0;
	//CString str("showhippa"), str2("flag");;

	CString num = AfxGetApp()->GetProfileString(strhipaa, strflag, "");

	//P2N("Registry for HIPAA %s, disable flag=%d\r\n", num, disable);

	//&& CHShield::disable == false)

	if(atoi(num) == 0 )	
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
		//CHShield::disable = true;
		}
	else
		{
		//CHShield::disable = false;
		}
}

void CHShield::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	P2N("CHShield::OnKeyDown\r\n");	
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
	EndDialog(IDOK);
}

BOOL CHShield::PreTranslateMessage(MSG* pMsg) 

{
	//////////////////////////////////////////////////////////////////////
	// Dispatch messages ourselvs
	if(pMsg->message == WM_CHAR)
		{
		OnChar(pMsg->wParam, LOWORD(pMsg->lParam), pMsg->lParam); 
		return 0;
		}
	if(pMsg->message == WM_KEYDOWN)
		{
		OnKeyDown(pMsg->wParam, LOWORD(pMsg->lParam), pMsg->lParam); 
		return 0;
		}
	if(pMsg->message == WM_KEYUP)
		{
		OnKeyUp(pMsg->wParam, LOWORD(pMsg->lParam), pMsg->lParam); 
		return 0;
		}
	return CDialog::PreTranslateMessage(pMsg);
}
