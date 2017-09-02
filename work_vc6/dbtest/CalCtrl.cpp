// CalCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "dbtest.h"
#include "CalCtrl.h"
#include "mxpad.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

char *dayarr[8] = 
					{"Mon", "Tue",  "Wed",  "Thu",  "Fri",  "Sat",  "Sun", ""};

/////////////////////////////////////////////////////////////////////////////
// CCalCtrl

CCalCtrl::CCalCtrl()

{	
	LOGFONT		m_lf;
	::GetObject((HFONT)GetStockObject(DEFAULT_GUI_FONT), sizeof(m_lf), &m_lf);
	font.CreateFontIndirect(&m_lf);

	pad = 8, head = 8; nav = 16;
	inited = false;

	ldrag = rdrag = 0;

	today = COleDateTime::GetCurrentTime();

	today.SetDateTime(today.GetYear(), today.GetMonth(), today.GetDay(), 0,0,0);
	selday = date = today;
	
	CString dstr; dstr = date.Format("%Y/%m/%d");

	bg = RGB(240,240,240);

	P2N("Construct cal ctrl at %s\r\n", dstr);
}

CCalCtrl::~CCalCtrl()
{
}


BEGIN_MESSAGE_MAP(CCalCtrl, CWnd)
	//{{AFX_MSG_MAP(CCalCtrl)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCalCtrl message handlers

void CCalCtrl::OnPaint() 

{
	CPaintDC dc(this); // device context for painting

	if(!inited)
		{
		inited = true;
		GetParent()->PostMessage(WM_CALCHANGE, 0, (LONG)&selday);
		}

	int dow = date.GetDayOfWeek();
	int month = date.GetMonth();

	// Calculate starting date
	COleDateTime fdate(date.GetYear(), date.GetMonth(), 1, 0, 0, 0);
	int fdow = fdate.GetDayOfWeek();

	//CString dstr; dstr = fdate.Format("%Y/%m/%d");
	//P2N("Painting cal ctrl %s fdow=%d\r\n", dstr, fdow);

	COleDateTimeSpan fd(fdow - 2, 0, 0, 0);
		
	COleDateTime sdate(fdate);
	sdate -= fd;

	//CString sstr; sstr = sdate.Format("%Y/%m/%d");
	//P2N("Sdate = %s\r\n", sstr);

	int loop, loop2;

	CFont* pOldFont=NULL;
	pOldFont = (CFont*) dc.SelectObject(font);
	
	RECT rect; GetClientRect(&rect);

	int step = (rect.right - 2 * pad) / 7;
	int vstep = (rect.bottom - (2 * pad + nav)) / 7;

	dc.SetBkMode(TRANSPARENT );

	CRect rc7(rect);
	rc7.top += pad/4; rc7.bottom = rc7.top + vstep; 
	rc7.left += pad/2; rc7.right -= pad / 2;

	CString ym; ym = date.Format("%B, %Y");
	dc.DrawText(ym, &rc7, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

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

	CRect rc6(rect);
	rc6.top += head + nav; rc6.bottom = rc6.top + vstep; 
	rc6.left += pad/2; rc6.right -= pad/2;
	dc.FillSolidRect(rc6, RGB(140,140,140));

	CRect rc5(rect);
	rc5.top += head + nav; rc5.bottom = rc5.top + vstep;
	rc5.left = pad; rc5.right = rc5.left + step;	
	
	for(loop = 0; loop < 7; loop++)
		{
		dc.DrawText(dayarr[loop], &rc5, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

		rc5.left += step;
		rc5.right = rc5.left +  step;
		}

	CRect rc4(rect);
	rc4.top = head + nav + vstep + pad;
	rc4.bottom = rc4.top + vstep;

	rc4.left = pad;
	rc4.right = rc4.left + step;

	COleDateTimeSpan oneday(1, 0, 0, 0);

	for(loop = 0; loop < 6; loop++)
		{
		for(loop2 = 0; loop2 < 7; loop2++)
			{
			if(sdate == selday)
				dc.Draw3dRect(rc4, RGB(128, 128, 128), RGB(128, 128, 128));

			if(sdate == today)
				{
				CRect rc3(rc4); 
				rc3.DeflateRect(3, 3);

				dc.FillSolidRect(rc3, RGB(140,140,140));
				}
		
			// Shift it to the correct day
			CString ddd; ddd.Format("%d", sdate.GetDay());
		
			// Is it in range?
			if(month == sdate.GetMonth())
				dc.SetTextColor(RGB(0, 0, 0));
			else
				dc.SetTextColor(RGB(150, 150, 150));

			dc.DrawText(ddd, &rc4, DT_CENTER | DT_SINGLELINE | DT_VCENTER );
			sdate += oneday;

			rc4.left += step;
			rc4.right += step;
			}
		
		rc4.left = pad;
		rc4.right = rc4.left + step;

		rc4.top += vstep;
		rc4.bottom += vstep;
		}

	//if (pOldFont) 
	//	dc.SelectObject(pOldFont);	//restore the old font object

#if 0

	for(loop = startrow; loop < rows; loop++)
		{
		CPtrArray *col = (CPtrArray *)rowarr[loop];

		cell *ccc;

		for(int loop2 = startcol; loop2 < cols ; loop2++)
			{
			ccc = (cell *)col->GetAt(loop2); 
			CString *str = (CString *)ccc->content;

			rc4.bottom = rc4.top + ccc->hh; 
			rc4.right = rc4.left + ccc->ww;

			CRect rc5(rc4); 
			rc5.top += 1; rc5.left += 1;
			rc5.DeflateRect(1, 1);

			if(loop == currrow && loop2 == currcol)
				{
				dc.Draw3dRect(rc5, RGB(128, 128, 128), RGB(128, 128, 128));
				}
			
			//CString tmp; tmp.Format("r:%d c:%d", ccc->cellrow, ccc->cellcol);
			//dc.DrawText(tmp, &rc4, DT_CENTER);

			dc.DrawText(*str, &rc5, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			rc4.left += ccc->ww; 
		
			// Clip to last displyed column
			if(rc4.left + ccc->ww >= rect.right)
				break;

			//P2N("Drawing cell %d %d '%s'\r\n", ccc->cellrow, ccc->cellcol, *str);
			}
		rc4.top += ccc->hh;
		rc4.left = head_size;

		// Clip to last displyed line
		if(rc4.top + ccc->hh >= rect.bottom)
			break;
		}

#endif

}

BOOL CCalCtrl::OnEraseBkgnd(CDC* pDC) 

{
	RECT rect; GetClientRect(&rect);

	//pDC->FillSolidRect(&rect, RGB(220,220,220));
	pDC->FillSolidRect(&rect, bg);

	//pDC->Draw3dRect(&rect, RGB(200,200,200), RGB(100,100,100));

	return 0;
	
	//return CWnd::OnEraseBkgnd(pDC);
}

void CCalCtrl::OnLButtonDown(UINT nFlags, CPoint point) 

{
	//P2N("CCalCtrl::OnLButtonDown Left Butt down\r\n");

	int month = date.GetMonth();

	RECT rect; GetClientRect(&rect);
	
//	int loop, loop2;
	int step = (rect.right - 2 * pad) / 7;
	int vstep = (rect.bottom - (2 * pad + nav)) / 7;

	// Left drag dohicky
	CRect rc8(rect);
	rc8.top = vstep/4; rc8.bottom = rc8.top +  vstep - 4; 
	rc8.left = pad * 2; rc8.right = rc8.left + step + 2 * pad;

	if(rc8.PtInRect(point))
		{
		ldrag = true;
		SetCapture();
		goto endd; 
		}

	// Right drag dohicky
	{
	CRect rc9(rect);
	rc9.top = vstep/4; rc9.bottom = rc9.top +  vstep - 4; 
	rc9.right -= 2 * pad; rc9.left = rc9.right - (step + 2 * pad); 

	if(rc9.PtInRect(point))
		{
		rdrag = true;
		SetCapture();
		goto endd;
		}
	}	

	HitTest(point);

endd:
	
	CWnd::OnLButtonDown(nFlags, point);
}


void CCalCtrl::SetDate(COleDateTime &nd, int nopost)

{
	date.SetDateTime(nd.GetYear(), nd.GetMonth(), nd.GetDay(), 0,0,0);
	selday = date;

	if(!nopost)
		GetParent()->PostMessage(WM_CALCHANGE, 0, (LONG)&selday);

	Invalidate();
}

void CCalCtrl::GetDate(COleDateTime &od)

{
	od = selday;
}

void CCalCtrl::SetToday()

{
	today = COleDateTime::GetCurrentTime();

	today.SetDateTime(today.GetYear(), today.GetMonth(), today.GetDay(), 0,0,0);
	selday = date = today;

	GetParent()->PostMessage(WM_CALCHANGE, 0, (LONG)&selday);

	Invalidate();
}

void CCalCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	rdrag = ldrag = false;
	ReleaseCapture();	
	CWnd::OnLButtonUp(nFlags, point);
}

void CCalCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(abs(point.x - oldpoint.x) > 5)
		{
		if(ldrag)
			{
			COleDateTime  nd = date;
			COleDateTimeSpan ts(30, 0, 0, 0);
			
			if(point.x > oldpoint.x)
				nd += ts;
			else
				nd -= ts;

			SetDate(nd);
			}
		else if(rdrag)
			{
			COleDateTime  nd = date;
			COleDateTimeSpan ts(365, 0, 0, 0);
		
			if(point.x > oldpoint.x)
				nd += ts;
			else
				nd -= ts;

			SetDate(nd);
			}
		oldpoint = point;
		}

	
	CWnd::OnMouseMove(nFlags, point);
}

int CCalCtrl::HitTest(CPoint point)

{
	int ret = 0;
	RECT rect; GetClientRect(&rect);
	
	int loop, loop2;
	int step = (rect.right - 2 * pad) / 7;
	int vstep = (rect.bottom - (2 * pad + nav)) / 7;

	CRect rc4(rect);
	rc4.top = head + nav + vstep + pad;
	rc4.bottom = rc4.top + vstep;

	rc4.left = pad;
	rc4.right = rc4.left + step;

	// Calculate starting date
	COleDateTime fdate(date.GetYear(), date.GetMonth(), 1, 0, 0, 0);
	int fdow = fdate.GetDayOfWeek();

	COleDateTimeSpan fd(fdow - 2, 0, 0, 0);
		
	COleDateTime sdate(fdate);
	sdate -= fd;

	COleDateTimeSpan oneday(1, 0, 0, 0);

	for(loop = 0; loop < 6; loop++)
		{
		for(loop2 = 0; loop2 < 7; loop2++)
			{
			// Shift it to the correct day
			CString ddd; ddd.Format("%d", sdate.GetDay());
		
			// Is it a hit
			if(rc4.PtInRect(point))
				{
				CString tmp = sdate.Format("%Y/%m/%d");

				//P2N("Hit at: %s\r\n", tmp);

				selday = sdate;

				if(date.GetMonth() != selday.GetMonth())
					{
					date = selday;
					}

				Invalidate();
				GetParent()->PostMessage(WM_CALCHANGE, 0, (LONG)&selday);
				ret = true;
				goto endd;
				}
			sdate += oneday;

			rc4.left += step;
			rc4.right += step;
			}
		
		rc4.left = pad;
		rc4.right = rc4.left + step;

		rc4.top += vstep;
		rc4.bottom += vstep;
		}
endd:

	return ret;
}

void CCalCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 

{
	if(HitTest(point))
		{
		GetParent()->PostMessage(WM_CALCDBLCLICK, (LONG)&selday);		
		}

	CWnd::OnLButtonDblClk(nFlags, point);
}

void CCalCtrl::Disable()

{
	ModifyStyle(0, WS_DISABLED);
	bg = RGB(140,140,140);
}
