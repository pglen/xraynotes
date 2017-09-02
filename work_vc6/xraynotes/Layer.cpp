
/* =====[ Layer.cpp ]==========================================

   Description:     The xraynotes project, implementation of the Layer.cpp

                    Defines the behavior for the application.

   Compiled:        MS-VC 6.00

   Notes:           <Empty Notes>

   Revisions:

      REV     DATE        BY            DESCRIPTION
      ----  --------  -----------  ----------------------------
      0.00  9/17/2007  Peter Glen   Initial version.

   ======================================================================= */

//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "xraynotes.h"
#include "UniqID.h"
#include "Layer.h"
#include "TextMode.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// Template for branching out to object kind

#if 0
	switch(layer)
		{
		case    LAYER_NONE      :   break;
		case    LAYER_LINES     :   break;
		case    LAYER_SLINE     :   break;
		case    LAYER_RECT      :   break;
		case    LAYER_ELLIPSE   :   break;
		case    LAYER_TEXT      :   break;
		case    LAYER_IMAGE     :   break;
		case    LAYER_MAG       :   break;
		case    LAYER_EYE       :   break;
		case    LAYER_MEASURE   :   break;
		case    LAYER_SVG       :   break;
		case    LAYER_CONT      :   break;
		default					:	break;	
		}
#endif

//////////////////////////////////////////////////////////////////////////
// Macro to call a function if defined
// If no derived function exist, pointer resolves the same as 'self'
//

#define CALL_IFDEF(func, rety, casted,arg1,arg2,arg3)			\
	{															\
	if( ((casted)this)->func != this->func)						\
		rety = ((casted)this)->func(arg1,arg2,arg3);  			\
	else														\
		rety = -1;												\
	}

//////////////////////////////////////////////////////////////////////////
// Macro to build the branch code:

#define BRANCH_CODE(func, retx, arg1,arg2,arg3)					\
	switch(gdata.kind)											\
		{														\
		case    LAYER_NONE      :								\
			break;                          					\
                                            					\
		case    LAYER_LINES     :           					\
			CALL_IFDEF(func, retx, CLayLines*, arg1,arg2,arg3);	\
			break;                          					\
			                                					\
		case    LAYER_SLINE     :                       		\
			CALL_IFDEF(func, retx, CLaySline*, arg1,arg2,arg3);	\
			break;                                      		\
                                                        		\
		case    LAYER_RECT      :                       		\
			CALL_IFDEF(func, retx, CLayRect*, arg1,arg2,arg3);	\
			break;                                      		\
                                                        		\
		case    LAYER_ELLIPSE   :                       		\
			CALL_IFDEF(func, retx, CLayCirc*, arg1,arg2,arg3);	\
			break;                                      		\
                                                        		\
		case    LAYER_TEXT:                       				\
			CALL_IFDEF(func, retx, CLayText*, arg1,arg2,arg3);	\
			break;                                      		\
                                                        		\
		case    LAYER_IMAGE     :								\
			CALL_IFDEF(func, retx, CLayImg*, arg1,arg2,arg3);	\
			break;												\
	                                                    		\
		default:			                            		\
			P2N("Invalid layer %d\r\n", gdata.kind);   			\
			break;                                  			\
		}                                   					\
    
//
// Still to go:		
//		case    LAYER_MAG       :   break;              		\
//		case    LAYER_EYE       :   break;              		\
//		case    LAYER_MEASURE   :   break;              		\
//		case    LAYER_SVG       :   break;              		\
//		case    LAYER_CONT      :   break;              		\

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLayer::CLayer()

{
	P2N("CLayer::CLayer()\r\n");
	InitVars();
}

CLayer::CLayer(int kindd)

{
	P2N("CLayer::CLayer(int kind=%d)\r\n", kindd);

	gdata.kind = kindd;
	InitVars();
}

//////////////////////////////////////////////////////////////////////////
// Destroy allocated sublayer

CLayer::~CLayer()

{
	//P2N("CLayer::~CLayer %s\r\n", GetKindStr());
}

//////////////////////////////////////////////////////////////////////////

void CLayer::Dump()

{
      P2N("kind=%s "             , GetKindStr()    );
      P2N("xx=%3d "              , gdata.xx        );
      P2N("yy=%3d "              , gdata.yy        );
      P2N("ww=%3d "              , gdata.ww        );
      P2N("hh=%3d "              , gdata.hh        );
	  P2N("focus=%d "            , focus	       );

	  if(gdata.kind == LAYER_TEXT)
		{
		P2N("text= '%s' ", ((CLayText*)this)->text);
		}
	  P2N("\r\n");
}

//////////////////////////////////////////////////////////////////////////
// Mostly for debugging ...

char *CLayer::GetKindStr()

{
    char * str = "Invalid ";
    switch(gdata.kind)
        {
        case LAYER_NONE     :     str =    "LAYER_NONE "   ; break;
        case LAYER_TEXT     :     str =    "LAYER_TEXT "   ; break;
		case LAYER_RECT     :     str =    "LAYER_RECT "   ; break;
        case LAYER_LINES    :     str =    "LAYER_LINES "  ; break;
        case LAYER_IMAGE    :     str =    "LAYER_IMAGE "  ; break;
        case LAYER_ELLIPSE  :     str =    "LAYER_ELLIPSE "; break;
        case LAYER_SLINE    :     str =    "LAYER_SLINE "  ; break;
        case LAYER_MAG      :     str =    "LAYER_MAG "    ; break;
        }
    return(str);
}

//////////////////////////////////////////////////////////////////////////

int CLayer::Draw(CDC *pDC, float factor, int dummy1)

{
	int ret = 0;

	if(in_draw)
		return ret;

	in_draw = true;

	//P2N("CLayer::Draw %s\r\n", GetKindStr());

	gl_factor = factor;

	BRANCH_CODE(Draw, ret, pDC, factor, 0);

	// Display error information in the object
	if(ret < 0)
		{
		RECT rc; GetRect(&rc);
		//Expand
		if(gdata.ww < 20)
			{
			gdata.ww = 300; gdata.hh = 40;
			GetRect(&rc);
			}

		CString str("No draw handler defined for this object");
		pDC->DrawText(str, &rc, DT_CENTER | DT_VCENTER);
		}

	in_draw = false;

	return ret;
}

//////////////////////////////////////////////////////////////////////////

void CLayer::QDump()

{
    P2N("kind=%s xx=%d yy=%d ww=%d hh=%d\r\n", GetKindStr(), gdata.xx, gdata.yy, gdata.ww, gdata.yy);
}

//////////////////////////////////////////////////////////////////////////
// Contains the common code for layers

int CLayer::MouseEvent(UINT event, UINT nFlags, CPoint point)

{
	int ret = 0;

	//P2N("CLayer::MouseEvent(event=%s, nFlags=%d, point.x=%d point.y=%d)\r\n",
	//				Ma2Str(event), nFlags, point.x, point.y);

	if(in_mouse)
		{
		P2N("CLayer::MouseEvent re-entered\r\n");
		return 0;
		}

	in_mouse = true;

	GetRect(&oldrc); oldrc.NormalizeRect();

	BRANCH_CODE(MouseEvent, ret, event, nFlags, point);

	// Post process if needed:
	if(ret < 0)
		{
		switch(event)
			{
			case    MOUSE_LEFTDOWN    : 
				dragoffx = gdata.xx - point.x; 
				dragoffy = gdata.yy - point.y; 
				break;

			case    MOUSE_LEFTUP      :				
				if(state == STATE_INIT)
					state = STATE_IDLE;
				else if(state == STATE_RESIZE)
					{	
					// If the rect/circle was inverted, normalize
					CRect rc; GetRect(&rc);
					gdata.xx = rc.left; gdata.yy = rc.top;
					gdata.ww = rc.Width(); gdata.hh = rc.Height();
					state = STATE_IDLE;
					}
				break;

			case    MOUSE_RIGHTDOWN   : break;
			case    MOUSE_RIGHTUP     : break;
			case    MOUSE_RIGHTDBL    : break;
			case    MOUSE_LEFTDBL     : break;

			//////////////////////////////////////////////////////////////
			case    MOUSE_MOVE		  :
				//P2N("CLayer::MouseEvent MOVE x=%d y=%d off_x=%d off_y=%d\r\n", 
				//				point.x, point.y, dragoffx, dragoffx);

				if(state == STATE_ROTATE)
					{
					//P2N("CLayer::MouseEvent Rotate: %d %d %d\r\n", 
					//		gdata.xx - dragoffx, 
					//			point.x - (gdata.xx - dragoffx), 
					//				point.y - (gdata.yy - dragoffy));

					Rotate(point.x - (gdata.xx - dragoffx));
					}
				else if(state == STATE_RESIZE)
					{
					int rr = gdata.xx + gdata.ww;
					int bb = gdata.yy + gdata.hh;

					//P2N("CLayer::MouseEvent Resize %d %d\r\n", point.x, point.y);
									
					// Move one of the points
					switch(tracker.lasthit)
						{						
						case UL_HIT:
							gdata.yy = point.y;	
							gdata.xx = point.x;
							gdata.ww = rr - gdata.xx;
							gdata.hh = bb - gdata.yy;					
							break;
							
						case UR_HIT:
							gdata.ww = point.x - gdata.xx;				
							gdata.yy = point.y;
							gdata.hh = bb - point.y;
							break;
						
						case LR_HIT:
							gdata.ww = point.x - gdata.xx;
							gdata.hh = point.y - gdata.yy;
							break;

						case LL_HIT:
							gdata.xx = point.x;
							gdata.ww = rr - point.x;
							gdata.hh = point.y - gdata.yy;
							break;
						
						//////////////////////////////////////////////////

						case UM_HIT:
							gdata.yy = point.y;	
							gdata.hh = bb - gdata.yy;
							break;
				
						case RM_HIT:
							gdata.ww = point.x - gdata.xx;
							break;

						case BM_HIT:
							gdata.hh = point.y - gdata.yy;
							break;

						case LM_HIT:
							gdata.xx = point.x;	
							gdata.ww = rr - point.x;
							break;

						//////////////////////////////////////////////////

						case ULC_HIT:
						case LLC_HIT:		
						case URC_HIT:		
						case LRC_HIT:		
							state = STATE_ROTATE;
							break;
						}

					if(misc.IsShiftDown())
						{
						int www = min(gdata.hh, gdata.ww);
						gdata.ww = gdata.hh = www;
						}

					//Invalidate();

					ret = 0;
					}
				else if(state == STATE_MOVE || focus)
					{
					//gdata.xx = point.x + dragoffx;
					//gdata.yy = point.y + dragoffy;
					Move(point.x + dragoffx, point.y + dragoffy);
					ret = 0;
					}
				else if(state == STATE_INIT)
					{
					gdata.hh = point.y - gdata.yy;
					gdata.ww = point.x - gdata.xx;
					ret = 0;
					}
				break;

			default:
				break;
			}
		}	

	GetRect(&newrc); newrc.NormalizeRect();

	updaterect = oldrc | newrc;	
	updaterect.InflateRect(4, 4);

	Invalidate();

	in_mouse = false;

	return ret;
}

//////////////////////////////////////////////////////////////////////////

int	 CLayer::Prop(int dummy1, int dummy2, int dummy3)

{
	int ret = 0;

	CRect rc; GetRect(&rc);

	BRANCH_CODE(Prop, ret, 0, 0, 0);

	// Postprocess (if needed)
	if(ret < 0)
		{

		}

	return 0;
}

//////////////////////////////////////////////////////////////////////////

int	 CLayer::KeyEvent(UINT event, UINT nFlags, UINT nChar)

{
	int ret = 0, wantupd = 0;
	CRect oldrc; GetRect(&oldrc); oldrc.NormalizeRect();	
			
	BRANCH_CODE(KeyEvent, ret, event, nFlags, nChar)

	if(ret < 0)
		{
		switch(event)
			{
			case    KEYDOWN     :
			//P2N("CLayer::KeyEvent %d %d %d\r\n", event, nFlags, nChar);

			switch(nChar)
				{
				case 'A':
					P2N("CLayer::KeyDown A %d %d\r\n", nFlags, nChar);
					wantupd = true;
					break;

				case  VK_LEFT     :
					gdata.xx--;
					if(misc.IsShiftDown())
						gdata.xx -= 10;
					if(misc.IsCtrlDown())
						gdata.xx -= 50;

					wantupd = true;
					break;

				case  VK_RIGHT     :
					gdata.xx++;
					if(misc.IsShiftDown())
						gdata.xx += 10;
					if(misc.IsCtrlDown())
						gdata.xx += 50;

					wantupd = true;
					break;

				case  VK_PRIOR    :
					gdata.yy -= 10;
					wantupd = true;
					break;

				case  VK_NEXT    :
					gdata.yy += 10;
					wantupd = true;
					break;

				case  VK_UP    :
					gdata.yy--;
					if(misc.IsShiftDown())
						gdata.yy -= 10;
					if(misc.IsCtrlDown())
						gdata.yy -= 50;
					wantupd = true;
					break;

				case  VK_DOWN    :
					gdata.yy++;
					if(misc.IsShiftDown())
						gdata.yy += 10;
					if(misc.IsCtrlDown())
						gdata.yy += 50;
					wantupd = true;
					break;
				}
			break;

			case    KEYUP         :
				break;
			case    KEYCHAR       :
				break;
			case    SYSKEYDOWN    :
				break;
			case    SYSKEYUP      :
				break;
			case    SYSCHAR       :
				break;
			}
		}
	
	if(wantupd)
		{
		CRect newrc; GetRect(newrc); newrc.NormalizeRect();

		updaterect = oldrc | newrc;
		updaterect.InflateRect(10, 10);

		// For testing ...
		CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);
		CXrayDoc *doc = ((CShadesDlg* )AfxGetApp()->m_pMainWnd)->GetCurrDoc();

		if(doc)
			{
			//dlg->InvalidateRect(updaterect, false);
			//dlg->Invalidate(false);
			Invalidate();
			}
		}

	return ret;
}

void CLayer::InitVars()

{
	//P2N("CLayer::InitVars\r\n");

    magic = LAYER_MAGIC;
	state = STATE_IDLE;

	gl_factor = 1;

	in_mouse = 0;
	//in_move = 0;

	focus = false;
	dragoffx = dragoffy = 0;

	memset(&gdata, 0, sizeof(gdata));
	memset(&pdata, 0, sizeof(pdata));

	gdata.line_width	= 1;
	gdata.line_type		= PS_SOLID;
	
	gdata.brush_type    = BS_SOLID; 
	gdata.brush_hatch	= HS_DIAGCROSS;
	gdata.hatch_color	= RGB(255, 255, 255);

	//P2N("Unique %s\r\n", instance.GetStr());

	// Test Pack
	//void *ptr = instance.Pack();
	//instance.unPack(ptr);
	//instance.PackFree(ptr);

	oldrc.SetRectEmpty();
	newrc.SetRectEmpty();

	gdata.alpha = 150;

	in_draw = 0;
}

int		CLayer::Init(POINT pt, int dummy1, int dummy2)

{
	int ret = 0;
	P2N("CLayer::Init(POINT pt) %d %d\r\n", gdata.xx, gdata.yy);

	BRANCH_CODE(Init, ret, pt, dummy1, dummy2);

	if(ret < 0)
		{
		state = STATE_INIT;

		gdata.xx = pt.x;
		gdata.yy = pt.y;

		gdata.ww = 8;
		gdata.hh = 8;
		}

	return ret;
}

//////////////////////////////////////////////////////////////////////////
// Hit test on layer, ret true for hit

int CLayer::HitTest(POINT pt, int dummy1, int dummy2)

{
	//P2N("CLayer::Hittest(POINT pt) x=%d y=%d\r\n", pt.x, pt.y);
	
	int ret;
	BRANCH_CODE(HitTest, ret, pt, dummy1, dummy2);

	// Postprocess (if needed)
	if(ret < 0)
		{
		ret = 0; // make sure we say none first

		CRect rect(gdata.xx, gdata.yy, 
					gdata.xx + gdata.ww, gdata.yy + gdata.hh);

		rect.NormalizeRect();
		rect.InflateRect(gdata.line_width, gdata.line_width);

		if(rect.PtInRect(pt))
			ret = true;
		}

	// Always test tracker
	if(tracker.HitTest(pt)) // != WHOLE_HIT)	
		{
		ret = true;
		state = STATE_RESIZE;
		}
	return ret;
}

//////////////////////////////////////////////////////////////////////////

int	 CLayer::Invalidate(int dummy1, int dummy2, int dummy3)

{
	//P2N("CLayer::Invalidate\r\n");
	
	int ret;
	BRANCH_CODE(Invalidate, ret, 0, 0, 0);

	// Postprocess (if needed)
	if(ret >= 0)
		return 0;

	CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);
	CXrayDoc *doc = ((CShadesDlg* )AfxGetApp()->m_pMainWnd)->GetCurrDoc();

	if(!doc)
		return 0;

	CRect ur(updaterect); ur.NormalizeRect();
	ur.InflateRect(4 + gdata.line_width, 4 + gdata.line_width);
	dlg->InvalidateRect(ur, false);

	// Flicker helps see the real update
	//dlg->InvalidateRect(ur);

	return 0;
}

void CLayer::SetFocus(int foc)

{	
	if(focus == foc)
		return;

	//P2N("CLayer::SetFocus %p foc=%d focus=%d\r\n", this, foc, focus);

	CRect rect(gdata.xx, gdata.yy, 
			gdata.xx + gdata.ww, gdata.yy + gdata.hh);

	// Reset state if newly selected object
	if(foc && !focus)
		state = STATE_MOVE;

	int old = focus; focus = foc;

	gdata.rotate_org = gdata.rotate_angle;

	// For testing ...
	if(focus != old)
		Invalidate();
}

//////////////////////////////////////////////////////////////////////////

void CLayer::Resize(int wto, int hto)

{
	GetRect(&oldrc); 

	if(wto != -1)
		gdata.ww = wto;	
	
	if(hto != -1)
		gdata.hh = hto;

	GetRect(&newrc); newrc.NormalizeRect();
	
	updaterect = oldrc | newrc;
	updaterect.InflateRect(4, 4);

	Invalidate();
}

//////////////////////////////////////////////////////////////////////////
// Move layer to position. -1 indicates no move in that dimention.
//

void CLayer::Move(int xto, int yto)

{
	//P2N("CLayer::Move %p %s %d %d\r\n", this, GetKindStr(), xto, yto);
			
	GetRect(&oldrc); oldrc.NormalizeRect();

	if(xto != -1)
		gdata.xx = xto;	
	
	if(yto != -1)
		gdata.yy = yto;

	GetRect(&newrc); newrc.NormalizeRect();
	
	updaterect = oldrc | newrc;
	updaterect.InflateRect(4, 4);

	Invalidate();
}

//////////////////////////////////////////////////////////////////////////

char	*CLayer::Ma2Str(int mact)

{
	char *ret = "none";

	switch(mact)
		{                       	
		case MOUSE_LEFTDOWN	:	  ret =     "MOUSE_LEFTDOWN" ; break;  	
		case MOUSE_LEFTUP	:	  ret =     "MOUSE_LEFTUP"   ; break;  	
		case MOUSE_LEFTDBL	:	  ret =     "MOUSE_LEFTDBL"	 ; break;  	  
		case MOUSE_RIGHTDOWN:	  ret =     "MOUSE_RIGHTDOWN"; break;  	 	
		case MOUSE_RIGHTUP	:	  ret =     "MOUSE_RIGHTUP"	 ; break;  	  
		case MOUSE_RIGHTDBL	:	  ret =     "MOUSE_RIGHTDBL" ; break;  	  	
		case MOUSE_MOVE     :	  ret =     "MOUSE_MOVE"     ; break;  	
		}
   return ret;
}

//////////////////////////////////////////////////////////////////////////

void CLayer::DrawFocus(CDC *pDC, RECT *rc, int solid)

{
	CPen pen;
	
	if(solid)
		pen.CreatePen(PS_SOLID, 1, RGB(192, 192, 192)); 
	else
		pen.CreatePen(PS_DOT, 1, RGB(192, 192, 192)); 

	CPen *old_pen = pDC->SelectObject(&pen);

	COLORREF old_col = pDC->GetBkColor();
	pDC->SetBkColor(RGB(64, 64, 64));

	pDC->MoveTo(rc->left,  rc->top);
	
	pDC->LineTo(rc->right, rc->top);
	pDC->LineTo(rc->right, rc->bottom);
	pDC->LineTo(rc->left,  rc->bottom);
	pDC->LineTo(rc->left,  rc->top);

	pDC->SelectObject(old_pen);
	pDC->SetBkColor(old_col);
}

//////////////////////////////////////////////////////////////////////////

void CLayer::GetRect(RECT *rc)

{
	CRect rc_tmp; GetRect(&rc_tmp);

	rc->left	= rc_tmp.left;
	rc->top		= rc_tmp.top;
	rc->right	= rc_tmp.right;
	rc->bottom	= rc_tmp.bottom;
}

//////////////////////////////////////////////////////////////////////////

void CLayer::GetRect(CRect *rc)

{
	rc->left	= gdata.xx;
	rc->top		= gdata.yy;
	rc->right	= gdata.xx + gdata.ww;
	rc->bottom	= gdata.yy + gdata.hh;

	rc->NormalizeRect();
}

void CLayer::GetMemDC(CDC *pDC, CDC *dcMem)

{
	GetMemDC(pDC, dcMem, abs(gdata.ww), abs(gdata.hh));
}

//////////////////////////////////////////////////////////////////////////

void CLayer::GetMemDC(CDC *pDC, CDC *dcMem, int ww, int hh)

{
	//P2N("creating dc %dx%d\r\n", ww,hh);
	dcMem->CreateCompatibleDC(pDC);
	save = dcMem->SaveDC();

	// CreateCompatibleBitmap does not work on color/memory device!
	bm =::CreateCompatibleBitmap(pDC->GetSafeHdc(),ww, hh);
	oldbm = (HBITMAP)::SelectObject(*dcMem, bm);
}

//////////////////////////////////////////////////////////////////////////

void CLayer::FreeMemDC(CDC *pDC, CDC *dcMem)

{
	::SelectObject(*dcMem, oldbm);  ::DeleteObject(bm);
	dcMem->RestoreDC(save);
	dcMem->DeleteDC();
}

//////////////////////////////////////////////////////////////////////////

void CLayer::PutMemDC(CDC *pDC, CDC *dcMem)

{
	CRect rc2; GetRect(&rc2); PutMemDC(pDC, dcMem, &rc2);
}

void CLayer::PutMemDC(CDC *pDC, CDC *dcMem, CRect *rc2)

{
	if(gdata.alpha != 255)
		{
		// Alpha blend
		BLENDFUNCTION m_bf;

		m_bf.BlendOp = AC_SRC_OVER;
		m_bf.BlendFlags = 0;
		m_bf.SourceConstantAlpha = gdata.alpha;
		m_bf.AlphaFormat = 0;

		AlphaBlend(pDC->GetSafeHdc(),
			(int)(rc2->left * gl_factor), 
				(int)(rc2->top * gl_factor),
					(int)(rc2->Width() * gl_factor), 
						(int)(rc2->Height() * gl_factor),
							*dcMem, 0,0, 
								rc2->Width(), rc2->Height(), m_bf);
		}
	else
		{
		::BitBlt(pDC->GetSafeHdc(),
			(int)(rc2->left * gl_factor), 
				(int)(rc2->top * gl_factor),
					(int)(rc2->Width() * gl_factor), 
						(int)(rc2->Height() * gl_factor),
							*dcMem, 0,0, SRCCOPY);
		}
}



void	CLayer::DrawTrackerX(CDC *pDC, CRect *ur, int circ)

{
	if(abs(gdata.ww) < 20)
		DrawTracker(pDC, ur, 1, circ);
	else if(abs(gdata.ww)< 30)
		DrawTracker(pDC, ur, 2, circ);
	else
		DrawTracker(pDC, ur, 4, circ);
}

//////////////////////////////////////////////////////////////////////////

void	CLayer::DrawTracker(CDC *pDC, CRect *rc, int twidth, int circ)

{
	tracker.Draw(pDC, rc, twidth, circ);
}

void CLayer::Rotate(int deg)

{
	//P2N("CLayer::Rotate %d\r\n", deg);

	gdata.rotate_angle = gdata.rotate_org + deg;
	Invalidate();
}
