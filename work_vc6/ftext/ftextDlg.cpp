
/* =====[ ftextDlg.cpp ]========================================== 
                                                                             
   Description:     The ftext project, implementation of the ftextDlg.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  6/15/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// ftextDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ftext.h"
#include "ftextDlg.h"


#include "mxpad.h"
#include "xPtrArr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef WS_EX_LAYERED
#define WS_EX_LAYERED 0x00080000 
#define LWA_COLORKEY 1 // Use color as the transparency color.
#define LWA_ALPHA    2 // Use bAlpha to determine the opacity of the layer
#endif

//  Function pointer for lyering API in User32.dll
typedef BOOL (WINAPI *lpfnSetLayeredWindowAttributes)
            (HWND hWnd, COLORREF cr, BYTE bAlpha, DWORD dwFlags);


char *sizetable[] = 
{
	"7",
	"8",
	"9",
	"10",
	"11",
	"12",
	"14",
	"16",
	"18",
	"24",
	"32",
	"48",
	"64",
	"72",
	"96",
	"128"
};

char *coltable[] = 

{
	"Black",
	"White",
	"Gray",
	"DarkGray", 
	"Red",
	"Green",
	"Blue",
	"Custom"
};


// Keep it sync with the coltable

int		colvalues[] = 

{
	RGB(0,0,0),			//"Black",
	RGB(255,255,255),	//"White",
	RGB(128,128,128),	//"Gray",
	RGB(64,64,64),		//"DarkGray", 
	RGB(255,0,0),		//"Red",
	RGB(0,255,0),		//"Green",
	RGB(0,0,255),		//"Blue"
	RGB(0,0,0)			//"Custom"
};


char *fonttable[] = 

{
	"MS Sans Serif",
	"Arial", 
	"Courier New",
	"Tahoma", 
	"Times New Roman",
	"Verdana", 
	"Lucida Console",
	"MS Serif", 
	"Helvetica"
};

lpfnSetLayeredWindowAttributes g_pSetLayeredWindowAttributes = NULL;

//int fxcount = 0;

int CALLBACK  EnumFontsProc( const LOGFONT *lplf,     
		const TEXTMETRIC *lptm, DWORD dwType, LPARAM lpData )

{
	CFtextDlg *dlg = (CFtextDlg*)lpData; ASSERT(dlg);

	//P2N(" %s %d/%d\r\n", lplf->lfFaceName, lplf->lfHeight, lplf->lfWidth);
	CString str(lplf->lfFaceName);

	//if(lplf->lfPitchAndFamily & FIXED_PITCH)
	//	{
	//	str += " (Fixed Pitch) ";	
	//	}

	dlg->m_fonts.AddString(str);

#if 0
	P2N("LONG lfHeight;                 %d\r\n", lplf->lfHeight); 
	P2N("LONG lfWidth;                  %d\r\n", lplf->lfWidth); 
	P2N("LONG lfEscapement;             %d\r\n", lplf->lfEscapement); 
	P2N("LONG lfOrientation;            %d\r\n", lplf->lfOrientation); 
	P2N("LONG lfWeight;                 %d\r\n", lplf->lfWeight); 
	P2N("BYTE lfItalic;                 %d\r\n", lplf->lfItalic); 
	P2N("BYTE lfUnderline;              %d\r\n", lplf->lfUnderline); 
	P2N("BYTE lfStrikeOut;              %d\r\n", lplf->lfStrikeOut); 
	P2N("BYTE lfCharSet;                %d\r\n", lplf->lfCharSet); 
	P2N("BYTE lfOutPrecision;           %d\r\n", lplf->lfOutPrecision); 
	P2N("BYTE lfClipPrecision;          %d\r\n", lplf->lfClipPrecision); 
	P2N("BYTE lfQuality;                %d\r\n", lplf->lfQuality); 
	P2N("BYTE lfPitchAndFamily;         %d\r\n", lplf->lfPitchAndFamily); 
	P2N("TCHAR lfFaceName[LF_FACESIZE]; %s\r\n", lplf->lfFaceName);
#endif

	// List only the first xx
	return --dlg->fxcount; // > 20000 ? 0 : 1;
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
// CFtextDlg dialog

CFtextDlg::CFtextDlg(CWnd* pParent /*=NULL*/)
	: CGrayDlg(CFtextDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFtextDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	HMODULE hUser32 = GetModuleHandle(_T("USER32.DLL"));
	g_pSetLayeredWindowAttributes = (lpfnSetLayeredWindowAttributes)
                        GetProcAddress(hUser32, "SetLayeredWindowAttributes");

	ts = NULL;
	m_title = "Proportional font Text Edit";
	noremember = true;
	fxcount = first = 0;
}

CFtextDlg::~CFtextDlg()

{
	if(ts)
		delete ts;
}

void CFtextDlg::DoDataExchange(CDataExchange* pDX)
{
	CGrayDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFtextDlg)
	DDX_Control(pDX, IDC_EDIT1, m_ed3);
	DDX_Control(pDX, IDC_COMBO2, m_size);
	DDX_Control(pDX, IDC_COMBO3, m_col);
	DDX_Control(pDX, IDC_COMBO1, m_fonts);
	DDX_Control(pDX, IDC_ED, m_ed2);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFtextDlg, CGrayDlg)
	//{{AFX_MSG_MAP(CFtextDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	ON_BN_CLICKED(IDC_BUTTON8, OnButton8)
	ON_BN_CLICKED(IDC_BUTTON9, OnButton9)
	ON_BN_CLICKED(IDC_BUTTON10, OnButton10)
	ON_BN_CLICKED(IDC_BUTTON11, OnButton11)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	ON_CBN_SELCHANGE(IDC_COMBO2, OnSelchangeCombo2)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON12, OnButton12)
	ON_BN_CLICKED(IDC_BUTTON13, OnButton13)
	ON_BN_CLICKED(IDC_BUTTON14, OnButton14)
	ON_CBN_SELCHANGE(IDC_COMBO3, OnSelchangeCombo3)
	ON_BN_CLICKED(IDC_BUTTON15, OnButton15)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON16, OnButton16)
	ON_BN_CLICKED(IDC_BUTTON17, OnButton17)
	ON_BN_CLICKED(IDC_BUTTON18, OnButton18)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFtextDlg message handlers

BOOL CFtextDlg::OnInitDialog()

{
	CGrayDlg::OnInitDialog();

	P2N("CFtextDlg::OnInitDialog()\r\n");

	//SetFocus();
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

	// Set the icon for this dialog. The framework does this 
	// automatically when the application's main window is not a dialog

	//////////////////////////////////////////////////////////////////////////
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	//RECT rc; GetClientRect(&rc);
	//OnSize(1, rc.right, rc.bottom);

	// CFont
	//m_fonts.AddString("Arial");
	//m_fonts.AddString("Times");
	//m_fonts.AddString("Times");

	LOGFONT logfont;  // pointer to logical font information
	memset(&logfont, 0, sizeof(logfont));

	CPaintDC dc(this); // device context for painting

	LOGFONT		m_lf;
	GetObject((HFONT)GetStockObject(DEFAULT_GUI_FONT), sizeof(m_lf), &m_lf);

	//P2N("Gui default %d\r\n", m_lf.lfHeight);
	//fxcount = 1;
	//EnumFontFamiliesEx(dc, &m_lf, EnumFontsProc, (DWORD)this, 0);

	for(int loops = 0; loops < sizeof(sizetable)/sizeof(char*); loops++)
		m_size.AddString(sizetable[loops]);

	for(int loopc = 0; loopc < sizeof(coltable)/sizeof(char*); loopc++)
		m_col.AddString(coltable[loopc]);

	for(int loop = 0; loop < sizeof(fonttable)/sizeof(char*); loop++)
		AddFont(fonttable[loop]);

	memset(&logfont, 0, sizeof(logfont));

	fxcount = 100;
	EnumFontFamiliesEx(dc, &logfont, EnumFontsProc, (DWORD)this, 0);

	m_fonts.SetCurSel(0);
	m_col.SetCurSel(0);

	CString num; int curr, prev = 0, found = 0;;
	for(int loops1 = 0; loops1 < sizeof(sizetable)/sizeof(char*); loops1++)
		{
		m_size.GetLBText(loops1, num);
		curr = atoi(num);
		
		if(abs(m_lf.lfHeight) > prev && abs(m_lf.lfHeight) <= curr)
			{
			//P2N("found %d\r\n", curr);
			m_size.SelectString(0, num);
			break;
			}		
		prev = curr;
		}


	Resize();

		ts = new CToolStrip(this);
	//ts.num_buttons = 9;
	ts->boddy = m_ed2.m_hWnd;
	//ts->Show();
	ts->Hide();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFtextDlg::OnSysCommand(UINT nID, LPARAM lParam)

{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
		{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
		}
	else
		{
		CGrayDlg::OnSysCommand(nID, lParam);
		}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFtextDlg::OnPaint() 

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
		CGrayDlg::OnPaint();
		}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.

HCURSOR CFtextDlg::OnQueryDragIcon()

{
	return (HCURSOR) m_hIcon;
}

void CFtextDlg::OnSize(UINT nType, int cx, int cy) 

{
	//P2N("CFtextDlg::OnSize cx=%d cy=%d\r\n", cx, cy);
	CGrayDlg::OnSize(nType, cx, cy);
	Resize();
}

//////////////////////////////////////////////////////////////////////////

void CFtextDlg::Resize()

{
	//return;

	// Make sure windows exist
	if(!IsWindow(m_hWnd) || !IsWindow(m_ed2.m_hWnd))
		return;

	if(!first++)
		{
		P2N("CFtextDlg::Resize()\r\n");

		GetWindowPlacement(&wpd); 
		//DUMPRECT(wpd.rcNormalPosition);	
	
		m_ed2.GetWindowPlacement(&wp_ed2);	
		//DUMPRECT(wp_ed2.rcNormalPosition);
		
		::GetWindowPlacement(GetDlgItem(IDOK)->m_hWnd, &wp_ok);	
		//DUMPRECT(wp_ok.rcNormalPosition);
		}


	RECT rc2; GetClientRect(&rc2);	//P2N_RECT(rc2);

	//return;

	WINDOWPLACEMENT wp;

	// Edit window
	m_ed2.GetWindowPlacement(&wp);	

	wp.rcNormalPosition.right = rc2.right - 
		(wpd.rcNormalPosition.right - wp_ed2.rcNormalPosition.right); 

	wp.rcNormalPosition.bottom = rc2.bottom - 
		(wpd.rcNormalPosition.bottom - wp_ed2.rcNormalPosition.bottom); 

	m_ed2.SetWindowPlacement(&wp);

	// OK button
	::GetWindowPlacement(GetDlgItem(IDOK)->m_hWnd, &wp);

	wp.rcNormalPosition.right = rc2.right - 
		(wpd.rcNormalPosition.right - wp_ok.rcNormalPosition.right); 

	wp.rcNormalPosition.left = wp.rcNormalPosition.right -
		(wp_ok.rcNormalPosition.right - wp_ok.rcNormalPosition.left); 

	wp.rcNormalPosition.bottom = rc2.bottom - 
		(wpd.rcNormalPosition.bottom - wp_ok.rcNormalPosition.bottom); 

	wp.rcNormalPosition.top = rc2.bottom -
		(wpd.rcNormalPosition.bottom - wp_ok.rcNormalPosition.top); 

	::SetWindowPlacement(GetDlgItem(IDOK)->m_hWnd, &wp);

	Invalidate();
}

void CFtextDlg::OnButton7() 

{
	m_ed2.ScrollHor(-1);	
}

void CFtextDlg::OnButton8() 
{
	m_ed2.ScrollHor(1);	
}

void CFtextDlg::OnButton9() 
{
	m_ed2.ScrollVer(-1);	
}

void CFtextDlg::OnButton10() 
{
	m_ed2.ScrollVer(1);	
}


void CFtextDlg::OnButton11() 

{
	LOGFONT logfont;  // pointer to logical font information
	memset(&logfont, 0, sizeof(logfont));

	CPaintDC dc(this); // device context for painting

	fxcount = 1;
	EnumFontFamiliesEx(dc, &logfont, EnumFontsProc, (DWORD)this,0);


#if 0

	CxPtrArr pa;

	P2N("test ptarr\r\n");
	
	pa.Add((void*)12341);
	pa.Add((void*)12342);
	pa += (void*)12343;

	pa.Dump();

	P2N("arr member %d\r\n", pa[0]);
	P2N("arr member %d\r\n", pa[1]);
	P2N("arr member %d\r\n", pa[2]);

	//try
	//	{
	//	void *aa = pa[3];
	//	}
	//catch(CxPtrArrExc E)
	//	{
	//	P2N("%s\r\n", E.ShowReason());
	//	}
	//catch(...)
	//	{
	//	P2N("%s\r\n", "caught");
	//	}	

	try
		{
		void *aa = pa[-1];
		}
	catch(CxPtrArr::CxPtrArrExc E)
		{
		P2N("%s\r\n", E.ShowReason());
		}

#endif

	m_ed2.SetFocus();
}

void CFtextDlg::OnButton5() 

{
	m_ed2.Bold(-1);
	m_ed2.SetFocus();
}

void CFtextDlg::OnButton6() 

{
	m_ed2.Italic(-1);	
	m_ed2.SetFocus();
}

void CFtextDlg::AddFont(const char *str) 

{
	//P2N("Adding %s\r\n", str);
	
	LOGFONT logfont; memset(&logfont, 0, sizeof(LOGFONT));

	CPaintDC dc(this);
	strcpy(logfont.lfFaceName, str); fxcount = 1;
	EnumFontFamiliesEx(dc, &logfont, EnumFontsProc, (DWORD)this, 0);
}

//////////////////////////////////////////////////////////////////////////

void CFtextDlg::OnSelchangeCombo2() 

{
	CComboBox *box = (CComboBox *)GetDlgItem(IDC_COMBO2); ASSERT(box);

	if(box->GetCurSel() < 0)
		return;

	CString str; box->GetLBText(box->GetCurSel(), str);
	m_ed2.Size(atoi(str));
	m_ed2.SetFocus();
}

void CFtextDlg::OnSelchangeCombo1() 

{
	CComboBox *box = (CComboBox *)GetDlgItem(IDC_COMBO1); ASSERT(box);

	CString str; box->GetLBText(box->GetCurSel(), str);
	m_ed2.Font(str);
	m_ed2.SetFocus();
}

void CFtextDlg::OnButton4() 

{
	m_ed2.Bold(false);	
	m_ed2.Italic(false);
	m_ed2.Underline(false);
	m_ed2.Strikeout(false);
	
	m_ed2.SetFocus();
}

void CFtextDlg::OnButton12() 
{
	CPaintDC dc(this); // device context for painting

	LOGFONT m_lf; memset(&m_lf, 0, sizeof(m_lf));
	//GetObject((HFONT)GetStockObject(DEFAULT_GUI_FONT), sizeof(m_lf), &m_lf);
	
	CHOOSEFONT cf; memset(&cf, 0, sizeof(cf));
	cf.lStructSize = sizeof(cf);
	cf.lpLogFont = &m_lf;
	//cf.Flags = CF_SCREENFONTS;
	cf.Flags = CF_BOTH;
	cf.hDC = dc;
	cf.hwndOwner = *this;
	
	ChooseFont(&cf);

	P2N("fontdlg ret=%d\r\n", CommDlgExtendedError ());
	
}

void CFtextDlg::OnButton13() 
{
	m_ed2.Underline(-1);	
	m_ed2.SetFocus();
		
}

void CFtextDlg::OnButton14() 
{
	m_ed2.Strikeout(-1);	
	m_ed2.SetFocus();	
}

//////////////////////////////////////////////////////////////////////////

void CFtextDlg::OnSelchangeCombo3() 

{
	if(m_col.GetCurSel() < 0)
		return;

	CString str; m_col.GetLBText(m_col.GetCurSel(), str);
	m_ed2.SetTextColor(colvalues[m_col.GetCurSel()]);

	P2N("color selected %d\r\n", colvalues[m_col.GetCurSel()]);

	m_ed2.SetFocus();
}


void CFtextDlg::OnButton15() 

{
	CColorDialog dlg; dlg.DoModal();
	COLORREF col = dlg.GetColor();

	colvalues[sizeof(coltable)/sizeof(char*)] = col;
	m_col.SelectString(0, "Custom");
	
	m_ed2.SetTextColor(col);
	m_ed2.SetFocus();	
}

//////////////////////////////////////////////////////////////////////////

void CFtextDlg::OnButton1() 

{
	m_ed2.SetAlign(A_LEFT);
	m_ed2.SetFocus();	
}

void CFtextDlg::OnButton2() 

{
	m_ed2.SetAlign(A_MID);
	m_ed2.SetFocus();	
}


void CFtextDlg::OnButton3() 

{
	m_ed2.SetAlign(A_RIGHT);
	m_ed2.SetFocus();	
}

void CFtextDlg::OnButton16() 
{
	m_ed2.Copy();
	m_ed2.SetFocus();	
}

void CFtextDlg::OnButton17() 
{
	m_ed2.Paste();
	m_ed2.SetFocus();	
}


void CFtextDlg::OnButton18() 

{
	//P2N("OnButton18\r\n");	

	//ts->num_buttons = 9;
	//ts->boddy = m_ed2.m_hWnd;
	//ts->Show();

	//m_ed2.SetFocus();

	m_ed2.DumpText();
}
