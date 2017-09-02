
/* =====[ clientDlg.cpp ]========================================== 
                                                                             
   Description:     The client project, implementation of the clientDlg.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  5/30/2007  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// clientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "process.h"
#include "malloc.h"

#include "client.h"
#include "clientDlg.h"
#include "xFile.h"
#include "../bluepoint/bluepoint.h"

#include "Support.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


int		hashfunc(char *buf, int cnt, unsigned int *hash)

{
	unsigned int hhh =  *hash;
	int loop;
	
	for(loop = 0; loop < cnt; loop++)
		{
		hhh += (unsigned char)*buf++;
		hhh = ROTATE_LONG_RIGHT(hhh, 10);          /* rotate right */
		}

	*hash = hhh;
	
	return 0;
}


///////////////////////////////////////////////////////////////////////////
// YieldToWinEx
// Let every accumulated message go through
// Return 1 if ESC | Ctrl-Break pressed -- 2 if EXITING

int     YieldToWinEx()

{
    int ret_val = 0;
    while(TRUE)
        {
        MSG msg;
        if(!PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
           break;

        // Ctrl-Break
        if(msg.message == WM_KEYDOWN)
            {
            if(msg.wParam == VK_CANCEL)
                {
                ret_val = 1; break;
                }
            }
        // Esc
        if(msg.message == WM_CHAR)
            {
            if(msg.wParam == VK_ESCAPE)
                {
                ret_val = 1; break;
                }
            }
        // App exit
        if(msg.message == WM_QUIT)
                {
                //wait_exit = TRUE;
                ret_val = 2; break;
                }
        // App kill
        if(msg.message == WM_DESTROY)
                {
                //wait_exit = TRUE;
                ret_val = 2; break;
                }
        // Every other message is processed
        TranslateMessage(&msg); DispatchMessage(&msg);
        }
    return(ret_val);
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
// CClientDlg dialog

CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClientDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CClientDlg)
	m_host = _T("");
	m_com = _T("");
	m_port = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	*buffer = 0;
	fsm = buffprog = rprog = 0;

	in_send = in_recv = 0;
	abort = false;
	buffpass = NULL;
	lprog = 0;
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)

{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CClientDlg)
	DDX_Control(pDX, IDC_BUTTON17, m_b17);
	DDX_Control(pDX, IDC_BUTTON16, m_b16);
	DDX_Control(pDX, IDC_BUTTON15, m_b15);
	DDX_Control(pDX, IDC_BUTTON14, m_b14);
	DDX_Control(pDX, IDC_LOG2, m_log);
	DDX_Control(pDX, IDC_BUTTON1, m_con);
    DDX_Control(pDX, IDC_BUTTON13,     m_Exec		 );   
    DDX_Control(pDX, IDOK        ,     m_Exit		 );	
    DDX_Control(pDX, IDCANCEL    ,     m_Cancel		 );
    DDX_Control(pDX, IDC_BUTTON12,     m_ClearLog	 );	
    DDX_Control(pDX, IDC_BUTTON2 ,     m_Disconnect	 );
    DDX_Control(pDX, IDC_BUTTON3 ,     m_Login		 );	
    DDX_Control(pDX, IDC_BUTTON5 ,     m_LS			 );
    DDX_Control(pDX, IDC_BUTTON6 ,     m_Slst		 );	
    DDX_Control(pDX, IDC_BUTTON7 ,     m_Recv		 );	
    DDX_Control(pDX, IDC_BUTTON11,     m_Abort		 );	
    DDX_Control(pDX, IDC_BUTTON8 ,     m_Help		 );	
    DDX_Control(pDX, IDC_BUTTON10,     m_Stat		 );	
    DDX_Control(pDX, IDC_BUTTON9 ,     m_PWD		 );	
    DDX_Control(pDX, IDC_BUTTON4 ,     m_eKey		 );	
	DDX_Control(pDX, IDC_STATUS, m_status);
	DDX_Text(pDX, IDC_EDIT1, m_host);
	DDX_Text(pDX, IDC_EDIT2, m_com);
	DDX_Text(pDX, IDC_EDIT8, m_port);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialog)
	//{{AFX_MSG_MAP(CClientDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_WM_ENDSESSION()
	ON_WM_QUERYENDSESSION()
	ON_BN_CLICKED(IDC_BUTTON8, OnButton8)
	ON_BN_CLICKED(IDC_BUTTON9, OnButton9)
	ON_BN_CLICKED(IDC_BUTTON10, OnButton10)
	ON_BN_CLICKED(IDC_BUTTON11, OnButton11)
	ON_BN_CLICKED(IDC_BUTTON12, OnButton12)
	ON_BN_CLICKED(IDC_BUTTON13, OnButton13)
	ON_BN_CLICKED(IDC_BUTTON15, OnButton15)
	ON_BN_CLICKED(IDC_BUTTON16, OnButton16)
	ON_BN_CLICKED(IDC_BUTTON17, OnButton17)
	ON_BN_CLICKED(IDC_BUTTON18, OnButton18)
	ON_BN_CLICKED(IDC_BUTTON14, OnButton14)
	ON_BN_CLICKED(IDC_BUTTON19, OnButton19)
	ON_BN_CLICKED(IDC_BUTTON20, OnButton20)
	ON_BN_CLICKED(IDC_BUTTON21, OnButton21)
	ON_BN_CLICKED(IDC_BUTTON22, OnButton22)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClientDlg message handlers

BOOL CClientDlg::OnInitDialog()

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

	P2N("Started XrayNotes client\r\n");

	m_log.AddLine("Started Client");

	m_host = "192.168.1.50";
	m_port = "9955";

	UpdateData(false);

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	((CEdit*)GetDlgItem(IDC_EDIT3))->SetWindowText("p");
	((CEdit*)GetDlgItem(IDC_EDIT4))->SetWindowText("p");
	((CEdit*)GetDlgItem(IDC_EDIT5))->SetWindowText("aa");
	((CEdit*)GetDlgItem(IDC_EDIT6))->SetWindowText("download.dat");
	((CEdit*)GetDlgItem(IDC_EDIT7))->SetWindowText("download.dat");

	AllButtons(false);

	//int rete = m_sock1.Create(m_port);
	//P2N("Socket created ret=%d lasterr=%d\r\n", rete, GetLastError());

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CClientDlg::OnPaint() 
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
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


#if 0
	fsm = 0;
	//_beginthread(connectthread, 0, this);
	
	while(true)
		{
		Sleep(2);
		YieldToWinEx();

		if(fsm != 0)
			break;
		}

	if(fsm != 1)
		{
		P2N("Could not connect\r\n");
		return;
		}

void connectthread( void *ptr )

{
	CClientDlg *dlg = (CClientDlg *)ptr;

	P2N("Started connect thread\r\n");
	int rete = dlg->m_sock1.Create(atoi(m_port));

	int retc = dlg->m_sock1.Connect(dlg->m_host, atoi(m_port));
	P2N("Socket connected ret=%d lasterr=%s\r\n", retc, sockerr2str(GetLastError()));

	if(retc > 0)
		dlg->fsm = 1;
	else
		{
		dlg->fsm = 2;
		dlg->m_sock1.Close();
		}
}

#endif

//////////////////////////////////////////////////////////////////////////

void CClientDlg::OnButton1() 

{
	char	line[_MAX_PATH];

	UpdateData();
	//AfxMessageBox(m_host);
	//AfxMessageBox(m_port);

	// Reentry from connect call
	if(fsm == 1)
		{
		P2N("Stopped connection\r\n");
		m_con.SetWindowText("Connect");
		m_status.SetText("Stopped Connection Attempt");

		//m_sock1.CancelBlockingCall();
		m_sock1.Close();
		//m_sock1.Create(atoi(m_port));

		fsm = 0;
		return;
		}
	
	if(fsm == 0)
		{
		//if(!wassock)
			{
			//wassock = true;

			//if(m_sock1 == NULL)
			//	m_sock1 = new CMySocket;

			int rete = m_sock1.Create(atoi(m_port));

			P2N("Socket created ret=%d lasterr=%d\r\n", rete, GetLastError());
			if(rete == 0)
				{
				m_status.SetText("Cannot create socket");
				return;
				}

			//m_sock1.CancelBlockingCall();

#if 0
			BOOL	ru = true ;
			rete = m_sock1.SetSockOpt(SO_REUSEADDR, &ru, sizeof(ru));
			P2N("Socket option 1 ret=%d lasterr=%d\r\n", rete, GetLastError());
			//SO_DONTLINGER
			rete = m_sock1.SetSockOpt(SO_DONTLINGER, &ru, sizeof(ru));
			P2N("Socket option 2 ret=%d lasterr=%d\r\n", rete, GetLastError());
#endif

			}
		}
	
	// Connect	
	CString str; str.Format("Connecting to: %s", m_host);
	m_status.SetText(str);
	m_con.SetWindowText("Stop");

	fsm = 1;
	
	// Clear line buffer
	rprog = buffprog = 0;

	P2N("Attempting to connect to: %s\r\n", m_host);
	int retc = m_sock1.Connect(m_host, atoi(m_port));
	P2N("Socket connected ret=%d lasterr=%s\r\n", retc, sockerr2str(GetLastError()));

	// Error?
	//if(retc == 0)
	//	{
	//	P2N("Completed blocking call\r\n");
	//	return;
	//	}

	int serr;
	GetRemLine(line, _MAX_PATH, &serr);	
	m_log.AddLine(line);

	if(strncmp(line, "220 ", 4) == 0)
		{
		str.Format("Connected to: %s", m_host);
		m_status.SetText(str);

		m_con.SetWindowText("Disconnect");
		m_con.EnableWindow(false);
		AllButtons(true);

		fsm = 2;
		}	
	else
		{
		m_con.SetWindowText("Connect");
		}

	SetFocus();
}

//////////////////////////////////////////////////////////////////////////

void CClientDlg::OnButton2() 

{
	CString str("quit\r\n"); 
	char	line[_MAX_PATH + 1];
	int serr;

	if(!m_sock1)
		return;
	
	m_sock1.Send(str, str.GetLength());
	
	// Eval reply
	GetRemLine(line, _MAX_PATH, &serr);

	m_log.AddLine(line);
	m_status.SetText(line);

	m_sock1.Close();
	fsm = 0;

	m_con.EnableWindow();
	m_con.SetWindowText("Connect");
	AllButtons(false);

	//m_status.SetText("disconnected");
}

//////////////////////////////////////////////////////////////////////////
	


void CClientDlg::OnButton3() 

{
	CString		str2("user p\r\n"); 
	char		line[_MAX_PATH];

	int		serr;

	if(!m_sock1)
		return;

	m_sock1.Send(str2, str2.GetLength());
	
	GetRemLine(line, _MAX_PATH, &serr);
	if(serr < 0)
		{
		P2N("Socket error on recv command. %s\r\n", sockerr2str(GetLastError()) );
		m_log.AddLine("Socket error on recv command");
		return;
		}
	m_log.AddLine(line);
	
	//P2N("got reply:'%s'\r\n", line);
	m_status.SetText((const char*)line);
	
	if(line[0] == '5')
		return;

	CString str = "pass p\r\n"; 
	m_sock1.Send(str, str.GetLength());

	GetRemLine(line, _MAX_PATH, &serr);
	m_log.AddLine(line);

	//P2N("got reply:'%s'\r\n", line);
	m_status.SetText((const char*)line);
}

void CClientDlg::OnButton4() 

{
	char	line[_MAX_PATH];
	int		serr;

	if(!m_sock1)
		return;

	// New command initiates, drain socket
	m_sock1.DrainRecv(NULL, 0);

	// Send command
	CString str("ekey\r\n"); 
	int sent = m_sock1.Send(str, str.GetLength());

	if(sent < 0)
		{
		P2N("Socket error on send command. %s\r\n", sockerr2str(GetLastError()) );
		return;
		}

	GetRemLine(line, _MAX_PATH, &serr);
	m_log.AddLine(line);

	if(serr < 0)
		{
		P2N("Socket error on recv command. %s\r\n", sockerr2str(GetLastError()) );
		return;
		}

	if(atoi(line) != 200)
		{
		P2N("Error on command. Server said: %s\r\n", line);
		return;
		}

	char *ptr = strchr(line, '\'');
	if(ptr)
		{
		char *ptr2 = strchr(ptr+1, '\'');
		if(ptr2)
			{
			*ptr2 = 0;
			ekey = ptr+1;
			P2N("Got key: '%s'\r\n", ekey);
			m_status.SetText(ekey);
			}
		}

	//P2N("Got key %s\r\n", line);
}

void CClientDlg::OnButton5() 

{
	int linenum = 0;

	char	line[_MAX_PATH + 1];
	int		serr;

	if(!m_sock1)
		return;

	if(in_recv) 
		{	
		P2N("In receive already\r\n");
		m_status.SetText("Reentered receive");
		return;
		}
	in_recv = true;

	CString str("list test\r\n"); 
	m_sock1.Send(str, str.GetLength());

	// Eval reply
	GetRemLine(line, _MAX_PATH, &serr);
	m_log.AddLine(line);

	if(atoi(line) != 214)
		{
		m_status.SetText(line);
		
		P2N("Error on command. Server said: %s\r\n", line);
		in_recv = false;
		return;
		}
	
	while(true)
		{	
		GetRemLine(line, _MAX_PATH, &serr);
		
		if(serr < 0)
			break;

		m_log.AddLine(line);

		// See if terminating condition
		if(strncmp(line, "214 ", 4) == 0)
			{
			P2N("Final line '%s'\r\n", line);
			m_status.SetText(line);
			break;
			}

		P2N("LS line: '%s'\r\n", line);
		
		linenum++;
		}

	P2N("%d lines received\r\n", linenum);

	in_recv = false;

	//if(strstr(buf, "214 End file list") != 0)
	//	break;

}

//////////////////////////////////////////////////////////////////////////
//

	CString sstr("slst test\r\n"); 
	

void CClientDlg::OnButton6() 

{
	char	line[_MAX_PATH + 1];
	int		serr;

	if(!m_sock1)
		return;

	// Send command
	m_sock1.Send(sstr, sstr.GetLength());

	// Eval reply
	GetRemLine(line, _MAX_PATH, &serr);
	m_log.AddLine(line);

	if(atoi(line) != 214)
		{
		m_status.SetText(line);
		P2N("Error on command. Server said: %s\r\n", line);
		return;
		}
	
	while(true)
		{
		GetRemLine(line, _MAX_PATH, &serr);
		
		if(serr < 0)
			break;

		m_log.AddLine(line);
		P2N("slst line: '%s'\r\n", line);
		
		// See if terminating condition
		if(strncmp(line, "214 ", 4) == 0)
			{
			m_status.SetText(line);
			//m_log.AddLine(line);
			break;
			}
		}

	//P2N("Got whole transaction\r\n");
}

//////////////////////////////////////////////////////////////////////////

void CClientDlg::OnButton7() 

{
	char	line[_MAX_PATH + 1];
	int		serr, len = 0, xlen = 0;
	int		oldc, olen, ret;
	int		startc = clock();
	int		prog = 0;
	unsigned int		hash = 0;

	CxFile fp;

	abort = false;

	if(in_recv)
		{	
		P2N("In receive already\r\n");
		m_status.SetText("Reentered receive");
		return;
		} 

	in_recv = true;

	CString str, file;
	((CEdit*)GetDlgItem(IDC_EDIT5))->GetWindowText(file);
	str = "retr " + file + "\r\n";

	//CString str("retr test/save\r\n"); 
	//CString str("retr test/rfcxxxx.txt\r\n"); 
	//CString str("retr aa\r\n"); 
	//CString str("retr test/all.c\r\n"); 
	
	m_sock1.Send(str, str.GetLength());

	GetRemLine(line, _MAX_PATH, &serr);
	
	if(serr < 0)
		{
		P2N("Failed to receive reply line\r\n");
		goto endd;
		}

	m_log.AddLine(line);

	//P2N("Got announcement reply: %s\r\n", line);

	if(atoi(line) == 211)
		{
		// Find the quotes
		char *ptr = strchr(line, '\'');
		if(ptr)
			xlen = len = atoi(ptr + 1 );

		P2N("Got transfer announcement: status=%d len=%d\r\n", atoi(line), len);
		}
	else
		{
		m_status.SetText(line);
		
		P2N("Error on command. Server said: %s\r\n", line);	
		goto endd;
		}	

	if(len == 0)
		{
		m_log.AddLine("Malformed reply on retr");
		goto endd;
		}

	ret = fp.Open("download.dat", CFile::modeCreate | CFile::modeWrite);
	if(!ret)
		{
		AfxMessageBox("Cannot create download file\r\n");
		goto endd;
		}

	// See how much we already have:
	olen = _MAX_PATH;
	GetOverFlow(buf, &olen);

	if(olen > 0)
		{
		P2N("Got overflow %d bytes\r\n", olen);
		len -= olen;
		hashfunc(buf, olen, &hash);
		fp.Write(buf, olen);
		}

	oldc = clock();
	
	// Receive subscribed data:
	while(true)
		{
		int get = BUFFSIZE;
		if(get > len) get = len;

		int ret = m_sock1.Receive(buf, get);
		
		if(ret > 0)
			{
			//ASSERT(ret <= get);
			//buf[ret] = 0;

			prog += ret;
			hashfunc(buf, ret, &hash);
			fp.Write(buf, ret);

			int newc = clock();

			if((newc - oldc) > 500)
				{
				int speed = 0;
				int elapsed = (newc - startc);

				if(elapsed > 0)	
					{
					speed = (prog) / elapsed;
					}

				CString str; str.Format("Progress %d %d kbytes/sec", prog, speed);
				m_status.SetText(str);

				//P2N("transfer at: %d bytes left %d clock=%d\r\n", ret, len, newc);
				oldc = newc;
				}

			YieldToWinEx();		
				
			len -= ret;

			if(abort)
				break;

			if(len <= 0)
				break;
			}	
		else
			{
			P2N("Error in recv\r\n");
			break;
			}
		}

	fp.Close();

	if(abort)
		{
		abort = false;
		goto endd;
		
		//P2N("abort buffer %s\r\n", buf);

		// Eval reply
		//GetRemLine(line, _MAX_PATH, &serr);
		//P2N("Abort line %s\r\n", line);

		//if(atoi(line) != 211)
		//	{
		//	m_status.SetText(line);
		//	P2N("Error on command. Server said: %s\r\n", line);
		//	return;
		//	}	
		}
 
	else if(ret > 0)
		{
		// Get terminating line:
		GetRemLine(line, _MAX_PATH, &serr);
		P2N("Got terminating : '%s'\r\n", line);

		// Get conclusion line:
		GetRemLine(line, _MAX_PATH, &serr);

		P2N("Got xconclusion reply: '%s'\r\n", line);

		m_log.AddLine(line);
		m_status.SetText(line);

		// Find the quotes
		char *ptr2 = strchr(line, '\'');
		if(ptr2)
			{
			int rhash = atoi(ptr2  + 1);
			str.Format("Got rhash=%d hash=%d" , rhash, hash);
			m_log.AddLine(str);
			}
		}
	else
		{	
		P2N("Receive terminated in error.\r\n");
		}

endd:
	in_recv = false;

	P2N("Transmit time %d seconds hash=%d\r\n", (clock() - startc) / CLOCKS_PER_SEC, hash);

	//m_status.SetText((const char*)line);
}

//////////////////////////////////////////////////////////////////////////
// Get accumulated data from line/char transmission queue,
// Reset line buffering pointers for more work
//
// Function name:    CClientDlg::GetOverFlow
// Description:      <empty description>
// Return type:		   
// Argument:         char *chr -> pointer to buffer to copy to
// Argument:         int *len  -> pointer to length, set to max buffer length
//
//////////////////////////////////////////////////////////////////////////

void		CClientDlg::GetOverFlow(char *chr, int *len)

{
	//P2N("rprog=%d buffprog=%d len=%d\r\n", rprog, buffprog, *len);

	if(rprog > buffprog)
		{
		*len = min(rprog - buffprog, *len);
		memcpy(chr, buffer + buffprog, *len);
		P2N("Overflow resudial len=%d buffer='%s'\r\n", *len, buffer + buffprog);
		}
	else
		{
		*len = 0;
		}

	rprog = buffprog = 0;	
}

//////////////////////////////////////////////////////////////////////////
// Get remote character via buffer receive
// 

int		CClientDlg::GetRemChar(char *chr, int *succ)

{
	*chr  = 0;			// Clear them right away

	if(buffprog >= rprog)
		{
		// Get new buffer

		int ret = m_sock1.Receive(buffer, _MAX_PATH);	
		if(ret > 0)
			{
			buffer[ret] = 0;
			rprog = ret;
			buffprog = 0;
			//P2N("RECEIVED: %s\r\n", buffer);
			}
		else
			{
			// Set error condition
			*succ = -1;
			return 0;
			}
		}

	*chr = buffer[buffprog];
	buffprog++;
	*succ = 0;			
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////
// Get remote line via buffer receive

int CClientDlg::GetRemLine(char *line, int maxlen, int *serr)

{
	int		prog = 0;

	while(true)
		{
		char chh;
		
		GetRemChar(&chh, serr);

		if(chh == '\n')
			{
			line[prog] = 0;
			prog = 0;
			break;
			}
		else if(chh == '\r')
			{
			// Ignore
			}
		else
			{
			if(prog < maxlen)
				line[prog++] = chh;
			else
				{
				P2N("Line lenght overflow\r\n");
				}
			}

		if(*serr < 0)
			{
			P2N("GetRemLine: Encountered error\r\n");
			break;
			}
		}

	return 0;
}

void CClientDlg::OnDestroy() 

{
	CDialog::OnDestroy();

	P2N("destroy dlg\r\n");

	if(m_sock1)
		{
		//if(m_sock1.IsBlocking())
			{		
			//P2N("destroy isblocking\r\n");
			}
		//m_sock1.CancelBlockingCall();
		m_sock1.Close();
		//delete m_sock1;
		}	

	if(buffpass) free(buffpass);
}

void CClientDlg::OnClose() 
{
	P2N("WM_close\r\n");	
	CDialog::OnClose();
}

void CClientDlg::OnEndSession(BOOL bEnding) 

{
	CDialog::OnEndSession(bEnding);
	P2N("End Session\r\n")	;
}

BOOL CClientDlg::OnQueryEndSession() 
{
	P2N("Query End Session\r\n");

	if (!CDialog::OnQueryEndSession())
		return FALSE;
		
	return TRUE;
}

void CClientDlg::OnOK() 
{
	if(m_sock1)
		{
		//m_sock1.CancelBlockingCall();
		m_sock1.Close();
		}
	
	CDialog::OnOK();
}

void CClientDlg::OnButton8() 

{
	int linenum = 0;

	char	line[_MAX_PATH];
	int		serr;

	if(!m_sock1)
		return;

	// Send command
	CString str("help\r\n"); 
	int sent = m_sock1.Send(str, str.GetLength());

	if(sent < 0)
		{
		P2N("Socket error on send command. %s\r\n", sockerr2str(GetLastError()) );
		m_log.AddLine(line);
		return;
		}

	GetRemLine(line, _MAX_PATH, &serr);

	if(serr < 0)
		{
		P2N("Socket error on recv command. %s\r\n", sockerr2str(GetLastError()) );
		return;
		}

	m_log.AddLine(line);

	if(atoi(line) != 214)
		{
		P2N("Error on command. Server said: %s\r\n", line);
		
		return;
		}

		while(true)
			{	
			GetRemLine(line, _MAX_PATH, &serr);
			
			if(serr < 0)
				break;

			m_log.AddLine(line);

			// See if terminating condition
			if(strncmp(line, "214 ", 4) == 0)
				{
				P2N("Final line '%s'\r\n", line);
				m_status.SetText(line);
				break;
				}

			P2N("help line: '%s'\r\n", line);
		
			linenum++;
			}

	P2N("%d lines received\r\n", linenum);
}


void CClientDlg::OnButton9() 

{
	char	line[_MAX_PATH];
	int		serr;

	if(!m_sock1)
		return;

	// Send command
	CString str("pwd\r\n"); 
	int sent = m_sock1.Send(str, str.GetLength());

	if(sent < 0)
		{
		P2N("Socket error on send command. %s\r\n", sockerr2str(GetLastError()) );
		return;
		}

	GetRemLine(line, _MAX_PATH, &serr);
	m_log.AddLine(line);

	if(serr < 0)
		{
		P2N("Socket error on recv command. %s\r\n", sockerr2str(GetLastError()) );
		return;
		}

	if(atoi(line) != 257)
		{
		P2N("Error on command. Server said: %s\r\n", line);
		return;
		}

	P2N("Got dir: '%s'\r\n", line + 4);

	m_status.SetText(line);
}

void CClientDlg::OnButton10() 

{
	CString str("stat\r\n");
	PingPong(str);
}

void CClientDlg::OnButton11() 

{
	int linenum = 0;

	//char	line[_MAX_PATH + 1];
	//int		serr;

	if(!m_sock1)
		return;

	abort = true;

	m_status.SetText("Aborting, Please wait ...");
	
	YieldToWinEx();
	Sleep(300);

	CString str("ABOR\r\n"); 
	m_sock1.Send(str, str.GetLength(), MSG_OOB);

	YieldToWinEx();
	Sleep(300);

	// Drain what is in the buffer
	m_sock1.DrainRecv(NULL, 0);
	Sleep(300);
	m_sock1.DrainRecv(NULL, 0);

	buffprog = rprog = 0;

	m_status.SetText("Aborted Transfer.");
}

void CClientDlg::OnButton12() 

{
	m_log.Clear();	
}

void CClientDlg::OnButton13() 

{ 
	UpdateData();
	CString str(m_com);
	str += "\r\n";
	m_com = "";
	UpdateData(false);

	P2N("Executing: '%s'", str);

	PingPong(str);
}

//////////////////////////////////////////////////////////////////////////

void CClientDlg::AllButtons(int enable)

{
	//m_con.EnableWindow(enable);
	m_Exec.EnableWindow(enable)	;
	//m_Exit.EnableWindow(enable);
	m_Cancel.EnableWindow(enable);
	//m_ClearLog.EnableWindow(enable);
	m_Disconnect.EnableWindow(enable);
	m_Login.EnableWindow(enable);
	m_LS.EnableWindow(enable);
	m_Slst.EnableWindow(enable);
	m_Recv.EnableWindow(enable);
	m_Abort.EnableWindow(enable);
	m_Help.EnableWindow(enable);
	m_Stat.EnableWindow(enable);
	m_PWD.EnableWindow(enable);
	m_eKey.EnableWindow(enable);	

	m_b14.EnableWindow(enable);
	m_b15.EnableWindow(enable);
	m_b16.EnableWindow(enable);
	m_b17.EnableWindow(enable);
}

//////////////////////////////////////////////////////////////////////////

int		CClientDlg::PingPong(CString &str)

{
	char line[_MAX_PATH + 1];
	int serr;

	// New command initiates, drain socket
	m_sock1.DrainRecv(NULL, 0);

	int sent = m_sock1.Send(str);

	if(sent < 0)
		{
		P2N("Socket error on send command. %s\r\n", sockerr2str(GetLastError()) );
		return 0;
		}

	int first = 0;

	GetRemLine(line, _MAX_PATH, &serr);
	if(serr < 0)
		{
		P2N("Socket error on recv command. %s\r\n", sockerr2str(GetLastError()) );
		return 0;
		}

	m_log.AddLine(line);
	AP2N(line); AP2N("\r\n");

	// Line continuation strings
	if(line[0] == '2' && line[3] == '-' )
		{
		while(true)
			{
			GetRemLine(line, _MAX_PATH, &serr);

			if(serr < 0)
				{
				P2N("Socket error on recv command. %s\r\n", sockerr2str(GetLastError()) );
				return 0;
				}
			m_log.AddLine(line);

			//AP2N(line); AP2N("\r\n");

			if(line[0] == '2')
				{
				if(line[3] == ' ' )
					{
					break;
					}
				}
			else if(line[0] == '5')
				{
				m_log.AddLine("Err");
				}
			}	
		}	
	return 0;
}

void CClientDlg::OnButton15() 

{
	char	line[_MAX_PATH];
	int		serr;

	if(!m_sock1)
		return;

	// New command initiates, drain socket
	m_sock1.DrainRecv(NULL, 0);

	// Send command
	CString str("echal\r\n"); 
	int sent = m_sock1.Send(str, str.GetLength());

	if(sent < 0)
		{
		P2N("Socket error on send command. %s\r\n", sockerr2str(GetLastError()) );
		return;
		}

	GetRemLine(line, _MAX_PATH, &serr);
	m_log.AddLine(line);

	if(serr < 0)
		{
		P2N("Socket error on recv command. %s\r\n", sockerr2str(GetLastError()) );
		return;
		}

	if(atoi(line) != 200)
		{
		P2N("Error on command. Server said: %s\r\n", line);
		return;
		}

	char *ptr = strchr(line, '\'');
	if(ptr)
		{
		char *ptr2 = strchr(ptr+1, '\'');
		if(ptr2)
			{
			*ptr2 = 0;
			echal = ptr+1;
			P2N("Got key: [%s]\r\n", echal);
			
			m_status.SetText(echal);
			}
		}
}

//////////////////////////////////////////////////////////////////////////

void CClientDlg::OnButton16() 

{
	char outb[2 * PASSLIM + 4];
	int olen = sizeof(outb);
	
	//char *pass = "p";

	CString pass;
	((CEdit*)GetDlgItem(IDC_EDIT3))->GetWindowText(pass);

	int len = echal.GetLength();

	//if(!len)
	//	{
	//	m_log.AddLine("No challange str yet");
	//	P2N("No challange str yet\r\n");
	//	return;
	//	}
	
	if(buffpass) free(buffpass);

	buffpass = (char*)malloc(len + 1);
	ASSERT(buffpass);

	// Duplicate string in memory
	memcpy(buffpass, echal, len); buffpass[len] = 0;

	bluepoint_encrypt(buffpass, len, pass, strlen(pass));
	bluepoint_tohex(buffpass, len, outb, &olen);

	P2N("tohex '%s'\r\n", outb);

	//char outb2[2 * PASSLIM + 4];
	//int olen2 = sizeof(outb);
	//bluepoint_fromhex(outb, olen, outb2, &olen2);
	//P2N("fromhex '%s'\r\n", outb2);
	//support.HeapCheck("ePass", false);

	CString cmd("epass ");	cmd += outb; cmd += "\r\n";
	PingPong(cmd);
}

void CClientDlg::OnButton17() 

{
	char outb[2 * PASSLIM + 4];
	int olen = sizeof(outb);
	outb[olen - 1] = 0;
	
	CString pass;
	((CEdit*)GetDlgItem(IDC_EDIT4))->GetWindowText(pass);

	if(buffpass) free(buffpass);

	int len = max(echal.GetLength(), (int)strlen(pass));

	buffpass = (char*)malloc(len + 1);
	ASSERT(buffpass);

	memset(buffpass, 0, len);
	buffpass[len] = 0;

	strcpy(buffpass, pass);

	//P2N("buffpass '%s'\r\n", buffpass);

	bluepoint_encrypt(buffpass, len, ekey, ekey.GetLength());
	bluepoint_tohex(buffpass, len, outb, &olen);

	//P2N("tohex '%s'\r\n", outb);

	CString cmd("eusr ");	cmd += outb; cmd += "\r\n";
	PingPong(cmd);
}

void CClientDlg::OnButton18() 

{
	char	*ptr;
	char	line[_MAX_PATH + 1];
	int		serr, len = 0;
	int		oldc, prog = 0;
	int		startc = clock();
	unsigned int		rhash = 0, hash = 0;

	CString str, str2; 
	ULONGLONG fsize, xlen = 0;

	// CFile
	CxFile fp;

	abort = false;

	//P2N("Storing %s\r\n");

	if(in_send)
		{	
		P2N("In send already\r\n");
		m_status.SetText("Reentered send");
		return;
		} 

	in_send = true; 

	CString filename;
	((CEdit*)GetDlgItem(IDC_EDIT6))->GetWindowText(filename);
	
	int ret = fp.Open(filename, CFile::modeRead);
	if(!ret)
		{
		AfxMessageBox("Cannot open upload file\r\n");
		goto endd;
		}

	fsize = fp.GetLength();
	str; str.Format("stor %s\r\n", filename); 

	m_sock1.Send(str, str.GetLength());

	GetRemLine(line, _MAX_PATH, &serr);
	
	if(serr < 0)
		{
		m_log.AddLine("Failed to receive reply line");
		P2N("Failed to receive reply line\r\n");
		goto endd;
		}	
	m_log.AddLine(line);

	if(line[0] != '2')
		{	
		//m_log.AddLine("Error on init store");
		goto endd; 
		}

	str2.Format("DATA %d\r\n", fsize); 
	m_sock1.Send(str2, str2.GetLength());

	GetRemLine(line, _MAX_PATH, &serr);
	if(serr < 0)
		{
		m_log.AddLine("Failed to receive reply line");
		P2N("Failed to receive reply line\r\n");
		goto endd;
		}	
	m_log.AddLine(line);
	
	oldc = clock();
	xlen = fsize;

	while(true)
		{
		int left = min(sizeof(cryptbuf), (int)fsize);
		int ret = fp.Read(cryptbuf, left);

		hashfunc(cryptbuf, ret, &hash);

		if(ret == 0)
			break;

		if(abort)
			break;

		m_sock1.Send(cryptbuf, ret);
		
		//AP2N("sent buffer %d\r\n", ret);

		fsize -= ret;
		prog  += ret;

		if(fsize == 0)
			break;

		int newc = clock();
		if((newc - oldc) > 500)
			{
			int speed = 0;
			int elapsed = (newc - startc);

			if(elapsed > 0)	
				{
				speed = prog / elapsed;
				}
			CString str; str.Format("Progress %d %d kbytes/sec", prog, speed);
			m_status.SetText(str);
			oldc = newc;
			}
		YieldToWinEx();			
		}

	if(abort)
		{
		goto endd;
		}

	P2N("Sent %s hash=%d\r\n", filename, hash);
	GetRemLine(line, _MAX_PATH, &serr);
	if(serr < 0)
		{
		m_log.AddLine("Failed to receive reply line");
		P2N("Failed to receive reply line\r\n");
		goto endd;
		}	
	m_log.AddLine(line);
	
	// Find the quotes
	ptr = strchr(line, '\'');
	if(ptr)
		{
		rhash = atoi(ptr + 1 );
		str.Format("Local hash: %d Remote hash: %d", hash, rhash);
		m_log.AddLine(str);
		if(hash != rhash)
			{
			m_log.AddLine("Error on transfer.");
			}
		}	
endd:
	in_send = false;
}


void CClientDlg::OnButton14() 

{
	CString filename;
	((CEdit*)GetDlgItem(IDC_EDIT7))->GetWindowText(filename);

	CString str = "del " + filename + "\r\n";
	PingPong(str);
}

void CClientDlg::OnButton19() 

{
	char	line[_MAX_PATH + 1];
	int		serr, len = 0, xlen = 0;
	int		blocksize;
	int		oldc, olen, ret;
	int		startc = clock();
	unsigned int		hash = 0;

	CxFile fp;

	abort = false;

	if(in_recv)
		{	
		P2N("In receive already\r\n");
		m_status.SetText("Reentered receive");
		return;
		} 

	in_recv = true;

	CString str, file;
	((CEdit*)GetDlgItem(IDC_EDIT5))->GetWindowText(file);
	str = "eretr " + file + "\r\n";

	//CString str("retr test/save\r\n"); 
	//CString str("eretr test/rfcxxxx.txt\r\n"); 
	//CString str("eretr testaa\r\n"); 
	//CString str("retr test/all.c\r\n"); 

	ret = fp.Open("download.dat", CFile::modeCreate | CFile::modeWrite);
	if(!ret)
		{
		AfxMessageBox("Cannot create download file\r\n");
		goto endd;
		}

	m_sock1.Send(str, str.GetLength());

	GetRemLine(line, _MAX_PATH, &serr);
	
	if(serr < 0)
		{
		P2N("Failed to receive reply line\r\n");
		goto endd;
		}

	m_log.AddLine(line);

	//P2N("Got announcement reply: %s\r\n", line);

	if(atoi(line) == 211)
		{
		// Find the quotes
		char *ptr = strchr(line, '\'');
		if(ptr)
			{
			xlen = len = atoi(ptr + 1 );
			char *ptr2 = strchr(ptr + 1, '\'');
			if(ptr2)
				{
				char *ptr3 = strchr(ptr2 + 1, '\'');
				if(ptr3)
					{
					blocksize = atoi(ptr3 + 1 );
					}
				}
			}

		P2N("Got transfer announcement: status=%d len=%d blocksize=%d\r\n", 
					atoi(line), len, blocksize);
		}
	else
		{
		m_status.SetText(line);
		
		P2N("Error on command. Server said: %s\r\n", line);	
		goto endd;
		}	

	if(len == 0)
		{
		m_log.AddLine("Malformed reply on eretr");
		goto endd;
		}

	// See how much we already have:
	olen = BUFFSIZE;
	GetOverFlow(buf, &olen);

	lprog = 0;
	if(olen > 0)
		{
		P2N("Overflow %d bytes\r\n", olen);

		if(olen < sizeof(cryptbuf))
			{
			memcpy(cryptbuf, buf, olen);
			lprog += olen;
			}
		len -= olen;
		}
	else
		{
		P2N("No overflow\r\n");
		}

	oldc = clock();

	// Receive subscribed data:
	while(true)
		{
		int get = BUFFSIZE;

		if(get > len) 
			get = len;

		int ret = m_sock1.Receive(buf, get);
		
		if(ret > 0)
			{
			// ---------------------------
			// ----------------          ^sizeof(cryptbuf) -> BUFFSIZE
			//                 ^lprog							
			//                 --------------------
			//                 ^  diff   ^        ^ret

			if(lprog + ret >= BUFFSIZE)		// Accumulate full size buffers		
				{
				int diff = BUFFSIZE - lprog;
				
				//P2N("Re-buffer %d\r\n", diff);

				memcpy(cryptbuf + lprog, buf, diff);
				lprog += diff;
				
				//P2N("decrypting %d bytes\r\n", sizeof(cryptbuf));

				//bluepoint_decrypt(cryptbuf, BUFFSIZE, "hello", 5);
				bluepoint_decrypt(cryptbuf, BUFFSIZE, ekey, ekey.GetLength());

				hashfunc(cryptbuf, BUFFSIZE, &hash);
				fp.Write(cryptbuf, BUFFSIZE);
	
				if(ret - diff)
					memcpy(cryptbuf, buf + diff, ret - diff);

				lprog = ret - diff;
				}
			else						// Accumulate partial size buffers		
				{
				//P2N("Partial Re-buffer %d\r\n", ret);
				memcpy(cryptbuf + lprog, buf, ret);
				lprog += ret;
				}

			int newc = clock();
			if((newc - oldc) > 500)
				{
				int speed = 0;
				int elapsed = (newc - startc);

				if(elapsed > 0)	
					{
					speed = (xlen - len) / elapsed;
					}

				CString str; str.Format("Progress %d %d kbytes/sec", len, speed);
				m_status.SetText(str);

				//P2N("transfer at: %d bytes left %d clock=%d\r\n", ret, len, newc);
				oldc = newc;
				}

			YieldToWinEx();		
				
			len -= ret;

			if(abort)
				break;

			if(len <= 0)	
				{				
				// Last buffer, decrypt
				if(lprog)
					{
					//P2N("Last buffer %d\r\n", ret);

					bluepoint_decrypt(cryptbuf, lprog, ekey, ekey.GetLength());
					//bluepoint_decrypt(cryptbuf, lprog, "hello", 5);

					hashfunc(cryptbuf, lprog, &hash);
					fp.Write(cryptbuf, lprog);
					lprog = 0;
					}
				break;
				}
			}	
		else
			{
			P2N("Error in recv\r\n");
			break;
			}
		}

	fp.Close();

	if(abort)
		{
		abort = false;

		//P2N("abort buffer %s\r\n", buf);

		// Eval reply
		//GetRemLine(line, _MAX_PATH, &serr);
		//P2N("Abort line %s\r\n", line);

		//if(atoi(line) != 211)
		//	{
		//	m_status.SetText(line);
		//	P2N("Error on command. Server said: %s\r\n", line);
		//	return;
		//	}	
		}
 
	else if(ret > 0)
		{
		// Get terminating line:
		GetRemLine(line, _MAX_PATH, &serr);
		P2N("Got terminating : '%s'\r\n", line);

		// Get conclusion line:
		GetRemLine(line, _MAX_PATH, &serr);

		P2N("Got conclusion reply: '%s'\r\n", line);

		m_log.AddLine(line);
		m_status.SetText(line);

		// Find the quotes
		char *ptr2 = strchr(line, '\'');
		if(ptr2)
			{
			int rhash = atoi(ptr2  + 1);
			str.Format("Got rhash=%d hash=%d" , rhash, hash);
			m_log.AddLine(str);
			}
		}
	else
		{	
		P2N("Receive terminated in error.\r\n");
		}

endd:
	in_recv = false;

	P2N("Transmit time %d seconds hash=%d\r\n", (clock() - startc) / CLOCKS_PER_SEC, hash);

	//m_status.SetText((const char*)line);
}

int CClientDlg::Decrypt(char *buf, int len)

{
	int ret = 0;

	return ret;
}

void CClientDlg::OnButton20() 

{
	char	line[_MAX_PATH + 1];
	char	*ptr;
	int		serr, len = 0, xlen = 0;
	int	oldc = 0, prog = 0;
	int		startc = clock();
	unsigned int rhash = 0, hash = 0;

	CString str, str2; 
	ULONGLONG fsize;

	// CFile
	CxFile fp;

	abort = false;

	if(in_send)
		{	
		P2N("In send already\r\n");
		m_status.SetText("Reentered send");
		return;
		} 

	in_send = true; 

	CString filename;
	((CEdit*)GetDlgItem(IDC_EDIT6))->GetWindowText(filename);

	int ret = fp.Open(filename, CFile::modeRead);
	if(!ret)
		{
		AfxMessageBox("Cannot open upload file\r\n");
		goto endd;
		}

	fsize = fp.GetLength();
	str; str.Format("estor %s\r\n", filename); 

	m_sock1.Send(str, str.GetLength());

	GetRemLine(line, _MAX_PATH, &serr);
	
	if(serr < 0)
		{
		m_log.AddLine("Failed to receive reply line");
		P2N("Failed to receive reply line\r\n");
		goto endd;
		}	
	m_log.AddLine(line);

	if(line[0] != '2')
		{	
		//m_log.AddLine("Error on init store");
		goto endd; 
		}

	str2.Format("EDATA %d\r\n", fsize); 
	m_sock1.Send(str2, str2.GetLength());

	GetRemLine(line, _MAX_PATH, &serr);
	if(serr < 0)
		{
		m_log.AddLine("Failed to receive reply line");
		P2N("Failed to receive reply line\r\n");
		goto endd;
		}	
	m_log.AddLine(line);

	// Test send timeout
	//goto endd;

	while(true)
		{
		int left = min(sizeof(cryptbuf), (int)fsize);
		int ret = fp.Read(cryptbuf, left);

		if(ret == 0)
			break;

		hashfunc(cryptbuf, ret, &hash);
		bluepoint_encrypt(cryptbuf, ret, ekey, ekey.GetLength());

		m_sock1.Send(cryptbuf, ret);
		
		fsize -= ret;
		prog += ret;

		if(fsize == 0)
			break;

		int newc = clock();
		if((newc - oldc) > 500)
			{
			int speed = 0;
			int elapsed = (newc - startc);

			if(elapsed > 0)	
				{
				speed = prog / elapsed;
				}
			CString str; str.Format("Progress %d %d kbytes/sec", prog, speed);
			m_status.SetText(str);
			oldc = newc;
			}
		YieldToWinEx();	
		}

	P2N("Sent %s hash=%d\r\n", filename, hash);

	GetRemLine(line, _MAX_PATH, &serr);

	if(serr < 0)
		{
		m_log.AddLine("Failed to receive reply line");
		P2N("Failed to receive reply line\r\n");
		goto endd;
		}	
	m_log.AddLine(line);
	
	// Find the quotes
	ptr = strchr(line, '\'');

	if(ptr)
		{
		rhash = atoi(ptr + 1 );
		str.Format("local hash: %d remote hash: %d\r\n", hash, rhash);
		m_log.AddLine(str);
		}
		
endd:
	in_send = false;

}

void CClientDlg::OnButton21() 
{
	CString str = "del download.dat\r\n";
	PingPong(str);	
}

void CClientDlg::OnButton22() 
{
	CString str = "NOOP\r\n";
	PingPong(str);	
	
}
