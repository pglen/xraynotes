
/* =====[ GrayWin.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the GrayWin.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  10/29/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// GrayWin.cpp: implementation of the CGrayDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "xraynotes.h"
#include "GrayDlg.h"
#include "mxpad.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGrayDlg::CGrayDlg(CWnd* pParent /*=NULL*/)
	: PersistDlg(CGrayDlg::IDD, pParent)
{
	InitVars();
}

CGrayDlg::CGrayDlg(UINT nIDTemplate, CWnd* pParentWnd)
	: PersistDlg(nIDTemplate, pParentWnd)

{
	InitVars();
}

CGrayDlg::~CGrayDlg()

{
	//if(!destroyed)
	//	P2N("CGrayDlg::~CGrayDlg() Not destroyed! %p %s\r\n", this, m_title);

	//P2N("CGrayDlg::~CGrayDlg() %p %s\r\n", this, m_title);
}

BEGIN_MESSAGE_MAP(CGrayDlg, PersistDlg)
	//{{AFX_MSG_MAP(CGrayDlg)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_DESTROY()
	ON_WM_SIZE()	
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_ACTIVATE()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CGrayDlg::OnDestroy()

{
	destroyed = true;
	//P2N("CGrayDlg::OnDestroy() IDD=%d %s\r\n", xidd, windowname);
	PersistDlg::OnDestroy();
}

BOOL CGrayDlg::OnInitDialog() 

{
	PersistDlg::OnInitDialog();
	return true;
}

void CGrayDlg::OnPaint() 

{
	CPaintDC dc(this); // device context for painting
	
	RECT rc; GetClientRect(&rc);

	// Draw header
	if(!noheader)
		{
		CDC dcMem; dcMem.CreateCompatibleDC(&dc);

		int save = dcMem.SaveDC();

		HBITMAP bm =::CreateCompatibleBitmap(dc.GetSafeHdc(),
				titlerect.Width(), titlerect.Height());
		HBITMAP old = (HBITMAP) SelectObject(dcMem, bm);

		CRect rc(0,0, titlerect.Width(), titlerect.Height()); 
	
		if(GetActiveWindow() == this)		
			dcMem.FillSolidRect(&rc, dheadcol);
		else
			dcMem.FillSolidRect(&rc, headcol);

		// Alpha blend header
		BLENDFUNCTION m_bf; INITBLEND(m_bf, 150);

		AlphaBlend(dc.GetSafeHdc(),
			titlerect.left, titlerect.top,
				titlerect.Width(), titlerect.Height(),
					dcMem, 0, 0, 
						titlerect.Width(), titlerect.Height(), m_bf);

		//dc.SetTextColor(m_crText);
		dc.SetBkMode(TRANSPARENT);

		// Draw Header
		CRect rcc(titlerect); rcc.top += 1; rcc.left += 2;
		
		CFont* oldFont = dc.SelectObject(&m_font);
		dc.DrawText(m_title, m_title.GetLength(), rcc, DT_LEFT);
		dc.SelectObject(oldFont);

		dcMem.RestoreDC(save);

		SelectObject(dcMem, old);

		DeleteObject(bm);
		}

	if(!noclosebox)
		{
		// Draw corner close	
		dc.FillSolidRect(closerect, RGB(32, 32, 32));

		CPen pen, *oldpen;
		pen.CreatePen(PS_SOLID, 2, RGB(255,255,255));
		oldpen = (CPen*)dc.SelectObject(pen);

		// Draw cross
		dc.MoveTo( closerect.left + 2, closerect.top + 2);
		dc.LineTo( closerect.right - 2, closerect.bottom -2);

		dc.MoveTo( closerect.left + 2, closerect.bottom - 2);
		dc.LineTo( closerect.right - 2, closerect.top + 2);
		}

	PersistDlg::OnPaint();

	// Do not call CGrayDlg::OnPaint() for painting messages
}

void CGrayDlg::OnLButtonDown(UINT nFlags, CPoint point) 

{
	RECT rc; GetClientRect(&rc);
	
	if(titlerect2.PtInRect(point) && !closerect2.PtInRect(point))
		{
		//P2N("CGrayDlg::OnLButtonDown Click in Header box\r\n");
		PersistDlg::OnLButtonDown(nFlags, point);
		PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));	
		return;
		}

	PersistDlg::OnLButtonDown(nFlags, point);
}

void CGrayDlg::OnLButtonUp(UINT nFlags, CPoint point) 

{
	RECT rc; GetClientRect(&rc);
	
	//P2N("CGrayDlg::OnLButtonUp x=%d y=%d\r\n", point.x, point.y);	

	if(closerect2.PtInRect(point))
		{
		//P2N("CGrayDlg::OnLButtonUp - Click in Close Box\r\n");	
		PersistDlg::OnLButtonUp(nFlags, point);
		//support.YieldToWinEx();
		Hide(IDCANCEL);
		return;
		}
	PersistDlg::OnLButtonUp(nFlags, point);
}


void CGrayDlg::OnSize(UINT nType, int cx, int cy) 

{
	PersistDlg::OnSize(nType, cx, cy);
	
	// Evaluate head/closerect
	RECT	rc3; GetClientRect(&rc3);

	closerect.top = rc3.top + 2;
	closerect.left = rc3.right - (titleheight + 2);
	closerect.bottom = closerect.top + titleheight;
	closerect.right = closerect.left + titleheight;

	// Hittest with generocity
	closerect2 = closerect; closerect2.InflateRect(2, 2);

	titlerect.top = 2;
	titlerect.left = 3;
	titlerect.bottom = titlerect.top + (titleheight + 1);
	titlerect.right = rc3.right - 2;	

	// Hittest with generocity
	titlerect2 = titlerect;	titlerect2.InflateRect(2,2);
}

void CGrayDlg::OnSetFocus(CWnd* pOldWnd) 

{
	//P2N("CGrayDlg::OnSetFocus\r\n");
	PersistDlg::OnSetFocus(pOldWnd);
}

void CGrayDlg::OnKillFocus(CWnd* pNewWnd) 

{
	//P2N("CGrayDlg::OnKillFocus\r\n");
	PersistDlg::OnKillFocus(pNewWnd);
}

void CGrayDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 

{
	//P2N("CGrayDlg::OnActivate %d\r\n", nState);
	PersistDlg::OnActivate(nState, pWndOther, bMinimized);	
	InvalidateRect(titlerect);
	//Invalidate();
}


void CGrayDlg::InitVars()

{
	nstate = 0;
	
	LOGFONT		m_lf;

	::GetObject((HFONT)GetStockObject(DEFAULT_GUI_FONT), sizeof(m_lf), &m_lf);
	m_lf.lfHeight = -12; m_font.CreateFontIndirect(&m_lf);

	dheadcol	= RGB(64, 64, 64);
	headcol		= RGB(128, 128, 128);
	titleheight = 16;
	//destroyed = 0;
	noclosebox = noheader = false;
}

// Filter alt keys for this dialog

int CGrayDlg::IsDlgKey(MSG *pMsg)

{
	int ret = 0;
	
	if(	pMsg->message == WM_SYSKEYDOWN ||
			pMsg->message == WM_SYSKEYUP ||
				pMsg->message == WM_SYSCHAR )
		{

		//P2N("CGrayDlg::IsDlgKey\r\n");

		CWnd *win, *win2;
		win = win2 = GetNextDlgTabItem(NULL);

		while(true)
			{
			CString text;
			win2->GetWindowText(text);

			int idx = text.Find("&");
			if(idx >= 0)
				{
				//P2N("Dialog text: '%s' %c %c \r\n", 
				//		text, toupper(text[idx+1]), toupper(pMsg->wParam));
				
				if(toupper(text[idx+1]) == toupper(pMsg->wParam) ) 
					{
					ret = true;	
					break;
					}
				}
			win2 = GetNextDlgTabItem(win2);

			// Back to the beginning of list
			if(win2 == win)
				break;
			}
		//P2N("CGrayDlg::IsDlgKey %d %s\r\n", ret, mxpad.num2msg(pMsg->message));
		}

	return ret;
}

BOOL CGrayDlg::PreTranslateMessage(MSG* pMsg) 

{
	// Send SYSTEM messages to main window

#if 0
	if(	pMsg->message == WM_SYSKEYDOWN ||
			pMsg->message == WM_SYSKEYUP ||
				pMsg->message == WM_SYSCHAR )
		{
		// Filter keys that do not belong to this one	
		if(!IsDlgKey(pMsg))
			{
			P2N("CGrayDlg::PreTranslateMessage - Dialog ALT key %d %c\r\n",
				pMsg->wParam, pMsg->wParam);

			return PersistDlg::PreTranslateMessage(pMsg);
			}
		}
#endif

	if(IsDialogMessage(pMsg))
		return true;

	return PersistDlg::PreTranslateMessage(pMsg);
}

void CGrayDlg::Show(int center)

{
	//P2N("CGrayDlg::Show() %p '%s' '%s' want_x=%d, want_y=%d\r\n", 
	//					this, m_title, windowname, want_x, want_y);

	PersistDlg::Show();	
}


void CGrayDlg::SetWindowText(LPCTSTR lpszString)

{
	//P2N("PersistDlg::SetWindowText '%s'\r\n", lpszString);
	m_title = lpszString;

	// If it is a window, show changes
	if(IsWindow(m_hWnd))
		InvalidateRect(&titlerect);
}

