
/* =====[ Splash.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the Splash.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  3/9/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// Splash.cpp : implementation file
//

#include "stdafx.h"
#include "xraynotes.h"

#include "shades.h"
#include "Splash.h"
//include "MaskBlt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// Params for splash window in milliseconds

#define SPLASH_MSEC 1400
#define TIMER_MSEC	20

/////////////////////////////////////////////////////////////////////////////
// CSplash dialog

CSplash::CSplash(CWnd* pParent /*=NULL*/)
	: CDialog(CSplash::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSplash)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_anim = true;
	addon = elapsed_time = 0;
	alpha = 10;
}

void CSplash::DoDataExchange(CDataExchange* pDX)

{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSplash)
	DDX_Control(pDX, IDC_DEMO, m_demo);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSplash, CDialog)
	//{{AFX_MSG_MAP(CSplash)
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_SYSKEYDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSplash message handlers

//////////////////////////////////////////////////////

void CSplash::OnTimer(UINT nIDEvent) 

{
	elapsed_time += TIMER_MSEC;

	if(elapsed_time > SPLASH_MSEC + addon)
		Hide();

	// Wait for the window to appear ...
	//while(true)
	//	{
	//	if(IsWindow(m_hWnd))
	//		break;
	//
	//		support.YieldToWinEx();
	//		Sleep(10);
	//		}


	// Cheat for logarithmic feel
	if(alpha > 120)
		alpha += 4;
	else if(alpha > 60)
		alpha += 2;
	else
		alpha += 1;

	// Set only if it is not already 255 (less processor needed)
	if(alpha <= 255)
		{
		Invalidate();
		}
	else
		{
		//KillTimer(0);
		}

	//	if(g_pSetLayeredWindowAttributes)
	//		{
	//		//g_pSetLayeredWindowAttributes(m_hWnd, 0, alpha, LWA_ALPHA);
	//		}

	//P2N("CSplash::OnTimer Alpha %d\r\n", alpha );
	
	CDialog::OnTimer(nIDEvent);
}

////////////////////////////////////////////////////// 

BOOL CSplash::OnInitDialog() 

{
	CDialog::OnInitDialog();
	
	// Set layered style for the dialog	
	//SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	//g_pSetLayeredWindowAttributes(m_hWnd, 0, 220, LWA_ALPHA);

#if 1
	//P2N("cwd = %s\r\n", support.GetCurrentDir());
	// Dynamic logo image

	if(support.IsFile("logox.jpg"))
		image.Load("logox.jpg");

	if(image.IsValid())
		{
		CClientDC dc(this);	
		
		//P2N("Showing OEM banner\r\n"); 

		CDC  dcMem;
		dcMem.CreateCompatibleDC(&dc);
		
		m_nWidth = image.GetWidth(); m_nHeight = image.GetHeight();
		m_bitmap.CreateCompatibleBitmap(&dc, m_nWidth, m_nHeight);
		m_bitmap2.CreateCompatibleBitmap(&dc, m_nWidth, m_nHeight);

		CBitmap *pOldBitmap  =  dcMem.SelectObject(&m_bitmap);
		
		//dcMem.FillSolidRect(&rc, RGB(25, 25, 25));

		image.Draw(dcMem.GetSafeHdc());
		dcMem.SelectObject(pOldBitmap);
		
		image2.Load("logo.jpg");

		if(!image2.IsValid())
			P2N("image.IsValid()failed\r\n");

		CBitmap *pOldBitmap2  =  dcMem.SelectObject(&m_bitmap2);
		image2.Draw(dcMem.GetSafeHdc());
		dcMem.SelectObject(pOldBitmap2);		
		}
	else
		{
		// Load bitmap resource
		//P2N("Showing DEFAULT banner\r\n"); 

		m_bitmap.LoadBitmap(IDB_BITMAP14);
		m_bitmap2.LoadBitmap(IDB_BITMAP14);
	
		BITMAP BitMap;	m_bitmap.GetBitmap(&BitMap);	
		m_nWidth = BitMap.bmWidth;
		m_nHeight = BitMap.bmHeight;	
		}
#endif
	

	//void MakeMaskBitmap(
	//CBitmap* pBmpSource, CBitmap* pBmpMask,
	//COLORREF clrpTransColor, // Pass 0xFFFFFFFF if unknown
	//int iTransPixelX,      // = 0
	//int iTransPixelY       // = 0

	//MakeMaskBitmap(&m_bitmap2, &m_bitmap3, 0x0);

	// Adapt it to image ...
	SetWindowPos(NULL, 0, 0, m_nWidth, m_nHeight, 
				SWP_NOMOVE | SWP_NOZORDER | SWP_NOREDRAW);

	CenterWindow();
	
	// Show regitration defaults
#if 0
	CString name = AfxGetApp()->GetProfileString("License", "Name", "Demo Version");
	CString company = AfxGetApp()->GetProfileString("License", "Company", "");

	m_demo.SetTrans();	
	m_demo.CenterText();
	m_demo.SetTextColor(RGB(0, 0, 0));
	m_demo.SetFontSize(-20);

	if(name == "Demo Version")
		{
		int used = support.GetDaysUsed();

		if((int)DEMO_DAYS < used)
			{
			company.Format("Evaluation period expired");
			addon = 1100;
			}
		else
			{
			company.Format("%d days left for evaluation", DEMO_DAYS - used);
			}
		m_demo.SetText(company);


		m_demo.SetWindowPos(NULL, 0, m_nHeight - 28, 0, 0,
			SWP_NOSIZE | SWP_NOZORDER | SWP_NOREDRAW);
		}
#endif

	SetTimer(0, TIMER_MSEC, NULL);
	SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//////////////////////////////////////////////////////////////////////////

BOOL CSplash::OnEraseBkgnd(CDC* pDC) 

{		

	// Changed to paint in the OnPaint function ...
#if 0
	P2N("CSplash::OnEraseBkgnd aplha = %d\r\n", alpha);
	CDC  dcMem; dcMem.CreateCompatibleDC(pDC);
	CBitmap *pOldBitmap  =  dcMem.SelectObject(&m_bitmap);
	pDC->BitBlt(0, 0, m_nWidth, m_nHeight, &dcMem, 0,0, SRCCOPY); 		
	dcMem.SelectObject(pOldBitmap);
#endif

	// We handled it, no need to draw
	return(true);
}

/// Dismiss after any mouse click

void CSplash::OnLButtonDown(UINT nFlags, CPoint point) 

{
	//P2N("CSplash::OnLButtonDown\r\n");	
	CDialog::OnLButtonDown(nFlags, point);
	Hide();
}

void CSplash::OnRButtonDown(UINT nFlags, CPoint point) 
{
	//P2N("CSplash::OnRButtonDown\r\n");	
	Hide();
	CDialog::OnRButtonDown(nFlags, point);	
}

//////////////////////////////////////////////////////////////////////////
// Dismiss after any keystroke

void CSplash::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 

{
	//P2N("Char %d\r\n", nChar);
	Hide();

	CDialog::OnChar(nChar, nRepCnt, nFlags);
}

void CSplash::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 

{
	//P2N("CSplash::OnKeyDown %c\r\n", nChar);	
	Hide();
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CSplash::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 

{
	//P2N("CSplash::OnSysKeyDown %c\r\n", nChar);	
	Hide();
	switch(nChar)   {
	case 'X':
		//P2N("Alt-x in spash\r\n");
		break;
	}

	CDialog::OnSysKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CSplash::PreTranslateMessage(MSG* pMsg) 

{
#if 1
	switch(pMsg->message)
		{
		case WM_CHAR:
			OnChar(pMsg->wParam, pMsg->lParam & 0xff, pMsg->lParam);
			return true;
		break;
			
		case WM_KEYDOWN:
			OnKeyDown(pMsg->wParam, pMsg->lParam & 0xff, pMsg->lParam);
			break;
		
		case WM_SYSCHAR:
			OnSysChar(pMsg->wParam, pMsg->lParam & 0xff, pMsg->lParam);
			return true;
		break;

		case WM_SYSKEYDOWN:
			OnSysKeyDown(pMsg->wParam, pMsg->lParam & 0xff, pMsg->lParam);
			return true;
		break;
		

		default:
			// None
		break;
		}	
#endif

	return CDialog::PreTranslateMessage(pMsg);
}

void CSplash::Show()

{
	if(!IsWindow(m_hWnd))
		Create(IDD);

	ShowWindow(SW_SHOW);
}

void CSplash::Hide()

{
	if(!IsWindow(m_hWnd))
		Create(IDD);
	
	// Was a bug!
	KillTimer(0);

	ShowWindow(SW_HIDE);
}


void CSplash::OnPaint() 

{
	CPaintDC dc(this); // device context for painting
	
//	P2N("CSplash::OnPaint aplha = %d\r\n", alpha);

	CDC *pDC = &dc;
	CDC  dcMem; dcMem.CreateCompatibleDC(pDC);
	int save = dcMem.SaveDC();

	CBitmap *pOldBitmap  =  dcMem.SelectObject(&m_bitmap);
		
	BLENDFUNCTION m_bf;			// AlphaBlend function parameter
	m_bf.BlendOp	= AC_SRC_OVER;
	m_bf.BlendFlags = 0;
	m_bf.SourceConstantAlpha = MIN(alpha, 255);
	m_bf.AlphaFormat = 0; 

	AlphaBlend(dc, 0,0, m_nWidth, m_nHeight, dcMem, 0, 0, m_nWidth, m_nHeight, m_bf); 	

	//pDC->BitBlt(0, 0, m_nWidth, m_nHeight, &dcMem, 0,0, SRCCOPY); 		

	//	BOOL WINAPI XntMaskBlt(HDC hdcDest, int nXDest, int nYDest, 
	//				int nWidth, int nHeight,
	//					HDC hdcSrc, int nXSrc, int nYSrc,
	//						HBITMAP hbmMask, int xMask, int yMask,
	//							DWORD dwRop);
	//

	//XntMaskBlt(dc, 0, 0, m_nWidth, m_nHeight,
	//					dcMem, 0, 0, 
	//						m_bitmap3, 0, 0, MAKEROP4(SRCCOPY, DSTCOPY) );

							//NULL, 0, 0, SRCCOPY);
	
	dcMem.SelectObject(pOldBitmap);
	dcMem.RestoreDC(save);
	
	// Do not call CDialog::OnPaint() for painting messages
}
