
/* =====[ xyzDlg.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the xyzDlg.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  11/25/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// xyzDlg.cpp : implementation file
//

#include "stdafx.h"
#include "xraynotes.h"
#include "xyzDlg.h"

#include "do3dp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CxyzDlg dialog


CxyzDlg::CxyzDlg(CWnd* pParent /*=NULL*/)
	: CGrayDlg(CxyzDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CxyzDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	windowname = "CxyzDlg";
}

void CxyzDlg::DoDataExchange(CDataExchange* pDX)
{
	CGrayDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CxyzDlg)
	DDX_Control(pDX, IDC_2D, m_2d);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CxyzDlg, CGrayDlg)
	//{{AFX_MSG_MAP(CxyzDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CxyzDlg message handlers

BOOL CxyzDlg::PreTranslateMessage(MSG* pMsg) 

{
	//P2N("CxyzDlg::PreTranslateMessage\r\n"); 

	if(pMsg->message == CTRL2D_MOVE)
		{
		Update3D(LOWORD(pMsg->lParam), HIWORD(pMsg->lParam));

		//P2N("CxyzDlg::PreTranslateMessage CTRL2D_MOVE x=%d y=%d\r\n", 
		//						LOWORD(pMsg->lParam), HIWORD(pMsg->lParam));	
		}
	if(pMsg->message == CTRL2D_START)
		{
		//P2N("CxyzDlg::PreTranslateMessage CTRL2D_START x=%d y=%d\r\n", 
		//						LOWORD(pMsg->lParam), HIWORD(pMsg->lParam)); 
		}

	if(pMsg->message == CTRL2D_END)
		{
		Update3D(LOWORD(pMsg->lParam), HIWORD(pMsg->lParam));

		//P2N("CxyzDlg::PreTranslateMessage CTRL2D_END x=%d y=%d\r\n", 
		//						LOWORD(pMsg->lParam), HIWORD(pMsg->lParam)); 
		}


	// Redirect all keystrokes except a limited number of ctrl
	if(pMsg->message == WM_KEYDOWN)
		{
		switch(pMsg->wParam)
			{
			case  VK_TAB:
			case  VK_ESCAPE:
			case  VK_RETURN:
			//P2N("CxyzDlg::PreTranslateMessage TAB RET\r\n"); 
			goto endd;		
			}
		//OnChar(pMsg->wParam, LOWORD(pMsg->lParam), pMsg->lParam); 
		return 0;
		}

	if(pMsg->message == WM_KEYUP)
		{
		}

	if(pMsg->message == WM_CHAR)
		{
		//OnChar(pMsg->wParam, LOWORD(pMsg->lParam), pMsg->lParam); 
		//return 0;
		}

endd:
	return CGrayDlg::PreTranslateMessage(pMsg);
}

BOOL CxyzDlg::OnInitDialog() 
{
	CGrayDlg::OnInitDialog();
	
	m_title = "Dicom 3D point of view";

	//m_2d.SetPos(20,20);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

static int in_3d = false;
static int oldx = 0, oldy = 0;

void CxyzDlg::Update3D(int xx, int yy)

{
	CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);

	CXrayDoc *doc = ((CShadesDlg* )AfxGetApp()->m_pMainWnd)->GetCurrDoc();
	if(!doc) return;

	// Make sure it is the correct kind:
	if(!doc->backlink) return;
	if(!doc->backlink->IsDicomFile()) return;

	//P2N("CxyzDlg::Update3D %d %d\r\n", xx, yy);

	if(in_3d)
		{
		support.YieldToWinEx();
		return;
		}

	if(xx == oldx && yy == oldy)
		return;

	in_3d = true;
	Do3Dp(false, doc->backlink, doc, xx, 100 - yy);
	in_3d = false;

	oldx = xx;	oldy = yy;
}
