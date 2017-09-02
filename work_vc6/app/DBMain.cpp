
/* =====[ DBMain.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the DBMain.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  10/28/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// DBMain.cpp : implementation file
//

#include "stdafx.h"
#include "xraynotes.h"

#include "HShield.h"	
#include "support.h"
#include "DBMain.h"
#include "AddrEntry.h"
#include "xcalDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString header = "Main DataBase Window";

/////////////////////////////////////////////////////////////////////////////
// CDBMain dialog

CDBMain::CDBMain(CWnd* pParent /*=NULL*/)
	: CGrayDlg(CDBMain::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDBMain)
	//}}AFX_DATA_INIT

	arr[0] = &m_bmp1;
	arr[1] = &m_bmp2;
	arr[2] = &m_bmp3;
	arr[3] = &m_bmp4;
	arr[4] = &m_bmp5;
	arr[5] = &m_bmp6;

	m_centerdlg = true;

	SetWindowText(header);

	//windowname = support.GetFileNameExt(__FILE__);

	windowname = "CDBMain";
}

void CDBMain::DoDataExchange(CDataExchange* pDX)
{
	CGrayDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDBMain)
	DDX_Control(pDX, IDC_PAT, m_pat);
	DDX_Control(pDX, IDC_BMP6, m_bmp6);
	DDX_Control(pDX, IDC_BMP4, m_bmp4);
	DDX_Control(pDX, IDC_BMP5, m_bmp5);
	DDX_Control(pDX, IDC_BMP3, m_bmp3);
	DDX_Control(pDX, IDC_BMP2, m_bmp2);
	DDX_Control(pDX, IDC_BMP1, m_bmp1);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDBMain, CGrayDlg)
	//{{AFX_MSG_MAP(CDBMain)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BMP1, OnBmp1)
	ON_BN_CLICKED(IDC_BMP2, OnBmp2)
	ON_BN_CLICKED(IDC_BMP4, OnBmp4)
	ON_BN_CLICKED(IDC_BMP5, OnBmp5)
	ON_BN_CLICKED(IDC_BMP6, OnBmp6)
	ON_BN_CLICKED(IDC_BMP3, OnBmp3)
	ON_WM_SYSKEYDOWN()
	ON_WM_SYSKEYUP()
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDBMain message handlers

void CDBMain::OnButton1() 

{
	//CAddrEntry addr;
	//addr.DoModal();		
}


void CDBMain::OnButton3() 

{
	//CDbtestDlg dlg;
	//dlg.DoModal();
}


BOOL CDBMain::OnInitDialog() 

{
	CGrayDlg::OnInitDialog();

	DecorateButtons();

	m_pat.SetText("No Patient Selected");
	m_pat.SetFontSize(24);	m_pat.SetTrans();
	UpdateCheck();

	return TRUE;  
}


BOOL CDBMain::PreTranslateMessage(MSG* pMsg) 

{
	switch(pMsg->message)
		{
		case WM_SYSKEYDOWN:
			OnSysKeyDown(pMsg->wParam, LOWORD(pMsg->lParam), pMsg->lParam); 
			return 0;
			break;

		case WM_SYSKEYUP:
			OnSysKeyUp(pMsg->wParam, LOWORD(pMsg->lParam), pMsg->lParam); 
			return 0;
			break;
		}
	return CGrayDlg::PreTranslateMessage(pMsg);
}


void CDBMain::OnBmp1() 

{
	//P2N("OnBmp1()\r\n");

	// The shield will know about actually popping up
	CHShield shield; 
	shield.DoModal();
 	
	// Get the address entry
	CAddrEntry addr;

	if(addr.DoModal() == IDOK)
		{
		//CString neh(header); neh += " - "; 
		//neh += addr.m_currp;
		//SetWindowText(neh);

		//m_curp = addr.m_currp; 
		m_pat.SetText(addr.m_currp);

		// Add global marker as well:
		// d,ksakjadslkjdsalkjddsa

		UpdateData(false);		
		}

	UpdateCheck();
}

//DEL void CDBMain::ShowAddrBook()
//DEL 
//DEL {
//DEL 	
//DEL }



//////////////////////////////////////////////////////////////////////////

void CDBMain::OnBmp2() 

{	
	P2N("OnBmp2()\r\n");	

	CXcalDlg	xc;
	xc.DoModal();
}

//DEL void CDBMain::OnPaint() 
//DEL 
//DEL {
//DEL 	CGrayDlg::OnPaint();
//DEL }

//DEL void CDBMain::OnLButtonDown(UINT nFlags, CPoint point) 
//DEL 
//DEL {
//DEL 	// Deactivated after moda/non model code was put in place ...
//DEL 
//DEL 	//drag_x = point.x; drag_y = point.y;
//DEL 	//RECT rc; GetClientRect(&rc);
//DEL 	//CRect rect(rc.right - 19, 1, rc.right - 6, 15);
//DEL 	//if(rect.PtInRect(point))
//DEL 	//	{
//DEL 	//	P2N("Click in close box\r\n");
//DEL 	//	//ShowWindow(SW_HIDE);
//DEL 	//	//EndDialog(IDOK);
//DEL 	//	}
//DEL 	//PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));	
//DEL 
//DEL 	CGrayDlg::OnLButtonDown(nFlags, point);
//DEL }


void CDBMain::OnBmp4() 
{
	P2N("OnBmp4()\r\n");		
}

void CDBMain::OnBmp5() 
{
	P2N("OnBmp5()\r\n");		
}

void CDBMain::OnBmp6() 
{
	P2N("OnBmp6()\r\n");		
}

void CDBMain::OnBmp3() 
{
	P2N("OnBmp3()\r\n");		
}

void CDBMain::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 

{
	for(int loop = 0; loop < sizeof(arr)/sizeof(void*); loop++)
		{
		if(arr[loop]->hotkey == (char)nChar)
			{
			//P2N("Hotkey for id=%d\r\n", arr[loop]->id);
			arr[loop]->SimulateDown();
			return;
			//break;
			}
		}	
	CGrayDlg::OnSysKeyDown(nChar, nRepCnt, nFlags);
}

void CDBMain::OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 

{
	for(int loop = 0; loop < sizeof(arr)/sizeof(void*); loop++)
		{
		if(arr[loop]->hotkey == (char)nChar)
			{
			//P2N("CDBMain::OnSysKeyUp Hotkey for id=%d\r\n", arr[loop]->id);
			arr[loop]->SimulatePush();
			return;
			//break;
			}
		}	
	CGrayDlg::OnSysKeyUp(nChar, nRepCnt, nFlags);
}

//////////////////////////////////////////////////////////////////////////

void CDBMain::OnCheck1() 

{
	CString flag; 

	int current  = ( (CButton*)GetDlgItem(IDC_CHECK1) )->GetCheck();
	
	flag.Format("%d", !current);
	
	AfxGetApp()->WriteProfileString(CHShield::strhipaa, CHShield::strflag, flag);
}

void CDBMain::UpdateCheck()

{
	CString flag  = AfxGetApp()->GetProfileString(CHShield::strhipaa, CHShield::strflag, "0");

	//P2N("Hippa shield flag %s\r\n", flag);

	( (CButton*)GetDlgItem(IDC_CHECK1) )->SetCheck(!atoi(flag));
}


void CDBMain::DecorateButtons()

{
	m_bmp1.bitmap = IDB_BITMAP26; m_bmp1.text = "&Patients";
	m_bmp1.id = 1; m_bmp1.tooltip = 
			"Create new patient records or Select/Manage existing patient data.\r\n"
					"HotKey for this button: Alt-P" ;

	m_bmp2.bitmap = IDB_BITMAP24; m_bmp2.text = "&Appointments";
	m_bmp2.id = 2; m_bmp2.tooltip = 
			"View/Manage appointments for patients. Must select patient record first.\r\n";
					"HotKey for this button: Alt-A" ;
	
	m_bmp3.bitmap = IDB_BITMAP27; m_bmp3.text = "Paymen&ts";
	m_bmp3.tooltip = "Manage payments. \r\n"
					"HotKey for this button:  Alt-T" ;
	
	m_bmp4.bitmap = IDB_BITMAP29; m_bmp4.text = "&Status";
	m_bmp4.tooltip = "Show status of various items. \r\n"
					"Hotkey for this button:  Alt-S" ;
	
	m_bmp5.bitmap = IDB_BITMAP28; m_bmp5.text = "&Reports";
	m_bmp5.tooltip = "Select reports to View/Print. \r\n"
					"Hotkey for this button:  Alt-R" ;
	//m_bmp5.hotkey = 'R';

	m_bmp6.bitmap = IDB_BITMAP30; m_bmp6.text = "Ass&ets";
	m_bmp6.tooltip = "Show, manage assets under supervision of the program.\r\n"
					"Hotkey for this button:  ALT-E" ;

}
