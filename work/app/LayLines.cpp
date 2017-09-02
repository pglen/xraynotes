
/* =====[ LayLines.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the LayLines.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  1/16/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// LayLines.cpp: implementation of the CLayLines class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "xraynotes.h"

#include "lineProp.h"
#include "Layer.h"	

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLayLines::CLayLines()

{
	InitVars();	
}

CLayLines::~CLayLines()

{

}

int CLayLines::Init(POINT pt, int dummy1, int dummy2)

{
	state = STATE_INIT;

	gdata.xx = pt.x;
	gdata.yy = pt.y;

	gdata.ww = 8;
	gdata.hh = 8;

	return 0;
}

void CLayLines::InitVars()

{
	gdata.kind = LAYER_LINES;

	//line_thickness = 1;
	line_dashness = line_init = 0;
	handlesize = 10;
	//line_color = RGB(0, 0, 0);
	pen_color = RGB(255, 255, 255);
}

//////////////////////////////////////////////////////////////////////////

int CLayLines::Draw(CDC *pDC, float factor, int dummy1)

{
	//P2N("CLayLines::Draw %d points\r\n", line_x.GetSize());

	ASSERT(gdata.kind == LAYER_LINES);

	// Check fundamental conditions
	if (!pDC)
		return 0;

	num_lines = line_x.GetSize();

	if(!num_lines)
		return 0;

	//////////////////////////////////////////////////////////////////////////
	//P2N("Drawing %d lines\r\n", num_lines);

	CPen Pen(PS_SOLID, gdata.line_width, gdata.fg_color);
	CPen Pen2;

	Pen2.CreatePen(PS_SOLID, gdata.line_width, gdata.bg_color);

	//if(pen_color == RGB(255, 255, 255))
	//else
	//	Pen2.CreatePen(PS_SOLID, line_thickness, gdata.bg_color);

	CPen* pOldPen = (CPen*)pDC->SelectObject(&Pen);

	// Draw it all
	pDC->MoveTo((int)((gdata.xx + line_x[0]) * factor), 
					(int)((gdata.yy + line_y[0]) * factor));

	for(int loop = 1; loop < num_lines; loop++)
		{
		// Select line colors
		if(line_dashness)
			{
			if(loop % 2)
				pDC->SelectObject(&Pen);
			else
				pDC->SelectObject(&Pen2);
			}
		pDC->LineTo((int)((gdata.xx + line_x[loop]) * factor),
						(int)((gdata.yy + line_y[loop]) * factor) );
		}

	// Free resource(s)
	pDC->SelectObject(pOldPen);

	if(focus)
		{
		CRect rect;	GetRect(&rect);	rect.NormalizeRect ();
		rect.InflateRect(2, 2);
	
		DrawFocus(pDC, &rect);
		}
	return 0;
}

//////////////////////////////////////////////////////////////////////////

int		CLayLines::MouseEvent(UINT event, UINT nFlags, CPoint point)

{
	//P2N("CLayLines::MouseEvent focus=%d event=%s %d %d\r\n", 
	//							focus, Ma2Str(event), point.x, point.y);

	int ret = -1;

	GetRect(&startrect);

	switch(event)
		{
		case MOUSE_MOVE:
			{
			switch(state)
				{
				case STATE_INIT:
					{
					AddLinePoint(point.x, point.y);
					ret = 0;
					}
				break;
				}
			}
		break;

		case    MOUSE_RIGHTDOWN   :
			{
			CShadesDlg *dlg; GMD(dlg);  CXrayDoc *doc; GCD(doc, dlg);
			if(doc && doc->m_currlay)
				{			
				// pop the RClick menu
				CXrayM *omenu = new CXrayM; ASSERT(omenu);
				omenu->LoadMenu(IDR_MENU15, 1);

				omenu->SetItemText(0, "Freehand Lines Object");
				// Disable multi items
				if(dlg->helper->GetNumSelObj() < 2)
					{	
					omenu->SetItemStatus(7, MF_GRAYED);
					omenu->SetItemStatus(8, MF_GRAYED);
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
	
//////////////////////////////////////////////////////////////////////////

void CLayLines::AddLinePoint(int px, int py)

{
	//PrintToNotepad("Addlinepoint %p x=%d y=%d count =%d\r\n",
	//	this, px, py, line_x.GetSize() );

	int rel_x = 0, rel_y = 0;

	if(line_x.GetSize() == 0)
		{
		// Remember first layer point as origin
		gdata.xx = px;
		gdata.yy = py;
		rel_x	= 20;
		rel_y	= 20;
		}

	// Auto Expand boundaries
	if(gdata.xx > px)
		{
		// Move points so the line stays
		int diff = gdata.xx - px;
		num_lines = line_x.GetSize();
		for(int loop = 0; loop < num_lines; loop++)
			{
			line_x[loop] += diff;
			gdata.ww = max(gdata.ww, (int)line_x[loop]);
			}
		gdata.xx -= diff;
		}	
	if(gdata.yy > py)
		{
		// Move points so the line stays
		int diff = gdata.yy - py;
		num_lines = line_y.GetSize();
		for(int loop = 0; loop < num_lines; loop++)
			{
			line_y[loop] += diff;
			gdata.hh = max(gdata.hh, (int)line_y[loop]);
			}
		gdata.yy -= diff;
		}

	// Convert it to layer relative coordinates
	rel_x =  px - gdata.xx;
	rel_y =  py - gdata.yy;

	gdata.ww = max(gdata.ww, rel_x);
	gdata.hh = max(gdata.hh, rel_y);
	
	line_x.Add(rel_x);
	line_y.Add(rel_y);
}

//////////////////////////////////////////////////////////////////////////
// Create rectangles around the points of the line,
// and invalidate them

int	 CLayLines::Invalidate(int dummy1, int dummy2, int dummy3)

{
	//P2N("CLayLines::Invalidate\r\n");
	
	CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);
	CXrayDoc *doc = ((CShadesDlg* )AfxGetApp()->m_pMainWnd)->GetCurrDoc();

	if(!doc)
		return 0;

	CRect ur(updaterect); ur.NormalizeRect();
	ur.InflateRect(4, 4);

	dlg->InvalidateRect(ur, false);

	// Flicker helps see the real update
	//dlg->InvalidateRect(ur);

	return 0;

	// Abandoned smart update
	
	//CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);
	//CXrayDoc *doc = ((CShadesDlg* )AfxGetApp()->m_pMainWnd)->GetCurrDoc();

	//if(!doc)
	//	return 0;

	num_lines = line_x.GetSize();

	if(!num_lines)
		return 0;

	int loop;
	int prevx  = oldrc.left + line_x[0];
	int prevy = oldrc.top + line_y[0];

	//P2N("oldrc= %d %d\r\n", oldrc.left, oldrc.top);

	// Invalidate old place's rectangles
	for(loop = 0; loop < num_lines; loop++)
		{
		CRect rc2;
		SetRect(&rc2, prevx, prevy,
							oldrc.left + line_x[loop], 
								oldrc.top + line_y[loop]);

		prevx  = oldrc.left + line_x[loop];
		prevy = oldrc.top + line_y[loop];

		rc2.NormalizeRect(); rc2.InflateRect(handlesize, handlesize);

		// Testing ...
		//CClientDC dc(dlg); DrawFocus(&dc, &rc2);

		dlg->InvalidateRect(&rc2, false);
		}

	prevx  = gdata.xx + line_x[0];
	prevy = gdata.yy + line_y[0];
	
	// Invalidate new rectangles
	for(loop = 1; loop < num_lines; loop++)
		{
		CRect rc2;
		SetRect(&rc2, prevx, prevy,
							gdata.xx + line_x[loop], 
								gdata.yy + line_y[loop]);

		prevx  = gdata.xx + line_x[loop];
		prevy = gdata.yy + line_y[loop];

		rc2.NormalizeRect(); rc2.InflateRect(handlesize, handlesize);

		// Testing ...
		//CClientDC dc(dlg); DrawFocus(&dc, &rc2);

		dlg->InvalidateRect(&rc2, false);
		}

	// Also draw the rects in focus
	if(focus)
		{
		CRect rc2;

		// Upper
		SetRect(&rc2, gdata.xx, gdata.yy,  gdata.xx + gdata.ww, gdata.yy);
		rc2.NormalizeRect(); rc2.InflateRect(4,4);
		dlg->InvalidateRect(&rc2, false);

		CRect rc3;
		// Right
		SetRect(&rc3, gdata.xx + gdata.ww, gdata.yy,  
					gdata.xx + gdata.ww, gdata.yy + gdata.hh);
		rc3.NormalizeRect(); rc3.InflateRect(4,4);
		dlg->InvalidateRect(&rc3, false);

		CRect rc4;
		// Lower
		SetRect(&rc4, gdata.xx, gdata.yy + gdata.hh,  
					gdata.xx + gdata.ww, gdata.yy + gdata.hh);
		rc4.NormalizeRect(); rc4.InflateRect(4,4);
		dlg->InvalidateRect(&rc4, false);

		CRect rc5;
		// Left
		SetRect(&rc5, gdata.xx, gdata.yy,  
					gdata.xx, gdata.yy + gdata.hh);
		rc5.NormalizeRect(); rc5.InflateRect(4,4);
		dlg->InvalidateRect(&rc5, false);

		// Testing ...
		//CClientDC dc(dlg); DrawFocus(&dc, &rc2);
		}

	return 0;
}
		
//////////////////////////////////////////////////////////////////////////
//
// Hittest rectables around the line's points
//

int CLayLines::HitTest(POINT pt, int dummy1, int dummy2)

{
	//P2N("CLaySline::HitTest(POINT pt.x=%d pt.y=%d\r\n", pt.x, pt.y);

	CRect rect;	GetRect(&rect);	rect.NormalizeRect ();

	// Quick check if it is withing boundaries
	if(!rect.PtInRect(pt))
		{
		//P2N("CLaySline::HitTest ret=%d\r\n", false);
		return false;
		}
		
	num_lines = line_x.GetSize();
	for(int loop = 0; loop < num_lines; loop++)
		{
		RECT rc2;
		SetRect(&rc2, gdata.xx + line_x[loop] - handlesize, 
						gdata.yy + line_y[loop] - handlesize,
							gdata.xx + line_x[loop] + handlesize, 
								gdata.yy + line_y[loop] + handlesize);

		if(PtInRect(&rc2, pt))
			{
			//P2N("CLaySline::HitTest ret=%d\r\n", true);
			return true;
			}
		//CShadesDlg  *dlg = GMD(dlg); CClientDC dc(dlg); 
		//DrawFocus(&dc, &rc2);	
		}

	//P2N("CLaySline::HitTest ret=%d\r\n", false);
		
 	return 0;
}

//////////////////////////////////////////////////////////////////////////

int	 CLayLines::Prop(int dummy1, int dummy2, int dummy3)

{
	ClineProp lp;	
	lp.lay = (CLaySline*)this;
	lp.DoModal();

	return 0;
}