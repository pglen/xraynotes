
/* =====[ xTracker.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the xTracker.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  2/2/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// xTracker.cpp: implementation of the CxTracker class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "xraynotes.h"
#include "xTracker.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
// Macros to draw corners

	
#define	DRAWONE(rcc, ll, tt)				\
	rcc.SetRect(ll - twidth, tt - twidth,	\
				ll + twidth, tt + twidth);	\
	DrawFocus(pDC, &rcc, true);				\


// Add a fake square root 2 to the circle width

#define	DRAWCIRC(rcc, ll, tt)		    	\
	{										\
	int twidth2 =  (twidth * 141) / 100;	\
	rcc.SetRect(ll - twidth2, tt - twidth2,	\
				ll + twidth2, tt + twidth2);\
	DrawFocEllipse(pDC, &rcc, true);		\
	}										\


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CxTracker::CxTracker()
{
	lasthit = 0;
}

CxTracker::~CxTracker()
{

}

//////////////////////////////////////////////////////////////////////////

void CxTracker::Draw(CDC *pDC, CRect *rc, int twidth, int circ)

{
	// Remember the last tracker dimentions
	whole = *rc;

	//if(circ)
	//	twidth =  (twidth * 141) / 100;  // fake sqrt 2

	//P2N("CxTracker::Draw twidth=%d\r\n", twidth);

	CPen pen; pen.CreatePen(PS_SOLID, 1, RGB(192, 192, 192)); 

	COLORREF old_col = pDC->GetBkColor(); 
	pDC->SetBkColor(RGB(64, 64, 64));

	LOGBRUSH lb; ZeroMemory(&lb, sizeof(lb)); lb.lbStyle = BS_NULL; 
	CBrush br; br.CreateBrushIndirect(&lb);

	CBrush	*old_brush	= pDC->SelectObject(&br);
	CPen	*old_pen	= pDC->SelectObject(&pen);

	// Draw corners
	DRAWONE(rc1, rc->left, rc->top);
	DRAWONE(rc2, rc->right, rc->top)
	DRAWONE(rc3, rc->right, rc->bottom);
	DRAWONE(rc4, rc->left,  rc->bottom);

	int midx = rc->left + rc->Width() / 2;
	int midy = rc->top  + rc->Height() / 2;

	// Draw mids
	DRAWONE(rc1m, midx,  rc->top);
	DRAWONE(rc2m, rc->right, midy);
	DRAWONE(rc3m, midx,  rc->bottom);		
	DRAWONE(rc4m, rc->left, midy);

	if(rc->Height() > 10 * twidth)
		{
		// Draw circles
		DRAWCIRC(rc1c, rc->left, rc->top + 3 * twidth);
		DRAWCIRC(rc2c, rc->right, rc->top + 3 * twidth)
		DRAWCIRC(rc3c, rc->right, rc->bottom - 3 * twidth);
		DRAWCIRC(rc4c, rc->left,  rc->bottom - 3 * twidth);
		}

	// Restore of CDC values
	pDC->SelectObject(old_brush);
	pDC->SelectObject(old_pen);
	pDC->SetBkColor(old_col);
}

//////////////////////////////////////////////////////////////////////////

void CxTracker::DrawFocus(CDC *pDC, RECT *rc, int solid)

{
	pDC->MoveTo(rc->left,  rc->top);
	
	pDC->LineTo(rc->right, rc->top);
	pDC->LineTo(rc->right, rc->bottom);
	pDC->LineTo(rc->left,  rc->bottom);
	pDC->LineTo(rc->left,  rc->top);
}


void CxTracker::DrawFocEllipse(CDC *pDC, RECT *rc, int solid)

{
	pDC->Ellipse(rc);
}


//////////////////////////////////////////////////////////////////////////

#define HITTEST_RECT(rrc, def)		\
	if(rrc.PtInRect(point))			\
		{							\
		ret = def; goto endd;		\
		}	

//////////////////////////////////////////////////////////////////////////

int CxTracker::HitTest(CPoint point)

{
	//P2N("CxTracker::HitTest\r\n");

	int ret = NO_HIT;

	HITTEST_RECT(rc1, UL_HIT);
	HITTEST_RECT(rc2, UR_HIT);
	HITTEST_RECT(rc3, LR_HIT);
	HITTEST_RECT(rc4, LL_HIT);

	HITTEST_RECT(rc1m, UM_HIT);
	HITTEST_RECT(rc2m, RM_HIT);
	HITTEST_RECT(rc3m, BM_HIT);
	HITTEST_RECT(rc4m, LM_HIT);

	HITTEST_RECT(rc1c, ULC_HIT);
	HITTEST_RECT(rc2c, LLC_HIT);
	HITTEST_RECT(rc3c, URC_HIT);
	HITTEST_RECT(rc4c, LRC_HIT);
	
	// Generic test
	//if(whole.PtInRect(point))
	//	{
	//	ret = WHOLE_HIT; goto endd;
	//	}

endd:

	lasthit = ret;

	//if(ret)
	//	P2N("CxTracker::HitTest ret=%d\r\n", ret);

	return ret;	
}

