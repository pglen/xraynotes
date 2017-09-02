
/* =====[ ToolShow.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the ToolShow.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  11/27/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// ToolShow.cpp : implementation file
//

#include "stdafx.h"
#include "xraynotes.h"
#include "ToolShow.h"


#include "MagDlg.h"
#include "gestureDlg.h"
#include "xScroll.h"
#include "yScroll.h"
#include "DicomWin.h"
#include "xyzDlg.h"

extern  CMagDlg		magdlg;
extern	CGestureDlg	gesdlg;
extern	CxScroll	xscroll;
extern	CyScroll	yscroll;
extern	CDicomWin	dicomwin;
extern	CxyzDlg		xyzdlg;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolShow dialog

CToolShow::CToolShow(CWnd* pParent /*=NULL*/)
	: CGrayDlg(CToolShow::IDD, pParent)
{
	//{{AFX_DATA_INIT(CToolShow)
	//}}AFX_DATA_INIT
}

void CToolShow::DoDataExchange(CDataExchange* pDX)

{
	CGrayDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CToolShow)
	DDX_Control(pDX, IDOK, m_ok);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CToolShow, CGrayDlg)
	//{{AFX_MSG_MAP(CToolShow)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_CHECK4, OnCheck4)
	ON_BN_CLICKED(IDC_CHECK6, OnCheck6)
	ON_BN_CLICKED(IDC_CHECK7, OnCheck7)
	ON_BN_CLICKED(IDC_CHECK5, OnCheck5)
	ON_BN_CLICKED(IDC_CHECK8, OnCheck8)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolShow message handlers

void CToolShow::OnCheck1() 
{
	if(magdlg.IsWindowVisible())
		magdlg.Hide();
	else
		{
		magdlg.Show();
		support.YieldToWinEx();
		SetFocus();
		}		
}

BOOL CToolShow::OnInitDialog() 

{
	CGrayDlg::OnInitDialog();

	//bAlpha = 220;

	//m_head.SetTrans();
	//m_head.SetFontSize(24);

	m_title = "Panel/Tool Control Window";

	// Prpagate current status onto checkboxes
	( (CButton*)GetDlgItem(IDC_CHECK1) )->SetCheck(magdlg.IsWindowVisible());
	( (CButton*)GetDlgItem(IDC_CHECK4) )->SetCheck(gesdlg.IsWindowVisible());

	( (CButton*)GetDlgItem(IDC_CHECK6) )->SetCheck(xscroll.IsWindowVisible());
	( (CButton*)GetDlgItem(IDC_CHECK7) )->SetCheck(yscroll.IsWindowVisible());

	// These windows may not exist
	if(IsWindow(dicomwin))
		( (CButton*)GetDlgItem(IDC_CHECK5) )->SetCheck(dicomwin.IsWindowVisible());

	if(IsWindow(xyzdlg))
	( (CButton*)GetDlgItem(IDC_CHECK8) )->SetCheck(xyzdlg.IsWindowVisible());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CToolShow::OnLButtonDown(UINT nFlags, CPoint point) 
{
	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));	

	CGrayDlg::OnLButtonDown(nFlags, point);
}

void CToolShow::OnCheck4() 
{
	if(gesdlg.IsWindowVisible())
		gesdlg.Hide();
	else
		{
		gesdlg.Show();
		support.YieldToWinEx();
		SetFocus();
		}		
}

void CToolShow::OnCheck6() 
{
	if(xscroll.IsWindowVisible())
		xscroll.Hide();
	else
		{
		xscroll.Show();
		support.YieldToWinEx();
		SetFocus();
		}		
}

void CToolShow::OnCheck7() 
{
	if(yscroll.IsWindowVisible())
		yscroll.Hide();
	else
		{
		yscroll.Show();
		support.YieldToWinEx();
		SetFocus();
		}		
}

void CToolShow::OnCheck5() 

{
	if(IsWindow(dicomwin) && dicomwin.IsWindowVisible())
		dicomwin.Hide();
	else
		{
		dicomwin.Show();
		support.YieldToWinEx();
		SetFocus();
		}	
}

void CToolShow::OnCheck8() 

{
	if(IsWindow(xyzdlg) && xyzdlg.IsWindowVisible())
		xyzdlg.Hide();
	else
		{
		xyzdlg.Show();
		support.YieldToWinEx();
		SetFocus();
		}		
}
