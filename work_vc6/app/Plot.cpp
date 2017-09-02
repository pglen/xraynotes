// Plot.cpp : implementation file
//

#include "stdafx.h"
#include "xraynotes.h"
#include "shades.h"
#include "shadesdlg.h"

#include "Plot.h"

#include "mxpad.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlot

CPlot::CPlot()

{
	inited = false;
	
	old_max = 0;
	scroll = 0;
	mag = 0;
	step = 0;
	lastmove = 0;
	ytimer = 0;

	lastclock = clock();
}


CPlot::~CPlot()
{
}


BEGIN_MESSAGE_MAP(CPlot, CStatic)
	//{{AFX_MSG_MAP(CPlot)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlot message handlers

void CPlot::OnPaint() 

{
	double loop;

	CPaintDC dc(this); // device context for painting

	RECT rc; GetClientRect(&rc);

	if(!inited)
		{
		inited = true;

		dc.FillSolidRect(&rc, RGB(220,220,220));
		dc.Draw3dRect(&rc, RGB(200,200,200), RGB(100,100,100));

		SetTimer(1, 200, NULL);
		}

	//////////////////////////////////////////////////////////////////////
	// Move to ad draw

	//dc.MoveTo(rc.left, rc.bottom - 3);
	//dc.LineTo(rc.right, rc.bottom - 3);
	
	int ss = int_arr.GetSize();
	int at2 = 0, at3 = 0;

	// Estabilish range
	for(loop = 0; loop < ss; loop++)
		{
		int xx = int_arr.GetAt((int)loop);

		at2 = max(at2, abs(xx));
		//at3 = min(at3, xx);
		}

	//P2N("max at2=%d\r\n", at2);
	//P2N("min at3=%d\r\n", at3);

	//if(at2 > 0 || at3 < 0)
		{	
		//dc.SetBkMode(0);
		dc.SetBkColor(RGB(220,220,220));
		CString str; str.Format(
			"Left drag to scroll, Right drag to magnify. Left dblclick to Reset, Right dblclick to Fit. "
			"maxval=%d - scroll=%d items=%d", at2, scroll, ss);
		dc.TextOut(2, 2, str);

		//str.Format("min=%06d", at3);
		//dc.TextOut(5, rc.bottom - 18, str);
		}
		
	if(at2 == 0) at2 = 1;
	//if(at3 == 0) at3 = 1;

	//if(at2 > old_max)
	//	{
	//	Invalidate();
	old_max = at2;
	//	}

	
	///////////////////////////////////////////////

	double fact =  ((rc.bottom - rc.top - 44) / 2);
	double mag2 = 1 + mag/100;

	if(mag2 < 0.002) 
		mag2 = 0.002;

	int offset = 0;

	//dc.MoveTo(rc.left + offset + 5, rc.bottom / 2 + 18);

	// Preset 
	if(ss)
		{
		int atp, zzp = int_arr.GetAt((int)scroll);
		atp = (int)((fact * zzp)  / at2);
		dc.MoveTo(rc.left + offset + 5, rc.bottom / 2 - atp + 18);
		}

	for(loop = scroll; loop < ss; loop += mag2)
		{
		int at, zz = int_arr.GetAt((int)loop);

		at = (int)((fact * zz)  / at2);

		//if(zz > 0)
		//else
		//	at = (fact * zz)  / abs(at3);
		//P2N("Get zz=%d -- at=%d\r\n", zz, at);
		//if(rc.left + loop + 5 > rc.right)
		//	{
		//	dc.TextOut(5, 5, "Training Plot (overflow)");
		//	break;
		//	}

		// Draw line
		if(rc.left + offset + 5 < rc.right)
			{
			//dc.MoveTo(rc.left + offset + 5, rc.bottom / 2);
			dc.LineTo(rc.left + offset + 5, rc.bottom / 2 - at + 18);
			}
		else
			{
			// End of window
			break;
			}

		offset++;
		}
}

//////////////////////////////////////////////////////////////////////////

void CPlot::AddLine(int hight)

{
	//RECT rc; GetClientRect(&rc);
	
	//P2N("line %d\r\n", hight);

	//if(hight > rc.bottom - 15)
	//	{
	//	hight = rc.bottom - 15;
	//	}

	//if(int_arr.GetSize() > (rc.right - 9))
	//	{
	//	int_arr.RemoveAt(0, 20);
	//	inited = false;
	//	}

	if(!changed)
		{
		if(::IsWindow(m_hWnd))
			SetTimer(1, 200, NULL);
		}
	
	int_arr.Add(hight);
	changed = true;

	//int asize = int_arr.GetSize();
	//if(asize > 10000)
	//	mag = 10;

	//WINDOWPLACEMENT wp;
	//GetWindowPlacement(&wp);
	
	//int ww = rc.right - rc.left;

	//if(asize > ww + 40 + scroll)
	//	{
	//	//P2N("Added scroll %d\r\n", scroll);
	//	scroll = asize - ww;
	//	Invalidate();
	//	YieldToWin();
	//	}

	// Self invalidate after some work
	//if(asize % 2000 == 0)
	//	{
	//	Invalidate();
	//	YieldToWin();
	//	}
}

BOOL CPlot::OnEraseBkgnd(CDC* pDC) 

{
	//P2N("Erase background\r\n");

	RECT rc; GetClientRect(&rc);

	pDC->FillSolidRect(&rc, RGB(220,220,220));
	pDC->Draw3dRect(&rc, RGB(200,200,200), RGB(100,100,100));

	//pDC->TextOut(5, rc.bottom - 18, "aaaa");
	//inited = true;
	//return CStatic::OnEraseBkgnd(pDC);

	return(true);
}

void CPlot::AddMarker()

{
	AddLine(0);
	AddLine(0);
	AddLine(0);
	AddLine(old_max);
	AddLine(-old_max);
	AddLine(0);
	AddLine(0);
	AddLine(0);
	AddLine(0);
}

void CPlot::ClearAll()

{
	inited = false;
	int_arr.RemoveAll(); 

	Invalidate(false);
	support.YieldToWinEx();
}

void CPlot::OnMouseMove(UINT nFlags, CPoint point) 
{
	//P2N("Scroll by: %d old=%d\r\n", point.x, point.y);


	if(MK_RBUTTON & nFlags )
		{
		//P2N("Scroll by: %d old=%d  diff=%d  ", old.x, point.x, point.x - old.x);

		if(MK_SHIFT  & nFlags )	
			{
			mag -= (point.x - old.x) * 20;
			}
		else
			{
			mag -= (point.x - old.x) * 2;
			}
		
		//P2N("Mag=%f\r\n", 1 + mag/100);
			
		Invalidate();
		}

	if(MK_LBUTTON & nFlags )
		{
		if(MK_SHIFT  & nFlags )	
			{	
			scroll -= (point.x - old.x) * 200;
			}
		else
			{
			scroll -= (point.x - old.x) * 20;
			}

		if(scroll < 0)
			scroll = 0;

		if(scroll > int_arr.GetSize() - 1)
			scroll = int_arr.GetSize() - 1;

		//P2N("Scroll=%d\r\n", scroll);
		Invalidate();
		}

	old.x = point.x;
	old.y = point.y;

	lastmove = clock();
	
	if(!ytimer)
		{
		lastpoint = point;
		ytimer = true;
		SetTimer(2, 2000, NULL);
		}

	CStatic::OnMouseMove(nFlags, point);
}

void CPlot::OnLButtonDblClk(UINT nFlags, CPoint point) 

{
	mag = 0;
	scroll = 0;

	Invalidate();

	CStatic::OnLButtonDblClk(nFlags, point);
}

void CPlot::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CStatic::OnRButtonDown(nFlags, point);
}

void CPlot::OnRButtonDblClk(UINT nFlags, CPoint point) 

{
	RECT rc; GetClientRect(&rc);
	unsigned int ss = int_arr.GetSize();

	mag = ss / (rc.right - rc.left) * 100;
	//P2N("ss=%d  rcwidth=%d mag=%f\r\n", ss, rc.right - rc.left, 1 + mag/100);
	scroll = 0;

	Invalidate();
	
	CStatic::OnRButtonDblClk(nFlags, point);
}

void CPlot::OnTimer(UINT nIDEvent) 

{		

	switch(nIDEvent)
		{
		case 1:
			{
			if(!::IsWindow(this->m_hWnd))
				return;
			
			unsigned int newclock = clock();

			//if(newclock > lastclock + CLOCKS_PER_SEC / 3)
				{
				//lastclock = newclock;
				if(changed)
					{
					RECT rc; GetClientRect(&rc);

					int asize = int_arr.GetSize();
					changed = false;
					mag = asize / (rc.right - rc.left) * 100;
					//P2N("ss=%d  rcwidth=%d mag=%f\r\n", asize, rc.right - rc.left, 1 + mag/100);
					scroll = 0;
					Invalidate();
					}
				}
			}
		break;

		case 2:
			{
			KillTimer(nIDEvent);
			ytimer = false;
			//unsigned int newclock = clock();
			//if(newclock > lastclock + 800)
			//	GetMouse
				{	
				//N("Idle timer for plot lastpoint=%d-%d   old:%d-%d\r\n", 
				//	lastpoint.x, lastpoint.y, old.x, old.y);
				}
			}
		break;

		}

	CStatic::OnTimer(nIDEvent);
}
