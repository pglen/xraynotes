// BmpButt.cpp : implementation file
//

#include "stdafx.h"
#include "dbtest.h"
#include "BmpButt.h"

#include "support.h"

#include "mxpad.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBmpButt

CBmpButt::CBmpButt()

{
	firstpaint = true;
	gotbmp = false;
	m_nWidth = m_nHeight = 0;
	
	disabled = false;
	buttdown = false;

	id = 0;
	hotkey = 0;
	capped = 0, fired = 0;		
}

CBmpButt::~CBmpButt()

{
}

BEGIN_MESSAGE_MAP(CBmpButt, CWnd)
	//{{AFX_MSG_MAP(CBmpButt)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_SYSKEYDOWN()
	ON_WM_SYSKEYUP()
	ON_WM_SYSCHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CBmpButt::FirstPaint()

{
	//P2N("CBmpButt::FirstPaint %p\r\n", this);

	// Obsolete
	//if(id == 0)
	//	{
	//	P2N("Warning: BmpButt with Zero ID %p %s\r\n", this, text);
	//	}

	//ModifyStyle(WS_BORDER | WS_THICKFRAME | WS_DLGFRAME | WS_DISABLED, 0);
	ModifyStyleEx(WS_EX_STATICEDGE | WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE, 0);
	
	//SetWindowPos(NULL, 0, 0, 0, 0, 
	//	SWP_DRAWFRAME  | SWP_FRAMECHANGED | SWP_NOSIZE  | SWP_NOREPOSITION | SWP_NOMOVE |SWP_SHOWWINDOW   );

	tip.Hide();

	BITMAP BitMap;
	gotbmp = m_bitmap.LoadBitmap(bitmap);
	
	if(gotbmp)
		{
		m_bitmap.GetBitmap(&BitMap);
		m_nWidth = BitMap.bmWidth;
		m_nHeight = BitMap.bmHeight;
		
		// Adjust CWnd to reflect
		WINDOWPLACEMENT wp;

		GetWindowPlacement(&wp); 
		wp.rcNormalPosition.right  = wp.rcNormalPosition.left  + m_nWidth + 10;
		wp.rcNormalPosition.bottom = wp.rcNormalPosition.top   + m_nHeight + 35;

		SetWindowPlacement(&wp);
		}
	
	SetFocus();
}

/////////////////////////////////////////////////////////////////////////////
// CBmpButt message handlers

void CBmpButt::OnPaint() 

{
	//P2N("CBmpButt::OnPaint %p\r\n", this);

	if(firstpaint)
		{
		firstpaint = false;
		FirstPaint();
		}

	CPaintDC dc(this); // device context for painting
		
	// Do not call CWnd::OnPaint() for painting messages
}


BOOL CBmpButt::OnEraseBkgnd(CDC* pDC) 

{
	//return CWnd::OnEraseBkgnd(pDC);

	RECT	rect;	GetClientRect(&rect);
	CRect	rect2(rect);
	
	//rect2.top		+= 2; 	rect2.left	  += 2;
	//rect2.right		+= 12; 	rect2.bottom  += 12;

	CDC  dcMem;
	dcMem.CreateCompatibleDC(pDC);
	
	//CClientDC dc(this);
	
	COLORREF col = RGB(150, 150, 150);
	//P2N("Getfocus%p this%p\r\n", GetFocus(), this);

	pDC->FillSolidRect(&rect, col);
	
	CRect	rect3(rect2);

	if(buttdown)
		{
		pDC->DrawEdge(rect3, EDGE_SUNKEN, BF_RECT );
		}
	else	
		{
		pDC->DrawEdge(rect3, EDGE_RAISED , BF_RECT );

		if(GetFocus() == this)
			{
			//col = RGB(200, 200, 200);
			pDC->DrawFocusRect(rect2);
			}
		}

	int offs = 5;
	if(buttdown)
		offs = 6;

	// Only paint if we have a bitmap
	if(gotbmp)
		{
		CBitmap *oldbmp  =  dcMem.SelectObject(&m_bitmap);
		
		CBrush  brush(HS_DIAGCROSS , RGB(198,198,198));
		CBrush  *oldbrush = (CBrush *)pDC->SelectObject(&brush);

		int rop = SRCCOPY;
		// Paint over if disabled
		if(disabled)
			{
			rop = MERGECOPY;
			}

		pDC->BitBlt(offs, offs, m_nWidth, m_nHeight, &dcMem, 0, 0, rop);
		
		dcMem.SelectObject(oldbmp);
		dcMem.SelectObject(oldbrush);
		}

	if(text != "")
		{
		pDC->SetBkMode(TRANSPARENT);
		CRect rect4(rect), rect5;

		pDC->DrawText(text, rect5, DT_CALCRECT);
		rect4.top = rect4.bottom - (rect5.Height() + 5);

		CPoint pp(1,1);
		if(buttdown)
			rect4 += pp;

		pDC->DrawText(text, rect4, DT_CENTER);
		}

	return false;
}

void CBmpButt::OnLButtonDown(UINT nFlags, CPoint point) 

{
	//P2N("CBmpButt::OnLButtonDown \r\n");

	TRACKMOUSEEVENT te;

	te.cbSize = sizeof(TRACKMOUSEEVENT);
	te.dwFlags = TME_LEAVE;
	te.hwndTrack = this->m_hWnd;
	te.dwHoverTime = 0;

	::_TrackMouseEvent(&te);

	tip.Hide();
	capped = fired = false;

	SetCapture();
	SetFocus();
	buttdown = true;
	Invalidate();

	//CWnd::OnLButtonDown(nFlags, point);
}

void CBmpButt::OnLButtonUp(UINT nFlags, CPoint point) 

{
	//P2N("CBmpButt::OnLButtonUp\r\n");
	
	CRect rc;
	GetClientRect(&rc);

	if(rc.PtInRect(point))
		{
		SimulatePush();
		}
	else
		{
		buttdown = false;
		Invalidate();
		}

	ReleaseCapture();

	TRACKMOUSEEVENT te;

	te.cbSize = sizeof(TRACKMOUSEEVENT);
	te.dwFlags = TME_CANCEL | TME_LEAVE;
	te.hwndTrack = this->m_hWnd;		
	::_TrackMouseEvent(&te);	

	//CWnd::OnLButtonUp(nFlags, point);
}

void CBmpButt::OnKillFocus(CWnd* pNewWnd) 

{
	CWnd::OnKillFocus(pNewWnd);
	Invalidate();
}

void CBmpButt::OnSetFocus(CWnd* pOldWnd) 

{
	CWnd::OnSetFocus(pOldWnd);
	Invalidate();
}

BOOL CBmpButt::PreTranslateMessage(MSG* pMsg) 

{

#if 0

	if(pMsg->message == WM_CHAR)
		{
		OnChar(pMsg->wParam, LOWORD(pMsg->lParam), pMsg->lParam); 
		return 0;
		}
	if(pMsg->message == WM_KEYDOWN)
		{
		OnKeyDown(pMsg->wParam, LOWORD(pMsg->lParam), pMsg->lParam); 
		return 0;
		}
	if(pMsg->message == WM_KEYUP)
		{
		OnKeyUp(pMsg->wParam, LOWORD(pMsg->lParam), pMsg->lParam); 
		return 0;
		}
	if(pMsg->message == WM_SYSCHAR)
		{
		OnSysChar(pMsg->wParam, LOWORD(pMsg->lParam), pMsg->lParam); 
		return 0;
		}
	if(pMsg->message == WM_SYSKEYDOWN)
		{
		OnSysKeyDown(pMsg->wParam, LOWORD(pMsg->lParam), pMsg->lParam); 
		return 0;
		}
	if(pMsg->message == WM_SYSKEYUP)
		{
		OnSysKeyUp(pMsg->wParam, LOWORD(pMsg->lParam), pMsg->lParam); 
		return 0;
		}
#endif


	if(pMsg->message == WM_MOUSELEAVE)
		{
		//P2N("Mouse leave message\r\n");

		capped = false;
		fired = false;

		if(buttdown)
			{
			buttdown = false; 
			Invalidate();
			ReleaseCapture();
			}
		return 0;
		}

	if(pMsg->message == WM_MOUSEHOVER )
		{
		//P2N("Mouse hover message\r\n");
		fired = true;

		if(capped)
			{
			capmouse.x = mouse.x;
			capmouse.y = mouse.y;
			
			TRACKMOUSEEVENT te;

			te.cbSize = sizeof(TRACKMOUSEEVENT);
			te.dwFlags = TME_HOVER | TME_CANCEL ;
			te.hwndTrack = this->m_hWnd;		
			::_TrackMouseEvent(&te);	

			// Autosize and autolocate
			CPoint screen = mouse;
			ClientToScreen(&screen); screen.x += 12; screen.y += 24;
			tip.Show(tooltip, screen);
			}
		}
	return CWnd::PreTranslateMessage(pMsg);
}


void CBmpButt::OnMouseMove(UINT nFlags, CPoint point) 

{
	TRACKMOUSEEVENT te;

	mouse.x = point.x;
	mouse.y = point.y;

	if(tooltip != "")
		{
		if(!capped)
			{
			capped = true;

			if(!fired)
				{
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
			}	
		else 
			{
			if(fired)
				{
				// Moved more than hover pixels?
				if	(abs(capmouse.x - point.x) > 5 ||
						abs(capmouse.y - point.y) > 5)
					{
					tip.Hide();
					//P2N("Move in no tracking\r\n");
					fired = false;
					capped = false;
					}
				}
			}
		}
	CWnd::OnMouseMove(nFlags, point);
}

void CBmpButt::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 

{
	//P2N("CBmpButt::OnChar '%c'\r\n", nChar);
	CWnd::OnChar(nChar, nRepCnt, nFlags);
}

void CBmpButt::SimulatePush()

{
	//P2N("CBmpButt::Simulating push\r\n");

	SetFocus();
	buttdown = false;
	Invalidate();

	//GetParent()->PostMessage(WM_USER + 10, GetDlgCtrlID(), (long)m_hWnd);

	GetParent()->PostMessage(WM_COMMAND, (BN_CLICKED << 16) + GetDlgCtrlID(), (long)m_hWnd);
}

void CBmpButt::SimulateDown()

{
	buttdown = true;
	SetFocus();
	Invalidate();
}

void CBmpButt::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 

{
	P2N("CBmpButt::OnKeyDown(UINT nChar\r\n", nChar);

	tip.Hide();
	
	if(nChar == ' ')
		{
		SimulateDown();
		}

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CBmpButt::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	//P2N("CBmpButt::OnKeyUp\r\n");

	if(nChar == ' ')
		{
		SimulatePush();
		}

	CWnd::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CBmpButt::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	//P2N("CBmpButt::OnSysKeyDown %d \r\n", nChar);

	CWnd::OnSysKeyDown(nChar, nRepCnt, nFlags);
}

void CBmpButt::OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	//P2N("CBmpButt::OnSysKeyUp %d \r\n", nChar);

	CWnd::OnSysKeyUp(nChar, nRepCnt, nFlags);
}

void CBmpButt::OnSysChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	//P2N("CBmpButt::OnSysChar %d \r\n", nChar);

	CWnd::OnSysChar(nChar, nRepCnt, nFlags);
}
