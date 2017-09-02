
/* =====[ label.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the label.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  10/27/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

//////////////////////////////////////////////////////////////////////////
// Label.cpp : implementation file
//

#include "stdafx.h"
#include "Resource.h"
#include "xraynotes.h"
#include "Label.h"

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
	//P2N("Construct CLabel %p\r\n", this);
	
	m_inited = false;

	m_crText = GetSysColor(COLOR_WINDOWTEXT);
	m_hBrush = ::CreateSolidBrush(GetSysColor(COLOR_3DFACE));

	::GetObject((HFONT)GetStockObject(DEFAULT_GUI_FONT),sizeof(m_lf),&m_lf);

	m_font.CreateFontIndirect(&m_lf);
	m_bTimer = FALSE;
	m_bState = FALSE;
	m_bLink = FALSE;
	m_hCursor = NULL;
	m_Type = None;

	m_center = false;
	m_right = false;
	m_frame = false;
	m_edge = false;
	m_modframe = false;

	m_transparent = false;

	m_bkcolor = GetSysColor(COLOR_3DFACE);
	m_crText = GetSysColor(COLOR_BTNTEXT);

	m_text = "none";
	m_hwndBrush = ::CreateSolidBrush(GetSysColor(COLOR_3DFACE));	
}

CLabel::~CLabel()

{
	m_font.DeleteObject();
	::DeleteObject(m_hBrush);
	::DeleteObject(m_hwndBrush);
}

CLabel& CLabel::SetText(const CString& strText, int recalc)

{
	//GetWindowRect(&orig);

	m_text = strText;

	if(recalc)
		xCalcSize();

	xReDrawParent();	xReDraw();
	
	//P2N("Setting text to %s\r\n", m_text);

	return *this;
}

CLabel& CLabel::SetTextP(const char * strText, ...)

{
	//GetWindowRect(&orig);

	CString strtmp;
	
	char *buff = strtmp.GetBuffer(STRTMP);
	va_list ArgList; va_start(ArgList, strText);
    _vsnprintf(buff, STRTMP, strText, ArgList);
	strtmp.ReleaseBuffer();

	m_text = strtmp;

	//P2N("CLabel::SetTextP '%s'\r\n", m_text);

	xCalcSize();	xReDrawParent();	xReDraw();
	
	return *this;
}

//////////////////////////////////////////////////////////////////////////
// Invalidate window we are sitting on, parent too if trans

void	CLabel::xReDraw()

{
	Invalidate();
	support.YieldToWinEx();
}

//////////////////////////////////////////////////////////////////////////

CLabel& CLabel::SetTextColor(COLORREF crText)

{
	m_crText = crText;

	xReDraw();

	return *this;
}

CLabel& CLabel::SetFontBold(BOOL bBold)
{	
	m_lf.lfWeight = bBold ? FW_BOLD : FW_NORMAL;

	xReDrawParent();
	xReconstructFont();
	
	xCalcSize();
	xReDraw();
	return *this;
}

CLabel& CLabel::SetFontUnderline(BOOL bSet)
{	
	m_lf.lfUnderline = bSet;
	xReconstructFont();
	xCalcSize();
	xReDraw();
	return *this;
}

CLabel& CLabel::SetFontItalic(BOOL bSet)
{
	m_lf.lfItalic = bSet;
	xReconstructFont();
	xCalcSize();
	xReDraw();
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
	xReDrawParent();

	nSize*=-1;
	m_lf.lfHeight = nSize;
	xReconstructFont();
	xCalcSize();
	xReDraw();
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

	xReDrawParent();
	xReconstructFont();
	xReDraw();
	xCalcSize();
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

	if(m_transparent)
		return NULL;

	return m_hBrush;
}

void CLabel::xReconstructFont()

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

CLabel& CLabel::FlashBk(BOOL bActivate)
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
		ModifyStyle(0, SS_NOTIFY);
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

	//P2N("Painting top=%d left=%d bottom=%d right=%d\r\n", 
	//						dc.m_ps.rcPaint.top,
	//							dc.m_ps.rcPaint.left,
	//								dc.m_ps.rcPaint.bottom,
	//									dc.m_ps.rcPaint.right);

	if(!m_inited)
		xInitLab();

	RECT	rc;
	GetClientRect(&rc);

	if(!m_transparent)
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

	//P2N("CLabel Paint %s\r\n", m_text);

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

void CLabel::CenterText(int flag)

{
	m_center = flag;
}

void CLabel::SetTrans(int flag)

{
	xReDrawParent();	
	m_transparent = flag;
	xReDrawParent();	

	xReDraw();
}

//////////////////////////////////////////////////////////////////////////
// Calculate text size, resize window

void CLabel::xCalcSize()

{
	PAINTSTRUCT Paint;

	if(!m_inited)
		xInitLab();
		
	CDC* dc = BeginPaint( &Paint );

	CFont* oldFont = dc->SelectObject(&m_font);
	textsize = dc->GetTextExtent(m_text);
	dc->SelectObject(oldFont);
	EndPaint(&Paint);

	int yframe = 2, xframe = 2;

	//if(	GetWindowLong(m_hWnd, GWL_STYLE) & SS_BLACKFRAME)

	if(m_frame)
		{
		//P2N("Frame around it\r\n");
		xframe += 4;
		yframe += 4;
		}
	if(m_edge )
		{
		xframe += 12;
		yframe += 12;
		}

	 if(m_modframe)
		{
		xframe += 12;
		yframe += 12;
		}

	//P2N("xCalcSize %p Orig rect left=%d right=%d top=%d bottom=%d '%s'\r\n",
	//				this, orig.left, orig.right, orig.top, orig.bottom, m_text);

	//P2N("New text size: xx=%d yy=%d\r\n", textsize.cx, textsize.cy);

	RECT prc(orig); 

	// Only mess with it if text is bigger than the window
	if((prc.right - prc.left) < textsize.cx)
		{		
		if(m_center)
			{
			// --------------------------------------------------
			//        | left        | mid            | right

			int hcnt = prc.left + (prc.right - prc.left) / 2;
			int vcnt = prc.top  + (prc.bottom - prc.top) / 2;
			
			//P2N("New text pos: xx=%d yy=%d\r\n", hcnt - textsize.cx/2, vcnt - textsize.cy/2);

			SetWindowPos(NULL, hcnt - textsize.cx/2, prc.top, 
								textsize.cx + xframe, textsize.cy + yframe, SWP_NOOWNERZORDER | SWP_NOMOVE);
			}
		else if(m_right)
			{
			SetWindowPos(NULL, prc.right - textsize.cx, prc.top, 
								textsize.cx + xframe, textsize.cy + yframe, SWP_NOOWNERZORDER | SWP_NOMOVE);
			}
		else
			{
			SetWindowPos( NULL, 0, 0, textsize.cx + xframe, textsize.cy + yframe, SWP_NOOWNERZORDER | SWP_NOMOVE);
			}
		}
}

//////////////////////////////////////////////////////////////////////////

void	CLabel::xReDrawParent()

{
	RECT prc; GetWindowRect(&prc);

	// xReDraw parent if transparent
	if(m_transparent)
		{
		GetParent()->ScreenToClient(&prc);
		GetParent()->InvalidateRect(&prc);
		}		
}

CLabel&		CLabel::operator =(const char *str)

{
	//P2N("operator = %s\r\n", str);
	
	m_text = str;

	xCalcSize();	xReDrawParent();	xReDraw();
	
	return *this;
}

void CLabel::xInitLab()

{
	m_inited = true;

	GetWindowRect(&orig);
		
	// Get text from resource
	if(m_text == "none")
		{
		GetWindowText(m_text);
		//P2N("Got window text from resource '%s'\r\n", m_text);
		}

	//P2N("Initial paint on '%s' \r\n", m_text);

	//DumpStyles();
	//DumpExStyles();
	//DumpSS();

	// Carry parameters over from resource cstatic
	DWORD style = ::GetWindowLong(m_hWnd, GWL_STYLE);

	if(style & SS_CENTER)		
		{
		//P2N(" center ");
		m_center = true;
		}
	else if(style & SS_RIGHT)		
		{
		//P2N(" right ");
		m_right = true;
		}
	else
		{
		//P2N(" left ");
		}

	if(style &  WS_BORDER)		
		{
		//P2N("Border on '%s'\r\n", m_text);
		m_frame = true;
		}
	if(style & WS_EX_CLIENTEDGE )
		{
		//P2N("Model Frame on '%s'\r\n", m_text);
		}

	// CWnd
	DWORD stylex = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);

	if(stylex & WS_EX_TRANSPARENT)
		{
		m_transparent = true;
		//P2N("Transparent\r\n");
		}
	if(stylex & WS_EX_STATICEDGE)
		{
		//P2N("static edge  on '%s'\r\n", m_text);
		m_edge = true;
		}
	if(stylex & WS_EX_DLGMODALFRAME)
		{
		//P2N("static edge  on '%s'\r\n", m_text);
		m_modframe = true;
		}

	//P2N("\r\n");
	//P2N("left=%d right=%d top=%d bottom=%d\r\n\r\n", 
	//			orig.left, orig.right, orig.top, orig.bottom);

	//xCalcSize();
}

void CLabel::DumpStyles()

{
	P2N("Window Styles:\r\n");
	DWORD style = ::GetWindowLong(m_hWnd, GWL_STYLE);

	if(style & WS_BORDER   )
		P2N("WS_BORDER   ");
	if(style & WS_CAPTION   )
		P2N("WS_CAPTION   ");
	if(style & WS_CHILD   )
		P2N("WS_CHILD   ");
	if(style & WS_CLIPCHILDREN   )
		P2N("WS_CLIPCHILDREN   ");
	if(style & WS_CLIPSIBLINGS   )
		P2N("WS_CLIPSIBLINGS   ");
	if(style & WS_DISABLED   )
		P2N("WS_DISABLED   ");
	if(style & WS_DLGFRAME   )
		P2N("WS_DLGFRAME   ");
	if(style & WS_GROUP   )
		P2N("WS_GROUP   ");
	if(style & WS_HSCROLL   )
		P2N("WS_HSCROLL   ");
	if(style & WS_MAXIMIZE   )
		P2N("WS_MAXIMIZE   ");
	if(style & WS_MAXIMIZEBOX   )
		P2N("WS_MAXIMIZEBOX   ");
	if(style & WS_MINIMIZE   )
		P2N("WS_MINIMIZE   ");
	if(style & WS_MINIMIZEBOX   )
		P2N("WS_MINIMIZEBOX   ");
	if(style & WS_OVERLAPPED   )
		P2N("WS_OVERLAPPED   ");
	if(style & WS_OVERLAPPEDWINDOW   )
		P2N("WS_OVERLAPPEDWINDOW   ");
	if(style & WS_POPUP   )
		P2N("WS_POPUP   ");
	if(style & WS_POPUPWINDOW   )
		P2N("WS_POPUPWINDOW   ");
	if(style & WS_SYSMENU   )
		P2N("WS_SYSMENU   ");
	if(style & WS_TABSTOP   )
		P2N("WS_TABSTOP   ");
	if(style & WS_THICKFRAME   )
		P2N("WS_THICKFRAME   ");
	if(style & WS_VISIBLE   )
		P2N("WS_VISIBLE   ");
	if(style & WS_VSCROLL   )
		P2N("WS_VSCROLL   ");

	P2N("\r\n");
}


void CLabel::DumpSS()

{
	P2N("Static Window Styles:\r\n");
	DWORD style = ::GetWindowLong(m_hWnd, GWL_STYLE);

	if(style & SS_BLACKFRAME   )
		P2N("  SS_BLACKFRAME   ");
	if(style & SS_BLACKRECT   )
		P2N("  SS_BLACKRECT   ");
	if(style & SS_CENTER   )
		P2N("  SS_CENTER   ");
	if(style & SS_GRAYFRAME   )
		P2N("  SS_GRAYFRAME   ");
	if(style & SS_GRAYRECT   )
		P2N("  SS_GRAYRECT   ");
	if(style & SS_ICON   )
		P2N("  SS_ICON   ");
	if(style & SS_LEFT   )
		P2N("  SS_LEFT   ");
	if(style & SS_LEFTNOWORDWRAP   )
		P2N("  SS_LEFTNOWORDWRAP   ");
	if(style & SS_NOPREFIX   )
		P2N("  SS_NOPREFIX   ");
	if(style & SS_RIGHT   )
		P2N("  SS_RIGHT   ");
	if(style & SS_SIMPLE   )
		P2N("  SS_SIMPLE   ");
	if(style & SS_USERITEM   )
		P2N("  SS_USERITEM   ");
	if(style & SS_WHITEFRAME   )
		P2N("  SS_WHITEFRAME   ");
	if(style & SS_WHITERECT   )
		P2N("  SS_WHITERECT   ");

	P2N("\r\n");
}

void CLabel::DumpExStyles()

{
	P2N("Extended Window Styles:\r\n");
	DWORD style = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);

	if(style & WS_EX_ACCEPTFILES   )
		P2N("  WS_EX_ACCEPTFILES  ");
	if(style & WS_EX_CLIENTEDGE   )
		P2N("WS_EX_CLIENTEDGE  ");
	if(style & WS_EX_CONTEXTHELP   )
		P2N("WS_EX_CONTEXTHELP  ");
	if(style & WS_EX_CONTROLPARENT   )
		P2N("WS_EX_CONTROLPARENT  ");
	if(style & WS_EX_DLGMODALFRAME   )
		P2N("WS_EX_DLGMODALFRAME  ");
	if(style & WS_EX_LEFT   )
		P2N("WS_EX_LEFT  ");
	if(style & WS_EX_LEFTSCROLLBAR   )
		P2N("WS_EX_LEFTSCROLLBAR  ");
	if(style & WS_EX_LTRREADING   )
		P2N("WS_EX_LTRREADING  ");
	if(style & WS_EX_MDICHILD   )
		P2N("WS_EX_MDICHILD  ");
	if(style & WS_EX_NOPARENTNOTIFY   )
		P2N("WS_EX_NOPARENTNOTIFY  ");
	if(style & WS_EX_OVERLAPPEDWINDOW   )
		P2N("WS_EX_OVERLAPPEDWINDOW  ");
	if(style & WS_EX_PALETTEWINDOW   )
		P2N("WS_EX_PALETTEWINDOW  ");
	if(style & WS_EX_RIGHT   )
		P2N("WS_EX_RIGHT  ");
	if(style & WS_EX_RIGHTSCROLLBAR   )
		P2N("WS_EX_RIGHTSCROLLBAR  ");
	if(style & WS_EX_RTLREADING   )
		P2N("WS_EX_RTLREADING  ");
	if(style & WS_EX_STATICEDGE   )
		P2N("WS_EX_STATICEDGE  ");
	if(style & WS_EX_TOOLWINDOW   )
		P2N("WS_EX_TOOLWINDOW  ");
	if(style & WS_EX_TOPMOST   )
		P2N("WS_EX_TOPMOST  ");
	if(style & WS_EX_TRANSPARENT   )
		P2N("WS_EX_TRANSPARENT  ");
	if(style & WS_EX_WINDOWEDGE   )
		P2N("WS_EX_WINDOWEDGE  ");

	P2N("\r\n");
}

//////////////////////////////////////////////////////////////////////////

COLORREF	CLabel::GetBkColor()

{
	return(m_bkcolor);
}
