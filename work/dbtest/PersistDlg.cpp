
/* =====[ PersistDlg.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the PersistDlg.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  11/7/2007  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// PersistDlg.cpp : implementation file
//

#include "stdafx.h"
#include "dbtest.h"
#include "mxpad.h"
#include "PersistDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PersistDlg dialog

PersistDlg::PersistDlg(CWnd* pParent /*=NULL*/)
	: CDialog(PersistDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(PersistDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	xidd = PersistDlg::IDD;
	InitVars();
}

PersistDlg::PersistDlg(UINT nIDTemplate, CWnd* pParentWnd)
	: CDialog(nIDTemplate, pParentWnd)

{
	xidd	= 	nIDTemplate;
	InitVars();
}

void PersistDlg::InitVars()

{
	bAlpha = 240;
	pre_want_x = 0;
	pre_want_y = 0;
	showstate = false;
	pre_showstate = 0;
	noheader = false;
	posload = false;
	rdown = 0;
}

void PersistDlg::DoDataExchange(CDataExchange* pDX)

{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PersistDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(PersistDlg, CDialog)
	//{{AFX_MSG_MAP(PersistDlg)
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

#define WS_EX_LAYERED 0x00080000 
#define LWA_COLORKEY 1 // Use color as the transparency color.
#define LWA_ALPHA    2 // Use bAlpha to determine the opacity of the layer

//  Function pointer for lyering API in User32.dll
typedef BOOL (WINAPI *lpfnSetLayeredWindowAttributes)
            (HWND hWnd, COLORREF cr, BYTE bAlpha, DWORD dwFlags);

lpfnSetLayeredWindowAttributes g_pSetLayeredWindowAttributes = NULL;

/////////////////////////////////////////////////////////////////////////////
// PersistDlg message handlers

BOOL PersistDlg::OnInitDialog() 

{
	CDialog::OnInitDialog();

    HMODULE hUser32 = GetModuleHandle(_T("USER32.DLL"));
	g_pSetLayeredWindowAttributes = (lpfnSetLayeredWindowAttributes)
                        GetProcAddress(hUser32, "SetLayeredWindowAttributes");

	//  Set layered style for the dialog
    SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);

	if (bAlpha < 255)
		{
        //  Call it with 255 as alpha - opacity
		if(g_pSetLayeredWindowAttributes)
			g_pSetLayeredWindowAttributes(m_hWnd, 0, bAlpha, LWA_ALPHA);
		}

	// Restore old placement, if any
	WINDOWPLACEMENT wp2;
	GetWindowPlacement(&wp2);

#if 0
	CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);

	if(!dlg->reset)
		{
		CString section("Positions");
		CString entity; entity.Format("DlgPos%d", xidd);

		CString str2 = AfxGetApp()->GetProfileString(section, entity, "");
		
		if(str2 != "")
			{
			// Position acq from registry
			posload = true;
			
			//P2N("PersistDlg::LoadPos IDD=%d Pos=%s\r\n", xidd, str2);

			sscanf(str2, "%d %d %d %d",
				&wp2.rcNormalPosition.top, &wp2.rcNormalPosition.left,
					&wp2.rcNormalPosition.bottom,  &wp2.rcNormalPosition.right);

			//SetWindowPos(NULL, wp2.rcNormalPosition.left, wp2.rcNormalPosition.top, 0, 0, SWP_NOSIZE  | SWP_NOZORDER );

			// Remember old positions as pre_wants
			pre_want_x = wp2.rcNormalPosition.left;
			pre_want_y = wp2.rcNormalPosition.top;
			pre_want_w = wp2.rcNormalPosition.right  - wp2.rcNormalPosition.left;
			pre_want_h = wp2.rcNormalPosition.bottom - wp2.rcNormalPosition.top;
			}

		CString entity2; entity2.Format("DlgShow%d", xidd);
		CString tmp2 = AfxGetApp()->GetProfileString(section, entity2, "1");

		//P2N("Load Visibility %s\r\n", tmp2);

		pre_showstate = atoi(tmp2);
		}
#endif

	//P2N("PersistDlg::OnInitDialog() IDD=%d\r\n", xidd);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void PersistDlg::OnDestroy() 

{
	//P2N("PersistDlg::OnDestroy()  IDD=%d\r\n", xidd);

	WINDOWPLACEMENT wp2;
	GetWindowPlacement(&wp2);

	int hh2  = wp2.rcNormalPosition.bottom - wp2.rcNormalPosition.top;	
	int ww2  = wp2.rcNormalPosition.right - wp2.rcNormalPosition.left;

	CString tmp; tmp.Format("%d %d %d %d",
		wp2.rcNormalPosition.top, wp2.rcNormalPosition.left,
				wp2.rcNormalPosition.bottom,  wp2.rcNormalPosition.right);	

	CString section("Positions");
	CString entity; entity.Format("DlgPos%d", xidd);
	
	//P2N("PersistDlg::SavePos  IDD=%d Pos= '%s'\r\n", xidd, tmp);
	
	AfxGetApp()->WriteProfileString(section, entity, tmp);

	CString tmp2; tmp2.Format("%d", showstate);

	//P2N("OnDestroy Showstate %s\r\n", tmp2);

	CString entity2; entity2.Format("DlgShow%d", xidd);
	AfxGetApp()->WriteProfileString(section, entity2, tmp2);
	
	CDialog::OnDestroy();	
}


BOOL PersistDlg::PreTranslateMessage(MSG* pMsg) 

{
	if(pMsg->message == WM_CHAR)
		{
		CWnd *dlg = ((CWnd* )AfxGetApp()->m_pMainWnd);
		dlg->SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);

		P2N("PersistDlg pretrans Char %c\r\n", pMsg->wParam);
		return(0);
		}

	if(pMsg->message == WM_KEYUP)
		{
		CWnd *dlg = ((CWnd* )AfxGetApp()->m_pMainWnd);
		dlg->SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);

		//P2N("mag pretrans keyup %c (%d) %x\r\n", pMsg->wParam, pMsg->wParam, pMsg->lParam);
		return(0);
		}

	if(pMsg->message == WM_SYSKEYDOWN)
		{
		CWnd *dlg = ((CWnd* )AfxGetApp()->m_pMainWnd);
		dlg->SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);

		//P2N("mag pretrans syskeydown %c (%d) %x\r\n", pMsg->wParam, pMsg->wParam, pMsg->lParam);
		//return(0);
		}

	if(pMsg->message == WM_SYSKEYUP)
		{
		CWnd *dlg = ((CWnd* )AfxGetApp()->m_pMainWnd);
		dlg->SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);

		//P2N("mag pretrans syskeyup %c (%d) %x\r\n", pMsg->wParam, pMsg->wParam, pMsg->lParam);
		//return(0);
		}
		
	return CDialog::PreTranslateMessage(pMsg);
}

void PersistDlg::OnShowWindow(BOOL bShow, UINT nStatus) 

{
	CDialog::OnShowWindow(bShow, nStatus);
	//P2N("PersistDlg::OnShowWindow %d %d\r\n", bShow, nStatus);
	showstate = bShow;	
}

void PersistDlg::OnClose() 

{
	//P2N("PersistDlg::OnClose()\r\n");	
	CDialog::OnClose();
}


void PersistDlg::OnPaint() 

{
	//CPaintDC dc(this); // device context for painting
	// Do not call CDialog::OnPaint() for painting messages

	CDialog::OnPaint();
}

// CDialog CWnd

void PersistDlg::Show()

{
	if(!::IsWindow(m_hWnd))
		{
		Create(xidd);
		}

	if(IsWindowVisible())
		{
		SetActiveWindow();

		//NextDlgCtrl();
		//SetFocus();
		//GotoDlgCtrl(GetDlgItem(GetDefID()));

		return;
		}

	ShowWindow(SW_SHOW);
	SetActiveWindow();
	//SetFocus();		
}

void PersistDlg::Hide()

{
	// Never used, ignote request
	if(!::IsWindow(m_hWnd))
		{
		//Create(xidd);
		return;
		}
	ShowWindow(SW_HIDE);
}

void PersistDlg::OnRButtonDown(UINT nFlags, CPoint point) 
{
	SetCapture();
	rdown = true;
	CDialog::OnRButtonDown(nFlags, point);
}

void PersistDlg::OnRButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();
	rdown = false;
	CDialog::OnRButtonUp(nFlags, point);
}

void PersistDlg::OnMouseMove(UINT nFlags, CPoint point) 

{
	static CPoint oldpoint;
	static int oldAlpha;

	// Adjust transparency with right drag
	if(rdown)
		{
		if(oldpoint.y > point.y)
			{
			bAlpha++;
			}
		if(oldpoint.y < point.y)
			{
			bAlpha--;
			}
	
		if(oldpoint.x > point.x)
			{
			bAlpha++;
			}
		if(oldpoint.x < point.x)
			{
			bAlpha--;
			}
		

		bAlpha = max(bAlpha, 50);
		bAlpha = min(bAlpha, 255);

		if(bAlpha != oldAlpha)
			{
			//  Call it with the new alpha - opacity
			if(g_pSetLayeredWindowAttributes)
				g_pSetLayeredWindowAttributes(m_hWnd, 0, bAlpha, LWA_ALPHA);

			Invalidate();
			}
		oldAlpha = bAlpha;
		oldpoint = point;
		}
	CDialog::OnMouseMove(nFlags, point);
}
