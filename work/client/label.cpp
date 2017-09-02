//////////////////////////////////////////////////////////////////////////
// Label.cpp : implementation file
//

#include "stdafx.h"
#include "Resource.h"
#include "Label.h"

#include "client.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CLabel, CStatic)

/////////////////////////////////////////////////////////////////////////////
// CLabel

CLabel::CLabel()
{
	inited = false;

	m_crText = GetSysColor(COLOR_WINDOWTEXT);
	m_hBrush = ::CreateSolidBrush(GetSysColor(COLOR_3DFACE));

	::GetObject((HFONT)GetStockObject(DEFAULT_GUI_FONT),sizeof(m_lf),&m_lf);

	m_font.CreateFontIndirect(&m_lf);
	m_bTimer = FALSE;
	m_bState = FALSE;
	m_bLink = TRUE;
	m_hCursor = NULL;
	m_Type = None;

	m_center = false;
	m_right = false;
	transparent = false;
	m_bkcolor = GetSysColor(COLOR_3DFACE);
	m_crText = GetSysColor(COLOR_BTNTEXT);

	m_hwndBrush = ::CreateSolidBrush(GetSysColor(COLOR_3DFACE));	
}


CLabel::~CLabel()
{
	m_font.DeleteObject();
	::DeleteObject(m_hBrush);
	::DeleteObject(m_hwndBrush);
}

CLabel& CLabel::SetText(const CString& strText)

{
	m_text = strText;
	ReDraw();

	return *this;
}

CLabel& CLabel::SetText(const char *strText)

{
	m_text = strText;
	ReDraw();

	return *this;
}

//////////////////////////////////////////////////////////////////////////

void	CLabel::ReDraw()

{
	// Invalidate window we are sitting on
	RECT prc; GetWindowRect(&prc);

	GetParent()->ScreenToClient(&prc);
	GetParent()->InvalidateRect(&prc);
	//Invalidate();

	//GetParent()->Invalidate();
}

//////////////////////////////////////////////////////////////////////////

CLabel& CLabel::SetTextColor(COLORREF crText)

{
	m_crText = crText;

	ReDraw();

	return *this;
}

CLabel& CLabel::SetFontBold(BOOL bBold)
{	
	m_lf.lfWeight = bBold ? FW_BOLD : FW_NORMAL;
	ReconstructFont();
	ReDraw();
	return *this;
}

CLabel& CLabel::SetFontUnderline(BOOL bSet)
{	
	m_lf.lfUnderline = bSet;
	ReconstructFont();
	ReDraw();
	return *this;
}

CLabel& CLabel::SetFontItalic(BOOL bSet)
{
	m_lf.lfItalic = bSet;
	ReconstructFont();
	ReDraw();
	return *this;	
}

CLabel& CLabel::SetSunken(BOOL bSet)
{
	if (!bSet)
		ModifyStyleEx(WS_EX_STATICEDGE,0,SWP_DRAWFRAME);
	else
		ModifyStyleEx(0,WS_EX_STATICEDGE,SWP_DRAWFRAME);
		
	return *this;	
}

CLabel& CLabel::SetBorder(BOOL bSet)
{
	if (!bSet)
		ModifyStyle(WS_BORDER,0,SWP_DRAWFRAME);
	else
		ModifyStyle(0,WS_BORDER,SWP_DRAWFRAME);
		
	return *this;	
}

CLabel& CLabel::SetFontSize(int nSize)
{
	nSize*=-1;
	m_lf.lfHeight = nSize;
	ReconstructFont();
	ReDraw();
	return *this;
}


CLabel& CLabel::SetBkColor(COLORREF crBkgnd)
{
	if (m_hBrush)
		::DeleteObject(m_hBrush);

	m_bkcolor = crBkgnd;

	m_hBrush = ::CreateSolidBrush(crBkgnd);
	return *this;
}

CLabel& CLabel::SetFontName(const CString& strFont)
{	
	strcpy(m_lf.lfFaceName,strFont);
	ReconstructFont();
	ReDraw();
	return *this;
}

BEGIN_MESSAGE_MAP(CLabel, CStatic)
	//{{AFX_MSG_MAP(CLabel)
	//ON_WM_CTLCOLOR_REFLECT()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLabel message handlers

HBRUSH CLabel::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	if (CTLCOLOR_STATIC == nCtlColor)
		{
		pDC->SelectObject(&m_font);
		pDC->SetTextColor(m_crText);
		pDC->SetBkMode(TRANSPARENT);
		//return NULL;
		}

	if (m_Type == Background)
		{
		if (!m_bState)
			return m_hwndBrush;
		}

	pDC->SetTextColor(m_crText);
	pDC->SetBkMode(TRANSPARENT);

	if(transparent)
		return NULL;

	return m_hBrush;
}

void CLabel::ReconstructFont()
{
	m_font.DeleteObject();
	BOOL bCreated = m_font.CreateFontIndirect(&m_lf);

	ASSERT(bCreated);
}


CLabel& CLabel::FlashText(BOOL bActivate)
{
	if (m_bTimer)
	{
		//SetWindowText(m_strText);
		KillTimer(1);
	}

	if (bActivate)
	{
		//GetWindowText(m_strText);
		m_bState = FALSE;
		
		m_bTimer = TRUE;
		SetTimer(1,500,NULL);
		m_Type = Text;
	}

	return *this;
}

CLabel& CLabel::FlashBackground(BOOL bActivate)
{

	if (m_bTimer)
		KillTimer(1);

	if (bActivate)
	{
		m_bState = FALSE;

		m_bTimer = TRUE;
		SetTimer(1,500,NULL);

		m_Type = Background;
	}

	return *this;
}


void CLabel::OnTimer(UINT nIDEvent) 
{
	m_bState = !m_bState;

	switch (m_Type)
	{
		case Text:
			if (m_bState)
				{
				m_backup = m_text;
				m_text = "";
				}
			else
				m_text = m_backup;
		break;

		case Background:
			InvalidateRect(NULL, FALSE);
			UpdateWindow();
		break;
	}
	
	CStatic::OnTimer(nIDEvent);
}

CLabel& CLabel::SetLink(BOOL bLink)

{
	m_bLink = bLink;

	if (bLink)
		ModifyStyle(0,SS_NOTIFY);
	else
		ModifyStyle(SS_NOTIFY,0);

	return *this;
}

void CLabel::OnLButtonDown(UINT nFlags, CPoint point) 

{
	CString strLink;

	//P2N("Left button down on clabel\r\n");

	if(m_bLink)
		{
		GetWindowText(strLink);
		ShellExecute(NULL, "open", strLink, NULL, NULL, SW_SHOWNORMAL);
		}
		
	CStatic::OnLButtonDown(nFlags, point);
}

BOOL CLabel::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 

{
	if (m_hCursor)
	{
		::SetCursor(m_hCursor);
		return TRUE;
	}

	return CStatic::OnSetCursor(pWnd, nHitTest, message);
}

CLabel& CLabel::SetLinkCursor(HCURSOR hCursor)

{
	m_hCursor = hCursor;
	return *this;
}

//////////////////////////////////////////////////////////////////////////

void	CLabel::OnPaint()

{
	CPaintDC dc(this); // device context for painting

	// Get initial text
	if(!inited)
		{
		inited = true;
		if(m_text == "")
			GetWindowText(m_text);
		}

	RECT	rc;
	GetClientRect(&rc);

	if(!transparent)
		dc.FillSolidRect(&rc, m_bkcolor);

	dc.SetTextColor(m_crText);
	dc.SetBkMode(TRANSPARENT);

	CFont* oldFont = dc.SelectObject(&m_font);

	int xx = 0, yy = 0;
	if(m_center)
		{
		CSize ss = dc.GetTextExtent(m_text);
		xx = (rc.right - rc.left)/2  - ss.cx/2;
		}
	else if(m_right)
		{
		CSize ss = dc.GetTextExtent(m_text);
		xx = rc.right - ss.cx;
		}

	dc.TextOut(xx, yy, m_text);
	dc.SelectObject(oldFont);

	//PrintToNotepad("CLabel Paint %s\r\n", m_text);

	//CStatic::OnPaint();
}

//////////////////////////////////////////////////////////////////////////
 
BOOL CLabel::OnEraseBkgnd(CDC *pDC)

{
	//P2N("Erase Background %p\r\n", this);
	return true;
}

void CLabel::GetText(CString &str)
{
	str = m_text;
}
