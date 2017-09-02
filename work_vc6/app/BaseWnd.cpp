
/* =====[ BaseWnd.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the BaseWnd.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  11/25/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// BaseWnd.cpp : implementation file
//

#include "stdafx.h"
#include "xraynotes.h"
#include "BaseWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CBaseWnd, CWnd)


#if 0

// Register the window class if it has not already been registered.
BOOL CBaseWnd::RegisterWindowClass()

{
    WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();
   // HINSTANCE hInst = AfxGetResourceHandle(); 

    if (!(::GetClassInfo(hInst, XCTRL_CLASSNAME, &wndcls)))
    {
		//P2N("CBaseWnd::RegisterWindowClass Creating class\r\n");

        // otherwise we need to register a new class
        wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
        wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = XCTRL_CLASSNAME;

        if (!AfxRegisterClass(&wndcls)) {
            AfxThrowResourceException();
            return FALSE;
        }
    }
    return TRUE;
}

#endif

#if 0

BOOL CBaseWnd::Create(const RECT& rect, CWnd* pParentWnd, UINT nID, DWORD dwStyle)

{
	//P2N("CBaseWnd::Create \r\n");

    ASSERT(pParentWnd->GetSafeHwnd());

    if (!CWnd::Create(XCTRL_CLASSNAME, NULL, dwStyle, rect, pParentWnd, nID)) 
        return FALSE;

	return true;
}

BOOL CBaseWnd::CreateEx( DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hwndParent, HMENU nIDorHMenu, LPVOID lpParam)

{
	//P2N("CBaseWnd::CreateEx\r\n");
	return true;
}

#endif

/////////////////////////////////////////////////////////////////////////////
// CBaseWnd derived from CWnd

CBaseWnd::CBaseWnd()

{
	//P2N("CBaseWnd::CBaseWnd\r\n");

	capped = fired = false;

	//tooltip = "Blank Tooltip";
	//RegisterWindowClass();

	firstpaint = true;

	//entered = false;

	xdrag = GetSystemMetrics(SM_CXDRAG);
	ydrag = GetSystemMetrics(SM_CYDRAG);

	hovertime = 1200;
}

CBaseWnd::~CBaseWnd()

{
	//P2N("CBaseWnd::~CBaseWnd\r\n");
}

BEGIN_MESSAGE_MAP(CBaseWnd, CWnd)
	//{{AFX_MSG_MAP(CBaseWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_ACTIVATE()
	ON_WM_MOUSEACTIVATE()
	ON_WM_SETFOCUS()
	ON_WM_CREATE()
	ON_WM_CHAR()
	ON_WM_SYSCHAR()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBaseWnd message handlers

void CBaseWnd::OnLButtonDown(UINT nFlags, CPoint point) 

{
	//P2N("CBaseWnd::OnLButtonDown \r\n");
	//SetFocus();
	CWnd::OnLButtonDown(nFlags, point);
}

void CBaseWnd::OnMouseMove(UINT nFlags, CPoint point) 

{
	lastmousepoint.x = point.x;	lastmousepoint.y = point.y;

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
			te.dwHoverTime = hovertime;
		
			::_TrackMouseEvent(&te);
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
				fired = capped = false;
				}
			}
		}

	if(entered.IsFirstTime())
		{
		//entered = true;
		OnMouseEnter();
		}

	CWnd::OnMouseMove(nFlags, point);
}

BOOL CBaseWnd::PreTranslateMessage(MSG* pMsg) 

{
	//if(!mxpad.is_junk(pMsg->message))
	//	P2N("CBaseWnd::PreTranslateMessage  %s\r\n", mxpad.num2msg(pMsg->message));

	//////////////////////////////////////////////////////////////////////
	// These messages never got to the window, send it to our unified handler
	
	if(pMsg->message == WM_KEYDOWN)
		{
		//OnChar(pMsg->wParam, LOWORD(pMsg->lParam), pMsg->lParam); 		
		return KeyEvent(WM_KEYDOWN, pMsg->wParam, pMsg->lParam);
		}
	if(pMsg->message == WM_KEYUP)
		{
		return KeyEvent(WM_KEYUP, pMsg->wParam, pMsg->lParam);
		}
	if(pMsg->message == WM_CHAR)
		{
		//OnChar(pMsg->wParam, LOWORD(pMsg->lParam), pMsg->lParam); 
		return KeyEvent(WM_CHAR, pMsg->wParam, pMsg->lParam);
		}

	// -------------------------------------------------------------------

	if(pMsg->message == WM_SYSCHAR)
		{
		//OnSysChar(pMsg->wParam, LOWORD(pMsg->lParam), pMsg->lParam); 
		return KeyEvent(WM_SYSCHAR, pMsg->wParam, pMsg->lParam);
		}
	if(pMsg->message == WM_SYSKEYDOWN)
		{
		//OnSysKeyDown(pMsg->wParam, LOWORD(pMsg->lParam), pMsg->lParam); 
		return KeyEvent(WM_SYSKEYDOWN, pMsg->wParam, pMsg->lParam);
		}
	if(pMsg->message == WM_SYSKEYUP)
		{
		//OnSysKeyUp(pMsg->wParam, LOWORD(pMsg->lParam), pMsg->lParam); 
		return KeyEvent(WM_SYSKEYUP, pMsg->wParam, pMsg->lParam);
		}

	// -------------------------------------------------------------------

	// MFC (4) has no message map enters defined for these
	if(pMsg->message == WM_MOUSELEAVE)
		{
		//P2N("CBaseWnd::Mouse leave message\r\n");
		
		tip.Hide();
		capped = fired = false;

		entered.Reset();
		OnMouseLeave();
		return 0;
		}

	if(pMsg->message == WM_MOUSEHOVER )
		{
		//P2N("Mouse hover message\r\n");

		fired = true;

		OnMouseHover();

		if(capped)
			{
			capmouse.x = lastmousepoint.x;
			capmouse.y = lastmousepoint.y;
			
			TRACKMOUSEEVENT te;

			te.cbSize = sizeof(TRACKMOUSEEVENT);
			te.dwFlags = TME_HOVER | TME_CANCEL ;
			te.hwndTrack = this->m_hWnd;		
			::_TrackMouseEvent(&te);	

			// Find the text for this one
			CString str; OnGetTipText(str);

			if(str != "")
				{
				// Autosize and autolocate
				CPoint screen = lastmousepoint;
				ClientToScreen(&screen); screen.x += 12; screen.y += 24;		
				capped = false;
				//P2N("CBaseWnd::PreTranslateMessage tooltip %s\r\n", str); 
				tip.Show(str, screen);
				}
			}
		}

	return CWnd::PreTranslateMessage(pMsg);
}

////////////////////////////////////////////////////////////////////////
// CStatic 

void CBaseWnd::PreSubclassWindow() 

{
	//P2N("CBaseWnd::PreSubclassWindow %p\r\n", this);

	ModifyStyle(WS_DISABLED | WS_BORDER , 
				WS_CLIPSIBLINGS | SS_NOTIFY , SWP_DRAWFRAME);

	// Clear this so border design does not show
	ModifyStyleEx( WS_EX_STATICEDGE, 0);

	EnableWindow(TRUE);

	// Send initization message
	OnInitControl();
	
	CWnd::PreSubclassWindow();
}

void CBaseWnd::OnPaint() 

{
	//P2N("CBaseWnd::OnPaint\r\n");
	if(firstpaint)
		{
		firstpaint = 0;
		OnFirstPaint();
		//EnableWindow(TRUE);   // Moved to presubclass ...
		}

	CPaintDC dc(this); // device context for painting

	// Test if we can draw here
	//if(GetFocus() == this)
	//	{
	//	dc.LineTo(5,0);
	//	}
	//else
	//	{
	//	dc.LineTo(5,5);
	//	}

	// In your derived class do:
	//
	// CBaseWnd::OnPaint();
	//
	// or use CPaintDC instead of CClientDC to clear invalidated areas
	// (otherwisw windows keeps sending paint messages
	//
}


void CBaseWnd::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	//P2N("CBaseWnd::OnChar %d %c\r\n", nChar, nChar);	
	CWnd::OnChar(nChar, nRepCnt, nFlags);
}

void CBaseWnd::OnSysChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	//P2N("CBaseWnd::OnSysChar %d %c\r\n", nChar, nChar);	
	CWnd::OnSysChar(nChar, nRepCnt, nFlags);
}

// Virtual function to init control
// Called after create before first paint

void CBaseWnd::OnInitControl()
{
	//tip.Create(IDD_DIALOG12, this);
	//P2N("CBaseWnd::InitControl\r\n");
}

// Virtual function to fill tooltip
// Called when tip needs a text

void CBaseWnd::OnGetTipText(CString &str)
{
	//P2N("CBaseWnd::OnGetTipText\r\n");
}

// Virtual function to ..

void CBaseWnd::OnMouseLeave()
{
	//P2N("CBaseWnd::OnMouseLeave()\r\n");
}

// Virtual function to ..

void CBaseWnd::OnMouseEnter()
{
	//P2N("CBaseWnd::OnMouseEnter()\r\n");
}

// Virtual function to ..

void CBaseWnd::OnMouseHover()

{
	//P2N("CBaseWnd::OnMouseHover()\r\n");
}

void CBaseWnd::OnFirstPaint()

{
	//P2N("CBaseWnd::OnFirstPaint()\r\n");
}

void CBaseWnd::InvalidateParent()

{
	RECT prc; GetWindowRect(&prc);

	GetParent()->ScreenToClient(&prc);
	GetParent()->InvalidateRect(&prc); 
}

void CBaseWnd::InvalidateParentRect(const RECT *rc)

{
	// Operate on a copy
	RECT prc; CopyRect(&prc, rc);

	ClientToScreen(&prc);
		
	GetParent()->ScreenToClient(&prc);
	GetParent()->InvalidateRect(&prc); 
}

void CBaseWnd::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 

{
	CWnd::OnActivate(nState, pWndOther, bMinimized);
	//P2N("CBaseWnd::OnActivate %d\r\n", nState);		
}

int CBaseWnd::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message) 

{
	//P2N("CBaseWnd::OnMouseActivate\r\n");	
	SetFocus(); 
	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

void CBaseWnd::OnSetFocus(CWnd* pOldWnd) 

{
	CWnd::OnSetFocus(pOldWnd);
	OnActivate(WA_ACTIVE, pOldWnd, false);
	InvalidateParent();

	//P2N("CBaseWnd::OnSetFocus\r\n");		
}

void CBaseWnd::OnKillFocus(CWnd* pNewWnd) 

{
	InvalidateParent();
	OnActivate(WA_INACTIVE, pNewWnd, false);
	CWnd::OnKillFocus(pNewWnd);
	
	//P2N("CBaseWnd::OnKillFocus\r\n");		
}

//////////////////////////////////////////////////////////////////////////
// In the derived class return true to stop key processing 
// (line in PreTranslateMessage)
//

int CBaseWnd::KeyEvent(UINT event, UINT nChar, UINT nFlags)

{
	//P2N("KeyEvent(UINT event=%d, UINT nChar=%d, UINT nFlags=%d\r\n",
	//										event, nChar, nFlags);
	return 0;
}

