
/* =====[ MagDlg.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the MagDlg.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  11/6/2007  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

//////////////////////////////////////////////////////////////////////////
//
// MagDlg.cpp : implementation file
//

#include "stdafx.h"
#include "xraynotes.h"
#include "shades.h"
#include "shadesdlg.h"

#include "MagDlg.h"

#include "xscroll.h"
#include "yscroll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CxScroll	xscroll;
extern CyScroll	yscroll;

/////////////////////////////////////////////////////////////////////////////
// CMagDlg dialog

double mag_lookup[16] = 

{
	1./6,	1./5,	1./4,	1./3,	1./2, 
	1,     
	2,		3,		4,		5,		6
};


CMagDlg::CMagDlg(CWnd* pParent /*=NULL*/)
	: CGrayDlg(CMagDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMagDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	old = dark = 0;
	push = false;
	parent = NULL;
	
	windowname = "CMagDlg";
	noheader = true;

}

void CMagDlg::DoDataExchange(CDataExchange* pDX)
{
	CGrayDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMagDlg)
	DDX_Control(pDX, IDC_MAG, m_mag);
	DDX_Control(pDX, IDC_SLIDER1, m_slide);
	DDX_Control(pDX, IDC_BUTTON1,   m_ToWidth );
	DDX_Control(pDX, IDC_BUTTON2,   m_ToHeight);    
	DDX_Control(pDX, IDC_BUTTON3,   m_ToFull);    
	DDX_Control(pDX, IDC_BUTTON4,   m_Lt);    
	DDX_Control(pDX, IDC_BUTTON5,   m_Gt);    
	DDX_Control(pDX, IDC_BUTTON6,   m_AutoL);    
	DDX_Control(pDX, IDC_BUTTON7,   m_AutoR);    
	DDX_Control(pDX, IDC_BUTTON9,   m_Fine);   	 
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMagDlg, CGrayDlg)
	//{{AFX_MSG_MAP(CMagDlg)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_WM_SYSCHAR()
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER1, OnReleasedcaptureSlider1)
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON9, OnButton9)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMagDlg message handlers


BOOL CMagDlg::OnInitDialog() 

{
	CGrayDlg::OnInitDialog();

	m_slide.SetRange(0, 10);
	m_slide.SetPageSize(1);
	
	m_slide.SetPos(5);

	m_mag.SetTrans();
	CString str; str.Format("%.5f", mag_lookup[m_slide.GetPos()] );
	m_mag.SetText(str);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//DEL void CMagDlg::OnLButtonUp(UINT nFlags, CPoint point) 
//DEL 
//DEL {
//DEL 	drag_x = point.x; drag_y = point.y;
//DEL 
//DEL 	//focus = false;
//DEL 	if(drag)
//DEL 		{
//DEL 		drag = 0; 
//DEL 		ReleaseCapture();
//DEL 		InvalidateRect(NULL, TRUE);
//DEL 		}
//DEL 	CGrayDlg::OnLButtonUp(nFlags, point);
//DEL }

//DEL void CMagDlg::OnLButtonDown(UINT nFlags, CPoint point) 
//DEL {
//DEL 	drag_x = point.x; drag_y = point.y;
//DEL 
//DEL 	RECT rc; GetClientRect(&rc);
//DEL 	CRect rect(rc.right - 17, 2, rc.right - 4, 16);
//DEL 
//DEL 	if(rect.PtInRect(point))
//DEL 		{
//DEL 		P2N("Click in close box\r\n");
//DEL 		ShowWindow(SW_HIDE);
//DEL 		//EndDialog(IDOK);
//DEL 		}
//DEL 
//DEL 	P2N("Mouse down: Drag_x=%d drag_y=%d\r\n", drag_x, drag_y);
//DEL 	//focus = true;	
//DEL 
//DEL 	CGrayDlg::OnLButtonDown(nFlags, point);
//DEL }

//DEL void CMagDlg::OnMouseMove(UINT nFlags, CPoint point) 
//DEL {
//DEL 	RECT wrc; GetWindowRect(&wrc);
//DEL 
//DEL 	CPoint point2 = point;
//DEL 
//DEL 	ClientToScreen( &point2 ); CRect rc(wrc);
//DEL 	
//DEL 	if(!drag)
//DEL 		{
//DEL 		//ReleaseCapture();
//DEL 		}
//DEL 	else
//DEL 		{
//DEL 		GetWindowRect(&rc);
//DEL 				SetWindowPos( NULL, (point.x +  rc.left) - drag_x , 
//DEL 						(point.y + rc.top) - drag_y, 0, 0, 
//DEL 								SWP_NOSIZE | SWP_NOZORDER ) ;  
//DEL 		goto endd;
//DEL 		}
//DEL 
//DEL 	// Moved?
//DEL 	if(drag_x != point.x || drag_y != point.y)
//DEL 		{
//DEL 		// Is drag treshold?
//DEL 
//DEL 		if((abs(drag_x - point.x)  > 3 ) || (abs(drag_y - point.y) > 3) )
//DEL 			{
//DEL 			//P2N("Moved\r\n");
//DEL 
//DEL 			drag_x = point.x;
//DEL 			drag_y = point.y;
//DEL 
//DEL 			// Button?
//DEL 			if(nFlags & MK_LBUTTON)
//DEL 				{
//DEL 				SetCapture();
//DEL 				drag = 1;
//DEL 				}
//DEL 			}
//DEL 		//goto endd;
//DEL 		}
//DEL 
//DEL endd:
//DEL 			
//DEL 	CGrayDlg::OnMouseMove(nFlags, point);
//DEL }

void CMagDlg::OnButton1() 
{
	if(((CShadesDlg* )parent)->current < 0)
		return;

	CXrayDoc *doc = (CXrayDoc*)((CShadesDlg* )parent)->docarr[((CShadesDlg* )parent)->current]; 

	RECT	rc; 
	((CShadesDlg* )parent)->GetClientRect(&rc);

	int renx = (rc.right - rc.left);
	doc->mag = (double)renx / doc->img->GetWidth();

	xscroll.m_scroll.SetScrollPos(50);
	yscroll.m_scroll.SetScrollPos(50);
			
	CString str; str.Format("%.5f", doc->mag);
	m_mag.SetText(str);

	doc->showstate = STATE_WIDTH;
	((CShadesDlg* )parent)->Invalidate();
}

void CMagDlg::OnButton2() 

{
	if(((CShadesDlg* )parent)->current < 0)
		return;

	CXrayDoc *doc = (CXrayDoc*)((CShadesDlg* )parent)->docarr[((CShadesDlg* )parent)->current]; 
	doc->showstate = STATE_HEIGHT;	

	RECT	rc; 
	((CShadesDlg* )parent)->GetClientRect(&rc);

	int reny = (rc.bottom - rc.top);
	doc->mag = (double)reny / doc->img->GetHeight();

	xscroll.m_scroll.SetScrollPos(50);
	yscroll.m_scroll.SetScrollPos(50);
			
	CString str; str.Format("%.5f", doc->mag);
	m_mag.SetText(str);

	((CShadesDlg* )parent)->Invalidate();
}


void CMagDlg::OnButton3() 

{
	if(((CShadesDlg* )parent)->current < 0)
		return;

	CXrayDoc *doc = (CXrayDoc*)((CShadesDlg* )parent)->docarr[((CShadesDlg* )parent)->current]; 
	doc->showstate = STATE_FULL;	

	m_slide.SetPos(5);

	xscroll.m_scroll.SetScrollPos(50);
	yscroll.m_scroll.SetScrollPos(50);
			
	doc->mag = mag_lookup[m_slide.GetPos()];

	CString str; str.Format("%.5f", mag_lookup[m_slide.GetPos()] );
	m_mag.SetText(str);

	((CShadesDlg* )parent)->Invalidate();
}

void CMagDlg::OnSysChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	P2N("Mag Syschar %c %d\r\n", nChar, nChar);
	
	CGrayDlg::OnSysChar(nChar, nRepCnt, nFlags);
}

BOOL CMagDlg::PreTranslateMessage(MSG* pMsg) 

{

#if 0

	if(pMsg->message == WM_CHAR)
		{
		((CShadesDlg* )parent)->SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
		//P2N("CMagDlg pretrans Char %c\r\n", pMsg->wParam);
		return(0);
		}

	if(pMsg->message == WM_KEYDOWN)
		{
		((CShadesDlg* )parent)->SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
		P2N("CMagDlg::PreTranslateMessage WM_KEYDOWN %c (%d) %x\r\n", pMsg->wParam, pMsg->wParam, pMsg->lParam);
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
		P2N("CMagDlg::PreTranslateMessage WM_KEYDOWN %c (%d) %x\r\n", pMsg->wParam, pMsg->wParam, pMsg->lParam);
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

void CMagDlg::OnButton5() 
{
	if( ((CShadesDlg* )parent)->current < 0)
		return;

	((CShadesDlg* )parent)->SendMessage(WM_SYSKEYDOWN, 'P', 0);
}

void CMagDlg::OnButton4() 
{
	if( ((CShadesDlg* )parent)->current < 0)
		return;

	((CShadesDlg* )parent)->SendMessage(WM_SYSKEYDOWN, 'N', 0);
}

void CMagDlg::OnButton6() 
{
	((CShadesDlg* )parent)->SendMessage(WM_SYSKEYDOWN, 'A', 0);	
}

//////////////////////////////////////////////////////////////////////////

//DEL void CMagDlg::OnPaint() 
//DEL 
//DEL {
//DEL 	CPaintDC dc(this); // device context for painting
//DEL 	
//DEL 	RECT rc; GetClientRect(&rc);
//DEL 
//DEL 	// Draw header
//DEL 	//dc.FillSolidRect(2, 2, rc.right - 4, 16, RGB(64, 64, 64));
//DEL 
//DEL 	// Draw corner close
//DEL 	dc.FillSolidRect(rc.right - 17, 2, rc.right - 4, 16, RGB(32, 32, 32));
//DEL 
//DEL 	CPen pen; pen.CreatePen(PS_SOLID, 2, RGB(255,255,255));
//DEL 	CPen *oldpen;
//DEL 
//DEL 	oldpen = (CPen*)dc.SelectObject(pen);
//DEL 
//DEL 	dc.MoveTo( rc.right - 14, 14);
//DEL 	dc.LineTo( rc.right - 4, 4);
//DEL 
//DEL 	dc.MoveTo( rc.right - 14, 4);
//DEL 	dc.LineTo( rc.right - 4, 14);
//DEL 
//DEL 	// Do not call CGrayDlg::OnPaint() for painting messages
//DEL }

void CMagDlg::OnButton7() 
{
	((CShadesDlg* )parent)->SendMessage(WM_SYSKEYDOWN, 'B', 0);		
}


void CMagDlg::OnReleasedcaptureSlider1(NMHDR* pNMHDR, LRESULT* pResult) 

{
	//UpdateDoc();
	*pResult = 0;
}

void CMagDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 

{
	//P2N("Scrolled to %d\r\n", m_slide.GetPos());	
	CGrayDlg::OnVScroll(nSBCode, nPos, pScrollBar);
}

//////////////////////////////////////////////////////////////////////////

void CMagDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 

{
	P2N("Scrolled to %d\r\n", m_slide.GetPos());	

	UpdateDoc();

	CGrayDlg::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CMagDlg::OnButton9() 

{
	if( ((CShadesDlg* )parent)->current < 0)
		return;

	CXrayDoc *doc = (CXrayDoc*)((CShadesDlg* )parent)->docarr[((CShadesDlg* )parent)->current]; 

	if(doc)
		{
		// Toggle fine state
		if(doc->finestate)
			doc->finestate = false;
		else
			doc->finestate = true;

		((CShadesDlg* )parent)->Invalidate(false);			
		}
}

//////////////////////////////////////////////////////////////////////////

void CMagDlg::UpdateDoc()

{
	int mag	 = m_slide.GetPos();
	
	if (mag == old)
		return;
	
	old = mag;

	double mag2 = mag_lookup[mag];

	CString str; str.Format("%.5f", mag_lookup[m_slide.GetPos()] );
	m_mag.SetText(str);

	P2N("Set slider to: %d %f\r\n", mag, mag2);

	if(((CShadesDlg* )parent)->current < 0)
		return;

	CXrayDoc *doc = (CXrayDoc*)((CShadesDlg* )parent)->docarr[((CShadesDlg* )parent)->current]; 

	if(doc)
		{
		doc->oldshowstate = -1;		// Force render
		doc->showstate = STATE_VARI;	
		doc->mag = mag2;
			
		doc->finestate = false;
		((CShadesDlg* )parent)->Invalidate();
		}
}

//DEL void CMagDlg::xCreate()
//DEL 
//DEL {
//DEL 	if(!::IsWindow(m_hWnd))
//DEL 		{
//DEL 		Create(IDD);
//DEL 		SetWindowPos( NULL, want_x, want_y, 0, 0, SWP_NOSIZE | SWP_NOZORDER );  	
//DEL 		}
//DEL }
