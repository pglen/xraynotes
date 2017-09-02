// Popup.cpp : implementation file
//

#include "stdafx.h"
#include "xcal.h"
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
	padding = 20;
	first = created = 0;
	bgcolor = RGB(0xff, 0xff, 0xee);

	::GetObject((HFONT)GetStockObject(DEFAULT_GUI_FONT),sizeof(m_lf),&m_lf);
	m_font.CreateFontIndirect(&m_lf);
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
	ON_WM_SHOWWINDOW()
	ON_WM_KEYDOWN()
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
		if(m_pSetLayeredWindowAttributes)
			m_pSetLayeredWindowAttributes(m_hWnd, 0, bAlpha, LWA_ALPHA);
		}

	::GetObject((HFONT)GetStockObject(DEFAULT_GUI_FONT), sizeof(m_lf), &m_lf);
	//m_lf.lfHeight = -26;

	m_font.CreateFontIndirect(&m_lf);

	//m_str.SetTrans();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

#endif

void CPopup::OnShowWindow(BOOL bShow, UINT nStatus) 

{
	//P2N("CPopup::OnShowWindow %d\r\n", bShow);
	CEdit::OnShowWindow(bShow, nStatus);
}

void CPopup::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 

{
	//P2N("CPopup::OnKeyDown Key down on tooltip\r\n");
	Hide();
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CPopup::PreTranslateMessage(MSG* pMsg) 

{
	//if(!mxpad.is_junk(pMsg->message))
	//	P2N("Message: %d %s\r\n", pMsg->message, mxpad.num2msg(pMsg->message));

	//P2N("CPopup::PreTranslateMessage:  %s\r\n", mxpad.num2msg(pMsg->message));

	if(pMsg->message == WM_KEYDOWN)
		{
		Hide();
		return 0;
		}

	return CEdit::PreTranslateMessage(pMsg);
}


void CPopup::PopAutoSize()
	
{
	PAINTSTRUCT Paint;
	
	Create();

	CDC* dc = BeginPaint( &Paint );
	CFont* oldFont = dc->SelectObject(&m_font);

	//CSize textsize = dc->GetTextExtent(str);
	
	CRect rc;
	dc->DrawText(str, &rc, DT_CALCRECT);

	dc->SelectObject(oldFont);
	EndPaint(&Paint);

	SetWindowPos(NULL, 0, 0, rc.Width() + padding, rc.Height() + padding, SWP_NOMOVE | SWP_NOZORDER);

	CRect	rc2;
	WINDOWPLACEMENT wp;

	GetWindowPlacement(&wp); rc2 = wp.rcNormalPosition;

	CRect	rc3;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rc3, false);
	int     xsize	=  rc3.Width();
    int     ysize	=  rc3.Height();

	//HDC		hdc		=  ::GetDC(HWND_DESKTOP);
    //int     xsize	=  ::GetDeviceCaps(hdc, HORZRES);
    //int     ysize	=  ::GetDeviceCaps(hdc, VERTRES);

	xsize -= 15; ysize -= 15;

	// Needs a turn around to show to the left
	if(rc2.right > xsize)
		{
		//P2N("Needs a turn around to show to the left\r\n");
		SetWindowPos( NULL, xsize - rc2.Width(), rc2.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE );  	
		}

	// Refresh window placement info
	GetWindowPlacement(&wp); rc2 = wp.rcNormalPosition;

	// Needs a turn around to show to the upper  side
	if(rc2.bottom > ysize)
		{
		//P2N("Needs a turn around to show to the upper side\r\n");
		SetWindowPos( NULL, rc2.left, ysize - rc2.Height(), 0, 0, SWP_NOZORDER | SWP_NOSIZE );  	
		}
}
			
void CPopup::SetText(const char *xstr)

{
	//P2N("Setting tooltip text: '%s'\r\n", xstr);
	str = xstr;

	// Autowrap to size
	int idx = 0;
	while(true)
		{
		idx += 64;

		if(idx > str.GetLength())
			break;

		// Wobble no more than half length
		for(int loop = 0; loop < 16; loop++)
			{
			if(isspace(str.GetAt(idx - loop)) )
				{
				idx -= loop - 1;
				break;
				}
			}
		str.Insert(idx, "\r\n");
		}
	
	PopAutoSize();
}

void CPopup::Move(CPoint screen)

{
	Create();

	//SetWindowPos(&wndTopMost, screen.x, screen.y, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE );
	SetWindowPos(NULL, screen.x, screen.y, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE  );
}

void CPopup::OnPaint() 

{
	if(!first++)
		{
		//P2N("CPopup::OnPaint First paint\r\n");
		ModifyStyle(WS_DISABLED, WS_THICKFRAME | SS_NOTIFY | WS_VISIBLE, SWP_DRAWFRAME);
		SetFocus();
		}

	CPaintDC dc(this); // device context for painting

	CFont* oldFont = dc.SelectObject(&m_font);

	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(0,0,0));	

	CRect rc; GetClientRect(&rc);
	rc.left += padding/2; rc.top += padding/3;

	dc.DrawText(str, &rc, DT_NOPREFIX | DT_LEFT );
	
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
	//P2N("CPopup::Show()\r\n");
	Create();

	SetTimer(1, 15000, NULL);

	//ShowWindow(SW_SHOWNOACTIVATE);
	
	EnableWindow(true);
	SetForegroundWindow();
	BringWindowToTop();
	ShowWindow(SW_SHOW);
	SetFocus();
	
	//SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_SHOWWINDOW );
}

void CPopup::Hide()

{
	Create();
	KillTimer(1);
	ShowWindow(SW_HIDE);
}

void CPopup::Create()

{
	if(!created)
		{
		created = true;

		RECT rc;  ::SetRect(&rc, 0, 0, 10, 10);
		int ret = CWnd::Create(0, 0, WS_VISIBLE , rc, parent, 100);

		//P2N("Create ret=%d err=%s\r\n", ret, strerror(errno));
		//ModifyStyle(WS_DISABLED,  WS_THICKFRAME | SS_NOTIFY |WS_VISIBLE, SWP_DRAWFRAME);
		//ModifyStyleEx(0, WS_EX_STATICEDGE, SWP_DRAWFRAME);
		}
}

void CPopup::Create(CWnd *pp)

{
	if(!created)
		{
		created = true;

		RECT rc;  ::SetRect(&rc, 0, 0, 10, 10);
		int ret = CWnd::Create(0, 0, 0, rc, pp, 100);
		}
}

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

