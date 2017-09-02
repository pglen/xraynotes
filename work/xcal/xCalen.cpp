
/* =====[ xCalen.cpp ]========================================== 
                                                                             
   Description:     The xcal project, implementation of the xCalen.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  2/14/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// xCalen.cpp : implementation file

#include "stdafx.h"
#include "xcal.h"
#include "xCalen.h"
#include "mxpad.h"
#include "AlaDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CxCalen

CxCalen::CxCalen()

{
	in_scroll = false;

	callback = NULL;

	LOGFONT		m_lf;
	::GetObject((HFONT)GetStockObject(DEFAULT_GUI_FONT), sizeof(m_lf), &m_lf);

	//m_lf.lfHeight = -14;
	//hhh = abs(m_lf.lfHeight) + 6;

	m_font.CreateFontIndirect(&m_lf);

	m_lf.lfWeight  = FW_BOLD;
	//m_lf.lfHeight = -16;

	m_fontb.CreateFontIndirect(&m_lf);

	m_lf.lfHeight -= 5;
	m_fontbh.CreateFontIndirect(&m_lf);

	starthour = INISTART;
	lpad = 60; hpad = 36;
	firstpaint = true;

	today = COleDateTime::GetCurrentTime();
	today.SetDateTime(today.GetYear(), today.GetMonth(), today.GetDay(), 0,0,0);
	selday = date = today;

	oneday.SetDateTimeSpan(1, 0, 0, 0);

	buttdown = false;

	tooltip = "hello";

	nightcolor	=	RGB(148, 148, 148);
	daycolor	=	RGB(228, 228, 228);
	bgcolor		=	RGB(128, 128, 128);

	// Allocate a week's worth of data
	for(int loop = 0; loop < WEEKDAYS; loop++)
		{
		for(int loop2 = 0; loop2 < 4 * HOURSINDAY; loop2++)
			{
			data[loop][loop2] = new CString;
			arr[loop][loop2].RemoveAll();
			}
		}
}

//////////////////////////////////////////////////////////////////////////

CxCalen::~CxCalen()

{
	// De-Allocate a week's worth of data
	for(int loop = 0; loop < WEEKDAYS; loop++)
		{
		for(int loop2 = 0; loop2 < 4 * HOURSINDAY; loop2++)
			{
			delete data[loop][loop2];
			}
		}
}

BEGIN_MESSAGE_MAP(CxCalen, CWnd)
	//{{AFX_MSG_MAP(CxCalen)
	ON_WM_PAINT()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETFOCUS()
	ON_WM_TIMER()
	ON_WM_LBUTTONUP()
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CxCalen message handlers

void CxCalen::PreSubclassWindow() 

{
	ModifyStyleEx(WS_EX_STATICEDGE | WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE, 0);
	ModifyStyle(0, SS_NOTIFY);			// Mouse Messages 
	SetFocus();							// Needed for grabbing input

	CWnd::PreSubclassWindow();
}

BOOL CxCalen::PreTranslateMessage(MSG* pMsg)
 
{
	//////////////////////////////////////////////////////////////////////
	// Dispatch messages ourselvs

	if(pMsg->message == WM_CHAR)
		{
		//P2N("WM_CHAR\r\n");
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
		
	if(pMsg->message == WM_MOUSELEAVE)
		{
		//P2N("Mouse leave message\r\n");
		capped = false;
		fired = false;

		if(buttdown)
			{
			buttdown = false; 
			Invalidate();
			}
		return 0;
		}

	if(pMsg->message == WM_MOUSEHOVER )
		{
		P2N("Mouse hover message\r\n");
		fired = true;

		if(capped)
			{
			capmouse.x = mouse.x;
			capmouse.y = mouse.y;
			
			TRACKMOUSEEVENT te;

			te.cbSize = sizeof(TRACKMOUSEEVENT);
			te.dwFlags = TME_HOVER | TME_CANCEL ;
			te.hwndTrack = this->m_hWnd;		
			::_TrackMouseEvent(&te);	

			// Autosize and autolocate
			CPoint screen = mouse;
			ClientToScreen(&screen); screen.x += 12; screen.y += 24;

			//CWnd *ff = 	GetFocus();
			
			tip.Move(screen);
			GetTooltip(mouse, tooltip);
			tip.SetText(tooltip);

			if(tooltip != "")
				{
				tip.Show();
				}
				
			// Grab focus back
			//if(ff)
			//	if(ff->m_hWnd)
			//		::SetFocus(ff->m_hWnd);
			//SetFocus();
			//GetParent()->SetFocus();
			//Sleep(1000);
			}
		}
	return CWnd::PreTranslateMessage(pMsg);
}

void CxCalen::OnPaint() 

{
	CPaintDC dc(this); // device context for painting
	
	if(firstpaint)
		{
		firstpaint = false;
		FirstPaint(&dc);
		}

	RECT rc; GetClientRect(&rc);

	rc.right -= sw + 3;
	rc.bottom -= sh + 3;

	rc.left += lpad;
	rc.top  += hpad;

	// Prepare calendar drawing
	int dow = date.GetDayOfWeek();
	int month = date.GetMonth();

	// Calculate starting date
	GetStartDate(&startdate);
			
	//COleDateTime fdate(date.GetYear(), date.GetMonth(), date.GetDay(), 0, 0, 0);
	//int fdow = fdate.GetDayOfWeek();

	//CString dstr; dstr = fdate.Format("%Y/%m/%d");
	//P2N("Painting cal ctrl %s fdow=%d\r\n", dstr, fdow);

	//COleDateTimeSpan fd(fdow - 2, 0, 0, 0);	
	//startdate = fdate - fd;
	//startdate -= fd;

	COleDateTime sdate = startdate;

	//dc.FillSolidRect(&rc, RGB(198,198,198));
	
	DrawGrid(&dc, &sdate);

	// Textual
	dc.SetBkMode(TRANSPARENT);
	CFont* oldFont = dc.SelectObject(&m_fontb);

	//CString sstr; sstr = sdate.Format("%Y/%m/%d");
	//P2N("Sdate = %s\r\n", sstr);

	//COleDateTimeSpan oneday(1, 0, 0, 0);
 
	sdate = startdate;

	DrawHeaders(&dc, &sdate);
	DrawContent(&dc);

	dc.SelectObject(oldFont);

	//dc.SelectObject(oldpen);
	// Do not call CWnd::OnPaint() for painting messages
}

void CxCalen::FirstPaint(CPaintDC *pDC)

{
	sw = GetSystemMetrics(SM_CXVSCROLL);
	sh = GetSystemMetrics(SM_CYHSCROLL);

	//Alloc(rows, cols);		

	//TRACE("First paint\r\n");
	P2N("First paint scroll_width=%d\r\n", sw);

	RECT rc;	GetClientRect(&rc);
	//rc.right -= 1;					// Gap next to scroll bar

	rc.left = rc.right - sw;
	rc.bottom -= sw;

	vs.Create(SBS_VERT, rc, this, 1000);
	vs.SetScrollRange(0, 12);
	vs.SetScrollPos(7);
	vs.EnableWindow(true);
	vs.ShowWindow(true);

	GetClientRect(&rc);

	rc.left += lpad;
	rc.top	= rc.bottom - sh;
	rc.right -= sh;
	hs.Create(SBS_HORZ, rc, this, 1001);
	hs.SetScrollRange(-26, 26);
	hs.SetScrollPos(0);
	hs.EnableWindow(true);
	hs.ShowWindow(true);

	SetTimer(1, 20, NULL);

	//P2N("CvSplit::OnPaint firstpaint\r\n");
}

void CxCalen::OnLButtonDblClk(UINT nFlags, CPoint point) 

{
	CAlaDlg dlg;
	RECT rc; GetClientRect(&rc);

	int dh = DayHitTest(point);
	int th = TimeHitTest(point);
	int thh = th;

	//P2N("CxCalen::OnLButtonDblClk hittest=%d %d\r\n", dh, th);

	// Hit?
	if(dh < 0 || th < 0)
		{
		goto endd;
		}
	
	{ // Dummy brace for goto

	// Calculate starting date
	COleDateTime fdate(date.GetYear(), date.GetMonth(), date.GetDay(), 0, 0, 0);
	int fdow = fdate.GetDayOfWeek();
	COleDateTimeSpan fd(fdow - 2, 0, 0, 0);
	COleDateTime sdate(fdate);
	sdate -= fd;
	sdate += dh;

	// Make arrangement for window sizes
	if(rc.bottom > LARGE_SIZE)
		{
		thh /= 4;
		//th *= 1;	
		
		for(int loop = 0; loop < arr[dh][th + starthour * 4].GetSize(); loop++)
			{
			P2N("arr %d\r\n", arr[dh][th + starthour * 4][loop]);
			}

		dlg.dwarr.Copy(arr[dh][th + starthour * 4]);
		}
	else if(rc.bottom > MED_SIZE)
		{
		thh /= 2;
		th *= 2;		
		
		//str = *data[dh][th + starthour * 4];
		//dlg.m_tree.InsertItem(str);
		//str += ", ";
		
		for(int loop = 0; loop < arr[dh][th + starthour * 4].GetSize(); loop++)
			{
			P2N("arr %d\r\n", arr[dh][th + starthour * 4][loop]);
			}

		for(int loop = 0; loop < arr[dh][th + starthour * 4 + 1].GetSize(); loop++)
			{
			P2N("arr %d\r\n", arr[dh][th + starthour * 4 + 1][loop]);
			}

		dlg.dwarr.Copy(arr[dh][th + starthour * 4]);
		dlg.dwarr.Append(arr[dh][th + starthour * 4 + 1]);
		}
	else
		{
		th *= 4;
	
		//str =  *data[dh][th + starthour * 4 + 0];
		//dlg.m_tree.InsertItem(str);
		//str += ", ";
		
		for(int loop = 0; loop < arr[dh][th + starthour * 4].GetSize(); loop++)
			{
			P2N("arr %d\r\n", arr[dh][th + starthour * 4][loop]);
			}

		dlg.dwarr.Copy(arr[dh][th + starthour * 4]);
		dlg.dwarr.Append(arr[dh][th + starthour * 4 + 1]);
		dlg.dwarr.Append(arr[dh][th + starthour * 4 + 2]);
		dlg.dwarr.Append(arr[dh][th + starthour * 4 + 3]);
		}

	CString str = sdate.Format("%B %d, %Y");
	dlg.headstr.Format("Details for: %s -- TimeSlot: %d - %d ", 
			str, thh + starthour, thh + 1 + starthour);
		
	dlg.DoModal();

	// CEdit
	GetParent()->PostMessage(CALITEM_DBLCLICK, GetDlgCtrlID(), 
					MAKELPARAM((thh + starthour) | (dh << 8), starthour * 4 + th) );
	}

endd:
	CWnd::OnLButtonDblClk(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////

int CxCalen::DayHitTest(CPoint point)

{
	int ret = -1;

	RECT rc; GetClientRect(&rc);
	
	rc.right -= sw;
	rc.bottom -= sh;

	// Partition it down
	for(int loop = 0; loop < WEEKDAYS; loop++)
		{
		RECT rc5;

		rc5.left = lpad + (loop * (rc.right - lpad)) / WEEKDAYS;
		rc5.top =  0;
		rc5.right = lpad + ((loop + 1) * (rc.right - lpad)) / WEEKDAYS; 
		rc5.bottom = rc.bottom;

		if(::PtInRect(&rc5, point))
			{
			ret = loop;
			break;
			}
		}

	return ret;
}

int CxCalen::TimeHitTest(CPoint point)

{
	int ret = -1;

	RECT rc; GetClientRect(&rc);
	
	rc.right -= sw;
	rc.bottom -= sh;

	// Partition it across
	for(int loop2 = 0; loop2 < SHOWHOURS * 4; loop2++)
		{
		RECT rc5;

		rc5.left = 0;
		rc5.right = rc.right;
		
		if(rc.bottom > LARGE_SIZE)
			{
			rc5.top =  hpad + (loop2 * (rc.bottom - hpad)) / (SHOWHOURS * 4);
			rc5.bottom = hpad + ((loop2 + 1) * (rc.bottom - hpad)) / (SHOWHOURS * 4);
			}
		else if(rc.bottom > MED_SIZE)
			{
			rc5.top =  hpad + (loop2 * (rc.bottom - hpad)) / (SHOWHOURS * 2);
			rc5.bottom = hpad + ((loop2 + 1) * (rc.bottom - hpad)) / (SHOWHOURS * 2);
			}
		else
			{
			rc5.top =  hpad + (loop2 * (rc.bottom - hpad)) / (SHOWHOURS );
			rc5.bottom = hpad + ((loop2 + 1) * (rc.bottom - hpad)) / (SHOWHOURS);
			}

		if(::PtInRect(&rc5, point))
			{
			//P2N("Hit loop %d\r\n", loop2);
			ret = loop2; // + starthour;
			break;
			}
		}
	return ret;
}

static int in_mw = false;

BOOL CxCalen::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 

{
	DoMouseWheel(nFlags, zDelta, pt); 
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

BOOL CxCalen::DoMouseWheel(UINT nFlags, short zDelta, CPoint pt) 

{
	//P2N("CxCalen::OnMouseWheel\r\n");

	if(!in_mw)
		{
		in_mw = true;
		int old_sh = starthour;
		// Simulate key feed to message box
		if(zDelta > 0)
			{
			SetStartHour(starthour - 1);
			}
		else
			{
			SetStartHour(starthour + 1);
			}
		
		// Invalidate if needed
		if(starthour != old_sh)
			{
			vs.SetScrollPos(starthour);
			Invalidate();
			//GetParent()->Invalidate();	
			}

		in_mw = false;
		}
	return 0;
}

void CxCalen::OnMouseMove(UINT nFlags, CPoint point) 

{
	TRACKMOUSEEVENT te;

	mouse.x = point.x;
	mouse.y = point.y;

	//if(tooltip != "")
		{
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
				te.dwHoverTime = 1500;
			
				::_TrackMouseEvent(&te);
				}
			}	
		else 
			{
			if(fired)
				{
				// Moved more than hover pixels?
				if	(abs(capmouse.x - point.x) > 5 ||
						abs(capmouse.y - point.y) > 5)
					{
					tip.Hide();
					//P2N("Move in no tracking\r\n");
					fired = false;
					capped = false;
					}
				}
			}
		}

	//P2N("CxCalen::OnMouseMove\r\n");
	CWnd::OnMouseMove(nFlags, point);
}

void CxCalen::OnLButtonDown(UINT nFlags, CPoint point) 

{
	//P2N("CxCalen::OnLButtonDown\r\n");

	TRACKMOUSEEVENT te;

	te.cbSize = sizeof(TRACKMOUSEEVENT);
	te.dwFlags = TME_LEAVE;
	te.hwndTrack = this->m_hWnd;
	te.dwHoverTime = 0;

	::_TrackMouseEvent(&te);

	//tip.Hide();
	capped = fired = false;

	//SetCapture();
	//SetFocus();
	//buttdown = true;
	//Invalidate();

	//SetFocus();

	CWnd::OnLButtonDown(nFlags, point);
}

void CxCalen::OnSetFocus(CWnd* pOldWnd) 

{
	CWnd::OnSetFocus(pOldWnd);
	//Invalidate();	
}


void CxCalen::SetItem(COleDateTime *ddd, const char *ste, int id)

{
	//P2N("Set item %s %s\r\n", ddd->Format("%c"), ste);

	// Prepare calendar drawing
	int dow = date.GetDayOfWeek();
	int month = date.GetMonth();

	// Calculate starting date
	COleDateTime fdate(date.GetYear(), date.GetMonth(), date.GetDay(), 0, 0, 0);
	int fdow = fdate.GetDayOfWeek();

	//CString dstr; dstr = fdate.Format("%Y/%m/%d");
	//P2N("Painting cal ctrl %s fdow=%d\r\n", dstr, fdow);

	COleDateTime sdate(fdate);
	COleDateTimeSpan fd(fdow - 2, 0, 0, 0);	
	sdate -= fd;

	COleDateTime edate(sdate);
	COleDateTimeSpan oneweek(7, 0, 0, 0), onesec(0, 0, 0, 1);
	edate += oneweek; edate -= onesec;

	//P2N("thisweek %s %s\r\n", sdate.Format("%c"), edate.Format("%c"));

	// Is it in this week?
	if (*ddd >= sdate &&  *ddd < edate)
		{
		int dday  = ddd->GetDayOfWeek() - sdate.GetDayOfWeek();
		int dhour = ddd->GetHour();
		int dmin = ddd->GetMinute();
		int dquart = dmin / 15;

		SetItem(dday, dhour, dquart, ste, id);
		}
	else
		{
		//P2N("Not in current week\r\n");
		}
}

//////////////////////////////////////////////////////////////////////////

void CxCalen::SetItem(int day, int hour, int quart, const char *ste, int id)

{
	//P2N("Set item day: %d hour: %d quart: %d ste:'%s'\r\n", day, hour, quart, ste);

	if(day >= WEEKDAYS)
		{
		P2N("Day is too big\r\n");
		}
	if(hour >= HOURSINDAY)
		{
		P2N("HOUR is too big\r\n");
		}

	// make sur it is boolean
	if(quart > 3)
		quart = 3;

	if(*data[day][hour * 4 + quart] != "")
		{
		*data[day][hour * 4 + quart] += ", ";
		}

	*data[day][hour * 4 + quart] += ste;
	arr[day][hour * 4 + quart].Add(id);
}


void CxCalen::SetStartHour(int hour)
{
	int old_st = starthour;

	starthour = min(max(hour, 0), 24 - SHOWHOURS);
	
	// Changed?
	if(old_st != starthour)
		{
		//GetParent()->Invalidate();	
		Invalidate();
		}
}

void CxCalen::DefHours()

{
	starthour = INISTART;
	//GetParent()->Invalidate();
	Invalidate();
}

void CxCalen::Fill(const char *str)

{
	P2N("CxCalen::Fill\r\n");

	ClearContents();
	Invalidate();

	GetStartDate(&startdate);
	GetParent()->PostMessage(CALITEM_WANTDATA, GetDlgCtrlID(), 0);

	if(callback)
		callback(this);
	
	//while(TRUE)
    //  {
    //	MSG msg;
    //    if(!PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    //       break;
    //    // Every other message is processed
    //    TranslateMessage(&msg); DispatchMessage(&msg);
    //    }

}

void CxCalen::OnTimer(UINT nIDEvent) 

{
	if(nIDEvent == 1)
		{
		KillTimer(nIDEvent);
		Fill("");
		//Invalidate();
		}

	CWnd::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////

void CxCalen::SetNewDate(COleDateTime *ndate)

{
	date.SetDateTime(ndate->GetYear(), ndate->GetMonth(), ndate->GetDay(), 0,0,0);
		
	COleDateTimeSpan dist = date - today;
	int ddist = dist.GetDays();
	hs.SetScrollPos(ddist / 7);

	//SetTimer(1, 20, NULL);
	
	Fill();

	//Invalidate();
	//GetParent()->Invalidate();
}

//////////////////////////////////////////////////////////////////////////
//
// Function name:    CxCalen::DrawContent
// Description:      draw content from database, adapt to size
// Return type:      void 
// Argument:         CPaintDC *dc
//
//////////////////////////////////////////////////////////////////////////

void CxCalen::DrawContent(CPaintDC *dc)

{
	CString str;
	RECT rc; GetClientRect(&rc);

	rc.right -= sw + 3;
	rc.bottom -= sh + 3;

	// Draw content
	RECT	rc7;
	for(int loop = 0; loop < WEEKDAYS; loop++)
		{
		for(int loop2 = 0; loop2 < SHOWHOURS; loop2++)
			{
			rc7.left	= lpad + 5 + (loop * (rc.right - lpad)) / WEEKDAYS;
			rc7.right	= lpad + -5 + ((loop + 1) * (rc.right - lpad)) / WEEKDAYS;

			if(rc.bottom > LARGE_SIZE)
				{
								
				if(*data[loop][(loop2 + starthour) * 4] != "")
					{
					//P2N("Data %d %d -- %s\r\n", loop, loop2  + starthour, *data[loop][(loop2 + starthour) * 2]);
					
					rc7.top		= hpad + (loop2 * (rc.bottom - hpad)) / SHOWHOURS ;
					rc7.bottom  = rc7.top + (rc.bottom - hpad) / (SHOWHOURS * 4);									
					dc->DrawText(*data[loop][(loop2 + starthour) * 4], &rc7, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					}

				if(*data[loop][(loop2 + starthour) * 4 + 1] != "")
					{
					//P2N("Data %d %d -- %s\r\n", loop, loop2  + starthour, *data[loop][(loop2 + starthour) * 2 + 1]);
					
					rc7.top		= hpad + (loop2 * (rc.bottom - hpad)) / SHOWHOURS ;
					rc7.top		+= (rc.bottom - hpad) / (SHOWHOURS * 4);
					rc7.bottom  = rc7.top + (rc.bottom - hpad) / (SHOWHOURS * 4);

					dc->DrawText(*data[loop][(loop2 + starthour) * 4 + 1], &rc7, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					}

				if(*data[loop][(loop2 + starthour) * 4 + 2] != "")
					{
					//P2N("Data %d %d -- %s\r\n", loop, loop2  + starthour, *data[loop][(loop2 + starthour) * 2 + 2]);
					
					rc7.top		= hpad + (loop2 * (rc.bottom - hpad)) / SHOWHOURS ;
					rc7.top		+= 2 * (rc.bottom - hpad) / (SHOWHOURS * 4);
					rc7.bottom  = rc7.top + (rc.bottom - hpad) / (SHOWHOURS * 4);

					dc->DrawText(*data[loop][(loop2 + starthour) * 4 + 2], &rc7, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					}

				if(*data[loop][(loop2 + starthour) * 4 + 3] != "")
					{
					//P2N("Data %d %d -- %s\r\n", loop, loop2  + starthour, *data[loop][(loop2 + starthour) * 2 + 3]);
					
					rc7.top		= hpad + (loop2 * (rc.bottom - hpad)) / SHOWHOURS ;
					rc7.top		+= 3 * (rc.bottom - hpad) / (SHOWHOURS * 4);
					rc7.bottom  = rc7.top + (rc.bottom - hpad) / (SHOWHOURS * 4);

					dc->DrawText(*data[loop][(loop2 + starthour) * 4 + 3], &rc7, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					}
				}
			else if(rc.bottom > MED_SIZE)
				{
				if(*data[loop][(loop2 + starthour) * 4] != "")
					{
					//P2N("Data %d %d -- %s\r\n", loop, loop2  + starthour, *data[loop][(loop2 + starthour) * 2]);

					//rc7.left	= lpad + 5 + (loop * (rc.right - lpad)) / WEEKDAYS;
					//rc7.right	= lpad + -5 + ((loop + 1) * (rc.right - lpad)) / WEEKDAYS;

					rc7.top		= hpad + (loop2 * (rc.bottom - hpad)) / SHOWHOURS;
					rc7.bottom  = rc7.top + (rc.bottom - hpad) / (SHOWHOURS * 2);

					str =  *data[loop][(loop2 + starthour) * 4 + 0];
					str +=  ", ";
					str += *data[loop][(loop2 + starthour) * 4 + 1];

					dc->DrawText(str, &rc7, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					}

				if(*data[loop][(loop2 + starthour) * 4 + 2] != "")
					{
					//P2N("Data %d %d -- %s\r\n", loop, loop2  + starthour, *data[loop][(loop2 + starthour) * 2 + 1]);
					
					//rc7.left	= lpad + 5 + (loop * (rc.right - lpad)) / WEEKDAYS;
					//rc7.right	= lpad + -5 + ((loop + 1) * (rc.right - lpad)) / WEEKDAYS;
					
					str = *data[loop][(loop2 + starthour) * 4 + 2];
					str +=  ", ";
					str += *data[loop][(loop2 + starthour) * 4 + 3];

					rc7.top		= hpad + (loop2 * (rc.bottom - hpad)) / SHOWHOURS;
					rc7.top		+= (rc.bottom - hpad) / (SHOWHOURS * 2);
					rc7.bottom  = rc7.top + (rc.bottom - hpad) / (SHOWHOURS * 2);

					dc->DrawText(str, &rc7, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					}
				}
			else
				{
				if(*data[loop][(loop2 + starthour) * 4] != "")
					{
					//P2N("Data %d %d -- %s\r\n", loop, loop2  + starthour, *data[loop][(loop2 + starthour) * 2]);

					rc7.left	= lpad + 5 + (loop * (rc.right - lpad)) / WEEKDAYS;
					rc7.right	= lpad + -5 + ((loop + 1) * (rc.right - lpad)) / WEEKDAYS;
					rc7.top		= hpad + (loop2 * (rc.bottom - hpad)) / SHOWHOURS;
					rc7.bottom  = rc7.top + (rc.bottom - hpad) / (SHOWHOURS);
					
					// Compose the string
					str = *data[loop][(loop2 + starthour) * 4 + 0];
					str +=  ", ";
					str += *data[loop][(loop2 + starthour) * 4 + 1];

					str += *data[loop][(loop2 + starthour) * 4 + 2];
					str +=  ", ";
					str += *data[loop][(loop2 + starthour) * 4 + 3];
					
					dc->DrawText(str, &rc7, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					}
				}	
			}
		}
}

//////////////////////////////////////////////////////////////////////////

void CxCalen::DrawHeaders(CPaintDC *dc, COleDateTime *sdate)

{
	CString str;
	RECT rc; GetClientRect(&rc);

	rc.right -= sw + 3;
	rc.bottom -= sh + 3;


	// Headers
	for(int loop3 = 0; loop3 < WEEKDAYS; loop3++)
		{
		RECT rc5;

		rc5.left = lpad + (loop3 * (rc.right - lpad)) / WEEKDAYS; 
		rc5.top = 3;
		rc5.bottom = hpad / 3; 
		rc5.right = lpad + ((loop3 + 1) * (rc.right - lpad)) / WEEKDAYS; 
		
		//CString str;

		//COleDateTime

		if(rc.right > LARGE_SIZE)
			{
			str = sdate->Format("%B %d, %Y");
			//str.Format("February %d, %04d", sdate., 2008);
			}
		else if(rc.right > LARGE_SIZE)
			{
			str = sdate->Format("%B %d");
			//str.Format("February %d", loop3 + 1);
			}
		else
			{
			str = sdate->Format("%b %d");
			//str.Format("Feb %d", loop3);
			}

		dc->FillSolidRect(&rc5, bgcolor);

		dc->SelectObject(&m_font);

		dc->DrawText(str, &rc5, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		if(rc.right > 700)
			{
			str = sdate->Format("%A");
			//str = sweekdays[loop3];
			}
		else
			{
			str = sdate->Format("%a");
			//str = weekdays[loop3];
			}
	
		rc5.left = lpad + (loop3 * (rc.right - lpad)) / WEEKDAYS; 
		rc5.top = hpad/3;
		rc5.bottom = hpad; 
		rc5.right = lpad + ((loop3 + 1) * (rc.right - lpad)) / WEEKDAYS; 
		
		dc->SelectObject(&m_fontbh);

		//if(GetFocus() == this)
		//else
		//	dc.SelectObject(&m_fontb);
	
		dc->FillSolidRect(&rc5, bgcolor);

		dc->DrawText(str, &rc5, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		*sdate += oneday;
		}

	// Paint year in the corner
	str = date.Format("%Y"); RECT rc6;
	rc6.left = 0;
	rc6.top = 0;
	rc6.bottom = hpad; 
	rc6.right = lpad;

	dc->FillSolidRect(&rc6, bgcolor);
	dc->SelectObject(&m_fontbh);
	dc->DrawText(str, &rc6, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	dc->SelectObject(&m_font);

	// Left side headers
	for(int loop4 = 0; loop4 < SHOWHOURS; loop4++)
		{
		RECT rc5;

		rc5.left = 0;
		rc5.top = hpad + (loop4 * (rc.bottom - hpad)) / SHOWHOURS;
		rc5.bottom = hpad + ((loop4 + 1) * (rc.bottom - hpad)) / SHOWHOURS;
		rc5.right = lpad;
		
		dc->FillSolidRect(&rc5, bgcolor);

		CString str;

		if(!true)
			{
			str.Format("%d-%d", loop4 + starthour, loop4 + 1 + starthour);
			}
		else
			{
			CString ampm("am");

			int hbeg = (loop4 + starthour), hend = (loop4 + starthour + 1);

			if( hbeg > HOURSLIGHT)
				{
				hbeg -= HOURSLIGHT; 
				}

			if( hend > HOURSLIGHT)
				{
				hend -=HOURSLIGHT;
				ampm = "pm";
				}
			str.Format("%02d-%02d %s", hbeg, hend, ampm);
			}

		dc->DrawText(str, &rc5, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
}

void CxCalen::OnLButtonUp(UINT nFlags, CPoint point) 

{
	buttdown = false;
	//Invalidate();
	
	//GetParent()->PostMessage(BN_CLICKED, id, 0);

	TRACKMOUSEEVENT te;

	te.cbSize = sizeof(TRACKMOUSEEVENT);
	te.dwFlags = TME_CANCEL | TME_LEAVE;
	te.hwndTrack = this->m_hWnd;		
	::_TrackMouseEvent(&te);	
	
	CWnd::OnLButtonUp(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////

void CxCalen::GetTooltip(CPoint point, CString &str)

{
	RECT rc; GetClientRect(&rc);

	int dh = DayHitTest(point), th = TimeHitTest(point);

	if(dh >= 0 && th >= 0)
		{
		if(rc.bottom > LARGE_SIZE)
			{
			// th *= 1;   // made it to noop
			//th -= th % 4;

			// Assign tooltip here
			str =  *data[dh][th + starthour * 4 + 0];

			}
		else if(rc.bottom > MED_SIZE)
			{
			th *= 2;	

			// Assign tooltip here
			str =  *data[dh][th + starthour * 4 + 0];

			if(*data[dh][th + starthour * 4 + 1] != "")
				{
				str += ", ";
				str += *data[dh][th + starthour * 4 + 1];
				}
			}
		else
			{
			th *= 4;	

			// Assign tooltip here
			str =  *data[dh][th + starthour * 4 + 0];

			if(*data[dh][th + starthour * 4 + 1] != "")
				{
				str += ", ";
				str += *data[dh][th + starthour * 4 + 1];
				}
			if(*data[dh][th + starthour * 4 + 2] != "")
				{
				str += ", ";
				str += *data[dh][th + starthour * 4 + 2];
				}
			if(*data[dh][th + starthour * 4 + 3] != "")
				{
				str += ", ";
				str += *data[dh][th + starthour * 4 + 3];
				}
			}		
		}

	P2N("Hover on item(s): '%s'\r\n", str);
}

void CxCalen::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
		
	if(::IsWindow(vs.m_hWnd))
		{
		RECT rc;	GetClientRect(&rc);
		RECT rc2;	vs.GetClientRect(&rc2);
		RECT rc3;	hs.GetClientRect(&rc3);

		vs.SetWindowPos(NULL, rc.right - sw, 0, rc2.right - rc2.left, rc.bottom - rc2.top - sh, SWP_NOOWNERZORDER);
		hs.SetWindowPos(NULL, 0, rc.bottom - sh, rc.right - rc.left - sw, rc3.bottom - rc3.top, SWP_NOOWNERZORDER);	
		}
}

void CxCalen::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 

{
	int old_sh = starthour;
	
	switch (nSBCode)
		{
		case SB_THUMBTRACK  :
				SetStartHour(nPos);
				pScrollBar->SetScrollPos(starthour);
				break;

		case SB_PAGEDOWN  :
				SetStartHour(starthour + 4);
		case SB_LINEDOWN  :
				SetStartHour(starthour + 1);
				pScrollBar->SetScrollPos(starthour);
				break;

		case SB_PAGEUP :
				SetStartHour(starthour - 4);
		case SB_LINEUP  :
				SetStartHour(starthour - 1);
				pScrollBar->SetScrollPos(starthour);
				break;
		}

	// Invalidate if needed
	if(starthour != old_sh)
		{
		//GetParent()->Invalidate();	
		Invalidate();
		}

	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

///////////////////////////////////////////////////////////////////////////////
void CxCalen::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 

{
	//P2N("Scroll %d nPos\r\n", nPos);

	if(in_scroll)
		return;

	in_scroll = true;
	COleDateTime dd;

	switch (nSBCode)
		{
		case SB_THUMBTRACK  :
				dd = today;
				if((int)nPos < 0)
					{
					dd -= 7 * abs((long)nPos) * oneday;
					}
				else
					{	
					dd += 7 * abs((long)nPos) * oneday;
					}
				SetNewDate(&dd);
				break;

		case SB_PAGEDOWN  :
				dd = date + (COleDateTime)(4 * 7 * oneday);
				SetNewDate(&dd);
				break;

		case SB_LINEDOWN  :
				dd = date + (COleDateTime)(7 * oneday);
				SetNewDate(&dd);
				break;

		case SB_PAGEUP :
				dd = date - (COleDateTime)(4 * 7 * oneday);
				SetNewDate(&dd);
				break;

		case SB_LINEUP  :
				dd = date - (COleDateTime)(7 * oneday);
				SetNewDate(&dd);
				break;

		case SB_ENDSCROLL:
			Fill();
			break;
		}

	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);

	in_scroll = false;
}

void CxCalen::ClearContents()
{
	// Clear prev contents
	for(int loop = 0; loop < WEEKDAYS; loop++)
		{
		for(int loop2 = 0; loop2 < 4 * HOURSINDAY; loop2++)
			{
			*data[loop][loop2] = "";
			arr[loop][loop2].RemoveAll();
			}
		}
}

//////////////////////////////////////////////////////////////////////////
// Calculate UI starting date

void CxCalen::GetStartDate(COleDateTime *sdate)

{
	COleDateTime fdate(date.GetYear(), date.GetMonth(), date.GetDay(), 0, 0, 0);
	int fdow = fdate.GetDayOfWeek();

	COleDateTimeSpan fd(fdow - 2, 0, 0, 0);
		
	*sdate = fdate - fd;

	//CString dstr; dstr = sdate->Format("%Y/%m/%d");
	//P2N("Got Starting date %s fdow=%d\r\n", dstr, fdow);
}

void CxCalen::InvalidateCal()
{

}

void CxCalen::Invalidate()
{
	P2N("CxCalen::Invalidate\r\n");

	RECT rc; GetClientRect(&rc);

	rc.right -= sw + 3;
	rc.bottom -= sh + 3;

	rc.left += lpad;
	rc.top  += hpad;

	CWnd::InvalidateRect(&rc);
}

//////////////////////////////////////////////////////////////////////////

void	CxCalen::DrawGrid(CPaintDC *dc, COleDateTime *sdate)

{
	RECT rc; GetClientRect(&rc);

	rc.right -= sw + 3;
	rc.bottom -= sh + 3;

	rc.left += lpad;
	rc.top  += hpad;

	CPen pen(PS_SOLID, 1, RGB(108, 108, 108));
	CPen pen4(PS_SOLID, 2, RGB(108, 108, 108));
	CPen *oldpen = (CPen*)dc->SelectObject(pen);
	
	// Highlite daytime (across)
	for(int loop22 = 0; loop22 < SHOWHOURS; loop22++)
		{
		// Highlite daytime (across)
		for(int loop22 = 0; loop22 < SHOWHOURS; loop22++)
		{
		if((loop22 * (rc.bottom - hpad) / SHOWHOURS) < rc.bottom - hpad)	
			if((loop22 + starthour > DSTART) && (loop22 + starthour < DEND) )
				{
				dc->FillSolidRect(lpad, hpad + (loop22 * (rc.bottom - hpad) / SHOWHOURS), 
								rc.right - (lpad ), (rc.bottom - hpad) / SHOWHOURS, daycolor );
				}
			else
				{
				dc->FillSolidRect(lpad, hpad + (loop22 * (rc.bottom - hpad) / SHOWHOURS), 
								rc.right - (lpad ), (rc.bottom - hpad) / SHOWHOURS, nightcolor);
				}
			}
		}

	// Partition it down
	for(int loop = 0; loop < WEEKDAYS; loop++)
		{
		dc->MoveTo(lpad + (loop * (rc.right - lpad)) / WEEKDAYS, 0);
		dc->LineTo(lpad + (loop * (rc.right - lpad)) / WEEKDAYS, rc.bottom);

		// Draw today's double lines
		if(*sdate == today)
			{
			dc->SelectObject(pen4);
			dc->MoveTo(lpad + 3 + (loop * (rc.right - lpad)) / WEEKDAYS, hpad);
			dc->LineTo(lpad + 3 + (loop * (rc.right - lpad)) / WEEKDAYS, rc.bottom);
			dc->SelectObject(pen);
			}
		if(*sdate == today + oneday)
			{
			dc->SelectObject(pen4);
			dc->MoveTo(lpad - 2 + (loop * (rc.right - lpad)) / WEEKDAYS, hpad);
			dc->LineTo(lpad - 2 + (loop * (rc.right - lpad)) / WEEKDAYS, rc.bottom);
			dc->SelectObject(pen);
			}
		*sdate += oneday;
		}
	
	// Select back first pen
	dc->SelectObject(pen);

	// Partition it across
	for(int loop2 = 0; loop2 < SHOWHOURS + 1; loop2++)
		{
		dc->MoveTo(0, hpad + (loop2 * (rc.bottom - hpad)) / SHOWHOURS);
		dc->LineTo(rc.right, hpad + (loop2 * (rc.bottom - hpad)) / SHOWHOURS);
		}

	// Partition it across, fine res
	if(rc.bottom > LARGE_SIZE)
		{
		CPen pen3(PS_SOLID, 1, RGB(168, 168, 168));
		(CPen*)dc->SelectObject(pen3);
	
		for(int loop2 = 0; loop2 < SHOWHOURS * 4; loop2++)
			{
			if(loop2 % 4)
				{
				dc->MoveTo(lpad, hpad + (loop2 * (rc.bottom - hpad)) / (SHOWHOURS * 4));
				dc->LineTo(rc.right, hpad + (loop2 * (rc.bottom - hpad)) / (SHOWHOURS * 4));
				}
			}
		}
	else if(rc.bottom > MED_SIZE)
		{
		CPen pen3(PS_SOLID, 1, RGB(168, 168, 168));
		(CPen*)dc->SelectObject(pen3);
	
		for(int loop2 = 0; loop2 < SHOWHOURS * 2; loop2++)
			{
			if(loop2 % 2)
				{
				dc->MoveTo(lpad, hpad + (loop2 * (rc.bottom - hpad)) / (SHOWHOURS * 2));
				dc->LineTo(rc.right, hpad + (loop2 * (rc.bottom - hpad)) / (SHOWHOURS * 2));
				}
			}
		}
	
	CPen pen2;
	pen2.CreatePen(PS_SOLID, 3, RGB(128, 128, 128));
	dc->SelectObject(pen2);
	
	// Draw boundary lines
	dc->MoveTo(0, hpad + (0 * rc.bottom) / SHOWHOURS );
	dc->LineTo(rc.right, hpad + (0 * rc.bottom) / SHOWHOURS);

	dc->MoveTo(lpad - 1 + (0 * (rc.right - lpad)) / WEEKDAYS, 0);
	dc->LineTo(lpad - 1 + (0 * (rc.right - lpad)) / WEEKDAYS, rc.bottom);

}

void CxCalen::OnRButtonDown(UINT nFlags, CPoint point) 

{
	RECT rc; GetClientRect(&rc);

	int dh = DayHitTest(point);
	int th = TimeHitTest(point);
	int thh = th;

	// Hit?
	if(dh < 0 || th < 0)
		{
		goto endd;
		}

		// Make arrangement for window sizes
	if(rc.bottom > LARGE_SIZE)
		{
		thh /= 4;
		//th *= 1;	
		}
	else if(rc.bottom > MED_SIZE)
		{
		thh /= 2; th *= 2;		
		}
	else
		{
		th *= 4;
		}

	// CEdit
	GetParent()->PostMessage(CALITEM_RCLICK, GetDlgCtrlID(), 
					MAKELPARAM((thh + starthour) | (dh << 8), starthour * 4 + th) );	

endd:
	CWnd::OnRButtonDown(nFlags, point);
}
