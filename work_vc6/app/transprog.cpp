
/* =====[ transprog.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the transprog.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  5/15/2007  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

//////////////////////////////////////////////////////////////////////////////
// Transparent Slider: Implementation file
//

#include "stdafx.h"
#include "mxpad.h"
#include "transprog.h"
#include "windows.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTransProg

CTransProg::CTransProg()
{
	m_dcBk = NULL;
	m_crThumbColor = NULL;
	m_crThumbColorSelected = NULL;
	//m_bRedraw = false;
}

CTransProg::~CTransProg()
{	
	::SelectObject(m_dcBk, m_bmpBkOld);
	::DeleteObject(m_bmpBk);
	::DeleteDC(m_dcBk);			
}

BEGIN_MESSAGE_MAP(CTransProg, CProgressCtrl)
	//{{AFX_MSG_MAP(CTransProg)
		ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
		ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTransProg message handlers
//-------------------------------------------------------------------
//
void CTransProg::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult) 
//
// Description	:	Sent by the slider control to notify the parent window 
//					about drawing operations. This notification is sent in 
//					the form of a WM_NOTIFY message.
// Parameters	:	pNMHDR - a pointer to a NM_CUSTOMDRAW structure.
//					pResult - value depends on the current drawing state.
{
	//NMCUSTOMDRAW

	P2N("Custom draw transparent progress\r\n");

	LPNMCUSTOMDRAW lpcd = (LPNMCUSTOMDRAW)pNMHDR;
	CDC *pDC = CDC::FromHandle(lpcd->hdc);
	DWORD dwStyle = this->GetStyle();
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
			break;
			}
		}
}

void CTransProg::DrawChannel(CDC *pDC, LPNMCUSTOMDRAW lpcd)

{
	CClientDC clientDC(GetParent());

	CRect crect; GetClientRect(crect);
	CRect wrect; GetWindowRect(wrect);
		
	GetParent()->ScreenToClient(wrect);

	//if(lpcd->uItemState == CDIS_FOCUS)

	int focus = 0;

	if(GetFocus() == this)
		{
		focus = true;
		}

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


void CTransProg::DrawThumb(CDC *pDC, LPNMCUSTOMDRAW lpcd)

{
#if 0
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

#endif

}

BOOL CTransProg::OnEraseBkgnd(CDC* pDC)

{
	//CSliderCtrl::OnEraseBkgnd(pDC);
	return FALSE;
}


void CTransProg::SetThumbColors(COLORREF face, COLORREF highlight)

{
	m_crThumbColor = face;
	m_crThumbColorSelected = highlight;
}

void CTransProg::DrawTransparent(BOOL bRepaint)

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

void CTransProg::DrawFocus(CDC *pDC, RECT *r, DWORD color, int border)

{
	CPen pen;

	if (pen.CreatePen(PS_DOT, 1,  color))
		{
		CPen* pOldPen = pDC->SelectObject(&pen);

		// Upper
		pDC->MoveTo(border/2, border);
		pDC->LineTo(r->right - border/2, border);

		// Side1
		//pDC->LineTo(r->right - (offset+border), r->bottom  - (offset+border));
		pDC->MoveTo(r->right - border/2, r->bottom  - (border));

		// Lower
		pDC->LineTo(border/2, r->bottom  - (border));

		// Side2
		//pDC->LineTo(offset+border, offset+border);
		}
}
