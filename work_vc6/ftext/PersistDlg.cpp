
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

#include "stdafx.h"
#include "ftext.h"
#include "PersistDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString PersistDlg::section("Positions");

CxPtrArr PersistDlg::modals;

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
	//AP2N("PersistDlg::InitVars() %p %s\r\n", this, windowname);

	bAlpha = 220;

	magic = PERSIST_MAGIC;

	pre_want_x = pre_want_y = want_x  = want_y = 0;
	want_w = want_h = -1;
	rdown = destroyed = shown = 0;
	showstate = pre_showstate = false;
	posload = modal = m_centerdlg = false;

	xdrag = GetSystemMetrics(SM_CXDRAG);
	ydrag = GetSystemMetrics(SM_CYDRAG);
	noremember = false;
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
	ON_WM_PAINT()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PersistDlg message handlers

BOOL PersistDlg::OnInitDialog() 

{
	CDialog::OnInitDialog();

	//P2N("PersistDlg::OnInitDialog() IDD=%d\r\n", xidd);
	
	SetAlpha(bAlpha);

	// Restore old placement, if any
	WINDOWPLACEMENT wp2;
	
	CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);

	if(dlg->resetpositions)
		{
		if(m_centerdlg)
			{
			CenterWindow();

			GetWindowPlacement(&wp2);

			pre_want_x = wp2.rcNormalPosition.left;
			pre_want_y = wp2.rcNormalPosition.top;
			pre_want_w = wp2.rcNormalPosition.right  - wp2.rcNormalPosition.left;
			pre_want_h = wp2.rcNormalPosition.bottom - wp2.rcNormalPosition.top;
			}
		else
			{
			// Reset, do nothing
			}
		}
	else
		{
		CString entity; entity.Format("DlgPos%d", xidd);

		CString str2 = AfxGetApp()->GetProfileString(section, entity, "");
		
		if(str2 != "")
			{
			// Position acq from registry
			posload = true;
			
			//P2N("PersistDlg::LoadPos IDD=%d pos='%s' name='%s'\r\n", xidd, str2, windowname);

			sscanf(str2, "%d %d %d %d",
				&wp2.rcNormalPosition.top, &wp2.rcNormalPosition.left,
					&wp2.rcNormalPosition.bottom,  &wp2.rcNormalPosition.right);

			// Save old positions as pre_wants
			pre_want_x = wp2.rcNormalPosition.left;
			pre_want_y = wp2.rcNormalPosition.top;
			pre_want_w = wp2.rcNormalPosition.right  - wp2.rcNormalPosition.left;
			pre_want_h = wp2.rcNormalPosition.bottom - wp2.rcNormalPosition.top;
			}
		else 
			{
			if(m_centerdlg)
				{
				CenterWindow();
				GetWindowPlacement(&wp2);

				want_x = wp2.rcNormalPosition.left;
				want_y = wp2.rcNormalPosition.top;
				want_w = wp2.rcNormalPosition.right  - wp2.rcNormalPosition.left;
				want_h = wp2.rcNormalPosition.bottom - wp2.rcNormalPosition.top;
				}
			}

		//P2N("Load Visibility flag: %s\r\n", tmp2);

		CString entity2; entity2.Format("DlgShow%d", xidd);
		CString tmp2 = AfxGetApp()->GetProfileString(section, entity2, "1");
		pre_showstate = atoi(tmp2);
		}

	// Execute the calculated dimentsions
	
	int flag =  SWP_NOZORDER;

	if(posload)
		{
		want_x = pre_want_x;
		want_y = pre_want_y;

		want_w = pre_want_w;
		want_h = pre_want_h;
		}

	if(want_w < 0 || want_h < 0)
			flag |= SWP_NOSIZE ;
		
	if(want_x < 0 || want_y < 0)
		flag |= SWP_NOMOVE ;	

	if(modal)
		flag |= SWP_NOSIZE;	

	if(!noremember)
		{
		SetWindowPos(NULL, want_x, want_y, want_w, want_h,  flag);  
		}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// Save old positions and visibility

void PersistDlg::OnDestroy() 

{
	//P2N("PersistDlg::OnDestroy() %p  IDD=%d name: '%s'\r\n", this, xidd, windowname);

	destroyed = true;

	WINDOWPLACEMENT wp2;
	GetWindowPlacement(&wp2);

	int hh2  = wp2.rcNormalPosition.bottom - wp2.rcNormalPosition.top;	
	int ww2  = wp2.rcNormalPosition.right - wp2.rcNormalPosition.left;

	CString tmp; tmp.Format("%d %d %d %d",
		wp2.rcNormalPosition.top, wp2.rcNormalPosition.left,
				wp2.rcNormalPosition.bottom,  wp2.rcNormalPosition.right);	

	
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
	//P2N("PersistDlg::PreTranslateMessage %p %s\r\n", this, mxpad.num2msg(pMsg->message));

	// ReRoute messages to main window
	if(pMsg->message == WM_CHAR)
		{
		CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);
		dlg->PostMessage(pMsg->message, pMsg->wParam, pMsg->lParam);

		//P2N("PersistDlg pretrans WM_CHAR %c\r\n", pMsg->wParam);
		return(0);
		}

	if(pMsg->message == WM_KEYDOWN)
		{
		CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);
		dlg->PostMessage(pMsg->message, pMsg->wParam, pMsg->lParam);

		//P2N("PersistDlg::PreTrans WM_KEYUP %c (%d) %x\r\n", pMsg->wParam, pMsg->wParam, pMsg->lParam);
		return(0);
		}

	if(pMsg->message == WM_KEYUP)
		{
		CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);
		dlg->PostMessage(pMsg->message, pMsg->wParam, pMsg->lParam);

		//P2N("PersistDlg::PreTrans WM_KEYUP %c (%d) %x\r\n", pMsg->wParam, pMsg->wParam, pMsg->lParam);
		return(0);
		}

	if(pMsg->message == WM_SYSCHAR)
		{
		CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);
		dlg->PostMessage(pMsg->message, pMsg->wParam, pMsg->lParam);

		//P2N("PersistDlg::PreTrans WM_SYSCHAR %c (%d) %x\r\n", pMsg->wParam, pMsg->wParam, pMsg->lParam);
		return(0);
		}

	if(pMsg->message == WM_SYSKEYDOWN)
		{
		CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);
		dlg->PostMessage(pMsg->message, pMsg->wParam, pMsg->lParam);

		//P2N("PersistDlg::PreTrans WM_SYSKEYDOWN %c (%d) %x\r\n", pMsg->wParam, pMsg->wParam, pMsg->lParam);
		return(0);
		}

	if(pMsg->message == WM_SYSKEYUP)
		{
		CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);
		dlg->PostMessage(pMsg->message, pMsg->wParam, pMsg->lParam);

		//P2N("PersistDlg::PreTrans WM_SYSKEYUP %c (%d) %x\r\n", pMsg->wParam, pMsg->wParam, pMsg->lParam);
		return(0);
		}

	return CDialog::PreTranslateMessage(pMsg);
}

void PersistDlg::OnShowWindow(BOOL bShow, UINT nStatus) 

{
	CDialog::OnShowWindow(bShow, nStatus);
	//P2N("PersistDlg::OnShowWindow %d %d\r\n", bShow, nStatus);
	showstate = bShow;	
}

void PersistDlg::OnPaint() 

{
	//CPaintDC dc(this); // device context for painting
	// Do not call CDialog::OnPaint() for painting messages
	// BLAH BLAH ...

	CDialog::OnPaint();
}


void PersistDlg::Show(int center)

{
	//P2N("PersistDlg::Show wnd=%p wname='%s' xidd=%d\r\n", m_hWnd, windowname, xidd);

	ASSERT(magic == PERSIST_MAGIC);
	
	xCreate();
	
	if(IsWindowVisible())
		{
		SetActiveWindow();
		return;
		}
	if(center)
		{
		CenterWindow();
		}

	// Finally, ready to show
	ShowWindow(SW_SHOW);
	SetActiveWindow();

	shown = true;
}

void PersistDlg::Hide(int ret)

{
	ASSERT(magic == PERSIST_MAGIC);
	
	//P2N("PersistDlg::Hide() %s\r\n", windowname);

	// If we never used this window, ignore request
	if(!m_hWnd || !::IsWindow(m_hWnd))
		{
		return;
		}

	// Terminate with the appropriate function:
	if(modal)
		{	
		//P2N("PersistDlg::Hide() MODAL\r\n");
		//modal = false;
		//((CShadesApp*)AfxGetApp())->inmodal--;

		// Will call 
		//EndDialog(ret);
		
		if(ret == IDOK)
			OnOK();
		else
			OnCancel();
		}
	else
		{
		ShowWindow(SW_HIDE);
		}
}

void PersistDlg::OnRButtonDown(UINT nFlags, CPoint point) 

{
	SetCapture();	rdown = true;
	CDialog::OnRButtonDown(nFlags, point);
}

void PersistDlg::OnRButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();	rdown = false;
	CDialog::OnRButtonUp(nFlags, point);
}

static int oldAlpha;


void PersistDlg::OnMouseMove(UINT nFlags, CPoint point) 

{
	static CPoint oldpoint;
	
	// Adjust transparency with right drag
	if(rdown)
		{
		if(oldpoint.y > point.y)
			bAlpha++;
		if(oldpoint.y < point.y)
			bAlpha--;

		if(oldpoint.x > point.x)
			bAlpha++;
		if(oldpoint.x < point.x)
			bAlpha--;
		
		// Limit alpha
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

int PersistDlg::DoModal()

{
	shown = true;
	int ret = 0;

	//P2N("PersistDlg::DoModal '%s'\r\n", windowname);
	
	SetAlpha(245);

	m_centerdlg = true;

	((CShadesApp*)AfxGetApp())->inmodal++;
	
	modals.AddUniq(this);

	modal++; // = true;
	ret = CDialog::DoModal();
	modal--; // = false;

	modals.RemoveValue(this);

	m_centerdlg = false;

	((CShadesApp*)AfxGetApp())->inmodal--;
	
	P2N("PersistDlg::Done Modal '%s'\r\n", windowname);

	return ret;
}

PersistDlg::~PersistDlg()

{
	// Release modal loop if not released already
	//P2N("PersistDlg::Destroy() '%s'\r\n", windowname);

	if(modal)
		{
		P2N("Forced Destruction from ~PersistDlg\r\n");

		//((CShadesApp*)AfxGetApp())->inmodal--;
		modal = false;
		CDialog::EndDialog(IDOK);
		}

	if(!destroyed && shown)
		{
		//P2N("PersistDlg::~PersistDlg() Not Destroyed! %p iswnd=%d title: '%s'\r\n", 
		//				this, IsWindow(m_hWnd), windowname);
		}
}	

void PersistDlg::SetAlpha(int alpha)

{
	bAlpha = alpha;
	oldAlpha = bAlpha;
	
	if (bAlpha == 255)
		{
		 // UnSet layered style for the dialog
	    SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) & ~WS_EX_LAYERED);
		}
	else
		{
		 // Set layered style for the dialog
	    SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);

        //  Call it with alpha - opacity
		if(g_pSetLayeredWindowAttributes)
			g_pSetLayeredWindowAttributes(m_hWnd, 0, bAlpha, LWA_ALPHA);
		}
}

void PersistDlg::xCreate()

{
	if(!::IsWindow(m_hWnd))
		{
		//P2N("PersistDlg::xCreate() %p '%s' want_x=%d want_y=%d xidd=%d\r\n", 
		//				this, windowname, want_x, want_y, xidd);
		Create(xidd);
		}
}


void PersistDlg::SetCenterFlag(int flag)
{
	m_centerdlg = flag;
}
