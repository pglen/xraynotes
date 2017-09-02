// 2dCtl.cpp: implementation of the C2dCtl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "xraynotes.h"
#include "2dCtl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(C2dCtl, CBaseWnd)
	//{{AFX_MSG_MAP(C2dCtl)
	ON_WM_KEYDOWN()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

C2dCtl::C2dCtl()

{
	//P2N("C2dCtl::C2dCtl\r\n");
	bullw = 16;
	xx = yy = capped = 0;
}


C2dCtl::~C2dCtl()
{
	//P2N("C2dCtl::~C2dCtl\r\n");
}

void C2dCtl::OnInitControl()

{
	RECT rc; GetClientRect(&rc);

	//P2N("C2dCtl::OnInitControl\r\n");

	//ModifyStyle(0, WS_CLIPSIBLINGS);

	//ModifyStyle(WS_BORDER | WS_DLGFRAME | WS_THICKFRAME, 0);
	//ModifyStyleEx(WS_EX_DLGMODALFRAME | WS_EX_STATICEDGE | WS_EX_TOOLWINDOW, 0);

	CenterRect(rc.right/2, rc.bottom/2, &bull);
	CopyRect(&sbull, &bull);
}

void C2dCtl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 

{
	RECT rc; GetClientRect(&rc);

	//P2N("C2dCtl::OnKeyDown %d %d\r\n", nChar, nChar);
	
	int diff = 5;
	if(misc.IsShiftDown())
		diff = 15;
	
	switch(nChar)
		{	
		case VK_LEFT:
			if(bull.left > -bullw/2)
				DoMove(bull.left + bullw/2 - diff, bull.top + bullw/2);
			break;

		case VK_RIGHT:
			if(bull.right - bullw/2 < rc.right)
				DoMove(bull.left + bullw/2 + diff, bull.top + bullw/2);
			break;

		case VK_UP:
			if(bull.right - bullw/2 < rc.right)
				DoMove(bull.left +bullw/2, bull.top + bullw/2 - diff);
			break;

		case VK_DOWN:
			if(bull.right - bullw/2 < rc.right)
				DoMove(bull.left +bullw/2, bull.top + bullw/2 + diff);
			break;

		case VK_HOME:
			DoMove(rc.right/2, rc.bottom/2);
			break;
		}

	CBaseWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void C2dCtl::OnPaint() 

{
	CPaintDC dc(this); // device context for painting
	
	RECT rc; GetClientRect(&rc);
	
	//P2N("C2dCtl::OnPaint Bull t=%d l=%d capped=%d\r\n", bull.top, bull.left, capped);

	COLORREF bcolor = RGB(128,128,128);
	COLORREF pcolor = RGB(140,140,140);

	if(GetFocus() == this)
		{
		bcolor = RGB(255, 255, 255);
		pcolor = RGB(12,12,12);
		}

	CBrush *oldb, br(bcolor), br2;
	CPen *oldp, pen(PS_SOLID, 1, pcolor);

	LOGBRUSH lb; 
	lb.lbStyle = BS_NULL;
	lb.lbColor = 0;
	lb.lbHatch = 0;

	br2.CreateBrushIndirect(&lb);

	oldp = dc.SelectObject(&pen);
	oldb = dc.SelectObject(&br2);

	// There is a shadow bull to see intent
	dc.Ellipse(&sbull);

	dc.SelectObject(&pen);
	dc.SelectObject(&br);

	dc.Ellipse(&bull);
	
	dc.DrawFocusRect(&rc);

	dc.SelectObject(oldp);
	dc.SelectObject(oldb);

	dc.MoveTo(2, rc.bottom/2);
	dc.LineTo(5, rc.bottom/2);

	dc.MoveTo(rc.right - 5, rc.bottom/2);
	dc.LineTo(rc.right - 2, rc.bottom/2);

	dc.MoveTo(rc.right/2, rc.top + 2);
	dc.LineTo(rc.right/2, rc.top + 5);

	dc.MoveTo(rc.right/2, rc.bottom - 5);
	dc.LineTo(rc.right/2, rc.bottom - 2);

	// Do not call CBaseWnd::OnPaint() for painting messages
}

void C2dCtl::CenterRect(int xx, int yy, RECT *rc)

{
	rc->left	=	xx - bullw/2;
	rc->top		=	yy - bullw/2;
	rc->right	=	xx  + bullw/2;
	rc->bottom	=	yy + bullw/2;
}

void C2dCtl::CenterRect(POINT *pt, RECT *rc)

{
	CenterRect(pt->x, pt->y, rc);
}

void C2dCtl::OnLButtonDown(UINT nFlags, CPoint point) 

{
	CRect rc(bull);
		
	if(rc.PtInRect(point))
		{	
		capped = true;
		SetCapture();
		GetParent()->PostMessage(CTRL2D_START, GetDlgCtrlID(), xx + (yy << 16));
		}
	else
		{
		DoMove(&point);
		}

	CBaseWnd::OnLButtonDown(nFlags, point);
}

void C2dCtl::OnLButtonUp(UINT nFlags, CPoint point) 

{
	if(capped)
		{
		capped = false;
		ReleaseCapture();	
		GetParent()->PostMessage(CTRL2D_END, GetDlgCtrlID(), xx + (yy << 16));
		}

	CBaseWnd::OnLButtonUp(nFlags, point);
}

void C2dCtl::OnMouseMove(UINT nFlags, CPoint point) 

{
	//RECT rc; GetClientRect(&rc)

	if(capped)
		{	
		DoMove(&point);
		}
	CBaseWnd::OnMouseMove(nFlags, point);
}

void C2dCtl::SetPos(int xxx, int yyy)

{
	if(!IsWindow(m_hWnd)) return;

	RECT rc; 

	GetClientRect(&rc);

	POINT np;

	np.x = xxx * rc.right / 100;
	np.y = yyy * rc.bottom / 100;

	DoMove(&np);
}

void C2dCtl::OnGetTipText(CString &str)
{
	//P2N("C2dCtl::OnGetTipText\r\n");
	
	CRect bu(bull);

	if(bu.PtInRect(lastmousepoint))
		str.Format("x=%d y=%d", xx, yy);
}

void	C2dCtl::DoMove(int xxx, int yyy)

{
	POINT pt; pt.x = xxx; pt.y = yyy;
	DoMove(&pt);
}


void	C2dCtl::DoMove(const POINT *pnp)

{
	RECT rc; GetClientRect(&rc);

	POINT np;

	// Force it in Range 
	np.x = MIN(MAX(pnp->x, 0), rc.right);
	np.y = MIN(MAX(pnp->y, 0), rc.bottom);

	// Union of old and new rectangles
	CRect oldrect(sbull);
	CenterRect(&np, &sbull);
	CRect newrect(sbull);

	// Add the two rectangles
	newrect |= oldrect;

	// Give visual feedback for the new location
	InvalidateParentRect(&newrect);
	support.YieldToWinEx();

	// Send message to parent
	xx = 100 * np.x / rc.right;
	yy = 100 * np.y / rc.bottom;

	GetParent()->PostMessage(CTRL2D_MOVE, GetDlgCtrlID(), xx + (yy << 16));

	// Union of old and new rectangles
	CRect oldrect2(bull);
	CenterRect(&np, &bull);
	CRect newrect2(bull);

	// Add the two rectangles
	newrect2 |= oldrect2;
	InvalidateParentRect(&newrect2);
}


void C2dCtl::OnLButtonDblClk(UINT nFlags, CPoint point) 

{
	GetParent()->PostMessage(CTRL2D_START, GetDlgCtrlID(), xx + (yy << 16));

	DoMove(&point);
	GetParent()->PostMessage(CTRL2D_END, GetDlgCtrlID(), xx + (yy << 16));

	CBaseWnd::OnLButtonDblClk(nFlags, point);
}
