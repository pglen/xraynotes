
/* =====[ dbtestDlg.cpp ]========================================== 
                                                                             
   Description:     The dbtest project, implementation of the dbtestDlg.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  12/19/2007  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// dbtestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "dbtest.h"
#include "dbtestDlg.h"
#include "sqlite3.h"
#include "mxpad.h"
#include "support.h"
#include "FileDialogST.h"
#include "AddrEntry.h"
#include "DBMain.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString dbfile_g("address.sqlt");


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
// CDbtestDlg dialog

CDbtestDlg::CDbtestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDbtestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDbtestDlg)
	m_table = _T("");
	m_sql = _T("");
	m_sql2 = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	rrr = 0;
	table_offset = 0;
	m_offset = 0;

}

void CDbtestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDbtestDlg)
	DDX_Control(pDX, IDC_COMBO1, m_combo);
	DDX_Control(pDX, IDC_GRID, m_grid);
	DDX_Text(pDX, IDC_EDIT10, m_table);
	DDX_Text(pDX, IDC_EDIT15, m_sql);
	DDX_Text(pDX, IDC_EDIT16, m_sql2);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDbtestDlg, CDialog)
	//{{AFX_MSG_MAP(CDbtestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON12, OnButton12)
	ON_BN_CLICKED(IDC_BUTTON13, OnButton13)
	ON_BN_CLICKED(IDC_BUTTON14, OnButton14)
	ON_BN_CLICKED(IDC_BUTTON15, OnButton15)
	ON_BN_CLICKED(IDC_BUTTON16, OnButton16)
	ON_BN_CLICKED(IDC_BUTTON17, OnButton17)
	ON_BN_CLICKED(IDC_BUTTON18, OnButton18)
	ON_BN_CLICKED(IDC_BUTTON19, OnButton19)
	ON_BN_CLICKED(IDC_BUTTON20, OnButton20)
	ON_BN_CLICKED(IDC_BUTTON21, OnButton21)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON24, OnButton24)
	ON_BN_CLICKED(IDC_BUTTON23, OnButton23)
	ON_BN_CLICKED(IDC_BUTTON25, OnButton25)
	ON_BN_CLICKED(IDC_BUTTON26, OnButton26)
	ON_BN_CLICKED(IDC_BUTTON27, OnButton27)
	ON_BN_CLICKED(IDC_BUTTON28, OnButton28)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDbtestDlg message handlers

BOOL CDbtestDlg::OnInitDialog()

{
	CDialog::OnInitDialog();

	TRACE("Init dialog\r\n");

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

	int loop, size = 50;

	for(loop = 0; loop < size; loop++)
		{
		CString str;
		CString key; key.Format("%d", loop);
		str = AfxGetApp()->GetProfileString("DBTEST", key, "");

		if(str != "")
			{
			//P2N("Combo load = %s\r\n", str);
			m_combo.AddString(str);
			}
		}


	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDbtestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDbtestDlg::OnPaint() 
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
HCURSOR CDbtestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

#include "HShield.h"

void CDbtestDlg::OnButton1() 

{
	CHShield shield; shield.DoModal();
	CAddrEntry addr; addr.DoModal();	
}


static int callback_dump(void *NotUsed, int argc, char **argv, char **azColName)

{
	CDbtestDlg *dlg = (CDbtestDlg *)NotUsed;

	for(int ii = 0;  ii < argc; ii++)
		{
		//P2N("%s='%s'  ", azColName[ii], argv[ii] ? argv[ii] : "NULL");

		if(dlg->rrr == 0)
			dlg->m_grid.AddStr(dlg->rrr, ii, azColName[ii]);

		dlg->table_name = argv[ii];
		dlg->m_grid.AddStr(dlg->rrr + 1, ii , argv[ii] ? argv[ii] : "NULL", false);

		dlg->qcount++;
		}

	//P2N("\r\n");
	dlg->rrr++;

	dlg->m_grid.AutoSize();	
	dlg->m_grid.Invalidate();
	return 0;
}


void CDbtestDlg::ExecSQL(const char *str)

{
	sqlite3 *db; char *zErrMsg = 0; int rc;

	//P2N("Opening DB file %s\\%s\r\n", GetCurrentDir(), dbfile_g);

	rc = sqlite3_open(dbfile_g, &db);

	if( rc )
		{
		MBOXP("SQL error: %s", sqlite3_errmsg(db));
		sqlite3_close(db);
		sqlite3_free(zErrMsg);
		return;
		}

	qcount = 0;
	rrr = 0;

	//CString qqq;
	//qqq = "SELECT * FROM sqlite_sequence";
	//qqq = "PRAGMA table_info (address)";

	rc = sqlite3_exec(db, str, callback_dump, this, &zErrMsg);
	
	if( rc != SQLITE_OK )
		{
		MBOXP("SQL error in:\r\n'%s'\r\n%s", str, sqlite3_errmsg(db));
		sqlite3_free(zErrMsg);
		}
	sqlite3_close(db);
}

void CDbtestDlg::OnButton3() 

{
	m_grid.Clear();
	ExecSQL("SELECT * FROM sqlite_master");
}

void CDbtestDlg::OnButton4() 

{
	m_grid.Clear();

	UpdateData();

	CString str; str.Format("PRAGMA table_info (%s)", m_table);
	ExecSQL(str);
}

void CDbtestDlg::OnButton12() 

{
	m_grid.AddStr(1,3, "HelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHello\r\nfd;lkfd;lkfd;lkjdfsa;lkfjdsa;lkjfds;lkjfdsa;lkjsaf;kdl");
}

void CDbtestDlg::OnButton13() 

{
	m_grid.Clear();		
}

void CDbtestDlg::OnButton14() 

{
	m_grid.Clear();

	UpdateData();
	
	//CString qqq = "select count(*)  from " + m_table;
	//ExecSQL(qqq);
	
	CString str; str.Format("select * from  %s limit 200 offset %d", m_table, m_offset);

	ExecSQL(str);
	m_offset += 200;

}

#if 0
void CDbtestDlg::OnCancel()

{
	CDialog::OnCancel();
}
#endif

void CDbtestDlg::OnButton15() 

{
	m_grid.AddStr(2, 0, "0 long");
	m_grid.AddStr(2, 20, "20 long");
	m_grid.AddStr(2, 19, "19 long");
	m_grid.AddStr(2, 18, "18 long");
}


void CDbtestDlg::OnButton16() 

{
	for(int loop = 0; loop < 1000; loop++)
		{
		for(int loop2 = 0; loop2 < 1000; loop2++)
			{
			CString str; str.Format("r=%d c=%d", loop, loop2);
			m_grid.AddStr(loop, loop2, str, false);
			}	
		}
	m_grid.InvalidateGrid();
}

//////////////////////////////////////////////////////////////////////////

void CDbtestDlg::OnButton17() 

{
	// CFileDialog

	CFileDialogST  fd(true);

	CString dir = support.GetCurrentDir();

			
	CString strFilter;
	
	strFilter += "SQL Lite Files";	strFilter += (TCHAR)NULL;	
	strFilter += "*.sqlt";			strFilter += (TCHAR)NULL;

	// Terminating NULL
	strFilter += (TCHAR)NULL;


	fd.m_ofn.lpstrInitialDir = dir;
	fd.m_ofn.lpstrFilter	= strFilter; 
	fd.m_ofn.nFilterIndex   = 1;


	if(fd.DoModal() == IDOK)
		{
		P2N("Selected file '%s'\r\n", fd.GetPathName());
		dbfile_g = fd.GetPathName();
		}
}


void CDbtestDlg::OnButton18() 

{
	CString str; 

	str.Format("SELECT name  FROM sqlite_master where type = 'table' limit 1 offset %d ",
		table_offset);

	ExecSQL(str);
	
	// Wrap around
	if(!qcount)
		{
		table_offset = 0;
		str.Format("SELECT name  FROM sqlite_master where type = 'table' limit 1 offset %d ",
		table_offset);

		ExecSQL(str);
		}

	table_offset++;

	//P2N("qcount = %d table=%s\r\n", qcount, table_name);		
	m_table = table_name;
	UpdateData(false);

	m_offset = 0;
}

void CDbtestDlg::OnButton19() 

{
	m_grid.Clear();
	UpdateData();
	ExecSQL(m_sql);
}

void CDbtestDlg::OnButton20() 

{
	UpdateData();
	m_grid.Clear();

	//CString txt;
	//m_combo.GetLBText(0, txt);
	//m_combo.AddString(m_sql2);

	m_combo.InsertString(0, m_sql2);
	ExecSQL(m_sql2);	
}

void CDbtestDlg::OnButton21() 

{
	m_grid.AutoSize();
}

void CDbtestDlg::OnButton2() 
{

	CString str = 
	"ALTER TABLE [database-name] table-name ADD [COLUMN] column-def\r\n"
	"ALTER TABLE table-name ADD COLUMN text";

	m_sql2 = str;
	UpdateData(false);
	
}

void CDbtestDlg::OnButton24() 
{
	CString str = 
	"INSERT [OR conflict-algorithm] INTO [database-name .] table-name [(column-list)] VALUES(value-list)";
	
	m_sql2 = str;
	UpdateData(false);
}

void CDbtestDlg::OnButton23() 
{	
	CString str = 
	
	"SELECT [DISTINCT] result [FROM table] "
	"[WHERE expr] [ORDER BY sort-expr-list]"
	" [LIMIT integer [( OFFSET | ,) integer]] ";

	m_sql2 = str;
	UpdateData(false);
}

void CDbtestDlg::OnButton25() 

{
	m_sql2 = "";
	UpdateData(false);
}


void CDbtestDlg::OnButton26() 

{
	CString str = 
	
	"CREATE TABLE [IF NOT EXISTS] [database-name .] table-name ("
    "column-def [, column-def]*  [, constraint]*)\r\n"
	"CREATE TABLE events ( id INTEGER PRIMARY KEY, name TEXT)";

	m_sql2 = str;
	UpdateData(false);
}

void CDbtestDlg::OnButton27() 

{
	CString str = 
	
	"name [type] [[CONSTRAINT name] column-constraint]*";

	m_sql2 = str;
	UpdateData(false);
}

void CDbtestDlg::OnButton28() 
{
	CString str = 
	"DROP TABLE [IF EXISTS] [database-name.] table-name";

	m_sql2 = str;
	UpdateData(false);
}

void CDbtestDlg::OnSelchangeCombo1() 
{
	CString str;
	
	m_combo.GetLBText(m_combo.GetCurSel(), str);

	m_sql2 = str;
		

	UpdateData(false);
}

void CDbtestDlg::OnDestroy() 

{
	// Save combo box
	int loop, size = m_combo.GetCount();

	size = min(50, size);

	for(loop = 0; loop < size; loop++)
		{
		CString str;
		m_combo.GetLBText(loop, str); 
	
		CString key; key.Format("%d", loop);
		AfxGetApp()->WriteProfileString("DBTEST", key, str);
		//P2N("Combo = %s\r\n", str);
		}

	CDialog::OnDestroy();
	
	P2N("Destroy Window\r\n");
}

BOOL CDbtestDlg::PreTranslateMessage(MSG* pMsg) 

{
	//P2N("CDbtestDlg::PreTranslateMessage %d %s\r\n", pMsg->message, mxpad.num2msg(pMsg->message));		
	return CDialog::PreTranslateMessage(pMsg);
}

//DEL BOOL CDbtestDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
//DEL {
//DEL 	
//DEL 	return CDialog::OnNotify(wParam, lParam, pResult);
//DEL }
