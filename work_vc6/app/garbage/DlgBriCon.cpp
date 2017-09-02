/////////////////////////////////////////////////////////////////////////////
// DlgBriCon.cpp : implementation file
//

#include "stdafx.h"
#include "shades.h"
#include "shadesDlg.h"
#include "MsgBox.h"
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
// DlgBriCon dialog

DlgBriCon::DlgBriCon(CWnd* pParent /*=NULL*/)
	: CGrayDlg(DlgBriCon::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgBriCon)
	//}}AFX_DATA_INIT

	//doc = NULL;
	drag = 0;
	first_show = true;
	//created = 0;
	
	m_busy = LoadCursor(NULL, IDC_WAIT);
	ASSERT(m_busy);
}

///////////////////////////////////////////////////////

DlgBriCon::~DlgBriCon()

{
}

///////////////////////////////////////////////////////

void DlgBriCon::DoDataExchange(CDataExchange* pDX)
{
	CGrayDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgBriCon)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Control(pDX, IDC_BUTTON1, m_b1);
	DDX_Control(pDX, IDC_BUTTON5, m_b5);
	DDX_Control(pDX, IDC_BUTTON4, m_b4);
	DDX_Control(pDX, IDC_BUTTON3, m_b3);
	DDX_Control(pDX, IDC_BUTTON2, m_b2);
	DDX_Control(pDX, IDC_SLIDER1, m_slide1);
	DDX_Control(pDX, IDC_SLIDER2, m_slide2);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(DlgBriCon, CGrayDlg)
	//{{AFX_MSG_MAP(DlgBriCon)
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
// DlgBriCon message handlers

BOOL DlgBriCon::OnInitDialog() 

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

	//m_slide2.SetFocus();
	
	//SetTimer(1, 100, NULL);

	return TRUE; 
}

/////////////////////////////////////////////////////////////////////

void DlgBriCon::OnReleasedcaptureSlider1(NMHDR* pNMHDR, LRESULT* pResult) 

{	
	int pos =  m_slide1.GetPos(); // - old_pos;
	
	P2N("OnReleasedcaptureSlider1  %d\r\n", pos);

	UpdateParam();

	*pResult = 0;
}

///////////////////////////////////////////////////////////

int		DlgBriCon::UpdateParam()

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
	//YieldToWinEx();

	//doc->render->Copy(*doc->img);
	//doc->render->Light(-m_slide1.GetPos(), -m_slide2.GetPos());
	//((CShadesDlg* )parent)->Invalidate(false);

	// Already running stuff ....
	if(doc->thhandle)
		{
		P2N("DlgBriCon Reenter thread prevented\r\n");
		return 0;
		}

	//if(in_update)
	//	return 0;

	//in_update = 1;

	doc->arg1		=  -m_slide1.GetPos(), 
	doc->arg2		=  -m_slide2.GetPos();
	doc->command	= PROC_LIGHT;
	doc->thhandle	= _beginthread(RunPreviewThread, 0, doc);

	//HideStatus();
	//SetCursor(oldc);

	//in_update = 0;

	return 0;
}

void DlgBriCon::RunWorkerFunc(int comm)

{

}

//////////////////////////////////////////////////////////
// Large slider increments

void DlgBriCon::OnButton1() 

{
	m_slide1.SetPos(m_slide1.GetPos() - 15);
	UpdateParam();
}

void DlgBriCon::OnButton2() 

{
	m_slide1.SetPos(m_slide1.GetPos() + 15);
	UpdateParam();
}

///////////////////////////////////////////////////////
// Reset button

void DlgBriCon::OnButton3() 

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
	P2N("DlgBriCon::OnButton3() Reset to original\r\n");

	CShadesDlg *shadesdlg = (CShadesDlg* )AfxGetApp()->m_pMainWnd;
	ASSERT(shadesdlg->magic == SHADE_MAGIC);

	if(shadesdlg->current < 0)
		{
		P2N("No document in DlgBriCon::OnButton3\r\n");
		return;
		}

	CXrayDoc *doc = (CXrayDoc*)(shadesdlg->docarr[shadesdlg->current]); 

	if(doc)
		{
		ASSERT(doc->magic == DOC_MAGIC);

		ShowStatus("Restoring image to previous state ...");	
		YieldToWinEx();

		doc->render->Copy(*doc->org);
		doc->NewRender();
		doc->changed = true;

		HideStatus();
		YieldToWinEx();

		shadesdlg->Invalidate();
		YieldToWinEx();
		}
#endif

}

///////////////////////////////////////////////////////

void DlgBriCon::OnCancel() 

{
	if(((CShadesDlg* )parent)->current >= 0)
		{		
		CXrayDoc *doc = (CXrayDoc*)((CShadesDlg* )parent)->docarr[((CShadesDlg* )parent)->current]; 

		ShowStatus("Operation Cancelled, Restoring Previous Image Settings ...");	
		doc->render->Copy(*doc->img);
		
		HideStatus();
		((CShadesDlg* )parent)->Invalidate();
		}

	CGrayDlg::OnCancel();
}

///////////////////////////////////////////////////////
// Small slider increments

void DlgBriCon::OnButton4() 

{
	m_slide1.SetPos(m_slide1.GetPos() + 5);
	UpdateParam();	
}

void DlgBriCon::OnButton5() 

{
	m_slide1.SetPos(m_slide1.GetPos() - 5);
	UpdateParam();
}

//////////////////////////////////////////////////////////////////////////

void DlgBriCon::OnOK() 

{
	if(((CShadesDlg* )parent)->current >= 0)
		{
		// Commit image change
		CXrayDoc *doc = (CXrayDoc*)((CShadesDlg* )parent)->docarr[((CShadesDlg* )parent)->current]; 
		
		ShowStatus("Committing Values ...");	
		
		doc->AddUndo();
		doc->img->Light(-m_slide1.GetPos(), -m_slide2.GetPos());

		//HideStatus();
		doc->changed = true;
		((CShadesDlg* )parent)->Invalidate();

		}
	CGrayDlg::OnOK();
}

//////////////////////////////////////////////////////////////////////////

BOOL DlgBriCon::OnEraseBkgnd(CDC* pDC) 

{
	BOOL ret = CGrayDlg::OnEraseBkgnd(pDC);

	m_slide1.DrawTransparent(true);
	m_slide2.DrawTransparent(true);

	P2N("DlgBriCon erase\r\n");
	
	return ret;		
}

void DlgBriCon::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 

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

int DlgBriCon::DoModal() 

{
	int ret = CGrayDlg::DoModal();

	return ret;	
}

void DlgBriCon::OnShowWindow(BOOL bShow, UINT nStatus) 

{
	CGrayDlg::OnShowWindow(bShow, nStatus);
	
	//SetFocus();
	//YieldToWinEx();

	if(bShow && first_show)
		{
		//first_show = false;

#if 0
		WINDOWPLACEMENT mwp;
		theApp.m_pMainWnd->GetWindowPlacement(&mwp);

		int mainbottom = 500;
		if(mwp.showCmd == SW_SHOWMAXIMIZED)
			{
			mainbottom = 600;
			}
		else
			{
			mainbottom = mwp.rcNormalPosition.bottom - 150;
			}

		WINDOWPLACEMENT wp;
		GetWindowPlacement(&wp);

		SetWindowPos(NULL,  wp.rcNormalPosition.left, mainbottom,			
							0, 0, SWP_NOSIZE | SWP_NOZORDER);
#endif

		}
}

void DlgBriCon::OnLButtonDown(UINT nFlags, CPoint point) 

{
#if 0
	drag_x = point.x; drag_y = point.y;	
#endif

	CGrayDlg::OnLButtonDown(nFlags, point);
}

void DlgBriCon::OnMouseMove(UINT nFlags, CPoint point) 

{
#if 0
	//P2N("OnMouseMove DlgBriCon \r\n");

	RECT rc;
	
	if(drag)
		{
		GetWindowRect(&rc);
				SetWindowPos( NULL, (point.x +  rc.left) - drag_x , 
						(point.y + rc.top) - drag_y, 0, 0, 
								SWP_NOSIZE | SWP_NOZORDER ) ;  
		goto endd;
		}

	// Moved?
	if(drag_x != point.x || drag_y != point.y)
		{
		// Is drag treshold?

		if((abs(drag_x - point.x)  > 3 ) || (abs(drag_y - point.y) > 3) )
			{
			//P2N("Moved\r\n");

			drag_x = point.x;
			drag_y = point.y;

			// Button?
			if(nFlags & MK_LBUTTON)
				{
				SetCapture();
				drag = 1;
				}
			}
		goto endd;
		}

endd:
	
#endif

	CGrayDlg::OnMouseMove(nFlags, point);
}

void DlgBriCon::OnLButtonUp(UINT nFlags, CPoint point) 

{
#if 0
	drag_x = point.x; drag_y = point.y;

	//focus = false;
	if(drag)
		{
		drag = 0; 
		ReleaseCapture();
		InvalidateRect(NULL, TRUE);
		//m_slide1.SetFocus();
		SetTimer(1, 100, NULL);
		}
#endif

	CGrayDlg::OnLButtonUp(nFlags, point);
}


void DlgBriCon::OnReleasedcaptureSlider2(NMHDR* pNMHDR, LRESULT* pResult) 

{
	int pos =  m_slide2.GetPos(); // - old_pos;
	
	P2N("OnReleasedcaptureSlider2  %d\r\n", pos);

	UpdateParam();

	*pResult = 0;
}

BOOL DlgBriCon::PreTranslateMessage(MSG* pMsg) 

{
#if 0
	if(pMsg->message == WM_CHAR)
		{
		((CShadesDlg* )parent)->SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
		//P2N("DlgBriCon pretrans Char %c\r\n", pMsg->wParam);
		return(0);
		}

	if(pMsg->message == WM_KEYDOWN)
		{
		((CShadesDlg* )parent)->SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
		//P2N("mag pretrans keydown %c (%d) %x\r\n", pMsg->wParam, pMsg->wParam, pMsg->lParam);
		return(0);
		}

	if(pMsg->message == WM_KEYUP)
		{
		((CShadesDlg* )parent)->SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
		//P2N("mag pretrans keyup %c (%d) %x\r\n", pMsg->wParam, pMsg->wParam, pMsg->lParam);
		return(0);
		}

	if(pMsg->message == WM_SYSKEYDOWN)
		{
		((CShadesDlg* )parent)->SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
		//P2N("mag pretrans syskeydown %c (%d) %x\r\n", pMsg->wParam, pMsg->wParam, pMsg->lParam);
		//return(0);
		}

	if(pMsg->message == WM_SYSKEYUP)
		{
		((CShadesDlg* )parent)->SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
		//P2N("mag pretrans syskeyup %c (%d) %x\r\n", pMsg->wParam, pMsg->wParam, pMsg->lParam);
		//return(0);
		}
#endif

	
	return CGrayDlg::PreTranslateMessage(pMsg);
}

void DlgBriCon::OnTimer(UINT nIDEvent) 

{
	SetFocus();	
	KillTimer(nIDEvent);
	CGrayDlg::OnTimer(nIDEvent);
}


#if 0

void DlgBriCon::OnButton10() 

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

void DlgBriCon::OnButton8() 

{
	m_slide2.SetPos(m_slide2.GetPos() + 5);
	UpdateParam();	
}

void DlgBriCon::OnButton7() 

{
	m_slide2.SetPos(m_slide2.GetPos() - 5);
	UpdateParam();		
}

void DlgBriCon::OnButton9() 

{
	m_slide2.SetPos(m_slide2.GetPos() + 15);
	UpdateParam();		
}

void DlgBriCon::OnButton6() 

{
	m_slide2.SetPos(m_slide2.GetPos() -	15);
	UpdateParam();		
}

void DlgBriCon::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 

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

//DEL void DlgBriCon::Show()
//DEL  
//DEL {
//DEL 	//PersistDlg:Show();
//DEL }
