	
/* =====[ LaySline.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the LaySline.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  11/28/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// LaySline.cpp: implementation of the CLaySline class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "xraynotes.h"
#include "LaySline.h"

#include "lineProp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
// Iterate line, call callback on every iteration
// Line is drawn from rc->top rc->left to rc->bottom rc->right

void	IterateLine(void *ptr, int step, RECT* rc, pfri_p callback)

{
	// Find a hit iterating calculated points of the line

	double	loopy = 0, loopx = 0;
	int		cnt = 0, lim, inc, hit = false;
	POINT	ptc;

	int		www = rc->right - rc->left;
	int		hhh = rc->bottom - rc->top;

	// Loop points on the larger axis
	if(abs(www) > abs(hhh))
		{
		double	xslope = (double)hhh / www;
		if(www < 0) inc = -step;
		else			 inc = step;
		lim = abs(www) + step;
	
		while(true)
			{
			ptc.x = int(rc->left + loopx);
			ptc.y = int(rc->top + loopx * xslope);

			if(callback(ptr, &ptc))
				break;

			loopx += inc;
			cnt += step;
			if(cnt >= lim)
				break;
			}
		} 
	else 
		{
		double	yslope = (double)www / hhh;
		if(hhh < 0) inc = -step;
		else   			 inc = step;
		lim = abs(hhh) + step;

		while(true)
			{
			ptc.x = int(rc->left + loopy * yslope);
			ptc.y = int(rc->top + loopy);

			if(callback(ptr, &ptc))
				break;

			loopy += inc;
			cnt	  += step;
			if(cnt >= lim)
				break;
			}
		}
}

//////////////////////////////////////////////////////////////////////////
// Expand hitrect	
	
void	expandrect(RECT *rc, int expand)

{
	if(rc->left < rc->right)		
		{
		rc->left  -= expand; 
		rc->right += expand; 
		}
	else
		{
		rc->left	 += expand;
		rc->right -= expand;
		}
	if(rc->top < rc->bottom)		
		{
		rc->top  -= expand; 
		rc->bottom += expand; 
		}
	else
		{
		rc->top	 += expand;
		rc->bottom -= expand;
		}
}

// Call this function if point iteration encountered

static	int 	callback(void *ptr, POINT *pt)

{
	CLaySline *obj = (CLaySline *)ptr;

	ASSERT(obj->gdata.kind == LAYER_SLINE);

	//P2N("callback %p point x=%d y=%d\r\n", ptr, pt->x, pt->y);

	//int dd = 2; // obj->_handlesize;
//	RECT rect; 
//	rect.top  = pt->y - dd;
//	rect.left = pt->x - dd;
//	rect.bottom  = pt->y + dd;
//	rect.right = pt->x + dd;

	// testing 
	//CShadesDlg *dlg; GMD(dlg);	CClientDC dc(dlg);
	//dc.SetPixel(pt->x, pt->y, RGB(255,0,0));
	//obj->DrawFocus(&dc, &rect);

	if(obj->_TestLinePt(pt->x, pt->y, obj->_iterpt))
		{
		obj->_iterhit = true;
		return 1;
		}
	return 0;
}

// Call this function if point invalidatation iteration encountered

static	int 	invalcallback(void *ptr, POINT *pt)

{
	CLaySline *obj = (CLaySline *)ptr;

	ASSERT(obj->gdata.kind == LAYER_SLINE);

	//P2N("invalcallback %p point x=%d y=%d\r\n", ptr, pt->x, pt->y);

	// officially x + 1(sqrt(2) but 1.5 covers it
	int hs = obj->_handlesize  + obj->_handlesize / 2;

	RECT rect; 
	
	rect.top  = pt->y; // - hs;
	rect.left = pt->x; // - hs;
	rect.bottom  = pt->y; // + hs;
	rect.right = pt->x; // + hs;

	expandrect(&rect, hs);

	CShadesDlg *dlg; GMD(dlg);	
	dlg->InvalidateRect(&rect, false);
	//dlg->InvalidateRect(&rect, true);

	// Testing ...
	//CClientDC dc(dlg); obj->DrawFocus(&dc, &rect);

	return 0;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLaySline::CLaySline(POINT pt)

{
	InitVars();
	Init(pt);
}

CLaySline::CLaySline()

{
	InitVars();
}

//////////////////////////////////////////////////////////////////////////

void CLaySline::InitVars()

{
	gdata.kind = LAYER_SLINE;

	sldata.ver		= 1;
	sldata.magic	= 1;
	sldata.line_thickness = 1;
	sldata.line_dashness = 0;

	sldata.arrow_start = sldata.arrow_end  = 0;

	sldata.cap_square = 0;
	sldata.arrow_len = sldata.arrow_elen = 20;
	sldata.arrow_ang = sldata.arrow_eang = 8;

	_handlesize = 10;	
}


CLaySline::~CLaySline()

{

}

int	 CLaySline::Draw(CDC *pDC, float factor, int dummy1)

{
	gl_factor = factor;

	//P2N("CLaySline::Draw xx=%d yy=%d ww=%d hh=%d\r\n",
	//		gdata.xx, gdata.yy, gdata.ww, gdata.hh);

	ASSERT(gdata.kind == LAYER_SLINE);
	
	CDC  dcMem;	
	//CDC	 *pDCm = &dcMem;
	CDC	 *pDCm = pDC;
	
	//GetMemDC(pDC, pDCm);

	LOGBRUSH lb; 
	lb.lbStyle = BS_SOLID;
	lb.lbColor = gdata.fg_color;
	lb.lbHatch = 0;

	int linestyle = PS_SOLID | PS_GEOMETRIC; 		

	if(sldata.cap_square)
		linestyle |= PS_ENDCAP_SQUARE;
		
	CPen Pen(linestyle, sldata.line_thickness, &lb);
	CPen* pOldPen = (CPen*)pDCm->SelectObject(&Pen);

	pDCm->MoveTo(gdata.xx, gdata.yy);
	pDCm->LineTo(gdata.xx + gdata.ww, gdata.yy + gdata.hh); 
	
	//pDCm->MoveTo(0,0);
	//pDCm->LineTo(gdata.ww, gdata.hh); 
	
	// Draw Arrows
	if(sldata.arrow_start || sldata.arrow_end)
		{
		double	angle = atan2(gdata.ww, gdata.hh);
		
		if(sldata.arrow_start)
			{
			int	alen  = sldata.arrow_len + sldata.line_thickness / 4;
			double	aang  = MATHPI * sldata.arrow_ang / 100;

			// Line angle:
			//P2N("Angle = %d\r\n", (int)(angle * 100));

			pDCm->MoveTo((int)((gdata.xx ) * factor),
					(int)((gdata.yy ) * factor));

			pDCm->LineTo((int)(gdata.xx + sin(angle + aang) * alen),
					(int)(gdata.yy + cos(angle + aang) * alen));

			pDCm->MoveTo((int)((gdata.xx ) * factor),
					(int)((gdata.yy ) * factor));

			pDCm->LineTo((int)(gdata.xx + sin(angle - aang) * alen),
					(int)(gdata.yy + cos(angle - aang) * alen));
			}

		if(sldata.arrow_end)
			{
			int	alen  = sldata.arrow_elen + sldata.line_thickness / 4;
			double	aang   = MATHPI * sldata.arrow_eang / 100;
			double	angle2 = angle2 = angle - MATHPI;

			int xxx = gdata.xx + gdata.ww;
			int	yyy = gdata.yy + gdata.hh;

			pDCm->MoveTo((int)((xxx ) * factor),
				(int)((yyy ) * factor));

			pDCm->LineTo((int)(xxx + sin(angle2 + aang) * alen),
					(int)(yyy + cos(angle2 + aang) * alen));

			pDCm->MoveTo((int)((xxx ) * factor),
				(int)((yyy ) * factor));

			pDCm->LineTo((int)(xxx + sin(angle2 - aang) * alen),
					(int)(yyy + cos(angle2 - aang) * alen));
			}
		}

	// Free resource(s)
	pDCm->SelectObject(pOldPen);

	//PutMemDC(pDC, pDCm);

	if(focus)
		{
		CRect rect(
			gdata.xx - _handlesize, gdata.yy - _handlesize, 
				gdata.xx + _handlesize, gdata.yy + _handlesize);
			
		rect.NormalizeRect ();

		//pDC->DrawFocusRect(&rect);
		DrawFocus(pDC, &rect);

		int midx = gdata.xx + gdata.ww / 2;
		int midy = gdata.yy + gdata.hh / 2;

		CRect rect1(
			midx - _handlesize, midy - _handlesize, 
			midx + _handlesize, midy + _handlesize);
			
		rect1.NormalizeRect ();

		//pDC->DrawFocusRect(&rect1);
		DrawFocus(pDC, &rect1);

		CRect rect2(
			gdata.xx + gdata.ww - _handlesize, gdata.yy + gdata.hh - _handlesize,
			gdata.xx + gdata.ww + _handlesize, gdata.yy + gdata.hh + _handlesize);
			
		//pDC->DrawFocusRect(&rect2);
		DrawFocus(pDC, &rect2);
		}

	//pDC->SelectObject(oldb);

//////////////////////////////////////////////////////////////////////////
// Test invalidate

#if 0
	updaterect.NormalizeRect();
	int resolv = 4;
	for(int loop = 0; loop < resolv; loop++)
		{
		CRect rc1(updaterect.left + loop * (updaterect.right - updaterect.left) / resolv, 
					updaterect.top + loop * (updaterect.bottom - updaterect.top) / resolv, 
						updaterect.left + (loop + 1) * (updaterect.right - updaterect.left) / resolv,
							updaterect.top + (loop + 1) * (updaterect.bottom - updaterect.top) / resolv);
		
		//rc1.NormalizeRect();
		//pDC->DrawFocusRect(rc1);
		//dlg->InvalidateRect(rc1, false);
		}
#endif

//////////////////////////////////////////////////////////////////////////
// Test hitest line

#if 0
	CShadesDlg *dlg; GMD(dlg);	CClientDC dc(dlg);

	double	loopy = 0, loopx = 0;
	int		cnt = 0, lim, inc;
		
	// Loop points on the larger axis
	if(abs(gdata.ww) > abs(gdata.hh))
		{
		double	xslope = (double)gdata.hh / gdata.ww;
		if(gdata.ww < 0) inc = -1;
		else			 inc = 1;
		lim = abs(gdata.ww);

		
		while(true)
			{
			dc.SetPixel(int(gdata.xx + loopx), int(gdata.yy + loopx * xslope), RGB(255,0,0));
			loopx += inc;
			if(cnt++ >= lim)
				break;
			}
	} 
	else 
		{
		double	yslope = (double)gdata.ww / gdata.hh;
		if(gdata.hh < 0) inc = -1;
		else   			 inc = 1;
		lim = abs(gdata.hh);

		while(true)
			{
			dc.SetPixel(int(gdata.xx + loopy * yslope), int(gdata.yy + loopy), RGB(255,0,0));
			loopy += inc;
			if(cnt++ >= lim)
				break;
			}
		}

#endif

	return 0;
}

int		CLaySline::Init(POINT pt, int dummy1, int dummy2)

{
	P2N("CLaySline::Init(POINT pt)\r\n");

	state = STATE_INIT;
	focus = true;

	gdata.xx = pt.x - 1;
	gdata.yy = pt.y - 1;

	gdata.ww = 8;
	gdata.hh = 8;

	GetRect(&startrect); GetRect(&endrect);

	return 0;
}

int		CLaySline::MouseEvent(UINT event, UINT nFlags, CPoint point)

{
	//P2N("CLaySline::MouseEvent focus=%d event=%s %d %d\r\n", 
	//							focus, Ma2Str(event), point.x, point.y);

	int ret = 0;

	GetRect(&startrect);

	switch(event)
		{
		case MOUSE_MOVE:
			{
			CRect oldrc(gdata.xx, gdata.yy,
			gdata.xx + gdata.ww, gdata.yy + gdata.hh);
	
			switch(state)
				{
				case STATE_INIT:
					{
					gdata.ww = point.x - gdata.xx;
					gdata.hh = point.y - gdata.yy;
					}
				break;

				case STATE_MOVE:
					{
					int rr = gdata.xx + gdata.ww; 
					int bb = gdata.yy + gdata.hh; 

					gdata.xx = point.x + dragoffx;
					gdata.yy = point.y + dragoffy;

					gdata.ww = rr - gdata.xx; 
					gdata.hh = bb - gdata.yy; 
					}
				break;

				case STATE_MOVE2:
					{
					gdata.ww = point.x - gdata.xx + dragoffx;
					gdata.hh = point.y - gdata.yy + dragoffy;
					}
				break;

				case STATE_MOVE3:
					{
					gdata.xx = point.x - dragoffx;
					gdata.yy = point.y - dragoffy;
					}
				break;
				}
		
			CRect newrc(gdata.xx, gdata.yy,  
				gdata.xx+gdata.ww, gdata.yy+gdata.hh);

			oldrc.NormalizeRect();
			newrc.NormalizeRect();

			updaterect = oldrc | newrc;
			//updaterect.InflateRect(10, 10);	
			}
		break;

		case    MOUSE_LEFTDOWN   : 	
			//P2N("CLaySline::MOUSE_LEFTDOWN x=%d y=%d\r\n", point.x, point.y);
			switch(state)
				{
				case STATE_MOVE:
					dragoffx = gdata.xx - point.x;
					dragoffy = gdata.yy - point.y;
				break;
				
				case STATE_MOVE3:
					{
					int midx = gdata.xx + gdata.ww / 2;
					int midy = gdata.yy + gdata.hh / 2;

					dragoffx = gdata.ww / 2 - (midx - point.x);
					dragoffy = gdata.hh / 2 - (midy - point.y);
					}
				break;
		
				case STATE_MOVE2:
					dragoffx = gdata.xx + gdata.ww - point.x;
					dragoffy = gdata.yy + gdata.hh - point.y;
				break;
				}
			break;

		case    MOUSE_LEFTUP   : 
			//P2N("CLaySline::MOUSE_LEFTUP x=%d y=%d\r\n", point.x, point.y);
			if(state == STATE_INIT)
				{
				state = STATE_IDLE;
				SetFocus(false);
				ret = true;
				}

			//P2N("CLaySline::MOUSE_LEFTUP x=%d y=%d\r\n", point.x, point.y);		
			break;

	    //case    MOUSE_RIGHTDOWN   : 			
		//	CXrayM *xxmenu = new CXrayM;
		//	xxmenu->LoadAndShow(IDR_MENU18, 1, point.x, point.y);		
		//	ret = true;
		//	break;

		case	MOUSE_LEFTDBL:
		case    MOUSE_RIGHTDOWN   :
			{
			//P2N("CLayText::MOUSE_RIGHTDOWN x=%d y=%d\r\n", point.x, point.y);		

			CShadesDlg *dlg; GMD(dlg);  CXrayDoc *doc; GCD(doc, dlg);
			if(doc && doc->m_currlay)
				{			
				// pop the RClick menu
				CXrayM *omenu = new CXrayM; ASSERT(omenu);
				omenu->LoadMenu(IDR_MENU15, 1);

				omenu->SetItemText(0, "Line Object");
				// Disable multi items
				if(dlg->helper->GetNumSelObj() < 2)
					{	
					omenu->SetItemStatus(5, MF_GRAYED);
					omenu->SetItemStatus(7, MF_GRAYED);
					omenu->SetItemStatus(8, MF_GRAYED);
					omenu->SetItemStatus(9, MF_GRAYED);
					}
				else
					{
					omenu->SetItemText(0, "Multiple Objects");
					}

				omenu->Show(point.x, point.y);
				ret = 0;
				}
			}
		break;
		}

	GetRect(&endrect);

	return ret;
}

int		CLaySline::KeyEvent(UINT event, UINT nFlags, UINT nChar)

{
	return -1;
}

int CLaySline::HitTest(POINT pt, int dummy1, int dummy2)

{
	//P2N("CLaySline::HitTest(POINT pt.x=%d pt.y=%d\r\n", pt.x, pt.y);

	// Quick check if it is within boundaries
	CRect fullrect;	GetRect(&fullrect);	fullrect.NormalizeRect ();
	fullrect.InflateRect(_handlesize, _handlesize);
	if(!fullrect.PtInRect(pt))
		{
		P2N("CLaySline::HitTest ret=%d\r\n", false);
		return false;
		}
	
	// Test for upper handle
	CRect rect(gdata.xx - _handlesize, gdata.yy - _handlesize, 
				gdata.xx + _handlesize, gdata.yy + _handlesize);
	rect.NormalizeRect ();

	if(rect.PtInRect(pt))
		{
		dragoffx = gdata.xx - pt.x;
		dragoffy = gdata.yy - pt.y;

		state = STATE_MOVE;
		//P2N("CLaySline::Hittest rect - %p\r\n", this);
		return true;
		}

	int midx = gdata.xx + gdata.ww / 2;
	int midy = gdata.yy + gdata.hh / 2;

	CRect rect1(
		midx - _handlesize, midy - _handlesize,
		midx + _handlesize, midy + _handlesize);

	rect1.NormalizeRect ();

	if(rect1.PtInRect(pt))
		{
		state = STATE_MOVE3;
		dragoffx = gdata.ww / 2 - (midx - pt.x);
		dragoffy = gdata.hh / 2 - (midy - pt.y);
		//P2N("CLaySline::Hittest rect1 - %p\r\n", this);
		return true;
		}
	
	CRect rect2(
		gdata.xx + gdata.ww - _handlesize, gdata.yy + gdata.hh - _handlesize,
		gdata.xx + gdata.ww + _handlesize, gdata.yy + gdata.hh + _handlesize);

	rect2.NormalizeRect ();

	if(rect2.PtInRect(pt))
		{
		dragoffx = gdata.xx + gdata.ww - pt.x;
		dragoffy = gdata.yy + gdata.hh - pt.y;

		state = STATE_MOVE2;
		//P2N("CLaySline::Hittest rect2 - %p\r\n", this);
		return true;
		}

	// Find a hit iterating calculated points of the line
	_iterpt = pt; _iterhit = false; 
	
	RECT rc; GetRect(&rc);
	IterateLine(this, _handlesize, &rc, callback);

	if(_iterhit)
		{
		dragoffx = gdata.ww / 2 - (midx - pt.x);
		dragoffy = gdata.hh / 2 - (midy - pt.y);

		state = STATE_MOVE3;
		//P2N("CLaySline::Hittest rectx - %p\r\n", this);
		return true;
		}
	
 	return 0;
}

//////////////////////////////////////////////////////////////////////////
/// Test one point of the line for proximity. 
/// Resolve as '_handlesize' sized rectangles

int	 CLaySline::_TestLinePt(int px, int py, CPoint pt)

{
	CRect rect3 (px - _handlesize, py - _handlesize,
					px + _handlesize, py + _handlesize);

	rect3.NormalizeRect();

	if(rect3.PtInRect(pt))
		{
		int midx = gdata.xx + gdata.ww / 2;
		int midy = gdata.yy + gdata.hh / 2;

		dragoffx = gdata.ww / 2 - (midx - pt.x);
		dragoffy = gdata.hh / 2 - (midy - pt.y);

		state = STATE_MOVE3;

		//P2N("CLaySline::Hittest rectx - %p\r\n", this);
		return true;
		}
	return false;
}

int	 CLaySline::Invalidate(int dummy1, int dummy2, int dummy3)

{
	//P2N("CLaySline::Invalidate\r\n");

	CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);
	CXrayDoc *doc = ((CShadesDlg* )AfxGetApp()->m_pMainWnd)->GetCurrDoc();

	if(!doc)
		return 0;

	//GetRect(&newrc); newrc.NormalizeRect();
	//CRect rc; CopyRect(&rc, &startrect); 
	//CRect rc2; CopyRect(&rc2, &endrect); 
	//updaterect = rc | rc2;

	CRect ur(updaterect); ur.NormalizeRect();
	ur.InflateRect(2 * sldata.line_thickness +
				_handlesize, 2 * sldata.line_thickness + _handlesize);
	
	dlg->InvalidateRect(ur, false);
	// Flicker helps see the real update
	//dlg->InvalidateRect(ur);

	// Update end points as well
	InvalHandle(dlg, gdata.xx, gdata.yy);
	InvalHandle(dlg, oldrc.left, oldrc.top);

	InvalHandle(dlg, gdata.xx + gdata.ww, gdata.yy + gdata.hh);
	InvalHandle(dlg, oldrc.right, oldrc.bottom);

	return 0;

	// Abandoned smart update

#if 0
	//P2N("CLaySline::Invalidate\r\n");

	//CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);
	//CXrayDoc *doc = ((CShadesDlg* )AfxGetApp()->m_pMainWnd)->GetCurrDoc();

	//if(!doc)
	//	return 0;

	CRect rc; CopyRect(&rc, &startrect); 
	CRect rc2; CopyRect(&rc2, &endrect); 

	// Testing ...
	//CClientDC dc(dlg); 
	//DrawFocus(&dc, &rc);	DrawFocus(&dc, &rc2);

	IterateLine(this, _handlesize, &rc, invalcallback);
	IterateLine(this, _handlesize, &rc2, invalcallback);

	if(arrow_start)
		{
		}
		
	if(arrow_end)
		{
		}

#endif

	return 0;
}


int	 CLaySline::Prop(int dummy1, int dummy2, int dummy3)

{
	ClineProp lp;	
	lp.lay = this;
	lp.DoModal();

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// Update end points as well as arrows

int	 CLaySline::InvalHandle(CWnd *dlg, int xx, int yy)

{
	int arr_len = MAX(sldata.arrow_len, sldata.arrow_elen);
	int endsize = MAX(_handlesize, sldata.line_thickness + arr_len);

	CRect rc(xx - endsize, yy - endsize, xx + endsize, yy + endsize);

	rc.NormalizeRect(); 
	rc.InflateRect(sldata.line_thickness + 4, sldata.line_thickness + 4);
	
	//dlg->InvalidateRect(rc);
	dlg->InvalidateRect(rc, false);
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////

void CLaySline::Dup(void **ptr, int *len)

{
	*len = sizeof(sldata);	*ptr = malloc(*len);
	if(*ptr)
		{
		memcpy(*ptr, &sldata, sizeof(sldata));
		}
}
