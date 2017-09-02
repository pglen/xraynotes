
/* =====[ xcalDlg.cpp ]========================================== 
                                                                             
   Description:     The xcal project, implementation of the xcalDlg.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  6/20/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// xcalDlg.cpp : implementation file
//

#include "stdafx.h"
#include "xraynotes.h"
#include "xcalDlg.h"
#include "mxpad.h"
#include "support.h"
#include "sqlite3.h"
#include "calen.h"
#include "parse.h"
#include "AlaDlg.h"

CString CXcalDlg::dbfile = "calendar.sqlt";

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static int callback_fill(void *NotUsed, int argc, char **argv, char **azColName)

{
	CString str;
	CxCalen *lastclass = (CxCalen *)NotUsed;

	//P2N("callback_fill\r\n");
	
	str = argv[0];	str += ", "; str += argv[1];

	//HTREEITEM item = lastclass->m_tree.InsertItem(str, 1, 0);
	//DWORD id = atoi(argv[2]);
	//lastclass->m_tree.SetItemData(item, id);
	
	//P2N("Got item name: '%s' date: '%s'  id: '%s'\r\n", argv[0], argv[1], argv[2]);

	COleDateTime ddd;

	if(argv[1])
		{
		str2date(argv[1], &ddd);
		lastclass->SetItem(&ddd, argv[0], atoi(argv[2]));
		}

	return 0;
}

void	CXcalDlg::fillcall(void *ptr)

{
	CxCalen *lastclass = (CxCalen *)ptr;

	// Start and end times
	COleDateTime dd = lastclass->startdate;
	COleDateTime ee = lastclass->startdate;
	ee +=  7 * lastclass->oneday;
	
	CString dstr; dstr = dd.Format("%Y/%m/%d");
	CString estr; estr = ee.Format("%Y/%m/%d");
	
	//P2N("Callback from calendar ctrl for %s to %s\r\n", dstr, estr);

	lastclass->ClearContents();

	// Fill in customer name tree
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	//P2N("dbfile %s%s\r\n", support.GetCurrentDir(), dbfile);

	rc = sqlite3_open(dbfile, &db);

	if(rc)
		{
		CString tmp;tmp.Format("SQL error: %s", sqlite3_errmsg(db));
		AfxMessageBox(tmp);

		sqlite3_close(db);
		sqlite3_free(zErrMsg);
		return;
		}

	CString qqq;
	qqq.Format("select name, sdate, id from calendar where sdate >= '%s' and sdate < '%s'", dstr, estr);

	//qqq.Format("select name, sdate, tidstr from calendar");

	//P2N("qqq=%s\r\n", qqq);

	// Pass the pointer to the control (instead of this)
	rc = sqlite3_exec(db, qqq, callback_fill, lastclass, &zErrMsg);

	if( rc != SQLITE_OK )
		{
		CString tmp; tmp.Format("SQL error: %s", sqlite3_errmsg(db));
		AfxMessageBox(tmp);

		sqlite3_free(zErrMsg);
		}

	sqlite3_close(db);

	lastclass->Invalidate();	
}

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

	//P2N("CXcalDlg::CXcalDlg\r\n");

	m_split.callback = fillcall;
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
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXcalDlg message handlers

BOOL CXcalDlg::OnInitDialog()

{
	CDialog::OnInitDialog();

#if 0
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
#endif

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
		
	//SIZE sizeMin; sizeMin.cx = 100; sizeMin.cy = 100;
	//spl.Create(this, 2, 2, sizeMin, NULL, WS_VISIBLE |WS_HSCROLL | WS_VSCROLL | SPLS_DYNAMIC_SPLIT);

	
	CheckCalDBfile(NULL);

	support.GetDataRoot(CXcalDlg::dbfile);
	CXcalDlg::dbfile += "calendar.sqlt";

	P2N("CXcalDlg::OnInitDialog dbfile=%s\r\n", CXcalDlg::dbfile);

	m_split.ShowWindow(SW_SHOW);
	
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

//DEL void CXcalDlg::SetItem(int day, int hour, const char *str)
//DEL {
//DEL 
//DEL 	
//DEL }

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


BOOL CXcalDlg::PreTranslateMessage(MSG* pMsg) 

{
	if(pMsg->message == CALITEM_DBLCLICK)
		{
		int lw = LOWORD(pMsg->lParam);
		int hw = HIWORD(pMsg->lParam);
		
		P2N("DoubleClick message from calendar ctrl=%d  hour=%d day=%d hiword=%d\r\n", 
					pMsg->wParam, (lw & 0xff), (lw >> 8), hw);

		COleDateTime dd = m_split.startdate;
		dd += (lw >> 8) * m_split.oneday;

		CString dstr; dstr = dd.Format("%Y/%m/%d");

		P2N("Ctrl day: %s\r\n", dstr);	

		memcpy(&msg2, pMsg, sizeof(MSG));
		SetTimer(1, 100, NULL);
		}

	if(pMsg->message == CALITEM_WANTDATA)
		{
		COleDateTime dd = m_split.startdate;
		CString dstr; dstr = dd.Format("%Y/%m/%d");
	
		//P2N("Message from calendar ctrl %d - want data for %s\r\n", pMsg->wParam, dstr);
		//Fill(&dd);
		}
 
	if(pMsg->message == CALITEM_RCLICK)
		{
		int lw = LOWORD(pMsg->lParam);
		int hw = HIWORD(pMsg->lParam);

		COleDateTime dd = m_split.startdate;
	
		dd += (lw >> 8) * m_split.oneday;

		P2N("Rclick message from calendar ctrl=%d  hour=%d day=%d hiword=%d\r\n", 
					pMsg->wParam, (lw & 0xff), (lw >> 8), hw);

		CString dstr; dstr = dd.Format("%Y/%m/%d");
		P2N("Ctrl day: %s\r\n", dstr);	
		}

	return CDialog::PreTranslateMessage(pMsg);
}

//////////////////////////////////////////////////////////////////////////
// 

void	CXcalDlg::Fill(COleDateTime *dd)

{
	BeginWaitCursor();

	//YieldToWinEx();

	// Fill in customer name tree
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	rc = sqlite3_open(dbfile, &db);

	if( rc )
		{
		CString tmp;tmp.Format("SQL error: %s", sqlite3_errmsg(db));
		AfxMessageBox(tmp);

		sqlite3_close(db);
		sqlite3_free(zErrMsg);
		return;
		}

	// Clear prev contents
	//for(int loop = 0; loop < WEEKDAYS; loop++)
	//	{
	//	for(int loop2 = 0; loop2 < 4 * HOURSINDAY; loop2++)
	//		{
	//		*data[loop][loop2] = "";
	//		arr[loop][loop2].RemoveAll();
	//		}
	//	}


	CString qqq = "select name, edate, id from calendar ";

//	if(str != NULL)
//		{
//		if(strlen(str))
//			{
//			CString filter; filter.Format(" where lname like '%%%s%%'",  str);
//			qqq += filter;
//			}
//		}

	//qqq += " order by udate desc";
	//P2N("qqq=%s\r\n", qqq);

	// Pass the pointer to the control (instead of this)
	rc = sqlite3_exec(db, qqq, callback_fill, &m_split, &zErrMsg);
	if( rc != SQLITE_OK )
		{
		CString tmp; tmp.Format("SQL error: %s", sqlite3_errmsg(db));
		AfxMessageBox(tmp);

		sqlite3_free(zErrMsg);
		}

	m_split.Invalidate();

	sqlite3_close(db);

	//m_tree.SortChildren(NULL);

	//if(!old)
	//	{
	//	m_tree.SelectItem(m_tree.GetRootItem());
	//	}
	//else
	//	{
	//	SelEntryInt(m_tree.GetItemData(old));
	//	}

	EndWaitCursor();

	//filling = false;
}

//////////////////////////////////////////////////////////////////////////

void CXcalDlg::OnTimer(UINT nIDEvent) 

{
	if(	nIDEvent == 1)
		{
		KillTimer(nIDEvent);
		
		//	MBOXP("calendar ctrl=%d  hour=%d day=%d hiword=%d\r\n", 
		//				pMsg->wParam, (lw & 0xff), (lw >> 8), hw);
		//MBOXP("dbl click from cal");

		CAlaDlg dlg;
		
		int lw = LOWORD(msg2.lParam);
		int hw = HIWORD(msg2.lParam);

		int day		= (lw >> 8);
		int hour	= (lw & 0xff);
		
		P2N("Saved info from calendar ctrl=%d  hour=%d day=%d hiword=%d\r\n", 
				msg2.wParam, (lw & 0xff), (lw >> 8), hw);

		COleDateTime dd;
		dd = m_split.startdate;
		dd += day * m_split.oneday;

		COleDateTimeSpan onehour;
		onehour.SetDateTimeSpan(0, 1, 0, 0);

		dd += hour * onehour;
		
		CString dstr; dstr = dd.Format("%Y/%m/%d  %H:%M:%S");
		CString hstr; hstr = dd.Format("%I %p");

		COleDateTime ddd = dd;
		ddd += onehour;
		CString hstr2; hstr2 = ddd.Format("%I %p");

		dlg.headstr.Format("Details for: %s -- TimeSlot: %d - %d ( %s - %s )", dstr, hour, hour + 1, hstr, hstr2);
		dlg.date	= dd;
		dlg.date2	= ddd;

		dlg.DoModal();
		}
		
	CDialog::OnTimer(nIDEvent);
}
