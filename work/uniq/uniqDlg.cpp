// uniqDlg.cpp : implementation file
//

#include "stdafx.h"
#include "uniq.h"
#include "uniqDlg.h"
#include "mxpad.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int     YieldToWinEx(HWND hWnd = NULL)

{
    int ret_val = 0;
    while(TRUE)
        {
        MSG msg;
        if(!PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE))
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
// CUniqDlg dialog

CUniqDlg::CUniqDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUniqDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUniqDlg)
	m_str = _T("");
	m_str2 = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	stop = false;
}

void CUniqDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUniqDlg)
	DDX_Text(pDX, IDC_EDIT1, m_str);
	DDX_Text(pDX, IDC_EDIT2, m_str2);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CUniqDlg, CDialog)
	//{{AFX_MSG_MAP(CUniqDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUniqDlg message handlers

BOOL CUniqDlg::OnInitDialog()
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
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CUniqDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CUniqDlg::OnPaint() 
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
HCURSOR CUniqDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CUniqDlg::OnButton1() 
{

	CUniqID id;

	m_str = id.GetStr();

	UpdateData(false);
}


int in_prog = 0;

void CUniqDlg::OnButton2() 

{
	if(in_prog)
		{
		stop = true;
		return;
		}
	in_prog = 1;
	stop = 0;

	unsigned int oldc = clock();

	while(true)
		{
		CUniqID *id = new CUniqID;

		//ASSERT(id);

		int len = arr.GetSize();
		
		if(id == NULL)
			{
			m_str2.Format("Memoryu error at: %s %d", id->GetStr(), len);
			UpdateData(false);
			break;
			}

		for(int loop = 0; loop < len; loop++)
			{
			CUniqID *id2 = (CUniqID *)arr[loop];

			if(*id == *id2)
				{
				m_str2.Format("Duplicate: %s", id->GetStr());
				UpdateData(false);
				break;
				}
			}

		if(len % 10 == 0)
			{
			//P2N("Adding %p %d %s\r\n", id, len, id->GetStr());

			// Update only 4/sec
			unsigned int now = clock();
			if(now > (oldc + CLOCKS_PER_SEC / 4) )
				{
				oldc = now;
				Progress(id, len);
				}
			}

		arr.Add(id);

		//if(len > 10000)
		//	break;

		if(stop)
			break;
		}

	int len = arr.GetSize();
	for(int loop = 0; loop < len; loop++)
		{
		CUniqID *id2 = (CUniqID *)arr[loop];
		delete id2;
		}

	arr.RemoveAll();

	in_prog = 0;
}


void CUniqDlg::OnOK()
{
	stop = true;
	CDialog::OnOK();
}

void CUniqDlg::OnCancel()
{
	stop = true;
	CDialog::OnCancel();
}

void CUniqDlg::Progress(CUniqID *id, int len)

{
	m_str = id->GetStr();
	m_str2.Format("Scanning %d", len);
	UpdateData(false);
	YieldToWinEx();
}