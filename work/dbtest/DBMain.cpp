
/* =====[ DBMain.cpp ]========================================== 
                                                                             
   Description:     The dbtest project, implementation of the DBMain.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  5/14/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// DBMain.cpp : implementation file
//

#include "stdafx.h"
#include "dbtest.h"
#include "mxpad.h"
#include "dbtestDlg.h"
#include "support.h"
#include "DBMain.h"
#include "AddrEntry.h"
#include "BmpButt.h"
#include "HShield.h"	
#include "Transcript.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//define PATIENT_BUTT	1
//define APPOINT_BUTT	2
//define STATUS_BUTT	3
//define REPORTS_BUTT	4
//define ASSETS_BUTT	5
//define PAYMENTS_BUTT	6

/////////////////////////////////////////////////////////////////////////////
// CDBMain dialog


CDBMain::CDBMain(CWnd* pParent /*=NULL*/)
	: CDialog(CDBMain::IDD, pParent)
{

	arr[0] = &m_bmp1;
	arr[1] = &m_bmp2;
	arr[2] = &m_bmp3;
	arr[3] = &m_bmp4;
	arr[4] = &m_bmp5;
	arr[5] = &m_bmp6;
	
	//{{AFX_DATA_INIT(CDBMain)
	m_curp = _T("");
	//}}AFX_DATA_INIT
}


void CDBMain::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDBMain)
	DDX_Control(pDX, IDC_BMP6, m_bmp6);
	DDX_Control(pDX, IDC_BMP4, m_bmp4);
	DDX_Control(pDX, IDC_BMP5, m_bmp5);
	DDX_Control(pDX, IDC_BMP3, m_bmp3);
	DDX_Control(pDX, IDC_BMP2, m_bmp2);
	DDX_Control(pDX, IDC_BMP1, m_bmp1);
	DDX_Text(pDX, IDC_MA_EDIT3, m_curp);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDBMain, CDialog)
	//{{AFX_MSG_MAP(CDBMain)
	ON_BN_CLICKED(IDC_MA_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_MA_BUTTON3, OnButton3)
	ON_WM_SYSKEYDOWN()
	ON_WM_SYSKEYUP()
	ON_BN_CLICKED(IDC_BMP1, OnBmp1)
	ON_BN_CLICKED(IDC_BMP2, OnBmp2)
	ON_BN_CLICKED(IDC_BMP3, OnBmp3)
	ON_BN_CLICKED(IDC_BMP4, OnBmp4)
	ON_BN_CLICKED(IDC_BMP5, OnBmp5)
	ON_BN_CLICKED(IDC_BMP6, OnBmp6)
	ON_WM_SYSCHAR()
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDBMain message handlers

void CDBMain::OnButton1() 

{
	CAddrEntry addr;
	addr.DoModal();		
}


void CDBMain::OnButton3() 

{
	CDbtestDlg dlg;
	dlg.DoModal();
}


BOOL CDBMain::OnInitDialog() 

{
	CDialog::OnInitDialog();
	
	BYTE	bAlpha = 255;

	if (bAlpha < 255)
		{
        //  Set layered style for the dialog
        SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);

        //  Call it with 255 as alpha - opacity
		if(m_pSetLayeredWindowAttributes)
			m_pSetLayeredWindowAttributes(m_hWnd, 0, bAlpha, LWA_ALPHA);
		}

	m_bmp1.bitmap = IDB_PATIENTS; m_bmp1.text = "&Patients";
	//m_bmp1.id = PATIENT_BUTT; 
	m_bmp1.tooltip = "Create new Patient records"
							" or Select/Manage existing patient records. "
			"HotKey for this button:  Alt-P" ;
	m_bmp1.hotkey = 'P';

	// -------------------------------------------------------------------
	m_bmp2.bitmap = IDB_APPOINT; m_bmp2.text = "&Appointments";
	//m_bmp2.id = APPOINT_BUTT; 
	m_bmp2.tooltip = "View/Manage appointnments for Patients. "
			"HotKey for this button: Alt-A" ;
	m_bmp2.hotkey = 'A';

	// -------------------------------------------------------------------
	m_bmp3.bitmap = IDB_PAYMENTS; m_bmp3.text = "Paymen&ts";
	//m_bmp3.id = PAYMENTS_BUTT; 
	m_bmp3.tooltip = "Payments Button. Manage payments."
			"HotKey for this button:  Alt-T" ;
	m_bmp3.hotkey = 'T';

	// -------------------------------------------------------------------
	m_bmp4.bitmap = IDB_STATUS; m_bmp4.text = "&Status";
	//m_bmp4.id = STATUS_BUTT; 
	m_bmp4.tooltip = "Status button. Show status of various items. "
			"Hotkey for this button:  Alt-S" ;
	m_bmp4.hotkey = 'S';

	// -------------------------------------------------------------------
	m_bmp5.bitmap = IDB_REPORTS; m_bmp5.text =  "&Reports";
	//m_bmp5.id = REPORTS_BUTT ; 
	m_bmp5.tooltip = "Reports button. Select reports to view/Print. "
			"Hotkey for this button:  Alt-R" ;
	m_bmp5.hotkey = 'R';

	// -------------------------------------------------------------------
	m_bmp6.bitmap = IDB_ASSETS; m_bmp6.text = "Ass&ets";
	//m_bmp6.id = ASSETS_BUTT; 
	m_bmp6.tooltip = "Assets Button. Show, manage assets under supervision of the program."
			"Hotkey for this button:  ALT-E" ;
	m_bmp6.hotkey = 'E';
		
	return TRUE;  
}

BOOL CDBMain::PreTranslateMessage(MSG* pMsg) 

{
	//P2N("CDBMain::PreTranslateMessage %d %s\r\n", pMsg->message, mxpad.num2msg(pMsg->message));		

	switch(pMsg->message)
		{
		//case BN_CLICKED:
//
//			P2N("CDBMain::PreTranslateMessage button %d clicked\r\n", pMsg->wParam);
//
//			switch(pMsg->wParam)
//				{
//				case PATIENT_BUTT:
//					{
//					//AddrBook();
//					return(true);
//					}
//					break;
//				}	
//			break;

		case WM_SYSKEYDOWN:
			OnSysKeyDown(pMsg->wParam, LOWORD(pMsg->lParam), pMsg->lParam); 
			return 0;
			break;

		case WM_SYSKEYUP:
			OnSysKeyUp(pMsg->wParam, LOWORD(pMsg->lParam), pMsg->lParam); 
			return 0;
			break;
		}
	
	return CDialog::PreTranslateMessage(pMsg);
}



void CDBMain::ShowAddrBook()

{
 	CHShield shield; shield.DoModal();
 	
	support.YieldToWinEx();

	CAddrEntry addr;
 	if(addr.DoModal() == IDOK)
 		{
		m_curp = addr.m_currp; 
		UpdateData(false);
 		}
}

//////////////////////////////////////////////////////////////////////////

void CDBMain::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 

{
	//P2N("CDBMain::OnSysKeyDown(nChar=%d %c\r\n", nChar, nChar);

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
	CDialog::OnSysKeyDown(nChar, nRepCnt, nFlags);
}

void CDBMain::OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 

{
	P2N("CDBMain::OnSysKeyUp(nChar=%d %c\r\n", nChar, nChar);

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
	CDialog::OnSysKeyUp(nChar, nRepCnt, nFlags);
}

//////////////////////////////////////////////////////////////////////////

void CDBMain::OnBmp1() 
{
	//P2N("CDBMain::OnBmp1\r\n");
 	ShowAddrBook();
}


#include "xcalDlg.h"


void CDBMain::OnBmp2() 
{
#if 0
	//P2N("CDBMain::OnBmp2\r\n");

	CTranscript ct;

	UpdateData(false);

	CString str = 
	"DIAGNOSTIC DATA: \r\n"
"The patient’s admission laboratory data was notable for his initial blood gas, which showed a pH of 7.02 with a pCO2 of \r\n"
"118 and a pO2 of 103. The patient’s electrocardiogram showed nonspecific ST-T wave changes. The patent’s CBC showed a \r\n"
"white count of 24,000, with 56% neutrophils and 3% bands.\r\n";

	ct.ref = &str;
	//ct.ref = &m_note;

	if(ct.DoModal() != IDYES)
		return;

	UpdateData(false);

#endif

	CXcalDlg	xc;
	
	//xc.

	xc.DoModal();

}

void CDBMain::OnBmp3() 

{
	//P2N("CDBMain::OnBmp3\r\n");
}

void CDBMain::OnBmp4() 

{
	//P2N("CDBMain::OnBmp4\r\n");	
}

void CDBMain::OnBmp5() 

{
	//P2N("CDBMain::OnBmp5\r\n");	
}

void CDBMain::OnBmp6() 

{
	//P2N("CDBMain::OnBmp6\r\n");	
}

void CDBMain::OnSysChar(UINT nChar, UINT nRepCnt, UINT nFlags) 

{
	//P2N("CDBMain::OnSysChar(nChar=%d %c\r\n", nChar, nChar);
	CDialog::OnSysChar(nChar, nRepCnt, nFlags);
}

void CDBMain::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 

{
	//P2N("CDBMain::OnChar(nChar=%d %c\r\n", nChar, nChar);
	CDialog::OnChar(nChar, nRepCnt, nFlags);
}
