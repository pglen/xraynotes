
/* =====[ LayText.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the LayText.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  11/28/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// LayText.cpp: implementation of the CLayText class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "xraynotes.h"
#include "LayText.h"

#include "textprop.h"
#include "TextMode.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLayText::CLayText()

{
	//P2N("CLayText::CLayText()\r\n");

	gdata.kind = LAYER_TEXT;

	cust_font.GetLogFont(&text_logfont);

	// Own a copy of the font
	text_font.CreateFontIndirect(&text_logfont);

	tiptext = "Type Screen Tip Text here";
	text = "Double Click or Start Typing Letters to Edit Text ...";

	txdata.text_paddingy = txdata.text_paddingx = 4; 
	txdata.text_align = txdata.text_3d = 0;
	txdata.text_ontop = 0;
	txdata.text_round	= 0;

	gdata.alpha = 128;
	gdata.bg_color	= RGB(255, 255, 128);
	gdata.fg_color	= RGB(0, 0, 0);

	text_cursor = 0;
	
}

CLayText::~CLayText()

{
	//P2N("CLayText::~CLayText\r\n");
}

//////////////////////////////////////////////////////////////////////////

int CLayText::Init(POINT pt, int dummy1, int dummy2)

{
	gdata.xx = pt.x;
	gdata.yy = pt.y;

	//P2N("CLayText::Init(POINT pt) %p %d %d\r\n", this, gdata.xx, gdata.yy);
	
	Render_Text(1);

	return 0;
}

int CLayText::Draw(CDC *pDC, float factor, int dummy1)

{
	//P2N("CLayText::Draw %d %d %f %s\r\n", gdata.xx, gdata.yy, factor, text);

	ASSERT(gdata.kind == LAYER_TEXT);

	CTextMode tm(pDC);

	RECT oldbound;	pDC->GetClipBox(&oldbound);

	CDC  dcMem;	GetMemDC(pDC, &dcMem);
	{
	CSaveDC sdc(pDC);

	// Fill it in
	CRect rc(0, 0, gdata.ww, gdata.hh);
	dcMem.FillSolidRect(rc, gdata.bg_color);

	// Mutually exclude
	if(txdata.text_3d)
		{
		dcMem.DrawEdge(rc, EDGE_RAISED, BF_RECT );
		}
	else if(txdata.text_round)
		{
		// Set new clipping 
		
		//P2N("oldbound t=%d l=%d b=%d r=%d\r\n", oldbound.top, oldbound.left,
		//	oldbound.bottom, oldbound.right);

		pDC->BeginPath();
		pDC->RoundRect(gdata.xx, gdata.yy, 
						gdata.xx + gdata.ww, gdata.yy+gdata.hh,
						  txdata.text_round, txdata.text_round);
		pDC->EndPath();
		
		pDC->SelectClipPath(RGN_AND);
		}

	PutMemDC(pDC, &dcMem);
	FreeMemDC(pDC, &dcMem);

	CFont *oldfont = pDC->SelectObject(&text_font);

	int oldbkmode = pDC->GetBkMode();
	pDC->SetBkMode(TRANSPARENT);

	// Correct text color if matches exact transparency
	//if(gdata.bg_color == RGB(255, 255, 255))
	//	gdata.bg_color = RGB(254, 254, 254);

	COLORREF oldcol = pDC->SetTextColor(gdata.fg_color);

	CRect txrc( (int)(gdata.xx * factor),
				(int)(gdata.yy * factor),
				(int)((gdata.xx + gdata.ww) * factor),
				(int)((gdata.yy + gdata.hh) * factor)
				);

	txrc.InflateRect(-txdata.text_paddingx, -txdata.text_paddingy);
	//txrc.InflateRect(text_padding, text_padding);

	if(txdata.text_ontop)
		pDC->DrawText(text, txrc, DT_LEFT | DT_EXPANDTABS |DT_WORDBREAK );
	else
		pDC->DrawText(text, txrc, DT_LEFT | DT_NOPREFIX | DT_EXPANDTABS  | DT_WORDBREAK   );

	// Bug: Select clip path will accumulte instead of clear

	// Mutually exclude
	if(txdata.text_3d)
		{
		}
	else if(txdata.text_round)
		{
		// Restore clipping 
//		pDC->BeginPath();
//		pDC->Rectangle(oldbound.left, oldbound.top,
//						oldbound.right, oldbound.bottom);
//		pDC->EndPath();		
//
//		pDC->SelectClipPath(RGN_COPY);
//		pDC->AbortPath();

		CRgn rgn; rgn.CreateRectRgn(oldbound.left, oldbound.top,
						oldbound.right, oldbound.bottom);
		pDC->SelectClipRgn(&rgn);
		}

	//pDC->SetBkMode(oldbkmode);

	if(focus)
		{
		CRect rect(gdata.xx, gdata.yy, 
			gdata.xx+gdata.ww, gdata.yy+gdata.hh);

		rect.InflateRect(2,2);
		DrawFocus(pDC, &rect);
		//pDC->DrawFocusRect(&rect);
		}

	//pDC->SetTextColor(oldcol);

	// Free Resources
	pDC->SelectObject( &oldfont);
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////

void CLayText::Render_Text(float factor)

{
	CShadesDlg  *dlg = GMD(dlg);

	CClientDC dc(dlg); 

	gdata.hh = 0; gdata.ww = 0;

	if(text.GetLength() == 0)
		{
		gdata.ww += 2 * txdata.text_paddingx;
		gdata.hh += 2 * txdata.text_paddingy;
		return;
		}

	CSize sst, sst2;
	CString tmp;

	text_font.GetLogFont(&text_logfont);

	//P2N("Font size=%d ",		text_logfont.lfHeight  );
	//P2N("Font italic=%d ",		text_logfont.lfItalic );
	//P2N("Font weight=%d ",		text_logfont.lfWeight );
	//P2N("Font facename=%s\r\n",	text_logfont.lfFaceName );

	// In case of empty line, we need the height
	CFont *oldfont = dc.SelectObject( &text_font );

	sst2 = dc.GetTextExtent("a");
	int  oneheight = sst2.cy;

	CString text2 = text.Left(text_cursor);

	int pos = 0, start = 0, count = 0;

	text_lines = 0;

	// Draw the background box to this size
	while(true)
		{
		pos = text2.Find("\r", pos);
		if(pos < 0)
			{
			tmp = text2.Mid(start, text.GetLength() - start);
			sst2 = dc.GetTextExtent(tmp);
			gdata.ww =  MAX(sst2.cx, gdata.ww);
			oneheight = MAX(oneheight, sst2.cy);
			if(!sst2.cy)
				sst2.cy = oneheight;

			gdata.hh += sst2.cy;

			//P2N("ww=%d hh=%d - text2='%s'\r\n", sst2.cx, sst2.cy, tmp);
			break;
			}
		text_lines++;

		// Exclude new line
		tmp = text2.Mid(start, (pos - start));

		pos++; start = pos;
		sst = dc.GetTextExtent(tmp);
		oneheight = MAX(oneheight, sst.cy);
		gdata.ww =  MAX(sst.cx, gdata.ww);

		if(!sst.cy)
			sst.cy = oneheight;

		gdata.hh += sst.cy;
		//P2N("ww=%d hh=%d - text1='%s'\r\n", sst.cx, sst.cy, tmp);
		}

	RECT	rc;
	rc.left = rc.top = rc.right = rc.bottom = 0;
	dc.DrawText(text, &rc, DT_LEFT | DT_NOPREFIX | DT_CALCRECT );

	dc.SelectObject( oldfont );

	gdata.ww = rc.right;
	gdata.hh = rc.bottom;

	gdata.ww += 2 * txdata.text_paddingx;
	gdata.hh += 2 * txdata.text_paddingy;

	// Hack-o-matic -- Added 4 if italic
	if(text_logfont.lfItalic)
		{
		gdata.ww += 4;
		//hh+=4;
		}

	//P2N("CLayText::Render_Text xx=%d yy=%d right=%d bottom=%d\r\n", 
	//		gdata.xx, gdata.yy, rc.right, rc.bottom);

	GetRect(&updaterect);
}

//////////////////////////////////////////////////////////////////////////

int CLayText::MouseEvent(UINT event, UINT nFlags, CPoint point)

{
	int ret = -1;

	//P2N("CLayText::MouseEvent event=%d x=%d y=%d\r\n", event, point.x, point.y);		

	switch(event)
		{
		case MOUSE_MOVE:
			//P2N("CLayText::MOUSE_MOVE x=%d y=%d\r\n", point.x, point.y);		
			ret = -1;
			break;

		//case    MOUSE_LEFTDOWN   : 	
			//
			//	state = STATE_MOVE;			
			//	dragoffx = gdata.xx - point.x; 
			//	dragoffy = gdata.yy - point.y; 
			//
			//	//P2N("CLayText::MOUSE_LEFTDOWN x=%d y=%d\r\n", point.x, point.y);
			//	ret = 0;
			//	break;

		case	MOUSE_LEFTDBL:
			{
			//P2N("CLayText::MOUSE_LEFTDBL x=%d y=%d\r\n", point.x, point.y);		

			CShadesDlg *dlg; GMD(dlg);  CXrayDoc *doc; GCD(doc, dlg);
			if(doc && doc->m_currlay)
				{
				CTextProp tp; tp.lay = (CLayText*)doc->m_currlay;
				tp.DoModal();
				}
			}
			ret = 0;
			break;

		case    MOUSE_RIGHTDOWN   :
			{
			//P2N("CLayText::MOUSE_RIGHTDOWN x=%d y=%d\r\n", point.x, point.y);		

			CShadesDlg *dlg; GMD(dlg);  CXrayDoc *doc; GCD(doc, dlg);
			if(doc && doc->m_currlay)
				{			
				// pop the RClick menu
				CXrayM *omenu = new CXrayM; ASSERT(omenu);
				omenu->LoadMenu(IDR_MENU15, 1);

				omenu->SetItemText(0, "Text Object");
				// Disable multi items
				if(dlg->helper->GetNumSelObj() < 2)
					{	
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

	return ret;
}

//////////////////////////////////////////////////////////////////////////


void CLayText::SetNewFont()

{
	CRect oldrc(gdata.xx, gdata.yy,
			gdata.xx+gdata.ww, gdata.yy+gdata.hh);

	text_font.Detach();
	text_font.CreateFontIndirect(&text_logfont);
	
	Render_Text(1);

	CRect newrc(gdata.xx, gdata.yy,  
		gdata.xx+gdata.ww, gdata.yy+gdata.hh);

	oldrc.NormalizeRect();
	newrc.NormalizeRect();

	updaterect = oldrc | newrc;
	updaterect.InflateRect(10, 10);
}


//////////////////////////////////////////////////////////////////////////

void CLayText::SetNewText(const char *str)

{
	CRect oldrc(gdata.xx, gdata.yy,
			gdata.xx+gdata.ww, gdata.yy+gdata.hh);

	text = str;
	Render_Text(1);

	CRect newrc(gdata.xx, gdata.yy,  
		gdata.xx+gdata.ww, gdata.yy+gdata.hh);

	oldrc.NormalizeRect();
	newrc.NormalizeRect();

	updaterect = oldrc | newrc;
	updaterect.InflateRect(10, 10);
}

//////////////////////////////////////////////////////////////////////////

void CLayText::SetNewPadding(int padx, int pady)

{
	CRect oldrc(gdata.xx, gdata.yy,
			gdata.xx+gdata.ww, gdata.yy+gdata.hh);

	txdata.text_paddingx = padx;
	txdata.text_paddingy = pady;

	Render_Text(1);

	CRect newrc(gdata.xx, gdata.yy,  
		gdata.xx+gdata.ww, gdata.yy+gdata.hh);

	oldrc.NormalizeRect();
	newrc.NormalizeRect();

	updaterect = oldrc | newrc;
	updaterect.InflateRect(10, 10);
}

//////////////////////////////////////////////////////////////////////////

int CLayText::KeyEvent(UINT event, UINT nFlags, UINT nChar)

{
	int ret = -1;

	//P2N("CLayText::KeyEvent %d %d %d\r\n", event, nFlags, nChar);

	 switch(event)
        {
        case    KEYDOWN     :

			if(nChar >= 'A' && nChar <= 'Z')
				{
				//P2N("CLayText::KeyDown A-Z\r\n");

				CTextProp tp; tp.lay = this;
				tp.DoModal();
				ret = 0;
				}

        case    KEYUP       :
        case    KEYCHAR     :

        case    SYSKEYDOWN    :
        case    SYSKEYUP      :
		case    SYSCHAR     :
            break;
        }

	 return ret;
}

int	 CLayText::Prop(int dummy1, int dummy2, int dummy3)

{
	CTextProp tp;	

	tp.lay = this;
	tp.DoModal();

	return 0;
}