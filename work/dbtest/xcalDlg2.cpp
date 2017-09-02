// xcalDlg2.cpp : implementation file
//

#include "stdafx.h"
#include "dbtest.h"
//include "about.h"
#include "xcalDlg2.h"
#include "mxpad.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//CSplitterWnd spl;


/////////////////////////////////////////////////////////////////////////////
// CXcalDlg dialog

CXcalDlg::CXcalDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CXcalDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CXcalDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	P2N("CXcalDlg::CXcalDlg\r\n");
}

void CXcalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXcalDlg)
	DDX_Control(pDX, IDC_SPLIT, m_split);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CXcalDlg, CDialog)
	//{{AFX_MSG_MAP(CXcalDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_XC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_XC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_XC_BUTTON3, OnButton3)
	ON_WM_SIZING()
	ON_WM_GETMINMAXINFO()
	ON_BN_CLICKED(IDC_XC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_XC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_XC_BUTTON6, OnButton6)
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXcalDlg message handlers

BOOL CXcalDlg::OnInitDialog()

{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
		
	//SIZE sizeMin; sizeMin.cx = 100; sizeMin.cy = 100;
	//spl.Create(this, 2, 2, sizeMin, NULL, WS_VISIBLE |WS_HSCROLL | WS_VSCROLL | SPLS_DYNAMIC_SPLIT);

	P2N("CXcalDlg::OnInitDialog\r\n");

	// CWnd
	//WINDOWPLACEMENT wp;
	//GetWindowPlacement(&wp);
	//m_split.SetWindowPlacement(&wp);

	m_split.ShowWindow(SW_SHOW);

	//m_split.SetFocus();
	//m_split.SetActiveWindow();

	RECT rc; GetClientRect(&rc);

	// Geometry for buttons
	WINDOWPLACEMENT wp;
	
	GetDlgItem(IDOK)->GetWindowPlacement(&wp);
	ok_x = rc.right - wp.rcNormalPosition.left;	
	ok_y = rc.bottom - wp.rcNormalPosition.top;	

	GetDlgItem(IDCANCEL)->GetWindowPlacement(&wp);
	can_x = rc.right - wp.rcNormalPosition.left;	
	can_y = rc.bottom - wp.rcNormalPosition.top;	

	GetDlgItem(IDC_XC_BUTTON1)->GetWindowPlacement(&wp);
	b1_x = rc.right - wp.rcNormalPosition.left;	
	b1_y = rc.bottom - wp.rcNormalPosition.top;	

	GetDlgItem(IDC_XC_BUTTON2)->GetWindowPlacement(&wp);
	b2_x = rc.right - wp.rcNormalPosition.left;	
	b2_y = rc.bottom - wp.rcNormalPosition.top;	

	GetDlgItem(IDC_XC_BUTTON3)->GetWindowPlacement(&wp);
	b3_x = rc.right - wp.rcNormalPosition.left;	
	b3_y = rc.bottom - wp.rcNormalPosition.top;	

	GetDlgItem(IDC_XC_BUTTON4)->GetWindowPlacement(&wp);
	b4_x = rc.right - wp.rcNormalPosition.left;	
	b4_y = rc.bottom - wp.rcNormalPosition.top;	

	GetDlgItem(IDC_XC_BUTTON5)->GetWindowPlacement(&wp);
	b5_x = rc.right - wp.rcNormalPosition.left;	
	b5_y = rc.bottom - wp.rcNormalPosition.top;	

	GetDlgItem(IDC_XC_BUTTON6)->GetWindowPlacement(&wp);
	b6_x = rc.right - wp.rcNormalPosition.left;	
	b6_y = rc.bottom - wp.rcNormalPosition.top;	

	GetDlgItem(IDC_LAB)->GetWindowPlacement(&wp);
	l1_x = rc.right - wp.rcNormalPosition.left;	
	l1_y = rc.bottom - wp.rcNormalPosition.top;	

#if 0
	// Test fill cal
	m_split.SetItem(3, 12, 0, "Hello", 10);
	m_split.SetItem(3, 14, 0, "Alarm at never neverland", 12);
	m_split.SetItem(2, 10, 0, "Make the alarm yours", 14);	
	m_split.SetItem(2, 12, 1, "Split neverland", 15);
	m_split.SetItem(4, 13, 0, "alarm up yours", 34);	
	m_split.SetItem(4, 13, 1, "Split up neverland", 45);

	//COleDateTime  ddd;
	//ddd = COleDateTime::GetCurrentTime();
	//m_split.SetItem(ddd, "Currnt entry", 45);

	//COleDateTimeSpan tstime(1, 0, 20, 0);
	//ddd  += tstime;
	//m_split.SetItem(ddd, "tomarrrow entry", 45);

#endif
	
	//P2N("CXcalDlg::OnSize S2C L:%d T:%d R:%d B:%d\r\n", wp.rcNormalPosition.left,wp.rcNormalPosition.top, 
	//				wp.rcNormalPosition.right, wp.rcNormalPosition.bottom);

	// Initial resize for controls to follow
	Resize();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CXcalDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		//CAboutDlg dlgAbout;
		//dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CXcalDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CXcalDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CXcalDlg::OnSize(UINT nType, int cx, int cy) 

{
	//P2N("New Size cx=%d cy=%d\r\n", cx, cy);
	CDialog::OnSize(nType, cx, cy);
	Resize();	
}

// Move controls and children according to rules:

void CXcalDlg::Resize()

{
	if(::IsWindow(m_split.m_hWnd))
		{
		// Examine size limitations
		WINDOWPLACEMENT wp;
		GetWindowPlacement(&wp);
#if 0		
		if((wp.rcNormalPosition.right -  wp.rcNormalPosition.left) < 450)
			{
			wp.rcNormalPosition.right =  wp.rcNormalPosition.left + 450;
			SetWindowPlacement(&wp);
			}
		if((wp.rcNormalPosition.bottom -  wp.rcNormalPosition.top) < 280)
			{
			wp.rcNormalPosition.bottom =  wp.rcNormalPosition.top + 280;
			SetWindowPlacement(&wp);
			}
#endif

		// Resize and move client windows to follow
		RECT rc; GetClientRect(&rc);
	
		m_split.SetWindowPos(NULL, 4,4, rc.right - 8, rc.bottom - (ok_y + 10), SWP_NOZORDER);
	
		GetDlgItem(IDOK)->SetWindowPos(NULL, rc.right - ok_x,  rc.bottom - ok_y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		GetDlgItem(IDCANCEL)->SetWindowPos(NULL, rc.right - can_x,  rc.bottom - can_y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		GetDlgItem(IDC_XC_BUTTON1)->SetWindowPos(NULL, rc.right - b1_x,  rc.bottom - b1_y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		GetDlgItem(IDC_XC_BUTTON2)->SetWindowPos(NULL, rc.right - b2_x,  rc.bottom - b2_y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		GetDlgItem(IDC_XC_BUTTON3)->SetWindowPos(NULL, rc.right - b3_x,  rc.bottom - b3_y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		GetDlgItem(IDC_XC_BUTTON4)->SetWindowPos(NULL, rc.right - b4_x,  rc.bottom - b4_y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		GetDlgItem(IDC_XC_BUTTON5)->SetWindowPos(NULL, rc.right - b5_x,  rc.bottom - b5_y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		GetDlgItem(IDC_XC_BUTTON6)->SetWindowPos(NULL, rc.right - b6_x,  rc.bottom - b6_y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		GetDlgItem(IDC_LAB    )->SetWindowPos(NULL, rc.right - l1_x,  rc.bottom - l1_y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		
		Invalidate();
		}
}

void CXcalDlg::OnButton1() 
{
	m_split.SetStartHour(m_split.starthour + 1);
}

void CXcalDlg::OnButton2() 
{
	m_split.SetStartHour(m_split.starthour - 1);
}

//////////////////////////////////////////////////////////////////////////

void CXcalDlg::OnButton3() 
{
	P2N("CXcalDlg::OnButton3\r\n");	
	m_split.DefHours();
}

void CXcalDlg::OnSizing(UINT fwSide, LPRECT pRect) 
{
	CDialog::OnSizing(fwSide, pRect);
	
	//P2N("CXcalDlg::OnSizing S2C L:%d T:%d R:%d B:%d\r\n", pRect->left, pRect->top, 
	//				pRect->right, pRect->bottom);
}

void CXcalDlg::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 

{
	lpMMI->ptMinTrackSize.x = 450;
	lpMMI->ptMinTrackSize.y = 280;

	CDialog::OnGetMinMaxInfo(lpMMI);
}

void CXcalDlg::OnButton4() 

{
	COleDateTime dd = m_split.date;
	dd += 7 * m_split.oneday;
	m_split.SetNewDate(&dd);
}

void CXcalDlg::OnButton5() 

{
	COleDateTime dd = m_split.date;
	dd -= 7 * m_split.oneday;
	m_split.SetNewDate(&dd);
}

void CXcalDlg::OnButton6() 

{
	COleDateTime today = COleDateTime::GetCurrentTime();
	m_split.SetNewDate(&today);
}

void CXcalDlg::SetItem(int day, int hour, const char *str)
{

	
}

BOOL CXcalDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 

{
	
	//P2N("CXcalDlg::OnMouseWheel\r\n");

	// Reflect this message  grid
	if(GetFocus() != &m_split)
		{
		//P2N("CXcalDlg::OnMouseWheel -- non focus\r\n");
		m_split.DoMouseWheel(nFlags, zDelta, pt);
		return true;
		}

	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}
