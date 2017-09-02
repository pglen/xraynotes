// GrayWin.cpp: implementation of the CGrayDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "dbtest.h"
#include "GrayWin.h"
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

}

CGrayDlg::CGrayDlg(UINT nIDTemplate, CWnd* pParentWnd)
	: PersistDlg(nIDTemplate, pParentWnd)

{
	m_title = "";
	nstate = 0;
}

CGrayDlg::~CGrayDlg()
{

}

BEGIN_MESSAGE_MAP(CGrayDlg, PersistDlg)
	//{{AFX_MSG_MAP(CGrayDlg)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()	
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_ACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CGrayDlg::OnInitDialog() 

{
	PersistDlg::OnInitDialog();
	
	
	LOGFONT		m_lf;

	::GetObject((HFONT)GetStockObject(DEFAULT_GUI_FONT), sizeof(m_lf), &m_lf);
	m_lf.lfHeight = -12;
	m_font.CreateFontIndirect(&m_lf);

	// Evaluate head/closerect
	RECT	rc3; GetClientRect(&rc3);
	
	closerect.top = rc3.top + 6;
	closerect.left = rc3.right - 16;
	closerect.bottom = closerect.top + 8;
	closerect.right = closerect.left + 8;

	titlerect.top = 2;
	titlerect.left = 2;
	titlerect.bottom = titlerect.top + 17;
	titlerect.right = rc3.right - 4;
	
	return true;
}

void CGrayDlg::OnPaint() 

{
	CPaintDC dc(this); // device context for painting
	
	RECT rc; GetClientRect(&rc);

	// Draw header
	if(!noheader)
		{
		if(GetActiveWindow() == this)		
			dc.FillSolidRect(titlerect, RGB(64, 64, 64));
		else
			dc.FillSolidRect(titlerect, RGB(128, 128, 128));
	}

	//dc.SetTextColor(m_crText);
	dc.SetBkMode(TRANSPARENT);

	// Header
	CRect rcc(8, 3, rc.right - 12, 17);
	
	CFont* oldFont = dc.SelectObject(&m_font);
	dc.DrawText(m_title, m_title.GetLength(), rcc, DT_LEFT);
	dc.SelectObject(oldFont);

	// Draw corner close
	dc.FillSolidRect(rc.right - 17, 2, rc.right - 4, 16, RGB(32, 32, 32));

	CPen pen; pen.CreatePen(PS_SOLID, 2, RGB(255,255,255));
	CPen *oldpen;

	oldpen = (CPen*)dc.SelectObject(pen);

	dc.MoveTo( rc.right - 14, 14);
	dc.LineTo( rc.right - 4, 4);

	dc.MoveTo( rc.right - 14, 4);
	dc.LineTo( rc.right - 4, 14);
	
	// Do not call CGrayDlg::OnPaint() for painting messages
}

void CGrayDlg::OnLButtonDown(UINT nFlags, CPoint point) 

{
	RECT rc; GetClientRect(&rc);
	
	if(closerect.PtInRect(point))
		{
		P2N("CGrayDlg::OnLButtonDown - Click in Close Box\r\n");
		//EndDialog(IDOK);
		
		ShowWindow(SW_HIDE);
		return;
		}

	if(titlerect.PtInRect(point))
		{
		P2N("CGrayDlg::OnLButtonDown Click in Header box\r\n");
		PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));	
		return;
		}

	//PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));			
	
	PersistDlg::OnLButtonDown(nFlags, point);
}

void CGrayDlg::OnSize(UINT nType, int cx, int cy) 

{
	PersistDlg::OnSize(nType, cx, cy);
	
	// Evaluate head/closerect
	RECT	rc3; GetClientRect(&rc3);

	closerect.top = rc3.top + 6;
	closerect.left = rc3.right - 16;
	closerect.bottom = closerect.top + 8;
	closerect.right = closerect.left + 8;

	titlerect.top = 2;
	titlerect.left = 2;
	titlerect.bottom = titlerect.top + 17;
	titlerect.right = rc3.right - 1;	
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

