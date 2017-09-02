
/* =====[ XrayM.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the XrayM.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  11/20/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// XrayM.cpp : implementation file
//

#include "stdafx.h"
#include "winuser.h"
#include <afxmt.h>

#include "xraynotes.h"
#include "XrayM.h"
#include "MxPad.h"
#include "TextMode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CPtrArray CXrayM::wndarr;

// ///////////////////////////////////////////////////////////////////////////
// CXrayMitem dialog

void CXrayMitem::InitVars()

{
	submenu = NULL;
	command = status = 	checked = 0;

	selected = 0;
	SetRect(&rect, 0,0,0,0);

	magic = MENUITEM_MAGIC;
	memset(key, 0, sizeof(key));

	itemstr = new CString;
	tipstr = new CString;
	tooltip = new CString;
}


CXrayMitem::CXrayMitem()

{
	InitVars();	
}

CXrayMitem::CXrayMitem(const char *ss)

{
	InitVars();
	*tipstr = *itemstr = ss;
}

CXrayMitem::~CXrayMitem()

{
	//P2N("CXrayMitem::~CXrayMitem()\r\n");
	delete tooltip;
	delete itemstr;
	delete tipstr;
}

CXrayM::~CXrayM()

{
	Clear();

	// Remove from STATIC stale list in still there
	for(int loopm = wndarr.GetSize() - 1; loopm >= 0; loopm--)
		{
		CXrayM *item = (CXrayM *)wndarr[loopm];
		ASSERT(item->magic == MENU_MAGIC);
		
		if(item == this)
			{
			//P2N("CXrayM::~CXrayM() matching item in static arr\r\n");
			wndarr.RemoveAt(loopm);
			}
		}
}

// ///////////////////////////////////////////////////////////////////////////
// CXrayM dialog

CXrayM::CXrayM(CWnd* pParent /*=NULL*/)
	: CGrayDlg(CXrayM::IDD, pParent)
{
	//P2N("CXrayM::CXrayM %p\r\n", this);

	bAlpha = 230;
	
	titleheight = 10;
	topgap = 16;
	leftgap = 24;
	gap = 4;
	textheight = 12;
	noremember = true;
	disabled = RGB(200, 200, 200);
	grayed = RGB(150, 150, 150);

	OldLoop = -1;

	// Set colors
	//dlgback		= RGB(100,100,100);
	dlgbackcol	 = ((CShadesApp*)AfxGetApp())->dlgbackcol;
	highlite	= RGB(150,150,150);
	//checkmark	= RGB(128, 128, 128);
	checkmark	= RGB(0, 0, 0);

	subopen = mparent = NULL;
	closeonup = m_loaded = fired = false;
	sendto = NULL;

	//closeonup = true;

	magic = MENU_MAGIC;

	// Add this to the list
	//P2N("Added to global array %p\r\n", this);
	wndarr.Add((void *)this);

	// Just testing
	//P2N("MF_ENABLED=%d MF_DISABLED=%d MF_GRAYED=%d MF_POPUP=%d\r\n", 
	//			MF_ENABLED, MF_DISABLED, MF_GRAYED, MF_POPUP);

	//{{AFX_DATA_INIT(CXrayM)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	windowname = support.GetFileNameExt(__FILE__);
}

void CXrayM::DoDataExchange(CDataExchange* pDX)

{
	CGrayDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXrayM)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CXrayM, CGrayDlg)
	//{{AFX_MSG_MAP(CXrayM)
	ON_WM_KILLFOCUS()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/// Append Menu Item to the end of menu

int CXrayM::AppendMenuItem(const char *str, int command, int status, int checked)

{
	ASSERT(magic == MENU_MAGIC);
	
	CXrayMitem *item = new CXrayMitem(str); 

	ASSERT(item->magic == MENUITEM_MAGIC);

	item->status = status;
	item->command = command;
	item->checked = checked;
	arr.Add(item);

	return arr.GetUpperBound();
}

/// Append Sub Menu Item to the end of menu

int CXrayM::AppendSubMenu(const char *str, CXrayM *submenu, int status)

{
	ASSERT(magic == MENU_MAGIC);
	
	if(!submenu)
		{
		P2N("CXrayM::AppendSubMenu Must Specify a submenu\r\n");
		return 0;
		}

	CXrayMitem *item = new CXrayMitem(str); 

	ASSERT(item->magic == MENUITEM_MAGIC);

	item->status = status;
	item->submenu = submenu;
	arr.Add(item);
	
	return arr.GetUpperBound();
}

/// Execute load and show in one step

void CXrayM::LoadAndShow(int nResource, int skip, int xx, int yy)

{
	ASSERT(magic == MENU_MAGIC);
	
	if(xx == -1 || yy == -1)
		{
		xx = 300; yy = 300;
		}

	// This does not work ....
	//MOUSEMOVEPOINT ppt;
	//GetMouseMovePoints
	//int ret = GetMouseMovePoints(sizeof(MOUSEMOVEPOINT), &ppt, NULL, 0, 
	//												GMMP_USE_DISPLAY_POINTS);

	LoadMenu(MAKEINTRESOURCE(nResource), skip);
	Show(xx, yy);
}

// ///////////////////////////////////////////////////////////////////////////
// CXrayM message handlers

/// Show menu at specified screen coordinates

void CXrayM::Show(int xx, int yy, int sub)

{
	ASSERT(magic == MENU_MAGIC);
	
	//support.YieldToWinEx();

	//P2N("Show CXrayM window\r\n");

	CMutex mx;	mx.Lock();

	//P2N("Scanning global array in %p size=%d\r\n", this, wndarr.GetSize());
	
	// Remove all pending menus from list
	for(int loopm = wndarr.GetSize() - 1; loopm >= 0; loopm--)
		{
		CXrayM *item = (CXrayM *)wndarr[loopm];

		ASSERT(item->magic == MENU_MAGIC);

		// Only process them after loading
		if(item->m_loaded)
			{
			// Only scan top menus
			if(item->mparent == NULL)
				{
				CString str;
				int flag = false;

				if(item->m_hWnd && IsWindow(item->m_hWnd))
					{
					if(item->IsWindowVisible())
						flag = true;
				
					item->GetMenuItemText(0, str);
					//P2N("Loop=%d ptr=%p flag=%d str=%s\r\n", loopm, item, flag, str);
					if(!flag)
						{
						//P2N("Removing stale menu from array %p\r\n", wndarr[loopm]);
						item->DestroyWindow();
						wndarr.RemoveAt(loopm);
						}
					}
				}
			else
				{
				// This is a submenu, remove it from safety list
				wndarr.RemoveAt(loopm);
				}
			}
		}
	mx.Unlock();
	
	CGrayDlg::xCreate();

	int ww, hh;	MeasureMenu(&ww, &hh);

	// Count items and separators
	int counti = 0, counts = 0;
	for(int loop = 0; loop < arr.GetSize(); loop++)
		{
		CXrayMitem *item = (CXrayMitem*)arr[loop];
	
		ASSERT(item->magic == MENUITEM_MAGIC);

		if(*item->itemstr == "")
			counts++;
		else
			counti++;
		}

	RECT bigrc;	AfxGetMainWnd()->GetClientRect(&bigrc);

	int www = ww + 2 * gap + 2 * leftgap + 12;
	int hhh = (hh + 2 * gap) * (counti) + (2 * gap) * (counts) 
						+ 2 * topgap;

	//P2N("Mmenu popup %d over %d\r\n", bigrc.bottom, yy + hhh);

	// Pop it upwards
	if( yy > bigrc.bottom/2 )
		{
		yy = yy - hhh;

		if(sub)
			yy += 2 * topgap;
		}

	//P2N("Mmenu popup %d over %d\r\n", bigrc.right, xx + www);

	if(bigrc.right < xx + www)
		xx = bigrc.right - www;

		
	// Position and size the window ....
	SetWindowPos( NULL, xx, yy, www,  hhh, SWP_NOZORDER );  
	//support.YieldToWinEx();
	
	CGrayDlg::Show();

	
}

void CXrayM::OnKillFocus(CWnd* pNewWnd) 

{
	CGrayDlg::OnKillFocus(pNewWnd);

	// Closes the menu if no submenus are open	
	if(!subopen)
		Hide();
}

BOOL CXrayM::PreTranslateMessage(MSG* pMsg) 

{
	//P2N("CXrayM::PreTranslateMessage %s\r\n", mxpad.num2msg(pMsg->message));

	// ////////////////////////////////////////////////////////////////////
	// Dispatch messages ourselvs

	if(pMsg->message == WM_CHAR)
		{
		//P2N("WM_CHAR\r\n");
		//OnChar(pMsg->wParam, LOWORD(pMsg->lParam), pMsg->lParam); 
		return 0;
		}
	if(pMsg->message == WM_KEYDOWN)
		{
		//OnKeyDown(pMsg->wParam, LOWORD(pMsg->lParam), pMsg->lParam); 
		return 0;
		}
	if(pMsg->message == WM_KEYUP)
		{
		//OnKeyUp(pMsg->wParam, LOWORD(pMsg->lParam), pMsg->lParam); 
		return 0;
		}
	if(pMsg->message == WM_SYSCHAR)
		{
		//OnSysChar(pMsg->wParam, LOWORD(pMsg->lParam), pMsg->lParam); 
		return 0;
		}
	if(pMsg->message == WM_SYSKEYDOWN)
		{
		//OnSysKeyDown(pMsg->wParam, LOWORD(pMsg->lParam), pMsg->lParam); 
		return 0;
		}
	if(pMsg->message == WM_SYSKEYUP)
		{
		//OnSysKeyUp(pMsg->wParam, LOWORD(pMsg->lParam), pMsg->lParam); 
		return 0;
		}

	if(pMsg->message == WM_MOUSEHOVER )
		{
		//P2N("Mouse hover message\r\n");
		fired = true;

		TRACKMOUSEEVENT te;

		te.cbSize = sizeof(TRACKMOUSEEVENT);
		te.dwFlags = TME_HOVER | TME_CANCEL ;
		te.hwndTrack = this->m_hWnd;		
		::_TrackMouseEvent(&te);
		
		// See if it is time to pop up a menu
		//OnLButtonDown(0, lastmouse);

		ScanForAction(lastmouse, true);
		}

	return CGrayDlg::PreTranslateMessage(pMsg);
}

// CMenu

void CXrayM::OnPaint() 

{
	ASSERT(magic == MENU_MAGIC);

	CGrayDlg::OnPaint();

	CClientDC dc(this); // device context for painting

	CTextMode tm(&dc);
	
	dc.SetBkMode(TRANSPARENT);

	RECT rcorg; GetClientRect(&rcorg);
	RECT rc; CopyRect(&rc, &rcorg);

	rc.top += topgap; rc.left += leftgap; rc.right -= 2 * gap;
	
	CFont* oldFont = dc.SelectObject(&m_fontMenu);
	
	for(int loop = 0; loop < arr.GetSize(); loop++)
		{
		CXrayMitem *item = (CXrayMitem*)arr[loop];
	
		ASSERT(item->magic == MENUITEM_MAGIC);

		item->rect.left  = rcorg.left + gap;
		item->rect.right = rc.right - gap;
		item->rect.top	 = rc.top + gap;
		
		// Separator
		if(*item->itemstr == "")
			{
			rc.top += gap;
			CRect rc2(2 * gap + gap, rc.top, rc.right -  gap, rc.top + 2);
			dc.Draw3dRect(rc2, RGB(20, 20, 20), RGB(120, 120, 120));
			rc.top += gap;
			}
		else
			{
			CRect rc3(rc); rc3.bottom = rc3.top + textheight + 2 * gap;
			rc3.left = rcorg.left + gap; rc3.right = rcorg.right - gap; 

			if(item->selected)
				if(item->status & MF_GRAYED || item->status & MF_DISABLED)
					dc.FillSolidRect(&rc3, RGB(80, 80, 80));	
				else
					dc.FillSolidRect(&rc3, highlite);
			else
				dc.FillSolidRect(&rc3, dlgbackcol);

			if((item->status & MF_GRAYED))
				dc.SetTextColor(grayed);
			else if (item->status & MF_DISABLED)
				dc.SetTextColor(disabled);
			else
				dc.SetTextColor(RGB(0, 0, 0));
				
			CString cstr, comm, tabb; comm = cstr = *item->itemstr;
		
			// Get tabbed accel key
			int idx = cstr.Find('\t', 0);	
			if(idx >=0)
				{
				comm = cstr.Left(idx);
				tabb = cstr.Mid(idx + 1);
				}

			rc.top += gap;

			// Draw dot Box / Check Box

			if(item->checked)
				{
				//CPen ppx; ppx.CreatePen(PS_SOLID, 1, RGB( 0, 0, 0));
				CPen ppx; ppx.CreatePen(0, 1, checkmark);
				CPen *ppp = (CPen*)dc.SelectObject(ppx);
			
				CRect rc4(2*gap, rc.top, 2 * gap + 6, rc.top + 12);
				
				dc.MoveTo(rc4.left, rc4.top + rc4.Height()/2);
				dc.LineTo(rc4.left + rc4.Width()/2, rc4.bottom);
				dc.LineTo(rc4.right, rc4.top);
				//dc.LineTo(rc4.right + 4, rc4.top);
				}
			else
				{
				CRect rc4(2 * gap, rc.top + 6, 2 * gap + 4, rc.top + 10);
				dc.Draw3dRect(rc4, RGB(20, 20, 20), RGB(120, 120, 120));
				}
			
			int hh = dc.DrawText(comm, &rc, DT_LEFT  | DT_TOP);
			rc.right -= 2 * gap;
			dc.DrawText(tabb, &rc, DT_RIGHT | DT_TOP );
			rc.right += 2 * gap;
				
			if(item->submenu)
				{
				CRect rc5(rcorg.right - (3*gap), rc.top + gap/2 ,  rcorg.right -  2 * gap, rc.top + 10);

				pp[0].x = rc5.left,  pp[0].y = rc5.top;
				pp[1].x = rc5.right, pp[1].y = rc5.top + rc5.Height()/2;
				pp[2].x = rc5.left,  pp[2].y = rc5.bottom;
				pp[3].x = rc5.left,  pp[3].y = rc5.top;

				CBrush bb; bb.CreateSolidBrush(RGB(60, 60, 60));
				CBrush *cc = (CBrush*)dc.SelectObject(bb);
				CPen ppx; ppx.CreatePen(0,1,RGB( 60, 60, 60));
				CPen *ppp = (CPen*)dc.SelectObject(ppx);
				
				dc.Polygon(pp, 3);
				
				//dc.SelectObject(ppp);
				}

			rc.top += hh;
			rc.top += gap;
			}
		item->rect.bottom = rc.top;
		}

	dc.SelectObject(oldFont);
	
}

// ////////////////////////////////////////////////////////////////////////
/// Hide menu. Closing and releasing resources is automatically done when 
/// a new menu opens.

void CXrayM::Hide(int special)

{
	//P2N("hiding %p\r\n", this);

	ASSERT(magic == MENU_MAGIC);

	CGrayDlg::Hide();

	// If we are in a menu we want parent to go
	if(mparent)
		{
		if(IsWindow(mparent->m_hWnd))
			mparent->SetFocus();
		}
	//support.YieldToWinEx();

	// Root window, close all destroy
	//if(!parent)
	//	DestroyWindow();
}

void CXrayM::OnMouseMove(UINT nFlags, CPoint point) 

{
	//P2N("CXrayM::OnMouseMove\r\n");

	// Hittest	
	int in_item = false;

	// Moved more than hover pixels?
	if	(abs(capmouse.x - point.x) > 5 ||
			abs(capmouse.y - point.y) > 5)
		{
		tip.Hide();
		}

	lastmouse = point;
	
	for(int loop = 0; loop < arr.GetSize(); loop++)
		{
		CXrayMitem *item = (CXrayMitem*)arr[loop];

		ASSERT(item->magic == MENUITEM_MAGIC);


		if(PtInRect(&item->rect, point))
			{
			in_item = true;

			if(OldLoop != loop)
				{
				//P2N("Mouse changed into %s\r\n", item->str);
				if(subopen)
					{
					subopen->Hide(true);
					subopen = NULL;
					}
				// If not a separator
				if(*item->itemstr != "" )
					{
					for(int loop2 = 0; loop2 < arr.GetSize(); loop2++)
						{
						CXrayMitem *item2 = (CXrayMitem*)arr[loop2];
						item2->selected = false;
						}
					SetFocus();
					fired = false;
					tip.Hide();
					item->selected = true;
					InvalidateRect(&item->rect);
					//Invalidate();

					TRACKMOUSEEVENT te;

					if(!fired)
						{
						te.cbSize = sizeof(TRACKMOUSEEVENT);
						te.dwFlags = TME_LEAVE;
						te.hwndTrack = this->m_hWnd;
						::_TrackMouseEvent(&te);
					
						te.cbSize = sizeof(TRACKMOUSEEVENT);
						te.dwFlags = TME_HOVER;
						te.hwndTrack = this->m_hWnd;
						
						if(item->submenu)
							te.dwHoverTime = 200;
						else
							te.dwHoverTime = 1500;
					
						::_TrackMouseEvent(&te);
						}
					}
				OldLoop = loop;	
				break;
				}
			}
		}

	// Off of all items, make sure it is reflected
	if(!in_item)
		{
		if(OldLoop != -1)
			{
			//P2N("Exited items\r\n");
			}
		OldLoop = -1;
		}

	CGrayDlg::OnMouseMove(nFlags, point);
}

// ////////////////////////////////////////////////////////////////////////
//

void	CXrayM::MeasureMenu(int *ww, int *hh)

{
	*ww = 100;	*hh = 100;

	int len = 0;
	CString *str = NULL;

	// Find longest one
	for(int loop = 0; loop < arr.GetSize(); loop++)
		{
		CXrayMitem *item = (CXrayMitem*)arr[loop];

		ASSERT(item->magic == MENUITEM_MAGIC);

		int len2 = item->itemstr->GetLength();
		if(len2 > len)
			{
			len = len2; 
			str = item->itemstr;
			}
		}

	SIZE ts;
	if(str)
		{
		GetMenuTextSize(*str, ts);
		*ww = ts.cx;
		*hh = ts.cy;
		textheight = ts.cy;
		}
}

//////////////////////////////////////////////////////////////////////////
// Get main window handle, failover to desktop

CWnd	*CXrayM::SafeGetMainWnd()

{
	CWnd *pWnd = AfxGetMainWnd();            // Get main window

	// Fail over
	if (pWnd == NULL) 
		pWnd = CWnd::GetDesktopWindow();

	ASSERT(pWnd);
	pWnd->AssertValid();
	return pWnd;
}

//////////////////////////////////////////////////////////////////////////
/// Obtain the width/height of menu text:	

int		CXrayM::GetMenuTextSize(const char *str, SIZE &size)

{
	CWnd *pWnd = SafeGetMainWnd();
	CDC *pDC = pWnd->GetDC();			// Get device context
	ASSERT(pDC);
	pDC->AssertValid();

	m_fontMenu.AssertValid();

	CFont*	pOldFont = pDC->SelectObject (&m_fontMenu);
	
	CRect txtrc;		
	pDC->DrawText(str, strlen(str),	&txtrc, DT_SINGLELINE | DT_LEFT | DT_CALCRECT);

	size.cx = txtrc.right - txtrc.left;
	size.cy = txtrc.bottom - txtrc.top;

	//P2N("Menu Text '%s' size cx=%d cy=%d\r\n", str, size.cx, size.cy);

	// Select back old font, release the DC
	pDC->SelectObject (pOldFont);	pWnd->ReleaseDC(pDC);		

	return 0;
}


int CXrayM::OnCreate(LPCREATESTRUCT lpCreateStruct) 

{
	if (CGrayDlg::OnCreate(lpCreateStruct) == -1)
		return -1;

	//P2N("OnCreate\r\n");

	LOGFONT m_lf; ZeroMemory ((PVOID) &m_lf,sizeof (LOGFONT));

	// Get system menu font
	NONCLIENTMETRICS nm; nm.cbSize = sizeof (NONCLIENTMETRICS);
	VERIFY(SystemParametersInfo(SPI_GETNONCLIENTMETRICS,
				nm.cbSize,&nm,0)); 

	m_lf =  nm.lfMenuFont; 
	VERIFY(m_fontMenu.CreateFontIndirect (&m_lf));

	m_fontMenu.AssertValid();
	
	//tip.Create(IDD_DIALOG12, this);
	tip.Hide();

	return 0;
}

void CXrayM::OnLButtonDown(UINT nFlags, CPoint point) 

{
	ASSERT(magic == MENU_MAGIC);
	CGrayDlg::OnLButtonDown(nFlags, point);
}

void CXrayM::ScanForAction(CPoint &point, int NoClick) 

{
	for(int loop = 0; loop < arr.GetSize(); loop++)
		{
		CXrayMitem *item = (CXrayMitem*)arr[loop];

		ASSERT(item->magic == MENUITEM_MAGIC);

		if(PtInRect(&item->rect, point))
			{
			if(! ( (item->status & MF_DISABLED) || (item->status &MF_GRAYED)) )
				{
				if(item->submenu)
					{
					OpenSubMenu(item, point);
					}
				else
					{
					if(!NoClick)
						{
						//P2N("Cliked on item ID %d %s\r\n", item->command, item->str);
						
						ExecItem(item);
						}
					else
						{	
						// Show tooltip
						//P2N("Tooltip time for %s: '%s'\r\n", item->str, item->tipstr);
						capmouse = point;
						// Autosize and autolocate
						CPoint screen = lastmouse;	ClientToScreen(&screen); 
						screen.x += 12; screen.y += 24;
						tip.Show(*item->tipstr, screen);						
						}
					}
				}
			break;
			}
		}
}

void CXrayM::OnRButtonDown(UINT nFlags, CPoint point) 

{
	ASSERT(magic == MENU_MAGIC);
	//ScanForAction(point, false); 
	CGrayDlg::OnRButtonDown(nFlags, point);
}

/// Clear all loaded menu items

void CXrayM::Clear()

{
	//P2N("Calling Clear for %p\r\n", this);
	//DumpMenu();

	ASSERT(magic == MENU_MAGIC);

	// Free menu memory
	for(int loop = arr.GetUpperBound(); loop >= 0; loop--)
		{
		CXrayMitem *item = (CXrayMitem*)arr[loop];

		ASSERT(item->magic == MENUITEM_MAGIC);

		// Kill submenus of this one
		if(item->submenu)
			{
			// Remove the deleted item from further links
			for(int loop2 = loop-1; loop2 >= 0; loop2--)
				{
				CXrayMitem *item2 = (CXrayMitem*)arr[loop2];

				if(item2->submenu == item->submenu)
					{
					//P2N("Killed duplicate submenu %p\r\n", item->submenu);
					item2->submenu = NULL;
					}
				}
			delete item->submenu;
			//item->submenu->DestroyWindow();
			//P2N("Scanning global array %p\r\n", this);
	
			item->submenu = NULL;
			}
		delete item;
		}
	arr.RemoveAll();
}

/// Set menu Item checked state by index

void CXrayM::SetCheck(int idx, int check)

{
	if(idx < arr.GetSize() && idx >= 0)
		{	
		((CXrayMitem*)arr[idx])->checked = check;
		}
}

/// Diagnostic function. Dump to MxPad

void CXrayM::DumpMenu(CXrayM *menu, CString indent)

{
	P2N("%s--------------------------------------------------------\r\n", indent);
	P2N("%sMenu %p\r\n", indent, this);

	for(int loop = 0; loop < menu->arr.GetSize(); loop++)
		{
		CXrayMitem *item = (CXrayMitem*)menu->arr[loop];

		ASSERT(item->magic == MENUITEM_MAGIC);

		P2N("%sstr: '%s'\r\n", indent, item->itemstr);
		//P2N("%stip: '%s'\r\n", indent, item->tipstr);

		P2N("%scom=%-8d sub=%-8x  stat=%-8d  check=%-8d  key=%-8s \r\n",
					indent, 
						item->command, item->submenu, 
							item->status, item->checked, item->key);
		if(item->submenu)
			{
			indent += "    ";
			DumpMenu(item->submenu, indent);
			indent = indent.Mid(0, indent.GetLength() - 4);
			}
		}
	P2N("%s--------------------------------------------------------\r\n", indent);
	P2N("%sMenu %p End\r\n", indent, this);
}


//////////////////////////////////////////////////////////////////////////
/// Return the index of the menu item by command
///

int CXrayM::FindMenuItem(int command)

{
	ASSERT(magic == MENU_MAGIC);
	
	for(int loop = 0; loop < arr.GetSize(); loop++)
		{
		CXrayMitem *item = (CXrayMitem*)arr[loop];

		ASSERT(item->magic == MENUITEM_MAGIC);

		if(item->command == command)
			{
			return(loop);
			}
		}
	return -1;
}

//////////////////////////////////////////////////////////////////////////
/// Return the index of the menu item by menu str
///
/// Case sensitive search
///

int CXrayM::FindMenuItem(const char *str)

{
	for(int loop = 0; loop < arr.GetSize(); loop++)
		{
		CXrayMitem *item = (CXrayMitem*)arr[loop];

		ASSERT(item->magic == MENUITEM_MAGIC);

		if(strstr(*item->itemstr, str) != NULL)
			{
			P2N("CXrayM::FindMenuItem loop=%d\r\n", loop);
			return(loop);
			}
		}
	
	return -1;
}

// ////////////////////////////////////////////////////////////////////////
/// Return the submenu searched by menu str
///
/// Case sensitive search
///

CXrayM *CXrayM::FindSubmenu(const char *str)

{
	ASSERT(magic == MENU_MAGIC);
	
	for(int loop = 0; loop < arr.GetSize(); loop++)
		{
		CXrayMitem *item = (CXrayMitem*)arr[loop];

		ASSERT(item->magic == MENUITEM_MAGIC);

		if(item->submenu)
			{
			if(strstr(*item->itemstr, str) != NULL)
				{
				return(item->submenu);
				}
			}
		}
	return NULL;
}

// ///////////////////////////////////////////////////////////////////////
/// Return the submenu searched by submenu idx

CXrayM *CXrayM::FindSubmenu(int idx)

{
	ASSERT(magic == MENU_MAGIC);
	
	for(int loop = 0; loop < arr.GetSize(); loop++)
		{
		CXrayMitem *item = (CXrayMitem*)arr[loop];

		ASSERT(item->magic == MENUITEM_MAGIC);

		if(item->submenu)
			{
			if(idx-- == 0)
				{
				return(item->submenu);
				}
			}
		}
	return NULL;
}

// ///////////////////////////////////////////////////////////////////////
/// Delete menuitem, specified by index.

void	CXrayM::DelMenuItem(int idx)

{
	ASSERT(magic == MENU_MAGIC);
	
	if(idx < arr.GetSize() && idx >= 0)
		{
		CXrayMitem *item = (CXrayMitem*)arr[idx];

		ASSERT(item->magic == MENUITEM_MAGIC);

		if(item->submenu)
			{
			//P2N("Del Submenu\r\n");
			}
		arr.RemoveAt(idx);
		}
}

/// Return the number of menu items. 

int CXrayM::GetNumItems()

{
	ASSERT(magic == MENU_MAGIC);
	
	return(arr.GetSize());
}

/// Load a menu from resource, skip number of items from head. (for right click)

BOOL CXrayM::LoadMenu(int nResource, int skip)

{
	return(CXrayM::LoadMenu(MAKEINTRESOURCE(nResource), skip));
}

#define MAXMENUDEPTH 12

// ///////////////////////////////////////////////////////////////////////
//
// Function name:   CXrayM::LoadMenu
// Description:     Load menu from resource
// Return type:     BOOL 
// Argument:        LPCTSTR lpszResourceName
// Argument:        int skip
// Caveat:			Submenus deeper than 24 are not loaded
//
// ////////////////////////////////////////////////////////////////////////

/// Load a menu from resource, skip number of items from head.

BOOL CXrayM::LoadMenu(LPCTSTR lpszResourceName, int skip)

{
	ASSERT(magic == MENU_MAGIC);
	
	//P2N("Loading %d \r\n", lpszResourceName);

	int menucnt = 0;

	if(m_loaded)
		return TRUE;

	Clear();

	ASSERT_VALID(this);	ASSERT(lpszResourceName != NULL);
	
	// Find the Menu Resource:
	HINSTANCE	hInst =	 AfxFindResourceHandle(lpszResourceName, RT_MENU);
	HRSRC		hRsrc =  ::FindResource(hInst,lpszResourceName, RT_MENU);

	if (hRsrc == NULL)
		{
		hInst = NULL;
		hRsrc = ::FindResource(hInst, lpszResourceName, RT_MENU);
		}

	if(hRsrc == NULL)
		{	
		MBOXP("Could not open menu resource %d", lpszResourceName);
		return FALSE;
		}
	
	// Load the Menu Resource:	
	HGLOBAL hGlobal = LoadResource(hInst, hRsrc);
	if(hGlobal == NULL)
		{
		MBOXP("Could not load menu resource %d", lpszResourceName);
		return FALSE;
		}	

	// Get Item template Header, and calculate offset of MENUITEMTEMPLATES:
	MENUITEMTEMPLATEHEADER *pTpHdr = 
			(MENUITEMTEMPLATEHEADER*)LockResource(hGlobal);

	BYTE* pTp=(BYTE*)pTpHdr + 
			(sizeof(MENUITEMTEMPLATEHEADER) + pTpHdr->offset);
	
	int seen_popup = 0, count = 0;
	CXrayM *lastclass[MAXMENUDEPTH]; lastclass[seen_popup] = this;
	int		lastend[MAXMENUDEPTH]; memset(lastend, 0, sizeof(lastend));

	// Processing of Menu Item Templates:
	while(true)
		{
		WORD    wFlags = 0, wID  = 0, uFlags;	
		WCHAR	*wstr;

		// Extract flags, ID and command string
		wFlags = *((WORD*)pTp); pTp += sizeof(WORD);
		uFlags = wFlags & ~(WORD)MF_END;

		if(wFlags & MF_POPUP)
			{
			wID = 0;
			}			
		else
			{
			// Step over the ID
			wID = *((WORD*)pTp); pTp += sizeof(WORD);
			}
		wstr = (WCHAR*)pTp;

		// Step over the string
		pTp += (wcslen(wstr) + 1) * sizeof(wchar_t);	
	
		// Skip not needed entry
		if(skip)
			{
			seen_popup = 0; count = 0;
			if(skip--)
				continue;
			}

		// Get an ordinary string we are not unicode
		CString	str; UnicodeToAnsi(wstr, str);
		
		//P2N("flags=%-5d id=%-5d end=%d %p '%S' '%s' \r\n", uFlags, wID, wFlags & MF_END,
		//								lastclass[seen_popup], wstr, str);

		if(wFlags & MF_POPUP)
			{
			if(seen_popup < sizeof(lastclass) / sizeof(void*))
				{
				CXrayM *sm = new CXrayM; ASSERT(sm);

				lastclass[seen_popup]->AppendSubMenu(str, sm, uFlags);	
				sm->mparent = lastclass[seen_popup];
					{
					seen_popup++;
					lastclass[seen_popup] = sm;
					}
				
				// Ended theis popup
				if(wFlags & MF_END)
					{
					// Tell the array this menu hes been terminated
					lastend[seen_popup] = true;
					}
				}
			else
				{
				lastclass[seen_popup]->AppendMenuItem("Warning: Exceeded menu stack depth"); 
				//P2N("Exceeded menu stack depth\r\n");
				}
			}
		else
			{
			int idx = lastclass[seen_popup]->AppendMenuItem(str, wID, uFlags, 
					uFlags & MF_CHECKED ? true : false);

			// Load tooltip
			if(wID)
				{
				CString ttip, cstr(str), addtip, tabb, comm(str);

				ttip.LoadString(wID);

				if(str != "")
					{
					// Get tabbed accel key, add to tooltip				
					int sidx = cstr.Find('\t', 0);	
					if(sidx >= 0)
						{
						comm = cstr.Left(sidx);
						tabb = cstr.Mid(sidx + 1);
						if(tabb != "")
							{
							// Set the key item as well
							lastclass[seen_popup]->SetItemKey(idx, tabb);
							addtip.Format("\r\nShortcut key: %s", tabb);
							}
						}
					}
				CString tipsum(comm + "\r\n");
				if(ttip != "")	tipsum += ttip;
				if(addtip != "") tipsum += addtip;

				lastclass[seen_popup]->SetToolTip(idx, tipsum);

				if(wFlags & MF_END)
					{
					int endit = true;
					for(int loop = seen_popup; loop >= 0; loop--)
						{
						// All seen submenus terminated?
						if(!lastend[seen_popup])
							{
							endit = false;
							}
						}
					if(endit)
						break;

					seen_popup--;				
					}
				}
			}

		// Safety valve
		if(seen_popup < 0)
			break;
		}

	m_loaded = true;

	//DumpMenu(this);

	return(TRUE);
}

/// Set menu item key by index

void CXrayM::SetItemKey(int idx, const char *newstr)

{
	if(idx < arr.GetSize() && idx >= 0)
		{	
		strncpy(((CXrayMitem*)arr[idx])->key, newstr, 
				sizeof( ((CXrayMitem*)arr[idx])->key ));
		}
}

/// Set menu item text by index

void CXrayM::SetItemText(int idx, const char *newstr)

{
	ASSERT(magic == MENU_MAGIC);
	
	if(idx < arr.GetSize() && idx >= 0)
		{	
		*((CXrayMitem*)arr[idx])->itemstr = newstr;
		}
}

/// Set menu item tooltip by index

void CXrayM::SetToolTip(int idx, const char *tipstr)

{
	ASSERT(magic == MENU_MAGIC);
	
	if(idx < arr.GetSize() && idx >= 0)
		{	
		*((CXrayMitem*)arr[idx])->tipstr = tipstr;
		}
}

/// Set menu item tooltip by menu string

void CXrayM::SetToolTip(const char *str, const char *tipstr)

{
	ASSERT(magic == MENU_MAGIC);
	
	int idx = FindMenuItem(str);

	if(idx < arr.GetSize() && idx >= 0)
		{	
		*((CXrayMitem*)arr[idx])->tipstr = tipstr;
		}
}

void CXrayM::PostNcDestroy() 

{
	CGrayDlg::PostNcDestroy();

	//P2N("CXrayM::PostNcDestroy %p\r\n", this); 

	if(!mparent)
		delete this;
}

/// Get menu item text by index

void	CXrayM::GetMenuItemText(int idx, CString &txt)

{
	ASSERT(magic == MENU_MAGIC);

	if(idx < arr.GetSize() && idx >= 0)
		{	
		txt = *((CXrayMitem*)arr[idx])->itemstr;
		}
	else
		{
		txt = "Err";
		}
}

// ////////////////////////////////////////////////////////////////////////
//
// UnicodeToAnsi converts the Unicode string pszW to an ANSI string
// and returns the ANSI string through str. 
//

DWORD   CXrayM::UnicodeToAnsi(LPWSTR pszW, CString &str)

{
    ULONG cbAnsi, cCharacters;
    DWORD dwError = NOERROR;

    // If input is null then just return an empty string
    if (pszW == NULL)
        {
		str = "";
        return NOERROR;
        }
    cCharacters = wcslen(pszW) + 1;		// + terminating NULL

    // Determine number of bytes to be allocated for ANSI string. 
    cbAnsi = cCharacters * 2;

    char *ptr =  str.GetBuffer(cbAnsi); ASSERT(ptr);

    // Actual conversion here:
    if (0 == WideCharToMultiByte(CP_ACP, 0, pszW, cCharacters, ptr,
    cbAnsi, NULL, NULL))
        {
        dwError = GetLastError();
		*ptr = '\0';
        }

	str.ReleaseBuffer();
	return dwError;
}

void CXrayM::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 

{
	//P2N("CXrayM::OnChar(UINT nChar='%c' %d\r\n", nChar, nChar);	
	CGrayDlg::OnChar(nChar, nRepCnt, nFlags);
}

void CXrayM::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 

{
	int loop;

	//P2N("CXrayM::OnKeyDown(UINT nChar='%c' %d\r\n", nChar, nChar);

	tip.Hide();

	TRACKMOUSEEVENT te;

	te.cbSize = sizeof(TRACKMOUSEEVENT);
	te.dwFlags = TME_HOVER | TME_CANCEL ;
	te.hwndTrack = this->m_hWnd;		
	::_TrackMouseEvent(&te);	

	if(nChar >= 'A' && nChar <= 'Z')
		{
		//P2N("CXrayM::OnKeyDown LETTER\r\n");

		int sel = false, iter = 0, lastsel = GetSelected();

		if(lastsel == -1)
			loop = 0;
		else 
			loop = lastsel + 1;

		while(true)
			{
			for(; loop < arr.GetSize(); loop++)
				{
				CXrayMitem *item = (CXrayMitem*)arr[loop];

				if(IsItemSelectAble(item))
					{
					if(toupper(*item->itemstr[0]) == (int)nChar)
						{
						//P2N("Matching letter in %s\r\n", item->itemstr);

						if(lastsel >= 0)
							{
							CXrayMitem *item2 = (CXrayMitem*)arr[lastsel];
							item2->selected = false;
							}
						item->selected = true;
						sel = true;
						Invalidate();
						break;
						}
					}
				}

			if(sel)
				break;
			loop = 0;
			
			// Only span it twice (like flowover from end)
			if(iter++ > 0)
				break;
			}
		}
	else if(nChar >= '0' && nChar <= '9')
		{
		//P2N("CXrayM::OnKeyDown NUMBER\r\n");

		}
	else switch(nChar)
		{
		case  VK_LEFT:
			// Hide all non top level menus
			if(mparent)
				Hide();
		break;

		case  VK_ESCAPE:
			//P2N("CXrayM::OnKeyDown VK_ESC\r\n");
			Hide();
		break;
		
		case  VK_RIGHT:
		case  VK_SPACE:
		case  VK_RETURN:
			for(loop = 0; loop < arr.GetSize(); loop++)
				{
				CXrayMitem *item = (CXrayMitem*)arr[loop];

				if(item->selected)
					{
					if(item->submenu)
						{
						CPoint point(pp[0].x, pp[0].y);
						OpenSubMenu(item, point);
						}
					else
						{
						if(nChar != VK_RIGHT)
							ExecItem(item);	break;
						}
					break;
					}
				}
			//P2N("CXrayM::OnKeyDown VK_SPACE VK_RETURN \r\n");
		break;
		
		case  VK_UP:
			GotoPrev();
			//P2N("CXrayM::OnKeyDown VK_UP\r\n");
		break;
		
		case  VK_DOWN:
			GotoNext();
			//P2N("CXrayM::OnKeyDown VK_DOWN\r\n");
		break;
		
		case  VK_PRIOR:
			for(loop = 0; loop < 5; loop++)
				GotoPrev();
			//P2N("CXrayM::OnKeyDown VK_PRIOR\r\n");
		break;
		
		case  VK_NEXT:
			for(loop = 0; loop < 5; loop++)
				GotoNext();
			//P2N("CXrayM::OnKeyDown VK_NEXT\r\n");
		break;
		
		case  VK_HOME:
			//P2N("CXrayM::OnKeyDown VK_HOME\r\n");
		break;

		case  VK_END:
			//P2N("CXrayM::OnKeyDown VK_END\r\n");
		break;

		//case  VK_LEFT:
		//	P2N("CXrayM::OnKeyDown VK_LEFT\r\n");
		//break;

		//case  VK_RIGHT:
		//	P2N("CXrayM::OnKeyDown VK_RIGHT\r\n");
		//break;
		}

	CGrayDlg::OnKeyDown(nChar, nRepCnt, nFlags);
}

//////////////////////////////////////////////////////////////////////////	
// Internals

int	CXrayM::IsItemSelectAble(CXrayMitem *item)

{
	ASSERT(item->magic == MENUITEM_MAGIC);

	if(item->status & MF_GRAYED || 
		item->status & MF_DISABLED || 
			*item->itemstr == "")
		return false;
	else
		return true;				
}

int	CXrayM::IsAnySelected()

{
	int sel = false;
	for(int loop = 0; loop < arr.GetSize(); loop++)
		{
		CXrayMitem *item = (CXrayMitem*)arr[loop];
		if(item->selected)
			{
			sel = true;	break;
			}
		}
	return sel;
}

int	CXrayM::GetSelected()

{
	int sel = -1;
	for(int loop = 0; loop < arr.GetSize(); loop++)
		{
		CXrayMitem *item = (CXrayMitem*)arr[loop];
		if(item->selected)
			{
			sel = loop;	break;
			}
		}
	return sel;
}

int	CXrayM::ExecItem(CXrayMitem *item)

{
	ASSERT(item->magic == MENUITEM_MAGIC);
	
	if(!item->command)
		return 0;

	if(sendto)
		sendto->PostMessage(WM_COMMAND, 
						((DWORD)item->command), 
							(DWORD)m_hWnd);
	else
		AfxGetMainWnd()->PostMessage(WM_COMMAND, 
						((DWORD)item->command), 
							(DWORD)m_hWnd);
	support.YieldToWinEx();

	// Hide parents:
	if(mparent)
		mparent->Hide();

	Hide();

	return 0;
}

int	CXrayM::GotoNext()

{
	if(!IsAnySelected())
		{
		// Nothing selected, select first selectable
		for(int loop = 0; loop < arr.GetSize(); loop++)
			{
			if(IsItemSelectAble((CXrayMitem*)arr[loop]))
				{
				((CXrayMitem*)arr[loop])->selected = true;
				Invalidate();
				break;
				}
			}
		return 0;
		}

	for(int loop = 0; loop < arr.GetSize(); loop++)
		{
		CXrayMitem *item = (CXrayMitem*)arr[loop];
		if(item->selected)
			{
			// At End?
			if(loop < arr.GetSize() - 1)
				{
				item->selected = false;
				for(int loop2 = loop + 1; loop2 < arr.GetSize(); loop2++)
					{
					CXrayMitem *item2 = (CXrayMitem*)arr[loop2];
					if(IsItemSelectAble(item2))
						{
						item2->selected = true;
						break;
						}
					}
				Invalidate();
				break;
				}
			}
		}

	return 0;
}


int	CXrayM::GotoPrev()

{
	if(!IsAnySelected())
		{
		// Select Last
		for(int loop2 = arr.GetSize() - 1; loop2 >= 0; loop2--)
			{
			CXrayMitem *item2 = (CXrayMitem*)arr[loop2];
			if(IsItemSelectAble(item2))
				{
				item2->selected = true;
				break;
				}
			}
		Invalidate();
		return 0;
		}

	for(int loopm = arr.GetSize() - 1; loopm >= 0; loopm--)
		{
		CXrayMitem *item = (CXrayMitem*)arr[loopm];
		if(item->selected)
			{
			// At the Beginning
			if(loopm > 0)
				{
				item->selected = false;
				for(int loop2 = loopm-1; loop2 >= 0; loop2--)
					{
					CXrayMitem *item2 = (CXrayMitem*)arr[loop2];
					if(IsItemSelectAble(item2))
						{
						item2->selected = true;
						break;
						}
					}
				Invalidate();
				break;
				}
			else
				{
				if(closeonup)
					Hide();
				}
			}
		}
	return 0;
}

int		CXrayM::OpenSubMenu(CXrayMitem *item, CPoint &point)

{
	ASSERT(magic == MENU_MAGIC);
	
	//P2N("CXrayM::OpenSubMenu %p\r\n", item);

	ASSERT(item->magic == MENUITEM_MAGIC);

	RECT rc; GetClientRect(&rc);

	CPoint point2(rc.right, rc.top); ClientToScreen(&point2);
	CPoint point3(point); ClientToScreen(&point3);

	//P2N("Cliked on submenu %d %s\r\n", item->submenu, item->str);

	subopen = item->submenu;
	item->submenu->mparent = this;
	item->submenu->Show(point2.x, point3.y - topgap, 1);

	return 0;
}

void CXrayM::OnLButtonUp(UINT nFlags, CPoint point) 

{
	CGrayDlg::OnLButtonUp(nFlags, point);
	ScanForAction(point, false); 
}

void CXrayM::OnRButtonUp(UINT nFlags, CPoint point) 

{	
	CGrayDlg::OnRButtonUp(nFlags, point);
	ScanForAction(point, false); 
}

// ////////////////////////////////////////////////////////////////////////
/// Set Item's Enabled/Disabled status by idx
/// Return old status.

int		CXrayM::SetItemStatus(int idx, int status)

{
	ASSERT(magic == MENU_MAGIC);
	
	int ret  = -1;

	if(idx < arr.GetSize() && idx >= 0)
		{
		CXrayMitem *item = (CXrayMitem*)arr[idx];

		ASSERT(item->magic == MENUITEM_MAGIC);

		ret = item->status;
		item->status = status;
		}
	return ret;
}

