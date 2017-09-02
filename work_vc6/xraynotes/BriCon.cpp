/////////////////////////////////////////////////////////////////////////////
// CBriCon.cpp : implementation file
//

#include "stdafx.h"
#include "xraynotes.h"
#include "BriCon.h"
#include "worker.h"

#include <process.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBriCon dialog

CBriCon::CBriCon(CWnd* pParent /*=NULL*/)
	: CGrayDlg(CBriCon::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBriCon)
	//}}AFX_DATA_INIT

	old_pos1 = old_pos2 = 0;
	m_busy = LoadCursor(NULL, IDC_WAIT);
	
	org_changed = 0;

	windowname = "CBriCon"; //support.GetFileNameExt(__FILE__);
}

///////////////////////////////////////////////////////

CBriCon::~CBriCon()

{
}

///////////////////////////////////////////////////////

void CBriCon::DoDataExchange(CDataExchange* pDX)
{
	CGrayDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBriCon)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Control(pDX, IDC_BUTTON1, m_b1);
	DDX_Control(pDX, IDC_BUTTON5, m_b5);
	DDX_Control(pDX, IDC_BUTTON4, m_b4);
	DDX_Control(pDX, IDC_BUTTON3, m_b3);
	DDX_Control(pDX, IDC_BUTTON2, m_b2);
	
	DDX_Control(pDX, IDC_BUTTON6, m_b6);
	DDX_Control(pDX, IDC_BUTTON7, m_b7);
	DDX_Control(pDX, IDC_BUTTON8, m_b8);
	DDX_Control(pDX, IDC_BUTTON9, m_b9);

	DDX_Control(pDX, IDC_SLIDER1, m_slide1);
	DDX_Control(pDX, IDC_SLIDER2, m_slide2);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBriCon, CGrayDlg)
	//{{AFX_MSG_MAP(CBriCon)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER1, OnReleasedcaptureSlider1)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_WM_ERASEBKGND()
	ON_WM_HSCROLL()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER2, OnReleasedcaptureSlider2)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON8, OnButton8)
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	ON_BN_CLICKED(IDC_BUTTON9, OnButton9)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBriCon message handlers

BOOL CBriCon::OnInitDialog() 

{
	CGrayDlg::OnInitDialog();

	CShadesDlg*  dlg =	(CShadesDlg* )parent;	
	ASSERT(dlg->magic == SHADE_MAGIC);

	m_title = "Brightness and Contrast";
	bri_level   = 0;
	old_pos = 0;

	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);
	
	WINDOWPLACEMENT pwp;
	((CWnd *)parent)->GetWindowPlacement(&pwp);

	if(!posload)
		{
		// Flush Right
		want_x = pwp.rcNormalPosition.right - 
					(wp.rcNormalPosition.right - wp.rcNormalPosition.left) - 70;

		// Middle of vertical
		want_y = (pwp.rcNormalPosition.bottom - pwp.rcNormalPosition.top) / 2 - 
					(wp.rcNormalPosition.bottom - wp.rcNormalPosition.top) / 2;		
		}
	else
		{
		want_x = pre_want_x;
		want_y = pre_want_y;
		}

	SetWindowPos(NULL, want_x, want_y, 0, 0,  SWP_NOZORDER | SWP_NOSIZE );  

	m_slide1.SetRange( -200, 200);
	m_slide1.SetTicFreq(20);
	m_slide1.SetPageSize( 10);
	m_slide1.SetPos(0);

	m_slide2.SetRange( -100, 100);
	m_slide2.SetTicFreq(10);
	m_slide2.SetPageSize( 5);
	m_slide2.SetPos(0);

	old_pos1 = 0;
	old_pos2 = 0;

	//m_slide2.SetFocus();
	//SetTimer(1, 100, NULL);

	return TRUE; 
}

/////////////////////////////////////////////////////////////////////

void CBriCon::OnReleasedcaptureSlider1(NMHDR* pNMHDR, LRESULT* pResult) 

{	
	int pos =  m_slide1.GetPos(); // - old_pos;
	
	P2N("OnReleasedcaptureSlider1  %d\r\n", pos);

	if(old_pos1 != pos)
		{
		UpdateParam();
		}

	old_pos1 = pos;

	*pResult = 0;
}

///////////////////////////////////////////////////////////

int		CBriCon::UpdateParam()

{
	//static int in_update = 0;

	if(((CShadesDlg* )parent)->current < 0)
		return 0;
	
	CXrayDoc *doc = (CXrayDoc*)((CShadesDlg* )parent)->docarr[((CShadesDlg* )parent)->current]; 

	if(!doc)
		return 0;

	ASSERT(doc->magic == DOC_MAGIC);

	//ShowStatus("Adjusting ...");
	//HCURSOR oldc = SetCursor(m_busy);
	//support.YieldToWinEx();

	//doc->render->Copy(*doc->img);
	//doc->render->Light(-m_slide1.GetPos(), -m_slide2.GetPos());
	//((CShadesDlg* )parent)->Invalidate(false);

	// Already running stuff ....
	if(doc->thhandle)
		{
		P2N("CBriCon Reenter thread prevented\r\n");
		return 0;
		}

	//if(in_update)
	//	return 0;

	//in_update = 1;

	// if changed ...
	if(	  doc->arg1	!= -m_slide1.GetPos() ||
		  doc->arg2	!= -m_slide2.GetPos()
	  )
		{
		doc->arg1		=  -m_slide1.GetPos(), 
		doc->arg2		=  -m_slide2.GetPos();

		doc->command	= PROC_LIGHT;
		
		org_changed++;

		doc->thhandle	= _beginthread(RunWorkerThread, 0, doc);
		}

	//HideStatus();
	//SetCursor(oldc);

	//in_update = 0;

	return 0;
}

void CBriCon::RunWorkerFunc(int comm)

{

}

//////////////////////////////////////////////////////////
// Large slider increments

void CBriCon::OnButton1() 

{
	m_slide1.SetPos(m_slide1.GetPos() - 15);
	UpdateParam();
}

void CBriCon::OnButton2() 

{
	m_slide1.SetPos(m_slide1.GetPos() + 15);
	UpdateParam();
}

///////////////////////////////////////////////////////
// Reset button

void CBriCon::OnButton3() 

{
	CShadesDlg *shadesdlg = (CShadesDlg* )AfxGetApp()->m_pMainWnd;
	ASSERT(shadesdlg->magic == SHADE_MAGIC);

	if(shadesdlg->current <= 0)
		{
		ShowStatus("Restoring image to previous state ...");	
		}

	m_slide1.SetPos(0);
	m_slide2.SetPos(0);

	UpdateParam();

#if 0
	P2N("CBriCon::OnButton3() Reset to original\r\n");

	CShadesDlg *shadesdlg = (CShadesDlg* )AfxGetApp()->m_pMainWnd;
	ASSERT(shadesdlg->magic == SHADE_MAGIC);

	if(shadesdlg->current < 0)
		{
		P2N("No document in CBriCon::OnButton3\r\n");
		return;
		}

	CXrayDoc *doc = (CXrayDoc*)(shadesdlg->docarr[shadesdlg->current]); 

	if(doc)
		{
		ASSERT(doc->magic == DOC_MAGIC);

		ShowStatus("Restoring image to previous state ...");	
		//support.YieldToWinEx();

		doc->img->Copy(*doc->org);
		doc->NewRender();
		doc->changed = true;

		HideStatus();
		support.YieldToWinEx();

		shadesdlg->Invalidate();
		support.YieldToWinEx();
		}
#endif

}

///////////////////////////////////////////////////////

void CBriCon::OnCancel() 

{
	if(((CShadesDlg* )parent)->current >= 0)
		{		
		CXrayDoc *doc = (CXrayDoc*)((CShadesDlg* )parent)->docarr[((CShadesDlg* )parent)->current]; 

		if(org_changed != doc->changed)
			{
			ShowStatus("Operation Cancelled, Restoring Previous Image Settings ...");	
			doc->render->Copy(*doc->img);
		
			HideStatus();
			((CShadesDlg* )parent)->Invalidate(FALSE);
			}
		}
	CGrayDlg::OnCancel();
}

///////////////////////////////////////////////////////
// Small slider increments

void CBriCon::OnButton4() 

{
	m_slide1.SetPos(m_slide1.GetPos() + 5);
	UpdateParam();	
}

void CBriCon::OnButton5() 

{
	m_slide1.SetPos(m_slide1.GetPos() - 5);
	UpdateParam();
}

//////////////////////////////////////////////////////////////////////////

void CBriCon::OnOK() 

{
	if(((CShadesDlg* )parent)->current >= 0)
		{
		// Commit image change
		CXrayDoc *doc = (CXrayDoc*)((CShadesDlg* )parent)->docarr[((CShadesDlg* )parent)->current]; 
		
		if(org_changed != doc->changed)
			{
			ShowStatus("Committing Values ...");	
			
			doc->AddUndo();
			doc->img->Light(-m_slide1.GetPos(), -m_slide2.GetPos());

			doc->changed = org_changed + 1;
			((CShadesDlg* )parent)->Invalidate(false);
			}
		}

	CGrayDlg::OnOK();
}

//////////////////////////////////////////////////////////////////////////

BOOL CBriCon::OnEraseBkgnd(CDC* pDC) 

{
	BOOL ret = CGrayDlg::OnEraseBkgnd(pDC);

	m_slide1.DrawTransparent(true);
	m_slide2.DrawTransparent(true);

	//P2N("CBriCon erase\r\n");
	
	return ret;		
}

void CBriCon::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 

{	
	switch (pScrollBar->GetDlgCtrlID())
		{
		case IDC_SLIDER1:
			P2N("Scroll on slider 1\r\n");
			//UpdateParam(m_slide1.GetPos());	
			break;

		case IDC_SLIDER2:
			P2N("Scroll on slider 2\r\n");
			//UpdateParam(m_slide2.GetPos());	
			break;
		}

	CGrayDlg::OnHScroll(nSBCode, nPos, pScrollBar);
}

int CBriCon::DoModal() 

{
	int ret = CGrayDlg::DoModal();
	return ret;	
}

void CBriCon::OnShowWindow(BOOL bShow, UINT nStatus) 

{
	CGrayDlg::OnShowWindow(bShow, nStatus); 
}

void CBriCon::OnLButtonDown(UINT nFlags, CPoint point) 

{
	CGrayDlg::OnLButtonDown(nFlags, point);
}

void CBriCon::OnMouseMove(UINT nFlags, CPoint point) 

{
	CGrayDlg::OnMouseMove(nFlags, point);
}

void CBriCon::OnLButtonUp(UINT nFlags, CPoint point) 

{
	CGrayDlg::OnLButtonUp(nFlags, point);
}

void CBriCon::OnReleasedcaptureSlider2(NMHDR* pNMHDR, LRESULT* pResult) 

{
	int pos =  m_slide2.GetPos(); // - old_pos;
	
	P2N("OnReleasedcaptureSlider2  %d\r\n", pos);

	if(old_pos2 != pos)
		{
		UpdateParam();
		}

	old_pos2 = pos;
	UpdateParam();

	*pResult = 0;
}

BOOL CBriCon::PreTranslateMessage(MSG* pMsg) 

{
	return CGrayDlg::PreTranslateMessage(pMsg);
}

void CBriCon::OnTimer(UINT nIDEvent) 

{
	SetFocus();	
	KillTimer(nIDEvent);
	CGrayDlg::OnTimer(nIDEvent);
}


#if 0

void CBriCon::OnButton10() 

{
	if(((CShadesDlg* )parent)->current < 0)
		return ;

	CXrayDoc *doc = (CXrayDoc*)((CShadesDlg* )parent)->docarr[((CShadesDlg* )parent)->current]; 

	//BeginWaitCursor();
	ShowStatus("Normalizing Histogram ...");

	HCURSOR oldc = SetCursor(m_busy);

	//doc->render.Copy(doc->img);
	doc->render->HistogramEqualize();
	
	//EndWaitCursor();

	SetCursor(oldc);

	HideStatus();
	((CShadesDlg* )parent)->Invalidate(false);

}

#endif

void CBriCon::OnButton8() 

{
	m_slide2.SetPos(m_slide2.GetPos() + 5);
	UpdateParam();	
}

void CBriCon::OnButton7() 

{
	m_slide2.SetPos(m_slide2.GetPos() - 5);
	UpdateParam();		
}

void CBriCon::OnButton9() 

{
	m_slide2.SetPos(m_slide2.GetPos() + 15);
	UpdateParam();		
}

void CBriCon::OnButton6() 

{
	m_slide2.SetPos(m_slide2.GetPos() -	15);
	UpdateParam();		
}

void CBriCon::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 

{
	switch (pScrollBar->GetDlgCtrlID())
		{
		case IDC_SLIDER1:
			//P2N("Scroll on slider 1\r\n");
			UpdateParam();	
			break;

		case IDC_SLIDER2:
			//P2N("Scroll on slider 2\r\n");
			UpdateParam();	
			break;
		}

	CGrayDlg::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CBriCon::Show()

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

	if(shadesdlg->current >= 0)
		{
		CXrayDoc *doc = (CXrayDoc*)(shadesdlg->docarr[shadesdlg->current]); 

		m_slide1.SetPos(0);
		m_slide2.SetPos(0);

		if(doc)
			{
			P2N("CBriCon::Show Making copy of the original\r\n");
			ASSERT(doc->magic == DOC_MAGIC);
			doc->org->Copy(*doc->img);

			org_changed = doc->changed;
			}
		}

	CGrayDlg::Show();
}

void CBriCon::UpdateCursor()

{
	//P2N("CBriCon::UpdateCursor\r\n");
	
	if(IsWindow(m_hWnd))
		SendMessage(WM_SETCURSOR, HTCLIENT, 0);
}
