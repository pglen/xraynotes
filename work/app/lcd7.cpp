// lcd7.cpp : implementation file
//

#include "stdafx.h"
#include "xraynotes.h"
#include "mxpad.h"
#include "lcd7.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Clcd7


Clcd7::Clcd7()
{
	// Set initial values
	gap		= 1;
	width	= 0;
	oldnum = num = 0;

	low		= 0;
	high	= 9;
	firstpaint = true;
	seg1= seg2= seg3= seg4= seg5= seg6= seg7 = false;

	fg = RGB(0, 0, 0);
	bg = RGB(100,100,100);

	nomessage = false;

}

Clcd7::~Clcd7()
{
}


BEGIN_MESSAGE_MAP(Clcd7, CWnd)
	//{{AFX_MSG_MAP(Clcd7)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Clcd7 message handlers

void Clcd7::OnPaint() 

{
	RECT rc; GetClientRect(&rc);

	if(firstpaint)
		{	
		firstpaint = false;
		if(width == 0)
			width = rc.right / 4;

		if(!nomessage)
			GetParent()->PostMessage(WM_TCHANGE, GetDlgCtrlID(), 0);

		nomessage = false;
		}

	CPaintDC dc(this); // device context for painting

	// Delimit number
	if(num < low)
		{
		num = high;
		GetParent()->PostMessage(WM_UNDERFLOW, GetDlgCtrlID() , 0);
		}

	if(num > high)
		{
		num = low;
		
		GetParent()->PostMessage(WM_OVERFLOW, GetDlgCtrlID() , 0);
		}

	// Start from all reset
	seg1 = seg2 = seg3 = seg4 = seg5 = seg6 = seg7 = false;

	// Coding of segments:
	//				  1
	//			 ---------
	//		4	|    3    | 6
	//			 ---------
	//		5	|         | 7
	//			 ---------
	//               2

	// Set the ones that need to be set:
	switch(num)
		{
		case 0: seg1 = seg2 = seg4 = seg5 = seg6 = seg7 = true;	break;
		case 1:	seg6= seg7 = true; break;
		case 2: seg1 = seg6 = seg3 = seg5 = seg2 = true; break;
		case 3: seg1 = seg6 = seg3 = seg7 = seg2 = true; break;
		case 4: seg4  = seg3 = seg6 = seg7 = true;	break;
		case 5: seg1  = seg4 = seg3 = seg7 = seg2 = true; break;
		case 6: seg1 = seg4  = seg5 = seg2 = seg7 = seg3 = true; break;
		case 7: seg1  = seg6  = seg7 = true; break;
		case 8: seg1 = seg2 = seg3 = seg4 = seg5 = seg6 = seg7 = true; break;
		case 9: seg1 = seg3 = seg4 = seg6 = seg7 = seg2 = true; break;
		case 10: seg1 = seg4 = seg5 = seg3 = seg6 = seg7 = true; break;  // A
		case 11: seg4 = seg5 = seg2 = seg3 = seg7 = true; break;  // b
		case 12: seg3 = seg5 = seg2 = true; break;  // c
		case 13: seg3 = seg5 = seg2 = seg7 = seg6 = true; break;  // d
		case 14: seg1 = seg4 = seg3 = seg5 = seg2 = true; break;  // e
		case 15: seg1 = seg4 = seg3 = seg5 = true; break;  // f
	}

	//P2N("seg=%d seg2=%d seg3=%d\r\n", seg1, seg2, seg3);

	int vhalf = rc.bottom / 2;
	
	CBrush Brush(fg);
	CBrush Wrush(bg);
	CPen	nullpen(PS_NULL, 0, RGB(0,0,0));

	CBrush *old = (CBrush*) dc.SelectObject(&Brush);
	CPen   *oldp = (CPen*) dc.SelectObject(&nullpen);

	//dc.SetPolyFillMode(WINDING);

	POINT poly[6];

	///////////////////////////////////////////////////
	///    0---------------------------1
	//        3--------------------2
	//

	poly[0].x = 0;						poly[0].y = 0;
	poly[1].x = rc.right;				poly[1].y = 0;
	poly[2].x = rc.right - (width);		poly[2].y = width;
	poly[3].x = width;					poly[3].y = width;

	if(seg1)
		dc.SelectObject(&Brush);
	else
		dc.SelectObject(&Wrush);

	dc.Polygon(poly, 4);
	
	///////////////////////////////////////////////////
	//        0--------------------1
	///    3---------------------------2
	
	poly[0].x = (width );				poly[0].y = rc.bottom - (width );
	poly[1].x = rc.right - (width);		poly[1].y = rc.bottom - (width );
	poly[2].x = rc.right;				poly[2].y = rc.bottom ;
	poly[3].x = 0;						poly[3].y = rc.bottom ;

	if(seg2)
		dc.SelectObject(&Brush);
	else
		dc.SelectObject(&Wrush);

	dc.Polygon(poly, 4);

	///////////////////////////////////////////////////
	//        0--------------------1
	///    5---------------------------2
	///        4--------------------3
	// 
	poly[0].x = width ;					poly[0].y = vhalf - width/2 + gap;
	poly[1].x = rc.right - width;		poly[1].y = vhalf - width/2 + gap;
	poly[2].x = rc.right;				poly[2].y = vhalf;
	poly[3].x = rc.right - width;		poly[3].y = vhalf + width/2 + gap;
	poly[4].x = width;					poly[4].y = vhalf + width/2 + gap;
	poly[5].x = 0;						poly[5].y = vhalf + gap;
	
	if(seg3)
		dc.SelectObject(&Brush);
	else
		dc.SelectObject(&Wrush);
	
	dc.Polygon(poly, 6);

	//////////////////////////////////////////////////
	//	 0
	//  |  3
	//  | |
	//  |  2
	//   1
		
	poly[0].x = 0;						poly[0].y = gap;
	poly[1].x = 0;						poly[1].y = vhalf;
	poly[2].x = width;					poly[2].y = vhalf - (width/2);
	poly[3].x = width;					poly[3].y = width + gap;

	if(seg4)
		dc.SelectObject(&Brush);
	else
		dc.SelectObject(&Wrush);

	dc.Polygon(poly, 4);

	poly[0].y += vhalf + gap;
	poly[1].y += vhalf -  gap;
	poly[2].y += vhalf -  gap - width/2;
	poly[3].y += vhalf - width/2 + gap;

	if(seg5)
		dc.SelectObject(&Brush);
	else
		dc.SelectObject(&Wrush);

	dc.Polygon(poly, 4);

	//////////////////////////////////////////////////
	//	        3
	//      0  |
	//       | |
	//      1  | 
	//          2
		
	poly[0].x = rc.right - (width);			poly[0].y = (width)  + gap;
	poly[1].x = rc.right - (width);			poly[1].y = vhalf - (width/2);
	poly[2].x = rc.right;					poly[2].y = vhalf - gap ;
	poly[3].x = rc.right;					poly[3].y = gap;

	if(seg6)
		dc.SelectObject(&Brush);
	else
		dc.SelectObject(&Wrush);

	dc.Polygon(poly, 4);

	poly[0].y += vhalf - width/2 + gap;
	poly[1].y += vhalf - width/2 - gap;
	poly[2].y += vhalf; 
	poly[3].y += vhalf;

	if(seg7)
		dc.SelectObject(&Brush);
	else
		dc.SelectObject(&Wrush);

	dc.Polygon(poly, 4);

	dc.SelectObject(old);
	dc.SelectObject(oldp);

	if(oldnum != num)
		{
		if(!nomessage)
			{
			GetParent()->PostMessage(WM_TCHANGE, GetDlgCtrlID(), 0);
			}
		}
	nomessage = false;
	oldnum = num;

	// Just for testing
	//dc.SetBkMode(TRANSPARENT );
	//CString str; str.Format("%d", num);
	//rc.bottom = rc.bottom / 2;
	//dc.DrawText(str, &rc, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	
	// Do not call CWnd::OnPaint() for painting messages
}

void Clcd7::OnLButtonDown(UINT nFlags, CPoint point) 

{
	SetCapture();
	CWnd::OnLButtonDown(nFlags, point);
}

void Clcd7::OnMouseMove(UINT nFlags, CPoint point) 

{
	if(nFlags & MK_LBUTTON  )
		{
		if(abs(point.x - oldpoint.x) > 10)
			{
			if(point.x > oldpoint.x)
				num++;
			else
				num--;

			// Limit it
			//if(num > high) 
			//	{
			//	num = low;
			//	}

			//if(num < low) 
			//	{
			//	num = high;
			//	}

			Invalidate();
			oldpoint = point;
			}
		}

	CWnd::OnMouseMove(nFlags, point);
}

void Clcd7::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();	
	CWnd::OnLButtonUp(nFlags, point);
}

void Clcd7::SetMinMax(int minn, int maxx)

{
	low = minn;
	high = maxx;

	if(num < low) num = low;
	if(num > high) num = high;

	if(::IsWindow(m_hWnd))
		Invalidate();
}

void Clcd7::SetFgColor(COLORREF col)

{
	fg = col;

	if(::IsWindow(m_hWnd))
		Invalidate();
}

void Clcd7::SetBgColor(COLORREF col)

{
	bg = col;

	if(::IsWindow(m_hWnd))
		Invalidate();
}

