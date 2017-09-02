
/* =====[ Chevron.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the Chevron.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  11/7/2007  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// Chevron.cpp : implementation file
// CLEANMEUP

#include "stdafx.h"
#include "xraynotes.h"
#include "Chevron.h"
#include "Splash.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CSplash	  splash;

/////////////////////////////////////////////////////////////////////////////
// CChevron dialog

CChevron::CChevron(CWnd* pParent /*=NULL*/)
	: PersistDlg(CChevron::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChevron)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	InitVars();	
}

CChevron::CChevron(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
	: PersistDlg(nIDTemplate, pParent)

{
	InitVars();
}


void CChevron::DoDataExchange(CDataExchange* pDX)

{
	PersistDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChevron)
	DDX_Control(pDX, IDC_PROMPT, m_prompt);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CChevron, PersistDlg)
	//{{AFX_MSG_MAP(CChevron)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_RBUTTONDOWN()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChevron message handlers

BOOL CChevron::OnInitDialog() 

{
	PersistDlg::OnInitDialog();
	
	BYTE	bAlpha = 50;

	if (bAlpha < 255)
		{
        //  Set layered style for the dialog
        SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);

        //  Call it with 255 as alpha - opacity
		if(g_pSetLayeredWindowAttributes)
			g_pSetLayeredWindowAttributes(m_hWnd, 0, bAlpha, LWA_ALPHA);
		}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//////////////////////////////////////////////////////////////////////////

void CChevron::Show()

{
	//P2N("CChevron::Show()\r\n");

	if(!IsWindow(m_hWnd))
		{
		Create(IDD);

		if(pre_want_x)	want_x = pre_want_x;
		if(pre_want_y)	want_y = pre_want_y;

		SetWindowPos( NULL, want_x, want_y, 0, 0, SWP_NOSIZE | SWP_NOZORDER );  	
		}

	PersistDlg::Show();
}

void CChevron::OnMouseMove(UINT nFlags, CPoint point) 

{
	CRect rc; GetWindowRect(&rc);

	CPoint point2 = point;	ClientToScreen( &point2 ); 
	
	//P2N("CChevron::OnMouseMove(UINT nFlags, CPoint point) \r\n");

	mouse.x = point.x; mouse.y = point.y;

	// Moved more than hover pixels?
	if( abs(capmouse.x - point.x) >  xdrag ||
		abs(capmouse.y - point.y) > ydrag)
		{
		tip.Hide();
		}

	// Modal Dialog in progress, do not do anything
	if(((CShadesApp*)AfxGetApp())->inmodal)		
		goto endd;

	TRACKMOUSEEVENT te;

	if(!entered)
		{
		entered = true;

		te.cbSize = sizeof(TRACKMOUSEEVENT);
		te.dwFlags = TME_LEAVE;
		te.hwndTrack = this->m_hWnd;
		::_TrackMouseEvent(&te);

		MouseEnter();
		}

	if(!capped)
		{
		// Moved more than hover pixels?
		if( abs(capmouse.x - point.x) >  xdrag ||
			abs(capmouse.y - point.y) > ydrag)
			{
			capped = true;

			//P2N("CChevron::OnMouseMove tresh\r\n");
			
			te.cbSize = sizeof(TRACKMOUSEEVENT);
			te.dwFlags = TME_HOVER;
			te.hwndTrack = this->m_hWnd;
			te.dwHoverTime = 1500;
		
			::_TrackMouseEvent(&te);
			}
		SetDark(true);
		}

	if(!drag)
		{
		if(dark)
			{
			if(!rc.PtInRect(point2))
				{
				ReleaseCapture();
				SetTransparency(100);
				dark = false;
				}
			}
		else
			{
			if(rc.PtInRect(point2))
				{
				//OpenFlyOut();
				}
			}
		}
	else  // in drag
		{
		if(dark)
			{
			// Hide flyout if dragging
			SetTransparency(100);
			dark = false;

			if(fo)
				{
				RECT wrc2; fo->GetWindowRect(&wrc2); CRect rc2(wrc2);
				if(!rc2.PtInRect(point2))
					fo->ShowWindow(false);
				}
			}

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

		if((abs(drag_x - point.x)  > xdrag ) || (abs(drag_y - point.y) > ydrag) )
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
		//goto endd;
		}
endd:
		PersistDlg::OnMouseMove(nFlags, point);
}

void CChevron::SetDark(int flag)

{
	dark = flag;
	if(dark)
		SetTransparency(darkval);
	else
		SetTransparency(lightval);
}


void CChevron::SetTransparency(int tran)

{
	//  Call it with 255 as alpha - opacity
	if(g_pSetLayeredWindowAttributes)
		g_pSetLayeredWindowAttributes(m_hWnd, 0, tran, LWA_ALPHA);
}


void CChevron::OnLButtonDown(UINT nFlags, CPoint point) 

{
	drag_x = point.x; drag_y = point.y;

	if(kick)
		CloseFlyOut();
	else
		OpenFlyOut();

	PersistDlg::OnLButtonDown(nFlags, point);
}

void CChevron::OnLButtonUp(UINT nFlags, CPoint point) 

{
	drag_x = point.x; drag_y = point.y;

	//focus = false;
	if(drag)
		{
		drag = 0; 
		ReleaseCapture();
		InvalidateRect(NULL, TRUE);
		}

	dark = false;

	PersistDlg::OnLButtonUp(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////

void CChevron::OpenFlyOut()

{	
	if(!fo)
		return;
	
	RECT wrc; GetWindowRect(&wrc);

	splash.Hide();

	//P2N("Opening Flyout\r\n");

	ReleaseCapture();

	kick = true;
	fo->want_x = wrc.right ; fo->want_y = wrc.top;
	fo->Show();
}

BOOL CChevron::OnEraseBkgnd(CDC* pDC) 

{
	RECT rc;	GetClientRect(&rc);

	//P2N("CChevron::OnEraseBkgnd t=%d l=%d b=%d r=%d\r\n", rc.top, rc.left, rc.bottom, rc.right);

	pDC->FillSolidRect(&rc, bgcolor);	
	
	//return PersistDlg::OnEraseBkgnd(pDC);
	return(true);
}


void CChevron::OnOK()
{
	P2N("Chevron OK: noop\r\n");
}

void CChevron::OnCancel()

{
	P2N("Chevron cancel: noop\r\n");
}

void CChevron::Move(int want_x, int want_y)

{
	SetWindowPos( NULL, want_x, want_y, 0, 0, SWP_NOSIZE | SWP_NOZORDER );  	
}

void CChevron::InitVars()

{	
	want_x = 0, want_y = 0; 
	
	fo		= NULL;
	drag	= 0;

	kick = push	= capped = false;
	darkval = 200, lightval = 100;

	bgcolor = RGB(192,192,192);
//	windowname = support.GetFileNameExt(__FILE__);

	capped = fired = false;

	tiptext = "XrayNotes Chevron";

	SetDark(false);
}

BOOL CChevron::PreTranslateMessage(MSG* pMsg) 

{
	//P2N("CChevron::PreTranslateMessage(%s\r\n", mxpad.num2msg(pMsg->message));

	// Dispatch keyboard messages to ourselves
	if(pMsg->message == WM_CHAR)
		{
		SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
		return 0;
		}
	if(pMsg->message == WM_KEYDOWN)
		{
		SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
		return 0;
		}
	if(pMsg->message == WM_KEYUP)
		{
		SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
		return 0;
		}
	if(pMsg->message == WM_SYSCHAR)
		{
		SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
		return 0;
		}
	if(pMsg->message == WM_SYSKEYDOWN)
		{
		SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
		return 0;
		}
	if(pMsg->message == WM_SYSKEYUP)
		{
		SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
		return 0;
		}

	// MFC (VC6) has no message map entiries defined for these
	if(pMsg->message == WM_MOUSELEAVE)
		{
		MouseLeave();
		return 0;
		}

	if(pMsg->message == WM_MOUSEHOVER)
		{
		//P2N("CChevron::PreTranslateMessage WM_MOUSEHOVER\r\n");
		MouseHover();
		return 0;
		}

	return PersistDlg::PreTranslateMessage(pMsg);
}

void CChevron::MouseHover()

{
	fired = true;
	if(capped)
		{
		capped = false;
		
		TRACKMOUSEEVENT te;

		te.cbSize = sizeof(TRACKMOUSEEVENT);
		te.dwFlags = TME_HOVER | TME_CANCEL ;
		te.hwndTrack = this->m_hWnd;		
		::_TrackMouseEvent(&te);	

		capmouse = mouse;
		// Autosize and autolocate
		CPoint screen = mouse;
		ClientToScreen(&screen); 
		screen.x += 12; screen.y += 24;
		tip.Show(tiptext, screen); 
		}
}

void CChevron::MouseLeave()

{
	//P2N("CChevron::MouseLeave\r\n");

	tip.Hide();

	TRACKMOUSEEVENT te;

	te.cbSize = sizeof(TRACKMOUSEEVENT);
	te.dwFlags = TME_HOVER | TME_CANCEL ;
	te.hwndTrack = this->m_hWnd;		
	::_TrackMouseEvent(&te);	

	capped = false;
	dark = false;
	ReleaseCapture();
	
	entered = capped = false;
	SetDark(false);
}

void CChevron::MouseEnter()

{
	//P2N("CChevron::MouseEnter\r\n");

	SetDark(true);
	OpenFlyOut();
}

void CChevron::CloseFlyOut()

{
	kick =  false;
	fo->ShowWindow(SW_HIDE);
}

void CChevron::OnRButtonDown(UINT nFlags, CPoint point) 
{
	if(kick)
		CloseFlyOut();
	else
		OpenFlyOut();

	PersistDlg::OnRButtonDown(nFlags, point);
}

void CChevron::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 

{
	P2N("CChevron::OnKeyDown(UINT nChar %c %d\r\n", 	nChar, nChar);

	switch(nChar)
		{	
		case  VK_ESCAPE:
			P2N("CChevron::OnKeyDown VK_ESC\r\n");
			//OpenFlyout();
			//Hide();
		break;
		
		case  VK_SPACE:
		case  VK_RETURN:
			OpenFlyOut();
			P2N("CChevron::OnKeyDown VK_SPACE VK_RETURN \r\n");
		break;
		
		case  VK_LEFT:
			P2N("CChevron::OnKeyDown VK_LEFT\r\n");
		break;

		case  VK_RIGHT:
			P2N("CChevron::OnKeyDown VK_RIGHT\r\n");
		break;

		case  VK_UP:
			P2N("CChevron::OnKeyDown VK_UP\r\n");
		break;
		
		case  VK_DOWN:
			P2N("CChevron::OnKeyDown VK_DOWN\r\n");
		break;
		
		case  VK_PRIOR:
			P2N("CChevron::OnKeyDown VK_PRIOR\r\n");
		break;
		
		case  VK_NEXT:
			P2N("CChevron::OnKeyDown VK_NEXT\r\n");
		break;
		
		case  VK_HOME:
			P2N("CChevron::OnKeyDown VK_HOME\r\n");
		break;

		case  VK_END:
			P2N("CChevron::OnKeyDown VK_END\r\n");
		break;

		//case  VK_RIGHT:
		//	P2N("CChevron::OnKeyDown VK_RIGHT\r\n");
		//break;

		}	
	PersistDlg::OnKeyDown(nChar, nRepCnt, nFlags);
}
