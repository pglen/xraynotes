
/* =====[ Triangle.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the Triangle.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  11/25/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// Triangle.cpp: implementation of the CTriangle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "xraynotes.h"
#include "Triangle.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CTriangle

CTriangle::CTriangle()

{
	trisize = 10;

	//    /\
	//   /- \

	arr[0].x =  0;			arr[0].y = trisize;
	arr[1].x =  trisize/2;	arr[1].y = 0;
	arr[2].x =  trisize;	arr[2].y = trisize;

	//   \ - /
	//    \ /

	uarr[0].x =  0;			uarr[0].y = 0;
	uarr[1].x =  trisize/2;	uarr[1].y = trisize;
	uarr[2].x =  trisize;	uarr[2].y = 0;

	//P2N("CTriangle::CTriangle Constructing triangle\r\n");

	Reset();

	rightcap = leftcap = false;
	erase	= false;
	upside	= false;
	tiptext = "Triangle Range Control";
}

CTriangle::~CTriangle()
{
	//P2N("CTriangle::~CTriangle\r\n");
}

BEGIN_MESSAGE_MAP(CTriangle, CBaseWnd)
	//{{AFX_MSG_MAP(CTriangle)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_CHAR()
	ON_WM_SYSCHAR()
	ON_WM_SYSKEYDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTriangle message handlers

void CTriangle::OnLButtonDown(UINT nFlags, CPoint point) 

{
	//P2N("CTriangle::OnLButtonDown\r\n");
	
	RECT rc; GetClientRect(&rc); rc.right -= arr[2].x;

	// Hittest on the triangles
	CRect rc5, rc6;

	rc5.left	= pos1 * rc.right / 255;
	rc5.right	= rc5.left + arr[2].x;
	rc5.top		= arr[1].y;
	rc5.bottom	= arr[2].y;

	rc6.left	= pos2 * rc.right / 255;;
	rc6.right	= rc6.left + arr[2].x;
	rc6.top		= arr[1].y;
	rc6.bottom	= arr[2].y + arr[2].y;

	if(rc5.PtInRect(point))
		{
		//P2N("Hit in left\r\n");
		leftcap = true;
		SetCapture();
		}
	else if(rc6.PtInRect(point))
		{
		//P2N("Hit in right\r\n");
		rightcap = true;
		SetCapture();
		}
	CBaseWnd::OnLButtonDown(nFlags, point);
}

//DEL void CTriangle::PreSubclassWindow() 
//DEL 
//DEL {
//DEL 	//P2N("CTriangle::PreSubclassWindow\r\n");
//DEL 	CBaseWnd::PreSubclassWindow();
//DEL }

void CTriangle::OnPaint() 

{
	CClientDC dc(this);	
	
	//P2N("CTriangle::OnPaint\r\n");

	RECT rc; GetClientRect(&rc); rc.right -= arr[2].x;
	
	POINT *parr;

	if(upside)
		{
		parr = uarr;
		}
	else
		{
		parr = arr;
		}
	
	for(int loop = 0; loop < 3; loop++)
		{
		arr1[loop].y = parr[loop].y;
		arr2[loop].y = parr[loop].y;

		arr1[loop].x = parr[loop].x + pos1 * rc.right / 255;
		arr2[loop].x = parr[loop].x + pos2 * rc.right / 255;
		}
	
	// Draw the triangles on the ends
	if(rightcap)
		{
		dc.Polygon(arr1, 3); 
		dc.Polygon(arr2, 3);
		}
	else
		{
		dc.Polygon(arr2, 3);
		dc.Polygon(arr1, 3); 
		}
	
	CBaseWnd::OnPaint();
}

//DEL BOOL CTriangle::PreTranslateMessage(MSG* pMsg) 
//DEL 
//DEL {
//DEL 	//P2N("CTriangle::PreTranslateMessage %p %s\r\n", this, mxpad.num2msg(pMsg->message));
//DEL 	return CBaseWnd::PreTranslateMessage(pMsg);
//DEL }

void CTriangle::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 

{
	//P2N("CTriangle::OnKeyDown %d %d\r\n", nChar, nChar);
	CBaseWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CTriangle::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	//P2N("CTriangle::OnKeyUp %d %d\r\n", nChar, nChar);
	
	CBaseWnd::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CTriangle::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 

{
	//P2N("CTriangle::OnChar %d %d\r\n", nChar, nChar);
	CBaseWnd::OnChar(nChar, nRepCnt, nFlags);
}

void CTriangle::OnSysChar(UINT nChar, UINT nRepCnt, UINT nFlags) 

{
	//P2N("CTriangle::OnSysChar %d %d\r\n", nChar, nChar);
	CBaseWnd::OnSysChar(nChar, nRepCnt, nFlags);
}

void CTriangle::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 

{
	//P2N("CTriangle::OnSysKeyDown %d %d\r\n", nChar, nChar);	
	CBaseWnd::OnSysKeyDown(nChar, nRepCnt, nFlags);
}

void CTriangle::OnMouseMove(UINT nFlags, CPoint point) 

{
	//P2N("CTriangle::OnMouseMove x=%d y=%d\r\n", point.x, point.y);

	if(leftcap)
		{
		RECT rc; GetClientRect(&rc); 
		RECT prc; GetWindowRect(&prc);

		GetParent()->ScreenToClient(&prc);
		GetParent()->InvalidateRect(&prc); 

		rc.right -= arr[2].x;

		pos1 = max(0, min(255, 255 * (point.x - arr[1].x) / rc.right));

		pos1 = min(pos1, pos2 - 5);
		erase = false; 
		
		Invalidate();

		//P2N("Left cap in %d\r\n", pos1); 
		GetParent()->PostMessage(WM_TRICHANGE1, GetDlgCtrlID() , pos1);
		}

	if(rightcap)
		{
		RECT rc; GetClientRect(&rc); 
		RECT prc; GetWindowRect(&prc);

		GetParent()->ScreenToClient(&prc);
		GetParent()->InvalidateRect(&prc); 

		rc.right -= arr[2].x;
		pos2 = max(0, min(255, 255 * (point.x - arr[1].x) / rc.right));
		pos2 = max(pos1 + 5, pos2);

		Invalidate();

		//P2N("Right cap in %d\r\n", pos2);
		GetParent()->PostMessage(WM_TRICHANGE2, GetDlgCtrlID() , pos2);
		}

	CBaseWnd::OnMouseMove(nFlags, point);
}

void CTriangle::OnLButtonUp(UINT nFlags, CPoint point) 

{
	//P2N("CTriangle::OnLButtonUp\r\n");

	if(leftcap)
		GetParent()->PostMessage(WM_TRIREL1, GetDlgCtrlID() , pos1);

	if(rightcap)
		GetParent()->PostMessage(WM_TRIREL2, GetDlgCtrlID() , pos2);

	rightcap = leftcap = false;
	ReleaseCapture();	
	CBaseWnd::OnLButtonUp(nFlags, point);
}

void CTriangle::OnInitControl()

{
	RECT rc; GetClientRect(&rc); 
	
	//P2N("CTriangle::OnInitControl %p ww=%d hh=%d\r\n", this, rc.right, rc.bottom);

	CBaseWnd::OnInitControl();
}

void CTriangle::OnGetTipText(CString &str)

{
	RECT rc; GetClientRect(&rc); rc.right -= arr[2].x;
	POINT point = lastmousepoint;

	// Hittest on the triangles
	CRect rc5, rc6;

	rc5.left	= pos1 * rc.right / 255;
	rc5.right	= rc5.left + arr[2].x;
	rc5.top		= arr[1].y;
	rc5.bottom	= arr[2].y;

	rc6.left	= pos2 * rc.right / 255;;
	rc6.right	= rc6.left + arr[2].x;
	rc6.top		= arr[1].y;
	rc6.bottom	= arr[2].y + arr[2].y;

	if(rc5.PtInRect(point))
		{
		str.Format("%d", pos1);
		}
	else if(rc6.PtInRect(point))
		{
		str.Format("%d", pos2);
		}
	else
		{
		str = tiptext;
		}
}

void CTriangle::OnMouseLeave()

{
	P2N("CTriangle::OnMouseLeave \r\n");
	//ReleaseCapture();
}

void CTriangle::OnMouseEnter()

{
	P2N("CTriangle::ONMouseEnter \r\n");
}


void CTriangle::Reset()
{
	pos1 = 0; pos2 = 255;
}
