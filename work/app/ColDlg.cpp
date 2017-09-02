
/* =====[ ColDlg.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the ColDlg.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  7/31/2007  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// ColDlg.cpp : implementation file
//

#include "stdafx.h"
#include <process.h>

#include "xraynotes.h"
#include "worker.h"
#include "ColDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColDlg dialog

CColDlg::CColDlg(CWnd* pParent /*=NULL*/)
	: CGrayDlg(CColDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CColDlg)
	//}}AFX_DATA_INIT

	in_apply = false;	
	windowname = "CColDlg"; //support.GetFileNameExt(__FILE__);
}


void CColDlg::DoDataExchange(CDataExchange* pDX)
{
	CGrayDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColDlg)
	DDX_Control(pDX, IDC_SLIDER3, m_slide3);
	DDX_Control(pDX, IDC_SLIDER2, m_slide2);
	DDX_Control(pDX, IDC_SLIDER1, m_slide1);
	DDX_Control(pDX, IDC_COLB, m_colb);
	DDX_Control(pDX, IDC_COLG, m_colg);
	DDX_Control(pDX, IDC_COLR, m_colr);

	DDX_Control(pDX, IDOK		, 	m_OK 	); 	
	DDX_Control(pDX, IDCANCEL	,  	m_Cancel);
	DDX_Control(pDX, IDC_BUTTON3,	m_Reset	);	
	DDX_Control(pDX, IDC_BUTTON4,	m_lt1	);	
	DDX_Control(pDX, IDC_BUTTON5,	m_gt1	);	
	DDX_Control(pDX, IDC_BUTTON2,	m_ltt1	);	
	DDX_Control(pDX, IDC_BUTTON1,	m_gtt1	);	
	DDX_Control(pDX, IDC_BUTTON6,	m_lt2	);	
	DDX_Control(pDX, IDC_BUTTON7,	m_gt2	);	
	DDX_Control(pDX, IDC_BUTTON8,	m_ltt2	);	
	DDX_Control(pDX, IDC_BUTTON9,	m_gtt2	);	
	DDX_Control(pDX, IDC_BUTTON10,  m_lt3	);	
	DDX_Control(pDX, IDC_BUTTON11,  m_gt3	);	
	DDX_Control(pDX, IDC_BUTTON12,  m_gtt3	);	
	DDX_Control(pDX, IDC_BUTTON13,  m_ltt3	);	

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CColDlg, CGrayDlg)
	//{{AFX_MSG_MAP(CColDlg)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER1, OnReleasedcaptureSlider1)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER2, OnReleasedcaptureSlider2)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER3, OnReleasedcaptureSlider3)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_WM_ERASEBKGND()
	ON_WM_TIMECHANGE()
	ON_WM_TIMER()
	ON_WM_VSCROLL()
	ON_WM_SETCURSOR()
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	ON_BN_CLICKED(IDC_BUTTON8, OnButton8)
	ON_BN_CLICKED(IDC_BUTTON9, OnButton9)
	ON_BN_CLICKED(IDC_BUTTON10, OnButton10)
	ON_BN_CLICKED(IDC_BUTTON12, OnButton12)
	ON_BN_CLICKED(IDC_BUTTON11, OnButton11)
	ON_BN_CLICKED(IDC_BUTTON13, OnButton13)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColDlg message handlers

void CColDlg::Show()

{
	CShadesDlg *shadesdlg = (CShadesDlg* )AfxGetApp()->m_pMainWnd;
	ASSERT(shadesdlg->magic == SHADE_MAGIC);

	if(!IsWindow(m_hWnd))
		Create(IDD);

	// If it is already visible, just show
	if(IsWindowVisible())
		{
		SetActiveWindow();
		return;
		}
	
		m_slide1.SetPos(0);
	m_slide2.SetPos(0);
	m_slide3.SetPos(0);

	if(shadesdlg->current >= 0)
		{
		CXrayDoc *doc = (CXrayDoc*)(shadesdlg->docarr[shadesdlg->current]); 
		ASSERT(doc->magic == DOC_MAGIC);
			
		if(doc)
			{
			P2N("CColDlg::Show Making copy of the original\r\n");
			ASSERT(doc->magic == DOC_MAGIC);
			doc->org->Copy(*doc->img);

			org_changed = doc->changed;
			}
		}

	CGrayDlg::Show();
}

BOOL CColDlg::OnInitDialog() 

{
	CGrayDlg::OnInitDialog();
	
	CShadesDlg*  dlg =	(CShadesDlg*)parent;	
	ASSERT(dlg->magic == SHADE_MAGIC);

	m_title = "Color Component Adjustment";
	
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);
	
	WINDOWPLACEMENT pwp;
	((CWnd *)parent)->GetWindowPlacement(&pwp);

	if(!posload)
		{
		// Flush Right
		want_x = pwp.rcNormalPosition.right - 
					(wp.rcNormalPosition.right - wp.rcNormalPosition.left) - 90;

		// Middle of vertical
		want_y = (pwp.rcNormalPosition.bottom - pwp.rcNormalPosition.top) / 2 - 
					(wp.rcNormalPosition.bottom - wp.rcNormalPosition.top) / 2 ;
		
		}
	else
		{
		want_x = pre_want_x;
		want_y = pre_want_y;
		}

	SetWindowPos(NULL, want_x, want_y, 0, 0,  SWP_NOZORDER | SWP_NOSIZE );  

	m_slide1.SetRange( -255, 255/**/);
	m_slide1.SetTicFreq(16);
	m_slide1.SetPageSize( 20);
	m_slide1.SetPos(0);

	m_slide2.SetRange( -255, 255);
	m_slide2.SetTicFreq(16);
	m_slide2.SetPageSize( 20);
	m_slide2.SetPos(0);

	m_slide3.SetRange( -255, 255);
	m_slide3.SetTicFreq(16);
	m_slide3.SetPageSize( 20);
	m_slide3.SetPos(0);

	m_colr.SetBkColor(RGB(255,0,0)); m_colr.SetText("  R  ");
	m_colg.SetBkColor(RGB(0,255,0)); m_colg.SetText("  G  ");
	m_colb.SetBkColor(RGB(0,0,255)); m_colb.SetText("  B  ");

	SetTimer(1, 100, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CColDlg::OnMouseMove(UINT nFlags, CPoint point) 

{		
	CGrayDlg::OnMouseMove(nFlags, point);
}

void CColDlg::OnLButtonDown(UINT nFlags, CPoint point) 

{
	//PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));	
	CGrayDlg::OnLButtonDown(nFlags, point);
}

void CColDlg::OnReleasedcaptureSlider1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//doc->render->Copy(*doc->img);
	P2N("R slider released\r\n", -m_slide1.GetPos());

	Apply();
	*pResult = 0;
}


void CColDlg::OnReleasedcaptureSlider2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	P2N("G slider released\r\n", -m_slide1.GetPos());
	Apply();
	*pResult = 0;
}

void CColDlg::OnReleasedcaptureSlider3(NMHDR* pNMHDR, LRESULT* pResult) 
{
	P2N("B slider released\r\n", -m_slide1.GetPos());
	Apply();
	*pResult = 0;
}

//////////////////////////////////////////////////////////////////////////

void CColDlg::Apply()

{
	if(in_apply)
		{
		P2N("CColDlg::Apply reenter\r\n");
		return;
		}

	//CWaitCursor wc;

	P2N("CColDlg::Apply\r\n");

	CShadesDlg*  dlg =	(CShadesDlg* )parent;
	ASSERT(dlg->magic == SHADE_MAGIC);

	if(dlg->current < 0)
		{
		ShowStatus("This function needs a loaded document ...");

		//P2N("CColDlg::Apply no doc\r\n");

		m_slide1.SetPos(0);
		m_slide2.SetPos(0);
		m_slide3.SetPos(0);

		return;
		}

	CXrayDoc *doc = (CXrayDoc*)dlg->docarr[dlg->current]; 
	ASSERT(doc->magic == DOC_MAGIC );

	//SetCursor(LoadCursor(NULL, IDC_WAIT));
	in_apply = true;

	//support.YieldToWinEx();

	// Is it magnified?
	if(doc->mag > 1)
		{	
		P2N("Mag selrender\r\n");
		
		//doc->img->Copy(*doc->org);
		//((CShadesDlg* )parent)->MarkToRender(true);

		//doc->img->ShiftRGB(-m_slide1.GetPos(), -m_slide2.GetPos(), -m_slide3.GetPos());
		//doc->NewRender();
		}
	else
		{
		P2N("NoMag selrender sel=%d\r\n", doc->render->SelectionIsValid());

		//doc->render->Copy(*doc->img);
		//((CShadesDlg* )parent)->MarkToRender();

		//doc->render->ShiftRGB(-m_slide1.GetPos(), -m_slide2.GetPos(), -m_slide3.GetPos());
		//doc->img->ShiftRGB(-m_slide1.GetPos(), -m_slide2.GetPos(), -m_slide3.GetPos());
		}
	
	if(!doc->thhandle)
		{
		doc->arg1		=  -m_slide1.GetPos(), 
		doc->arg2		=  -m_slide2.GetPos();
		doc->arg3		=  -m_slide3.GetPos();

		doc->command  = PROC_COLORIZE;
		doc->thhandle = _beginthread(RunWorkerThread, 0, doc);
		}

	//SelectionGetBox
	//dlg->Invalidate(false);

	//ShowCursor(true);

	in_apply = false;

	//SetCursor(LoadCursor(NULL, IDC_ARROW));

	P2N("CColDlg::Apply ended\r\n");
}

void CColDlg::OnButton3() 

{
	// Reset image
	CShadesDlg*  dlg =	(CShadesDlg* )parent;	
	ASSERT(dlg->magic == SHADE_MAGIC);

	if(dlg->current >= 0)
		{
		CXrayDoc *doc = (CXrayDoc*)dlg->docarr[dlg->current]; 
		ASSERT(doc->magic == DOC_MAGIC);
		
		if(org_changed != doc->changed)
			{
			ShowStatus("Restoring previous Image State");
			
			m_slide1.SetPos(0);
			m_slide2.SetPos(0);
			m_slide3.SetPos(0);

			doc->render->Copy(*doc->org);
			////doc->NewRender();

			dlg->Invalidate(false);		

			ShowStatus("");
			}
		}
}

void CColDlg::OnOK() 

{
	CShadesDlg *shadesdlg = (CShadesDlg* )AfxGetApp()->m_pMainWnd;
	ASSERT(shadesdlg->magic == SHADE_MAGIC);

	if(shadesdlg->current >= 0)
		{
		// Commit image change
		CXrayDoc *doc = (CXrayDoc*)shadesdlg->docarr[shadesdlg->current]; 
		
		if(org_changed != doc->changed)
			{
			ShowStatus("Committing Values ...");	
			doc->AddUndo();

			//doc->img->Light(-m_slide1.GetPos(), -m_slide2.GetPos());
			//((CShadesDlg* )parent)->MarkToRender(true);
			doc->img->ShiftRGB(-m_slide1.GetPos(), -m_slide2.GetPos(), -m_slide3.GetPos());
			doc->changed = org_changed + 1;

			((CShadesDlg* )parent)->Invalidate(false);
			}
		}
	CGrayDlg::OnOK();
}

BOOL CColDlg::PreTranslateMessage(MSG* pMsg) 

{
	return CGrayDlg::PreTranslateMessage(pMsg);
}

BOOL CColDlg::OnEraseBkgnd(CDC* pDC) 

{
	BOOL ret = CGrayDlg::OnEraseBkgnd(pDC);
	
	//m_slide1.DrawTransparent(true);
	//m_slide2.DrawTransparent(true);
	//m_slide3.DrawTransparent(true);

	return ret;
}

void CColDlg::OnTimeChange() 

{
	CGrayDlg::OnTimeChange();	
	// TODO: Add your message handler code here
}

void CColDlg::OnTimer(UINT nIDEvent) 

{
	//return;
	//SetFocus();	
	
	//m_slide3.SetFocus();
	//m_slide2.SetFocus();
	//m_slide1.SetFocus();

	if(nIDEvent == 1)
		{
		KillTimer(nIDEvent);

		CShadesDlg*  dlg =	(CShadesDlg* )parent;
		ASSERT(dlg->magic == SHADE_MAGIC);
		
		if(dlg->current >= 0)
			{
			CXrayDoc *doc = (CXrayDoc*)dlg->docarr[dlg->current]; 
			ASSERT(doc->magic == DOC_MAGIC);
	
			doc->org->Copy(*doc->img);
			}
		}
	CGrayDlg::OnTimer(nIDEvent);
}


void CColDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 

{
	P2N("CColDlg::OnVScroll\r\n");

	if(nSBCode == SB_ENDSCROLL || nSBCode == SB_THUMBTRACK) //	|| nSBCode == SB_THUMBPOSITION)
		{
		Apply();
		}

	CGrayDlg::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CColDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 

{
	CShadesDlg *shadesdlg = (CShadesDlg* )AfxGetApp()->m_pMainWnd;
	ASSERT(shadesdlg->magic == SHADE_MAGIC);

	if(shadesdlg->current >= 0)
		{
		CXrayDoc *doc = (CXrayDoc*)(shadesdlg->docarr[shadesdlg->current]); 
		if(doc)
			{
			ASSERT(doc->magic == DOC_MAGIC);
					
			//P2N("CColDlg::OnSetCursor\r\n");
			if(doc->thhandle)
				{
				SetCursor(LoadCursor(NULL, IDC_WAIT));
				return true;
				}
			}
		}

	SetCursor(LoadCursor(NULL, IDC_ARROW));
	return true;
	//return CGrayDlg::OnSetCursor(pWnd, nHitTest, message);
}

void CColDlg::OnCancel() 

{
	OnButton3();
	CGrayDlg::OnCancel();
}

// Just an alias to make it symmetrical

void CColDlg::UpdateParam()
{
	Apply();
}

void CColDlg::OnButton4() 
{
	m_slide1.SetPos(m_slide1.GetPos() + 5);
	UpdateParam();	
}


void CColDlg::OnButton5() 
{
	m_slide1.SetPos(m_slide1.GetPos() - 5);
	UpdateParam();		
}

void CColDlg::OnButton2() 
{
	m_slide1.SetPos(m_slide1.GetPos() + 15);
	UpdateParam();		
}

void CColDlg::OnButton1() 
{
	m_slide1.SetPos(m_slide1.GetPos() - 15);
	UpdateParam();			
}

void CColDlg::OnButton6() 
{
	m_slide2.SetPos(m_slide2.GetPos() + 5);
	UpdateParam();			
}

void CColDlg::OnButton7() 
{
	m_slide2.SetPos(m_slide2.GetPos() - 5);
	UpdateParam();				
}

void CColDlg::OnButton8() 
{
	m_slide2.SetPos(m_slide2.GetPos() + 15);
	UpdateParam();				
}

void CColDlg::OnButton9() 
{
	m_slide2.SetPos(m_slide2.GetPos() - 15);
	UpdateParam();			
}

void CColDlg::OnButton10() 
{
	m_slide3.SetPos(m_slide3.GetPos() + 5);
	UpdateParam();				
}

void CColDlg::OnButton12() 
{
	m_slide3.SetPos(m_slide3.GetPos() + 15);
	UpdateParam();					
}

void CColDlg::OnButton11() 
{
	m_slide3.SetPos(m_slide3.GetPos() - 5);
	UpdateParam();					
}

void CColDlg::OnButton13() 
{
	m_slide3.SetPos(m_slide3.GetPos() - 15);
	UpdateParam();					
}

void CColDlg::UpdateCursor()

{
	//P2N("CColDlg::UpdateCursor\r\n");
	
	if(IsWindow(m_hWnd))
		SendMessage(WM_SETCURSOR, HTCLIENT, 0);
}
