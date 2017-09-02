// vgridDlg.cpp : implementation file
//

#include "stdafx.h"
#include "vgrid.h"
#include "vgridDlg.h"
#include "xraym.h"
#include "mxpad.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVgridDlg dialog

CVgridDlg::CVgridDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVgridDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVgridDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	mode = 0;
	grain = 4;

	for(int loop = 0; loop < 16; loop++)
		{
		red_arr[loop] = RGB(loop * (256/16),0,0);
		green_arr[loop] = RGB(0,loop * (256/16),0);
		blue_arr[loop] = RGB(0, 0, loop * (256/16));
		gray_arr[loop] = RGB(loop * (256/16), loop * (256/16), loop * (256/16));
		}
	
}

void CVgridDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVgridDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CVgridDlg, CDialog)
	//{{AFX_MSG_MAP(CVgridDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_MOVE()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_NEW_SHOWGRIDDEFAULT, OnNewShowgriddefault)
	ON_COMMAND(ID_NEW_SHOWCOLORPATETTE, OnNewShowcolorpatette)
	ON_COMMAND(ID_NEW_SHOWREDGRADIENT, OnNewShowredgradient)
	ON_COMMAND(ID_NEW_COLORGRADIENTS_SHOWGREENGRADIENT, OnNewColorgradientsShowgreengradient)
	ON_COMMAND(ID_NEW_COLORGRADIENTS_SHOWBLUEGRADIENT, OnNewColorgradientsShowbluegradient)
	ON_COMMAND(ID_NEW_SHOWBLAHBLAH, OnNewShowblahblah)
	ON_COMMAND(ID_NEW_SHOWBLEH, OnNewShowbleh)
	ON_COMMAND(ID_NEW_SHOWFINEGRID, OnNewShowfinegrid)
	ON_COMMAND(ID_NEW_SHOWCOARSEGRID, OnNewShowcoarsegrid)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVgridDlg message handlers

BOOL CVgridDlg::OnInitDialog()
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

			pSysMenu->AppendMenu(MF_STRING, IDM_VERT, "Vertical Grid");
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CVgridDlg::OnSysCommand(UINT nID, LPARAM lParam)

{

	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if((nID & 0xFFF0) == IDM_VERT)
	{
		AfxMessageBox("hh");
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVgridDlg::OnPaint() 
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
		RECT rc; GetClientRect(&rc);

		CPaintDC dc(this); // device context for painting

		if(mode == 0)
			{
			for(int loop = 0; loop < rc.right; loop += grain)
				{
				dc.MoveTo(loop, rc.top);
				dc.LineTo(loop, rc.bottom);
				}
			for(int loop2 = 0; loop2 < rc.bottom; loop2 += grain)
				{
				dc.MoveTo(rc.left, loop2);
				dc.LineTo(rc.right, loop2);
				}
			}
		else if(mode == 1)
			{
			int step = rc.right / 16;
			for(int loop = 0; loop < rc.right; loop += step)
				{
				CRect rc2(loop, 0, loop+step, rc.bottom);
				
				//P2N("loop=%d\r\n", loop , (loop+1) );
				
				CBrush br(red_arr[loop/step]);
				dc.FillRect(&rc2, &br);
				}
			}
		else if(mode == 2)
			{
			int step = rc.right / 16;
			for(int loop = 0; loop < rc.right; loop += step)
				{
				CRect rc2(loop, 0, loop+step, rc.bottom);
				
				//P2N("loop=%d\r\n", loop , (loop+1) );
				
				CBrush br(green_arr[loop/step]);
				dc.FillRect(&rc2, &br);
				}
			}
		else if(mode == 3)
			{
			int step = rc.right / 16;
			for(int loop = 0; loop < rc.right; loop += step)
				{
				CRect rc2(loop, 0, loop+step, rc.bottom);				
				CBrush br(blue_arr[loop/step]);
				dc.FillRect(&rc2, &br);
				}
			}
		else if(mode == 4)
			{
			int step = rc.right / 16;
			for(int loop = 0; loop < rc.right; loop += step)
				{
				CRect rc2(loop, 0, loop+step, rc.bottom);				
				CBrush br(gray_arr[loop/step]);
				dc.FillRect(&rc2, &br);
				}
			}
		else if(mode == 5)
			{
			int step = rc.right / 16;

			for(int loop = 0; loop < rc.right; loop += step)
				{
				CRect rc2(loop, 0, loop+step, rc.bottom/4);
				CBrush br2(red_arr[loop/step]);
				dc.FillRect(&rc2, &br2);

				CRect rc3(loop, rc.bottom/4, loop+step, 2*rc.bottom/4);
				CBrush br3(green_arr[loop/step]);
				dc.FillRect(&rc3, &br3);

				CRect rc4(loop, 2*rc.bottom/4, loop+step, 3*rc.bottom/4);
				CBrush br4(blue_arr[loop/step]);
				dc.FillRect(&rc4, &br4);

				CRect rc5(loop, 3*rc.bottom/4, loop+step, 4*rc.bottom/4);
				CBrush br5(gray_arr[loop/step]);
				dc.FillRect(&rc5, &br5);
				}
			}
		else if(mode == 6)
			{
			int radi = min(rc.bottom, rc.right) / 3;
			int centx = rc.right/2, centy = rc.bottom / 2;

			CRect rc2(centx - radi, centy - radi, centx + radi, centy + radi);
			
			CBrush br2(RGB(255,255,255));
			CPen pen(PS_SOLID, 1, RGB(0,0,0));
			dc.SelectObject(pen);
			dc.SelectObject(br2);
	
			dc.Ellipse(&rc2);
			}


		}
	CDialog::OnPaint();		
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CVgridDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CVgridDlg::OnSize(UINT nType, int cx, int cy) 

{
	CDialog::OnSize(nType, cx, cy);
	UpdateTitle();
	Invalidate(false);	
}

void CVgridDlg::UpdateTitle()

{
	RECT rc; GetWindowRect(&rc);

	CString str; str.Format("Vgrid xx=%d yy=%d ww=%d hh=%d", 
					rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);

	SetWindowText(str);
}

void CVgridDlg::OnMove(int x, int y) 

{
	//CDialog::OnMove(x, y);
	UpdateTitle();
}


void CVgridDlg::OnRButtonDown(UINT nFlags, CPoint point) 

{
	P2N("CVgridDlg::OnRButtonDown\r\n");

	CXrayM *omenu = new CXrayM; 

	CPoint pt2(point);	ClientToScreen(&pt2);

	omenu->LoadMenu(IDR_MENU1, 1);
	omenu->Show(pt2.x, pt2.y);
	
	CDialog::OnRButtonDown(nFlags, point);
}

void CVgridDlg::OnNewShowgriddefault() 
{
	mode = 0; grain = 4;
	Invalidate();
	
	//P2N("CVgridDlg::OnNewShowgriddefault\r\n");	
}

void CVgridDlg::OnNewShowcolorpatette() 
{
	mode = 5;
	Invalidate();

	//P2N("CVgridDlg::OnNewShowcolorpatette()\r\n");
}

void CVgridDlg::OnNewShowredgradient() 
{
	mode = 1;
	Invalidate();	
}


void CVgridDlg::OnNewColorgradientsShowgreengradient() 
{
	mode = 2;
	Invalidate();
	
}

void CVgridDlg::OnNewColorgradientsShowbluegradient() 
{
	mode = 3;
	Invalidate();
	
}

void CVgridDlg::OnNewShowblahblah() 

{
	mode = 4;
	Invalidate();
}

void CVgridDlg::OnNewShowbleh() 

{
	mode = 6;
	Invalidate();
}

void CVgridDlg::OnNewShowfinegrid() 
{
	mode = 0;
	grain = 2;
	Invalidate();
}

void CVgridDlg::OnNewShowcoarsegrid() 
{
	mode = 0;
	grain = 8;
	Invalidate();
}
