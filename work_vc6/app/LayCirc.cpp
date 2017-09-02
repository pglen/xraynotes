
/* =====[ LayCirc.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the LayCirc.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  11/28/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// LayCirc.cpp: implementation of the CLayCirc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "xraynotes.h"
#include "LayCirc.h"


#include "gdirot.h"
#include "XntMask.h"
#include "BmpDC.h"

#include "ShapeProp.h"

#include "Layer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
// 
// Clear Clipping region BUG: this is hard 
// All solved by the SaveDC/RestoreDC 
// (encapsulated in the saveDC class)

//CShadesDlg *dlg; GMD(dlg);
//CRect rcc(dlg->xpos, dlg->ypos, 
//		dlg->xpos + dlg->renx, dlg->ypos + dlg->reny);

// Version ONE : uses path, restores clipbox
//pDC->BeginPath();
//pDC->Rectangle(&rcc);
//pDC->EndPath();		
//pDC->SelectClipPath(RGN_COPY);

// Version TWO : uses regions, restores clipbox
//CRgn rgn; rgn.CreateRectRgnIndirect(&rcc);
//pDC->SelectClipRgn(&rgn);

// Version THREE : destroys all of it (faster)
//pDC->SelectClipRgn(NULL);	

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLayCirc::CLayCirc()

{
	//P2N("CLayCirc::CLayCirc()\r\n");
	gdata.kind = LAYER_ELLIPSE;
}


CLayCirc::~CLayCirc()

{

}

//////////////////////////////////////////////////////////////////////////

int CLayCirc::Draw(CDC *pDC, float factor, int dummy1)

{
	//P2N("CLayCirc::Draw xx=%d yy=%d ww=%d hh=%d focus=%d alpha=%d\r\n",
	//		gdata.xx, gdata.yy, gdata.ww, gdata.hh, focus, gdata.alpha);

	ASSERT(gdata.kind == LAYER_ELLIPSE);
	
	CRect rcorg; GetRect(&rcorg); 

	// The GDI support objects
	XntMask xmask; CGdiRot rotdc; 

	//P2N("CLayCirc::Draw\r\n");
	
	float angle = -(float)((MATHPI * gdata.rotate_angle) / 180.0);
	
	// Get new rotated dimensions
	RECT rc5; 
	rotdc.GetRotateRC(rcorg.Width(), rcorg.Height(), angle, &rc5);
	dstW = rc5.right; dstH = rc5.bottom;

    CDC  dcMem;	GetMemDC(pDC, &dcMem);
	{ //CSaveDC 
	CSaveDC sdc(pDC);

	LOGBRUSH lb; 
	lb.lbStyle = gdata.brush_type;
	lb.lbColor = gdata.bg_color; 
	lb.lbHatch = gdata.brush_hatch;

	if(lb.lbStyle == BS_HATCHED)
		{		
		lb.lbColor = gdata.hatch_color;
		}

	CBrush brush; brush.CreateBrushIndirect(&lb);	
	CPen pen; 
	pen.CreatePen(gdata.line_type, gdata.line_width, gdata.fg_color);
		
	// Fill it in, draw and transfer
	CRect rc(0, 0, abs(gdata.ww), abs(gdata.hh));	
	//CRect rc2; GetRect(&rc2);		
	
	// Fake black back to non transparent
	if(gdata.bg_color == 0)
		{
		gdata.bg_color = RGB(1,1,1);	
		}

	if(lb.lbStyle == BS_NULL)
		{
		dcMem.SetBkColor(RGB(0,0,0));
		lb.lbStyle = BS_SOLID;
		lb.lbColor = RGB(0,0,0);		
		}
	else
		{
		dcMem.SetBkColor(gdata.bg_color);
		dcMem.SetTextColor(gdata.fg_color);
		}

	dcMem.SelectObject(&brush);	dcMem.SelectObject(&pen);
	dcMem.Ellipse(&rc);

	//if(gdata.line_type != PS_NULL)
	//	rc2.InflateRect(1, 1);  // So lines show up good

	// Creata PATH for transfer
	// Overridden by mask/rotate code

	//pDC->BeginPath(); pDC->Ellipse(&rc2); pDC->EndPath();		
	//pDC->SelectClipPath(RGN_AND);
	//PutMemDC(pDC, &dcMem);

	if(gdata.rotate_angle != 0)
		{
		// Create the new memory DCs
		CBmpDC hdcDst(dcMem, dstW, dstH);

		CRect rc4;
		rc4.left = rcorg.left + (rcorg.Width()/2 - dstW/2);
		rc4.top  = rcorg.top + (rcorg.Height()/2 - dstH/2);
		rc4.right  = rc4.left + dstW;
		rc4.bottom = rc4.top + dstH;

		//////////////////////////////////////////////////////////////////////////
		// Rotate that memory HDC
		rotdc.RotateMemoryDC(dcMem, rcorg.Width(), rcorg.Height(), angle, hdcDst);

		HBITMAP hBmpMask = xmask.MakeMaskBmpDC(hdcDst, dstW, dstH, 0);

		// Put it out
		xmask.MaskBlt(pDC->GetSafeHdc(), rc4.left, rc4.top,	dstW, dstH, 
					hdcDst, 0, 0, hBmpMask, 0, 0,
							MAKEROP4(SRCCOPY, DSTCOPY), gdata.alpha);
		}
	else
		{
		HBITMAP hBmpMask =	xmask.MakeMaskBmpDC(dcMem, 
								rcorg.Width(), rcorg.Height(), 0);

		// Put it out
		xmask.MaskBlt(pDC->GetSafeHdc(), rcorg.left, rcorg.top,	
					rcorg.Width(), rcorg.Height(),
						dcMem, 0, 0, hBmpMask, 0, 0,
							MAKEROP4(SRCCOPY, DSTCOPY), gdata.alpha);

		//PutMemDC(pDC, &dcMem);
		}

	FreeMemDC(pDC, &dcMem);
	} // SaveDC

	if(focus)
		{
		CRect rect; GetRect(&rect);	rect.InflateRect(2,2);

		if(angle != 0)
			{
			CRect rc3, rc4; GetRect(&rc3); 

			rc4.left = rc3.left + (rc3.Width()/2 - dstW/2);
			rc4.top  = rc3.top + (rc3.Height()/2 - dstH/2);

			rc4.right  = rc4.left + dstW;
			rc4.bottom = rc4.top + dstH;

			rect = rc4;
			}

		DrawFocus(pDC, &rect);
		DrawTrackerX(pDC, &rect, state == STATE_ROTATE ? true : false);
		}
	return -0;
}

void CLayCirc::InitVars()

{
	gdata.kind = LAYER_ELLIPSE;	
	
	//brushtype = BS_SOLID;
	//hatchtype = HS_DIAGCROSS;
}

int CLayCirc::Init(POINT pt, int dummy1, int dummy2)

{
	state = STATE_INIT;

	gdata.xx = pt.x;
	gdata.yy = pt.y;

	gdata.ww = 8;
	gdata.hh = 8;

	return 0;
}

//////////////////////////////////////////////////////////////////////////

int CLayCirc::MouseEvent(UINT event, UINT nFlags, CPoint point)

{
	int ret = -1;

	switch(event)
		{
		case MOUSE_RIGHTDOWN   :
			{
			CShadesDlg *dlg; GMD(dlg);  CXrayDoc *doc; GCD(doc, dlg);
			if(doc && doc->m_currlay)
				{			
				// pop the RClick menu
				CXrayM *omenu = new CXrayM; ASSERT(omenu);
				omenu->LoadMenu(IDR_MENU15, 1);

				omenu->SetItemText(0, "Circle Object");
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
	return ret;
}

int	 CLayCirc::Prop(int dummy1, int dummy2, int dummy3)

{
	CShapeProp sp;	

	sp.m_title = "Circle Properties";
	sp.lay = this;
	sp.DoModal();

	return 0;
}


int	 CLayCirc::Invalidate(int dummy1, int dummy2, int dummy3)

{
	//P2N("CLayRect::Invalidate\r\n");
	
	CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);
	CXrayDoc *doc = ((CShadesDlg* )AfxGetApp()->m_pMainWnd)->GetCurrDoc();

	if(!doc)
		return 0;

	CRect ur(updaterect); ur.NormalizeRect();

	ur.InflateRect(4 + gdata.line_width, 4 + gdata.line_width);

	if(dstW > gdata.ww)
		{
		ur.InflateRect(dstW - gdata.ww, 0);
		}

	if( dstH > gdata.hh)
		{
		ur.InflateRect(0, dstH - gdata.hh);
		}

	dlg->InvalidateRect(ur, false);

	// Flicker helps see the real update
	//dlg->Invalidate();

	return 0;
}
