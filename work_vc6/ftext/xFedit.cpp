
/* =====[ xFedit.cpp ]========================================== 
                                                                             
   Description:     The ftext project, implementation of the xFedit.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  4/15/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#include "stdafx.h"
#include "ftext.h"
#include "xFedit.h"
#include "Version.h"
#include "ClipBoard.h"
#include "ParseStr.h"

#include "support.h"
#include "misc.h"

#include "../common/PaintDCSave.h"
#include "../common/mxpad.h"
#include "../common/memdc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define USEMEMDC	1

#define MICROSEC(start, end, freq)								\
	(((end.QuadPart - start.QuadPart) * 1000000) / freq.QuadPart)

//#define PERF

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CxFedit, CBaseWnd)
	//{{AFX_MSG_MAP(CxFedit)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_TIMER()
	ON_WM_SETFOCUS()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

CxFedit::CxFedit()

{
	mousedown = oldx = oldy = currx = curry = 0;
				
	leftgap	= rightgap = 5;	
	topgap	= 5;
	bottomgap = 5;

	bgcolor	= RGB(255, 255, 255); bgnofocus	= RGB(230, 230, 230);
	currfgcol = RGB(0, 0, 0); currbgcol = RGB(255, 255, 255);
	vscroll = scroll = cnt = prevcurfont = 0;
	
	linespace = 0, charspace = 0; lastfont = 0;
 
	align = A_LEFT;
 
	in_sel = 0;   

	selstart = selend = ssrow = sscol = serow = secol = -1;

	xdrag = ydrag = 5;

	VERIFY(m_bitmap.LoadBitmap(IDB_BITMAP2));
	//m_bitmap.LoadBitmap(IDB_BITMAP2);
	BITMAP BitMap;	m_bitmap.GetBitmap(&BitMap);	
	m_nWidth = BitMap.bmWidth;	m_nHeight = BitMap.bmHeight;	

	memset(&currlf, 0, sizeof(currlf));
}

//////////////////////////////////////////////////////////////////////////

CxFedit::~CxFedit()

{
	int loop;

	for (loop = parr.GetUpperBound(); loop >= 0; loop--)
		{
		ASSERT(((CLineItem*)parr[loop])->magic == LIMAGIC);
		delete (CLineItem*)parr[loop];
		}

	for(loop = 0; loop < fontarr.GetSize(); loop++)
		{
		ASSERT(((CFontItem*)fontarr[loop])->magic == FOMAGIC);
		delete (CFontItem*)fontarr[loop];
		}
}

//////////////////////////////////////////////////////////////////////////

void CxFedit::OnMouseEnter()

{
	mousedown = false;
	//P2N("CxFedit::OnMouseEnter()\r\n");
}

//////////////////////////////////////////////////////////////////////////

void CxFedit::OnMouseLeave()

{
	mousedown = false;
	//in_sel = false;

	//P2N("CxFedit::OnMouseLeave()\r\n");
}

//////////////////////////////////////////////////////////////////////////

void CxFedit::OnPaint() 

{
	CPaintDCSave dc(this); // device context for painting

	CMemDC		*dc3 = NULL; CDC *pDC = &dc;

	int xx = leftgap, yy = 0;

	CxRect rcall, rc; GetClientRect(rcall); 

	//CRect rect; GetClientRect(&rect);
	//rect.left	= 0;	rect.top = 0;
	//rect.right	= rcall.right - sw;
	//rect.bottom	= rcall.bottom - sh;

#ifdef USEMEMDC
	dc3 = new CMemDC(&dc, rcall); ASSERT(dc3);
	pDC = dc3;
#endif

	// Sensitive to focus status
	if(GetFocus() != this)
		pDC->FillSolidRect(rcall, bgnofocus);	
	else
		pDC->FillSolidRect(rcall, bgcolor);	

	CDC  dcMem; dcMem.CreateCompatibleDC(pDC);
	int save = dcMem.SaveDC();
	CBitmap *pOldBitmap  =  dcMem.SelectObject(&m_bitmap);
		
	BLENDFUNCTION m_bf;			// AlphaBlend function parameter
	m_bf.BlendOp	= AC_SRC_OVER;
	m_bf.BlendFlags = m_bf.AlphaFormat = 0; 
	m_bf.SourceConstantAlpha = min(255, 255);

	int mid = rcall.right / 2 -  m_nWidth/2;

	//VERIFY(AlphaBlend(*pDC, mid, 5, m_nWidth, m_nHeight, dcMem, 0, 0, m_nWidth, m_nHeight, m_bf)); 
	//VERIFY(AlphaBlend(*pDC, mid, 5, m_nWidth, m_nHeight, dcMem, 0, 0, rect.Width(), m_nHeight, m_bf)); 
	//VERIFY(BitBlt(pDC->GetSafeHdc(), mid, 5, m_nWidth, m_nHeight, dcMem, 0, 0, SRCCOPY)); 	
	
	pDC->SelectObject(GetFont());
	
	CxRect rced; GetEditRect(rced); 

	int drawlines = 1 + rced.Height() / charrc.Height();
	drawlines += vscroll; drawlines = min(drawlines, parr.GetSize());

	CRgn rgn; rgn.CreateRectRgnIndirect(rced);
	pDC->SelectClipRgn(&rgn);
 
#ifdef PERF
	LARGE_INTEGER lStart, lEnd, lFreq;
	QueryPerformanceFrequency(&lFreq); 
	//P2N("QueryPerformanceFrequency %d\r\n", lFreq.LowPart);
	QueryPerformanceCounter(&lStart);
#endif

	rc.left	= leftgap; rc.top = topgap;
	
	pDC->SetTextAlign(TA_BASELINE);	
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetBkColor(RGB(255,255,255));

	// Init text color to current
	pDC->SetTextColor(currfgcol);

	DWORD prevfont = 0, prevbg = 0, prevfg = currfgcol;

	NormalizeSel();
		
	//////////////////////////////////////////////////////////////////////
	// Draw the text
	for(int loop = vscroll; loop < drawlines; loop++)
		{
		CLineItem *li =  (CLineItem*)parr[loop];

		ASSERT(li->magic == LIMAGIC);
		ASSERT(li->line.GetLength() == li->charr.GetSize());

		//P2N("Drawing %s\r\n", parr[loop].line);

		CFontItem *fi = (CFontItem*)fontarr[0];

		int basey = rc.top + li->base;	rc.left += li->start;
			
		// Paint char by char
		int len = li->line.GetLength();
		for(int loopc = scroll; loopc < len; loopc++)
			{		
			CCharItem *ci = (CCharItem*)li->charr[loopc];
			ASSERT(ci->magic == CHMAGIC);

			char ss[2] = {0,0};	ss[0] = li->line[loopc];

			// Font change?				
			if( ci->ff != prevfont)  
				{
				fi = (CFontItem*)fontarr[ci->ff];	
				ASSERT(fi->magic == FOMAGIC);

				//P2N("Changing to font %d\r\n", li->farrd[loopc]);
				pDC->SelectObject(fi->font);
				prevfont = 	ci->ff;
				}

			// Color change?
			DWORD currfg = ((CCharItem*)li->charr[loopc])->fg;
			if(currfg != prevfg)					
				{
				pDC->SetTextColor(currfg);	prevfg = currfg ;
				}

			int currpos; RowCol2Linear(loop, loopc, &currpos);
			
			// Select rect painted
			if((selstart <= currpos) && (selend >= currpos)) 
				{
				//P2N("highlite start-%d end=%d curr=%d\r\n",
				//					selstart, selend, currpos);

				CxRect rc2(rc);
				rc2.right = rc2.left + ci->ww; 
				rc2.bottom = rc2.top + li->height;

				pDC->FillSolidRect(rc2, RGB(0, 100, 255));
				}

			// Here is the actual output
			if(ss[0] != '\t')
				pDC->TextOut(rc.left, basey, ss, 1);			

			//pDC->TabbedTextOut(rc.left, basey, ss, 1, 0, NULL, 0);			

			// Paint one back if italic
			if(loopc)
				{
				CCharItem *cip = (CCharItem*)li->charr[loopc-1];
				if(((CFontItem*)fontarr[cip->ff])->lf.lfItalic)
					{					
					ss[0] = li->line[loopc - 1];
					//P2N("painting prev %s\r\n", ss);

					// Font change?				
					if(cip->ff != prevfont)  
						{
						fi = (CFontItem*)fontarr[cip->ff];	
						ASSERT(fi->magic == FOMAGIC);
						
						pDC->SelectObject(fi->font); 
						prevfont = 	cip->ff;
						}
					pDC->TextOut(rc.left - cip->ww, basey, ss, 1);			
					}
				}

			rc.left += ((CCharItem*)li->charr[loopc])->ww;
			rc.left += charspace;
			}	

		// Paint the whole text (obsolete)
		//pDC->DrawText(sline, &rc, 0);

#if 0
		// Draw lines for testing
		CPen pen(PS_SOLID, 1, RGB(0,0,255));
		dc.SelectObject(pen);
		
		int xsum = 0, len2 = li->line.GetLength();
		for(int loop2 = 0; loop2 < len2; loop2++)
			{
			xsum  += li->parrd[loop2];
			//P2N("top=%d loop2 %d parr %d\r\n", rc.top, loop2, li->parr[loop2]);
			pDC->MoveTo(leftgap + xsum, rc.top + 2);
			pDC->LineTo(leftgap + xsum, rc.top + 10);
			}
#endif

		// Advance to next line
		rc.top += li->height; 
		rc.top += linespace; 
		rc.left = leftgap;
		}

#ifdef PERF
	QueryPerformanceCounter(&lEnd);
	P2N("Paint time: %d microsec\r\n", MICROSEC(lStart, lEnd, lFreq));
#endif

	if(GetFocus() != this)
		SetCaret();

	if(dc3)
		delete(dc3);

	CBaseWnd::OnPaint();
}

//////////////////////////////////////////////////////////////////////////

void CxFedit::OnInitControl()

{
	C2N();	P2N("Started CxFedit Application\r\n\r\n");

	//////////////////////////////////////////////////////////////////////////
	// Set up initial font

	LOGFONT		m_lf;
	GetObject((HFONT)GetStockObject(DEFAULT_GUI_FONT), sizeof(m_lf), &m_lf);

	// Just to see big font for developing
	m_lf.lfHeight = -32;

	memcpy(&currlf, &m_lf, sizeof(currlf));

//	currface =	m_lf.lfFaceName;
//	currsize =	m_lf.lfHeight;
//	currbold =	m_lf.lfWeight;
//	curritalic =m_lf.lfItalic; 

	int idx = fontarr.Add(new CFontItem(&m_lf));
	SetFont(& ((CFontItem*)fontarr[idx])->font);

	int oldw = m_lf.lfWeight;
	m_lf.lfWeight = FW_BOLD;
	fontarr.Add(new CFontItem(&m_lf));

	m_lf.lfWeight = oldw; m_lf.lfItalic = true;
	fontarr.Add(new CFontItem(&m_lf));
	
#if 0
	// Dumping font array
	for(int loop = 0; loop < fontarr.GetSize(); loop++)
		{
		CFontItem *fi = (CFontItem*)fontarr[loop];
		P2N("font %s hh=%d bo=%d it=%d\r\n", fi->lf.lfFaceName, 
				fi->lf.lfHeight, fi->lf.lfWeight, fi->lf.lfItalic);
		}
#endif

	//////////////////////////////////////////////////////////////////////////
	// Set up scroll bars

	sw = GetSystemMetrics(SM_CXVSCROLL);
	sh = GetSystemMetrics(SM_CYHSCROLL);

	CxRect rc; GetClientRect(&rc);
	//rc.right -= 1;					// Gap next to scroll bar

	rc.left = rc.right - sw; rc.bottom -= sw;

	vs.Create(SBS_VERT, rc, this, 1001);
	vs.SetScrollRange(0, 100);
	vs.EnableWindow(true);	vs.ShowWindow(true);

	GetClientRect(&rc);
	
	rc.top = rc.bottom - sh; rc.right -= sh;
	hs.Create(SBS_HORZ, rc, this, 1000);
	hs.SetScrollRange(0, 100);
	hs.EnableWindow(true);	hs.ShowWindow(true);

	//////////////////////////////////////////////////////////////////////////
	// Get base font dimentions

	CClientDC  dc(this); int ss = dc.SaveDC();
	dc.SelectObject(GetFont());
	memset(&charrc, 0, sizeof(charrc));
	dc.DrawText("a", 1, &charrc, DT_CALCRECT);
	dc.RestoreDC(ss);

	//caret.LoadBitmap(IDB_BITMAP1);
	//CreateCaret(&caret); ShowCaret();

	AddLine("1 dsf;lidsljkdsfgfgfgfgfgdsdggf;lkl;khlkjhdfdsdf");
	//			"sgfdsgsddlkjddfljl;kl;kj;lklk;j;lkd");

	AddLine("2 qwertyuiopedffa");
	AddLine("3 dsf;lidscljkqdsfab");
	AddLine("4 dsf;lidsvljkdwsfsc");
	AddLine("5 dsf;lidsbljkdesfdd");
	AddLine("6 dsf;lidsbljkdesf22");
	AddLine("7 dsf;lidsbljkdesf33");
	AddLine("8 dsf;lidsbljkdesf44");
	AddLine("9 dsf;lidsbljkdesf55");

	SetCaret();

	// Test counter
#if 0
	LARGE_INTEGER lStart, lEnd, lFreq;
	QueryPerformanceFrequency(&lFreq); 
	//P2N("QueryPerformanceFrequency %d\r\n", lFreq.LowPart);
	QueryPerformanceCounter(&lStart);
	Sleep(3000);
	QueryPerformanceCounter(&lEnd);
	P2N("Sleep 1 %d microsec\r\n", MICROSEC(lStart, lEnd, lFreq);
#endif

	xdrag = GetSystemMetrics(SM_CXDRAG);
	ydrag = GetSystemMetrics(SM_CYDRAG);

	SetTimer(1, 600, NULL);
}

//////////////////////////////////////////////////////////////////////////

void CxFedit::AddLine(const char *str)

{
	CString cstr = str;	AddLine(cstr);
}

void CxFedit::AddLine(CString &str)

{
	//P2N("CxFedit::AddLine '%s'\r\n", str);
	CLineItem *li = new CLineItem(str);	ASSERT(li);
	parr.Add(li);
	BuildDist(parr.GetUpperBound());

	
}

void CxFedit::InsertLine(CString &str, int idx)

{
	//P2N("CxFedit::InsertLine '%s'\r\n", str);
	CLineItem *li = new CLineItem(str);	ASSERT(li);
	parr.InsertAt(idx, li);
	BuildDist(idx);
}

//////////////////////////////////////////////////////////////////////////
// Build the dimentions array for rendering/navigation

void	CxFedit::BuildDist(int yy)

{
	ASSERT(parr.IsValidIdx(yy));

	CxRect rced; GetEditRect(rced);

	CLineItem *li = (CLineItem *)parr[yy];
	
	CClientDC  dc(this); int sss = dc.SaveDC(); 
	dc.SelectObject(GetFont());

	char ss[2] = {0,0};

	DWORD prevfont = 0;
	
	// Force rescan
	int	sum = 0, italic = false, len = li->line.GetLength();
	
	li->charr.SetSize(len);
	for(int loopcr = 0; loopcr < len; loopcr++)
		{
		// Create new placeholders for characters
		if( ((CCharItem*)li->charr[loopcr]) == NULL)
			{
			li->charr[loopcr] = new CCharItem;
			}
		}

	li->height = 0;

	for(int loopc = 0; loopc < len; loopc++)
		{
		CxRect rc; 	ss[0] = li->line[loopc];

		CFontItem *fi = (CFontItem*)fontarr[0];

		DWORD fidx = ((CCharItem*)li->charr[loopc])->ff;
		if(fidx != prevfont)
			{
			fi = (CFontItem*)fontarr[fidx];	ASSERT(fi->magic == FOMAGIC);
			//P2N("BuildDist: Changing to font %d\r\n", li->farrd[loopc]);
			prevfont = fidx;
			dc.SelectObject(fi->font);
			italic = fi->lf.lfItalic;
			}

		CSize sss = dc.GetTextExtent(ss, 1);

		if(ss[0] == '\t')
			sss.cx *= 8;

		//if(italic)
		//	sss.cx += sss.cx/8;
		//	}
	
		if(li->height < sss.cy)
			{
			li->height = sss.cy;
			li->base = fi->tm.tmAscent;
			//P2N("li->height=%d li->base=%d\r\n", li->height, li->base);
			}

		((CCharItem*)li->charr[loopc])->ww = sss.cx;
		sum += sss.cx;

		if(rced.Width() > sum)
			{
			if(li->align == A_LEFT)
				li->start = 0;
			else if(li->align == A_MID)
				li->start = rced.Width() / 2 -  sum / 2;
			else if(li->align == A_RIGHT)
				li->start =  rced.Width() - sum;
			else
				{
				ASSERT(TRUE);
				}
			}
		}

	// Empty line, still reserve space for it
	if(!len)
		li->height = ((CFontItem *)fontarr[0])->tm.tmHeight;
	
	//P2N("li->align %d  li->start %d\r\n", li->align , li->start);

	dc.RestoreDC(sss);
}

//////////////////////////////////////////////////////////////////////////

void CxFedit::OnGetTipText(CString &str)

{
	//str = ("Hello");
}

void CxFedit::OnLButtonDown(UINT nFlags, CPoint point) 

{
	//P2N("CxFedit::OnLButtonDown %d %d\r\n", point.x, point.y);

	mousedown = true;
	inpoint = point;

	Point2Cur(point, currx, curry);
	EnsureVisible();

	// Erase selection in not hit on one
	int currpos = RowCol2Linear(curry, currx);

	if(currpos < selstart || currpos > selend)
		{
		ssrow = serow = -1;
		sscol = secol = -1;
		NormalizeSel();
		InvalEd();
		}

	SetCaret();

	CBaseWnd::OnLButtonDown(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////

int CxFedit::KeyEvent(UINT event, UINT nChar, UINT nFlags)

{
	int		ret = 0, drawlines;
	CxRect rcall; GetClientRect(rcall);
	
#if 0
	CString kstr = "none";
	switch(event)
		{
		case WM_KEYDOWN:	kstr = "WM_KEYDOWN";	break;
		case WM_SYSKEYDOWN: kstr = "WM_SYSKEYDOWN"; break;
		case WM_KEYUP:		kstr = "WM_KEYUP";		break;
		case WM_SYSKEYUP:	kstr = "WM_SYSKEYUP";	break;
		case WM_CHAR:		kstr = "WM_CHAR";		break;
		case WM_SYSCHAR:	kstr = "WM_SYSCHAR";	break;
		}

	P2N("KeyEvent(UINT event %s, UINT nChar %c %d, UINT nFlags %x)\r\n",
				kstr, nChar, nChar, nFlags);
#endif

	// Blanket event for adding char
	if(event == WM_CHAR && !misc.IsCtrlDown() && !misc.IsAltDown())
		{
		AddChar(nChar);
		ret = 1;
		}

	// Control characters
	if(event == WM_KEYDOWN)
		{
		switch(nChar)
			{
			case VK_UP: 
				curry--; curry = max(curry, 0);
				currx = min(currx, 
					((CLineItem*)parr[curry])->line.GetLength());
				SetCaret();
				ret = 1;
				break;

			case VK_DOWN : 
				drawlines = (rcall.Height() - (topgap + bottomgap + sh)) 
								/ charrc.Height() + 2;
				curry++;
				curry = min(curry,  parr.GetUpperBound());
				currx = min(currx, 
						((CLineItem*)parr[curry])->line.GetLength());
				SetCaret();
				ret = 1;
				break;

			case VK_LEFT : 
				currx--; 
				if(currx < 0)
					{
					currx = 0; 
					
					if(curry)
						{
						curry--; curry = max(curry, 0);
						if(curry < parr.GetSize())
							currx = ((CLineItem*)parr[curry])->
									line.GetLength();
						}
					}
				SetCaret();
				ret = 1;				
				break;

			case VK_RIGHT : 
				if(curry < parr.GetSize())
					{	
					currx++;				
					if(currx > ((CLineItem*)parr[curry])->line.GetLength())
						{
						if(curry < parr.GetSize() - 1)
							currx = 0, curry++;
						else
							currx--;   // restore to eol
						}								
					}
				SetCaret();
				ret = 1;
				break;

			case VK_RETURN: 
				AddChar(nChar);
				//P2N("Enter to start new line\r\n");				
				ret = 1;
				break;

			case VK_TAB: 
				//if(nFlags & 
				P2N("Tab to get out\r\n");				
				ret = 1;
				break;

			case VK_DELETE: 
				AddChar(nChar);
				//P2N("Delete key\r\n");				
				ret = 1;
				break;
		
			case VK_HOME: 
				currx = 0;
				SetCaret();
				ret = 1;
				break;

			case VK_END: 
				if(curry < parr.GetSize())
					currx = ((CLineItem*)parr[curry])->line.GetLength();
				SetCaret();
				ret = 1;
				break;

			// ctrl -v
			case 'V': 
				if(misc.IsCtrlDown())
					{
					//P2N("Paste CTRL-V\r\n");
					Paste();					
					ret = 1;
					}
				break;
			
			// ctrl -c
			case 'C': 
				if(misc.IsCtrlDown())
					{
					P2N("Copy CTRL-C\r\n");
					Copy();
					ret = 1;
					}
				break;
			}
		}

	return ret;
}

//////////////////////////////////////////////////////////////////////////

void CxFedit::OnSize(UINT nType, int cx, int cy) 

{
	//P2N("CxFedit::OnSize\r\n");

	CBaseWnd::OnSize(nType, cx, cy);
	
	if(!IsWindow(m_hWnd))
		return;

	if(!IsWindow(vs.m_hWnd))
		return;

	CxRect rc;	GetClientRect(&rc);
	CxRect rc2;	vs.GetClientRect(&rc2);
	CxRect rc3;	hs.GetClientRect(&rc3);

	vs.SetWindowPos(NULL, rc.right - sw, 0, rc2.right - rc2.left, 
						rc.bottom - rc2.top - sh, SWP_NOOWNERZORDER);

	hs.SetWindowPos(NULL, 0, rc.bottom - sh, rc.right - rc.left - sw, 
						rc3.bottom - rc3.top, SWP_NOOWNERZORDER);

	for(int loop = 0; loop < parr.GetSize(); loop++)
		BuildDist(loop);

	//P2N("OnSize(UINT nType, int cx %d, int cy %d)\r\n", cx, cy);	
}

//////////////////////////////////////////////////////////////////////////

void CxFedit::InvalLine()

{
	RECT rc; GetClientRect(&rc);

	int yyy = curry - vscroll;

	rc.top  = topgap  + yyy *  charrc.bottom;
	rc.bottom  = topgap  + (yyy + 1) *  charrc.bottom;

	InvalidateRect(&rc);
}

//////////////////////////////////////////////////////////////////////////

void CxFedit::SetCaret(int rgb, int timer)

{
	// Ensure it is visible
	if(!timer)
		EnsureVisible();

	DrawCaret(bgcolor, oldx, oldy);
	DrawCaret(rgb, currx, curry, true);
	oldx = currx; oldy = curry;
}

//////////////////////////////////////////////////////////////////////////

void CxFedit::DrawCaret(int rgb, int xx, int yy, int drawline)

{
	CxRect rced; GetEditRect(rced);
	COLORREF oldcol;

	CClientDC dc(this);	int ss = dc.SaveDC(); CDC *pDC = &dc;

	// Paint away from decorations
	CRgn rgn; rgn.CreateRectRgnIndirect(rced);
	pDC->SelectClipRgn(&rgn);

	CString str = " "; int xsum = 0, ysum = 0;
	
	if(parr.IsValidIdx(yy) && yy >= vscroll)
		{
		for(int loop = vscroll; loop < yy; loop++)
			ysum += ((CLineItem*)parr[loop])->height + linespace;

		CLineItem *li = (CLineItem*)parr[yy];

		ASSERT(li->magic == LIMAGIC);

		if(xx < li->charr.GetSize())
			str = li->line[xx];

		// Suitable defaults
		CFontItem *fi = (CFontItem*)fontarr[0];
		int tmpcol = 0, lim = min(xx, li->charr.GetSize());

		for(int loopx = scroll; loopx < lim; loopx++)
			xsum += ((CCharItem*)li->charr[loopx])->ww + charspace;
			
		// Set font
		if(xx < li->charr.GetSize())
			{
			int fidx = ((CCharItem*)li->charr[xx])->ff;
			fi = (CFontItem*)fontarr[fidx];	
			ASSERT(fi->magic == FOMAGIC);

			tmpcol = ((CCharItem*)li->charr[loopx])->fg;
			}

		pDC->SetTextAlign(TA_BASELINE);	pDC->SetTextColor(tmpcol);	
		pDC->SelectObject(fi->font);

		xx = min(xx, li->charr.GetUpperBound());

		// Set location
		CxRect rc;
		rc.left = leftgap + xsum + li->start;
		rc.top = topgap + ysum;

		if(li->charr.IsValidIdx(xx))
			rc.right  = rc.left + ((CCharItem*)li->charr[xx])->ww; 
		else	
			rc.right = rc.left + charrc.right;

		rc.bottom = rc.top + li->height;
		
		int basey = rc.top + li->base, currpos;
		RowCol2Linear(yy, xx, &currpos);

		// Select rect painted		
		if(drawline == 0 && (selstart <= currpos && selend >= currpos)) 
			{
			CxRect rc2(rc);

			if(li->charr.IsValidIdx(xx))
				rc2.right  = rc2.left + ((CCharItem*)li->charr[xx])->ww; 
			else	
				rc2.right = rc2.left + charrc.right;

			rc2.bottom = rc2.top + li->height;
			pDC->FillSolidRect(rc2, RGB(0, 100, 255));
			pDC->SetBkMode(TRANSPARENT);
			}
		else
			{
			pDC->SetBkMode(OPAQUE);	
			oldcol = pDC->SetBkColor(rgb);
			}

		if(str[0] != '\t')
			pDC->TextOut(rc.left, basey, str, 1);
		
		//pDC->SetBkColor(oldcol);

		// testing ...
		//dc.DrawText(str, rc,  DT_BOTTOM | DT_SINGLELINE);

		if(drawline)
			{			
			dc.MoveTo(rc.left, basey - fi->tm.tmAscent); // - abs(fi->tm.tmHeight));	
			dc.LineTo(rc.left, basey + fi->tm.tmDescent);
			}
		}

	dc.RestoreDC(ss);
}

//////////////////////////////////////////////////////////////////////////

void	CxFedit::AddChar(char nChar)

{
	int full = false;

	//P2N("Ascii %c\r\n", nChar);

	// Expand string array to current line
	int lens = parr.GetSize();
	for(int loopy = lens; loopy < curry + 1; loopy++)
		AddLine("");

	CLineItem *oli = (CLineItem*)parr[curry];
	CString &str = oli->line;

	// Correct currx to end of line
	if(currx > str.GetLength())
		currx = str.GetLength();

	// Split at split point
	CString str3 = str.Mid(currx), str2 = str.Left(currx);
	
	//P2N("%s %s -%c- %s\r\n",  str, str2, nChar, str3);

	if(nChar == VK_DELETE)
		{
		if(str.GetLength() > currx + 1)
			{
			str3 = str.Mid(currx + 1);
			str = str2; str += str3;
			oli->charr.RemoveAt(currx);
			}
		else
			{
			// Grab next line
			if(curry < parr.GetSize() - 1)
				{
				CLineItem* nli = ((CLineItem*)parr[curry + 1]);

				str3 = nli->line;				
				str = str2; str += str3;
		
				// Copy prev lines formatting 
				for(int loop = 0; loop < nli->charr.GetSize(); loop++)
					oli->charr.Add(nli->charr[loop]);
		
				parr.RemoveAt(curry + 1);
				full = true;
				}
			}
		}
	else if(nChar == VK_BACK)
		{
		if(currx > 0)
			{
			currx--;
			oli->charr.RemoveAt(currx);
			str2 = str.Left(currx );
			str = str2; str += str3;
			}
		else
			{
			if(curry > 0)
				{
				// Wrap line up
				CString str4 = ((CLineItem*)parr[curry])->line;

				CLineItem* pli = ((CLineItem*)parr[curry-1]); 
				ASSERT(pli->magic == LIMAGIC);

				CLineItem* nli = ((CLineItem*)parr[curry]);
				ASSERT(nli->magic == LIMAGIC);

				// Copy prev. line's formatting 
				for(int loop = 0; loop < nli->charr.GetSize(); loop++)
					pli->charr.Add(nli->charr[loop]);
						
				currx = pli->line.GetLength();  pli->line += str4;			
				
				// Items needed for the prev line (they are ptrs)
				//delete (CLineItem*)parr[curry];	

				// Remove item from line list
				parr.RemoveAt(curry); 

				curry--;		// Step up one line
				full = true;
				}
			}		
		}
	else if(nChar == VK_RETURN)
		{		
		str = str2;	curry++; currx = 0;

		CLineItem *li = new CLineItem;
		
		// Copy prev lines formatting
		for(int loop = str2.GetLength(); loop < (int)oli->charr; loop++)
			li->charr.Add(oli->charr[loop]);
		
		oli->charr.SetSize(str.GetLength()); li->line = str3; 
	
		if(curry < parr.GetSize() )
			parr.InsertAt(curry, li);
		else
			curry = parr.Add(li);

		BuildDist(curry-1);	BuildDist(curry);
	
		full = true;
		}
	else
		{
		CCharItem *ci = new CCharItem;

		ci->bg = currbgcol; ci->fg = currfgcol;
		ci->ff = FindFontMatch(&currlf);
		oli->charr.InsertAt(currx, ci);

		str = str2; str += nChar; str += str3;
		currx++;
		}

	BuildDist(curry); EnsureVisible(); SetCaret(); 
	
	//if(full)
		//Invalidate();
	
	InvalEd();

	//else
	//	InvalLine();
}

//////////////////////////////////////////////////////////////////////////

void CxFedit::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 

{
	//P2N("OnVScroll command %s Pos=%d\r\n", mxpad.num2sbarcom(nSBCode), nPos);
	//P2N("OnVScroll %d hor nPos=%d - hs.pos=%d %d\r\n", nSBCode, nPos, vs.GetScrollPos());

	switch (nSBCode)
		{
		case SB_THUMBTRACK  :
			pScrollBar->SetScrollPos(vscroll);		
			vscroll = nPos;
			break;
	
		case SB_PAGEDOWN  :
				vscroll = vscroll + 5;
		case SB_LINEDOWN  :
				vscroll = vscroll + 1;
				break;

		case SB_PAGEUP :
				vscroll = max(vscroll - 5, 0);
		case SB_LINEUP  :
				vscroll = max(vscroll - 1, 0);
				break;

		case SB_ENDSCROLL:
			break;
		}

	pScrollBar->SetScrollPos(vscroll);		
	Invalidate();
	
	CBaseWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

//////////////////////////////////////////////////////////////////////////

void CxFedit::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 

{
	int mm, xx;	pScrollBar->GetScrollRange(&mm, &xx);

	//P2N("OnHScroll %d hor nPos=%d - hs.pos=%d %d min=%d max=%d\r\n", nSBCode, nPos, 
	//			hs.GetScrollPos(), pScrollBar->GetScrollPos(), mm, xx);
	
	switch (nSBCode)
		{
		case SB_THUMBTRACK  :
			pScrollBar->SetScrollPos(scroll);		
			scroll = nPos;
			break;
	
		case SB_PAGEDOWN  :
				scroll = scroll + 5;
		case SB_LINEDOWN  :
				scroll = scroll + 1;
				break;

		case SB_PAGEUP :
				scroll = max(scroll - 5, 0);
		case SB_LINEUP  :
				scroll = max(scroll - 1, 0);
				break;

		case SB_ENDSCROLL:
			break;
		}

	pScrollBar->SetScrollPos(scroll);		
	Invalidate();

	CBaseWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CxFedit::OnTimer(UINT nIDEvent) 

{
	if(GetFocus() != this)
		{
		//SetCaret();
		return;
		}

	if(cnt++ % 2 == 0)
		SetCaret(RGB(128, 128, 128), true);
	else
		SetCaret(RGB(192, 192, 192), true);
	
	//P2N("Caret Timer\r\n");		
	CBaseWnd::OnTimer(nIDEvent);
}

void CxFedit::ScrollHor(int num)

{
	//P2N("CxFedit::ScrollHor(int num) %d\r\n", num);
	
	scroll += num;	scroll = max(0, scroll);
	Invalidate();
	hs.SetScrollPos(scroll);

	//P2N("CxFedit::ScrollHor %d\r\n", hs.GetScrollPos());
}

void CxFedit::OnSetFocus(CWnd* pOldWnd) 

{
	CBaseWnd::OnSetFocus(pOldWnd);	
	SetCaret(RGB(128, 128, 128));		
}

void CxFedit::ScrollVer(int num)

{
	vscroll += num;	vscroll = max(0, vscroll);
	Invalidate();
	vs.SetScrollPos(num);

	//P2N("CxFedit::ScrollVer %d\r\n", vs.GetScrollPos());
}


BOOL CxFedit::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{

	if(zDelta < 0)
		vscroll += 1;
	else
		vscroll -= 1, vscroll = max(0, vscroll);

	vs.SetScrollPos(vscroll);	
	Invalidate();

	return CBaseWnd::OnMouseWheel(nFlags, zDelta, pt);
}

//////////////////////////////////////////////////////////////////////////
// Ensure the current cursor is on the visible window

void CxFedit::EnsureVisible()

{	
	int changed = false;

	CxRect rcall, rc; GetClientRect(rcall); 	
	DWORD pointx = rcall.Width() - (leftgap + rightgap + sw);
	rcall.bottom -= topgap + bottomgap + sh;

	//////////////////////////////////////////////////////////////////////
	// Y - direction

	int drawlines = 0, ysum = 0;
	for(int loop = vscroll; loop < parr.GetSize(); loop++)
		{
		CLineItem *li = (CLineItem*)parr[loop];
		ASSERT(li->magic == LIMAGIC);
		
		ysum += li->height;		// Accumulate height
		ysum += linespace;
		if(ysum > rcall.bottom)
			{
			drawlines = loop - vscroll - 1; 
			if(vscroll + drawlines < curry )	
				{
				vscroll = curry - drawlines, changed = true;
				}
			//P2N("EnsureVisible() drawlines=%d\r\n", drawlines);
			break;
			}
		}					
	if(vscroll > curry)
		{
		vscroll = curry,  changed = true;
		}	

	/////////////////////////////////////////////////////////////////////
	// X - direction

	int drawchars = 0, so = 0, curr = 0;
	if(parr.IsValidIdx(curry))
		{
		CLineItem *li = (CLineItem*)parr[curry];
		ASSERT(li->magic == LIMAGIC);
	
		DWORD xsum = 0;
		// Search treshold for pointx
		for(int loop = scroll; loop < li->charr.GetSize(); loop++)
			{
			xsum += ((CCharItem*)li->charr[loop])->ww;
			xsum += charspace;

			if(pointx <= xsum + charrc.right)
				{
				drawchars = loop - scroll;  
				//P2N("EnsureVisible() drawchars=%d\r\n", drawchars);
				
				if(scroll + drawchars < currx) 	
					{
					scroll = currx - drawchars + 1, changed = true;
					}
				break;
				}
			}
		}
	if(scroll > currx)
		{
		scroll = currx,  changed = true;
		}	

	//////////////////////////////////////////////////////////////////////

	if(changed)
		{	
		vs.SetScrollPos(vscroll); hs.SetScrollPos(scroll);		
		Invalidate();
		}
}

//////////////////////////////////////////////////////////////////////////

void CxFedit::OnLButtonUp(UINT nFlags, CPoint point) 

{
	mousedown = false;

	if(in_sel)
		{
		ReleaseCapture();

		RowCol2Linear(ssrow, sscol, &selstart);
		RowCol2Linear(serow, secol, &selend);

		P2N("ssrow=%d sscol=%d serow=%d secol=%d\r\n",
							ssrow, sscol, serow, secol);

		in_sel = false;
		}
	
	CBaseWnd::OnLButtonUp(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////

void CxFedit::OnMouseMove(UINT nFlags, CPoint point) 

{
	if(mousedown)
		{
		if(!in_sel)
			{
			CPoint  diff = point - inpoint;

			//P2N("diff x=%x y-%d\r\n", diff.x, diff.y);
			
			if(abs(diff.x) > xdrag || abs(diff.y) > ydrag)
				{
				SetCapture(); in_sel = true;
				
				Point2Cur(point, sscol, ssrow);
				RowCol2Linear(ssrow, sscol, &selstart);
			
				//P2N("CxFedit::OnMouseMove start drag %d %d\r\n", point.x, point.y);		
				//P2N("CxFedit::OnMouseMove start drag ssrow=%d sscol=%d\r\n", ssrow, sscol);		
				//P2N("CxFedit::OnMouseMove start drag selstart=%d\r\n", selstart);		
				}
			}
		else
			{
			Point2Cur(point, secol, serow);	curry = serow, currx = secol;
			int rr, cc;  Linear2RowCol(selend, rr, cc);

			//P2N("selend=%d serow=%d, secol=%d, rr=%d, cc=%d\r\n", 
			//					selend, serow, secol, rr, cc);

			// Drag moved opver to the top
			if(point.y < 0)
				{
				if(vscroll > 0)
					vscroll--;
				}
			else
				EnsureVisible();

			InvalEd();

			//P2N("CxFedit::OnMouseMove draging %d %d\r\n", point.x, point.y);	
			//P2N("CxFedit::OnMouseMove drag ssrow=%d sscol=%d\r\n", serow, secol);		
			//P2N("CxFedit::OnMouseMove drag selstart=%d selend=%d\r\n", selstart, selend);		
			}
		}

	CBaseWnd::OnMouseMove(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////
// Translate point to cursor position

void CxFedit::Point2Cur(CPoint point, int &cxx, int &cyy) 

{
	int  found = 0, ysum = 0, xsum = 0, prev = 0;
	
	// Calc Y ------------	
	for(int loop = vscroll; loop < parr.GetSize(); loop++)
		{
		ysum += ((CLineItem*)parr[loop])->height + linespace; 

		if(ysum > point.y )
			break;
		}	
	cyy = loop;

	// Calc X ------------
	if(cyy < parr.GetSize())
		{
		CLineItem *li =  (CLineItem*)parr[cyy];
	
		xsum = li->start;
		for (int loop = scroll; loop < li->charr.GetSize(); loop++)
			{
			xsum += ((CCharItem*)li->charr[loop])->ww;
			xsum += charspace;

			if((point.x <= xsum ) && (point.x > prev))
				{
				//P2N("Point2cur mark %d\r\n", loop);
				found = true; cxx = loop;
				break;
				}
			prev = xsum;
			}		
		if(!found)
			{
			cxx = loop;
			}
		}
	else
		{
		// Assume average char width
		cxx = (point.x - leftgap) / charrc.right;
		}

	//P2N("Point2cur x=%d y=%d cxx=%d cyy=%d\r\n", point.x, point.y, cxx, cyy);
}

//////////////////////////////////////////////////////////////////////////
//

int		CxFedit::RowCol2Linear(int row, int col, int *lin)

{
	int xsum = 0, yyy = min(row, parr.GetSize());

	for(int loop = 0; loop < yyy; loop++)
		{
		CLineItem *li = (CLineItem*)parr[loop];

		ASSERT(li->magic == LIMAGIC);	
		ASSERT(li->line.GetLength() == li->charr.GetSize());

		xsum += li->charr.GetSize();
		}	

	xsum += col;

	if(lin)
		*lin = xsum;

	//P2N("LinearPos () row=%d col=%d lin=%d\r\n", row, col, lin);

	return xsum;	
}

//////////////////////////////////////////////////////////////////////////

void	CxFedit::Linear2RowCol(int lin, int &row, int &col)

{
	for(int loop = 0; loop < parr.GetSize(); loop++)
		{
		CLineItem *li = (CLineItem*)parr[loop];
		
		ASSERT(li->magic == LIMAGIC);	
		ASSERT(li->line.GetLength() == li->charr.GetSize());

		// Less than length left over
		if(lin < li->charr.GetSize())
			{
			break;
			}
		lin -= li->charr.GetSize();
		}

	row = loop;	col = lin;
}

//////////////////////////////////////////////////////////////////////////

void CxFedit::Size(int val)

{
	if(parr.IsValidIdx(curry))
		{
		CLineItem *li =  (CLineItem*)parr[curry];
		ASSERT(li->magic == LIMAGIC);
	
		//if(li->charr.GetSize() >= 0  && currx  < li->charr.GetSize())
		if(li->charr.IsValidIdx(currx))
			{
			// Find suitable font, add if none
			LOGFONT		m_lf;
		
			int fidx = ((CCharItem*)li->charr[currx])->ff;

			CFontItem *fi = (CFontItem*)fontarr[fidx];
			ASSERT(fi->magic == FOMAGIC);

			memcpy(&m_lf,  &fi->lf, sizeof(m_lf));
			m_lf.lfHeight = val;
			
			int hh = FindFontMatch(&m_lf);
			memcpy(&currlf,  &m_lf, sizeof(m_lf));		
		
			//P2N("set size at current pos x=%d y=%d size=%d font=%d\r\n", 
			//			currx, curry, val, li->farrd[currx]);

			BuildDist(curry); Invalidate();
			}
		}		
}

//////////////////////////////////////////////////////////////////////////
//
// Find matching font, ret -1 if none found

int CxFedit::FindFontMatch(LOGFONT *lf)

{
	// Search font array

	for(int loop = 0; loop < fontarr.GetSize(); loop++)
		{
		CFontItem *fi = (CFontItem*)fontarr[loop];
	
		//P2N("font %s hh=%d bo=%d it=%d\r\n", fi->lf.lfFaceName, 
		//		fi->lf.lfHeight, fi->lf.lfWeight, fi->lf.lfItalic);

		if(strcmp(fi->lf.lfFaceName,  lf->lfFaceName) == 0)			
			if(fi->lf.lfHeight == lf->lfHeight)
				if(fi->lf.lfWeight == lf->lfWeight)
					if(fi->lf.lfItalic == lf->lfItalic)
						if(fi->lf.lfUnderline == lf->lfUnderline)
							if(fi->lf.lfStrikeOut == lf->lfStrikeOut)
								return loop;					
		}

	// Add new if none there
	int ret = fontarr.Add(new CFontItem(lf));
	return ret;
}

//////////////////////////////////////////////////////////////////////////

void CxFedit::Font(const char *str)

{
	if(parr.IsValidIdx(curry))
		{
		CLineItem *li =  (CLineItem*)parr[curry];
		ASSERT(li->magic == LIMAGIC);
	
		if(li->charr.GetSize() >= 0  && currx  < li->charr.GetSize())
			{
			// Find suitable font, add if none
			//LOGFONT		m_lf;
		
			//CFontItem *fi = (CFontItem*)fontarr[li->farrd[currx]];
			//ASSERT(fi->magic == FOMAGIC);

			//memcpy(&m_lf,  &fi->lf, sizeof(m_lf));		
			
			strncpy(currlf.lfFaceName, str, sizeof(currlf.lfFaceName));

			lastfont = FindFontMatch(&currlf);

			//memcpy(&currlf,  &m_lf, sizeof(m_lf));		
			
			//P2N("set size at current pos x=%d y=%d size=%d font=%d\r\n", 
			//			currx, curry, val, li->farrd[currx]);

			BuildDist(curry); Invalidate();
			}
		}		
}

//////////////////////////////////////////////////////////////////////////

void CxFedit::Italic(int flag)

{
	if(!parr.IsValidIdx(curry))
		return;

	CLineItem *li =  (CLineItem*)parr[curry];
	ASSERT(li->magic == LIMAGIC);


	//////////////////////////////////////////////////////////////////////////
	if(li->charr.GetSize() >= 0  && currx  < li->charr.GetSize())
		{		
		NormalizeSel();

		if(flag == -1)
			currlf.lfItalic = !currlf.lfItalic;
		else
			currlf.lfItalic = flag;

		if(selstart == selend)
			{
			lastfont = FindFontMatch(&currlf);
			BuildDist(curry); 
			}
		else
			{
			//P2N("Italic on selection\r\n");

			int lines = parr.GetSize();
			for(int loopr = 0; loopr <  lines; loopr++)
				{
				CLineItem *li =  (CLineItem*)parr[loopr];

				ASSERT(li->magic == LIMAGIC);
				ASSERT(li->line.GetLength() == li->charr.GetSize());

				//P2N("Dumping line: %s\r\n", li->line);
					
				int linelen = li->line.GetLength();
				for(int loopc = 0; loopc < linelen; loopc++)
					{			
					int currpos;
					RowCol2Linear(loopr, loopc, &currpos);
					
					if(selstart <= currpos && selend >= currpos)
						{
						((CCharItem*)li->charr[loopc])->ff = 
								FindFontMatch(&currlf);
						}
					}
				BuildDist(loopr); 
				//P2N("\r\n");
				}
			Invalidate();
			}
		}
}

//////////////////////////////////////////////////////////////////////////

void CxFedit::Underline(int flag)
{
	if(parr.IsValidIdx(curry))
		{
		CLineItem *li =  (CLineItem*)parr[curry];
		ASSERT(li->magic == LIMAGIC);
	
		if(li->charr.IsValidIdx(currx))
			{
			if(flag == -1)
				currlf.lfUnderline = !currlf.lfUnderline;
			else
				currlf.lfUnderline = flag;

			lastfont = FindFontMatch(&currlf);
			BuildDist(curry); Invalidate();
			}
		}		
}

//////////////////////////////////////////////////////////////////////////

void CxFedit::Bold(int flag)

{
	if(!parr.IsValidIdx(curry))
		return;

	if(flag == -1)
		{
		// Toggle bold/normal
		if(currlf.lfWeight == FW_BOLD)
			currlf.lfWeight = FW_NORMAL;
		else
			currlf.lfWeight = FW_BOLD;
		}
	else if(flag)
		currlf.lfWeight = FW_BOLD;
	else
		currlf.lfWeight = FW_NORMAL;

	lastfont = FindFontMatch(&currlf);
	
	CLineItem *li =  (CLineItem*)parr[curry];
	ASSERT(li->magic == LIMAGIC);

	if(selstart == selend)
		{
		if(li->charr.IsValidIdx(currx))
			{			
			BuildDist(curry); Invalidate();
			}
		}		
	else	//////////////////////////////////////////////////////////////
		{
		int loop, sr = ssrow, sc = sscol, er = serow, ec = secol;

		Linear2RowCol(selstart, sr, sc); Linear2RowCol(selend, er, ec);

		for(loop = sr; loop <= er; loop++)
			{
			if(!parr.IsValidIdx(loop))
				break;

			int startt, endd;

			CLineItem *li = (CLineItem*)parr[loop];
			ASSERT(li->magic == LIMAGIC);

			if(loop == sr)
				startt = sc, endd = li->charr.GetSize();			
			else if(loop == er)
				startt = 0, endd = ec;			
			else
				startt = 0, endd = li->charr.GetSize();

			for(int loop2 = startt; loop2 < endd; loop2++)
				{
				// Boldify pre - existing font	
				int fidx = ((CCharItem*)li->charr[loop2])->ff;

				CFontItem *fi = (CFontItem*)fontarr[fidx];	
				ASSERT(fi->magic == FOMAGIC);

				currlf = fi->lf;

				if(flag)
					currlf.lfWeight = FW_BOLD;
				else
					currlf.lfWeight = FW_NORMAL;

				lastfont = FindFontMatch(&currlf);

				int currpos = RowCol2Linear(loop, loop2);
					
				if(selstart <= currpos && selend >= currpos)
					((CCharItem*)li->charr[loop2])->ff = lastfont;
				}
			}
		}

}

//////////////////////////////////////////////////////////////////////////

void CxFedit::Strikeout(int flag)

{
	if(parr.IsValidIdx(curry))
		{
		CLineItem *li =  (CLineItem*)parr[curry];
		ASSERT(li->magic == LIMAGIC);
	
		if(li->charr.IsValidIdx(currx))
			{			
			if(flag == -1)
				currlf.lfStrikeOut = !currlf.lfStrikeOut;
			else
				currlf.lfStrikeOut = flag;

			lastfont = FindFontMatch(&currlf);
			BuildDist(curry); Invalidate();
			}
		}		
}

//////////////////////////////////////////////////////////////////////////

int CxFedit::SetTextColor(int col)

{
	int ret = currfgcol;
	currfgcol = col;

	// Also apply to current selection
	NormalizeSel();
	if(selstart != selend)
		{
		P2N("CxFedit::SetTextColor selection\r\n");
		}

	return ret;
}

int CxFedit::SetAlign(int align)

{
	int ret = 0;

	if(parr.IsValidIdx(curry))
		{
		CLineItem *li =  (CLineItem*)parr[curry];
		li->align = align;

		BuildDist(curry);
		Invalidate();
		}

	return ret;
}

//////////////////////////////////////////////////////////////////////////

void CxFedit::GetEditRect(RECT *rc)

{
	RECT rcall; GetClientRect(&rcall);

	rcall.top		+= topgap;
	rcall.bottom	-= bottomgap + sh;
	rcall.left		+= leftgap;
	rcall.right		-= rightgap + sw;

	*rc = rcall;
}


//////////////////////////////////////////////////////////////////////////

void CxFedit::InvalEd()

{
	RECT rced; GetEditRect(&rced);
	InvalidateRect(&rced);	
}


void CxFedit::NormalizeSel()

{
	// Nothing selected
	if(ssrow == serow && sscol == secol)
		{
		selstart = selend = -1;
		return;
		}

	RowCol2Linear(ssrow, sscol, &selstart);	
	RowCol2Linear(serow, secol, &selend);
	
	// Swap values
	if(selstart > selend)
		{
		int tmp;
		tmp = selstart; selstart = selend; selend = tmp;
		}

	//P2N("CxFedit::NormalizeSel() selstart %d selend %d\r\n", selstart, selend);
}

void CxFedit::Copy()

{
	NormalizeSel();

	if(selstart == selend)
		{
		P2N("Nothing selected\r\n");
		return;
		}

	int  sr = ssrow, sc = sscol, er = serow, ec = secol;

	Linear2RowCol(selstart, sr, sc);
	Linear2RowCol(selend, er, ec);

	//CStringArray sa;

	CString strall;
	int start = 0;

	for(int loop = sr; loop <= er; loop++)
		{
		if(parr.IsValidIdx(loop))
			{
			if(start)
				strall += "\r\n";

			CLineItem *li =  (CLineItem*)parr[loop];
			ASSERT(li->magic == LIMAGIC);

			CString str;
			if(loop == sr)
				{
				str = li->line.Mid(sc); 	
				}
			else if(loop == er)
				{
				str = li->line.Left(ec + 1); 	
				}
			else
				{
				str = li->line;
				}
			P2N("copy line '%s'\r\n", str);	

			strall += str;			
			start++;
			}
		}

	clip.PutText(strall);
}


//////////////////////////////////////////////////////////////////////////

void CxFedit::Paste()

{
	NormalizeSel();

	CString pastestr; //(" hello world ");

	// Clipboard
	clip.GetText(pastestr);

	CParseStr parse(pastestr);

	// No selection to replace
	if(selstart == selend)
		{
		if(!parr.IsValidIdx(curry))
			return;

		CLineItem *li =  (CLineItem*)parr[curry];
		CString aa,bb;

		aa = li->line.Left(currx); 
		
		if(currx < li->line.GetLength())
			bb = li->line.Mid(currx);

		for(int loop = 0; loop < parse.GetCount(); loop ++)
			{
			CString *str = parse.GetAt(loop);

			if(loop == 0)
				{	
				li->line = aa +  *str;			

				if(loop == parse.GetCount() - 1)
					li->line += bb;

				BuildDist(curry );
				currx += pastestr.GetLength();		
				}
			else
				{
				// Insert cut from first line
				if(loop == parse.GetCount() - 1)
					InsertLine(*str + bb, curry);
				else
					InsertLine(*str, curry);

				curry++;
				}
			}
		
		InvalEd();
		return;
		}

	int  sr = ssrow, sc = sscol, er = serow, ec = secol;

	//////////////////////////////////////////////////////////////////////////

	Linear2RowCol(selstart, sr, sc); Linear2RowCol(selend, er, ec);

	for(int loop = sr; loop <= er; loop++)
		{
		if(parr.IsValidIdx(loop))
			{
			CLineItem *li =  (CLineItem*)parr[loop];
			ASSERT(li->magic == LIMAGIC);

			CString str;
			if(loop == sr)
				{
				str = li->line.Left(sc); li->line = str + pastestr;
				}
			else if(loop == er)
				{
				str = li->line.Mid(ec + 1); li->line = pastestr + str;
				}
			else
				{
				//for(int loops = 0; loops < pastestr.GetLength(); loops++)
				//	{
				//	}
				li->line = pastestr;
				}
			BuildDist(loop);
			}
		}

	curry = serow; currx = pastestr.GetLength();
	ssrow = sscol = serow = secol = -1;

	InvalEd();

	//support.YieldToWinEx();
}

BOOL CxFedit::PreTranslateMessage(MSG* pMsg) 

{
	if(pMsg->message == WM_COMMAND )
		{
		P2N("msg %d wParam %d, lParam %p\r\n", pMsg->message, pMsg->wParam, pMsg->lParam);
		return 0;
		}	
	return CBaseWnd::PreTranslateMessage(pMsg);
}

//////////////////////////////////////////////////////////////////////////

void CxFedit::DumpText()

{
	NormalizeSel();

	int lines = parr.GetSize();
	for(int loopr = 0; loopr <  lines; loopr++)
		{
		CLineItem *li =  (CLineItem*)parr[loopr];

		ASSERT(li->magic == LIMAGIC);
		ASSERT(li->line.GetLength() == li->charr.GetSize());

		//P2N("Dumping line: %s\r\n", li->line);
			
		int linelen = li->line.GetLength();
		for(int loopc = 0; loopc < linelen; loopc++)
			{			
			CCharItem *ci = (CCharItem*)li->charr[loopc];

			int currpos; RowCol2Linear(loopr, loopc, &currpos);
			
			if((selstart <= currpos && selend >= currpos) ||
				(selstart == selend)) 
				{
				P2N("%c(%d) %d  ", li->line[loopc], 
						ci->ww, ci->ff);
				}
			}
		P2N("\r\n");
		}
}
