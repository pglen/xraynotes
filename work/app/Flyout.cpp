
/* =====[ Flyout.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the Flyout.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  11/6/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// Flyout.cpp : implementation file
// This is a port of an older implementation with capture/release
// CLEANMEUP

#include "stdafx.h"
#include "xraynotes.h"
#include "Flyout.h"

#include <winuser.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFlyout dialog

CFlyout::CFlyout(CWnd* pParent /*=NULL*/)
	: CDialog(CFlyout::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFlyout)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	want_x = 0, want_y = 0, dark = 0;
	fired = capped = currbutt = 0, bitmap = 0;
	entered = false;

	prev_button = -1;
	maxbutton = 0;
	buttonw = 67;
	
	memset(arr_l, 0, sizeof(arr_l));
	memset(arr_r, 0, sizeof(arr_r));
}

void CFlyout::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFlyout)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFlyout, CDialog)
	//{{AFX_MSG_MAP(CFlyout)
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFlyout message handlers

BOOL CFlyout::OnInitDialog() 

{
	CDialog::OnInitDialog();
	
	BYTE	bAlpha = 200;

	if (bAlpha < 255)
		{
        //  Set layered style for the dialog
        SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);

        //  Call it with 255 as alpha - opacity
		if(g_pSetLayeredWindowAttributes)
			g_pSetLayeredWindowAttributes(m_hWnd, 0, bAlpha, LWA_ALPHA);
		}
	m_bitmap.LoadBitmap(bitmap);

	BITMAP BitMap;
	m_bitmap.GetBitmap(&BitMap);
	m_nWidth = BitMap.bmWidth;	m_nHeight = BitMap.bmHeight;

	maxbutton = m_nWidth / buttonw;

	xdrag = GetSystemMetrics(SM_CXDRAG);
	ydrag = GetSystemMetrics(SM_CYDRAG);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFlyout::Show()

{
	if(!IsWindow(m_hWnd))
		{
		Create(IDD);
		SetWindowPos( NULL, want_x, want_y, m_nWidth + 20, m_nHeight + 8, SWP_NOZORDER );  	
		}

	tip.Hide();
	
  	CPoint	pt(want_x, want_y);
	CRect	rc3; misc.CurrentMonRect(pt, &rc3);

	// Adjust to desktop limits
	if(want_x > rc3.left + rc3.Width() / 2)
		{
		want_x -= (m_nWidth + 46);
		}
	
	// Test animation
	int cnt = 10;
	if(gl_animated)
		{
		for(int loop = 0; loop <= cnt; loop++)
			{
			SetWindowPos( NULL, want_x, want_y, loop * m_nWidth/cnt + 20, m_nHeight + 8, SWP_NOZORDER );  	
			Sleep(15);
			ShowWindow(SW_SHOW);
			}	
		}

	ShowWindow(SW_SHOW);
}


BOOL CFlyout::OnEraseBkgnd(CDC* pDC) 

{
	RECT rect, m_rDataBox;
	
	GetClientRect(&rect); CopyRect(&m_rDataBox,&rect);

	CBrush NewBrush(0x00ffffff); pDC->SetBrushOrg(0,0) ;
	CBrush* pOldBrush = (CBrush*)pDC->SelectObject(&NewBrush);   

	pDC->PatBlt(rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, PATCOPY);
	pDC->SelectObject(pOldBrush);
	
	CDC  dcMem;	dcMem.CreateCompatibleDC(pDC);
	int save = dcMem.SaveDC();
	
	CBitmap *pOldBitmap  =  dcMem.SelectObject(&m_bitmap);
	CClientDC dc(this);

	int xx = 0, yy = 0, ww = rect.right-rect.left, hh = rect.bottom-rect.top;

	pDC->BitBlt(10, 4, m_nWidth, m_nHeight, &dcMem, 0, 0, SRCCOPY ); 	

	dcMem.SelectObject(pOldBitmap);
	dcMem.RestoreDC(save);

	return false;

	//return CDialog::OnEraseBkgnd(pDC);
}


void CFlyout::OnMouseMove(UINT nFlags, CPoint point) 

{
	//P2N("MouseMove %d %d\r\n", point.x, point.y);

	RECT wrc; GetWindowRect(&wrc);
	CRect rc(wrc);
	
	mouse.x = point.x; mouse.y = point.y;

	// Moved more than hover pixels?
	if( abs(capmouse.x - point.x) >  xdrag ||
		abs(capmouse.y - point.y) > ydrag)
		{
		tip.Hide();
		}
  
	if(entered.IsFirstTime())
		{
		MouseEnter();	
		}

	TRACKMOUSEEVENT te;

	if(!capped)
		{
		// Moved more than hover pixels?
		if( abs(capmouse.x - point.x) >  xdrag ||
			abs(capmouse.y - point.y) > ydrag)
			{
			capped = true;
			
			te.cbSize = sizeof(TRACKMOUSEEVENT);
			te.dwFlags = TME_HOVER;
			te.hwndTrack = this->m_hWnd;
			te.dwHoverTime = 1500;
		
			::_TrackMouseEvent(&te);
			}

		if(!fired)
			{
			capmouse.x = point.x;
			capmouse.y = point.y;

			
			}
		}	
	else 
		{
		if(fired)
			{
			// Moved more than hover pixels?
			if	(ABS(capmouse.x - point.x) > xdrag ||
					ABS(capmouse.y - point.y) > ydrag)
				{
				tip.Hide();
				//P2N("Move in no tracking\r\n");
				fired = false;
				}
			}
		}
		
	CPoint point2(point); ClientToScreen( &point2 );
	
	if(dark)
		{
		if(!rc.PtInRect(point2))
			{
			ReleaseCapture();
			dark = false;
			prev_button = -1;
			ShowWindow(SW_HIDE );
			return;
			}
		else
			{
			// RECT Hittest
			int	button = (point.x )/ buttonw ;
			if(button != prev_button)
				{
				//P2N("New button %d\r\n", button);
				prev_button = button;
				Invalidate();
				}
			}
		}
	else
		{
		if(rc.PtInRect(point2))
			SetCapture();

		dark = true;		
		}
	CDialog::OnMouseMove(nFlags, point);
}

void CFlyout::OnPaint() 

{
	CPaintDC dc(this); // device context for painting

	CPen Pen3;	Pen3.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	CPen* pOldPen = (CPen*)dc.SelectObject(&Pen3);

	// Vert
	RECT	rc3; GetClientRect(&rc3);

	if(prev_button >= 0)
		{
		lastrc.left		= prev_button * buttonw + 5;
		lastrc.top		= 2;
		lastrc.right	= lastrc.left + 62;
		lastrc.bottom	= lastrc.top + 66;

		CRect rc2(lastrc);	//rc2.InflateRect(2, 2);

		CPen Pen(PS_SOLID, 1, RGB(100, 100, 100));
		pOldPen = (CPen*)dc.SelectObject(&Pen);

		// Vert
		dc.MoveTo(rc2.left, rc2.top); 
		dc.LineTo(rc2.right, rc2.top);

		dc.MoveTo(rc2.left, rc2.bottom); 
		dc.LineTo(rc2.right, rc2.bottom);

		// Horiz
		dc.MoveTo(rc2.left, rc2.top); 
		dc.LineTo(rc2.left, rc2.bottom);

		// Vert
		dc.MoveTo(rc2.left, rc2.bottom); 
		dc.LineTo(rc2.right, rc2.bottom);

		// Horiz
		dc.MoveTo(rc2.right, rc2.top); 
		dc.LineTo(rc2.right, rc2.bottom);

		CPen Pen2(PS_SOLID, 1, RGB(0, 0, 0));
		dc.SelectObject(&Pen2);

		// Vert
		dc.MoveTo(rc2.left, rc2.bottom + 1); 
		dc.LineTo(rc2.right + 2, rc2.bottom + 1);

		// Horiz
		dc.MoveTo(rc2.right + 1, rc2.top); 
		dc.LineTo(rc2.right + 1, rc2.bottom + 2);
	
		dc.SelectObject(pOldPen);
		}

	// Do not call CDialog::OnPaint() for painting messages
}

//////////////////////////////////////////////////////////////////////////

void CFlyout::OnLButtonDown(UINT nFlags, CPoint point) 

{
	//P2N("CFlyout::OnLButtonDown idx=%d\r\n", prev_button);

	if(arr_l[prev_button].func)
		{
		CPoint point2 = point; ClientToScreen(&point2);
		((CShadesDlg*)parent)->currmouse = point2;

		int butt = prev_button;
		prev_button = -1;
		dark = false;
		currbutt = butt; 
		SetTimer(2, 10, NULL);
		}
	CDialog::OnLButtonDown(nFlags, point);
}

void CFlyout::OnRButtonDown(UINT nFlags, CPoint point) 

{
	//P2N("CFlyout::OnRButtonDown idx=%d\r\n", prev_button);

	if(arr_r[prev_button].func)
		{
		CPoint point2 = point; ClientToScreen(&point2);
		((CShadesDlg*)parent)->currmouse = point2;

		int butt = prev_button;
		prev_button = -1;
		dark = false;
		currbutt = butt; 
		SetTimer(1, 10, NULL);
		}

	CDialog::OnRButtonDown(nFlags, point);
}

BOOL CFlyout::PreTranslateMessage(MSG* pMsg) 

{
	// Dispatch keyboard messages to ourselvs
	if(pMsg->message == WM_CHAR)
		return 0;
	if(pMsg->message == WM_KEYDOWN)
		return 0;
	if(pMsg->message == WM_KEYUP)
		return 0;

	if(pMsg->message == WM_SYSCHAR)
		{
		CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);
		dlg->PostMessage(pMsg->message, pMsg->wParam, pMsg->lParam);

		//P2N("CFlyout::PreTrans WM_SYSCHAR %c (%d) %x\r\n", pMsg->wParam, pMsg->wParam, pMsg->lParam);
		return(0);
		}

	if(pMsg->message == WM_SYSKEYDOWN)
		{
		CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);
		dlg->PostMessage(pMsg->message, pMsg->wParam, pMsg->lParam);

		//P2N("CFlyout::PreTrans WM_SYSKEYDOWN %c (%d) %x\r\n", pMsg->wParam, pMsg->wParam, pMsg->lParam);
		return(0);
		}

	if(pMsg->message == WM_SYSKEYUP)
		{
		CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);
		dlg->PostMessage(pMsg->message, pMsg->wParam, pMsg->lParam);

		//P2N("CFlyout::PreTrans WM_SYSKEYUP %c (%d) %x\r\n", pMsg->wParam, pMsg->wParam, pMsg->lParam);
		return(0);
		}

	if(pMsg->message == WM_MOUSEHOVER )
		{
		MouseHover();
		return 0;
		}

	if(pMsg->message == WM_MOUSELEAVE)
		{
		MouseLeave();
		//P2N("CFlyout::PreTranslateMessage Mouse leave message\r\n");
		return 0;
		}
	return CDialog::PreTranslateMessage(pMsg);
}

//////////////////////////////////////////////////////////////////////////

void CFlyout::OnTimer(UINT nIDEvent) 

{
	CShadesDlg * dlg = ((CShadesDlg*)parent);

	switch(nIDEvent)
		{
		case 1:
			KillTimer(nIDEvent);
			Hide();
			
			if (arr_r[currbutt].func)
			{
				P2N("Calling current button %d\r\n", currbutt);
				//(*dlg->functs.*arr_r[currbutt].func)(parent);				
				(*dlg->functs.*arr_r[currbutt].func)(parent);				

				}

			break;

		case 2:
			KillTimer(nIDEvent);
			Hide();
			if(arr_l[currbutt].func)
				P2N("Calling (2) current button %d\r\n", currbutt);
				(*dlg->functs.*arr_l[currbutt].func)(parent);

				//arr_l[currbutt].func(parent);
			break;
		}

	CDialog::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////

void CFlyout::Hide()

{
	if(!IsWindow(m_hWnd))
		{
		Create(IDD);
		SetWindowPos( NULL, want_x, want_y, m_nWidth + 20, m_nHeight + 8, SWP_NOZORDER );  	
		}

	ReleaseCapture();

	TRACKMOUSEEVENT te;

	te.cbSize = sizeof(TRACKMOUSEEVENT);
	te.dwFlags = TME_HOVER | TME_CANCEL ;
	te.hwndTrack = this->m_hWnd;		
	::_TrackMouseEvent(&te);	

	te.cbSize = sizeof(TRACKMOUSEEVENT);
	te.dwFlags = TME_LEAVE | TME_CANCEL ;
	te.hwndTrack = this->m_hWnd;		
	::_TrackMouseEvent(&te);	

	tip.Hide();

	dark = 	entered = capped = false;
	ReleaseCapture();	
	
	ShowWindow(SW_HIDE);
}

void CFlyout::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 

{	
	//P2N("CFlyout::OnKeyDown(UINT nChar %c %d\r\n", 	nChar, nChar);

	tip.Hide();
	TRACKMOUSEEVENT te;

	te.cbSize = sizeof(TRACKMOUSEEVENT);
	te.dwFlags = TME_HOVER | TME_CANCEL ;
	te.hwndTrack = this->m_hWnd;		
	::_TrackMouseEvent(&te);	

	switch(nChar)
		{	
		case  VK_ESCAPE:
			//P2N("CFlyout::OnKeyDown VK_ESC\r\n");
			Hide();
		break;
		
		case	VK_DOWN:
		case	VK_SPACE:
		case	VK_RETURN:
			{			
			CPoint point2((lastrc.right - lastrc.left) / 2, lastrc.bottom);
			ClientToScreen(&point2);
			((CShadesDlg*)parent)->currmouse = point2;

			int butt = prev_button;
			prev_button = -1;
			dark = false;
			currbutt = butt; 

			if(misc.IsShiftDown() || misc.IsCtrlDown() || nChar == VK_DOWN)
				SetTimer(1, 10, NULL);
			else
				SetTimer(2, 10, NULL);
			}
			//P2N("CFlyout::OnKeyDown VK_SPACE VK_RETURN \r\n");
		break;
		
		case  VK_LEFT:
			if(prev_button > 0)
				prev_button--;
			else
				prev_button = maxbutton;
			
			Invalidate();

			//P2N("CFlyout::OnKeyDown VK_LEFT\r\n");
		break;

		case  VK_RIGHT:
			if(prev_button < maxbutton)
				prev_button++;
			else
				prev_button = 0;

			Invalidate();

			//P2N("CFlyout::OnKeyDown VK_RIGHT\r\n");
		break;

		case  VK_UP:
			//P2N("CFlyout::OnKeyDown VK_UP\r\n");
		break;
		
		//case  VK_DOWN:
		//	//P2N("CFlyout::OnKeyDown VK_DOWN\r\n");
		//break;
		
		case  VK_PRIOR:
			//P2N("CFlyout::OnKeyDown VK_PRIOR\r\n");
		break;
		
		case  VK_NEXT:
			//P2N("CFlyout::OnKeyDown VK_NEXT\r\n");
		break;
		
		case  VK_HOME:
			prev_button = 0;
			Invalidate();

			//P2N("CFlyout::OnKeyDown VK_HOME\r\n");
		break;

		case  VK_END:
			prev_button = maxbutton;
			Invalidate();

			//P2N("CFlyout::OnKeyDown VK_END\r\n");
		break;

		//case  VK_RIGHT:
		//	P2N("CFlyout::OnKeyDown VK_RIGHT\r\n");
		//break;

		}
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CFlyout::MouseHover()

{
	P2N("CFlyout::MouseHover\r\n");

	fired = true;
	if(capped)
		{
		capped = false;
		
		TRACKMOUSEEVENT te;

		te.cbSize = sizeof(TRACKMOUSEEVENT);
		te.dwFlags = TME_HOVER | TME_CANCEL ;
		te.hwndTrack = this->m_hWnd;		
		::_TrackMouseEvent(&te);	

		// Exists?
		if(arr_l[prev_button].str)
			{
			// Empty?
			if(arr_l[prev_button].str[0] != '\0')
				{
				capmouse = mouse;
				// Autosize and autolocate
				CPoint screen = mouse;
				ClientToScreen(&screen); screen.x += 12; screen.y += 24;

				tip.Move(screen);
				tip.SetText(arr_l[prev_button].str);
				tip.Show(); //Window(true);
				}
			}	
		}
}

void CFlyout::MouseLeave()

{
	//P2N("CFlyout::MouseLeave\r\n");

	Hide();
}

void CFlyout::MouseEnter()

{
	//P2N("CFlyout::MouseEnter\r\n");

	TRACKMOUSEEVENT te;

	te.cbSize = sizeof(TRACKMOUSEEVENT);
	te.dwFlags = TME_LEAVE;
	te.hwndTrack = this->m_hWnd;
	::_TrackMouseEvent(&te);
}
