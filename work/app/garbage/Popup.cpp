// Popup.cpp : implementation file
//

#include "stdafx.h"
#include "xraynotes.h"
#include "support.h"


//include "shades.h"

#include "Popup.h"
#include "mxpad.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPopup dialog


CPopup::CPopup()
	
{
	//{{AFX_DATA_INIT(CPopup)
	//}}AFX_DATA_INIT

	parent	= GetDesktopWindow();
	//parent = AfxGetApp()->m_pMainWnd;

	padding = 20;
	first =  0;
	//maxwidth = 128;
	maxwidth = 0;
	bgcolor = RGB(0xff, 0xff, 0xee);

	idletime = 15000;

	::GetObject((HFONT)GetStockObject(DEFAULT_GUI_FONT),sizeof(m_lf),&m_lf);
	m_font.CreateFontIndirect(&m_lf);

	m_lf.lfWeight = FW_BOLD;
	m_fontb.CreateFontIndirect(&m_lf);

	//windowname = "CPopup";
}

void CPopup::DoDataExchange(CDataExchange* pDX)
{
	CEdit::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPopup)
	//DDX_Control(pDX, IDC_TEXT, m_str);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPopup, CEdit)
	//{{AFX_MSG_MAP(CPopup)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_ACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPopup message handlers

BOOL CPopup::OnEraseBkgnd(CDC* pDC) 

{
	RECT rect; 
	GetClientRect(&rect);

	CBrush NewBrush;

	//NewBrush.CreateSolidBrush(RGB(0xff, 0xff, 0x80)) ;
	NewBrush.CreateSolidBrush(bgcolor) ;

	pDC->SetBrushOrg(0,0) ;
	CBrush* pOldBrush = (CBrush*)pDC->SelectObject(&NewBrush);
	pDC->PatBlt(rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, PATCOPY);
	pDC->SelectObject(pOldBrush);
	
	//return CTransDlg::OnEraseBkgnd(pDC);
	return(false);
}

#if 0

BOOL CPopup::OnInitDialog() 
{
	CEdit::OnInitDialog();
	
	BYTE	bAlpha = 255;

	if (bAlpha < 255)
		{
        //  Set layered style for the dialog
        SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);

        //  Call it with 255 as alpha - opacity
		if(g_pSetLayeredWindowAttributes)
			g_pSetLayeredWindowAttributes(m_hWnd, 0, bAlpha, LWA_ALPHA);
		}

	//::GetObject((HFONT)GetStockObject(DEFAULT_GUI_FONT), sizeof(m_lf), &m_lf);
	//m_lf.lfHeight = -26;
	//m_font.CreateFontIndirect(&m_lf);

	//m_str.SetTrans();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

#endif

//DEL void CPopup::OnShowWindow(BOOL bShow, UINT nStatus) 
//DEL 
//DEL {
//DEL 	//P2N("CPopup::OnShowWindow %d\r\n", bShow);
//DEL 	CEdit::OnShowWindow(bShow, nStatus);
//DEL }

//DEL void CPopup::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
//DEL 
//DEL {
//DEL 	P2N("CPopup::OnKeyDown Key down on tooltip\r\n");
//DEL 	Hide();
//DEL 	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
//DEL }

//DEL BOOL CPopup::PreTranslateMessage(MSG* pMsg) 
//DEL 
//DEL {
//DEL 	//if(!mxpad.is_junk(pMsg->message))
//DEL 	//	P2N("Message: %d %s\r\n", pMsg->message, mxpad.num2msg(pMsg->message));
//DEL 
//DEL 	//P2N("CPopup::PreTranslateMessage:  %s\r\n", mxpad.num2msg(pMsg->message));
//DEL 
//DEL 	if(pMsg->message == WM_KEYDOWN)
//DEL 		{
//DEL 		Hide();
//DEL 		return 0;
//DEL 		}
//DEL 
//DEL 	return CEdit::PreTranslateMessage(pMsg);
//DEL }


void CPopup::PopAutoSize()
	
{
	PAINTSTRUCT Paint;
	
	xCreate();

	CDC* dc = BeginPaint(&Paint);
	CFont* oldFont = dc->SelectObject(&m_font);

	//CSize textsize = dc->GetTextExtent(str);
	
	CRect rc;
	dc->DrawText(str, &rc, DT_CALCRECT | DT_EXPANDTABS | DT_NOPREFIX );
	dc->SelectObject(oldFont);
	EndPaint(&Paint);

	int ww = rc.Width() + padding;
	int hh = rc.Height() + padding;

	CRect	rc3; misc.CurrentMonRect(here, &rc3);

	//P2N("GetMonitorInfo left=%d right=%d top=%d bottom=%d\r\n",  
	//					rc3.left, rc3.right, rc3.top, rc3.bottom);

	//P2N("Herex=%d herey=%d\r\n", here.x, here.y);

	int ll = here.x, tt = here.y;

	// Needs a turn around to show to the left
	if(ll > rc3.left + rc3.Width() / 2)
		{
		//P2N("Needs a turn around to show to the left\r\n");		
		ll = here.x - ww - 20;
		}
	// Needs a turn around to show to the upper side
	if(tt > rc3.top + rc3.Height()/2)
		{
		//P2N("Needs a turn around to show to the upper side\r\n");
		tt = here.y - hh - 20;
		}

	// Commit it here
	SetWindowPos( NULL, ll, tt, ww, hh, SWP_NOZORDER );  	

	// Refresh window placement info
	//GetWindowPlacement(&wp); rc2 = wp.rcNormalPosition;
}
			
void CPopup::SetText(const char *xstr)

{
	//P2N("Setting tooltip text: '%s'\r\n", xstr);
	str = xstr;

	// Autowrap to size
	if(maxwidth)
		{
		int idx = 0;
		while(true)
			{
			idx += maxwidth;

			if(idx > str.GetLength())
				break;

			// Wobble no more than half length
			for(int loop = 0; loop < maxwidth/2; loop++)
				{
				if(isspace(str.GetAt(idx - loop)) )
					{
					idx -= loop - 1;
					break;
					}
				}
			str.Insert(idx, "\r\n");
			}
		}
	PopAutoSize();
}

void CPopup::Move(CPoint screen)

{
	xCreate();

	here = screen;

	//SetWindowPos(&wndTopMost, screen.x, screen.y, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE );
	//SetWindowPos(NULL, screen.x, screen.y, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE  );
}

void CPopup::OnPaint() 

{
	if(!first++)
		{
		//P2N("CPopup::OnPaint First paint\r\n");
		//ModifyStyle(WS_DISABLED, WS_THICKFRAME | SS_NOTIFY | WS_VISIBLE, SWP_DRAWFRAME);
		ModifyStyle(WS_DISABLED, WS_BORDER | /**/SS_NOTIFY | WS_VISIBLE, SWP_DRAWFRAME);
		SetFocus();
		}

	CPaintDC dc(this); // device context for painting

	CFont* oldFont = dc.SelectObject(&m_font);

	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(0,0,0));	

	CRect rc; GetClientRect(&rc);
	rc.left += padding/2; rc.top += padding/3;

	//CString leftstr, rightstr;
	//int idx = str.Find("<b>");
	//if(idx)
	//	{
	//	dc.SelectObject(&m_fontb);
	//	leftstr = str.Left(idx);
	//	rightstr = str.Mid(idx+3);
	//	dc.DrawText(str, &rc, DT_NOPREFIX | DT_LEFT | DT_EXPANDTABS);
	//	}
	//else
	//	{
	//	dc.DrawText(str, &rc, DT_NOPREFIX | DT_LEFT | DT_EXPANDTABS);
	//	}

	dc.DrawText(str, &rc, DT_NOPREFIX | DT_LEFT | DT_EXPANDTABS);

	dc.SelectObject(oldFont);

	// Do not call CEdit::OnPaint() for painting messages
}

void CPopup::OnTimer(UINT nIDEvent) 

{
	//P2N("Timer Fired\r\n");
	//EndDialog(IDOK);

	KillTimer(nIDEvent);
	Hide();

	CEdit::OnTimer(nIDEvent);
}

void CPopup::Show()

{
	CString shstr = str.Left(24);
	P2N("CPopup::Show: '%s'\r\n", shstr);
	
	xCreate();

	//SetTimer(1, 15000, NULL);
	SetTimer(1, idletime, NULL);

	BringWindowToTop();
	ShowWindow(SW_SHOW);
}

void CPopup::Hide()

{
	// If it was never used, ignore
	if(!IsWindow(m_hWnd))
		return;

	KillTimer(1);
	ShowWindow(SW_HIDE);
}

void CPopup::xCreate()

{
	if(!IsWindow(m_hWnd))
		{

		RECT rc;  ::SetRect(&rc, 0, 0, 10, 10);
		int ret = CWnd::Create(0, 0, WS_VISIBLE , rc, parent, 100);

		//P2N("Create ret=%d err=%s\r\n", ret, strerror(errno));
		//ModifyStyle(WS_DISABLED,  WS_THICKFRAME | SS_NOTIFY |WS_VISIBLE, SWP_DRAWFRAME);
		//ModifyStyleEx(0, WS_EX_STATICEDGE, SWP_DRAWFRAME);
		}
}

//DEL void CPopup::Create(CWnd *pp, int aa)
//DEL 
//DEL {
//DEL 	if(!IsWindow(m_hWnd))
//DEL 		{
//DEL 
//DEL 		RECT rc;  ::SetRect(&rc, 0, 0, 10, 10);
//DEL 		int ret = CWnd::Create(0, 0, 0, rc, pp, 100);
//DEL 		}
//DEL }

#if 0
void CPopup::OnActivate(UINT nState, CEdit* pWndOther, BOOL bMinimized) 

{
	//N("CPopup::OnActivate\r\n");
	CEdit::OnActivate(nState, pWndOther, bMinimized);
}
#endif

// Overloaded to do everything

void	CPopup::Show(const char *str, CPoint screen)

{
	Move(screen); SetText(str); Show();
}

