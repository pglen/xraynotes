// xWnd.cpp : implementation file
//

#include "stdafx.h"
#include "xcal.h"
#include "xWnd.h"
#include "mxpad.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CxWnd

CxWnd::CxWnd()
{
	firstpaint	= true;
}

CxWnd::~CxWnd()
{
}


BEGIN_MESSAGE_MAP(CxWnd, CWnd)
	//{{AFX_MSG_MAP(CxWnd)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CxWnd message handlers

void CxWnd::PreSubclassWindow() 

{
	ModifyStyleEx(WS_EX_STATICEDGE | WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE, 0);
	ModifyStyle(0, SS_NOTIFY);			// Mouse Messages 
	SetFocus();			
	CWnd::PreSubclassWindow();
}

BOOL CxWnd::PreTranslateMessage(MSG* pMsg) 

{
	// Dispatch messages ourselvs
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
	return CWnd::PreTranslateMessage(pMsg);
}

void CxWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	if(firstpaint)
		{
		P2N("CxWnd::OnPaint firstpaint\r\n");
		firstpaint = false;
		}

	// Do not call CWnd::OnPaint() for painting messages
}
