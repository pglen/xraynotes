
/* =====[ CSimpleGrid.cpp ]========================================== 
                                                                             
   Description:     The dbtest project, implementation of the CSimpleGrid.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  12/19/2007  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// CSimpleGrid.cpp : implementation file
//

#include "stdafx.h"
#include "dbtest.h"
#include "support.h"

#include "SimpleGrid.h"

#include "mxpad.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CSimpleGrid, CWnd)
	//{{AFX_MSG_MAP(CSimpleGrid)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_RBUTTONDOWN()
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSimpleGrid

CSimpleGrid::CSimpleGrid()

{
	firstpaint = true;

	vrows = rows = 0;
	vcols = cols = 0;
	currrow = currcol = 0;
	init_alloc = first_bold = wasbold = 0;
	startrow = startcol = 0;
	capped = fired = 0;
	init_row = 10, init_col = 15;
	head_size = 5;
	maxwidth	= 64;

	//P2N("Construct CSimpleGrid::CSimpleGrid()\r\n");
	
	LOGFONT		m_lf;

	cell_www = cell_hhh = 0;

	memset(&charrc, 0, sizeof(charrc));

	::GetObject((HFONT)GetStockObject(DEFAULT_GUI_FONT), sizeof(m_lf), &m_lf);

	m_font.CreateFontIndirect(&m_lf);
	m_lf.lfWeight  = FW_BOLD;
	m_fontb.CreateFontIndirect(&m_lf);

	//tip.Create(this);
}

//////////////////////////////////////////////////////////////////////////

CSimpleGrid::~CSimpleGrid()

{
	for(int loop = 0; loop < rows ; loop++)
		{
		CPtrArray *col = (CPtrArray *)rowarr[loop];

		for(int loop2 = 0; loop2 < cols; loop2++)
			{
			cell *ccc = (cell *)col->GetAt(loop2);

			ASSERT(ccc->magic == CELL_MAGIC);

			CString *str = (CString *)ccc->content;

			//P2N("Destructing cell %d %d %s\r\n", ccc->cellrow, ccc->cellcol, *str);

			delete str;
			delete ccc;
			}
		delete col;
		}
}

/////////////////////////////////////////////////////////////////////////////
// CSimpleGrid message handlers

void CSimpleGrid::OnPaint() 

{
	if(firstpaint)
		{
		firstpaint = false;
		FirstPaint();
		}

	CPaintDC dc(this); // device context for painting
	RECT rect;	GetClientRect(&rect);

	//rect.left += 10;
	//rect.top += 10;
	//int www = (rect.right - rect.left) / cols;
	//int hhh = (rect.bottom - rect.top) / rows;

	CPen dashed(PS_SOLID, 0, RGB(0x80, 0x80, 0x80));
	CPen *old =  dc.SelectObject( &dashed);

	RECT rc4; rc4.left = rc4.top = rc4.bottom = rc4.right = head_size;

	for(int loop = startrow; loop < rows ; loop++)
		{
		CPtrArray *col = (CPtrArray *)rowarr[loop];

		cell *ccc;
		for(int loop2 = startcol; loop2 < cols; loop2++)
			{
			ccc = (cell *)col->GetAt(loop2);

			CString *str = (CString *)ccc->content;

			rc4.right  = rc4.left + ccc->ww;
			rc4.bottom = rc4.top  + ccc->hh; 

			//  t-  r|  b-  l|
			
			dc.MoveTo(rc4.left,  rc4.top);
			dc.LineTo(rc4.right, rc4.top);
			dc.LineTo(rc4.right, rc4.bottom);
			dc.LineTo(rc4.left, rc4.bottom);
			dc.LineTo(rc4.left, rc4.top);

			rc4.left += ccc->ww; 

			// Clip after last displyed column
			if(rc4.left  >= rect.right)
				break;

			}	
		rc4.top += ccc->hh; 
		rc4.left = head_size;

		// Clip after last displyed line
		if(rc4.top + ccc->hh >= rect.bottom)
			break;
		}

	dc.SelectObject(old);

	dc.SetBkMode(TRANSPARENT);

	CFont	*oldFont, *oldFont2;
	oldFont = dc.SelectObject(&m_font);
	
	rc4.left = rc4.top = rc4.bottom = rc4.right = head_size;

	for(loop = startrow; loop < rows; loop++)
		{
		if(wasbold)
			{
			dc.SelectObject(oldFont2);
			wasbold = false;
			}
		if(loop == 0 && first_bold)
			{
			oldFont2 = dc.SelectObject(&m_fontb);
			wasbold = true;
			}
		
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
			if(rc4.left >= rect.right)
				break;

			//P2N("Drawing cell %d %d '%s'\r\n", ccc->cellrow, ccc->cellcol, *str);
			}
		rc4.top += ccc->hh;
		rc4.left = head_size;

		// Clip to last displyed line
		if(rc4.top + ccc->hh >= rect.bottom)
			break;
		}

	dc.SelectObject(oldFont);
}

//////////////////////////////////////////////////////////////////////////

void CSimpleGrid::AddStr(int qrow, int qcol, const char *str, int inval)

{
	//P2N("adding %d %d %s\r\n", qrow, qcol, str);

	int expand = 0;
	if(qrow + 1> rows)
		{
		//P2N("Row overflow\r\n");
		vs.EnableWindow();
		vs.SetScrollRange(0, qrow);
		expand = true;
		//return;
		}
		
	if( qcol + 1 > cols)
		{
		//P2N("Col overflow\r\n");
		hs.EnableWindow();
		hs.SetScrollRange(0, qcol);
		expand = true;
		//return;
		}

	// Expand grid to accomodate
	if(expand)
		{
		ReAlloc( max(qrow + 1, rows), max(qcol + 1, cols) );
		//return;
		}
	
	CPtrArray *colarr = (CPtrArray *)rowarr[qrow];
	ASSERT(colarr);

	//P2N("getting col %d max=%d\r\n", qcol, colarr->GetSize());
	cell *ccc = (cell *)colarr->GetAt(qcol); 

	ASSERT(ccc->magic == CELL_MAGIC);

	//RECT rc4; 
	//rc4.left = ccc->xx; rc4.top = ccc->yy;
	//rc4.bottom = ccc->yy + ccc->hh; rc4.right = ccc->xx + ccc->ww;

	*((CString *)ccc->content) = str;

	if(inval)
		{
		//InvalidateRect(&rc4);
		Invalidate();
		support.YieldToWinEx();
		}
}


BOOL CSimpleGrid::OnEraseBkgnd(CDC* pDC) 

{
	RECT rect, m_rDataBox;
	
	GetClientRect(&rect);

	CopyRect(&m_rDataBox, &rect);
	
	//P2N("Erase background\r\n");	

	CBrush NewBrush(RGB(192,192,192)) ; 
	pDC->SetBrushOrg(0,0) ;
	CBrush* pOldBrush = (CBrush*)pDC->SelectObject(&NewBrush);   
	pDC->PatBlt(rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, PATCOPY);
	pDC->SelectObject(pOldBrush);

	//return CWnd::OnEraseBkgnd(pDC);
	return true;
}

void CSimpleGrid::OnLButtonDown(UINT nFlags, CPoint point) 

{
	//P2N("Left button down\r\n");

#if 0
	TRACKMOUSEEVENT te;

	te.cbSize = sizeof(TRACKMOUSEEVENT);
	te.dwFlags = TME_LEAVE;
	te.hwndTrack = this->m_hWnd;
	te.dwHoverTime = 0;

	::_TrackMouseEvent(&te);
#endif

	SetFocus();

	int roww, coll;	CRect rc;
	Pt2Cell(point, &roww, &coll, &rc);

	//P2N("Pt2Cell(row=%d, col=%d CRect top=%d left=%d bottom=%d right=%d\r\n", 
	//			roww, coll, rc.top, rc.left, rc.bottom, rc.right);

	// Mark current cell
	SetCurr(point);

	CWnd::OnLButtonDown(nFlags, point);
}

BOOL CSimpleGrid::PreTranslateMessage(MSG* pMsg) 

{
	//if(!mxpad.is_junk(pMsg->message))
	//	P2N("Message: %d %s\r\n", pMsg->message, mxpad.num2msg(pMsg->message));

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

	//////////////////////////////////////////////////////////////////////
	
	if(pMsg->message == WM_MOUSELEAVE)
		{
		tip.Hide();
		//P2N("Mouse leave message\r\n");
		capped = false;
		fired = false;
		return 0;
		}

	if(pMsg->message == WM_MOUSEHOVER )
		{
		//P2N("Mouse hover message\r\n");
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

			//tip.Move(screen);
			//tip.SetText("off cell");
			
			// Find the text for this one
	
			CString str2 ("");
			CString *str = &str2;

			RECT rc4; SetRectEmpty(&rc4);

			for(int loop = startrow; loop < rows; loop++)
				{
				cell *ccc;
				CPtrArray *col = (CPtrArray *)rowarr[loop];

				for(int loop2 = startcol; loop2 < cols; loop2++)
					{
					ccc = (cell *)col->GetAt(loop2);

					rc4.right  = rc4.left + ccc->ww;
					rc4.bottom = rc4.top  + ccc->hh; 

					CRect rc5(rc4);
					
					if(rc5.PtInRect(mouse))
						{
						str = (CString *)ccc->content;
						str2 = *str;

						//P2N("Found string hit %s\r\n", str2);
						break;
						}
					rc4.left += ccc->ww; 
					}

				rc4.top += ccc->hh;
				rc4.left = 0;
				}

			if(str2 != "")
				{
				// Autosize and autolocate
				CPoint screen = mouse;
				ClientToScreen(&screen); screen.x += 12; screen.y += 24;
				capped = false;
				tip.Show(str2, screen);
				}
			}
		}

	return CWnd::PreTranslateMessage(pMsg);
}

void CSimpleGrid::OnMouseMove(UINT nFlags, CPoint point) 

{
	//P2N("Mouse Move xx=%d yy=%d\r\n", point.x, point.y);

	mouse.x = point.x;
	mouse.y = point.y;

	TRACKMOUSEEVENT te;

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
			if	(ABS(capmouse.x - point.x) > 5 ||
					ABS(capmouse.y - point.y) > 5)
				{
				tip.ShowWindow(false);
				//P2N("Move in no tracking\r\n");
				fired = false;
				capped = false;
				}
			}
		}
		
	CWnd::OnMouseMove(nFlags, point);
}

int CSimpleGrid::OnCreate(LPCREATESTRUCT lpCreateStruct) 

{
	//P2N("Create function called\r\n");
	
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
		
	// TODO: Add your specialized creation code here
	//P2N("CSimpleGrid::Created window\r\n");
	return 0;
}

void CSimpleGrid::Clear()

{
	for(int loop = 0; loop < rows; loop++)
		for(int loop2 = 0; loop2 < cols; loop2++)
			AddStr(loop, loop2, "", false);

	AutoSize();
	Invalidate();
}

//////////////////////////////////////////////////////////////////////////
// 
#if 0

void CSimpleGrid::AllocX(int newrow, int newcol)

{
	//P2N("Realloc from %d:%d to %d:%d\r\n", rows, cols, newrow, newcol);

	RECT rect; GetClientRect(&rect);

	if(!init_alloc)
		{
		init_alloc = true;
		cell_www = ((rect.right - rect.left) - sw) / newcol;
		cell_hhh = ((rect.bottom - rect.top) - sh) / newrow;
		vcols = newcol;
		vrows = newrow;
		}

	int pad = 0;

	// Add rows blindly
	for(int loop = rows; loop < newrow + 1; loop++)
		{
		CPtrArray *col = new CPtrArray;

		//P2N("\r\nrow alloc Row %d\r\n", loop);

		for(int loop2 = 0; loop2 < newcol + 1; loop2++)
			{
			cell *ccc = new cell;
			//ASSERT(cell);

			//P2N("Col %d ", loop2);

			// Initialize cell
			memset(ccc, 0, sizeof(cell));
			
			ccc->magic = CELL_MAGIC;

			ccc->ww = cell_www;
			ccc->hh = cell_hhh;
			ccc->cellrow = loop;
			ccc->cellcol = loop2;
			ccc->color = RGB(0,0,0);

			CString *str = new CString; 
			//str->Format("x%d:%d", loop, loop2);
			ccc->content = str;

			col->Add(ccc);
			}
		P2N("row Alloc cols %d\r\n", col->GetSize() );
		rowarr.Add(col);

		// If there was a row 
		cols = newcol;
		}

	rows = newrow;

	// Add cols diferencial
	for(loop = 0; loop < rows + 1; loop++)
		{
		CPtrArray *col = (CPtrArray *)rowarr[loop];

		//P2N("\r\ncol alloc Row %d\r\n", loop);

		for(int loop2 = cols + 1; loop2 < newcol + 2; loop2++)
			{
			//P2N("Col %d ", loop2);

			cell *ccc = new cell;
			//ASSERT(cell);

			// Initialize cell
			memset(ccc, 0, sizeof(cell));
			ccc->magic = CELL_MAGIC;
			
			ccc->ww			= cell_www;
			ccc->hh			= cell_hhh;
			ccc->cellrow	= loop;
			ccc->cellcol	= loop2;
			ccc->color		= RGB(0,0,0);

			CString *str = new CString; 
			//str->Format("a=%d:%d", loop, loop2);
			ccc->content = str;

			col->Add(ccc);
			//P2N("Alloc cols %d\r\n", col->GetSize() );
			}	
		}

	cols = newcol;
}

#endif


void CSimpleGrid::FirstPaint()
			
{
	//ModifyStyleEx(WS_EX_NOPARENTNOTIFY,  WS_EX_TOPMOST, 0);
	//ModifyStyleEx(WS_EX_STATICEDGE, 0);

	ModifyStyle(WS_DISABLED | WS_BORDER , WS_CLIPSIBLINGS /*| WS_THICKFRAME */| SS_NOTIFY , SWP_DRAWFRAME);

	sw = GetSystemMetrics(SM_CXVSCROLL);
	sh = GetSystemMetrics(SM_CYHSCROLL);

	//Alloc(rows, cols);		

	//TRACE("First paint\r\n");
	P2N("First paint scroll_width=%d\r\n", sw);

	ReAlloc(init_row, init_col);

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

	hrcmenu.LoadMenu(IDR_MENU1);
	hrclickmenu = (BCMenu *)hrcmenu.GetSubMenu(0);

	//this->EnableWindow();
	//this->SetActiveWindow();

	this->SetFocus();

	//P2N("Text height %d\r\n", rc.bottom);
}

//////////////////////////////////////////////////////////////////////////


void CSimpleGrid::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 

{
	//P2N("Vscroll pos=%d\r\n", nPos);

	switch (nSBCode)
		{
		case SB_THUMBTRACK  :
				startrow = nPos;
				//P2N("Thumb track\r\n");
				pScrollBar->SetScrollPos(startrow);
				InvalidateGrid();
				break;

		case SB_PAGEDOWN  :
				startrow = min(rows, startrow + 5);
		case SB_LINEDOWN  :
				startrow = min(rows, startrow + 1);;
				//P2N("Vscroll down\r\n");
				pScrollBar->SetScrollPos(startrow);
				InvalidateGrid();
				break;

		case SB_PAGEUP :
				startrow = max(startrow -5, 0);
		case SB_LINEUP  :
				startrow = max(0, startrow - 1);
				pScrollBar->SetScrollPos(nPos);
				//P2N("Vscroll up\r\n");
				pScrollBar->SetScrollPos(startrow);
				InvalidateGrid();
				break;
		}

	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

//////////////////////////////////////////////////////////////////////////

void CSimpleGrid::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 

{
	//P2N("Hscroll pos=%d\r\n", nPos);

	switch (nSBCode)
		{
		case SB_THUMBTRACK  :
				startcol = nPos;
				//P2N("Thumb track\r\n");
				pScrollBar->SetScrollPos(startcol);
				InvalidateGrid();
				break;

		case SB_PAGEDOWN  :
				startcol = min(cols, startcol + 5);
		case SB_LINEDOWN  :
				startcol = min(cols, startcol + 1);;
				//P2N("Vscroll down\r\n");
				pScrollBar->SetScrollPos(startcol);
				InvalidateGrid();
				break;

		case SB_PAGEUP :
				startcol = max(startcol -5, 0);
		case SB_LINEUP  :
				startcol = max(0, startcol - 1);
				pScrollBar->SetScrollPos(nPos);
				//P2N("Vscroll up\r\n");
				pScrollBar->SetScrollPos(startcol);
				InvalidateGrid();
				break;
		}
	
	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CSimpleGrid::InvalidateCell(int row, int col)

{

	
}


//////////////////////////////////////////////////////////////////////////

void CSimpleGrid::InvalidateGrid()

{
	RECT rect;	GetClientRect(&rect);

	rect.right    -= sw;
	rect.bottom -= sh;

	InvalidateRect(&rect);	
}

//////////////////////////////////////////////////////////////////////////
//
//                            cols
//  --------------------------
// |                          |
// |       ------------------------
// |      |                   |    |
// |      |                   |    |
// |      |                   |    | 
// |      |                   |    |
// |      |                   |    |
//  ------|-------------------     |
//        |               rows     | newcol
//         ------------------------
//                                newrow   

void CSimpleGrid::ReAlloc(int newrow, int newcol)

{
	//P2N("Realloc from %d:%d to %d:%d\r\n", rows, cols, newrow, newcol);

	RECT rect; GetClientRect(&rect);

	if(!init_alloc)
		{
		init_alloc = true;
		cell_www = ((rect.right - rect.left) - sw) / newcol;
		cell_hhh = ((rect.bottom - rect.top) - sh) / newrow;
		vcols = newcol;
		vrows = newrow;
		}
	
	int rowdiff = newrow - rows;
	int coldiff = newcol - cols;

	if(coldiff > 0 || rowdiff > 0)
		{
		//P2N("rDiff=%d cdiff=%d\r\n", rowdiff, coldiff);
		
		// Iterate every row
		for(int loop = 0; loop < newrow; loop++)
			{
			int new_row = false;
			CPtrArray *col;
			if(loop >= rows)
				{
				//P2N("Row alloc: %d\r\n", loop);
				col = new CPtrArray;
				rowarr.Add(col);
				new_row = true;
				}
			else
				{
				col = (CPtrArray *)rowarr.GetAt(loop);
				}

			// Iterate every col
			for(int loop2 = 0; loop2 < newcol; loop2++)
				{
				if(loop2 >= cols || new_row)
					{
					cell *ccc = new cell;
					//ASSERT(cell);

					// Initialize cell
					memset(ccc, 0, sizeof(cell));
					
					ccc->magic = CELL_MAGIC;

					ccc->ww = cell_www;
					ccc->hh = cell_hhh;
					ccc->cellrow = loop;
					ccc->cellcol = loop2;
					ccc->color = RGB(0,0,0);

					CString *str = new CString; 
					//str->Format("x%d:%d", loop, loop2);
					ccc->content = str;

					col->Add(ccc);

					//P2N("C=%d ", loop2);
					}
				
				}
			//P2N("Alloc: %d\r\n" , col->GetSize() );
			
			}
		
		// Assign rows
		cols = newcol;
		rows = newrow;

		//P2N("Allocated rows %d\r\n", rowarr.GetSize() );
		}
}

//////////////////////////////////////////////////////////////////////////

void CSimpleGrid::AutoSize()

{
	//P2N("Autosize\r\n");

	CDWordArray sarr;
	sarr.SetSize(cols);

	// Determine max width per row
	for(int loop = 0; loop < rows; loop++)
		{
		CPtrArray *col = (CPtrArray *)rowarr[loop];

		cell *ccc;
		for(int loop2 = 0; loop2 < cols ; loop2++)
			{
			ccc = (cell *)col->GetAt(loop2); 
			CString *str = (CString *)ccc->content;

			sarr[loop2] = max(sarr[loop2], (DWORD)str->GetLength());
			}
		}

	// Limit rows to maximum size
	for(loop = 0; loop < cols ; loop++)
		{
		sarr[loop] = min(sarr[loop], (DWORD)maxwidth);
		//P2N("c=%d-w=%d  ", loop, sarr[loop]); 
		}

	// Apply it for the whole 
	for(loop = 0; loop < rows; loop++)
		{
		CPtrArray *col = (CPtrArray *)rowarr[loop];

		cell *ccc;
		for(int loop2 = 0; loop2 < cols ; loop2++)
			{
			ccc = (cell *)col->GetAt(loop2); 

			// Restore normal width
			if(sarr[loop2] == 0)
				ccc->ww = cell_www;
			else
				ccc->ww = sarr[loop2] * (charrc.right - charrc.left);
			}
		}
	
	//InvalidateGrid();
	//P2N("\r\n");
}

//////////////////////////////////////////////////////////////////////////

void CSimpleGrid::OnRButtonDown(UINT nFlags, CPoint point) 

{
	P2N("Rbutton down\r\n");

	// Mark current cell
	SetCurr(point);
	InvalidateGrid();

	RECT rc4; rc4.left = rc4.top = rc4.bottom = rc4.right = 0;

	int	found = false;
	cell *ccc = NULL;
		
	for(int loop = startrow; loop < rows; loop++)
		{
		CPtrArray *col = (CPtrArray *)rowarr[loop];

		for(int loop2 = startcol; loop2 < cols; loop2++)
			{
			ccc = (cell *)col->GetAt(loop2);
			
			rc4.right  = rc4.left + ccc->ww;
			rc4.bottom = rc4.top  + ccc->hh; 

			CRect rc5(rc4);
			
			if(rc5.PtInRect(point))
				{
				found = true;
				goto endd;
				}
			rc4.left += ccc->ww; 
			}

		rc4.top += ccc->hh;
		rc4.left = 0;
		}

endd:
	if(found)
		{
		if(ccc)
			{
			CPoint pt(point);

			ClientToScreen(&pt);

			CString *str = (CString *)ccc->content;
			P2N("Rbutton down %s\r\n", *str);
			cccc = ccc;

			//YieldToWinEx();

			hrclickmenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, pt.x, pt.y, this, NULL);
			}
		}

	CWnd::OnRButtonDown(nFlags, point);
}

BOOL CSimpleGrid::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 

{
	P2N("Command id=%d code=%d\r\n", nID, nCode);
	
	if(nCode == 0)
		{
		int cutflag = false;

		switch(nID)
			{
			case ID_RCLICK_CUT:
				cutflag = true;
				
			case ID_RCLICK_COPY:

				P2N("Copy to clip '%s'\r\n", *((CString *)cccc->content));
			    if (::OpenClipboard(AfxGetApp()->m_pMainWnd->GetSafeHwnd()))
					{
					if(::EmptyClipboard())
						{
						int len = ((CString *)cccc->content)->GetLength();
						HANDLE  txthandle = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, len + 1);

						char *txtptr = (char *)GlobalLock(txthandle);
						ASSERT(txtptr);

						strncpy(txtptr, *((CString *)cccc->content), len);
						txtptr[len] = '\0';
            
						::SetClipboardData(CF_TEXT, txthandle);
						}
					CloseClipboard();
					if(cutflag)
						{
						*((CString *)cccc->content) = "";
						InvalidateGrid();
						}
					}
				break;

			case ID_RCLICK_PASTE:
				{
				P2N("Paste operation\r\n");

				if (::OpenClipboard(AfxGetApp()->m_pMainWnd->GetSafeHwnd()))
					{	
					HANDLE htext = GetClipboardData(CF_TEXT);
					if (htext)
						{
						LPVOID ptr = GlobalLock(htext);
						ASSERT(ptr);

						P2N("Got clipboard text=%s\r\n", ptr);
						*((CString *)cccc->content) = (char *)ptr;

						GlobalUnlock(htext);
						InvalidateGrid();
						}
					}
				}
				break;
			}
		}

	return CWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CSimpleGrid::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 

{
	P2N("CSimpleGrid::OnChar %d\r\n", nChar);
	CWnd::OnChar(nChar, nRepCnt, nFlags);
}

void CSimpleGrid::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 

{
	P2N("CSimpleGrid::OnKeyDown nChar=%d nRepCnt=%d nFlags=%x\r\n", nChar, nRepCnt, nFlags);

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CSimpleGrid::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 

{	
	P2N("CSimpleGrid::OnKeyUp %d\r\n", nChar);
	CWnd::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CSimpleGrid::SetCurr(CPoint point)

{
	RECT rect;	GetClientRect(&rect);

	CRect rc6;	
	RECT rc4; rc4.left = rc4.top = rc4.bottom = rc4.right = head_size;
	for(int loop = startrow; loop < rows; loop++)
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

			if((currrow == loop) && (currcol == loop2))
				{
				rc6 = rc5;
				}

			if(rc5.PtInRect(point))
				{
				currrow = loop;
				currcol = loop2;

				// Erase old position's select rect
				InvalidateRect(rc6);
			
				// Put new pos in
				InvalidateRect(rc5);
				//Invalidate();
				break;
				}

			rc4.left += ccc->ww; 

			// Clip to last displyed column
			if(rc4.left + ccc->ww >= rect.right)
				break;
			}
		rc4.top += ccc->hh;
		rc4.left = head_size;

		// Clip to last displyed line
		if(rc4.top + ccc->hh >= rect.bottom)
			break;
		}
}


void CSimpleGrid::OnLButtonDblClk(UINT nFlags, CPoint point) 

{
	int roww, coll;	CRect rc;
	Pt2Cell(point, &roww, &coll, &rc);
	
	CPtrArray *col = (CPtrArray *)rowarr[roww];
	if(col)
		{	
		cell *ccc =  (cell *)col->GetAt(coll); 
		if(cell)
			{
			CString *str; str = (CString *)ccc->content;
			if(str)
				{	
				CString str2; str2.Format("'%s'", *str);
				MBOX(str2);
				}
			}
		}

	CWnd::OnLButtonDblClk(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////
// Translate point to cell coordinates

void CSimpleGrid::Pt2Cell(CPoint point, int *rowp, int *colp, CRect *rc)

{
	RECT rect;	GetClientRect(&rect);

	RECT rc4; rc4.left = rc4.top = rc4.bottom = rc4.right = head_size;

	for(int loop = startrow; loop < rows; loop++)
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

			if(rc5.PtInRect(point))
				{
				if(rowp)
					*rowp = loop;
				if(colp)
					*colp = loop2;
				if(rc)
					*rc = rc5;
				break;
				}

			rc4.left += ccc->ww; 

			// Clip to last displyed column
			if(rc4.left + ccc->ww >= rect.right)
				break;
			}
		rc4.top += ccc->hh;
		rc4.left = head_size;

		// Clip to last displyed line
		if(rc4.top + ccc->hh >= rect.bottom)
			break;
		}
}

//////////////////////////////////////////////////////////////////////////

void CSimpleGrid::Resized()

{
	RECT rc;	GetClientRect(&rc);
	RECT rc2;	vs.GetClientRect(&rc2);
	RECT rc3;	hs.GetClientRect(&rc3);

	vs.SetWindowPos(NULL, rc.right - sw, 0, rc2.right - rc2.left, rc.bottom - rc2.top - sh, SWP_NOOWNERZORDER);
	hs.SetWindowPos(NULL, 0, rc.bottom - sh, rc.right - rc.left - sw, rc3.bottom - rc3.top, SWP_NOOWNERZORDER);
}

void CSimpleGrid::OnSize(UINT nType, int cx, int cy) 

{
	CWnd::OnSize(nType, cx, cy);
	
	Resized();
}
