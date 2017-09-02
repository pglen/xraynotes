// CalCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "dbtest.h"
#include "ctimeCtrl.h"
#include "mxpad.h"
#include "calctrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static char *dayarr[8] =
					{"Mon", "Tue",  "Wed",  "Thu",  "Fri",  "Sat",  "Sun", ""};

/////////////////////////////////////////////////////////////////////////////
// CTimeCtrl

CTimeCtrl::CTimeCtrl()

{
	LOGFONT		m_lf;
	::GetObject((HFONT)GetStockObject(DEFAULT_GUI_FONT), sizeof(m_lf), &m_lf);
	font.CreateFontIndirect(&m_lf);

	pad = 8, head = 8; nav = 16;
	inited = false;

	ldrag = rdrag = 0;

	bg = RGB(240,240,240);
	fg = RGB(100, 100, 100);
}

CTimeCtrl::~CTimeCtrl()
{
}

BEGIN_MESSAGE_MAP(CTimeCtrl, CWnd)
	//{{AFX_MSG_MAP(CTimeCtrl)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTimeCtrl message handlers

void CTimeCtrl::OnPaint()

{
	CPaintDC dc(this); // device context for painting

	if(!inited)
		{
		inited = true;
		InitialPaint();
		}

	RECT rect; GetClientRect(&rect);

	int step = (rect.right - 2 * pad) / 5;
	int vstep = (rect.bottom - (2 * pad + nav)) / 3;

	dc.SetBkMode(TRANSPARENT );

	CFont* pOldFont=NULL;
	pOldFont = (CFont*) dc.SelectObject(font);

	// Draw left drag dohicky
	CRect rc8(rect);
	rc8.top = vstep/4; rc8.bottom = rc8.top +  vstep - 4;
	rc8.left = pad * 2; rc8.right = rc8.left + step + 2 * pad;
	dc.FillSolidRect(rc8, RGB(200,200,200));
	dc.DrawText("<<->>", &rc8, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	// Draw righr drag dohicky
	CRect rc9(rect);
	rc9.top = vstep/4; rc9.bottom = rc9.top +  vstep - 4;
	rc9.right -= 2 * pad; rc9.left = rc9.right - (step + 2 * pad);
	dc.FillSolidRect(rc9, RGB(200,200,200));
	dc.DrawText("<<->>", &rc9, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

}


BOOL CTimeCtrl::OnEraseBkgnd(CDC* pDC)

{
	RECT rect; GetClientRect(&rect);

	pDC->FillSolidRect(&rect, bg);
	//pDC->Draw3dRect(&rect, RGB(200,200,200), RGB(100,100,100));

	return 0;
	//return CWnd::OnEraseBkgnd(pDC);
}

void CTimeCtrl::OnLButtonDown(UINT nFlags, CPoint point)

{
	//P2N("CTimeCtrl::OnLButtonDown Left Butt down\r\n");

	//int month = date.GetMonth();

	RECT rect; GetClientRect(&rect);

	
	int step = (rect.right - 2 * pad) / 5;
	int vstep = (rect.bottom - (2 * pad + nav)) / 3;

	CRect rc8(rect);
	CRect rc9(rect);

	// Left drag dohicky
	rc8.top = vstep/4; rc8.bottom = rc8.top +  vstep - 4;
	rc8.left = pad * 2; rc8.right = rc8.left + step + 2 * pad;

	if(rc8.PtInRect(point))
		{
		ldrag = true;
		SetCapture();
		goto endd;
		}

	// Draw righr drag dohicky

	rc9.top = vstep/4; rc9.bottom = rc9.top +  vstep - 4;
	rc9.right -= 2 * pad; rc9.left = rc9.right - (step + 2 * pad);

	if(rc9.PtInRect(point))
		{
		rdrag = true;
		SetCapture();
		goto endd;
		}

endd:

	CWnd::OnLButtonDown(nFlags, point);
}

void CTimeCtrl::SetTime(COleDateTime &nd)

{
	int hh = nd.GetHour();
	int mm = nd.GetMinute();

	hour1.num = hh / 10;
	hour2.num = hh % 10;

	minute1.num = mm / 10;
	minute2.num = mm % 10;

	Invalidate();
}


void CTimeCtrl::GetTime(COleDateTime &od)

{
	COleDateTime today = COleDateTime::GetCurrentTime();

	int hh = hour1.num * 10 + hour2.num;
	int mm = minute1.num * 10 + minute2.num;

	today.SetTime(hh, mm, 0);

	od = today;
}

void CTimeCtrl::SetToday()

{
	COleDateTime today = COleDateTime::GetCurrentTime();
	
	SetTime(today);
}

void CTimeCtrl::OnLButtonUp(UINT nFlags, CPoint point)

{
	rdrag = ldrag = false;
	ReleaseCapture();
	CWnd::OnLButtonUp(nFlags, point);
}

void CTimeCtrl::OnMouseMove(UINT nFlags, CPoint point)

{
	if(abs(point.x - oldpoint.x) > 5)
		{
		if(ldrag)
			{
			//P2N("Ldrag\r\n");

			int cc = hour1.num * 10 + hour2.num;

			if(point.x > oldpoint.x)
				cc++;
			else
				cc--;

			if(cc > 23)
				cc = 0;

			if(cc < 0)
				cc = 23;

			hour1.num = cc / 10;
			hour2.num = cc % 10;

			Invalidate();
			}
		else if(rdrag)
			{
			//P2N("Rdrag\r\n");
			
			int cc = minute1.num * 10 + minute2.num;

			if(point.x > oldpoint.x)
				cc++;
			else
				cc--;

			if(cc > 59)
				cc = 0;

			if(cc < 0)
				cc = 59;

			minute1.num = cc / 10;
			minute2.num = cc % 10;

			Invalidate();
			}
		oldpoint = point;
		}


	CWnd::OnMouseMove(nFlags, point);
}

int CTimeCtrl::HitTest(CPoint point)

{
	int ret = 0;
	RECT rect; GetClientRect(&rect);

//endd:

	return ret;
}

void CTimeCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)

{
	if(HitTest(point))
		{
		//GetParent()->PostMessage(WM_CALCDBLCLICK, (LONG)&selday);
		}

	CWnd::OnLButtonDblClk(nFlags, point);
}

BOOL CTimeCtrl::PreTranslateMessage(MSG* pMsg)

{
	//P2N("Pretrans %d\r\n", pMsg->message);

	if(pMsg->message == WM_OVERFLOW)
		{
		P2N("CTimeCtrl::Pretrans overflow on %d\r\n", pMsg->wParam);

		switch( pMsg->wParam)
			{
			case 2:
				if(hour1.num == 2)
					{
					hour2.num = 3;
					}
				else
					{
					hour1.num++;
					}
				Invalidate();
				break;

			case 6:
				if(minute1.num == 5)
					{
					minute2.num = 9;
					}
				else
					{
					minute1.num++;
					}
				Invalidate();
				break;
			}
		}

	if(pMsg->message == WM_UNDERFLOW)
		{
		P2N("CTimeCtrl::Pretrans underflow on %d\r\n", pMsg->wParam);
	
		switch(pMsg->wParam)
			{
			case 2:
				if(hour1.num == 1)
					{
					hour2.num = 3;
					}
				else
					{
					hour1.num--;
					}
				Invalidate();
				break;

			case 6:
				minute1.num--;
				Invalidate();
				break;
			}
		}

	if(pMsg->message == WM_TCHANGE)
		{
		//P2N("Pretrans change on %d\r\n", pMsg->wParam);
	
		switch(pMsg->wParam)
			{
			case 2:
				if(hour1.num == 2)
					{
					hour2.SetMinMax(0, 3);
					}
				else
					{
					hour2.SetMinMax(0, 9);
					}
				break;

			case 6:
				break;
			}
		//GetParent()->PostMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
		}

	return CWnd::PreTranslateMessage(pMsg);
}


void CTimeCtrl::InitialPaint()

{
	RECT rc; 
		
	::SetRect(&rc, 8, 30, 38, 85);
	hour1.SetBgColor(bg);
	hour1.SetFgColor(fg);
	hour1.SetMinMax(0, 2);
	hour1.Create(0, 0, WS_VISIBLE, rc, this, 1);
	
	::SetRect(&rc, 42, 30, 72, 85);
	hour2.SetBgColor(bg);
	hour2.SetFgColor(fg);
	hour2.SetMinMax(0, 9);	
	hour2.Create(0, 0, WS_VISIBLE, rc, this, 2);
	
	::SetRect(&rc, 73, 44, 77, 50);
	dot1.Create("", WS_VISIBLE | SS_BLACKRECT, rc, this, 3);

	::SetRect(&rc, 73, 64, 77, 70);
	dot2.Create("", WS_VISIBLE | SS_BLACKRECT, rc, this, 4);

	::SetRect(&rc, 78, 30, 108, 85);
	minute1.SetBgColor(bg);
	minute1.SetFgColor(fg);
	minute1.SetMinMax(0, 5);
	minute1.Create(0, 0, WS_VISIBLE, rc, this, 5);
	
	::SetRect(&rc, 112, 30, 142, 85);
	minute2.SetBgColor(bg);
	minute2.SetFgColor(fg);
	minute2.SetMinMax(0, 9);	
	minute2.Create(0, 0, WS_VISIBLE, rc, this, 6);
}


void CTimeCtrl::Disable()

{
	ModifyStyle(0, WS_DISABLED);

	hour1.SetFgColor(RGB(200,200,200));
	hour2.SetFgColor(RGB(200,200,200));

	minute1.SetFgColor(RGB(100,100,100));
	minute2.SetFgColor(RGB(100,100,100));
}

