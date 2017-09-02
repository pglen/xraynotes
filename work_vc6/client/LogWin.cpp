
/* =====[ LogWin.cpp ]========================================== 
                                                                             
   Description:     The timedim project, implementation of the LogWin.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  8/30/2007  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// LogWin.cpp : implementation file
//

#include "stdafx.h"
#include "client.h"
#include "LogWin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogWin

CLogWin::CLogWin()

{
	inited = false;
	created = false;

	//::GetObject((HFONT)GetStockObject(DEFAULT_GUI_FONT),sizeof(m_lf),&m_lf);
	::GetObject((HFONT)GetStockObject(ANSI_FIXED_FONT),sizeof(m_lf),&m_lf);
	
	m_font.CreateFontIndirect(&m_lf);

	maxlines = DEFAULT_MAX_LINES;
	lines = 0;
	in_timer = 0;
	prevnl = false;
	bgcolor = RGB(245, 245, 245);
}

CLogWin::~CLogWin()

{
//	P2N("Logwind destructor called\r\n");
}

BEGIN_MESSAGE_MAP(CLogWin, CWnd)
	//{{AFX_MSG_MAP(CLogWin)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_HSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_CHAR()
	ON_WM_VSCROLL()
	ON_WM_RBUTTONDOWN()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_MOUSEWHEEL()
	ON_WM_KEYDOWN()
	ON_WM_SYSKEYDOWN()
	ON_WM_ACTIVATE()
	ON_WM_MOUSEACTIVATE()
	ON_WM_GETDLGCODE()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogWin message handlers

void CLogWin::AddLine(const char *str)

{
	AddString(str);
	AddString("\r\n");
}

void CLogWin::AddString(const char *str)

{
	CString newstr(str);
	
	int hasnewline = false;
	if(newstr.Find("\r\n") >= 0 )
		{
		hasnewline = true;
		newstr.Replace("\r\n", " ");
		}

	if(prevnl)
		{
		// Add to end
		strlist.AddTail(newstr);
		}
	else
		{
		// Append to current last
		if(!strlist.IsEmpty())
			{
			strlist.GetTail() += newstr;
			}
		else
			{
			strlist.AddTail(newstr);
			}
		}

	prevnl = hasnewline;

	//P2N("Added new str %p %s\r\n", newstr, *newstr);

	int cnt = strlist.GetCount();
	SetScrollRange(SB_VERT, 0, cnt);
	SetScrollPos(SB_VERT, cnt);
	
	int slen = newstr.GetLength();

	if(slen > 100)
		{
		//P2N("Adjusted scroll range %d\r\n", slen);
		SetScrollRange(SB_HORZ, 0, slen + 100);
		}

	// Pop first entry
	if(strlist.GetCount() > maxlines)
		{
		//CString &old = 
		strlist.RemoveHead();
		//delete	&old;
		}

	// Set refresh timer
	if(!in_timer)
		{
		in_timer = true;
		SetTimer(1, 300, NULL);
		}

	//delete newstr;
}

//////////////////////////////////////////////////////////////////////////

void CLogWin::Clear()

{
	POSITION pos = strlist.GetHeadPosition();
	while(pos)
		{
		CString &str = strlist.GetNext(pos);
		//P2N("In Clear() %s\r\n",  str);
		}

	strlist.RemoveAll();
	Invalidate();
}

//////////////////////////////////////////////////////////////////////////

void CLogWin::OnPaint() 

{
	CPaintDC dc(this); // device context for painting
	
	//P2N("CLogWin::Paint\r\n");

	int smin, smax;
	GetScrollRange(SB_VERT, &smin, &smax);

	int scroll =   GetScrollPos(SB_HORZ);
	int vscroll = smax - GetScrollPos(SB_VERT);

	if(!inited)
		{
		inited = true;
				
		//ModifyStyle(0 /*WS_DISABLED*/, WS_HSCROLL | WS_VSCROLL | WS_CLIPSIBLINGS | WS_CHILD );

		//WINDOWPLACEMENT wp;
		//GetWindowPlacement(&wp); 

		//wp.rcNormalPosition.right -= 1;	SetWindowPlacement(&wp);
		//wp.rcNormalPosition.right += 1; SetWindowPlacement(&wp);

		//EnableWindow(m_hWnd, true);
		//EnableScrollBarCtrl(SB_HORZ, true);
		//EnableScrollBarCtrl(SB_VERT, true);

		//EnableScrollBarCtrl(SB_BOTH, true);

		//ShowScrollBar(SB_BOTH, true);
	
		// CWnd

		//WNDPROC oldWndProc = (WNDPROC)SetWindowLong(m_hWnd, GWL_WNDPROC,
		//			(DWORD)afxWndProc);

		//ASSERT(oldWndProc != NULL);

		//int proc = (int)GetSuperWndProcAddr();
		//HWND hh = m_hWnd;
		//UnsubclassWindow();
		//SubclassWindow(hh );
		}

	RECT	rc;
	GetClientRect(&rc);
	
	//dc.SetBoundsRect(&rc, DCB_ACCUMULATE);

	dc.SetBkMode(TRANSPARENT);
	CFont* oldFont = dc.SelectObject(&m_font);

	int xx = 0, yy = 0;
	CSize ss = dc.GetTextExtent("A");
		
	// This many lines to go back ....
	int back = max((rc.bottom - rc.top) / ss.cy, 0);

	lines = back;
	//P2N("bak %d\r\n", back);
	
	POSITION pos = strlist.GetTailPosition();

	back += vscroll;
	
	//back = min(back, strlist.GetCount());

	while(pos)
		{
		back--;

		if(!back)
			break;
		
		strlist.GetPrev(pos);
		}

	if(pos <= 0)
		pos = strlist.GetHeadPosition();

	while(pos)
		{
		CString &str = strlist.GetNext(pos);

		int scr = min(scroll, str.GetLength() );
		
		CString str2 = str.Mid(scr);

		dc.TextOut(xx, yy, str2);
		//dc.ExtTextOut(xx, yy, ETO_CLIPPED, &rc, *str, NULL);
		yy += ss.cy;
		}

	dc.SelectObject(oldFont);
}

//////////////////////////////////////////////////////////////////////////

void CLogWin::OnDestroy() 

{
	CWnd::OnDestroy();
	
	//P2N("CLogWin::Destroying\r\n");	

#if 0
	while(true)
		{
		if(strlist.GetCount() == 0)
			break;

		CString &str = strlist.RemoveHead();

		//P2N("String is: %s\r\n", str);
		//delete &str;
		}
#endif


#if 0
	//P2N("Called destructor\r\n");

	POSITION pos = strlist.GetHeadPosition();
	while(true)
		{
		if(!pos)
			break;

		CString &str = strlist.GetNext(pos);
		//P2N("In destructor %s\r\n",  str);
		//delete str;
		}
#endif

}

void CLogWin::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 

{
	//P2N("CLogWin::Hor Scroll code=%d pos=%d  bar=%p\r\n", nSBCode, nPos, pScrollBar);	

	switch(nSBCode)
	{
	case SB_LINELEFT   :
		SetScrollPos(SB_HORZ, GetScrollPos(SB_HORZ)-1);
		Invalidate();
		break;

	case SB_LINERIGHT   :
		SetScrollPos(SB_HORZ, GetScrollPos(SB_HORZ)+1);
		Invalidate();
		break;

	case SB_PAGELEFT    :
		SetScrollPos(SB_HORZ, GetScrollPos(SB_HORZ)-10);
		Invalidate();
		break;

	case SB_PAGERIGHT:
		SetScrollPos(SB_HORZ, GetScrollPos(SB_HORZ)+10);
		Invalidate();
		break;

	case SB_THUMBPOSITION      :
		SetScrollPos(SB_HORZ, nPos);
		Invalidate();
		break;

	case SB_THUMBTRACK      :
		SetScrollPos(SB_HORZ, nPos);
		Invalidate();
		break;
	}

	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

//////////////////////////////////////////////////////////////////////////

BOOL CLogWin::PreTranslateMessage(MSG* pMsg) 

{
	//P2N("CLogWin::Pretranslage %d %s\r\n", pMsg->message, mxpad.num2msg(pMsg->message));
	return CWnd::PreTranslateMessage(pMsg);
}

BOOL CLogWin::PreCreateWindow(CREATESTRUCT& cs) 
{
	//P2N("Precreatewindow\r\n");
	//cs.style |= WS_HSCROLL;
	return CWnd::PreCreateWindow(cs);
}

LRESULT CLogWin::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	//if(!mxpad.is_junk(message))
	//	P2N("CLogWin::Window proc %s\r\n", mxpad.num2msg(message));

	return CWnd::WindowProc(message, wParam, lParam);
}

void CLogWin::OnLButtonDown(UINT nFlags, CPoint point) 

{
	//	EnableScrollBarCtrl(SB_HORZ, true);
	//EnableScrollBarCtrl(SB_VERT, true);

	//P2N("OnLButtonDown\r\n");

	//SetScrollPos( SB_VERT, 20);
	//SetScrollPos( SB_HORZ, 20);
		
	CWnd::OnLButtonDown(nFlags, point);
}

void CLogWin::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 

{
	//P2N("CLogWin::OnChar() chh=%c\r\n", nChar);
	CWnd::OnChar(nChar, nRepCnt, nFlags);
}


void CLogWin::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 

{
	//P2N("CLogWin::OnVScroll\r\n");
		switch(nSBCode)
	{
	case SB_LINEDOWN   :
		SetScrollPos(SB_VERT, GetScrollPos(SB_VERT)+1);
		Invalidate();
		break;

	case SB_LINEUP   :
		SetScrollPos(SB_VERT, GetScrollPos(SB_VERT)-1);
		Invalidate();
		break;

	case SB_PAGEDOWN    :
		SetScrollPos(SB_VERT, GetScrollPos(SB_VERT)+10);
		Invalidate();
		break;

	case SB_PAGEUP :
		SetScrollPos(SB_VERT, GetScrollPos(SB_VERT)-10);
		Invalidate();
		break;

	case SB_THUMBPOSITION      :
		SetScrollPos(SB_VERT, nPos);
		Invalidate();
		break;

	case SB_THUMBTRACK      :
		SetScrollPos(SB_VERT, nPos);
		Invalidate();
		break;
	}
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CLogWin::OnRButtonDown(UINT nFlags, CPoint point) 
{
	//P2N("OnRButtonDown() \r\n");
	CWnd::OnRButtonDown(nFlags, point);
} 

void CLogWin::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	//P2N("CLogWin::OnShowWindow\r\n");
	
	if(!created)
		{
		created = true;

		//SetTimer(1, 100, NULL);
		//return;
		}
	SetWindowPos(&wndTop, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
	CWnd::OnShowWindow(bShow, nStatus);
}

void CLogWin::OnTimer(UINT nIDEvent) 

{		
	if(nIDEvent == 1)
		{
		KillTimer(nIDEvent);
		in_timer = false;
		Invalidate();
		}

	//ASSERT(IsWindow(AfxGetApp()->m_pMainWnd->m_hWnd));
	
	//ShowWindow(true);
	
	CWnd::OnTimer(nIDEvent);
}

int CLogWin::OnCreate(LPCREATESTRUCT lpCreateStruct) 

{
	//P2N("Intercepted Create\r\n");

	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

void CLogWin::PreSubclassWindow() 
{
	//P2N("CLogWin::intercepted pre subluss\r\n");

	ModifyStyle(0, SS_NOTIFY);
	ModifyStyle(0, WS_CLIPSIBLINGS);
	//ModifyStyleEx( WS_EX_STATICEDGE, 0);

	CWnd::PreSubclassWindow();
}

//////////////////////////////////////////////////////////////////////////
// 

//////////////////////////////////////////////////////////////////////////
//
// Function name:    CLogWin::CreateFrom
// Description:      <empty description>
// Return type:      void 
// Argument:         CWnd *wnd		== parent window
// Argument:         int id			== control to model after
//
//////////////////////////////////////////////////////////////////////////

void CLogWin::CreateFrom(CWnd *wnd, int id)

{
	//P2N("Create from %p %d\r\n", wnd, id);

	ASSERT(wnd);

	CWnd *me = wnd->GetDlgItem(id);
	ASSERT(me);
	
	me->ShowWindow(false);

	RECT wrc;
	me->GetWindowRect(&wrc);
	wnd->ScreenToClient(&wrc);
	
	//Create(NULL, "Log Window", WS_HSCROLL | WS_VSCROLL | WS_CHILD| WS_DLGFRAME  , wrc, wnd, 0);
	CreateEx( WS_EX_CLIENTEDGE  , NULL, NULL,   WS_HSCROLL | WS_VSCROLL | 
						/*WS_TABSTOP  | */ WS_CHILD, wrc, wnd, 1);

	//WINDOWPLACEMENT wp;
	//GetWindowPlacement(&wp); 
	//wp.rcNormalPosition.right -= 1;	SetWindowPlacement(&wp);
	//wp.rcNormalPosition.right += 1; SetWindowPlacement(&wp);


	SetScrollRange(SB_HORZ, 0, 100);
	SetScrollRange(SB_VERT, 0, 1);
	SetScrollPos(SB_VERT, 0);

	EnableScrollBarCtrl(SB_HORZ, false);
	EnableScrollBarCtrl(SB_VERT, false);

	ShowScrollBar(SB_BOTH);

	//EnableScrollBarCtrl(SB_HORZ, false);
	//EnableScrollBarCtrl(SB_VERT, false);

	ShowWindow(true);
}

BOOL CLogWin::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 

{
	if(zDelta > 0)
		SetScrollPos(SB_VERT, GetScrollPos(SB_VERT) + 10);
	else
		SetScrollPos(SB_VERT, GetScrollPos(SB_VERT) - 10);

	Invalidate();

	//P2N("OnMouseWheel() \r\n");

	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void CLogWin::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{

	//P2N("CLogWin::OnKeyDown() %c\r\n", nChar);
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CLogWin::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult) 
{
//	P2N("Child notify %s\r\n", mxpad.num2msg(message));
	return CWnd::OnChildNotify(message, wParam, lParam, pLResult);
}

void CLogWin::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	//P2N("CLogWin::OnSysKeyDown() %d\r\n", nChar);
	
	CWnd::OnSysKeyDown(nChar, nRepCnt, nFlags);
}

// WM_ACTIVATE

void CLogWin::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 

{
	//P2N("CLogWin::Activate window\r\n");

	if(nState != WA_INACTIVE)
		{
		SetFocus();
		}

	//CWnd::OnActivate(nState, pWndOther, bMinimized);
}

int CLogWin::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message) 

{
	//P2N("Mouse Activate\r\n");
	
	SetFocus();	
	//Invalidate();
	return MA_ACTIVATE;
	//return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

//////////////////////////////////////////////////////////////////////////

UINT CLogWin::OnGetDlgCode() 

{
	// WM_GETDLGCODE
	return DLGC_WANTMESSAGE;
}

BOOL CLogWin::OnEraseBkgnd(CDC* pDC) 
{
	RECT	rc;
	GetClientRect(&rc);

	//P2N("CLogWin::Erase Dialog\r\n");

	pDC->FillSolidRect(&rc, bgcolor);

	return 1;
	//return CWnd::OnEraseBkgnd(pDC);
}

LRESULT CLogWin::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 

{
	//if(!mxpad.is_junk(message))
	//	P2N("Default window procedure %s\r\n", mxpad.num2msg(message));

	return CWnd::DefWindowProc(message, wParam, lParam);
}

void CLogWin::OnMouseMove(UINT nFlags, CPoint point) 

{
	//P2N("CLogWin::OnMouseMove(x=%d y=%d)\r\n", point.x, point.y);	
	CWnd::OnMouseMove(nFlags, point);
}

BOOL CLogWin::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	//P2N("on notify\r\n");
	return CWnd::OnNotify(wParam, lParam, pResult);
}
