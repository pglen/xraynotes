// serialsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "serials.h"
#include "serialsDlg.h"

#include <io.h>
#include <direct.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MASTERSUM 88
#define MBOX AfxMessageBox

CString fname("license.txt");
CString fcount("counter.txt");


void	CSerialsDlg::Str2Clip(const char *str)

{
	CString all = str;

	// Determine if we have an object sitting on the clipboard
	if (!OpenClipboard())
		{
		AfxMessageBox("Cannot open Clipboard.");
		return;
		}

	int len = all.GetLength() + 1;

    // Build a copy of the text itself
    HANDLE  txthandle = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, len);
	char *txtptr = (char *)GlobalLock(txthandle);
    ASSERT(txtptr);
    strcpy(txtptr, all);
    ::GlobalUnlock(txthandle);

	EmptyClipboard();
	SetClipboardData(CF_TEXT, txthandle);
	CloseClipboard();	
}

///////////////////////////////////////////////////////////////////////////////
// Return sum of string

int		SumStr(const char *str)

{
    CString str2 = str;

	int sum = 0, loop, lim = str2.GetLength();

	str2.MakeUpper();

	for(loop = 0; loop < lim; loop++)
		{
		char chh = str2.GetAt(loop);

		if(isalnum(chh))
			sum += chh;
		}
	return(sum);
}

///////////////////////////////////////////////////////////////////////////////
// Return TRUE if OK

int		CheckLic(const char *str, int *res)

{
	int sum2;
	
	sum2 = SumStr(str);

	if(res)
		*res = sum2;
	
	// Terminal condition
	if(!(sum2 % MASTERSUM))
		{
		return true;
		}
	else
		{
		return false;
		}			
}

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
// CSerialsDlg dialog

CSerialsDlg::CSerialsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSerialsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSerialsDlg)
	m_name = _T("");
	m_cust = _T("");
	m_serial = _T("");
	saved = true;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSerialsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSerialsDlg)
	DDX_Text(pDX, IDC_EDIT1, m_name);
	DDX_Text(pDX, IDC_EDIT2, m_cust);
	DDX_Text(pDX, IDC_EDIT3, m_serial);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSerialsDlg, CDialog)
	//{{AFX_MSG_MAP(CSerialsDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSerialsDlg message handlers

BOOL CSerialsDlg::OnInitDialog()
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
	
	// TODO: Add extra initialization here

	int		result;
	struct _stat buf;

#if 0
	struct _finddata_t c_file;
    long hFile;

    /* Find first .c file in current directory */
    if( (hFile = _findfirst( "license.txt", &c_file )) == -1L )
		MBOX( "No lic. file in current directory!\n" );

	//MBOX((char*)&c_file);
#endif

	result = _stat(fname, &buf );
	if( result < 0 )
		{
		//MBOX( "Problem getting file information" );
		//return(0);
		}

	//m_stat.Format("inode = %d", buf.st_ino);
	UpdateData(false);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSerialsDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSerialsDlg::OnDestroy()
{
	WinHelp(0L, HELP_QUIT);
	CDialog::OnDestroy();
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSerialsDlg::OnPaint() 
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
HCURSOR CSerialsDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSerialsDlg::OnOK() 
{
	if(!saved)
		{
		if(AfxMessageBox("Unsaved Key. Arey you sure you want to exit?", MB_YESNO | MB_ICONQUESTION) != IDYES)
			return;
		}
	//AfxMessageBox("hello");
	CDialog::OnOK();
}


void CSerialsDlg::OnButton1() 

{
	int sum = 0, sum2;
	int loop, loop2;

	UpdateData();

	if(m_name == "")
		{
		AfxMessageBox("Please enter a Customer Name");
		return;
		}

	sum += SumStr(m_name); sum += SumStr(m_cust);
	srand(clock());

	// Loop until found
	for(loop2 = 0; loop2 < 100000; loop2++)
		{
		m_serial = "";
		// Produce base
		for(loop = 0; loop < 4; loop++)
			m_serial += (char)(rand() % ('Z'-'A') + 'A');

		m_serial += "-";

		for(loop = 0; loop < 4; loop++)
			m_serial += (char)(rand() % ('9'-'0') + '0');

		m_serial += "-";

		for(loop = 0; loop < 4; loop++)
			m_serial += (char)(rand() % ('Z'-'A') + 'A');

		m_serial += "-";

		for(loop = 0; loop < 4; loop++)
			m_serial += (char)(rand() % ('9'-'0') + '0');
	
		sum2 = sum + SumStr(m_serial);

		// Terminal condition
		if((sum2 % MASTERSUM) == 0)
			{
			//CString str; str.Format("loop iterations %d sum=%d", loop2, sum2);
			//MBOX(str);
			break;
			}
		}

	if(loop2 == 100000)
	{
		//AfxMessageBox("Try again");
		m_serial = "Invalid, try again";
	}

	//AfxMessageBox(m_serial);
	saved = false;

	UpdateData(false);
}

//////////////////////////////////////////////////////////////////////////


void CSerialsDlg::OnButton2() 

{
	UpdateData();

	if(m_serial == "")
		{
		AfxMessageBox("Please generate a key first");
		return;
		}

	int sum = 0;
	sum += SumStr(m_name); sum += SumStr(m_cust);
	int sum2 = sum + SumStr(m_serial);

	// Terminal condition
	if(!(sum2 % MASTERSUM))
	{
		AfxMessageBox("Key Is Valid");
	}
	else
	{
		AfxMessageBox("Key is Invalid", MB_ICONSTOP);
	}			
}

//////////////////////////////////////////////////////////////////////////

void CSerialsDlg::OnCancel() 

{
	//CDialog::OnCancel();
}

void CSerialsDlg::OnOk2() 

{

}

void CSerialsDlg::OnButton3() 

{

	UpdateData();

	if(m_name == "")
		{
		AfxMessageBox("Please enter a Customer Name");
		return;
		}

	if(m_serial == "")
		{
		AfxMessageBox("Please generate a key first");
		return;
		}

	if(saved)
		{
		AfxMessageBox("This license was saved already");
		return;
		}

	CString all;

	all+= m_name; all+= m_cust; all+= m_serial;

	if(!CheckLic(all, NULL))
		{
		AfxMessageBox("Not a valid license, cannot save");
		return;
		}

	all = "";

	all +="\r\n------- Begin License ------------\r\n\r\n";

	all += "Name: ......... '";	all += m_name;		all += "'\r\n";
	all += "Company: ...... '";	all += m_cust;		all += "'\r\n";
	all += "License: ...... '";	all += m_serial;	all += "'\r\n";

	all +="\r\n------- End License ------------\r\n\r\n";

	Str2Clip(all);

	CFileException fileException;
	CFile xcpt;

	if (!xcpt.Open(fname, CFile::modeReadWrite | CFile::modeNoTruncate  |
						CFile::modeCreate, NULL))
		{
		AfxMessageBox("Cannot open/create the license file.");
		return;
		}

	xcpt.SeekToEnd();

	xcpt.Write(all, all.GetLength());
	xcpt.Close();

	CFile xcpt2;

	if (!xcpt2.Open(fcount, 	CFile::modeReadWrite | CFile::modeNoTruncate  |
						CFile::modeCreate, NULL))
		{
		AfxMessageBox("Cannot open/create the license counter file.");
		return;
		}

	char	buff[18];

	xcpt2.Read(buff, 18);
	int	cnt = atoi(buff); cnt++;
	xcpt2.SeekToBegin();
	CString buf2; buf2.Format("%d", cnt);
	xcpt2.Write(buf2, buf2.GetLength());
	xcpt2.Close();

	// -------------------------------------------------------------------

	CFile xcpt3;

	if (!xcpt3.Open("license.csv", CFile::modeReadWrite | CFile::modeNoTruncate  |
						CFile::modeCreate, NULL))
		{
		AfxMessageBox("Cannot open/create the license CSV file.");
		return;
		}

    time_t osBinaryTime;            // C run-time time (defined in <time.h>)
    time( &osBinaryTime ) ;         // Get the current time from the OS
    CTime time3(osBinaryTime );

	CString dstr = time3.Format( "%a %c");

#if 0
    dstr.Format("%s %02d-%02d-%02d %02d:%02d:%02d",
            (int)time3.GetMonth(),
                    (int)time3.GetDay(),
                        (int)time3.GetYear() - 2000,	
							(int)time3.GetHour(), 
								(int)time3.GetMinute(), 
									(int)time3.GetSecond());
#endif

	xcpt3.SeekToEnd();
	
	xcpt3.Write(dstr, dstr.GetLength());
	xcpt3.Write(",", 1);

	xcpt3.Write(m_name, m_name.GetLength());
	xcpt3.Write(",", 1);

	xcpt3.Write(m_cust, m_cust.GetLength());
	xcpt3.Write(",", 1);

	xcpt3.Write(m_serial, m_serial.GetLength());
	xcpt3.Write("\r\n", 2);

	xcpt3.Close();


	saved = true;
}


void CSerialsDlg::OnButton4() 

{
	UpdateData();
	Str2Clip(m_name);	
}

//////////////////////////////////////////////////////////////////////////


void CSerialsDlg::OnButton5() 
{
	UpdateData();
	Str2Clip(m_cust);	
}


void CSerialsDlg::OnButton6() 
{
	UpdateData();
	Str2Clip(m_serial);	
}

void CSerialsDlg::OnButton7() 
{
	//	
	UpdateData();

	CString all = "";

	all += "\r\nPaste items within quotes (') into license dialog.\r\n";
	all += "Double quote ('') means a blank item.\r\n";

	all +="\r\n ------------ Begin XrayNotes License ------------\r\n\r\n";
	
	all += "    User Name: ........ '";	all += m_name;		all += "'\r\n";
	all += "    Company: .......... '";	all += m_cust;		all += "'\r\n";
	all += "    License: .......... '";	all += m_serial;	all += "'\r\n";

	all +="\r\n ------------ End XrayNotes License ------------\r\n\r\n";

	Str2Clip(all);
}
