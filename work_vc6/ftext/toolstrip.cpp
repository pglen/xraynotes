
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

#include <winuser.h>
#include "../common/support.h"

#include "ftext.h"
#include "toolstrip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef WS_EX_LAYERED
#define WS_EX_LAYERED 0x00080000 
#define LWA_COLORKEY 1 // Use color as the transparency color.
#define LWA_ALPHA    2 // Use bAlpha to determine the opacity of the layer
#endif

//  Function pointer for lyering API in User32.dll
typedef BOOL (WINAPI *lpfnSetLayeredWindowAttributes)
            (HWND hWnd, COLORREF cr, BYTE bAlpha, DWORD dwFlags);

extern lpfnSetLayeredWindowAttributes g_pSetLayeredWindowAttributes;

/////////////////////////////////////////////////////////////////////////////
// CToolStrip dialog

CToolStrip::CToolStrip(CWnd* pParent /*=NULL*/)
	: PersistDlg(CToolStrip::IDD, pParent)
{
	//{{AFX_DATA_INIT(CToolStrip)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	want_x = 0, want_y = 0;
	
	fired = capped = currbutt = 0, bitmap = 0;
	entered = false;
	
	xgap = ygap = 4;
	lheader = 16;

	prev_button = -1;

	buttonw = num_buttons = 0;

	lbuttdown = dragging = old_in_close = in_close = false;
	
	bitmap = IDB_BITMAP2;

	//windowname = "toolstrip";
}

//////////////////////////////////////////////////////////////////////////

CToolStrip::~CToolStrip()
{
	free(arr_l);
	free(arr_r);
}


void CToolStrip::DoDataExchange(CDataExchange* pDX)
{
	PersistDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CToolStrip)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CToolStrip, PersistDlg)
	//{{AFX_MSG_MAP(CToolStrip)
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_ACTIVATE()
	ON_WM_MOUSEACTIVATE()
	ON_WM_RBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolStrip message handlers

BOOL CToolStrip::OnInitDialog() 

{
	PersistDlg::OnInitDialog();
	
	BYTE	bAlpha = 220;

	if (bAlpha < 255)
		{
        //  Set layered style for the dialog
        SetWindowLong(m_hWnd, GWL_EXSTYLE, 
				GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);

        //  Call it with 255 as alpha - opacity
		if(g_pSetLayeredWindowAttributes)
			g_pSetLayeredWindowAttributes(m_hWnd, 0, bAlpha, LWA_ALPHA);
		}
	m_bitmap.LoadBitmap(bitmap);

	BITMAP BitMap;	m_bitmap.GetBitmap(&BitMap);
	m_nWidth = BitMap.bmWidth;	m_nHeight = BitMap.bmHeight;
	
	if(num_buttons == 0)
		num_buttons = m_nWidth / m_nHeight;
	
	//DUMPINT(num_buttons);

	buttonw = m_nWidth / num_buttons;

	arr_l = (toolstripact*)malloc(num_buttons * sizeof(toolstripact)); ASSERT(arr_l);
	arr_r = (toolstripact*)malloc(num_buttons * sizeof(toolstripact)); ASSERT(arr_r);

	memset(arr_l, 0, num_buttons * sizeof(toolstripact));
	memset(arr_r, 0, num_buttons * sizeof(toolstripact));

	arr_l[0].str = "Prior Approval";
	arr_l[1].str = "afds;ofdsal;ifdsa;lfdsadfsalkjdsfa\r\n"
		"SADOISDAIUOYDSAIUODSAIUOYDSAYIUSDA";


	xdrag = GetSystemMetrics(SM_CXDRAG);
	ydrag = GetSystemMetrics(SM_CYDRAG);

	//ASSERT(boddy);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//////////////////////////////////////////////////////////////////////////

void CToolStrip::Show()

{
	xCreate();

	tip.Hide();
	
  	CPoint	pt(want_x, want_y);
	CRect	rc3; misc.CurrentMonRect(pt, &rc3);

	// Adjust to desktop limits
	if(want_x > rc3.left + rc3.Width() / 2)
		{
		want_x -= (m_nWidth + 46);
		}

#if 0
	// Test animation
	int cnt = 10;
	//if(gl_animated)
		{
		//CenterWindow(this);
		for(int loop = 0; loop <= cnt; loop++)
			{
			SetWindowPos( NULL, want_x, 
					want_y, loop * m_nWidth/cnt + xgap + lheader, 
						m_nHeight + ygap, SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE );  	
			
			Sleep(15);
			ShowWindow(SW_SHOWNOACTIVATE);
			}	
		}
#endif

		
	RECT rc33; GetClientRect(&rc33);

	rclose.left		= rc33.left + 2; 
	rclose.top		= rc33.top + 2;
	rclose.right	= rc33.left + lheader - 2;
	rclose.bottom	= rc33.top  + lheader - 2;


	ShowWindow(SW_SHOWNOACTIVATE);
}


BOOL CToolStrip::OnEraseBkgnd(CDC* pDC) 

{
	RECT rect, m_rDataBox;
	
	GetClientRect(&rect); CopyRect(&m_rDataBox,&rect);

	// Paint solid foundation
	//CBrush NewBrush(0x00ffffff); pDC->SetBrushOrg(0,0) ;
	CBrush NewBrush(0x00aaaaaa); pDC->SetBrushOrg(0,0) ;
	CBrush* pOldBrush = (CBrush*)pDC->SelectObject(&NewBrush);   

	pDC->PatBlt(rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, PATCOPY);
	pDC->SelectObject(pOldBrush);
	
	CDC  dcMem;	dcMem.CreateCompatibleDC(pDC);
	int save = dcMem.SaveDC();
	
	// Paint bitmap
	CBitmap *pOldBitmap  =  dcMem.SelectObject(&m_bitmap);
	CClientDC dc(this);

	int xx = 0, yy = 0, ww = rect.right-rect.left, hh = rect.bottom-rect.top;

	pDC->BitBlt(lheader + xgap / 2, ygap/2, m_nWidth, m_nHeight, &dcMem, 0, 0, SRCCOPY ); 	

	dcMem.SelectObject(pOldBitmap);
	dcMem.RestoreDC(save);

	return false;

	//return PersistDlg::OnEraseBkgnd(pDC);
}

//////////////////////////////////////////////////////////////////////////

void CToolStrip::OnMouseMove(UINT nFlags, CPoint point) 

{
	//P2N("MouseMove %d %d\r\n", point.x, point.y);

	mouse.x = point.x; mouse.y = point.y;

	if(entered.IsFirstTime())
		{
		MouseEnter();	
		}

	// See if mouse in close 
	in_close = PtInRect(&rclose, point);
	if(in_close != old_in_close)
		{	
		old_in_close = in_close;
		InvalidateRect(&rclose);
		}
		
	// This one stopped the sending on mouse up ...
	//if(DragDetect(m_hWnd, point))

	//Moved more than hover pixels?
		
	if(lbuttdown && ( abs(capmouse.x - point.x) > xdrag ||	
			abs(capmouse.y - point.y) > ydrag ))
		{ 
		dragging = true;

		tip.Hide();

		if(GetCapture() != this)
			SetCapture(); 

		//P2N("DragDetect(this, point) %d %d (%d %d)\r\n", pp.x, pp.y,
		//		winrect.left, winrect.top);
	
		RECT rc; GetWindowRect(&rc);
		int  newX   = rc.left +  point.x - capmouse.x;
		int  newY   = rc.top  + point.y - capmouse.y;
		int  width  = rc.right - rc.left;
		int  height = rc.bottom - rc.top;
		UINT flags  = SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE;
		SetWindowPos ( NULL, newX, newY, width, height, flags);
		
		PersistDlg::OnMouseMove(nFlags, point);
		return;				
		}

	RECT wrc; GetWindowRect(&wrc);	CRect rc(wrc);

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
			if	(abs(capmouse.x - point.x) > xdrag ||
					abs(capmouse.y - point.y) > ydrag)
				{
				tip.Hide();
				P2N("Drag Started\r\n");
				//P2N("Move in no tracking\r\n");
				fired = false;
				}
			}
		}
		
	CPoint point2(point); ClientToScreen( &point2 );
	
	//if(dark)
		{
		CRect rc4(wrc); rc4.right = rc4.left + lheader;
		if(rc4.PtInRect(point2))
			{
			if(prev_button != -1)
				{
				P2N("in header, deactivate buttons\r\n");
				prev_button = -1;
				Invalidate();
				}
			goto endd;
			}
		
		if(!rc.PtInRect(point2))
			{
			//ReleaseCapture();
			//dark = false;
			prev_button = -1;
			Invalidate();
			//ShowWindow(SW_HIDE );
			return;
			}
		else
			{
			// RECT Hittest
			int	button = (point.x) / buttonw ;
			if(button != prev_button)
				{
				//P2N("New button %d\r\n", button);
				prev_button = min(button, num_buttons - 1);
				Invalidate();
				}
			}
		}

			

endd:
	PersistDlg::OnMouseMove(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////

void CToolStrip::OnPaint() 

{
	CPaintDC dc(this); // device context for painting

	CPen Pen3;	Pen3.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	CPen* pOldPen = (CPen*)dc.SelectObject(&Pen3);

	//P2N("CToolStrip::OnPaint %d\r\n", prev_button);

	// Vert
	RECT rc3; GetClientRect(&rc3);


	
	// Draw left header CROSS
	
	CPen cPen;	
		
	if(in_close)
		cPen.CreatePen(PS_SOLID, 2, RGB(90, 90, 90)); 
	else
		cPen.CreatePen(PS_SOLID, 2, RGB(190, 190, 190)); 

	dc.SelectObject(&cPen);
	
	dc.MoveTo(rclose.left , rclose.top); 
	dc.LineTo(rclose.right, rclose.bottom);

	dc.MoveTo(rclose.left, rclose.bottom); 
	dc.LineTo(rclose.right, rclose.top);


	// Draw left header lines
	CPen hPen(PS_SOLID, 1, RGB(220, 220, 220));
	pOldPen = (CPen*)dc.SelectObject(&hPen);

	dc.MoveTo(rc3.left + lheader / 3, rc3.top + ygap + lheader); 
	dc.LineTo(rc3.left + lheader / 3, rc3.bottom - ygap);

	dc.MoveTo(rc3.left + 2 * lheader / 3, rc3.top + ygap + lheader); 
	dc.LineTo(rc3.left + 2 * lheader / 3, rc3.bottom - ygap);


	// Draw frame around the buttons
	if(prev_button >= 0)
		{
		lastrc.left		= lheader + xgap / 2 + prev_button * buttonw;
		lastrc.top		= ygap / 2 ;
		lastrc.right	= lastrc.left + buttonw;
		lastrc.bottom	= lastrc.top + buttonw;

		CRect rc2(lastrc);	//rc2.InflateRect(2, 2);

		CPen Pen(PS_SOLID, 1, RGB(100, 100, 100));
		//pOldPen = (CPen*)
		dc.SelectObject(&Pen);

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
		}

	dc.SelectObject(pOldPen);

	// Do not call PersistDlg::OnPaint() for painting messages
}

void CToolStrip::OnLButtonUp(UINT nFlags, CPoint point) 

{
	//P2N("CToolStrip::OnLButtonUp\r\n");

	lbuttdown = false;

	if(dragging)
		{
		ReleaseCapture();
		dragging = false;
		}

	// See if mouse in close 
	if(PtInRect(&rclose, point))
		{
		PersistDlg::OnLButtonDown(nFlags, point);
		Hide();
		}

	PersistDlg::OnLButtonUp(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////

void CToolStrip::OnLButtonDown(UINT nFlags, CPoint point) 

{
	//P2N("CToolStrip::OnLButtonDown idx=%d\r\n", prev_button);

	capmouse = point;
	lbuttdown = true;

#if 0
	if(arr_l[prev_button].func)
		{
		CPoint point2 = point; ClientToScreen(&point2);
		int butt = prev_button;
		prev_button = -1;
		currbutt = butt; 		
		}
#endif

	SetTimer(1, 10, NULL);

	PersistDlg::OnLButtonDown(nFlags, point);
}

void CToolStrip::OnRButtonUp(UINT nFlags, CPoint point) 

{
	//P2N("CToolStrip::OnRButtonUp idx=%d\r\n", prev_button);
	
	PersistDlg::OnRButtonUp(nFlags, point);
}

void CToolStrip::OnRButtonDown(UINT nFlags, CPoint point) 

{
	//P2N("CToolStrip::OnRButtonDown idx=%d\r\n", prev_button);
	
	capmouse = point;

#if 0
	if(arr_r[prev_button].func)
		{
		CPoint point2 = point; ClientToScreen(&point2);
		int butt = prev_button;
		prev_button = -1;
		currbutt = butt; 
		}
#endif

	SetTimer(2, 10, NULL);

	PersistDlg::OnRButtonDown(nFlags, point);
}


BOOL CToolStrip::PreTranslateMessage(MSG* pMsg) 

{
	//P2N("PreTranslateMessage %d\r\n", pMsg->message);

	// Dispatch keyboard messages to ourselvs
	if(pMsg->message == WM_CHAR)
		return 0;

	if(pMsg->message == WM_KEYDOWN)
		return 0;

	if(pMsg->message == WM_KEYUP)
		return 0;

	if(pMsg->message == WM_SYSCHAR)
		{
		//CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);
		//dlg->PostMessage(pMsg->message, pMsg->wParam, pMsg->lParam);

		//P2N("CToolStrip::PreTrans WM_SYSCHAR %c (%d) %x\r\n", pMsg->wParam, pMsg->wParam, pMsg->lParam);
		return(0);
		}

	if(pMsg->message == WM_SYSKEYDOWN)
		{
		//CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);
		//dlg->PostMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
		//P2N("CToolStrip::PreTrans WM_SYSKEYDOWN %c (%d) %x\r\n", pMsg->wParam, pMsg->wParam, pMsg->lParam);
		return(0);
		}

	if(pMsg->message == WM_SYSKEYUP)
		{
		//CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);
		//dlg->PostMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
		//P2N("CToolStrip::PreTrans WM_SYSKEYUP %c (%d) %x\r\n", pMsg->wParam, pMsg->wParam, pMsg->lParam);
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
		return 0;
		}
	return PersistDlg::PreTranslateMessage(pMsg);
}

//////////////////////////////////////////////////////////////////////////

void CToolStrip::OnTimer(UINT nIDEvent) 

{
	//P2N("CToolStrip::OnTimer %d\r\n", nIDEvent);

	switch(nIDEvent)
		{
		case 1:
			KillTimer(nIDEvent);
			//Hide();

			// Post to buddy or parent
			if(boddy)
				{
				::PostMessage(boddy, WM_COMMAND, prev_button, (DWORD)m_hWnd);
				}
			else
				GetParent()->PostMessage(WM_COMMAND, prev_button, (DWORD)m_hWnd);

			if(arr_l[currbutt].func)
				arr_l[currbutt].func(NULL);				
			break;

		case 2:
			KillTimer(nIDEvent);
			//Hide();
	
			// Post to buddy or parent
			if(boddy)
				{
				::PostMessage(boddy, WM_COMMAND, prev_button | 0x80, (DWORD)m_hWnd);
				}
			else
				PostMessage(WM_COMMAND, prev_button | 0x80, (DWORD)m_hWnd);

			if(arr_r[currbutt].func)
				arr_r[currbutt].func(NULL);				

			break;
		}

	PersistDlg::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////

void CToolStrip::Hide()

{
	//ReleaseCapture();

	xCreate();

	TRACKMOUSEEVENT te;

	te.cbSize = sizeof(TRACKMOUSEEVENT);
	te.dwFlags = TME_HOVER | TME_CANCEL ;
	te.hwndTrack = this->m_hWnd;		
	::_TrackMouseEvent(&te);	

	//te.cbSize = sizeof(TRACKMOUSEEVENT);
	//te.dwFlags = TME_LEAVE | TME_CANCEL ;
	//te.hwndTrack = this->m_hWnd;		
	//::_TrackMouseEvent(&te);	

	tip.Hide();

	//dark = 	
	entered = capped = false;
	ReleaseCapture();	
	
	ShowWindow(SW_HIDE);
}

void CToolStrip::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 

{	
	//P2N("CToolStrip::OnKeyDown(UINT nChar %c %d\r\n", 	nChar, nChar);

	tip.Hide();
	TRACKMOUSEEVENT te;

	te.cbSize = sizeof(TRACKMOUSEEVENT);
	te.dwFlags = TME_HOVER | TME_CANCEL ;
	te.hwndTrack = this->m_hWnd;		
	::_TrackMouseEvent(&te);	

	switch(nChar)
		{	
		case  VK_ESCAPE:
			//P2N("CToolStrip::OnKeyDown VK_ESC\r\n");
			Hide();
		break;
		
		case	VK_DOWN:
		case	VK_SPACE:
		case	VK_RETURN:
			{			
			CPoint point2((lastrc.right - lastrc.left) / 2, lastrc.bottom);
			ClientToScreen(&point2);
			//((CShadesDlg*)parent)->currmouse = point2;

			int butt = prev_button;
			prev_button = -1;
			//dark = false;
			currbutt = butt; 

			if(misc.IsShiftDown() || misc.IsCtrlDown() || nChar == VK_DOWN)
				SetTimer(1, 10, NULL);
			else
				SetTimer(2, 10, NULL);
			}
			//P2N("CToolStrip::OnKeyDown VK_SPACE VK_RETURN \r\n");
		break;
		
		case  VK_LEFT:
			if(prev_button > 0)
				prev_button--;
			else
				prev_button = num_buttons;
			
			Invalidate();

			//P2N("CToolStrip::OnKeyDown VK_LEFT\r\n");
		break;

		case  VK_RIGHT:
			if(prev_button < num_buttons)
				prev_button++;
			else
				prev_button = 0;

			Invalidate();

			//P2N("CToolStrip::OnKeyDown VK_RIGHT\r\n");
		break;

		case  VK_UP:
			//P2N("CToolStrip::OnKeyDown VK_UP\r\n");
		break;
		
		//case  VK_DOWN:
		//	//P2N("CToolStrip::OnKeyDown VK_DOWN\r\n");
		//break;
		
		case  VK_PRIOR:
			//P2N("CToolStrip::OnKeyDown VK_PRIOR\r\n");
		break;
		
		case  VK_NEXT:
			//P2N("CToolStrip::OnKeyDown VK_NEXT\r\n");
		break;
		
		case  VK_HOME:
			prev_button = 0;
			Invalidate();

			//P2N("CToolStrip::OnKeyDown VK_HOME\r\n");
		break;

		case  VK_END:
			prev_button = num_buttons;
			Invalidate();

			//P2N("CToolStrip::OnKeyDown VK_END\r\n");
		break;

		//case  VK_RIGHT:
		//	P2N("CToolStrip::OnKeyDown VK_RIGHT\r\n");
		//break;

		}
	PersistDlg::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CToolStrip::MouseHover()

{
	P2N("CToolStrip::MouseHover\r\n");

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
		if(prev_button < 0)
			{
			capmouse = mouse;
			CPoint screen = mouse;
			ClientToScreen(&screen); screen.x += 12; screen.y += 24;

			tip.Move(screen);
			tip.SetText("Click on the 'x' to close or drag by the handle to move" );
			tip.Show(); //Window(true);
			}
		else
			{
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
}

void CToolStrip::MouseLeave()

{
	prev_button = -1;
	Invalidate();
	in_close = false;
	entered = 0;
	ReleaseCapture(); 

	//P2N("CToolStrip::MouseLeave\r\n");

	//Hide();
}

void CToolStrip::MouseEnter()

{
	//P2N("CToolStrip::MouseEnter\r\n");

	//lbuttdown = false;

	TRACKMOUSEEVENT te;

	te.cbSize = sizeof(TRACKMOUSEEVENT);
	te.dwFlags = TME_LEAVE;
	te.hwndTrack = this->m_hWnd;
	::_TrackMouseEvent(&te);

	te.cbSize = sizeof(TRACKMOUSEEVENT);
	te.dwFlags = TME_HOVER;
	te.hwndTrack = this->m_hWnd;
	te.dwHoverTime = 1500;

	::_TrackMouseEvent(&te);
}

void CToolStrip::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	PersistDlg::OnActivate(nState, pWndOther, bMinimized);
	
	//P2N("CToolStrip::OnActivate %d %p\r\n", nState, pWndOther);	
}

int CToolStrip::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message) 

{
	//P2N("CToolStrip::OnMouseActivate\r\n");
	return MA_NOACTIVATE;
	//return PersistDlg::OnMouseActivate(pDesktopWnd, nHitTest, message);
}


void CToolStrip::xCreate()
{

	if(!IsWindow(m_hWnd))
		{
		Create(IDD);
		SetWindowPos( NULL, want_x, want_y, lheader + m_nWidth + xgap, 
							m_nHeight + ygap, SWP_NOZORDER | SWP_NOACTIVATE );  	

		if(boddy)
			CenterWindow(FromHandle(boddy));
		}
}
