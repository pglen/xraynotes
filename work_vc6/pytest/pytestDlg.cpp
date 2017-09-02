// pytestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "pytest.h"
#include "pytestDlg.h"

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
	virtual BOOL OnInitDialog();
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
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPytestDlg dialog

CPytestDlg::CPytestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPytestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPytestDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	phand = NULL;
}

void CPytestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPytestDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPytestDlg, CDialog)
	//{{AFX_MSG_MAP(CPytestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPytestDlg message handlers

BOOL CPytestDlg::OnInitDialog()
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
	
	if(!phand)
		phand = LoadLibrary("python25.dll" );


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPytestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPytestDlg::OnPaint() 
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
HCURSOR CPytestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

#include "python.h"

//////////////////////////////////////////////////////////////////////////
// Pointer to functions

typedef PyAPI_FUNC(void)		(*pfv)();
typedef PyAPI_FUNC(PyObject *)	(*pfo)();
typedef PyAPI_FUNC(char *)		(*pfs)();
typedef PyAPI_FUNC(int)			(*pfo_osvar)(PyObject *, char *, ...);

//////////////////////////////////////////////////////////////////////////
// sprintf _cdecl

void CPytestDlg::OnButton1() 

{
	pfs	pPy_GetVersion                  = (pfs)GetProcAddress(phand, "Py_GetVersion");

	if(!pPy_GetVersion)
		{
		AfxMessageBox("Python function Py_GetVersion is not available.");
		}

	//CString tmp; tmp.Format("Returned %p\r\n", pPy_GetVersion);
	//AfxMessageBox(tmp);

	char *str = pPy_GetVersion();
	CString tmp2; tmp2.Format("Version %s\r\n", str);
	AfxMessageBox(tmp2);

	pfo_osvar	pPyArg_ParseTuple                  = (pfo_osvar)GetProcAddress(phand, "PyArg_ParseTuple");

	if(!pPyArg_ParseTuple)
		{
		AfxMessageBox("Python function pPyArg_ParseTuple is not available.");
		}

	const char *ss;

	PyObject pp;
	PyObject *args = &pp;

	if(!pPyArg_ParseTuple(args, "s", &ss) )
		return ;	
	
	//PyObject* ss = PyString_FromString(Py_GetVersion());

}


BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
