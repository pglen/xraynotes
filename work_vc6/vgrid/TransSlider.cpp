
/* =====[ TransSlider.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the TransSlider.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  12/11/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// Transparent Slider: Implementation file
//

#include "stdafx.h"
#include "vgrid.h"
#include "transslider.h"
#include "windows.h"
#include "misc.h"
#include "mxpad.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTransSlider

CTransSlider::CTransSlider()
{
	oldpos = 0;

	m_dcBk = NULL;
	m_crThumbColor = NULL;
	m_crThumbColorSelected = NULL;

	message = WM_USER + 10;
}

CTransSlider::~CTransSlider()
{	
	::SelectObject(m_dcBk, m_bmpBkOld);
	::DeleteObject(m_bmpBk);
	::DeleteDC(m_dcBk);			
}

BEGIN_MESSAGE_MAP(CTransSlider, CSliderCtrl)
	//{{AFX_MSG_MAP(CTransSlider)
		ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
		ON_WM_ERASEBKGND()
	ON_WM_MOUSEWHEEL()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYUP()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTransSlider message handlers
//-------------------------------------------------------------------
//
void CTransSlider::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult) 
//
// Description	:	Sent by the slider control to notify the parent window 
//					about drawing operations. This notification is sent in 
//					the form of a WM_NOTIFY message.
// Parameters	:	pNMHDR - a pointer to a NM_CUSTOMDRAW structure.
//					pResult - value depends on the current drawing state.
{
	//NMCUSTOMDRAW

	//P2N("CTransSlider::OnCustomDraw\r\n");

	LPNMCUSTOMDRAW lpcd = (LPNMCUSTOMDRAW)pNMHDR;

	CDC *pDC = CDC::FromHandle(lpcd->hdc);
	DWORD dwStyle = this->GetStyle();

	// Disable custom draw
	*pResult = CDRF_DODEFAULT;
	return;


	switch(lpcd->dwDrawStage)
		{
		case CDDS_PREPAINT:
			*pResult = CDRF_NOTIFYITEMDRAW;
		break;
		
		case CDDS_ITEMPREPAINT:
			{
			switch(lpcd->dwItemSpec)
				{
				case TBCD_TICS:
					*pResult = CDRF_DODEFAULT;
					break;

				case TBCD_THUMB:
					//DrawThumb(pDC, lpcd);
					//*pResult = CDRF_SKIPDEFAULT;
					*pResult = CDRF_DODEFAULT;
				break;

				case TBCD_CHANNEL:
					DrawChannel(pDC, lpcd);
					*pResult = CDRF_SKIPDEFAULT;
					//*pResult = CDRF_DODEFAULT;
					break;
				}
			default:
				*pResult = CDRF_DODEFAULT;
				break;

			break;
			}
		}
}

void CTransSlider::DrawChannel(CDC *pDC, LPNMCUSTOMDRAW lpcd)

{
	CClientDC clientDC(GetParent());

	CRect crect; GetClientRect(crect);
	CRect wrect; GetWindowRect(wrect);
		
	GetParent()->ScreenToClient(wrect);

	//if(lpcd->uItemState == CDIS_FOCUS)

	int focus = 0;

	if(GetFocus() == this)
		focus = true;
	else
		focus = false;


	//P2N("Drawchannel -- Focus=%d \r\n", focus);

	//P2N("DrawChannel p=%p l=%d t=%d\r\n", this, wrect.left, wrect.top);

	if (m_dcBk == NULL)
		{
		m_dcBk = CreateCompatibleDC(clientDC.m_hDC);
		m_bmpBk = CreateCompatibleBitmap(clientDC.m_hDC, crect.Width(), crect.Height());
		m_bmpBkOld = (HBITMAP)::SelectObject(m_dcBk, m_bmpBk);
		::BitBlt(m_dcBk, 0, 0, crect.Width(), crect.Height(), clientDC.m_hDC, wrect.left, wrect.top, SRCCOPY);
		}

	// This bit does the tics marks transparently.
	// create a memory dc to hold a copy of the oldbitmap data that includes the tics,
	// because when we add the background in we will lose the tic marks
	
	HDC hSaveHDC;
	HBITMAP hSaveBmp;

	int iWidth = crect.Width();
	int iHeight = crect.Height();
	
	hSaveHDC = ::CreateCompatibleDC(pDC->m_hDC);
	hSaveBmp = ::CreateCompatibleBitmap(hSaveHDC, iWidth, iHeight);
	HBITMAP hSaveCBmpOld = (HBITMAP)::SelectObject(hSaveHDC, hSaveBmp);			
	
	// Set the colours for the monochrome mask bitmap
	COLORREF crOldBack = ::SetBkColor(pDC->m_hDC, RGB(0,0,0));
	COLORREF crOldText = ::SetTextColor(pDC->m_hDC, RGB(255,255,255));		

	::BitBlt(hSaveHDC, 0, 0, iWidth, iHeight, pDC->m_hDC, crect.left, crect.top, SRCCOPY);
	::BitBlt(pDC->m_hDC, 0, 0, iWidth, iHeight, m_dcBk, 0, 0, SRCCOPY);
	::BitBlt(pDC->m_hDC, 0, 0, iWidth, iHeight, hSaveHDC, 0, 0, SRCAND);
	
	// Draw focus net
	if(focus)
		{
		//pDC->TextOut(10, 10, "Hello");
		//DrawFocus(pDC, &crect, GetSysColor(COLOR_3DDKSHADOW), 4, 0, 0, 8);
		DrawFocus(pDC, &crect, GetSysColor(COLOR_3DDKSHADOW), 14);
		//DrawFocus(pDC, &lpcd->rc, GetSysColor(COLOR_3DDKSHADOW), 14);
		
		}

	// Restore and clean up
	::SetBkColor(pDC->m_hDC, crOldBack);
	::SetTextColor(pDC->m_hDC, crOldText);
	::SelectObject(hSaveHDC, hSaveCBmpOld);
	::DeleteObject(hSaveBmp);
	::DeleteDC(hSaveHDC);

	crect = lpcd->rc;
	
	if ((crect.bottom - crect.top) > (crect.right - crect.left))
		crect.InflateRect(1, 0, 1, 0);
	else								//is there a better way to know vert from horiz sliders??
		crect.InflateRect(0, 2, 0, 2);

	DrawEdge(pDC->m_hDC, &crect, EDGE_SUNKEN, BF_RECT | BF_ADJUST);
	
}

void CTransSlider::DrawThumb(CDC *pDC, LPNMCUSTOMDRAW lpcd)

{
	CRect crect;
	GetThumbRect(&crect);
	COLORREF col;
	
	if (lpcd->uItemState & CDIS_SELECTED)
		col = m_crThumbColorSelected;
	else
		col = m_crThumbColor;

	if (col == NULL && lpcd->uItemState & CDIS_SELECTED)
		col = GetSysColor(COLOR_3DHIGHLIGHT);
	else if (col == NULL && !(lpcd->uItemState & CDIS_SELECTED))
		col = GetSysColor(COLOR_3DFACE);

	HBRUSH hbrush = CreateSolidBrush(col);
    HBRUSH hbOld = (HBRUSH)SelectObject(pDC->m_hDC, hbrush);
    
	Ellipse(pDC->m_hDC, crect.left, crect.top, crect.right, crect.bottom);
    SelectObject(pDC->m_hDC, hbOld);
    DeleteObject(hbrush);
}

BOOL CTransSlider::OnEraseBkgnd(CDC* pDC)

{
	int ret = CSliderCtrl::OnEraseBkgnd(pDC);
	return ret;
}


void CTransSlider::SetThumbColors(COLORREF face, COLORREF highlight)

{
	m_crThumbColor = face;
	m_crThumbColorSelected = highlight;
}

void CTransSlider::DrawTransparent(BOOL bRepaint)

{
	if (m_dcBk != NULL && m_bmpBkOld != NULL)
		{
		::SelectObject(m_dcBk, m_bmpBkOld);
		}
	
	::DeleteObject(m_bmpBk);
	::DeleteDC(m_dcBk);
	
	m_dcBk = NULL;
	m_bmpBk = NULL;

	if (bRepaint == TRUE) 
		{
		Invalidate();
		EnableWindow(FALSE);
		EnableWindow(TRUE);
		}
} 

/////////////////////////////////////////////////////////////////////////

void CTransSlider::DrawFocus(CDC *pDC, RECT *r, DWORD color, int border)

{
	CPen pen;

	DrawEdge(pDC->m_hDC, r, EDGE_BUMP, BF_RECT | BF_ADJUST);

#if 0

	int offset = 2;

	if (pen.CreatePen(PS_DOT, 1,  RGB(100,100,100) ))
		{
		CPen* pOldPen = pDC->SelectObject(&pen);

		// Upper
		//pDC->MoveTo(border/2, border);
		//pDC->LineTo(r->right - border/2, border);

		// Side1
		pDC->MoveTo(r->right - border, r->top + border);
		pDC->LineTo(r->right - border,  r->bottom - border);
		
		// Lower
		//pDC->LineTo(border/2, r->bottom  - (border));

		// Side2
		//pDC->MoveTo(r->left + border/2,  r->top + border );
		//pDC->LineTo(r->left + border/2,  r->bottom - border);

		pDC->SelectObject(pOldPen);
		}

#endif

}


BOOL CTransSlider::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 

{
	P2N("CTransSlider::OnMouseWheel %d\r\n", zDelta);

	int mm, xx;
	GetRange( mm, xx ) ;
	int pp = GetPos();

	if(zDelta > 0)
		{
		SetPos(pp - (xx-mm) / 100);
		}
	else
		{
		SetPos(pp + (xx-mm) / 100);
		}

	GetParent()->SendMessage(WM_VSCROLL, (pp << 16) + 8, (long)this->m_hWnd);

	return CSliderCtrl::OnMouseWheel(nFlags, zDelta, pt);
}

void CTransSlider::OnPaint() 
{
	//P2N("CTransSlider::OnPaint\r\n");

	CSliderCtrl::OnPaint();

	//CPaintDC dc(this); // device context for painting

	// Draw a focus rect around the control
	CClientDC dc(this);
	CRect rc; GetClientRect(&rc);
	//rc.DeflateRect(4,4);

	if(GetFocus() == this)
		{
		//P2N("Focus %p\r\n", this);
		DrawEdge(dc, &rc, EDGE_BUMP, BF_RECT | BF_ADJUST);
		}
}

void CTransSlider::OnMouseMove(UINT nFlags, CPoint point) 

{
	//P2N("CTransSlider::OnMouseMove\r\n");
	int newpos = GetPos();

	if(oldpos != newpos)
		{
		oldpos = newpos;
		OnScroll(SCROLL_TRACK, newpos);
		}
	
	CSliderCtrl::OnMouseMove(nFlags, point);
}

void CTransSlider::OnLButtonUp(UINT nFlags, CPoint point) 

{
	OnScroll(SCROLL_END, GetPos());
	CSliderCtrl::OnLButtonUp(nFlags, point);
}


void CTransSlider::OnLButtonDown(UINT nFlags, CPoint point) 

{
	OnScroll(SCROLL_START, GetPos());
	CSliderCtrl::OnLButtonDown(nFlags, point);
}

void CTransSlider::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 

{
	int newpos = GetPos();

	OnScroll(SCROLL_TRACK, newpos);
	OnScroll(SCROLL_END, newpos);
	CSliderCtrl::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CTransSlider::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 

{
	OnScroll(SCROLL_START, GetPos());	

	P2N("CSliderCtrl::OnKeyDown %d\r\n", GetPos());

	if(nChar == VK_RIGHT)
		{
		if(misc.IsCtrlDown())
			{
			SetPos(GetPos() + GetRangeMax() / 10);
			}
		}
	if(nChar == VK_LEFT)
		{
		if(misc.IsCtrlDown())
			{
			SetPos(GetPos() - GetRangeMax() / 10);
			}
		}

	CSliderCtrl::OnKeyDown(nChar, nRepCnt, nFlags);

}


void CTransSlider::OnScroll(int opcode, int pos)

{
	//P2N("CTransSlider::OnScroll %d %d\r\n", opcode, pos);
	GetParent()->PostMessage(message, GetDlgCtrlID(), MAKELPARAM(pos, opcode));
}
