
/* =====[ EditEx.cpp ]========================================== 
                                                                             
   Description:     The dbtest project, implementation of the EditEx.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  1/5/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */


//////////////////////////////////////////////////////////////////////////
// EditEx.cpp : implementation file
//

#include "stdafx.h"
#include "dbtest.h"
#include "EditEx.h"
#include "mxpad.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditEx

CEditEx::CEditEx()

{
	disabled = false;
	grey = RGB(162, 162, 162);
	NewBrush.CreateSolidBrush(grey); 
	first = 0;
	tipshow = true;
}

CEditEx::~CEditEx()

{
}


BEGIN_MESSAGE_MAP(CEditEx, CEdit)
	//{{AFX_MSG_MAP(CEditEx)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_KEYDOWN()
	ON_WM_KILLFOCUS()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditEx message handlers



HBRUSH CEditEx::CtlColor(CDC* pDC, UINT nCtlColor) 

{
	//P2N("CEditEx::CtlColor %d\r\n", nCtlColor); 

	if(disabled)
		{
		pDC->SetBkColor(grey);
		return NewBrush;
		}
	else
		return NULL;
}

void CEditEx::OnMouseMove(UINT nFlags, CPoint point) 

{
	mouse.x = point.x;
	mouse.y = point.y;

	//P2N("mouse move x=%d y=%d\r\n", point.x, point.y);

	TRACKMOUSEEVENT te;

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
				StopTip();
				}
			}
		}
			
	CEdit::OnMouseMove(nFlags, point);
}

BOOL CEditEx::PreTranslateMessage(MSG* pMsg) 

{
	//P2N("CEditEx::PreTranslateMessage %d %s\r\n", pMsg->message, mxpad.num2msg(pMsg->message));		

	//////////////////////////////////////////////////////////////////////
	// Dispatch messages to ourselvs (no subclassing needed)

#if 0
	if(pMsg->message == WM_CHAR)
		{
		OnChar(pMsg->wParam, LOWORD(pMsg->lParam), pMsg->lParam); 
		return 0;
		}

	if(pMsg->message == WM_KEYDOWN)
		{
		StopTip();
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
		StopTip();
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
		StopTip();
		return 0;
		}

	if(pMsg->message == WM_MOUSEHOVER )
		{
		//P2N("CEditEx::Mouse hover message\r\n");
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

			ShowTip();
			
			capped = false;		
			}
		}	

	return CEdit::PreTranslateMessage(pMsg);
}

//////////////////////////////////////////////////////////////////////////

void CEditEx::StopTip()

{
	tip.Hide();

	//P2N("CEditEx::StopTip\r\n");

	TRACKMOUSEEVENT te;

	te.cbSize = sizeof(TRACKMOUSEEVENT);
	te.dwFlags = TME_HOVER | TME_CANCEL ;
	te.hwndTrack = this->m_hWnd;		
	::_TrackMouseEvent(&te);	

	capped = false;
	fired = false;
}

BOOL CEditEx::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 

{
	// Simulate key feed to message box
	if(zDelta > 0)
		{
		//P2N("Mouse Wheel Up\r\n");
		PostMessage(WM_KEYDOWN, 38, 0);
		PostMessage(WM_KEYUP, 38, 0);
		}
	else
		{
		//P2N("Mouse Wheel Down\r\n");
		PostMessage(WM_KEYDOWN, 40, 0);
		PostMessage(WM_KEYUP, 40, 0);
		}
		
	return CEdit::OnMouseWheel(nFlags, zDelta, pt);
}

void CEditEx::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 

{
	//P2N("CEditEx::OnKeyDown(UINT nChar=%d ctrl=%d)\r\n", nChar, GetKeyState(VK_CONTROL) & 0x80);

	if((GetKeyState(VK_CONTROL) & 0x80) && nChar == 65)
		{
		SetSel(0, -1);	
		}
	
	//if((GetKeyState(VK_CONTROL) & 0x80) && nChar == 90)
	//	{
	//	P2N("CEditEx::OnKeyDown Undo\r\n");
	//	return;
	//	//SetSel(0, -1);	
	//	}

	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CEditEx::LoadTip(int id)

{
	if(id)
		if(!tiptext.LoadString(id))
			AfxMessageBox("CEditEx::LoadTip: Error Loading String for tooltip.");
}

void CEditEx::OnKillFocus(CWnd* pNewWnd) 

{
	CEdit::OnKillFocus(pNewWnd);
	StopTip();	
}

void CEditEx::OnPaint() 

{
	if(!first++)
		{
		//P2N("FirstPaint %p\r\n", this);
		//SubclassWindow(m_hWnd);
		}

	//CPaintDC dc(this); // device context for painting

	CEdit::OnPaint(); // for painting messages
}

void CEditEx::ShowTip()

{
	//P2N("CEditEx::ShowTip Showing \r\n");
	
	CPoint screen = mouse;
	ClientToScreen(&screen); screen.x += 12; screen.y += 24;

	if(tiptext != "")
		{		
		if(tipshow)
			{
			tip.Show(tiptext, screen);
			}
		}
}

void CEditEx::OnSetFocus(CWnd* pOldWnd) 

{
	//P2N("CEditEx::OnSetFocus\r\n");

	CEdit::OnSetFocus(pOldWnd);

	sarr.RemoveAll();
	rsarr.RemoveAll();

	CString str;
	GetWindowText(str);
	sarr.Add(str);

	SetSel(0, -1);		
}

//////////////////////////////////////////////////////////////////////////
// Called from the onchildnotify routine to allow reflection 
// in derived classed

void CEditEx::OnChange() 

{	
	if(sarr.GetSize() > 10)
		sarr.RemoveAt(0);

	CString str;

	GetWindowText(str);
	//P2N("CEditEx::OnChange '%s'\r\n", str);
	sarr.Add(str);	
}

void CEditEx::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 

{
	//P2N("CEditEx::OnChar (UINT nChar=%d ctrl=%d)\r\n", nChar, GetKeyState(VK_CONTROL) & 0x80);

	// CTRL-Z Re Handle undo/redo 
	if((GetKeyState(VK_CONTROL) & 0x80) && nChar == 26)
		{
		//P2N("CEditEx::OnChar Undo\r\n");

		if(sarr.GetSize() > 0)
			{
			int startt, endd;

			GetSel(startt, endd);

			//for(int loop = 0; loop < sarr.GetSize(); loop++)
			//	{
			//	CString sss = sarr.GetAt(loop);
			//	P2N("Stack '%s'\r\n", sss);
			//	}

			// Remove the last entry
			sarr.RemoveAt(sarr.GetUpperBound());
			
			CString str, str2;
			if(sarr.GetSize())
				{
				str = sarr.GetAt(sarr.GetUpperBound());
				}
			GetWindowText(str2);
			rsarr.Add(str2);

			SetWindowText(str);
			// Remove the newly changed one
			if(sarr.GetSize())
				sarr.RemoveAt(sarr.GetUpperBound());

			//SetSel(startt, endd);
			//SetSel(-1, -1);
			SetSel(str.GetLength(), str.GetLength());

			return;
			}
		}

	if((GetKeyState(VK_CONTROL) & 0x80) && nChar == 25)
		{
		//P2N("CEditEx::OnChar Redo\r\n");
		
		//for(int loop = 0; loop < rsarr.GetSize(); loop++)
		//	{
		//	CString sss = rsarr.GetAt(loop);
		//	P2N("rStack '%s'\r\n", sss);
		//	}

		CString str;
		if(rsarr.GetSize())
			{
			str = rsarr.GetAt(rsarr.GetUpperBound());

			// Remove the redone item
			rsarr.RemoveAt(rsarr.GetUpperBound());
			SetWindowText(str);
			SetSel(str.GetLength(), str.GetLength());	
			}
		}

	CEdit::OnChar(nChar, nRepCnt, nFlags);
}


BOOL CEditEx::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult) 

{
	//P2N("CEditEx::OnChildNotify %d %s wParam=%d lParam=%d \r\n", message, mxpad.num2msg(message), wParam, lParam);	

	if(message == WM_COMMAND)
		{
		//P2N("Window handle=%d control=%d EN_CHANGE=%d message=%d\r\n", lParam,  LOWORD(wParam), EN_CHANGE, HIWORD(wParam));

		if(EN_CHANGE == HIWORD(wParam))
			{
			OnChange();
			}
		}

	return CEdit::OnChildNotify(message, wParam, lParam, pLResult);
}


void CEditEx::Enable()
{
	disabled = false;
	SetReadOnly(false);
	Invalidate();
}

void CEditEx::Disable()

{
	disabled = true;
	SetReadOnly(true);
	Invalidate();
}
