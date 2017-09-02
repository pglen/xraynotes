// DCMConverterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DCMConverter.h"
#include "DCMConverterDlg.h"

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
// CDCMConverterDlg dialog

CDCMConverterDlg::CDCMConverterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDCMConverterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDCMConverterDlg)
	m_sDcmSrcFile = _T("");
	m_sBmpSrcFile = _T("");
	m_sJPGDestFile = _T("");
	m_sDcmDestFile = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDCMConverterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDCMConverterDlg)
	DDX_Text(pDX, IDC_EDIT_DCM_SOURCE_FILE, m_sDcmSrcFile);
	DDX_Text(pDX, IDC_EDIT_BMP_SOURCE_FILE, m_sBmpSrcFile);
	DDX_Text(pDX, IDC_EDIT_DEST_BMP_FILE, m_sJPGDestFile);
	DDX_Text(pDX, IDC_EDIT_DCM_DEST_FILE, m_sDcmDestFile);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDCMConverterDlg, CDialog)
	//{{AFX_MSG_MAP(CDCMConverterDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CONVERT, OnButtonConvert)
	ON_BN_CLICKED(IDC_BUTTON_CONVERT2DCM, OnButtonConvert2dcm)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_DCM, OnButtonOpenDcm)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_BMP, OnButtonOpenBmp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDCMConverterDlg message handlers

BOOL CDCMConverterDlg::OnInitDialog()
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

void CDCMConverterDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDCMConverterDlg::OnPaint() 
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
		CRect rect;
		GetDlgItem(IDC_IMAGE_CONTAINER)->GetWindowRect(rect);
		ScreenToClient(rect);
		if(dcm.IsValid())
			dcm.Draw2(GetDC()->m_hDC,rect);
		
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDCMConverterDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CDCMConverterDlg::OnButtonOpenDcm() 
{
	CFileDialog dlg(TRUE,NULL,NULL,OFN_OVERWRITEPROMPT, "DCM Files (*.dcm)|*.dcm|");
	if(dlg.DoModal()!=IDOK) return;
	dcm.LoadDCM(dlg.GetFileName());
	if(!dcm.IsValid()){
		AfxMessageBox("Load dicom file failed!");
	
	}else{		
		m_sDcmSrcFile=dlg.GetPathName();
		m_sJPGDestFile=m_sDcmSrcFile;
		m_sJPGDestFile.TrimRight(dlg.GetFileExt());
		m_sJPGDestFile+=_T("jpg");

		UpdateData(FALSE);
		Invalidate(TRUE);
		GetDlgItem(IDC_BUTTON_CONVERT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_CONVERT2DCM)->EnableWindow(FALSE);
	}		
}

void CDCMConverterDlg::OnButtonConvert() 
{
	CString sMsg;
	sMsg.Format("Save JPG file to '%s' ",m_sJPGDestFile);
	if(dcm.SaveAsJPG(m_sJPGDestFile)){
		sMsg+="successfully.";

	}else{
		sMsg+="failed!";
	}
	AfxMessageBox(sMsg);
}


void CDCMConverterDlg::OnButtonOpenBmp() 
{
	CFileDialog dlg(TRUE,NULL,NULL,OFN_OVERWRITEPROMPT,"JPG Files(*.jpg)|*.jpg|BMP Files (*.bmp)|*.bmp|TIFF Files(*.tif)|*.tif||");
	if(dlg.DoModal()!=IDOK) return;
	dcm.Load(dlg.GetFileName(),CXIMAGE_FORMAT_BMP);
	if(!dcm.IsValid()){
		AfxMessageBox("Load bmp file failed!");
	
	}else{
		m_sBmpSrcFile=dlg.GetPathName();
		m_sDcmDestFile=m_sBmpSrcFile;
		m_sDcmDestFile.TrimRight(dlg.GetFileExt());
		m_sDcmDestFile+=_T("dcm");

		UpdateData(FALSE);
		Invalidate(TRUE);
		GetDlgItem(IDC_BUTTON_CONVERT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CONVERT2DCM)->EnableWindow(TRUE);
	}
	
}

void CDCMConverterDlg::OnButtonConvert2dcm() 
{
	CString sMsg;
	sMsg.Format("Save dcm file to '%s' ",m_sDcmDestFile);
	if(dcm.SaveAsDCM(m_sDcmDestFile)){
		sMsg+="successfully.";

	}else{
		sMsg+="failed!";
	}
	AfxMessageBox(sMsg);
	
}


