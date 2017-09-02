// vSplit.cpp : implementation file
//

#include "stdafx.h"
#include "xcal.h"
#include "vSplit.h"

#include "mxpad.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CvSplit

CvSplit::CvSplit()

{	
	in_border	= 0;
	firstpaint	= true;
	split		= 20;
	wnd1 = 	wnd2 = NULL;
}

CvSplit::~CvSplit()

{

}

BEGIN_MESSAGE_MAP(CvSplit, CxWnd)
	//{{AFX_MSG_MAP(CvSplit)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_CHAR()
	ON_WM_CREATE()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CvSplit message handlers

void CvSplit::OnPaint() 

{
	CPaintDC dc(this); // device context for painting
	
	if(firstpaint)
		{
		P2N("CvSplit::OnPaint firstpaint\r\n");
		firstpaint = false;
		}

	RECT rc; GetClientRect(&rc);

	dc.FillSolidRect(&rc, RGB(198,198,198));

	int sp = (rc.right * split) / 100;
	CPen pen(PS_SOLID, 1, RGB(128, 128, 128));

	//CPen *oldpen = (CPen*)dc.SelectObject(pen);
	// Draw split line
	//dc.MoveTo(sp, 0);
	//dc.LineTo(sp, rc.bottom);

	rc2.top = 2; rc2.left = sp;
	rc2.right = rc2.left + 6; rc2.bottom = rc.bottom - 2;
	
	dc.DrawEdge(&rc2, BDR_RAISEDINNER, BF_RECT);

	//dc.Draw3dRect(sp, 4, 5, rc.bottom - 8, RGB(128, 128, 128), RGB(128, 128, 128));
	//dc.SelectObject(oldpen);

	
	// Do not call CWnd::OnPaint() for painting messages
}

void CvSplit::OnLButtonDown(UINT nFlags, CPoint point) 

{
	P2N("CvSplit::OnLButtonDown\r\n");	

	if(PtInRect( &rc2, point) )
		{
		P2N("CvSplit::OnLButtonDown in point\r\n");	
		in_border = true;
		SetCapture();
		}

	CWnd::OnLButtonDown(nFlags, point);
}

void CvSplit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 

{
	P2N("CvSplit::OnChar\r\n");	
	
	CWnd::OnChar(nChar, nRepCnt, nFlags);
}

int CvSplit::OnCreate(LPCREATESTRUCT lpCreateStruct) 

{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	P2N("CvSplit::OnCreate\r\n");
	
	return 0;
}

void CvSplit::PreSubclassWindow() 

{
	
	ModifyStyleEx(WS_EX_STATICEDGE | WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE, 0);
	ModifyStyle(0, SS_NOTIFY);			// Mouse Messages 
	SetFocus();							// Needed for grabbing input
		
	//P2N("CvSplit::PreSubclassWindow\r\n");
}

BOOL CvSplit::PreTranslateMessage(MSG* pMsg) 

{
	//P2N("CvSplit::PreTranslateMessage\r\n");
#if 1
		//////////////////////////////////////////////////////////////////////
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
#endif

	return CWnd::PreTranslateMessage(pMsg);
}

BOOL CvSplit::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 

{
	
	P2N("CvSplit::OnMouseWheel\r\n");

	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

//////////////////////////////////////////////////////////////////////////

void CvSplit::OnMouseMove(UINT nFlags, CPoint point) 

{
	if(in_border)
		{
		RECT rc; GetClientRect(&rc);
		P2N("in border %d %d\r\n", point.x, point.y);

		point.x = min(point.x, rc.right -1);
		point.x = max(point.x, 1);

		split =  (100 * point.x) / rc.right;
		Invalidate();

		// Send messages to descendents
		if(wnd1)
			wnd1->Invalidate();

		if(wnd2)
			wnd2->Invalidate();
		}

	CWnd::OnMouseMove(nFlags, point);
}

void CvSplit::OnLButtonUp(UINT nFlags, CPoint point) 

{
	ReleaseCapture();
	in_border = false;
	CWnd::OnLButtonUp(nFlags, point);
}
