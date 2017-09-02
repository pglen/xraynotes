// PicList.cpp : implementation file
//

#include "stdafx.h"
#include "xraynotes.h"
#include "PicList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPicList

CPicList::CPicList()

{
	curr_row = 0;

	firstpaint = true;
	leftspacer = topgap = 5;
	startrow = startcol = vrows = vcols = 0;
	visrows = maxlen = 0;

	charrc.SetRectEmpty();

	LOGFONT		m_lf;
	::GetObject((HFONT)GetStockObject(DEFAULT_GUI_FONT), sizeof(m_lf), &m_lf);
	m_font.CreateFontIndirect(&m_lf);

	back = RGB(220, 220, 220); 
	highlite = RGB(128, 128, 128);
	high_nofocus = RGB(158,158,158);
}

CPicList::~CPicList()

{
	int len = rowarr.GetSize();

	for(int loop = 0; loop < len; loop++)
		delete (ListItem*)rowarr[loop];	
}


BEGIN_MESSAGE_MAP(CPicList, CBaseWnd)
	//{{AFX_MSG_MAP(CPicList)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPicList message handlers

void CPicList::OnPaint() 

{
	if(firstpaint)
		{
		firstpaint = false;
		FirstPaint();
		}

	CPaintDC dc(this); // device context for painting

	RECT rect;	GetClientRect(&rect);

	CWnd *focuswin = GetFocus();

	dc.SetBkMode(true);
	
	CFont	*oldFont = dc.SelectObject(&m_font);
	int pos = 0, alen = rowarr.GetSize();

	for(int loop = startrow; loop < alen; loop++)
		{
		CRect rcc(leftspacer, topgap + pos * charrc.bottom, rect.right - leftspacer - sw, 
					topgap + (pos + 1) * charrc.bottom);
	
		if(curr_row == loop)
			{
			CRect rcc(leftspacer/2, topgap + pos * charrc.bottom, rect.right - leftspacer/2 - sw, 
					topgap + (pos + 1) * charrc.bottom);
	
			COLORREF ccc = high_nofocus;
			
			if(focuswin == this)
				ccc = highlite;
			
			dc.FillSolidRect(rcc, ccc);
			}

		if(loop < rowarr.GetSize())
			{
			ListItem *lit = (ListItem*)rowarr[loop];

			ASSERT(lit->magic == ListItem_Magic);

			//CxStr str = lit->content->SafeMid(startcol);
			CxStr str = *lit->content;
			ASSERT(str.magic == STR_MAGIC);

			dc.DrawText(str, str.GetLength(), rcc, DT_LEFT | DT_SINGLELINE | DT_VCENTER );	
			}

		// Clip drawing after the bottom
		if(rect.bottom < rcc.bottom)
			{
			//P2N("Clipped at %d\r\n", rcc.bottom);
			break;
			}
		pos++;
		}
	dc.SelectObject(oldFont);
}


BOOL CPicList::OnEraseBkgnd(CDC* pDC) 
{
	RECT rect;	GetClientRect(&rect);

	//P2N("Erase background\r\n");	

	CBrush NewBrush(back) ; 
	pDC->SetBrushOrg(0,0) ;
	CBrush* pOldBrush = (CBrush*)pDC->SelectObject(&NewBrush);   
	pDC->PatBlt(rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, PATCOPY);
	pDC->SelectObject(pOldBrush);

	return true;	
}

void CPicList::FirstPaint()

{
	sw = GetSystemMetrics(SM_CXVSCROLL);
	sh = GetSystemMetrics(SM_CYHSCROLL);

	// Create scroll bars
	RECT rc;	GetClientRect(&rc);
	//rc.right -= 1;					// Gap next to scroll bar
	rc.left = rc.right - sw;
	rc.bottom -= sw;

	vs.Create(SBS_VERT, rc, this, 1000);
	vs.EnableWindow(false);
	vs.ShowWindow(true);

	GetClientRect(&rc);	
	
	rc.top = rc.bottom - sh;
	rc.right -= sh;
	hs.Create(SBS_HORZ, rc, this, 1001);
	hs.EnableWindow(false);
	hs.ShowWindow(true);

	CClientDC  dc(this); // device context for painting
	dc.DrawText("a", 1, &charrc, DT_CALCRECT);

	visrows = (rc.bottom - sh - 2 * topgap) / charrc.bottom;

	if( (rc.bottom - sh - 2 * topgap) % charrc.bottom)
		visrows++;

	UpdateBars(GetFocus() == this);

	//P2N("Visrows: %d rc.bottom=%d charrc.bottom=%d\r\n", visrows,
	//	rc.bottom, charrc.bottom);
}


void testit(char * Format, ...)

{
	//char arr[256];

	va_list ArgList; va_start(ArgList, Format);
	
	for(int loop = 0; loop < 4; loop++)
		{
		int arg = va_arg(ArgList, int);
		P2N("arg-%d %d %x\r\n", loop, arg, arg, arg);
		}

	//int len = _vsnprintf(arr, 255, Format, ArgList);
	//P2N(arr);
}


void CPicList::AddStr(const char *str)

{	
	CxStr *newstr = new CxStr(str); ASSERT(newstr);

	ListItem *lit = new ListItem; ASSERT(lit);

	lit->content = newstr;

	maxlen = max(maxlen, newstr->GetLength());

	rowarr.Add(lit);

	if(IsWindow(m_hWnd))
		{			
		UpdateBars(true);
		}

	//P2N("Added %s\r\n", *newstr);

	
}

// AfxGetModuleState

void CPicList::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 

{	
	P2N("CPicList::OnHScroll pos=%d\r\n", nPos);

	switch (nSBCode)
		{
		case SB_THUMBTRACK  :
				startcol = nPos;
				P2N("CPicList::OnHScroll Thumb track %d\r\n", nPos);
				pScrollBar->SetScrollPos(startcol);
				Invalidate();
				break;

		case SB_PAGEDOWN  :
				startcol = min(maxlen, startcol + 5);
		case SB_LINEDOWN  :
				startcol = min(maxlen, startcol + 1);;
				//P2N("Vscroll down\r\n");
				pScrollBar->SetScrollPos(startcol);
				Invalidate();
				break;

		case SB_PAGEUP :
				startcol = max(maxlen, 0);
		case SB_LINEUP  :
				startcol = max(0, startcol - 1);
				//P2N("Vscroll up\r\n");
				pScrollBar->SetScrollPos(startcol);
				Invalidate();
				break;
		}

	CBaseWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CPicList::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	P2N("Vscroll pos=%d\r\n", nPos);

	switch (nSBCode)
		{
		case SB_THUMBTRACK  :
				startrow = nPos;
				//P2N("Thumb track\r\n");
				pScrollBar->SetScrollPos(startrow);
				Invalidate();
				break;

		case SB_PAGEDOWN  :
				startrow = min(rowarr.GetSize() - 1, startrow + 5);

		case SB_LINEDOWN  :
				startrow = min(rowarr.GetSize() - 1, startrow + 1);

				//P2N("Vscroll down\r\n");
				pScrollBar->SetScrollPos(startrow);
				Invalidate();
				break;

		case SB_PAGEUP :
				startrow = max(startrow - 5, 0);

		case SB_LINEUP  :
				startrow = max(0, startrow - 1);
				pScrollBar->SetScrollPos(nPos);
				//P2N("Vscroll up\r\n");
				pScrollBar->SetScrollPos(startrow);
				Invalidate();
				break;
		}

	CBaseWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CPicList::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 

{
	switch(nChar)
		{
		case  VK_HOME:
				SetCurrRow(0);
		break;
		
		case  VK_END:
				SetCurrRow(rowarr.GetUpperBound());
		break;
		
		case  VK_NEXT:
				SetCurrRow(
					min(rowarr.GetUpperBound(), curr_row + 5));
		break;
		
		case  VK_PRIOR:
				SetCurrRow(
					max(0, curr_row - 5));
		break;
		
		case  VK_UP:
				SetCurrRow(curr_row - 1);
		break;
		
		case  VK_DOWN:
				SetCurrRow(curr_row + 1);
		break;

		default:
			break;
		}

	CBaseWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}


int CPicList::KeyEvent(UINT event, UINT nChar, UINT nFlags)

{
	int ret = true;

	//P2N("CPicList::KeyEvent(UINT event=%d, UINT nChar=%d, UINT nFlags=%d\r\n",
	//										event, nChar, nFlags);

	if(event == WM_KEYDOWN)
		{
		OnKeyDown(nChar, 1, nFlags);

		if(nChar == VK_TAB)
			{
			ret = false;
			}
		}

	if(event == WM_SYSKEYDOWN)
		{
		ret = false;
		}
	if(event == WM_SYSCHAR)
		{
		ret = false;
		}
	if(event == WM_SYSKEYUP)
		{
		ret = false;
		}

	return ret;
}

void CPicList::OnLButtonDown(UINT nFlags, CPoint point) 

{
	CRect rect; GetClientRect(&rect);

	P2N("CPicList::OnLButtonDown xx=%d yy=%d\r\n", point.x, point.y);

	for(int pos = 0; pos < visrows; pos++)
		{
		CRect rcc(leftspacer, topgap + pos * charrc.bottom, rect.right - leftspacer - sw, 
					topgap + (pos + 1) * charrc.bottom);
	
		if(rcc.PtInRect(point))
			{
			if(pos + startrow < rowarr.GetSize())
				{
				P2N("hit at %d\r\n", pos + startrow);
				SetCurrRow(pos + startrow);
				}
			break;
			}
		}
	CBaseWnd::OnLButtonDown(nFlags, point);
}

void CPicList::SetCurrRow(int crow)

{
	if(crow >= rowarr.GetSize() || crow < 0)
		{
		P2N("CPicList::Setcurr_row Attempted to set past end\r\n");
		return;
		}

	curr_row = crow;

	if(curr_row < startrow)
		startrow = curr_row;
				
	if(curr_row > startrow + (visrows - 1))
		startrow = curr_row - (visrows - 1);

	vs.SetScrollPos(startrow);

	Invalidate();
}


void CPicList::UpdateBars(int enable)

{
	//P2N("Updatebars %d\r\n", enable);

	if(enable)
		{
		CRect rc; GetClientRect(&rc);

		if(rowarr.GetSize() >= visrows)
			{
			if(IsWindow(vs.m_hWnd))
				{
				vs.EnableWindow();
				vs.SetScrollRange(0, rowarr.GetUpperBound() - (visrows - 1));
				}
			}

		if(maxlen * charrc.right > rc.right)
			{
			if(IsWindow(hs.m_hWnd))
				{
				hs.EnableWindow();
				vs.SetScrollRange(0, maxlen);
				}
			}
		}
	else
		{
		if(IsWindow(vs.m_hWnd))
			vs.EnableWindow(false);

		if(IsWindow(hs.m_hWnd))
			hs.EnableWindow(false);
		}
}

void CPicList::OnSetFocus(CWnd* pOldWnd) 

{
	CBaseWnd::OnSetFocus(pOldWnd);

	UpdateBars(true);

	P2N("CPicList::OnSetFocus \r\n");
}

void CPicList::OnKillFocus(CWnd* pNewWnd) 

{
	CBaseWnd::OnKillFocus(pNewWnd);

	UpdateBars(false);

	P2N("CPicList::OnKillFocus \r\n");	
}

BOOL CPicList::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 

{
	P2N("CPicList::OnMouseWheel %d\r\n", zDelta);

	int pos = curr_row;

	if(zDelta < 0)
		pos += 1;
	else
		pos -= 1;

	SetCurrRow(pos);

	return CBaseWnd::OnMouseWheel(nFlags, zDelta, pt);
}


void CPicList::OnLButtonDblClk(UINT nFlags, CPoint point) 

{
	CRect rect; GetClientRect(&rect);

	//P2N("CPicList::OnLButtonDblClk xx=%d yy=%d\r\n", point.x, point.y);

	for(int pos = 0; pos < visrows; pos++)
		{
		CRect rcc(leftspacer, topgap + pos * charrc.bottom, rect.right - leftspacer - sw, 
					topgap + (pos + 1) * charrc.bottom);
	
		if(rcc.PtInRect(point))
			{
			if(pos + startrow < rowarr.GetSize())
				{
				P2N("hit at %d\r\n", pos + startrow);
				SetCurrRow(pos + startrow);

				ListItem *lit = (ListItem*)rowarr[curr_row];

				
				//AfxMessageBox("Dbl Click");
				CString tmp; tmp.Format("Double Click %d %s", 
								curr_row, (int)((ListItem*)rowarr[curr_row])->content);
				xMsgBox(tmp);
				}
			break;
			}
		}

	CBaseWnd::OnLButtonDblClk(nFlags, point);
}

void CPicList::OnInitControl()

{
	CxStr *ss = new CxStr ("Hello World String");

	*ss += " wwwwwz";
	*ss += " eeeEz";

	ss->Dump();	

	CxStr mm;
	
	//mm = ss->SafeMid(25, 5);
	//mm.Dump();

	CxStr rr; rr.LoadString(IDS_ABOUTBOX);
	
	rr.Dump();	rr.Compact(); rr.Dump();

	//P2N("Tested: '%s' '%s' '%s'\r\n", (CSTR)*ss, (CSTR)mm, (CSTR)rr);

	CxStr::DumpAll();
	
	delete ss;


	//P2N("Added2 %s -- %s\r\n", (int)ss, (int)sss);
}