
/* =====[ Status.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the Status.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  5/14/2007  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#include "stdafx.h"
#include "xraynotes.h"
#include "Status.h"

#include <process.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
//
// Function name:    RunProgressThread
// Description:      <empty description>
// Return type:      
// Argument:         void *lpParam
//
//////////////////////////////////////////////////////////////////////////

void	RunProgressThread(void *lpParam)

{
	CStatus  *status = (CStatus *)lpParam;

	CShadesDlg *dlg = (CShadesDlg* )status->parent;

	if(dlg->current < 0)
		{
		P2N("Invalid boundary (<0) on RunProgressThread\r\n");
		_endthread();
		return;
		}

	if(dlg->current >= ((CShadesDlg* )status->parent)->docarr.GetSize() )
		{
		P2N("Invalid boundary (<maxdoc) on RunProgressThread\r\n");
		_endthread();
		return;
		}
	//support.YieldToWinEx();

	//P2N("Started Progress Thread\r\n");
	CXrayDoc *doc = (CXrayDoc*)((CShadesDlg* )status->parent)->docarr[((CShadesDlg* )status->parent)->current]; 
	ASSERT(doc->magic == DOC_MAGIC);

	CxImg *img = doc->render;

	if(doc->commit)
		img = doc->img;

	while(true)
		{
		//P2N("Looping Progress Thread\r\n");

		if(img == NULL)
			break;

		if(!img->IsValid())
			{
			//P2N("Ivalid image in RunProgressThread\r\n");
			break;
			}

		int prog = img->GetProgress();

		//P2N("Thread %d\r\n", prog);

		if(IsWindow(status->m_hWnd))
			status->m_prog.SetPos(prog);

        support.YieldToWinEx();

		if(prog >= 100)
			break;

        if(img->GetEscape())
            break;

		// Worker thread ended
		if(!doc->thhandle)
			break;

		// Ease up on the processor ...
        Sleep(333);
		}

	//P2N("Ended Progress Thread\r\n");

	 _endthread();
 
    return;
}

/////////////////////////////////////////////////////////////////////////////
// CStatus dialog

CStatus::CStatus(CWnd* pParent /*=NULL*/)
	: CDialog(CStatus::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStatus)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	showprog = 0; 

	//windowname = "CStatus";
}


void CStatus::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatus)
	DDX_Control(pDX, IDC_PROGRESS1, m_prog);
	DDX_Control(pDX, IDC_TEXT2, m_text2);
	DDX_Control(pDX, IDC_TEXT, m_text);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CStatus, CDialog)
	//{{AFX_MSG_MAP(CStatus)
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatus message handlers

BOOL CStatus::OnInitDialog() 

{
	// Set style to not receive focus

	// CWnd
	//P2N("CStatus::OnInitDialog\r\n");

	CDialog::OnInitDialog();

	//if (bAlpha < 255)
		{
        //  Set layered style for the dialog
        SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);

        //  Call it with 255 as alpha - opacity
		if(g_pSetLayeredWindowAttributes)
			{
			//g_pSetLayeredWindowAttributes(m_hWnd, 0, bAlpha, LWA_ALPHA);
			g_pSetLayeredWindowAttributes(m_hWnd, RGB(0, 255, 0), 0, LWA_COLORKEY);
			}
		}

	// Load bmp for transparent background
    HINSTANCE  hInstResource = NULL;
    
    // Find correct resource handle
    hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(IDB_BITMAP22), RT_BITMAP);
    
    // Load bitmap In
    m_hBitmap = (HBITMAP)::LoadImage(hInstResource, MAKEINTRESOURCE(IDB_BITMAP22),
                                   IMAGE_BITMAP, 0, 0, 0);  
	
	ASSERT(m_hBitmap);

    // Get bitmap size
	BITMAP  csBitmapSize;
  
    int nRetValue = ::GetObject(m_hBitmap, sizeof(csBitmapSize), &csBitmapSize);
    ASSERT(nRetValue);

	m_dwWidth = (DWORD)csBitmapSize.bmWidth;
	m_dwHeight = (DWORD)csBitmapSize.bmHeight;

	//m_text.SetTrans();
	m_text.SetFontSize(-26);
	//m_text.SetBkColor(RGB(0,255,0));

	//m_text2.SetTrans();
	m_text2.SetFontSize(-26);
	//m_text2.SetTextColor(RGB(200, 200, 200));

	m_prog.SetRange(0, 100);
	timer = 2000;

	::GetObject((HFONT)GetStockObject(ANSI_FIXED_FONT), sizeof(m_lf), &m_lf);
	m_lf.lfHeight = -26;

	m_font.CreateFontIndirect(&m_lf);
	//m_font.AssertValid();

	return TRUE;  
	//return false;
}

void CStatus::Show()

{
	if(!IsWindow(m_hWnd))
		{
		Create(IDD);
		//created = true;
		SetWindowPos( NULL, want_x, want_y, 0, 0, SWP_NOSIZE | SWP_NOZORDER );  
		}

	KillTimer(1);

	PAINTSTRUCT Paint;
	
	CDC* dc = BeginPaint( &Paint );
	CFont* oldFont = dc->SelectObject(&m_font);
	textsize = dc->GetTextExtent(text);
	dc->SelectObject(oldFont);
	EndPaint(&Paint);

	//P2N("New text size: xx=%d yy=%d\r\n", ss.cx, ss.cy);

	if(showprog)
		{
		SetWindowPos( NULL, want_x - textsize.cx/2, want_y - textsize.cy/2, 
			textsize.cx + 10, textsize.cy + 50, SWP_NOZORDER | SWP_NOACTIVATE ); 
		}
	else
		{
		SetWindowPos( NULL, want_x - textsize.cx/2, want_y - textsize.cy/2, 
			textsize.cx + 10, textsize.cy + 10, SWP_NOZORDER | SWP_NOACTIVATE ); 
		}
		
	m_prog.SetWindowPos( NULL, 0, 0, 
			textsize.cx - 10, 20, SWP_NOMOVE | SWP_NOZORDER |SWP_NOACTIVATE ); 

	if(showprog)
		{
		m_prog.ShowWindow(SW_SHOW | SWP_NOACTIVATE );
		}
	else	
		{
		// Set timer on no progress
		if(text.GetLength() > 32)			// Longer text needs more time to read
			SetTimer(1, timer, NULL);
		else
			SetTimer(1, timer * 2, NULL);

		m_prog.ShowWindow(SW_HIDE);
		}

	m_prog.SetPos(0);
	
	// Only start if progress is visible
	if(showprog)
		{
		_beginthread(RunProgressThread, 0, this);
		}

	// Instead of showing it, set timer to show it delayed
	// This allows fast paced timers to cancel out
	//SetTimer(2, 100, NULL);

	ShowWindow(SW_SHOWNOACTIVATE);
}

//////////////////////////////////////////////////////////////////////////

void CStatus::OnTimer(UINT nIDEvent) 

{
	//P2N("CStatus::OnTimer %d\r\n", nIDEvent);

	KillTimer(nIDEvent);
			
	switch(nIDEvent)
		{
		case 1:
			showprog = 0;
			ShowWindow(SW_HIDE);
			break;

		//case 2:
		//	ShowWindow(SW_SHOWNOACTIVATE);
		//	break;
		}

	CDialog::OnTimer(nIDEvent);
}

void CStatus::Show(const char *str, int tme)

{
	timer = tme;
	text  = str;
	Show();
}

void CStatus::Hide()

{
	if(!IsWindow(m_hWnd))
		return;

	KillTimer(1);
	KillTimer(2);

	ShowWindow(SW_HIDE);
	support.YieldToWinEx(m_hWnd);
}

BOOL CStatus::OnEraseBkgnd(CDC* pDC) 

{
	BOOL bRetValue = CDialog::OnEraseBkgnd(pDC);

	if (!m_hBitmap)
        return bRetValue;

	CRect   rect;
    GetClientRect(rect);
    
    CDC dc;
    dc.CreateCompatibleDC(pDC);

    HBITMAP    pbmpOldBmp = NULL;

    pbmpOldBmp = (HBITMAP)::SelectObject(dc.m_hDC, m_hBitmap);
    
	int ixOrg, iyOrg;

	for (iyOrg = 0; iyOrg < rect.Height(); iyOrg += m_dwHeight)
        {
		for (ixOrg = 0; ixOrg < rect.Width(); ixOrg += m_dwWidth)
            {
			pDC->BitBlt (ixOrg, iyOrg, rect.Width(), rect.Height(), &dc, 0, 0, SRCCOPY);
            }
        }

    ::SelectObject(dc.m_hDC, m_hBitmap);

	return bRetValue;

	//return CDialog::OnEraseBkgnd(pDC);
}

void CStatus::OnLButtonDown(UINT nFlags, CPoint point) 

{
	//if(!showprog)
	//	Hide();

	//P2N("Left butt on status\r\n");

	CDialog::OnLButtonDown(nFlags, point);
}

void CStatus::OnPaint() 

{
	CPaintDC dc(this); // device context for painting

	CFont* oldFont = dc.SelectObject(&m_font);

	dc.SetBkMode(TRANSPARENT);

	dc.SetTextColor(RGB(255,255,255));	
	dc.TextOut(0, 0, text);

	dc.SetTextColor(RGB(255,255,255));	
	dc.TextOut(0, 1, text);

	dc.SetTextColor(RGB(0,0,0));
	dc.TextOut(1, 2, text);

	dc.SetTextColor(RGB(0,0,0));
	dc.TextOut(1, 3, text);

	dc.SelectObject(oldFont);

	// Do not call CDialog::OnPaint() for painting messages
}

void CStatus::CenterStatus(CWnd *wnd)

{
	RECT rc; wnd->GetWindowRect(&rc);

	//P2N("CStatus::CenterStatus top=%d left=%d right=%d bottom=%d\r\n", rc.top, rc.left, rc.right, rc.bottom);

	want_x = (rc.right - rc.left) / 2;
	want_y = (rc.bottom - rc.top) / 4;

	//P2N("CStatus::CenterStatus want_x=%d want_y=%d\r\n", want_x, want_y);

	if(!IsWindow(m_hWnd))
		{
		Create(IDD);
		SetWindowPos( NULL, want_x, want_y, 0, 0, SWP_NOSIZE | SWP_NOZORDER );  
		}
}

LRESULT CStatus::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 

{
#if 0
	switch(message)
		{
		case WM_NCACTIVATE:
			return 1;
			break;

		case WM_SETFOCUS:
			return 0;
			break;

		case WM_ACTIVATE:
			return 0;
			break;
		}
#endif

	//P2N("CStatus::WindowProc %s\r\n", mxpad.num2msg(message));

	return CDialog::WindowProc(message, wParam, lParam);
}

