
/* =====[ shadesDlg.cpp ]==========================================

   Description:     The xraynotes project, implementation of the shadesDlg.cpp

                    Defines the behavior for the application.

   Compiled:        MS-VC 6.00

   Notes:           <Empty Notes>

   Revisions:

      REV     DATE        BY            DESCRIPTION
      ----  --------  -----------  ----------------------------
      0.00  3/3/2007  Peter Glen   Initial version.
	  0.00  9/10/2008 Peter Glen   PyDialog

   ======================================================================= */

#include "stdafx.h"
#include "winuser.h"

#include <stdio.h>
#include <io.h>
#include <direct.h>

#include "xraynotes.h"

#include "Splash.h"
#include "chevron.h"
#include "Flyout.h"
#include "Status.h"

#include "multimon.h"
#include "ColDlg.h"
#include "MagDlg.h"
#include "xScroll.h"
#include "yScroll.h"
#include "gestureDlg.h"
#include "ProcDlg.h"
#include "BriCon.h"
#include "DicomWin.h"
#include "PyGlg.h"
#include "TestDlg.h"
#include "XrayM.h"
#include "dbmain.h"
#include "MagWnd.h"
#include "xyzDlg.h"
#include "textprop.h"
#include "about.h"
#include "memdc.h"
#include "dicomfile.h"
#include "doclist.h"
#include "ToolShow.h"
#include "patdmg.h"
#include "AddrEntry.h"
#include "CurveDlg.h"
#include "Alpha.h"
#include "scrkbd.h"

#include "ShadesSel.h"

// Define this for flicker free drawing, undef for testing
#define USEMEMDC	1

CFlyout nwfo, swfo, nefo, sefo;

CChevron  ne(IDD_DIALOG15);
CChevron  se(IDD_DIALOG16);
CChevron  nw(IDD_DIALOG17);
CChevron  sw(IDD_DIALOG18);

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// Supplementary windows

CSplash		splash;

CxScroll    xscroll;
CyScroll    yscroll;
CMagDlg     magdlg;
CGestureDlg gesdlg;

//////////////////////////////////////////////////////////////////////////
// Helper windows

CProcDlg    procdlg;
CBriCon		bcondlg;
CColDlg     coldlg;
CDicomWin   dicomwin;
CPyGlg		pydlg;
CMagWnd		magwin;
CxyzDlg		xyzdlg;
CCurveDlg	cdlg;
CDBMain		dbmain;
CScrKbd		kbd;

//////////////////////////////////////////////////////////////////////////
// Window List (make sure it ends with NULL)

void	*winlist[] = {
					&ne, &nw, &se, &sw,
					&xscroll, &yscroll, &magdlg, &gesdlg,
					&procdlg, &bcondlg, &coldlg,
					&dicomwin, &pydlg,
					&magwin, &xyzdlg,  &cdlg,
					&dbmain, &kbd,
					NULL};

//////////////////////////////////////////////////////////////////////////
// Modal Dialogs

CStatus     *statusdlg = NULL;

//include	"functions.cpp"

/*!
    \page keystrokes Keyboard Shortcuts

	Keystrokes and hotkeys description for the XrayNotes Application.
	This page contains a list of keystrokes grouped by modifier.

    \section aaaa ALT keys

	  \li	\ref altkeys "Main Alt Key Combinations

    \section cccc CTRL-ALT keys

	\li	\ref ctrlaltkeys "Main Ctrl - Alt Key Combinations

  Define groups for DOXYGEN

	defgroup CTRLALT Ctrl Alt Keys
	defgroup ALT Alt Keys

*/

// \page ctrlkeys XrayNotes CTRL Keystrokes
// \page ctrlaltkeys XrayNotes CTRL-ALT Keystrokes
// \context ctrlaltkeys Main Window CTRL-ALT Keystrokes

//////////////////////////////////////////////////////////////////////////
// Pages for DOCX

//- \page XrayNotesKeys Main Window Keystrokes
//-	Keystrokes and hotkeys description for the XrayNotes Application.
//-	This page contains a list of keystrokes grouped by modifier.
//-
//- \context altkeys XrayNotes Main Window ALT Keystrokes
//- The main window alt keys are listed in the follwoing table. When no
//- modal dialog is active, the following alt keys are in operation:<p>
//- <tablehere altkeys>

//- \context ctraltlkeys Main Window CTRL-ALT Keystrokes
//- <tablehere ctrlaltkeys>

//- \context ctrlkeys Main Window CTRL Keystrokes
//- The main dialog CTRL keys are listed in the following table. When no
//- dialog is active, the following CTRL keys are in operation:<p>
//- <tablehere ctrlkeys>

//- <p><center>Navigation <linkhere index [Show Main Page]>
//- </center>

/////////////////////////////////////////////////////////////////////////////
// CShadesDlg dialog

CShadesDlg::CShadesDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CShadesDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CShadesDlg)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    //m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	helper = new CShadesHelp(this);
	functs = new CShadesFunc(this);
	sels   = new CShadesSel(this);

    m_LogoFont.CreateFont(64, 0, 0, 0, 0, 1, FALSE, FALSE,
      ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
      0, "Arial Black");

	LOGFONT		m_lf;
	::GetObject((HFONT)GetStockObject(DEFAULT_GUI_FONT), sizeof(m_lf), &m_lf);
	m_Font.CreateFontIndirect(&m_lf);

	showrender = true;

    sLogoString = " XrayNotes Demo ";

    //xx = -2;
    //P2N("looking at xx %d %d %u\r\n", xx.value, (int)xx, (unsigned int)xx);

    current = -1;
    //showstate = STATE_HEIGHT;
    autoadvance = false, autodir = false;

    hscroll = vscroll = 0;

    magic = SHADE_MAGIC;

    rbdown = lbdown = 0;
    handlehit = 0;
    windowmove = 0; drag = 0;
    moire = 0;

    resetpositions = 0;
    movetresh = 6;
    handlesel = 0;
	busy = 0;
	plugin_running = 0;
	saptr = NULL;
	//drawstate = DRAWSTATE_NONE;

	minmax = false;

	keylock = false;
}

CShadesDlg::~CShadesDlg()

{
	//P2N("CShadesDlg::~CShadesDlg()\r\n");
	delete helper;
	delete functs;
	delete sels;

	//delete dbmain;
}

void CShadesDlg::DoDataExchange(CDataExchange* pDX)

{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CShadesDlg)
        // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CShadesDlg, CDialog)
    //{{AFX_MSG_MAP(CShadesDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_DESTROY()
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
    ON_WM_SYSKEYDOWN()
    ON_WM_TIMER()
    ON_WM_CHAR()
    ON_WM_KEYDOWN()
    ON_WM_KEYUP()
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSEWHEEL()
    ON_WM_RBUTTONDOWN()
    ON_WM_LBUTTONDOWN()
    ON_WM_QUERYENDSESSION()
    ON_WM_SETCURSOR()
    ON_WM_RBUTTONUP()
    ON_WM_LBUTTONUP()
    ON_WM_RBUTTONDBLCLK()
    ON_WM_SETTINGCHANGE()
    ON_WM_LBUTTONDBLCLK()
	ON_WM_CLOSE()
	ON_COMMAND(ID_OPEN_OPENDICOMFILE, OnOpenOpendicomfile)
	ON_COMMAND(ID_OPEN_OPENDICOMSERIES, OnOpenOpendicomseries)
	ON_COMMAND(ID_OPEN_DEBUGDUMPHANDLES, OnOpenDebugdumphandles)
	ON_COMMAND(ID_OPEN_OPENPYTHONDIALOG, OnOpenOpenpythondialog)
	ON_COMMAND(ID_OPEN_OPENIMAGEFILE, OnOpenOpenimagefile)
	ON_WM_SYSKEYUP()
	ON_COMMAND(ID_OPEN_NEWIMAGE, OnOpenNewimage)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_RCLICKNODOC_EXITWITHOUTPROMPTFORCESAVE, OnRclicknodocExitwithoutpromptforcesave)
	ON_COMMAND(ID_OPEN_OPENLASTDICOMFILE, OnOpenOpenlastdicomfile)
	ON_COMMAND(ID_OPEN_OPENLASTDICOMSERIES, OnOpenOpenlastdicomseries)
	ON_COMMAND(ID_CLOSE_CLOSECURRENTDEFAULT, OnCloseClosecurrentdefault)
	ON_COMMAND(ID_RCLICKTEXT_PROPERTIES, OnRclicktextProperties)
	ON_COMMAND(ID_RCLICKTEXT_CHANGEBACKGROUNDCOLOR, OnRclicktextChangebackgroundcolor)
	ON_COMMAND(ID_RCLICKTEXT_CHANGEFOREGROUNDCOLOR, OnRclicktextChangeforegroundcolor)
	ON_COMMAND(ID_RCLICKTEXT_SENDTOBACK, OnRclicktextSendtoback)
	ON_COMMAND(ID_RCLICKTEXT_SENDTOFRONT, OnRclicktextSendtofront)
	ON_COMMAND(ID_RCLICKTEXT_SENDBACKWARDONEZORDER, OnRclicktextSendbackwardonezorder)
	ON_COMMAND(ID_RCLICKTEXT_SENDFORWARDONE, OnRclicktextSendforwardone)
	ON_COMMAND(ID_RCLICKTEXT_ADDNEWTEXTDEFAULT, OnRclicktextAddnewtextdefault)
	ON_COMMAND(ID_RCLICKTEXT_ADDNEWGRAYTEXT, OnRclicktextAddnewgraytext)
	ON_COMMAND(ID_RCLICKTEXT_DUMPTEXTOBJECTS, OnRclicktextDumptextobjects)
	ON_COMMAND(ID_RCLICKTEXT_ALIGN_ALIGNTOTHELEFT, OnRclicktextAlignAligntotheleft)
	ON_COMMAND(ID_RCLICKTEXT_ALIGN_ALIGNTOTHERIGHT, OnRclicktextAlignAligntotheright)
	ON_COMMAND(ID_RCLICKTEXT_ALIGN_ALIGHTOTHETOP, OnRclicktextAlignAlightothetop)
	ON_COMMAND(ID_RCLICKTEXT_ALIGN_ALIGNTOTHEBOTTOM, OnRclicktextAlignAligntothebottom)
	ON_COMMAND(ID_RCLICKTEXT_ALIGN_HORIZONTALCENTER, OnRclicktextAlignHorizontalcenter)
	ON_COMMAND(ID_RCLICKTEXT_ALIGN_DISTRIBUTEHORIZONTALLY, OnRclicktextAlignDistributehorizontally)
	ON_COMMAND(ID_RCLICKTEXT_ALIGN_DISTRIBUTEVERTICALLY, OnRclicktextAlignDistributevertically)
	ON_COMMAND(ID_RCLICKTEXT_ALIGN_ENSUREVISIBLE, OnRclicktextAlignEnsurevisible)
	ON_COMMAND(ID_RCLICKNOD_INSERTTEXTOBJECT, OnRclicknodInserttextobject)
	ON_COMMAND(ID_RCLICKLINE_LINEPROPERTIES, OnRclicklineLineproperties)
	ON_COMMAND(ID_EXITALT_ANOTHER, OnExitaltAnother)
	ON_COMMAND(ID_EXITALT_DUMPDOCUMENTS, OnExitaltDumpdocuments)
	ON_COMMAND(ID_RCLICKNODOC_CLOSECURRENTDOCUMENT, OnRclicknodocClosecurrentdocument)
	ON_COMMAND(ID_RCLICKNOD_ENSUREALLOBJECTSAREVISIBLE, OnRclicknodEnsureallobjectsarevisible)
	ON_COMMAND(ID_RCLICKNOD_INSERTSTRAIGHTLINE, OnRclicknodInsertstraightline)
	ON_COMMAND(ID_RCLICKNOD_INSERTRECTANGLEOBJECT, OnRclicknodInsertrectangleobject)
	ON_COMMAND(ID_RCLICKTEXT_DELETEOBJECT, OnRclicktextDeleteobject)
	ON_COMMAND(ID_RCLICKNOD_DRAWFREEHANDLINES, OnRclicknodDrawfreehandlines)
	ON_COMMAND(ID_RCLICKNOD_DRAWINSERTCIRCLE, OnRclicknodDrawinsertcircle)
	ON_COMMAND(ID_RCLICKNOD_INSERTIMAGEOBJECT, OnRclicknodInsertimageobject)
	ON_COMMAND(ID_RCLICKTEXT_CHANGEOBJECTTRANSPARENCY, OnRclicktextChangeobjecttransparency)
	// ON_WM_ACTIVATEAPP()  //TODO
	ON_COMMAND(ID_RCLICKTEXT_ROTATEOBJECT, OnRclicktextRotateobject)
	ON_COMMAND(ID_RCLICKNODOC_SHOWONSCREENKEYBOARD, OnRclicknodocShowonscreenkeyboard)
	ON_COMMAND(ID_RCLICKTEXT_MOVEOBJECT, OnRclicktextMoveobject)
	ON_COMMAND(ID_RCLICKTEXT_RESIZE, OnRclicktextResize)
	ON_COMMAND(ID_RCLICKNODOC_TOGGLEMINMAX, OnRclicknodocToggleminmax)
	ON_WM_MOVE()
	ON_COMMAND(ID_RCLICKNODOC_RESETDIALOGPOSITIONS, OnRclicknodocResetdialogpositions)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// Macros to configure XNT dialogs

#define CONFIGDLG(win, wname, wantx, wanty, wantw, wanth)	\
	/*ASSERT(win.magic == PERSIST_MAGIC);*/	\
	/*win.windowname = wname;*/				\
	win.want_h = wanth;						\
    win.want_w = wantw;						\
	win.want_x = wantx;						\
	win.want_y = wanty;						\
    win.parent = this;						\
    support.YieldToWinEx();

//	win.xCreate();							\

//#define CONFSCROLL(win, wname, wantx, wanty, wantw, wanth)	\
//	ASSERT(win.magic == PERSIST_MAGIC);	\
//	win.windowname = wname				\
//	win.want_w = wantw;					\
//	win.want_y = wanty;					\
//  win.parent = this;					\
//	win.Show();

/////////////////////////////////////////////////////////////////////////////
// CShadesDlg message handlers

BOOL CShadesDlg::OnInitDialog()

{
    CDialog::OnInitDialog();

	m_hIcon =	AfxGetApp()->LoadIcon(IDI_MAINICON);
	//m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON6);	ASSERT(m_hIcon);

	// Load Winsock
    WORD wVersionRequested;   WSADATA wsaData;
	int	err;

	wVersionRequested = MAKEWORD( 2, 2 );
    err = WSAStartup( wVersionRequested, &wsaData );

    if ( err != 0 )
        {
        /* Tell the user that we could not find a usable */
        /* WinSock DLL.                                  */
        xMsgBox("Cannot initialize Windows Socket, networking functions will not be available.");
        //return;
        }

    //P2N("Winsock Ver %d.%d %s \r\n", HIBYTE(wsaData.wVersion), LOBYTE(wsaData.wVersion), wsaData.szDescription);
    //Exception();

    SetWindowText("XrayNotes NG");

	nwfo.parent = this;  nwfo.bitmap = IDB_BITMAP17; nwfo.backlink = functs;
    nefo.parent = this;  nefo.bitmap = IDB_BITMAP19; nefo.backlink = functs;
    swfo.parent = this;  swfo.bitmap = IDB_BITMAP16; swfo.backlink = functs;
    sefo.parent = this;  sefo.bitmap = IDB_BITMAP23; sefo.backlink = functs;

    // Load cursors:
    arrowcur = LoadCursor(NULL, IDC_ARROW);	ASSERT(arrowcur);
	waitcur = LoadCursor(NULL, IDC_WAIT);	ASSERT(waitcur);

	///////////////////////////////////////////////////
	// Load cursors

	targcircle	= myLoadCursor(IDC_CURSOR1);
    pencil		= myLoadCursor(IDC_CURSOR2);
    selectcur	= myLoadCursor(IDC_CURSOR3);
    markcur		= myLoadCursor(IDC_CURSOR4);
    rectcur		= myLoadCursor(IDC_CURSOR9);
	circcur		= myLoadCursor(IDC_CURSOR10);
	textcur		= myLoadCursor(IDC_CURSOR11);
	linecur		= myLoadCursor(IDC_CURSOR12);
	imgcur		= myLoadCursor(IDC_CURSOR13);

	// -------------------------------------------------------------------

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

	if( ((CShadesApp*)AfxGetApp())->albino)
		m_bitmap.LoadBitmap(IDB_BITMAP36);
	else
		m_bitmap.LoadBitmap(IDB_BITMAP2);

    BITMAP BitMap;
    m_bitmap.GetBitmap(&BitMap);
    m_nWidth = BitMap.bmWidth;
    m_nHeight = BitMap.bmHeight;

    // Set the icon for this dialog.
    SetIcon(m_hIcon, TRUE);         // Set big icon
    SetIcon(m_hIcon, FALSE);        // Set small icon

    //ShowWindow(SW_SHOWMAXIMIZED);

    //P2N("Monitor count: %d\r\n", GetSystemMetrics(SM_CMONITORS));

    // Cheat on the Desktop one pixel for poppable status bar
    HDC     hdc     = ::GetDC(HWND_DESKTOP);
    int     xsize   =  ::GetDeviceCaps(hdc, HORZRES);
    int     ysize   =  ::GetDeviceCaps(hdc, VERTRES);

    SetWindowPos( NULL, 1, 1, xsize - 1, ysize - 1, SWP_NOZORDER);

	InitFunctions();

    InitDlgPos();

	helper->InitChevrons();
	helper->InitChevPos();

	// Dialog Windows initial configuration


	// These are initially center:
	magwin.SetCenterFlag(true);

	// Initial state for the dialogs
	gesdlg.Show();
	magdlg.Show();

	xscroll.Show();
	yscroll.Show();

	// Create status window
	statusdlg = new CStatus(this);
	statusdlg->parent = this;
	statusdlg->CenterStatus(this);

    // The timer version of the fill
    if(!SetTimer(1, 10, NULL))
        {
        MBOX("Cannot create timer, timer related functions will not be available.");
        }

    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CShadesDlg::OnSysCommand(UINT nID, LPARAM lParam)

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


//////////////////////////////////////////////////////////////////////////

void CShadesDlg::OnDestroy()

{
    //P2N("CShadesDlg::OnDestroy()\r\n");

	int ndoc = docarr.GetSize();
	CString tmp;

    WinHelp(0L, HELP_QUIT);

	//////////////////////////////////////////////////////////////////////
	// Save Last session data for OpenLastSession
	int loop4;
    for(loop4 = 0; loop4 < ndoc; loop4++)
        {
		tmp.Format("LastFile%d", loop4);
		AfxGetApp()->WriteProfileString(strConfig, tmp, ((CXrayDoc*)docarr[loop4])->filename);
        //P2N("Last session file '%s'\r\n", ((CXrayDoc*)docarr[loop4])->filename);
        }

	// Write empty trailing name for boundary
	tmp.Format("LastFile%d", loop4);
	AfxGetApp()->WriteProfileString(strConfig, tmp, "");

	//////////////////////////////////////////////////////////////////////
	// Push file names into recent list

	CStringArray sarr;
	// Read in old file names
	for(int loop5 = 0; loop5 < MAXRECENT; loop5++)
        {
		tmp.Format("Recent%d", loop5);
		CString fname = AfxGetApp()->GetProfileString(strConfig, tmp, "");

		if(fname == "")						// End of list
			break;

		int haveit = false;
		// See if we have it already, merge into list
		 for(loop4 = 0; loop4 < ndoc; loop4++)
			{
			if(((CXrayDoc*)docarr[loop4])->filename == fname)
				haveit = true;
			}
		if(!haveit)
			sarr.Add(fname);
		}

	int cnt = 0, cnt2 = 0;

	 // Write it out
	for(int loop6 = 0; loop6 < MAXRECENT; loop6++)
		{
		if(loop6 >= ndoc + sarr.GetSize() )
			break;

		if(((CXrayDoc*)docarr[loop6])->dcarr.GetSize() == 0)
			{
			tmp.Format("RecentImage%d", cnt++);
			}
		else if( ((CXrayDoc*)docarr[loop6])->dcarr.GetSize() == 1)
			{
			tmp.Format("RecentDICOM%d", cnt2++);
			}
		else
			{
			tmp.Format("RecentDICOMSer%d", cnt2++);
			}

		if(loop6 < ndoc)
			{
			AfxGetApp()->WriteProfileString(strConfig, tmp, ((CXrayDoc*)docarr[loop6])->filename);
			}
		else if (loop6 < ndoc + sarr.GetSize() )
			{
			AfxGetApp()->WriteProfileString(strConfig, tmp, sarr[loop6 - ndoc]);
			}
		else
			{
			break;
			}
		}

#if 0
    // Iterate save cycle
    int ndoc = docarr.GetSize();
    for(int loop = 0; loop < ndoc; loop++)
        {
        if(((CXrayDoc*)docarr[loop])->changed)
            {
            CString str; str.Format("Save changes to: %s ", ((CXrayDoc*)docarr[loop])->filename);
            MBOX(str, "");
            }
        }
#endif

    // Destroy all documents
    int ndoc2 = docarr.GetSize();
    for(int loop2 = 0; loop2 < ndoc2; loop2++)
        {
        delete ((CXrayDoc*)docarr[loop2] );
        }

	delete statusdlg;

	// Destroy plugin arrays
	for(int loop3 = 0; loop3 < plugarr.GetSize(); loop3++)
		{
		Plugin *pp = (Plugin *)plugarr[loop3];
		ASSERT(pp->magic == PLUG_MAGIC);

		FreeLibrary(pp->hm);

		P2N("Deleted %d %s\r\n", pp->comid, pp->Name);
		delete pp;
		}

	// The last build array for dicom multi
	if(saptr)
		delete [] saptr;

    CDialog::OnDestroy();
}

//////////////////////////////////////////////////////////////////////////
//
// Function name:    CShadesDlg::OnPaint
// Description:      <empty description>
// Return type:      void
//
// If you add a minimize button to your dialog, you will need the code below
// to draw the icon.  For MFC applications using the document/view model,
// this is automatically done for you by the framework.
//
//////////////////////////////////////////////////////////////////////////
//
//  ------------------------------  -> image
//  |x                           |
//  |    x2  -------------------------
//  |        |                   |   |
//  |        |                   |   |
//  ---------|--------------------   |
//           |                       |
//           -------------------------  -> display
//
//////////////////////////////////////////////////////////
// Paint the current document and transfer to screen

void CShadesDlg::OnPaint()

{
	CPaintDC	dc2(this);
	CRect rect; GetClientRect(&rect);

	// Assign pointers for default behaviour
	CMemDC		*dc3 = NULL;
	CDC			*pDC = &dc2;

	{	CSaveDC sdc(pDC);

	//RECT *rc = &dc2.m_ps.rcPaint;
	//P2N("Painting main dlg %d top=%d left=%d bottom=%d right=%d\r\n",
	//		rc->top, rc->left, rc->bottom, rc->right);

    if (IsIconic())
        {
        SendMessage(WM_ICONERASEBKGND, (WPARAM) pDC->GetSafeHdc(), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);

        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        pDC->DrawIcon(x, y, m_hIcon);

		if(!dc3)
			EraseBkgnd(pDC);

        return;
        }

	CXrayDoc *doc = GetCurrDoc();
    if(!doc)
        {
        CDialog::OnPaint();

		if(dc3)
			delete dc3;
		else
			EraseBkgnd(pDC);

		PaintLogo(pDC);

        return;
        }

	ASSERT(doc->magic == DOC_MAGIC);

#ifdef USEMEMDC
	// Device context for painting via memory device
	// If it is a DICOM file, no memory buffering
	if(!doc->IsDicomFile())
		{
		dc3 = new CMemDC(&dc2, rect); EraseBkgnd(dc3);
		pDC = dc3;
		}
	else
		{
		if(!dicomwin.in_play)
			EraseBkgnd(pDC);
		}
#endif

	//P2N("Painting current=%d %p memdc=%p\r\n", current, doc, dc3);

	// On demand render
	ReRender(doc, showrender);

	DrawImage(pDC);

	// -------------------------------------------------------------------
	// Draw objects, clip them to image:

	int lsize = doc->m_layers.GetSize();

	//P2N("Drawing %d Layers\r\n", lsize);

	CRgn rgn; rgn.CreateRectRgn(xpos, ypos, xpos + renx, ypos + reny);
	pDC->SelectClipRgn(&rgn);

	//CRect oldbound; pDC->GetClipBox(&oldbound); P2N_RECT(oldbound);

	for(int looplay = 0; looplay < lsize; looplay++)
		{
		//P2N("Draw Layer %d\r\n", looplay);

		((CLayer*)doc->m_layers[looplay])->Draw(pDC, 1);
		}

	// Allow clipping region to draw all:
	pDC->SelectClipRgn(NULL);

	// Draw objects end
	// -------------------------------------------------------------------

    // For update testing
    //pDC->DrawEdge(updaterect, BDR_RAISEDOUTER, BF_RECT);

    // For selection testing
    if(doc->render->SelectionIsValid() )
        {
        //RECT rcs;
        //doc->render->SelectionGetBox(rcs);
        //pDC->Rectangle(&rcs); //, RGB(255,255,255), RGB(128,128,128));
        //pDC->DrawEdge(&rcs, BDR_RAISEDOUTER, BF_RECT);
        }

    if((GetKeyState( VK_SHIFT)  & 0x80) && (GetKeyState( VK_CONTROL)  & 0x80))
		DrawGrid(pDC);

	PaintLogo(pDC);

	} //CSaveDC sdc(pDC);

	// Will flush the mem device to screen
	if(dc3) delete dc3;
}

// End of paint
//////////////////////////////////////////////////////////////////////////


BOOL CShadesDlg::OnEraseBkgnd(CDC* pDC)

{
	//P2N("erase\r\n");

	// Enable if bg erase needed
#ifndef USEMEMDC
	EraseBkgnd(pDC);
#endif

    return false;
}

//////////////////////////////////////////////////////////////////////////

int	 CShadesDlg::EraseBkgnd(CDC* pDC)

{
	RECT rect; GetClientRect(&rect);

    // Pre render before background is erased

    {
	CSaveDC sdc(pDC);

	// Clear it with a brush first

	//CBrush NewBrush(0x0ffffff);
	//CBrush* pOldBrush = (CBrush*)pDC->SelectObject(&NewBrush);
	//pDC->SetBrushOrg(0,0) ;
    //pDC->PatBlt(rect.left, rect.top, rect.right-rect.left,
	//		rect.bottom-rect.top ,PATCOPY);

    CDC  dcMem; dcMem.CreateCompatibleDC(pDC);
	int save = dcMem.SaveDC();

    CBitmap *pOldBitmap = dcMem.SelectObject(&m_bitmap);

    int xx = 0, yy = 0,
			ww = rect.right-rect.left, hh = rect.bottom-rect.top;
    int loopx = 0, loopy = 0;

    for (loopy = 0; loopy < hh; loopy += m_nHeight)
        {
        for (loopx = 0; loopx < ww; loopx += m_nWidth)
            {
            pDC->BitBlt(loopx, loopy, m_nWidth, m_nHeight,
					&dcMem, 0,0, SRCCOPY );
            }
        }
    dcMem.SelectObject(pOldBitmap);

	dcMem.RestoreDC(save);
	//pDC->SelectObject(pOldBrush);
	}

	return 0;
}

void CShadesDlg::OnCancel()

{
    P2N("Cancelled by ESCAPE on dialog\r\n");
    //CDialog::OnCancel();
}

//////////////////////////////////////////////////////////////////////////
//
//-	\table altkeys border=1 align=center width=700 bgcolor=#eeeeee
//

void CShadesDlg::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)

{
	int nokey = 0;
	int *xnullptr = (int*)0;

	//static UINT oldkey = 0;
	//if(nChar != oldkey)
		{
		//oldkey = nChar;
		//P2N("CShadesDlg::OnSysKeyDown %c (%d) %d %d\r\n",
		//		isalnum(nChar) ? nChar : '-', nChar, nFlags, nFlags >> 25);
		}

    //P2N("Sys Char: %c (%d) ctrl=%d shift=%d alt=%d\r\n", nChar, nChar,
    //  GetAsyncKeyState(VK_CONTROL),
	//	GetAsyncKeyState(VK_SHIFT), GetAsyncKeyState(VK_MENU));

	if(keylock)
		{
		ShowStatus("Keyboard is locked, use CTRL-ALT-Space to unlock.");
		return;
		}

    switch(nChar)
        {
		RECT chrc;
		//- ALT-1  Select NW Chevron (Upper Left)   Active when no modal dialog is up.\
		//-
		case '1':
			//P2N("SYSKEY ALT - 1 %c %x\r\n",  nChar, nFlags);
			nw.GetWindowRect(&chrc);
			nwfo.want_x = chrc.right; nwfo.want_y = chrc.top;
			nwfo.Show();
			break;

		//- ALT-2  Select NE Chevron (Upper Right)   Active when no modal dialog is up.
		case '2':
			//P2N("SYSKEY ALT - 2 %c %x\r\n",  nChar, nFlags);
			ne.GetWindowRect(&chrc);
			nefo.want_x = chrc.right; nefo.want_y = chrc.top;
			nefo.Show();
			break;

		//- ALT-3  Select SW Chevron (Lower Left)   Active when no modal dialog is up.
		case '3':
			//P2N("SYSKEY ALT - 3 %c %x\r\n",  nChar, nFlags);
			sw.GetWindowRect(&chrc);
			swfo.want_x = chrc.right; swfo.want_y = chrc.top;
			swfo.Show();
			break;

		//- ALT-4  Select SE Chevron (Lower Right)   Active when no modal dialog is up.
		case '4':
			//P2N("SYSKEY ALT - 4 %c %x\r\n",  nChar, nFlags);
			se.GetWindowRect(&chrc);
			sefo.want_x = chrc.right; sefo.want_y = chrc.top;
			sefo.Show();
			break;

		//- ALT-LEFT  Switch to Previous Document  Will switch even if in AutoPlay mode.\
		//- This hotkey is compatible how browsers switch windows.
		case VK_LEFT:
			helper->PrevDoc();
		break;

		//- ALT-RIGHT  Switch to Next Document  Will switch even if in AutoPlay mode.\
		//- This hotkey is compatible how browsers switch windows.
		case VK_RIGHT:
			helper->NextDoc();
		break;

		//- ALT-F4  Exit XrayNotes Program  Will prompt to save files.
	    case 115:
            P2N("SYSKEY ALT - F4 %c %x\r\n",  nChar, nFlags);
            EndDialog(0, false);
		break;

		//- ALT-A  Start Auto Advanceing Documents.  Acts like presentation mode
        case 'A':
            if(autoadvance)
                {
                ShowStatus("Stopped AutoAdvance");
                autoadvance = false; autodir = false;
                KillTimer(2);
                }
            else
                {
                if(docarr.GetSize() > 1)
                    {
                    ShowStatus("Switching to AutoAdvance");
                    autoadvance = true;
                    SetTimer(2, 4000, NULL);
                    }
                else
                    {
                    ShowStatus("Cannot switch to autoadvance, two or more documents needed");
                    }
                }
            break;

		//- ALT-B  Show Brigthness Window  Will pop up the color adjustment window.\
		//- Window has persistant location and is non-modal.
		case 'B':
			functs->BrightCb(NULL);
		break;

		//- ALT-C  Color Adjustment Window  Will pop up the color adjustment window.\
		//- Window has persistant location and is non-modal.
		case 'C':
			functs->ColorCb(NULL);
		break;

		// ALT-D  Open last DICOM file   &nbsp
		//case 'D':
		//	helper->OpenLastDicomFile();
		//break;

		//- ALT-E  Edit image file  Opens XrayNotes File Dialog
        case 'E':
            functs->OpenFileCb(NULL);
        break;

		//- ALT-F  Open last File  Opens lastly opened/edited Image File
		case 'F':
           helper->OpenLastFile();
        break;

		//- ALT-G  Open Gesture Dialog   Gesture Dialog is semi transparent\
		//- and non-modal.
		case 'G':
			gesdlg.Show();
		break;

		//- ALT-H  Put Horizontal scroll bar dialog in focus  Will\
		//- set focus to the horizontal scroll bar. The idea is to allow\
		//- the user to operate the scroll bars entirely from the keyboard.\
		//- (alt-h -> arrow left/right or pgup/pgdown) See also the keystroke ALT-V.

        case 'H':
            P2N("SYSKEY H %c %x \r\n",  nChar, nFlags);
            ///
            /// Main dialog Shortcut key: ALT-H to put horizontal scroll dialog in focus
            ///
            xscroll.SetFocus();
        break;

        case 'I':
			magdlg.Show();
			break;

		//- ALT-L  Load Last Image File  &nbsp
		case 'L':
			helper->OpenLastFile();
			break;
		break;

       	//- ALT-M  Patient Management  Open Patient Management Dialog
 		case 'M':
			functs->PatCb(NULL);
		break;

		//- ALT-N  Goto Next Document  Works even when in Auto (presentation mode)
        case 'N':
			helper->NextDoc();
		break;

		//- ALT-O  Open Magnifier  Show Magnifier screen
        case 'O':
            functs->MagCb(NULL);
            break;

        //- ALT-P  Show Previous Document  Works also in presentation mode
        case 'P':
            helper->PrevDoc();
            //Invalidate();
            break;

		//- ALT-R  Auto Advance Document in Reverse  Same as auto advance but documents progress backwards
        case 'R':
            if(autoadvance)
                {
                ShowStatus("Stopped AutoAdvance");
                autoadvance = false;
                KillTimer(2);
                }
            else
                {
                if(docarr.GetSize() > 1)
                    {
                    ShowStatus("Switching to AutoAdvance (reverse direction)");
                    autoadvance = true; autodir = true;
                    SetTimer(2, 4000, NULL);
                    }
                else
                    {
                    ShowStatus("Cannot switch to autoadvance, two or more documents needed");
                    }
                }
            break;

		//- ALT-S  Signal Drocessing  Image Digital Signal processing dialog.\
		//- (Sharpen/Emboss Pinch/Punch ...)
		case 'S':
			functs->ProcCb(NULL);
			break;

		//- ALT-T  Insert Text object  Inserts Text object into document.\
		//- Text is placed at current mouse position.
		case 'T':
			{
			OnRclicknodInserttextobject();

			CXrayDoc *doc = GetCurrDoc();

			if(!doc)
				{
				CTestDlg  td;
				td.DoModal();
				}
			}
		break;

		//- ALT-V  Focus/Show vertical scroll bar dialog  Sets the\
		//- vertical scroll bar in focus.\
        //- The idea is to allow the user to operate the scroll bars\
		//- entirely from the keyboard. (ALT-V -> arrow left/right\
		//- or pgup/pgdown). See also the keystroke ALT-H.

		case 'V':
            P2N("SYSKEY V %c %x \r\n",  nChar, nFlags);

            if(GetAsyncKeyState(VK_SHIFT))
                {
                if(yscroll.IsWindowVisible())
                    {
                    yscroll.ShowWindow(SW_HIDE);
                    }
                else
                    {
                    yscroll.Show();
                    yscroll.SetFocus();
                    }
                }
            else
                {
                yscroll.Show();
                yscroll.SetFocus();
                }
        break;

        case 'X':
            //- ALT-X  Exit the XrayNotes application. Advanced usage key.  This method of\
			//- exit forces the application to save all files with the assumption\
			//- that all 'Save File?' dialog questions are answered with "Yes"

            P2N("SYSKEY ALT - X %c %x\r\n",  nChar, nFlags);
            //CDialog::OnCancel();
			ShowStatus("Exiting with Forced Save");
            EndDialog(0, true);
            break;

        case 'Z':       /// ALT-Z Show all supplementary windows

            if(magdlg.IsWindowVisible())
                SuppWindows(false);
            else
	            SuppWindows(true);

            break;
        }


    CDialog::OnSysKeyDown(nChar, nRepCnt, nFlags);
}

//////////////////////////////////////////////////////////////////////////
///- \table

void CShadesDlg::OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)

{
	//P2N("CShadesDlg::OnSysKeyUp %c (%d) %d %d\r\n",
	//					isalnum(nChar) ? nChar : '-',  nChar,
	//						nFlags, nFlags >> 25);

	CDialog::OnSysKeyUp(nChar, nRepCnt, nFlags);
}


void    CShadesDlg::OnOK()

{
	P2N("CShadesDlg::OnOK()\r\n");
    //xMsgBox("", "OK");
}


void CShadesDlg::OnTimer(UINT nIDEvent)

{
	CString fname;

    switch(nIDEvent)
        {
        case 1:
			// Timer 1: Initiailization
            KillTimer(nIDEvent);

            //P2N("Timer fired\r\n");

			// Wait for things to settle
            while(true)
                {
                if(::IsWindow(m_hWnd))
                    break;
                support.YieldToWinEx();
                Sleep(20);
                }

            //P2N("Window stabilized\r\n");
			//splash.DoModal();

			splash.Show();

            // Load last gesture
            fname = dataroot + "default.gesture";
            gesdlg.LoadSave(true, fname);

            // Load initial files if needed
            if(cline.argarr[0] != "")
                {
                if(_access(cline.argarr[0], 0))
                    {
                    P2N("Fname from command line: %s\r\n", cline.argarr[0]);
                    }
                }
        break;

        // Timer 2: Autoadvace timer shot
        case 2:
            if(autoadvance)
                {
                if(autodir)
                    helper->PrevDoc();
                else
                    helper->NextDoc();
                //Invalidate();
                }
            break;

        // Timer 3: New display Setings
        case 3:
            {
            KillTimer(nIDEvent);

            HDC     hdc     = ::GetDC(HWND_DESKTOP);
            int     xsize   =  ::GetDeviceCaps(hdc, HORZRES);
            int     ysize   =  ::GetDeviceCaps(hdc, VERTRES);

            SetWindowPos( NULL, 0, 0, xsize -1, ysize - 1, SWP_NOZORDER);

            //Invalidate();
            //support.YieldToWinEx();

            RECT rc; GetClientRect(&rc);

            int width = rc.right - rc.left;
            int height = rc.bottom - rc.top;

            //nw.Move(10, 10);
            //ne.Move(rc.right - 40, 10);

            //sw.Move(10, rc.bottom - 100);
            //se.Move(rc.right - 40, rc.bottom - 100);

            //mdlg.Move(rc.left + 80, rc.bottom - 122);
            //gdlg.Move(rc.left + 80, rc.bottom - 350);

            //xscroll.MoveWidth( rc.right - (rc.right - 580 + 50), rc.bottom - 95, width - 580);
            //xscroll.Invalidate();

            //yscroll.MoveWidth( rc.right - 70, 90, height - 220);
            //yscroll.Invalidate();

            Invalidate();
            }
        }

    CDialog::OnTimer(nIDEvent);
}

void CShadesDlg::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)

{
	//P2N("CShadesDlg::OnChar '%c' (%d) states: ctrl=%d shift=%d\r\n",
	//								nChar, nChar, IsCtrl(), IsShift());

	// char-q
    if(nChar == 'q')
        {
        helper->NextDoc();
        //Invalidate();
        }

	/// <tr><td>CTRL-TAB<td>Open Document Selection page<td>

	// char-tab
    if(nChar == '\t')
        {
        P2N("Tab char -- states: shift: %d ctrl: %d   alt: %d\r\n",
			misc.IsShiftDown(), misc.IsCtrlDown(), misc.IsAltDown() );

        if(GetKeyState(VK_CONTROL) & 0x80)
            {
            P2N("CTRL Tab key\r\n");
            functs->ListDocsCb(NULL);
            }
        }

	/// <tr><td>ESC<td>Cancel current mode<td>

	// char-esc
    if(nChar == 27) // ESC char
        {
		//P2N("ESC key down %d\r\n", GetKeyState(VK_CONTROL));
        if(autoadvance)
            {
            ShowStatus("Stopped AutoAdvance (Pressed ESC)");
            autoadvance = false;
            KillTimer(2);
            }
        }

    CDialog::OnChar(nChar, nRepCnt, nFlags);
}


BOOL CShadesDlg::PreTranslateMessage(MSG* pMsg)

{
    // Configure pre translate to send all keys to the dialog
    if(pMsg->message == WM_CHAR)
        {
        //P2N("CShadesDlg pretrans Char %c\r\n", pMsg->wParam);
        return(0);
        }
    if(pMsg->message == WM_KEYDOWN)
        {
        //P2N("pretrans keydown %c (%d) %x\r\n", pMsg->wParam, pMsg->wParam, pMsg->lParam);
        return(0);
        }
    if(pMsg->message == WM_KEYUP)
        {
        //P2N("pretrans keyup %c (%d) %x\r\n", pMsg->wParam, pMsg->wParam, pMsg->lParam);
        return(0);
        }
    return CDialog::PreTranslateMessage(pMsg);
}

//-	\table ctrlaltkeys border=1 align=center width=700 bgcolor=#eeeeee

//////////////////////////////////////////////////////////////////////////
//

void CShadesDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)

{
  	//P2N("CShadesDlg::OnKeyDown %c (%d)  shift=%d ctrl=%d alt=%d\r\n",
	//						nChar, nChar, misc.IsShiftDown(),
	//							misc.IsCtrlDown(), misc.IsAltDown());

	CXrayDoc *doc = GetCurrDoc();

	if(keylock)
		{
		ShowStatus("Keyboard is locked, use CTRL-ALT-Space to unlock.");
		if(!(misc.IsCtrlDown() && misc.IsAltDown() && nChar == ' '))
			return;
		}

	// See for CLayer Keystrokes

	if(doc && (doc->drawstate == TOOL_IDLE || doc->drawstate == TOOL_FOCUS))
		{
		int hit = false, lsize = doc->m_layers.GetSize();

		if(doc->m_currlay)
			{
			doc->m_currlay->KeyEvent(KEYDOWN, nFlags, nChar);
			}
		}

	/// Odd ctrl-alt sequences
	if(misc.IsCtrlDown() && misc.IsAltDown())
		{
		int openmenu = false, res;
		RECT rc; GetClientRect(&rc);
		CPoint point(rc.right/3, rc.bottom/3);

		switch(nChar)
			{
			//- CTRL-ALT-N  Open Right Click On Doc Menu   &nbsp
			case 'N':
				P2N("CShadesDlg::OnKeyDown CTRL-ALT-N\r\n");
				res = IDR_MENU6; openmenu = true;
			break;

			//- CTRL-ALT-R  Open Right Click Menu &nbsp
			case 'R':
				//P2N("CShadesDlg::OnKeyDown CTRL-ALT-N\r\n");
				//res = IDR_MENU1; openmenu = true;
			break;

			//- CTRL-ALT-F  Open Close File Menu  &nbsp
			case 'F':
				P2N("CShadesDlg::OnKeyDown CTRL-ALT-N\r\n");
				res = IDR_MENU5; openmenu = true;
			break;

			//- CTRL-ALT-Space  Lock Keyboard  &nbsp
			case ' ':
				P2N("CShadesDlg::OnKeyDown CTRL-ALT-Space\r\n");
			keylock = ! keylock;
			if(keylock)
				ShowStatus("Keyboard Locked");
			else
				ShowStatus("Keyboard Unlocked");
			break;
			}

		if(openmenu)
			{
			CXrayM *xxmenu = new CXrayM;
			xxmenu->LoadMenu(res, 1);
			xxmenu->Show(point.x, point.y);
			}
		}
	else if(misc.IsCtrlDown() )
		{
		//-	\table ctrlkeys border=1 align=center width=700 bgcolor=#eeeeee
		switch(nChar)
			{
			//- CTRL-D  Open Last Image file  Opens last loaded (saved) image file.
			case 'D':
				helper->OpenLastFile();
				break;

			//- CTRL-E  Close Current File   Closes currently active document
			case 'E':
				functs->CloseFileCb(NULL);

			// CTRL-Q  Show XYZ Dialog.
			case 'Q':
				P2N("CTRL-Q Show XYZ Dialog.\r\n");
				xyzdlg.Show();
				break;

			//- CTRL-S  Save File  Save curently active document.
			case 'S':
				//P2N("Ctrl-S Save file\r\n");
				functs->SaveFileCb(NULL);
				break;
			}
		}

	//////////////////////////////////////////////////////////////////////
	// Context dependent key sequences

	if(doc && doc->drawstate == TOOL_MARK)
        {

		}

	//////////////////////////////////////////////////////////////////////
	// Context less key sequences

    if(nChar == VK_ESCAPE)
        {
        //P2N("ESC key down shift: %d ctrl: %d  Alt: %d\r\n",
		//						misc.IsShiftDown(), misc.IsCtrlDown(), misc.IsAltDown());

		Idle();

		if(pydlg.running)
			{
			P2N("Shown Py dlg\r\n");
			pydlg.Show();
			}
        }

	if(nChar == VK_TAB)
        {
        P2N("TAB key down shift: %d ctrl: %d  Alt: %d\r\n",
								misc.IsShiftDown(), misc.IsCtrlDown(), misc.IsAltDown());

        if(GetKeyState(VK_CONTROL) & 0x80)
            {
            P2N("CTRL Tab key\r\n");
            functs->ListDocsCb(NULL);
            }
        }
//endd:

    CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CShadesDlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)

{
    //P2N("CShadesDlg::OnKeyUp %c (%d)\r\n", nChar, nChar);
    CDialog::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CShadesDlg::DrawGrid(CDC *pDC)

{
    CXrayDoc *doc = GetCurrDoc();

	CxImg *img = doc->render;

	if(doc->IsDicomFile())
		{
		img = ((CxImg*)doc->rendarr[doc->currframe]);
		}

    CRect rect;
    rect.left   = xpos + 1;  rect.top    = ypos + 1;
    rect.right  = rect.left + img->GetWidth() - 2;
    rect.bottom = rect.top + img->GetHeight() - 2;

    // Center lines
    CPen penDash;
    penDash.CreatePen(PS_DASH, 1, RGB(0,0,0) );
    CPen* pOldPen = pDC->SelectObject(&penDash);

#if 0
    pDC->MoveTo(0, rect.top);
    pDC->LineTo(0, rect.bottom);
    pDC->MoveTo(rect.left, 0);
    pDC->LineTo(rect.right, 0);
#endif

    // Major unit lines
	DWORD	style[2] = {1,2} ;
    CPen penDot;
	LOGBRUSH lb; CBrush	br(RGB(128,128,128));
	br.GetLogBrush(&lb);

	//penDot.CreatePen(PS_SOLID, 1, RGB(128,128,128));
    penDot.CreatePen(PS_USERSTYLE, 1, &lb, sizeof(style)/sizeof(DWORD), style);
    pDC->SelectObject(&penDot);

    int gridstep = 50;

    for (int x = rect.left; x < rect.right; x += gridstep)
	    {
        pDC->MoveTo(x, rect.top);
        pDC->LineTo(x, rect.bottom);
		}

    for (int y = rect.top; y < rect.bottom; y += gridstep)
		{
        pDC->MoveTo(rect.left, y);
        pDC->LineTo(rect.right, y);
		}

#if 0

    CPen penSolid;
    penSolid.CreatePen(PS_ALTERNATE, 1, RGB(0, 0, 0));
    pDC->SelectObject(&penSolid);

    for ( x = rect.left; x < rect.right; x += gridstep * 2)
		{
		pDC->MoveTo(x, rect.top);
		pDC->LineTo(x, rect.bottom);
		}

    for (y = rect.top; y < rect.bottom; y += gridstep * 2)
		{
        pDC->MoveTo(rect.left, y);
        pDC->LineTo(rect.right, y);
		}
#endif

#if 0
    // Outlines
    CPen penSolid;
    penSolid.CreatePen(PS_SOLID, 1, m_gridColor);
    pDC->SelectObject(&penSolid);
    pDC->MoveTo(rect.left, rect.top);
    pDC->LineTo(rect.right, rect.top);
    pDC->LineTo(rect.right, rect.bottom);
    pDC->LineTo(rect.left, rect.bottom);
    pDC->LineTo(rect.left, rect.top);
#endif

    pDC->SelectObject(pOldPen);
    //pDC->SetBkColor(oldBkColor);

}

void CShadesDlg::OnMouseMove(UINT nFlags, CPoint point)

{
	currmouse = point;

	if(lbdown && minmax)
		{
		P2N("MinMax move\r\n");

		}

	// Nothing to operate on ...
    if(current < 0 )
        return;

    CXrayDoc *doc = ((CXrayDoc*)docarr[current]);
    if(!doc)
        {
        P2N("NULL doc in paint\r\n");
        return;
        }

    if(doc->magic != DOC_MAGIC)
        {
        P2N("Invalid doc magic\r\n");
        return;
        }


    switch(doc->drawstate)
        {
	    case TOOL_MARK:
            if(nFlags & MK_LBUTTON)
                sels->MarkMove(point);
            break;

        case TOOL_SELECT:
            if(nFlags & MK_LBUTTON)
                sels->SelMove(point);
            break;

		// ---------------------------------------------------------------

		case TOOL_SLINE2:
            if(nFlags & MK_LBUTTON)
                if(doc->m_currlay)
					doc->m_currlay->MouseEvent(MOUSE_MOVE, nFlags, point);
            break;

		case TOOL_RECT2:
			if(nFlags & MK_LBUTTON)
			    if(doc->m_currlay)
					doc->m_currlay->MouseEvent(MOUSE_MOVE, nFlags, point);
            break;

		case TOOL_CIRCLE2:
			if(nFlags & MK_LBUTTON)
			    if(doc->m_currlay)
					doc->m_currlay->MouseEvent(MOUSE_MOVE, nFlags, point);
            break;

		case TOOL_TEXT2:
			if(nFlags & MK_LBUTTON)
			    if(doc->m_currlay)
					doc->m_currlay->MouseEvent(MOUSE_MOVE, nFlags, point);
            break;

		case TOOL_IMAGE2:
			if(nFlags & MK_LBUTTON)
			    if(doc->m_currlay)
					doc->m_currlay->MouseEvent(MOUSE_MOVE, nFlags, point);
            break;

		case TOOL_DRAW2:
			if(nFlags & MK_LBUTTON)
			    if(doc->m_currlay)
					doc->m_currlay->MouseEvent(MOUSE_MOVE, nFlags, point);
            break;

		case TOOL_FOCUS:
		case TOOL_IDLE:

			if(nFlags & MK_LBUTTON)
				{
				int lsize = doc->m_layers.GetSize();

				// Send keys to all in focus
				for(int looplay = 0; looplay < lsize; looplay++)
					{
					if(((CLayer*)doc->m_layers[looplay])->focus)
						{
						((CLayer*)doc->m_layers[looplay])->
								MouseEvent(MOUSE_MOVE, nFlags, point);
						}
					}
				}
			    //if(doc->m_currlay)
				//	doc->m_currlay->MouseEvent(MOUSE_MOVE, nFlags, point);
            break;
        }

    if(drag)
        {
        RECT rc;

        //GetWindowRect(&rc);
        //CPoint point2 = point;    ClientToScreen( &point2 );
        //SetWindowPos( NULL, (point.x +  rc.left) - drag_x ,
        //              (point.y + rc.top) - drag_y, 0, 0,
        //              SWP_NOSIZE | SWP_NOZORDER ) ;

        GetWindowRect(&rc);
        SetWindowPos( NULL, (point.x +  rc.left) - drag_x ,
                 (point.y + rc.top) - drag_y, 0, 0,
                       SWP_NOSIZE | SWP_NOZORDER ) ;
        goto endd;
        }
endd:
    CDialog::OnMouseMove(nFlags, point);
}

BOOL CShadesDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)

{
    //P2N("Mouse wheel flags=%d delta=%d px=%d py=%d\r\n", nFlags, zDelta, pt.x, pt.y);

    yscroll.ExtMouseWheel(nFlags, zDelta, pt);

    return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}


void CShadesDlg::OnRButtonDown(UINT nFlags, CPoint point)

{
	CXrayDoc *doc = GetCurrDoc();
	if(!doc)
		{
	    //P2N("CShadesDlg::OnRButtonDown (no doc) x=%d y=%d\r\n", point.x, point.y);

		CXrayM *xxmenu = new CXrayM;
		xxmenu->LoadMenu(IDR_MENU6, 1);

		//xxmenu->Dump();

		// Disable close if no doc is open
		int comm = xxmenu->FindMenuItem(ID_RCLICKNODOC_CLOSECURRENTDOCUMENT);
		xxmenu->SetItemStatus(comm, MF_GRAYED);
		xxmenu->Show(point.x, point.y);

		CDialog::OnRButtonDown(nFlags, point);

		return;
		}

    RECT  rc; GetSelRect(&rc);

    drag_x	= point.x; drag_y	= point.y;
    old_x	= point.x; old_y	= point.y;

	int insideclick = false, hit = 0, looplay;
	handlehit = 0;

	// See if right click on object
	if(doc->drawstate == TOOL_IDLE || doc->drawstate == TOOL_FOCUS)
		{
		// If there is an object in focus, do it
		if(doc->m_currlay)
			{
			// Select this one, send event

			hit = true;
			doc->drawstate = TOOL_FOCUS;
			doc->m_currlay->MouseEvent(MOUSE_RIGHTDOWN, nFlags, point);
			}
		else
			{
			// Otherwise hit test objects
			int lsize = doc->m_layers.GetSize();
			for(looplay = lsize - 1; looplay >= 0; looplay--)
				{
				if(((CLayer*)doc->m_layers[looplay])->HitTest(point))
					{
					hit = true;
					// Select this one, send event
					doc->drawstate = TOOL_FOCUS;
					doc->m_currlay = (CLayer*)doc->m_layers[looplay];
					//doc->m_currlay->focus = true;
					doc->m_currlay->MouseEvent(MOUSE_RIGHTDOWN, nFlags, point);
					break;
					}
				}
			}
		}
	// Was not an obj, eval app surface
	if(!hit)
		{
		CRect id; id.left = xpos, id.top = ypos;
		id.right = id.left + renx, id.bottom = id.top + reny;

		// Is in image/doc?
		if(id.PtInRect(point))
			{
			//P2N("CShadesDlg::OnRButtonDown Inside doc\r\n");
			insideclick = true;
			CXrayM *xxmenu = new CXrayM;
			xxmenu->LoadAndShow(IDR_MENU17, 1, point.x, point.y);
			}
		else
			{
			//P2N("CShadesDlg::OnRButtonDown Outside doc\r\n");
			CXrayM *xxmenu = new CXrayM;
			xxmenu->LoadAndShow(IDR_MENU6, 1, point.x, point.y);
			}
		}

    // Hittest freehand selection
    int cnt = xmarkarr.GetSize() ;
    if(cnt > 0)
        {
        //dc.MoveTo(xmarkarr[0], ymarkarr[0]);

        int old_x = 0, old_y = 0, loop;
        for(loop = 0; loop < cnt; loop++)
            {
            if(ABS(int(old_x - xmarkarr[loop]) ) > 12 || ABS(int(old_y - ymarkarr[loop]) ) > 12)
                {
                //CRect htest(xmarkarr[loop] - 4, ymarkarr[loop] - 4,
                //              xmarkarr[loop] + 4, ymarkarr[loop] + 4);

                // Wrap loop
                int loop_p = loop - 1, loop_n = loop + 1;
                if(loop == 0) {loop_p = cnt - 1;}
                if(loop == cnt-1) {loop_p = loop_n = 0;}

                CRect htest(xmarkarr[loop_p] , ymarkarr[loop_p],
                                xmarkarr[loop_n], ymarkarr[loop_n]);

                htest.NormalizeRect();

                if(htest.PtInRect(point))
                    {
                    handlehit = HH;
                    handlesel = loop;
                    P2N("Point in freehand selection %d\r\n", loop);

                    //hrclickmenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this, NULL);

					MBOX("hrclickmenu");

                    break;
                    }
                }
            }
		}

    if(!handlehit)
        {
        // Store handle array in undo list ....

        xmarkarr.RemoveAll();
        ymarkarr.RemoveAll();

        xmarkarr.Add(point.x);
        ymarkarr.Add(point.y);

        InvalidateRect(&rc, false);
        support.YieldToWinEx();

        rbdown = true;
        }

    if(doc->IsDicomFile())
		{
		if(doc->showstate == STATE_MUL)
			{
			// Scan the stucture from current state
			for(int loop3 = 0; loop3 < doc->num_to_show; loop3++)
				{
				CRect sr;

				sr.left  = saptr[loop3].xpos ;
				sr.right = saptr[loop3].xpos + saptr[loop3].renx;

				sr.top    = saptr[loop3].ypos ;
				sr.bottom = saptr[loop3].ypos + saptr[loop3].reny;

				if(sr.PtInRect(point))
					{
					P2N("CShadesDlg::OnRButtonDown DICOM hit in rect %d\r\n", loop3);
					CXrayM *xxmenu = new CXrayM;
					xxmenu->LoadMenu(IDR_MENU8, 1);
					xxmenu->Show(point.x, point.y);
					break;
					}
				}
			}
		else
			{
			CXrayM *xxmenu = new CXrayM;
			if(insideclick)
				{
				xxmenu->LoadMenu(IDR_MENU8, 1);
				}
			else
				{
				xxmenu->LoadMenu(IDR_MENU6, 1);
				}
			xxmenu->Show(point.x, point.y);
			}
		}
    else if(current < 0 )
        {
        CXrayM *xxmenu = new CXrayM;
		xxmenu->LoadAndShow(IDR_MENU6, 1, point.x, point.y);
		}

    //P2N("CShadesDlg right button down iconic=%d\r\n", IsIconic());

    CDialog::OnRButtonDown(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////

void CShadesDlg::OnLButtonDown(UINT nFlags, CPoint point)

{
    //P2N("CShadesDlg::OnLButtonDown\r\n");

	CXrayDoc *doc = GetCurrDoc();

	// See if hit inside the doc
	CRect id(xpos, ypos, xpos + renx, ypos + reny);

	if(minmax)
		{
		RECT corner, rect;  GetClientRect(&rect);

		corner.left = rect.right - 12;	corner.top  = rect.bottom - 12;
		corner.right = rect.right - 2;	corner.bottom = rect.bottom - 2;

		if(PtInRect(&corner, point))
			{
			P2N("CShadesDlg::OnLButtonDown resize\r\n");
			}
		else
			{
			// Is in image/doc?

			if(!doc || !id.PtInRect(point))
				{
				PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
				}
			}

		return;
		}

	lbdown = true;  drag_x = point.x; drag_y = point.y;

    if(!doc)
        {
        //P2N("CShadesDlg::OnLButtonDown (no doc)\r\n");
		CDialog::OnLButtonDown(nFlags, point);
		return;
		}
	RECT  rc;  GetClientRect(&rc);


	if(id.PtInRect(point))
		{
		//P2N("CShadesDlg::OnLButtonDown Inside doc\r\n");
		}
	else
		{
		//P2N("CShadesDlg::OnLButtonDown Outside doc\r\n");
		//doc->drawstate = TOOL_IDLE;
		}

	switch(doc->drawstate)
		{
		case TOOL_IDLE:
		case TOOL_FOCUS:
			{
			//P2N("CShadesDlg::OnLButtonDown tool idle state at: %d %d\r\n", point.x, point.y);

			// Hit test objects
			int hit = false, lsize = doc->m_layers.GetSize();



			// Hit test back/front
			int dir = misc.IsAltDown();

			if(dir)
				helper->GetFirstLayer();
			else
				helper->GetLastLayer();

			while(true)
				{
				CLayer *ylay;

				if(dir) ylay = helper->GetNextLayer();
				else     ylay = helper->GetPrevLayer();

				if(!ylay)
					break;

				if(ylay->HitTest(point))
					{
					hit = true;

					// Select this one
					doc->m_currlay = ylay;

					// Do focus
					if(misc.IsCtrlDown())
						doc->m_currlay->SetFocus(!doc->m_currlay->focus);
					else
						doc->m_currlay->SetFocus();
					break;
					}
				}

			if(hit)
				{
				// Clear all other focus if neither shift nor ctrl is down
				if(!misc.IsCtrlDown() && !misc.IsShiftDown())
					for(int looplay2 = 0; looplay2 < lsize; looplay2++)
						{
						CLayer *lay = (CLayer*)doc->m_layers[looplay2];
						if(lay != doc->m_currlay)
							lay->SetFocus(false);
						}

				// Send to all that are in focus
				for(int loop = 0; loop < lsize; loop++)
					{
					CLayer *ylay = (CLayer*)doc->m_layers[loop];
					if(ylay->focus)
						ylay->MouseEvent(MOUSE_LEFTDOWN, nFlags, point);
					}
				}
			else
				{
				// Did not hit anything, clear focus from all
				doc->drawstate = TOOL_IDLE;
				doc->m_currlay = NULL;

				// Clear all other focus
				for(int looplay = 0; looplay < lsize; looplay++)
					{
					((CLayer*)doc->m_layers[looplay])->SetFocus(false);
					}
				}
			}
		break;

		case TOOL_SLINE:
			{
			helper->ClearAllObjFocus();

			CLaySline *lay = new CLaySline(point);
			doc->AddLayer(lay);
			doc->drawstate = TOOL_SLINE2;
			}
		break;

		case TOOL_SELECT:
			P2N("Selection started at: %d %d\r\n", point.x, point.y);
			sels->SelLdown(point);
			break;

		case TOOL_MARK:
			P2N("Marking started at: %d %d\r\n", point.x, point.y);
			sels->MarkLdown(point);
			break;

		case TOOL_RECT:
			{
			P2N("CShadesDlg::OnLButtonDown Tool rect\r\n");

			helper->ClearAllObjFocus();

			CLayRect *lay = new CLayRect; ASSERT(lay);
			lay->Init(point);
			doc->AddLayer((CLayer*)lay);
			doc->drawstate = TOOL_RECT2;
			}
			break;

		case TOOL_CIRCLE:
			{
			P2N("CShadesDlg::OnLButtonDown Tool circ\r\n");

			helper->ClearAllObjFocus();

			CLayCirc *lay = new CLayCirc; ASSERT(lay);
			lay->Init(point);
			doc->AddLayer((CLayer*)lay);
			doc->drawstate = TOOL_CIRCLE2;
			}
			break;

		case TOOL_DRAW:
			{
			P2N("CShadesDlg::OnLButtonDown Tool draw\r\n");

			helper->ClearAllObjFocus();

			CLayLines *lay = new CLayLines; ASSERT(lay);
			lay->Init(point);
			doc->AddLayer((CLayer*)lay);
			doc->drawstate = TOOL_DRAW2;
			}
			break;

		case TOOL_TEXT:
			{
			P2N("CShadesDlg::OnLButtonDown Tool text\r\n");

			helper->ClearAllObjFocus();

			CLayText *lay = new CLayText; ASSERT(lay);
			lay->Init(point);

			// Create gray round
			if(doc->graytext)
				{
				doc->graytext = false;
				lay->txdata.text_round = 120;
				lay->txdata.text_paddingy = lay->txdata.text_paddingx = 20;

				lay->gdata.bg_color	= RGB(128, 128, 128);
				lay->SetNewText("Circlular\r\n  Text");
				}
			doc->AddLayer((CLayer*)lay);
			doc->drawstate = TOOL_TEXT2;
			}
			break;

		case TOOL_IMAGE:
			{
			P2N("CShadesDlg::OnLButtonDown Tool image\r\n");

			helper->ClearAllObjFocus();

			CLayImg *lay = new CLayImg; ASSERT(lay);
			lay->Init(point);

			doc->AddLayer((CLayer*)lay);
			doc->drawstate = TOOL_IMAGE2;
			}
			break;

		default:
			P2N("CShadesDlg::OnLButtonDown Null tool state\r\n");
			break;
		}

	if(doc->IsDicomFile())
		{
		if(doc->showstate == STATE_MUL)
			{
			// Scan the stucture from current state
			for(int loop3 = 0; loop3 < doc->num_to_show; loop3++)
				{
				CRect sr;

				sr.left  = saptr[loop3].xpos ;
				sr.right = saptr[loop3].xpos + saptr[loop3].renx;

				sr.top    = saptr[loop3].ypos ;
				sr.bottom = saptr[loop3].ypos + saptr[loop3].reny;

				if(sr.PtInRect(point))
					{
					P2N("CShadesDlg::OnLButtonDown DICOM hit in rect %d\r\n", loop3);

					dicomwin.SetCurrFrame(saptr[loop3].frame);

					break;
					}
				}
			}
		else
			{
			//P2N("CShadesDlg::OnLButtonDown DICOM\r\n");
			}
		}

    if(doc->showstate != STATE_FULL)
        {
        //ShowStatus("This function is only available in full view");
        return;
        }

#if 0
    RECT    rc;
    GetClientRect(&rc);

    int sdiff = doc->img->GetWidth() - (rc.right - rc.left);
    int hdiff = doc->img->GetHeight() - (rc.bottom - rc.top);

    sdiff = MAX(sdiff, 0);
    hdiff = MAX(hdiff, 0);

    P2N("Red Eye Remove %d %d scrollx=%d scrolly=%d\r\n", point.x, point.y,
            hscroll * sdiff / 100, vscroll * hdiff / 100);

    RECT rc2;
    rc2.left = point.x + hscroll * sdiff / 100;
    rc2.top = doc->img->GetHeight() - (point.y + vscroll * hdiff / 100);

    rc2.left -= 4; rc2.top += 4;
    rc2.right = rc2.left + 8; rc2.bottom = rc2.top - 8;

    //doc->render.DrawLine(point.x + hscroll * sdiff / 100, point.x + hscroll * sdiff / 100 + 20,
    //              doc->img.GetHeight() - (point.y + vscroll * hdiff / 100),
    //                  doc->img.GetHeight() - (point.y + vscroll * hdiff / 100), 0x00aaaaaa);

    //P2N("Inv\r\n");
    //doc->render->SelectionAddEllipse(rc2);

    //P2N("Inv2\r\n");
    //doc->render->RedEyeRemove();
    //doc->changed = true;

    //P2N("Inv3\r\n");
    //Invalidate();

    RECT rc3;
    rc3.left = point.x;
    rc3.top  =  point.y;

    rc3.left -= 4; rc3.top += 4;
    rc3.right = rc3.left + 8; rc3.bottom = rc3.top - 8;

    InvalidateRect(&rc3);

#endif

//endd:

	if(windowmove)
        {
        drag = true;
        }

    //P2N("Inv4\r\n");
    CDialog::OnLButtonDown(nFlags, point);
}


//////////////////////////////////////////////////////////////////////////

BOOL CShadesDlg::OnQueryEndSession()

{
    P2N("CShadesDlg::OnQueryEndSession\r\n");

    //if (!CDialog::OnQueryEndSession())
    //  return FALSE;

	// Always answer YES
    return true;
}

//////////////////////////////////////////////////////////////////////////

BOOL CShadesDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)

{

	CXrayDoc *doc = GetCurrDoc();

    //P2N("Set Cursor message\r\n");
	if(doc)
		{
		switch(doc->drawstate)
			{
			case TOOL_SLINE:
			case TOOL_SLINE2:
				SetCursor(linecur);
				break;

			case TOOL_SELECT:
				SetCursor(selectcur);
				break;

			case TOOL_MARK:
				SetCursor(markcur);
				break;

			case TOOL_IDLE:
				if(busy)
					{
					//P2N("CShadesDlg::OnSetCursor: set wait\r\n");
					SetCursor(waitcur);
					}
				else
					{
					//P2N("CShadesDlg::OnSetCursor: set arrow\r\n");
					SetCursor(arrowcur);
					}
				break;

			case TOOL_RECT:
			case TOOL_RECT2:
				SetCursor(rectcur);
				break;

			case TOOL_CIRCLE:
			case TOOL_CIRCLE2:
				SetCursor(circcur);
				break;

			case TOOL_TEXT:
			case TOOL_TEXT2:
				SetCursor(textcur);
				break;

			case TOOL_DRAW:
			case TOOL_DRAW2:
				SetCursor(pencil);
				break;

			case TOOL_IMAGE:
			case TOOL_IMAGE2:
				SetCursor(imgcur);
				break;

			case TOOL_FOCUS:
				break;

			default:
				P2N("CShadesDlg::OnSetCursor: Invalid tool state\r\n");
				if(busy)
					SetCursor(waitcur);
				else
					SetCursor(arrowcur);
				break;
			}
		}
	else
		{
		if(busy)
			SetCursor(waitcur);
		else
			SetCursor(arrowcur);
		}

    //SetCursor(targcircle);
    //SetCursor(LoadCursor(NULL, IDC_CROSS));

    return true;
    //return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

//////////////////////////////////////////////////////////////////////////
//
// Function name:    CShadesDlg::OnRButtonUp
// Description:      <empty description>
// Return type:      void
// Argument:         UINT nFlags
// Argument:         CPoint point
//
//////////////////////////////////////////////////////////////////////////

void CShadesDlg::OnRButtonUp(UINT nFlags, CPoint point)

{
    rbdown = false;

    if(current < 0)
        goto endd;

    //MarkToRender();

    //RECT rc;
    //GetSelRect(&rc);
    //InvalidateRect(&rc, false);

endd:
    CDialog::OnRButtonUp(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////

void CShadesDlg::GetSelRect(RECT *rc)

{
    rc->left = 32000; rc->top = 32000;
    rc->right = 0; rc->bottom = 0;

    int cnt = xmarkarr.GetSize();

    if(cnt > 0)
        {
        for(int loop = 0; loop < cnt; loop++)
            {
            rc->left    = MIN(rc->left, (long)xmarkarr[loop]);
            rc->top     = MIN(rc->top, (long)ymarkarr[loop]);
            rc->right   = MAX(rc->right, (long)xmarkarr[loop]);
            rc->bottom  = MAX(rc->bottom, (long)ymarkarr[loop]);
            }
        // Inclusive - Exclusive conversion -- add pad for handles
        rc->left  -=8;  rc->top -=8;
        rc->right +=8;  rc->bottom +=8;
        }
    else
        {
        rc->left    = 0; rc->top    = 0;
        rc->right   = 0; rc->bottom = 0;
        }

    //P2N("GetSelRect() -> left=%d top=%d right=%d bottom=%d\r\n",
    //          rc->left, rc->top, rc->right, rc->bottom);
}

void CShadesDlg::OnLButtonUp(UINT nFlags, CPoint point)

{
	if(minmax)
		{
		PostMessage(WM_NCLBUTTONUP, HTCAPTION, MAKELPARAM(point.x, point.y));
		return;
		}

	CXrayDoc *doc = GetCurrDoc();
	if(!doc) goto endd;

	RECT rc2;

	switch(doc->drawstate)
		{
		case TOOL_MARK:
			sels->CalcSelRect(&rc2);
			InvalidateRect(&rc2, false);
			sels->MarkToRender(false);
			helper->TmpRestorePanels();
			break;

		case TOOL_SELECT:
			Invalidate(false);
			sels->SelToRender(false);
			break;

		// ===========================================================
		// Tool functions:

		case TOOL_SLINE:
		case TOOL_SLINE2:

			if(doc->m_currlay)
				{
				doc->m_currlay->MouseEvent(MOUSE_LEFTUP, nFlags, point);
				doc->m_currlay->Invalidate();
				}
			if(doc->multidraw) // If multi shot, start again
				doc->drawstate = TOOL_SLINE;
			else
				Idle();
			break;

		case TOOL_RECT:
		case TOOL_RECT2:
			if(doc->m_currlay)
				{
				doc->m_currlay->MouseEvent(MOUSE_LEFTUP, nFlags, point);
				doc->m_currlay->Invalidate();
				}
			if(doc->multidraw) // If multi shot, start again
				doc->drawstate = TOOL_RECT;
			else
				Idle();
			break;

		case TOOL_CIRCLE:
		case TOOL_CIRCLE2:
			if(doc->m_currlay)
				{
				doc->m_currlay->MouseEvent(MOUSE_LEFTUP, nFlags, point);
				doc->m_currlay->Invalidate();
				}
			if(doc->multidraw) // If multi shot, start again
				doc->drawstate = TOOL_CIRCLE;
			else
				Idle();
			break;

		case TOOL_TEXT:
		case TOOL_TEXT2:
			if(doc->m_currlay)
				{
				doc->m_currlay->MouseEvent(MOUSE_LEFTUP, nFlags, point);
				doc->m_currlay->Invalidate();
				}

			if(doc->multidraw) // If multi shot, start again
				doc->drawstate = TOOL_TEXT;
			else
				Idle();
			break;

		case TOOL_IMAGE:
		case TOOL_IMAGE2:
			if(doc->m_currlay)
				{
				doc->m_currlay->MouseEvent(MOUSE_LEFTUP, nFlags, point);
				doc->m_currlay->Invalidate();
				}

			if(doc->multidraw) // If multi shot, start again
				doc->drawstate = TOOL_IMAGE;
			else
				Idle();
			break;


		case TOOL_DRAW:
		case TOOL_DRAW2:
			if(doc->m_currlay)
				{
				doc->m_currlay->MouseEvent(MOUSE_LEFTUP, nFlags, point);
				doc->m_currlay->Invalidate();
				}

			if(doc->multidraw) // If multi shot, start again
				doc->drawstate = TOOL_DRAW;
			else
				Idle();
			break;

		case TOOL_FOCUS:
		case TOOL_IDLE:
			if(doc->m_currlay)
				{
				doc->m_currlay->MouseEvent(MOUSE_LEFTUP, nFlags, point);
				doc->m_currlay->Invalidate();
				}
			break;
		}

endd:

    lbdown = false;
    handlehit = false;
    windowmove = drag = false;

    CDialog::OnLButtonUp(nFlags, point);
}

void CShadesDlg::OnRButtonDblClk(UINT nFlags, CPoint point)

{
    P2N("R Butt dbl click\r\n");
    CDialog::OnRButtonDblClk(nFlags, point);
}

void CShadesDlg::EndDialog(int ret, int noprompt)

{
    //P2N("End dialog\r\n");

	// No exiting while modal dialog is up
	if(((CShadesApp*)AfxGetApp())->inmodal > 0)
        {
		P2N("Cannot Exit, a Modal dialog is still open %d\r\n", ((CShadesApp*)AfxGetApp())->inmodal);

        //MsgBox("Cannot exit, a modal dialog is still up.", "Exiting XrayNotes");
        ShowStatus("Cannot Exit, a Modal Dialog is still open");
        return;
        }

	// End all threads ....
 	int ndoc = docarr.GetSize();
	for(int loop = 0; loop < ndoc; loop++)
		{
		CXrayDoc *doc = ((CXrayDoc*)docarr[loop]);

		if( doc->thhandle )
			{
			ShowStatus("Exiting while Image Processing is in progress");
			doc->img->SetEscape(-1);
			doc->img->Copy(*doc->org);
			P2N("Aborted DSP processing\r\n");
			support.YieldToWinEx();
			}
		}

    if(!noprompt)
		{
		if(!xMsgBox("Are you Sure?", "Exiting XrayNotes", MBTYPE_YESNO) )
			{
			// Cancelled ending of app
			return;
			}
		}

	((CShadesApp*)AfxGetApp())->forcesave = noprompt;

	if(!AfxGetApp()->SaveAllModified())
		{
		// Restore forcesave to default
		((CShadesApp*)AfxGetApp())->forcesave = false;

		// Cancelled ending of app
		return;
		}

	// Close all interaction dialogs
	CloseAllDialogs();

	// Close dialog
	CDialog::EndDialog(ret);
}

void CShadesDlg::Exception()

{
	P2N("CShadesDlg::Exception()\r\n");
}

#if 0

// exception
    // Divide by null
    //int aa = 2, bb = 1, cc = 0;
    //
    //try
    //{
    //aa = bb/cc;
    //}
    //catch( ... )
    //{
    //  P2N("Got divide by zero exception \r\n");
    //}

    //catch( ... )
    //  {
    //  ReportException(__FILE__, __LINE__);
    //
    //      P2N("Got EXCEPTION NULL Derefernce\r\n");
    //      return;
    //      }

    //  try
    /// {
    //  // Violate
    //  //char *ptr2 = NULL;    *ptr2 = 0;
    //  }
    //
    //  catch( ... )
    //      {
    //      ReportException(__FILE__, __LINE__);
    //
    //      P2N("Got EXCEPTION NULL Derefernce\r\n");
    //      return;
    //      }


#endif

//////////////////////////////////////////////////////////////////////////
//
// Function name:    CShadesDlg::PrompFileName
// Description:      <empty description>
// Return type:      int
// Argument:         const char *str
//
//////////////////////////////////////////////////////////////////////////

int CShadesDlg::PromptFileName(const char *str, CFileDialogST &dlgFile)

{
    P2N("PromptFileName %s\r\n", str);

    CString fileName;

    int dwFlags = 0;
    int *pType = NULL;

    dlgFile.m_ofn.Flags |= dwFlags | OFN_ALLOWMULTISELECT | OFN_EXPLORER;

    int nDocType = (pType != NULL) ? *pType : 0;
    int nIndex = 0;

    dlgFile.m_ofn.nFilterIndex = 0; //nIndex + 1;
    CString strDefExt = "jpg";
    dlgFile.m_ofn.lpstrDefExt = strDefExt;

    CString strFilter;

    strFilter += "Common image formats (jpg, gif, png)";
    strFilter += (TCHAR)NULL;
    strFilter += "*.jpg;*.gif;*.png";
    strFilter += (TCHAR)NULL;

    strFilter += "JPEG Images (*.jpg)";
    strFilter += (TCHAR)NULL;
    strFilter += "*.jpg";
    strFilter += (TCHAR)NULL;

    strFilter += "GIF Images (*.gif)";
    strFilter += (TCHAR)NULL;
    strFilter += "*.gif";
    strFilter += (TCHAR)NULL;

    strFilter += "PNG Images (*.png)";
    strFilter += (TCHAR)NULL;
    strFilter += "*.png";
    strFilter += (TCHAR)NULL;

    strFilter += "*.*";
    strFilter += (TCHAR)NULL;
    strFilter += "*.*";
    strFilter += (TCHAR)NULL;
    strFilter += (TCHAR)NULL;

    dlgFile.m_ofn.lpstrFilter = strFilter;
    dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(1000 *_MAX_PATH);

    BOOL bRet = (dlgFile.DoModal() == IDOK) ? TRUE : FALSE;
    fileName.ReleaseBuffer();

    return bRet;
}


void CShadesDlg::SelDoc(int idx)

{
    // Exception -> no document
    if(docarr.GetSize() == 0)
        {
        current = -1;
        return;
        }

	if(idx >= docarr.GetSize() ||   idx < 0)
		{
		P2N("CShadesDlg::SelDoc Document out of bounds (%d)\r\n", idx);
		return;
		}

	// Wrap the focs events into loose message handling

	helper->OnDocSwitch();
	support.YieldToWinEx();

	// The switch
    current = idx;

	// Actualy happens here
	Invalidate();
	support.YieldToWinEx();

	helper->OnDocSetFocus();
	support.YieldToWinEx();
}



void CShadesDlg::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)

{
    CDialog::OnSettingChange(uFlags, lpszSection);

    //MBOXP("Setting changed %d  '%s'\r\n", uFlags, lpszSection);

    //if(SPI_SETWORKAREA == uFlags)
    //  {
    //  P2N("Workarea changed %d  '%s'\r\n", uFlags, lpszSection);
    //  }

    if(_stricmp(lpszSection, "Windows") == 0 || _stricmp(lpszSection, "Desktop") == 0)
        {
        P2N("Windows changed to: %d\r\n", uFlags);

        SetTimer(3, 300, NULL); 
        // Desktop
        }
}

void CShadesDlg::Idle()

{
	CXrayDoc *doc = GetCurrDoc();

	if(doc)
		{
		doc->drawstate = TOOL_IDLE;
		doc->multidraw = false;
		SetCursor(LoadCursor(NULL, IDC_ARROW));

		if(doc->m_currlay)
			{
			doc->m_currlay->state = STATE_IDLE;
			InvalidateRect(&doc->m_currlay->updaterect);
			}
		}

	// Also Close all menus

	nwfo.Hide();
    nefo.Hide();
    swfo.Hide();
    sefo.Hide();
}



void CShadesDlg::OnLButtonDblClk(UINT nFlags, CPoint point)

{
    P2N("CShadesDlg::OnLButtonDblClk\r\n");

	CXrayDoc *doc = GetCurrDoc();
	if(!doc)
		{
		CDialog::OnLButtonDblClk(nFlags, point);
		return;
		}

	if(doc->drawstate == TOOL_IDLE || doc->drawstate == TOOL_FOCUS)
		{
		// Hit test objects first
		int hit = false, lsize = doc->m_layers.GetSize();
		for(int looplay = lsize - 1; looplay >= 0; looplay--)
			{
			if(((CLayer*)doc->m_layers[looplay])->HitTest(point))
				{
				hit = true;
				// Clear all other focus
				//if(misc.IsShift())
				//for(int looplay2 = 0; looplay2 < lsize; looplay2++)
				//	{
				//	((CLayer*)doc->m_layers[looplay2])->SetFocus(false);
				//	}

				// Select this one
				doc->m_currlay = (CLayer*)doc->m_layers[looplay];
				doc->drawstate = TOOL_FOCUS;

				doc->m_currlay->SetFocus();
				doc->m_currlay->MouseEvent(MOUSE_LEFTDBL, nFlags, point);

				break;
				}
			}
		}

	switch(doc->drawstate)
		{
		case TOOL_MARK:
			{
			RECT rc; GetClientRect(&rc);

			if(handlesel < xmarkarr.GetSize() && handlesel > 0)
				{
				DWORD xxx = xmarkarr[handlesel] + ((int)xmarkarr[handlesel + 1] - (int)xmarkarr[handlesel]) / 2;
				DWORD yyy = ymarkarr[handlesel] + ((int)ymarkarr[handlesel + 1] - (int)ymarkarr[handlesel]) / 2;

				xmarkarr.InsertAt(handlesel, xxx);
				ymarkarr.InsertAt(handlesel, yyy);

				//P2N("XmarkArr insert %d %d\r\n", xxx, yyy);
				}
			InvalidateRect(&rc, false);
			}
			break;
		}

    CDialog::OnLButtonDblClk(nFlags, point);
}


void CShadesDlg::OnClose()

{
	P2N("CShadesDlg::OnClose Closing application\r\n");
	CDialog::OnClose();
}

BOOL CShadesDlg::OnCommand(WPARAM wParam, LPARAM lParam)

{
	P2N("Oncommand %d %x accel=%s\r\n", wParam, wParam, (wParam >> 16) == 0 ? "false" : "true");

	if(wParam >= WM_OPEN_RECENT && wParam < WM_OPEN_RECENT + MAXRECENT)
		{
		// Figure out the file name CWnd
		CXrayM *mm = (CXrayM*)FromHandlePermanent((HWND)lParam);
		ASSERT(mm); ASSERT(mm->magic == MENU_MAGIC);

		CString str;
		mm->GetMenuItemText(wParam - WM_OPEN_RECENT, str);

		P2N("Open recent file %d %s\r\n", wParam - WM_OPEN_RECENT, str);

		if(str !="")
			{
			functs->OpenImageFile(str);
			}
		}
	else if(wParam >= WM_OPEN_RECENT_DICOM && wParam < WM_OPEN_RECENT_DICOM + MAXRECENT)
		{
		// Figure out the file name CWnd
		CXrayM *mm = (CXrayM*)FromHandlePermanent((HWND)lParam);
		ASSERT(mm); ASSERT(mm->magic == MENU_MAGIC);

		CString str;
		mm->GetMenuItemText(wParam - WM_OPEN_RECENT_DICOM, str);

		P2N("Open recent dicom file %d %s\r\n", wParam - WM_OPEN_RECENT_DICOM, str);

		if(str !="")
			{
			functs->OpenDicomFile(this, false, str);
			}
		}
	else if(wParam >= WM_OPEN_RECENT_DICOM_S && wParam < WM_OPEN_RECENT_DICOM_S + MAXRECENT)
		{
		// Figure out the file name CWnd
		CXrayM *mm = (CXrayM*)FromHandlePermanent((HWND)lParam);
		ASSERT(mm); ASSERT(mm->magic == MENU_MAGIC);

		CString str;
		mm->GetMenuItemText(wParam - WM_OPEN_RECENT_DICOM_S, str);

		P2N("Open recent dicom series %d %s\r\n", wParam - WM_OPEN_RECENT_DICOM_S, str);

		if(str != "")
			{
			functs->OpenDicomFile(this, true, str);
			}
		}

	else switch(wParam & 0xffff)
		{
		case ID_DICOM_MOVEZORDERTOWARDSTHEFIRST:
			//P2N("OnCommand ID_DICOM_MOVEZORDERTOWARDSTHEFIRST \r\n");
			helper->MoveDicomZ(false);
			break;

		case ID_DICOM_MOVEZORDERTOWARDSTHELAST:
			//P2N("OnCommand ID_DICOM_MOVEZORDERTOWARDSTHELAST \r\n");
			helper->MoveDicomZ(true);
			break;

		case ID_RCLICKNODOC_OPENDBMAIN:
			{
			//dbmain.DoModal();
			}
			break;

		case ID_OPEN_OPENLASTDICOMFILE:
			P2N("OnCommand ID_OPEN_OPENLASTDICOMFILE \r\n");
			helper->OpenLastDicomFile();
			break;

		case WM_CLEAR_RECENT_DICOM:
			//P2N("OnCommand WM_CLEAR_RECENT_DICOM \r\n");
			{
			for(int loop6 = 0; loop6 < MAXRECENT; loop6++)
				{
				CString tmp; tmp.Format("RecentDICOM%d", loop6);
				AfxGetApp()->WriteProfileString(strConfig, tmp, "");
				}
			}
			break;

		case WM_CLEAR_RECENT:
			//P2N("OnCommand WM_CLEAR_RECENT \r\n");
			{
			for(int loop6 = 0; loop6 < MAXRECENT; loop6++)
				{
				CString tmp; tmp.Format("RecentImage%d", loop6);
				AfxGetApp()->WriteProfileString(strConfig, tmp, "");
				}
			}
			break;

		case ID_OPEN_OPENLASTSESSION:
			//P2N("OnCommand ID_OPEN_OPENLASTSESSION \r\n");
			helper->OpenLastSession();
			break;

		case ID_CLOSE_CLOSEALL:
			//P2N("OnCommand ID_CLOSE_CLOSEALL \r\n");
			functs->CloseAll(this);
			break;

		case ID_CLOSE_CLOSECURRENTDEFAULT:
			P2N("OnCommand ID_CLOSE_CLOSECURRENTDEFAULT \r\n");
			functs->CloseFileCb(this);
			break;

		//case ID_OPEN_OPENLASTDICOMSERIES:
		//	P2N("OnCommand ID_OPEN_OPENLASTDICOMSERIES \r\n");
		///	helper->OpenLastDicomSeries();
		//	break;

		case ID_HELP:
			support.Help();
			break;

		case ID_OPEN_OPENLASTFILE:
			helper->OpenLastFile();
		break;

		case ID_PROCESS_DUMMY:
			// Stop plugin:
			for(int loop = 0; loop < plugarr.GetSize(); loop++)
				{
				Plugin *pp = (Plugin *)plugarr[loop];

				if(pp->running)
					{
					pp->Stop();
					pp->running = false;

					// Enable plugin menus:
					//for(DWORD loop2 = 2; loop2 < ppmenu->GetMenuItemCount(); loop2++)
					//	{
					//	ppmenu->EnableMenuItem(loop2, MF_ENABLED | MF_BYPOSITION );
					//	}
					}
				}
			break;
		}

	// Scan the plugin tables
	for(int loop = 0; loop < plugarr.GetSize(); loop++)
		{
		Plugin *pp = (Plugin *)plugarr[loop];

		if(pp->comid == (int)wParam)
			{
			CxImg *img;

			P2N("Command for '%s'\r\n", pp->Description());

			if(current < 0)
				{
				xMsgBox("No document loaded, cannot execute plugin.");
				goto endd;
				}

			CXrayDoc *doc = ((CXrayDoc*)docarr[current]);
			if(!doc)
				{
				xMsgBox("No document present, cannot execute plugin.");
				goto endd;
				}

			ASSERT(doc->magic == DOC_MAGIC);

			// Disable plugin menus:
			//for(DWORD loop = 2; loop < ppmenu->GetMenuItemCount(); loop++)
			//	{
			//	ppmenu->EnableMenuItem(loop, MF_GRAYED | MF_BYPOSITION );
			//	}

			img = doc->render;

			if(!img->IsValid())
				{
				xMsgBox("No image present, cannot execute plugin.");
				goto endd;
				}

			plugin_running = pp->running = true;
			int ret = pp->Process(img->GetPixPtr(), img->GetEffWidth(), img->GetHeight(), img->GetBitsPpix() >> 3);
			plugin_running = pp->running = false;

			// Enable plugin menus:
			//for(DWORD loop2 = 2; loop2 < ppmenu->GetMenuItemCount(); loop2++)
			//	{
			//	ppmenu->EnableMenuItem(loop2, MF_ENABLED | MF_BYPOSITION );
			//	}

			// Interpret return value acording to specs
			P2N("Plugin process returned %d\r\n", ret);

			if(ret & BIT_0)
				{
				//P2N("BIT_0 is set\r\n");
				// This is a NOOP
				}
			if(ret & BIT_1)
				{
				//P2N("BIT_1 is set\r\n");
				doc->changed = true;
				Invalidate();
				}
			if(ret & BIT_2)
				{
				//P2N("BIT_2 is set\r\n");
				xMsgBox(pp->Message());
				//Invalidate();
				}
			if(ret & BIT_3)
				{
				//P2N("BIT_3 is set\r\n");
				xMsgBox(pp->Message());
				}
			break;
			}
		}
endd:
	return CDialog::OnCommand(wParam, lParam);
}

//////////////////////////////////////////////////////////////////////////

void CShadesDlg::BusyCursor(int bus)

{
	//P2N("Updating busy cursor\r\n");
	busy = bus;

	procdlg.UpdateCursor();
	bcondlg.UpdateCursor();
	coldlg.UpdateCursor();
	dicomwin.UpdateCursor();

	UpdateCursor();
}


void CShadesDlg::UpdateCursor()

{
	//P2N("CShadesDlg::UpdateCursor\r\n");
	if(IsWindow(m_hWnd))
		SendMessage(WM_SETCURSOR, HTCLIENT, 0);
}

void CShadesDlg::OnOpenOpendicomfile()

{
	//P2N("CShadesDlg::OnOpenOpendicomfile\r\n");
	functs->OpenDicomFile(this);
}

void CShadesDlg::OnOpenOpendicomseries()
{
	functs->OpenDicomSeries(this);
}

void CShadesDlg::InvalidateSurface()

{
	RECT rect;

	rect.top = ypos;
	rect.left = xpos;

	rect.bottom = rect.top + reny; // - 100;
	rect.right  = rect.left + renx; // - 100;

	InvalidateRect(&rect, false);
}

//////////////////////////////////////////////////////////////////////////

void CShadesDlg::ReRender(CXrayDoc *doc, int show)

{
	RECT rect; GetClientRect(&rect);

	BusyCursor();

	// Changed show state, re-render image from storage
    if(doc->oldshowstate != doc->showstate)
        {
		//P2N("CShadesDlg::ReRender\r\n");

		int resamp = true;

        switch(doc->showstate)
            {
			case STATE_MUL:
                reny = doc->img->GetHeight();
                renx = doc->img->GetWidth();

				// Only resample if different state
				if(doc->oldshowstate == STATE_FULL)
					resamp = false;
				break;

            case STATE_WIDTH:
			    renx = (rect.right - rect.left);
                reny = (renx *  doc->img->GetHeight()) / doc->img->GetWidth();
				if(show)
					ShowStatus("Rendering to WIDTH View ...");
                break;

            case STATE_HEIGHT:
                reny = (rect.bottom - rect.top);
                renx = (reny *  doc->img->GetWidth()) / doc->img->GetHeight();
                if(show)
					ShowStatus("Rendering to HEIGHT View ...");
                break;

            case STATE_FULL:
                reny = doc->img->GetHeight();
                renx = doc->img->GetWidth();
                if(show && (!doc->IsDicomFile()))
					ShowStatus("Rendering to FULL View ...");
                break;

            case STATE_VARI:
            case STATE_FINE:
                reny = (int)(doc->mag * doc->img->GetHeight() );
                renx = (int)(doc->mag * doc->img->GetWidth()  );

                // Is it magnified?
                if(doc->mag > 1)
                    {
                    }
                else
                    {
                    //doc->img->Resample(renx, reny,  1,  doc->render);
                    }
                break;
            }

		if(resamp)
			{
			// Sensitive to DICOM presence
			if(doc->IsDicomFile())
				{
				//P2N("CShadesDlg::ReRender DICOM\r\n");

				doc->currframe = min(doc->currframe, doc->rendarr.GetSize() - 1);

				if( ((CxImg*)doc->rendarr[doc->currframe])->GetWidth() != (DWORD)renx ||
					((DicomFile*)doc->dcarr[doc->currframe])->changed
				  )
					{
					P2N("Dicom Resamp '%s'\r\n", doc->filename);

					ShowStatus("Rendering DICOM Frame ...");

					((DicomFile*)doc->dcarr[doc->currframe])->changed = false;

					((CxImg*)doc->imgarr[doc->currframe])->Resample
									(renx, reny,  0,  (CxImg*)doc->rendarr[doc->currframe]);
					}
				}
			else
				{
				//P2N("CShadesDlg::ReRender IMAGE\r\n");

				doc->img->Resample(renx, reny,  0,  doc->render);
				doc->oldshowstate = doc->showstate;
				}
			}

        HideStatus();
		helper->OnDocChange();
		}

	showrender = true;
	BusyCursor(false);
}

void CShadesDlg::OnOpenDebugdumphandles()

{
	for(int loop2 = 0; loop2 < docarr.GetSize(); loop2++)
		{
		CXrayDoc* adoc = (CXrayDoc*)docarr[loop2];

		P2N("Document %d %p %x %s %s\r\n", loop2, adoc, adoc->magic,
			adoc->filename, helper->state2str(adoc->showstate));
		}
}

void CShadesDlg::OnOpenOpenpythondialog()

{
	pydlg.Show();
}

void CShadesDlg::SuppWindows(int show)

{
	if(!show)
		{
		magdlg.Hide();
        xscroll.Hide();
        yscroll.Hide();
        gesdlg.Hide();
		}
	else
		{
		magdlg.Show();
        xscroll.Show();
        yscroll.Show();
        gesdlg.Show();
		}
}

void CShadesDlg::OnOpenOpenimagefile()

{
	functs->OpenFileCb(NULL);
}

//////////////////////////////////////////////////////////////////////////

CXrayDoc* CShadesDlg::GetCurrDoc()

{
	ASSERT(magic == SHADE_MAGIC);

	CXrayDoc *doc = NULL;

	if(current < 0)
		goto endd;

	if(current >= docarr.GetSize())
        {
        P2N("CShadesDlg::GetCurrDoc: Boundary error on docarr\r\n");
        return doc;
        }

	doc = (CXrayDoc *)docarr[current];

	if(!doc)
		goto endd;

	ASSERT(doc->magic == DOC_MAGIC );

endd:
	return doc;
}

void CShadesDlg::CloseAllDialogs()

{
	procdlg.Hide();
	bcondlg.Hide();
	coldlg.Hide();
	dicomwin.Hide();
	pydlg.Hide();
	magwin.Hide();

	support.YieldToWinEx();
	SetActiveWindow();
}

void CShadesDlg::OnOpenNewimage()

{
	//P2N("New Image\r\n");

	CString name;
	int uniq = CXrayDoc::gl_enum++;

	// Find unique name for this file
	while(true)
		{
		name.Format("untitled%d.png", uniq);

		if(_access(name, 00) < 0)
			break;

		uniq = CXrayDoc::gl_enum++;
		}

	CXrayDoc *doc = new CXrayDoc;   ASSERT(doc);

	// New Image, make sure it is saved
	doc->changed = true;
	doc->filename = name;
	doc->uniq = uniq;
   	current =  docarr.Add(doc);
    //lastfilename = doc->filename;

	// Create STD image file
    int     xsize   =  ::GetDeviceCaps(::GetDC(HWND_DESKTOP), HORZRES);
	xsize = max(MIN_SCREEN_WIDTH, xsize);

	//P2N("Correted xsize %d\r\n", xsize);

	doc->img->Create(xsize, (int)(PAPER_ASPECT_RATIO * xsize), 24);
	doc->img->Clear(255);

	Invalidate();
}

void CShadesDlg::OnFileNew( )

{
	P2N("CShadesDlg::OnFileNew\r\n");
}

void CShadesDlg::OnFileOpen( )

{
	P2N("CShadesDlg::OnFileOpen\r\n");
}

void CShadesDlg::OnFilePrint( )

{
	P2N("CShadesDlg::OnFilePrint\r\n");
}

void CShadesDlg::OnRclicknodocExitwithoutpromptforcesave()
{
	P2N("OnRclicknodocExitwithoutpromptforcesave\r\n");

	EndDialog(0, true);
}

void CShadesDlg::OnOpenOpenlastdicomfile()
{
	P2N("OnOpenOpenlastdicomfile: open last dicom file\r\n");

	helper->OpenLastDicomFile();
}

void CShadesDlg::InitFunctions()

{
	//nwfo.arr_l[9].func = &CShadesFunc::TestCb;

	nwfo.arr_l[0].func = &CShadesFunc::OpenFileCb;
    nwfo.arr_l[1].func = &CShadesFunc::ScanDocCb;
    nwfo.arr_l[2].func = &CShadesFunc::CloseFileCb;
    nwfo.arr_l[3].func = &CShadesFunc::SaveFileCb;
    nwfo.arr_l[4].func = &CShadesFunc::SaveAsCb;
    nwfo.arr_l[5].func = &CShadesFunc::PrintCb;
    nwfo.arr_l[6].func = &CShadesFunc::CopyCb;
    nwfo.arr_l[7].func = &CShadesFunc::CutCb;
    nwfo.arr_l[8].func = &CShadesFunc::PasteCb;
    nwfo.arr_l[9].func = &CShadesFunc::ExitCb;
	nwfo.arr_r[9].func = &CShadesFunc::rExitCb;

	nwfo.arr_r[0].func = &CShadesFunc::rOpenFileCb;
	nwfo.arr_r[2].func = &CShadesFunc::rCloseFileCb;
	nwfo.arr_r[6].func = &CShadesFunc::rCopyCb;
    nwfo.arr_r[8].func = &CShadesFunc::rPasteCb;

	// -------------------------------------------------------------------

	swfo.arr_l[0].func = &CShadesFunc::CropCb;
    swfo.arr_r[0].func = &CShadesFunc::rCropCb;
    swfo.arr_l[1].func = &CShadesFunc::NormCb;
    swfo.arr_l[2].func = &CShadesFunc::BrightCb;
    swfo.arr_l[3].func = &CShadesFunc::ColorCb;
    swfo.arr_l[4].func = &CShadesFunc::MagCb;
    swfo.arr_l[5].func = &CShadesFunc::ProcCb;
    swfo.arr_l[6].func = &CShadesFunc::UndoCb;
    swfo.arr_l[7].func = &CShadesFunc::RedoCb;
    swfo.arr_l[8].func = &CShadesFunc::SelectCb;
    swfo.arr_l[9].func = &CShadesFunc::MarkCb;

	swfo.arr_r[5].func = &CShadesFunc::rProcCb;
    swfo.arr_r[4].func = &CShadesFunc::rMagCb;

  	// -------------------------------------------------------------------

	nefo.arr_l[0].func = &CShadesFunc::ListDocsCb;
    nefo.arr_l[1].func = &CShadesFunc::DocPropCb;
    nefo.arr_l[2].func = &CShadesFunc::ToggleUICb;
    nefo.arr_l[3].func = &CShadesFunc::ConfCb;
    nefo.arr_l[4].func = &CShadesFunc::OptCb;
    nefo.arr_l[5].func = &CShadesFunc::AutoCb;
    nefo.arr_l[6].func = &CShadesFunc::PatCb;
    nefo.arr_l[7].func = &CShadesFunc::cNewCb;
    nefo.arr_l[8].func = &CShadesFunc::NewCb;
    nefo.arr_l[9].func = &CShadesFunc::ExitCb;
	nefo.arr_r[9].func = &CShadesFunc::rExitCb;

	// -------------------------------------------------------------------

	sefo.arr_l[0].func = &CShadesFunc::LineCb;
    sefo.arr_l[1].func = &CShadesFunc::DrawCb;
    sefo.arr_l[2].func = &CShadesFunc::TextCb;
    sefo.arr_l[3].func = &CShadesFunc::RectCb;
    sefo.arr_l[4].func = &CShadesFunc::EllipseCb;
    sefo.arr_l[5].func = &CShadesFunc::ImageCb;
    sefo.arr_l[6].func = &CShadesFunc::SVGCb;
    sefo.arr_l[7].func = &CShadesFunc::MagDocCb;
    sefo.arr_l[8].func = &CShadesFunc::MeasCb;
    sefo.arr_l[9].func = &CShadesFunc::IdleCb;

    sefo.arr_r[2].func = &CShadesFunc::rTextCb;

}


void CShadesDlg::DumpSap()

{
	// Dump stucture of docs:

	//for(loop = 0; loop < doc->num_to_show; loop++)
	//	{
	//	P2N("Coor %d %d %d %d  -- ",
	//		saptr[loop].renx, saptr[loop].reny,
	//			saptr[loop].xpos, saptr[loop].ypos);
	//	if(saptr[loop].doc)
	//		{
	//		P2N("dump doc %p 0x%x\r\n", saptr[loop].doc, saptr[loop].doc->magic);
	//		}
	//	else
	//		P2N("dump doc %p\r\n", saptr[loop].doc);
	//	}
}

	//xxmenu->SetToolTip("Open/Edit", "Open and or Edit Image File");
			//xxmenu->SetToolTip("Open DICOM File", "Open DICOM Image File");
			//xxmenu->SetToolTip("Open DICOM Series", "Open a set of DICOM Image Files. "
			//		"\r\nThe files are loaded from the common directory");


void CShadesDlg::OnOpenOpenlastdicomseries()

{
	P2N("CShadesDlg::OnOpenOpenlastdicomseries()\r\n");
	helper->OpenLastDicomSeries();
}

void CShadesDlg::OnCloseClosecurrentdefault()
{
	functs->CloseFileCb(NULL);
}

//////////////////////////////////////////////////////////////////////
// Paint Logo

void CShadesDlg::PaintLogo(CDC *pDC)

{
	//return ;

    RECT rect; GetClientRect(&rect);
	{
	CSaveDC sdc(pDC);

    // Calc blend rectange
    CFont* oldFont = pDC->SelectObject(&m_LogoFont);
    CSize sz = pDC->GetTextExtent(sLogoString, sLogoString.GetLength());
    sz.cx += 10; sz.cy += 10;
    pDC->SelectObject(oldFont);

	CDC  dcMem;   dcMem.CreateCompatibleDC(pDC);

	int save = dcMem.SaveDC();

	// CreateCompatibleBitmap does not work on color/memory device!
    HBITMAP bm =::CreateCompatibleBitmap(pDC->GetSafeHdc(), sz.cx, sz.cy);
    HBITMAP oldbm = (HBITMAP)::SelectObject(dcMem, bm);

   //P2N("Logo string size ww=%d hh=%d\r\n", sz.cx, sz.cy);

    CRect rc2(0, 0, sz.cx, sz.cx);
    dcMem.FillSolidRect(rc2, RGB(255, 255, 255) );

    // Alpha blend
    BLENDFUNCTION m_bf;

    m_bf.BlendOp = AC_SRC_OVER;
    m_bf.BlendFlags = 0;
    m_bf.SourceConstantAlpha = 40;
    m_bf.AlphaFormat = 0;

    CFont* XoldFont = pDC->SelectObject(&m_LogoFont);
    int OldMode = pDC->SetBkMode(TRANSPARENT);

    //int hhd = 55; int vvd = 5;

    int rr = rect.right / 2 - (sz.cx /2);;
    int ll = rect.top  ;

    CRect rc3(rr, ll, rr +  sz.cx, ll + sz.cy);

    pDC->SetBkMode(TRANSPARENT );
    pDC->BeginPath();
    COLORREF  OldCol = pDC->SetTextColor(RGB(0, 0, 0));
    pDC->DrawText(sLogoString, sLogoString.GetLength(), &rc3, DT_VCENTER | DT_SINGLELINE | DT_CENTER);
    pDC->EndPath();
    pDC->SelectClipPath(RGN_COPY);

    AlphaBlend(pDC->GetSafeHdc(), rc3.left, rc3.top, sz.cx, sz.cy, dcMem, 0,0, sz.cx, sz.cy, m_bf);

    pDC->SelectClipRgn(NULL);
    dcMem.FillSolidRect(rc2, RGB(0, 0, 0) );
    pDC->BeginPath();
    CRect rc4(rr + 2, ll + 2, rr +  sz.cx + 2, ll + sz.cy + 2);
    pDC->DrawText(sLogoString, sLogoString.GetLength(), &rc4, DT_VCENTER | DT_SINGLELINE | DT_CENTER);
    pDC->EndPath();
    pDC->SelectClipPath(RGN_COPY);

    AlphaBlend(pDC->GetSafeHdc(), rc4.left, rc4.top, sz.cx, sz.cy, dcMem, 0,0, sz.cx, sz.cy, m_bf);

    pDC->SelectObject(XoldFont);
    pDC->SetTextColor(OldCol);
    pDC->SetBkMode(OldMode);

    ::SelectObject(dcMem, oldbm);   ::DeleteObject(bm);

    //dcMem.DeleteDC();

    // Allow clipping region to draw all:
    pDC->SelectClipRgn(NULL);


	// Paint lower right handle
	//    /|
	//   / |
	//  /  |
	// /---|

	if(minmax)
		{
		RECT corner;

		corner.left = rect.right - 12;	corner.top  = rect.bottom - 12;
		corner.right = rect.right - 2;	corner.bottom = rect.bottom - 2;

		CPen pen; pen.CreatePen(PS_SOLID, 2, RGB(128, 128, 128));
		CPen* pOldPen = pDC->SelectObject(&pen);

		pDC->MoveTo(corner.left, corner.bottom);
		pDC->LineTo(corner.right, corner.top);

		pDC->MoveTo(corner.left + 4, corner.bottom);
		pDC->LineTo(corner.right, corner.top + 4);

		pDC->MoveTo(corner.left + 8, corner.bottom);
		pDC->LineTo(corner.right, corner.top + 8);

		pDC->SelectObject(&pOldPen);
		}

	dcMem.RestoreDC(save);
	}
}

void CShadesDlg::OnRclicktextProperties()

{
	CXrayDoc *doc = GetCurrDoc();
	if(!doc) return;

	if(!doc->m_currlay)
		return;

	doc->m_currlay->Prop(0,0,0);
}

#include "PicSel.h"

void CShadesDlg::OnRclicktextChangebackgroundcolor()

{
	CXrayDoc *doc = GetCurrDoc();
	if(!doc) return;

	CLayer *lay = doc->m_currlay;

	if(!lay)
		return;

	CPicSel ps;

	ps.lay = lay;
	ps.liveptr = &lay->gdata.bg_color;
	ps.uppd = PIC_APPLY_BG;

	ps.DoModal();
}


void CShadesDlg::OnRclicktextChangeforegroundcolor()

{
	CXrayDoc *doc = GetCurrDoc();
	if(!doc) return;

	CLayText *lay = (CLayText*)doc->m_currlay;

	if(!lay)
		return;

	CPicSel ps;

	ps.lay = lay;
	ps.liveptr = &lay->gdata.fg_color;
	ps.uppd = PIC_APPLY_FG;

	ps.DoModal();
}

void CShadesDlg::OnRclicktextSendtoback()

{
	CXrayDoc *doc = GetCurrDoc();
	if(!doc) return;

	CLayText *lay = (CLayText*)doc->m_currlay;
	if(!lay) return;

	// Find current in array
	int found = -1, lsize = doc->m_layers.GetSize();
	for(int looplay = 0; looplay < lsize; looplay++)
		{
		if(((CLayer*)doc->m_layers[looplay]) == lay)
			{
			found = looplay;
			break;
			}
		}

	///  ins       tmp
	///   ^ ------- ^
	///   1 2 3 4 5 6 7 8
	///				 -2 -1
	///   6 1 2 3 4 5 7 8

	// Flip it
	if(found >= 0)
		{
		void *tmp = doc->m_layers[found];

		for(int loop = found - 1; loop >= 0; loop--)
			doc->m_layers[loop + 1] = doc->m_layers[loop];

		doc->m_layers[0] = tmp;
		Invalidate(false);
		}
}


void CShadesDlg::OnRclicktextSendtofront()

{
	CXrayDoc *doc = GetCurrDoc();
	if(!doc) return;

	CLayText *lay = (CLayText*)doc->m_currlay;
	if(!lay) return;

	// Find current in array
	int found = -1, lsize = doc->m_layers.GetSize();
	for(int looplay = 0; looplay < lsize; looplay++)
		{
		if(((CLayer*)doc->m_layers[looplay]) == lay)
			{
			found = looplay;
			break;
			}
		}

	///      org     insert
	///       ^ ------- ^
	///   1 2 3 4 5 6 7 8
	///				 -2 -1
	///   1 2 4 5 6 7 8 3

	if(found >= 0)
		{
		void *org = doc->m_layers[found];

		// Shift all others down
		for(int loop = found; loop < lsize - 1; loop++)
			doc->m_layers[loop] = doc->m_layers[loop + 1];

		doc->m_layers[lsize - 1] = org;

		Invalidate(false);
		}
}

//////////////////////////////////////////////////////////////////////////

void CShadesDlg::OnRclicktextSendbackwardonezorder()

{
	CXrayDoc *doc = GetCurrDoc();
	if(!doc) return;

	CLayText *lay = (CLayText*)doc->m_currlay;
	if(!lay) return;

	// Find current in array
	int found = -1, lsize = doc->m_layers.GetSize();
	for(int looplay = 0; looplay < lsize; looplay++)
		{
		if(((CLayer*)doc->m_layers[looplay]) == doc->m_currlay)
			{
			found = looplay;
			break;
			}
		}

	if(found >= 0)
		{
		if(found == 0)
			{
			ShowStatus("Object already at the bottom of Z-Order");
			return;
			}
		void *tmp = doc->m_layers[found - 1];
		doc->m_layers[found - 1] = doc->m_layers[found];
		doc->m_layers[found] = tmp;

		Invalidate(false);
		}
}

//////////////////////////////////////////////////////////////////////////

void CShadesDlg::OnRclicktextSendforwardone()

{

	CXrayDoc *doc = GetCurrDoc();
	if(!doc) return;

	CLayText *lay = (CLayText*)doc->m_currlay;
	if(!lay) return;

	// Find current in array
	int found = -1, lsize = doc->m_layers.GetSize();
	for(int looplay = 0; looplay < lsize; looplay++)
		{
		if(((CLayer*)doc->m_layers[looplay]) == doc->m_currlay)
			{
			found = looplay;
			break;
			}
		}

	if(found >= 0)
		{
		if(found == (lsize - 1))
			{
			ShowStatus("Object already at the top of Z-Order");
			return;
			}

		void *tmp = doc->m_layers[found + 1];
		doc->m_layers[found + 1] = doc->m_layers[found];
		doc->m_layers[found] = tmp;
		Invalidate(false);
		}
}

void CShadesDlg::OnRclicktextAddnewtextdefault()

{
	CXrayDoc *doc = GetCurrDoc();
	if(!doc) return;

	functs->TextCb(this);
}

void CShadesDlg::OnRclicktextAddnewgraytext()
{
	CXrayDoc *doc = GetCurrDoc();
	if(!doc) return;

	doc->graytext = true;
	functs->TextCb(this);
}

void CShadesDlg::OnRclicktextDumptextobjects()

{
	CXrayDoc *doc = GetCurrDoc();
	if(!doc) return;

	int lsize = doc->m_layers.GetSize() ;
	for(int looplay = 0; looplay < lsize; looplay++)
		{
		((CLayer*)doc->m_layers[looplay])->Dump();
		}
}

void CShadesDlg::OnRclicktextAlignAligntotheleft()

{
	CXrayDoc *doc = GetCurrDoc(); if(!doc) return;

	int minleft = INT_MAX;
	int lsize = doc->m_layers.GetSize() ;

	// Find leftmost
	for(int looplay = 0; looplay < lsize; looplay++)
		{
		if( ((CLayer*)doc->m_layers[looplay])->focus)
			if( ((CLayer*)doc->m_layers[looplay])->gdata.xx < minleft)
				minleft = ((CLayer*)doc->m_layers[looplay])->gdata.xx;
		}

	// Set them
	for(int looplay = 0; looplay < lsize; looplay++)
		{
		if( ((CLayer*)doc->m_layers[looplay])->focus)
			{
			((CLayer*)doc->m_layers[looplay])->Move(minleft, -1);
			((CLayer*)doc->m_layers[looplay])->Invalidate();
			}
		}
}

void CShadesDlg::OnRclicktextAlignAligntotheright()

{
	CXrayDoc *doc = GetCurrDoc(); if(!doc) return;

	int maxright = 0;
	int lsize = doc->m_layers.GetSize() ;

	// Find rightmost
	for(int looplay = 0; looplay < lsize; looplay++)
		{
		if( ((CLayer*)doc->m_layers[looplay])->focus)
			if( ((CLayer*)doc->m_layers[looplay])->gdata.xx > maxright)
				maxright = ((CLayer*)doc->m_layers[looplay])->gdata.xx;
		}

	// Set them
	for(int looplay = 0; looplay < lsize; looplay++)
		{
		if( ((CLayer*)doc->m_layers[looplay])->focus)
			{
			((CLayer*)doc->m_layers[looplay])->Move(maxright, -1);
			((CLayer*)doc->m_layers[looplay])->Invalidate();
			}
		}
}

void CShadesDlg::OnRclicktextAlignAlightothetop()

{
	CXrayDoc *doc = GetCurrDoc(); if(!doc) return;

	int top = INT_MAX;
	int lsize = doc->m_layers.GetSize() ;

	// Find topmost
	for(int looplay = 0; looplay < lsize; looplay++)
		{
		if( ((CLayer*)doc->m_layers[looplay])->focus)
			if( ((CLayer*)doc->m_layers[looplay])->gdata.yy < top)
				top = ((CLayer*)doc->m_layers[looplay])->gdata.yy;
		}

	// Set them
	for(int looplay = 0; looplay < lsize; looplay++)
		{
		if( ((CLayer*)doc->m_layers[looplay])->focus)
			{
			((CLayer*)doc->m_layers[looplay])->Move(-1, top);
			((CLayer*)doc->m_layers[looplay])->Invalidate();
			}
		}
}

void CShadesDlg::OnRclicktextAlignAligntothebottom()
{
	CXrayDoc *doc = GetCurrDoc(); if(!doc) return;

	int top = 0;
	int lsize = doc->m_layers.GetSize() ;

	// Find topmost
	for(int looplay = 0; looplay < lsize; looplay++)
		{
		if( ((CLayer*)doc->m_layers[looplay])->focus)
			if( ((CLayer*)doc->m_layers[looplay])->gdata.yy > top)
				top = ((CLayer*)doc->m_layers[looplay])->gdata.yy;
		}

	// Set them
	for(int looplay = 0; looplay < lsize; looplay++)
		{
		if( ((CLayer*)doc->m_layers[looplay])->focus)
			{
			((CLayer*)doc->m_layers[looplay])->Move(-1, top);
			((CLayer*)doc->m_layers[looplay])->Invalidate();
			}
		}
}

static int compare(const void *elem1, const void *elem2 )

{
	CLayer *ptr1 = *((CLayer **)elem1);
	CLayer *ptr2 = *((CLayer **)elem2);

	//P2N("Compare %s to %s\r\n", *ptr1, *ptr2);

	if(ptr1->gdata.xx == ptr2->gdata.xx)
		return 0;

	if(ptr1->gdata.xx > ptr2->gdata.xx)
		return 1;

	return(-1);
}


void CShadesDlg::OnRclicktextAlignHorizontalcenter()
{

}

void CShadesDlg::OnRclicktextAlignDistributehorizontally()

{
	CXrayDoc *doc = GetCurrDoc(); if(!doc) return;

	int lsize = doc->m_layers.GetSize() ;

	// Build array
	CPtrArray  arr;

	// Sort by x position
	for(int looplay = 0; looplay < lsize; looplay++)
		{
		if( ((CLayer*)doc->m_layers[looplay])->focus)
			arr.Add(doc->m_layers[looplay]);
		}
	int asize = arr.GetSize();

	// No sense to do anything here
	if(asize < 3)
		return;

	qsort(arr.GetData(), asize, sizeof(void*), compare);

	int diff = ((CLayer*)arr[asize - 1])->gdata.xx -
				((CLayer*)arr[0])->gdata.xx;

	diff /= (asize - 1);

	for(int loop3 = 0; loop3 < asize; loop3++)
		{
		//P2N("Sorted %d\r\n", ((CLayer*)arr[loop3])->gdata.xx);

		((CLayer*)arr[loop3])->Move(
				((CLayer*)arr[0])->gdata.xx + loop3 * diff, -1);
		((CLayer*)arr[loop3])->Invalidate();
		}
}

static int compare2(const void *elem1, const void *elem2 )

{
	CLayer *ptr1 = *((CLayer **)elem1);
	CLayer *ptr2 = *((CLayer **)elem2);

	//P2N("Compare %s to %s\r\n", *ptr1, *ptr2);

	if(ptr1->gdata.yy == ptr2->gdata.yy)
		return 0;

	if(ptr1->gdata.yy > ptr2->gdata.yy)
		return 1;

	return(-1);
}

void CShadesDlg::OnRclicktextAlignDistributevertically()

{
	CXrayDoc *doc = GetCurrDoc(); if(!doc) return;

	int lsize = doc->m_layers.GetSize() ;

	// Build array
	CPtrArray  arr;

	// Sort by x position
	for(int looplay = 0; looplay < lsize; looplay++)
		{
		if( ((CLayer*)doc->m_layers[looplay])->focus)
			arr.Add(doc->m_layers[looplay]);
		}
	int asize = arr.GetSize();

	// No sense to do anything here
	if(asize < 3)
		return;

	qsort(arr.GetData(), asize, sizeof(void*), compare2);

	int diff = ((CLayer*)arr[asize - 1])->gdata.yy -
				((CLayer*)arr[0])->gdata.yy;

	diff /= (asize - 1);

	for(int loop3 = 0; loop3 < asize; loop3++)
		{
		//P2N("Sorted %d\r\n", ((CLayer*)arr[loop3])->gdata.xx);

		((CLayer*)arr[loop3])->Move(-1,
					((CLayer*)arr[0])->gdata.yy + loop3 * diff);

		((CLayer*)arr[loop3])->Invalidate();
		}
}

void CShadesDlg::OnRclicktextAlignEnsurevisible()

{
	CXrayDoc *doc = GetCurrDoc(); if(!doc) return;

	int lsize = doc->m_layers.GetSize() ;

	for(int looplay = 0; looplay < lsize; looplay++)
		{
		CLayer* xlay = (CLayer*)doc->m_layers[looplay];

		if(xlay->gdata.xx < xpos)
			{
			xlay->Move(xpos, -1);
			xlay->Invalidate();
			}

		if(xlay->gdata.xx + xlay->gdata.ww > xpos + renx)
			{
			xlay->Move(xpos + renx - xlay->gdata.ww, -1);
			xlay->Invalidate();
			}
		}
}

void CShadesDlg::OnRclicknodInserttextobject()

{
	//P2N("CShadesDlg::OnRclicknodInserttextobject\r\n");

#if 1

	CXrayDoc *doc = GetCurrDoc();

	if(!doc)
		return;

	CLayText *lay = new CLayText; ASSERT(lay);
	lay->Init(appmouse);

	// Clear all focuses

	helper->ClearAllObjFocus();

	//int lsize = doc->m_layers.GetSize();
	//for(int looplay2 = 0; looplay2 < lsize; looplay2++)
	//	((CLayer*)doc->m_layers[looplay2])->SetFocus(false);

	doc->AddLayer((CLayer*)lay);
	doc->m_currlay = lay;

	lay->SetFocus();
	Invalidate(false);

#endif

	//TextCb(NULL);
}

void CShadesDlg::OnRclicklineLineproperties()
{
	P2N("Line prop\r\n");
}

void CShadesDlg::OnExitaltAnother()
{
	P2N("Dumping All objects\r\n");

	CXrayDoc *doc = GetCurrDoc();
	if(doc)
		{
		int lsize = doc->m_layers.GetSize() ;
		//P2N("Drawing %d Layers\r\n", lsize);

		for(int looplay = 0; looplay < lsize; looplay++)
			{
			//P2N("Draw Layer %d\r\n", looplay);
			((CLayer*)doc->m_layers[looplay])->Dump();
			}
		}
}

void CShadesDlg::OnExitaltDumpdocuments()

{
	P2N("Dumping All documents\r\n");
	helper->DumpDocs();
}

void CShadesDlg::OnRclicknodocClosecurrentdocument()

{
	functs->CloseFileCb(NULL);
}

void CShadesDlg::OnRclicknodEnsureallobjectsarevisible()

{
	OnRclicktextAlignEnsurevisible();
}

void CShadesDlg::OnRclicknodInsertstraightline()

{
	//P2N("CShadesDlg::OnRclicknodInsertstraightline\r\n");

	CShadesDlg *dlg; GETMAINDIALOG(dlg);
	CXrayDoc *doc; GETCURRDOC(doc, dlg);
	if(!doc)
		return;

	if(GetKeyState(VK_SHIFT)  & 0x80)
		doc->multidraw = true;

	doc->drawstate = TOOL_SLINE;
}

void CShadesDlg::OnRclicknodInsertrectangleobject()

{
	CShadesDlg *dlg; GETMAINDIALOG(dlg);
	CXrayDoc *doc; GETCURRDOC(doc, dlg);
	if(!doc)
		return;

	if(GetKeyState(VK_SHIFT)  & 0x80)
		doc->multidraw = true;

	doc->drawstate = TOOL_RECT;
}

//////////////////////////////////////////////////////////////////////////

void CShadesDlg::OnRclicktextDeleteobject()

{
	//P2N("CShadesDlg::OnRclicktextDeleteobject\r\n");

	CShadesDlg *dlg; GMD(dlg);	CXrayDoc *doc; GCDR(doc, dlg);

	CLayer *lay = doc->m_currlay;

	if(!lay)
		return;

	ASSERT(lay->magic == LAYER_MAGIC);

	// Iterate reverse, delete in case of criteria match
	int len = doc->m_layers.GetSize();
	for(int loop = len - 1; loop >= 0; loop--)
		{
		// Save pointer before removing from array
		CLayer	*xlay = (CLayer*)doc->m_layers[loop];

		if(xlay == lay || xlay->focus)
			{
			doc->m_layers.RemoveAt(loop);
			delete xlay;
			}
		}

	// Reflect that no selection is active
	doc->m_currlay = NULL;

	doc->changed++;
	Invalidate(false);
}

void CShadesDlg::OnRclicknodDrawfreehandlines()

{
	functs->DrawCb(NULL);
}

void CShadesDlg::OnRclicknodDrawinsertcircle()
{
	functs->EllipseCb(NULL);
}

void CShadesDlg::OnRclicknodInsertimageobject()
{
	functs->ImageCb(NULL);
}


void CShadesDlg::OnRclicktextChangeobjecttransparency()

{
	CAlpha al;
	al.DoModal();
}

void CShadesDlg::OnActivateApp(BOOL bActive, HTASK hTask)

{
	CDialog::OnActivateApp(bActive, (DWORD)hTask);

	//P2N("CShadesDlg::OnActivateApp\r\n");

	CPaintDC	dc(this);

	EraseBkgnd(&dc);
	Invalidate();

}

//////////////////////////////////////////////////////////////////////////
// Paint freehand selection

void	CShadesDlg::DrawFreeHandSel(CDC *pDC)

{
	// -------------------------------------------------------------------
    int cnt = xmarkarr.GetSize() ;
    if(cnt > 1)
        {
        pDC->MoveTo(xmarkarr[0], ymarkarr[0]);

        int old_x = 0, old_y = 0, loop;
        for(loop = 1; loop < cnt; loop++)
            {
            pDC->LineTo(xmarkarr[loop], ymarkarr[loop]);
            }

        if(!lbdown)
            pDC->LineTo(xmarkarr[0], ymarkarr[0]);

        if(!lbdown)
            for(loop = 0; loop < cnt; loop++)
            {
            //if(ABS(int(old_x - xmarkarr[loop]) ) > 12 || ABS(int(old_y - ymarkarr[loop]) ) > 12)
                {
                old_x =  xmarkarr[loop];
                old_y =  ymarkarr[loop];

                //if(GetAsyncKeyState(VK_SHIFT))
                pDC->Rectangle(
                        xmarkarr[loop] - 2, ymarkarr[loop] - 2,
                            xmarkarr[loop] + 2, ymarkarr[loop] + 2);
                }
            }

        // Final rect
        //pDC->Rectangle(
        //          xmarkarr[loop] - 3, ymarkarr[loop] - 3,
        //              xmarkarr[loop] + 3, ymarkarr[loop] + 3);
        //
        }

    // Paint selected rectangle
    if(selrect.right - selrect.left > 0  && selrect.bottom - selrect.top > 0)
        {
        //P2N("Painting sel rect\r\n");

        CPen dashed(PS_DOT   , 1, RGB(0,0,0));

        CPen *old =  pDC->SelectObject( &dashed);

        pDC->MoveTo(selrect.left, selrect.top);

        pDC->LineTo(selrect.right, selrect.top);
        pDC->LineTo(selrect.right, selrect.bottom);
        pDC->LineTo(selrect.left, selrect.bottom);
        pDC->LineTo(selrect.left, selrect.top);

        pDC->SelectObject(old);

        int hesize = 2;

        if(selrect.right - selrect.left > 50)
            hesize = 4;

        // Corner points
        pDC->Rectangle(selrect.right - hesize, selrect.top - hesize,
                        selrect.right + hesize, selrect.top + hesize);

        pDC->Rectangle(selrect.right - hesize, selrect.bottom - hesize,
                        selrect.right + hesize, selrect.bottom + hesize);

        pDC->Rectangle(selrect.left - hesize, selrect.bottom - hesize,
                        selrect.left + hesize, selrect.bottom + hesize);

        pDC->Rectangle(selrect.left - hesize, selrect.top - 3,
                        selrect.left + hesize, selrect.top + hesize);

        //int hquart = selrect.left + (selrect.right - selrect.left) / 4 ;
        //
        // Move handle
        //pDC->Rectangle(hquart - hesize * 4, selrect.top - hesize ,
        //              hquart + hesize * 4, selrect.top + hesize);

        int hmid = selrect.left + (selrect.right - selrect.left) / 2 ;
        int vmid = selrect.top + (selrect.bottom - selrect.top) / 2 ;

        // Mid points
        pDC->Rectangle(hmid - hesize, selrect.top - hesize,
                        hmid + hesize, selrect.top + hesize);

        pDC->Rectangle(selrect.right - hesize, vmid - hesize,
                        selrect.right + hesize, vmid + hesize);

        pDC->Rectangle(hmid - hesize, selrect.bottom - hesize,
                        hmid + hesize, selrect.bottom + hesize);

        pDC->Rectangle(selrect.left - hesize, vmid - hesize,
                        selrect.left + hesize, vmid + hesize);
        }
}

//////////////////////////////////////////////////////////////////////////
// Paint freehand selection

void	CShadesDlg::DrawImage(CDC *pDC)

{
	CXrayDoc *doc = ((CXrayDoc*)docarr[current]);

	RECT    rc;   GetClientRect(&rc);

    int sdiff = doc->img->GetWidth() - (rc.right - rc.left);
    int hdiff = doc->img->GetHeight() - (rc.bottom - rc.top);

    sdiff = MAX(sdiff, 0);
    hdiff = MAX(hdiff, 0);

    //P2N("Painting state=%d ww=%d hh=%d\r\n", doc->showstate,
	//						doc->img->GetWidth(),  doc->img->GetHeight());

    xpos = 0, ypos = 0, renx = 0, reny = 0;

    switch(doc->showstate)
        {
		case STATE_MUL:
			{
			P2N("CShadesDlg::OnPaint STATE_MUL currframe=%d\r\n", doc->currframe);

			CFont* oldFont = pDC->SelectObject(&m_Font);
			pDC->SetBkMode(TRANSPARENT);

			int topgap = 10, gap = 1;

			// ----------------------
			// |      |      |      |
			// ----------------------
			// |      |      |      |
			// ----------------------

			if(saptr)
				delete [] saptr;

			saptr = new showarr[doc->num_to_show];

			// Init stucture:
			for(int loop = 0; loop < doc->num_to_show; loop++)
				{
				saptr[loop].renx = saptr[loop].reny = 0;
				saptr[loop].xpos = saptr[loop].ypos = 0;
				saptr[loop].frame = 0;
				}

			int		idx = doc->currframe;
			int		cnt = 0;

			// Collect images to render
			for(int loop2 = 0; loop2 < doc->imgarr.GetSize(); loop2++)
				{
				saptr[cnt].frame = idx;
				saptr[cnt].img = (CxImg*)doc->imgarr[idx];

				cnt++; idx++;

				if(cnt >= doc->num_to_show)
					break;

				if(idx >= doc->imgarr.GetSize())
					idx = 0;
				}

			int ycnt = (int)floor(sqrt(doc->num_to_show));
			int	xcnt = (int)ceil((double)doc->num_to_show / ycnt);

			//P2N("Mul render %d xcnt=%d  ycnt=%d\r\n", doc->num_to_show, xcnt, ycnt);

			int imgww = doc->img->GetWidth();
			int imghh = doc->img->GetHeight();

			// Precalc with left gap fixed
			int scrhh = (rc.bottom - rc.top) -  ((ycnt + 1) * gap + 2 * topgap);
			int scrww = (rc.right - rc.left) -  ((xcnt + 1) * gap );

			int onehh = scrhh / ycnt;
			int realwidth = onehh * imgww/imghh;
			int leftgap = (scrww - (realwidth + gap) * xcnt) / 2;

			//P2N("leftgap=%d\r\n", leftgap);

			// OVERFLOW: Precalc again with top gap fixed
			if(leftgap < 0)
				{
				//P2N("OVERFLOW: Precalc with left gap %d\r\n", leftgap);

				leftgap = 10;
				topgap  = 10;

				scrhh = (rc.bottom - rc.top) -  ((ycnt + 1) * gap);
				scrww = (rc.right - rc.left) -  ((xcnt + 1) * gap + 2 * leftgap);

				realwidth = scrww / xcnt;
				onehh	  = realwidth * imghh/imgww;
				topgap = (scrhh - (onehh + gap) * ycnt) / 2;
				}

			//int hgap =  ((rc.right - rc.left) - xcnt * realwidth) / xcnt;
			//sww = rc.right - rc.left -  (xcnt + 1) * hgap;
			//int oneww = sww / xcnt;
			//P2N("onehh=%d imghh=%d oneww=%d imgww=%d\r\n",
			//	onehh, imghh, oneww,  imgww);

			int xxx = leftgap, yyy = topgap, horiz_prog = 0;

			ASSERT(doc->imgarr.GetSize() == doc->rendarr.GetSize());

			// Fill in stucture from current doc and paint ...
			for(int loop3 = 0; loop3 < doc->num_to_show; loop3++)
				{
				saptr[loop3].renx = onehh * imgww/imghh;
				saptr[loop3].reny = onehh;

				saptr[loop3].xpos = xxx;
				saptr[loop3].ypos = yyy;

				xxx += realwidth + gap;
				horiz_prog++;

				if(horiz_prog >= xcnt)
					{
					horiz_prog = 0;
					xxx = leftgap;
					yyy += onehh + gap;
					}

				//CXrayDoc *doc2 = saptr[loop3].doc;

				// Resample on demand
				if((doc->showstate != doc->oldshowstate) || (doc->num_to_show != doc->old_num_to_show)
					|| doc->currframe != doc->oldcurrframe)
					{
					doc->oldcurrframe = doc->currframe;

					BusyCursor();
					//P2N("Resample for STATE_MUL render %d %d\r\n", saptr[loop3].renx, saptr[loop3].reny);

					((CxImg*)doc->imgarr[saptr[loop3].frame])->Resample(saptr[loop3].renx, saptr[loop3].reny,  0,
											(CxImg*)doc->rendarr[loop3]);
					BusyCursor(false);
					}

				//P2N("Render %d %d %d %d\r\n", saptr[loop3].xpos, saptr[loop3].ypos,
				//	saptr[loop3].renx, saptr[loop3].reny);

				((CxImg*)doc->rendarr[loop3])->DrawX(pDC->GetSafeHdc(),  saptr[loop3].xpos, saptr[loop3].ypos,
							saptr[loop3].renx, saptr[loop3].reny,
									NULL, false, hscroll * sdiff / 100, vscroll * hdiff / 100);

				// Draw frame number on image
				CString nstr; nstr.Format("%d", saptr[loop3].frame);

				CRect rc(saptr[loop3].xpos + 4, saptr[loop3].ypos + 4,
							saptr[loop3].xpos + 24, saptr[loop3].ypos + 24);

				rc.OffsetRect(-1, -1);
				pDC->SetTextColor(RGB(32, 32, 32));
				pDC->DrawText(nstr, rc, DT_CENTER);

				rc.OffsetRect(1, 1);
				pDC->SetTextColor(RGB(128, 128, 128));
				pDC->DrawText(nstr, rc, DT_CENTER);
				}

			// Reset render variables
			doc->oldshowstate = doc->showstate;
					doc->old_num_to_show = doc->num_to_show;

			//delete [] saptr;

			pDC->SelectObject(oldFont);

			break;
			}

        case STATE_WIDTH:
            {
            renx = (rc.right - rc.left);
            reny = (renx *  doc->img->GetHeight()) / doc->img->GetWidth();

            // Needs to scroll this much:
            sdiff = renx - (rc.right - rc.left);
            hdiff = reny - (rc.bottom - rc.top);

            sdiff = MAX(sdiff, 0);
            hdiff = MAX(hdiff, 0);

            // Align to the middle if smaller
            if(renx < (rc.right - rc.left))
                {
                xpos = (rc.right - rc.left) / 2 - renx / 2;
                }
            if(reny < (rc.bottom - rc.top))
                {
                ypos = (rc.bottom - rc.top) / 2 - reny / 2;
                }

            //P2N("scale ww=%d hh=%d\r\n", renx, reny);

            //RGBQUAD fill = {10, 10, 10, 10};
            //doc->render->DrawLineX(10, 10, 200, 200, fill);
            //doc->render->DrawString(NULL, 32, 32, "Prepaired with XrayNotes Demo Version", fill, "Courier", 32);

            //doc->render->DrawX(pDC->GetSafeHdc(),  xpos, ypos, renx, reny,
              //          NULL, false, hscroll * sdiff / 100, vscroll * hdiff / 100);

			if(doc->IsDicomFile())
				{
				((CxImg*)doc->rendarr[doc->currframe])->DrawX(pDC->GetSafeHdc(),  xpos, ypos, renx, reny,
							NULL, false, hscroll * sdiff / 100, vscroll *  hdiff / 100);
				}
			else
				{
				doc->render->DrawX(pDC->GetSafeHdc(),  xpos, ypos, renx, reny,
							NULL, false, hscroll * sdiff / 100, vscroll * hdiff / 100);
				}

            // Paint MOIRE
            if(moire)
                {
                RECT rc; GetClientRect(&rc);

                CBitmap     m_bitmap, m_bitmapb;

                //m_bitmapb.LoadBitmap(IDB_BITMAP20);
                m_bitmapb.LoadBitmap(IDB_BITMAP18);

                CDC  dcMem; dcMem.CreateCompatibleDC(pDC);
				int  save = dcMem.SaveDC();

                m_bitmap.CreateCompatibleBitmap(pDC, rc.right, rc.bottom);

                CBitmap *pOldBitmap  = (CBitmap *)SelectObject(dcMem, m_bitmap);

                CBrush  bb; bb.CreatePatternBrush(&m_bitmapb);
                CBrush *old = (CBrush *)dcMem.SelectObject(bb );

                dcMem.PatBlt( rc.top, rc.left, rc.right, rc.bottom, PATCOPY);
                pDC->BitBlt(rc.top, rc.left, rc.right, rc.bottom, &dcMem, 0, 0, SRCAND);

                SelectObject(dcMem, pOldBitmap);
				SelectObject(dcMem, old);
				dcMem.RestoreDC(save);
                }
            }
            break;

        case STATE_HEIGHT:
            {
            reny = (rc.bottom - rc.top);
            renx = (reny *  doc->img->GetWidth()) / doc->img->GetHeight();

            sdiff = renx - (rc.right - rc.left);
            hdiff = reny - (rc.bottom - rc.top);

            sdiff = MAX(sdiff, 0); hdiff = MAX(hdiff, 0);

            // Align to the middle if smaller
            if(renx < (rc.right - rc.left))
                xpos = (rc.right - rc.left) / 2 - renx / 2;

            if(reny < (rc.bottom - rc.top))
                ypos = (rc.bottom - rc.top) / 2 - reny / 2;

            if(doc->IsDicomFile())
				{
				((CxImg*)doc->rendarr[doc->currframe])->DrawX(pDC->GetSafeHdc(),  xpos, ypos, renx, reny,
							NULL, false, hscroll * sdiff / 100, vscroll *  hdiff / 100);
				}
			else
				{
				doc->render->DrawX(pDC->GetSafeHdc(),  xpos, ypos, renx, reny,
							NULL, false, hscroll * sdiff / 100, vscroll * hdiff / 100);
				}
            break;
            }

        case STATE_FULL:
            {
            reny = doc->img->GetHeight();
            renx = doc->img->GetWidth();

            // Needs to scroll this much:
            sdiff = renx - (rc.right - rc.left);
            hdiff = reny - (rc.bottom - rc.top);

            sdiff = MAX(sdiff, 0);
            hdiff = MAX(hdiff, 0);

            // Align to the middle if smaller
            if(doc->img->GetWidth() < (unsigned)(rc.right - rc.left))
                {
                xpos = (rc.right - rc.left) / 2 - doc->img->GetWidth() / 2;
                }
            if(doc->img->GetHeight() < (unsigned)(rc.bottom - rc.top))
                {
                ypos = (rc.bottom - rc.top) / 2 - doc->img->GetHeight() / 2;
                }

			if(doc->IsDicomFile())
				{
				((CxImg*)doc->rendarr[doc->currframe])->DrawX(pDC->GetSafeHdc(),  xpos, ypos, renx, reny,
							NULL, false, hscroll * sdiff / 100, vscroll *  hdiff / 100);
				}
			else
				{
				doc->render->DrawX(pDC->GetSafeHdc(),  xpos, ypos, renx, reny,
							NULL, false, hscroll * sdiff / 100, vscroll * hdiff / 100);
				}
            }
            break;

        // ---------------------------------------------------------------
        case STATE_VARI:
        case STATE_FINE:

            reny = (int)(doc->mag * doc->img->GetHeight() );
            renx = (int)(doc->mag * doc->img->GetWidth()  );

            // Needs to scroll this much:
            sdiff = renx - (rc.right - rc.left);
            hdiff = reny - (rc.bottom - rc.top);

            sdiff = MAX(sdiff, 0);
            hdiff = MAX(hdiff, 0);

            // Align to the middle if smaller
            if(renx < (rc.right - rc.left))
                {
                xpos = (rc.right - rc.left) / 2 - renx / 2;
                }
            if(reny < (rc.bottom - rc.top))
                {
                ypos = (rc.bottom - rc.top) / 2 - reny / 2;
                }

            // Is it magnified?
            if(doc->mag > 1)
                {
                //CWaitCursor cursor;

                if(doc->oldshowstate != doc->showstate)
                    {
                    // Temporary set for re-entry
                    doc->oldshowstate = doc->showstate;

                    //ShowStatus("Rendering to VARIABLE View ...");

                    int method = 0;

                    //ShowStatus("Rendering to VARIABLE Please wait ...", 0);

                    P2N("Rendering magnified view scrollx=%d scrolly=%d sdiff=%d hdiff=%d\r\n",
                                hscroll, vscroll, hscroll * sdiff / 100, vscroll * hdiff / 100);

                    RECT  clip; GetClientRect(&clip);
                    //clip.right /= 2; clip.bottom /= 2;
                    //HideStatus();

                    doc->img->DrawX(pDC->GetSafeHdc(),  xpos, ypos, renx, reny,
                                    &clip, doc->finestate,
                                        hscroll * sdiff / 100, vscroll * hdiff / 100);

                    // There is a need for new render in magnified view
                    doc->oldshowstate = -1;
                    }
                }
            else
                {
                //CWaitCursor cursor;

                //if(doc->oldshowstate != doc->showstate)
                //  {
                //  P2N("Mag 1 or less: Resample to : xx=%d yy=%d\r\n", renx, reny);
//
//                  //ShowStatus("Rendering to VARIABLE View ...");
//
//                  doc->oldshowstate = doc->showstate;
//                  doc->img->Resample(renx, reny,  1,  doc->render);
//
//                  //HideStatus();
//                  }


                doc->render->DrawX(pDC->GetSafeHdc(),  xpos, ypos, renx, reny,
                        NULL, false, hscroll * sdiff / 100, vscroll * hdiff / 100);
                }

            // Paint MOIRE
            if(moire)
                {
                RECT rc; GetClientRect(&rc);

                CBitmap     m_bitmap, m_bitmapb;

                m_bitmapb.LoadBitmap(IDB_BITMAP20);
                //m_bitmapb.LoadBitmap(IDB_BITMAP18);

                CDC  dcMem;  dcMem.CreateCompatibleDC(pDC);
				int  save = dcMem.SaveDC();

                m_bitmap.CreateCompatibleBitmap(pDC, rc.right, rc.bottom);

                CBitmap *pOldBitmap  = (CBitmap *)SelectObject(dcMem, m_bitmap);

                CBrush  bb; bb.CreatePatternBrush(&m_bitmapb);
                CBrush *old = (CBrush *)dcMem.SelectObject(bb );
                dcMem.PatBlt( rc.top, rc.left, rc.right, rc.bottom,   PATCOPY      );

                pDC->BitBlt(rc.top, rc.left, rc.right, rc.bottom, &dcMem, 0, 0, SRCAND );

                SelectObject(dcMem, pOldBitmap);
                }
            break;

            } // switch(showstate)
}


void CShadesDlg::OnRclicktextRotateobject()

{
	//P2N("CShadesDlg::OnRclicktextRotateobject\r\n");

	CXrayDoc *doc = GetCurrDoc();
	if(!doc) return;

	CLayer *lay = doc->m_currlay;

	if(!lay)
		return;

	lay->state = STATE_ROTATE;
	lay->Invalidate();
}

void CShadesDlg::OnRclicknodocShowonscreenkeyboard()
{
	kbd.Show();
}

void CShadesDlg::OnRclicktextMoveobject()

{

	CXrayDoc *doc = GetCurrDoc();
	if(!doc) return;

	CLayer *lay = doc->m_currlay;

	if(!lay)
		return;

	lay->state = STATE_MOVE;
	lay->Invalidate();
}

void CShadesDlg::OnRclicktextResize()
{

	CXrayDoc *doc = GetCurrDoc();
	if(!doc) return;

	CLayer *lay = doc->m_currlay;

	if(!lay)
		return;

	lay->state = STATE_RESIZE;
	lay->Invalidate();
}

void CShadesDlg::OnRclicknodocToggleminmax()

{
	//P2N("CShadesDlg::OnRclicknodocToggleminmax\r\n");

	minmax = !minmax;

	HDC     hdc     = ::GetDC(HWND_DESKTOP);
    int     xsize   = ::GetDeviceCaps(hdc, HORZRES);
    int     ysize   = ::GetDeviceCaps(hdc, VERTRES);

	if(minmax)
		{
		SetWindowPos( NULL, xsize / 8 , ysize / 8, 3 * xsize / 4, 3 * ysize / 4, SWP_NOZORDER);

		RECT rc; GetWindowRect(&rc);

		//nw.Move(rc.left + 10, rc.top + 10);
		//sw.Move(rc.left + 10, rc.bottom - 100);
		//ne.Move(rc.right - 40, rc.top + 10);
		//se.Move(rc.right - 40, rc.bottom - 100);
		}
	else
		{
		SetWindowPos( NULL, 1, 1, xsize - 1, ysize - 1, SWP_NOZORDER);

		RECT rc; GetWindowRect(&rc);

		//nw.Move(rc.left + 10, rc.top + 10);
		//sw.Move(rc.left + 10, rc.bottom - 100);
		//ne.Move(rc.right - 40, rc.top + 10);
		//se.Move(rc.right - 40, rc.bottom - 100);
		}

	Invalidate();
}

void CShadesDlg::OnMove(int x, int y)

{
	CDialog::OnMove(x, y);

	if(IsWindow(nw.m_hWnd))
		{
		RECT rc; GetWindowRect(&rc);

		nw.Move(rc.left + 10, rc.top + 10);
		sw.Move(rc.left + 10, rc.bottom - 100);
		ne.Move(rc.right - 40, rc.top + 10);
		se.Move(rc.right - 40, rc.bottom - 100);
		}
}

//////////////////////////////////////////////////////////////////////////

void CShadesDlg::InitDlgPos()

{
	RECT rc; GetClientRect(&rc);

	CONFIGDLG(dicomwin, "dicomwin", rc.right - 300, rc.top + 70, -1, -1);
	CONFIGDLG(magdlg, "magdlg", rc.left + 80, rc.bottom - 122, -1, -1);
	CONFIGDLG(pydlg, "pydlg", rc.left + 80, 100, -1, -1);
	CONFIGDLG(gesdlg, "gesdlg", rc.left + 80, rc.bottom - 380, -1, -1)
	CONFIGDLG(magwin, "magwin", (rc.right - rc.left) / 2, (rc.bottom - rc.top) / 2, -1, -1)
	CONFIGDLG(xyzdlg, "xyzdlg", rc.left + 80, rc.top + 150, -1, -1);

	// Scroll bars
	CONFIGDLG(xscroll, "xscroll", rc.right - (xscroll.want_w + 50),
				rc.bottom - 100, rc.right - 600, 45);

	CONFIGDLG(yscroll, "yscroll", rc.right - 60, 90, 45, rc.bottom - 200);
}

//////////////////////////////////////////////////////////////////////////

HICON	CShadesDlg::myLoadCursor(UINT res)

{
	HICON icc = AfxGetApp()->LoadCursor(res);
	ASSERT(icc);

	return icc;
}

//////////////////////////////////////////////////////////////////////////

void CShadesDlg::OnRclicknodocResetdialogpositions()

{
	//xMsgBox("OnRclicknodocResetdialogpositions");

	InitDlgPos();

	RECT rc; GetWindowRect(&rc);

	for(int xxloop = 0; winlist[xxloop] != NULL; xxloop++)
		{
		PersistDlg *dlg = (PersistDlg*)winlist[xxloop];
		dlg->MoveToPos(rc.left, rc.top);
		}

	helper->InitChevPos(rc.left, rc.top);

	Invalidate();

}

