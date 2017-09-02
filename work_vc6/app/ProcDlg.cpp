
/* =====[ ProcDlg.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the ProcDlg.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  5/14/2007  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#include "stdafx.h"
#include "xraynotes.h"
#include "shades.h"
#include "shadesDlg.h"
#include "ProcDlg.h"
#include "worker.h"
#include "xImg.h"

#include <process.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	SOFTEN	1
#define SHARPEN	2

//////////////////////////////////////////////////////////////////////////
//
// Function name:    RunWorkerThread
// Description:      <empty description>
// Return type:      
// Argument:         void *lpParam -- POINTER TO DOCUMENT
//
//////////////////////////////////////////////////////////////////////////

void	RunWorkerThread(void *lpParam)

{
	CXrayDoc *doc = (CXrayDoc*)lpParam;
	
	if(doc == NULL)
		return;
	
	ASSERT(doc->magic == DOC_MAGIC);

	CShadesDlg *dlg = (CShadesDlg* )AfxGetApp()->m_pMainWnd;
	ASSERT(dlg->magic == SHADE_MAGIC);

	int processed = true;

	CxImg *img = doc->render;

	//if(doc->preview)
	//	img = doc->render;
	//else
	//	img = doc->img;

	if(doc->commit)
		img = doc->img;

	if(!img->IsValid())
		{
		return;
		}

	dlg->BusyCursor(true);
	
	// Prepare for elaboration
    img->SetProgress(0);
    img->SetEscape(0);

	switch(doc->command)
		{
		case SOFTEN:
			{
			ShowStatus("Rendering Softer Image", 1);	
			long kernel[]={1,1,1,1,8,1,1,1,1};
			img->Filter(kernel,3,16,0);
			}
		break;

		case SHARPEN:
			{
			ShowStatus("Rendering Sharper Image", 1);	
			long kernel[]= {-1,-1,-1,-1,15,-1,-1,-1,-1};
			img->Filter(kernel,3,7,0);
			}
		break;

		case PROC_EMBOSS:
			{
			ShowStatus("Rendering Embossed Image", 1);	
			long kernel[]={0,0,-1,0,0,0,1,0,0};
			img->Filter(kernel,3,0,127);
			}
		break;

		case PROC_ERODE:
			{
			ShowStatus("Rendering Eroded Image", 1);	
			//long kernel[]={0,0,-1,0,0,0,1,0,0};
			//doc->img->Filter(kernel,3,0,127);
			img->Erode();
			}
		break;

		case PROC_NEGATIVE:
			{
			ShowStatus("Rendering Negative Image", 1);	
			img->Negative();
			}
		break;
   	
		case PROC_HISTOGRAM_LOG:
			{
			ShowStatus("Rendering Histogram", 1);	
			img->HistogramRoot();
			}
		break;

		case PROC_HISTOGRAM_ROOT:
			{
			ShowStatus("Rendering Histogram", 1);	
			img->HistogramLog();
			}
		break;

		case PROC_HISTOGRAM_EQUALIZE:
			{
			ShowStatus("Rendering Histogram", 1);	
			img->HistogramEqualize();
			}
		break;

		case PROC_HISTOGRAM_NORMALIZE:
			{
			ShowStatus("Rendering Histogram", 1);	
			img->HistogramNormalize();
			}
		break;

		case PROC_HISTOGRAM_STRETCH:
			{
			ShowStatus("Rendering Histogram Stretch", 1);	
			img->HistogramStretch();
			}
		break;

		case PROC_HISTOGRAM_STRETCH1:
			{
			ShowStatus("Rendering Histogram Stretch (linked)", 1);	
			img->HistogramStretch(1);
			}
		break;

		case PROC_GRAYSCALE:
			{
			ShowStatus("Rendering Histogram Stretch (linked)", 1);	
			img->GrayScale();
			}
		break;

		case PROC_REPAIR:
			{
			ShowStatus("Rendering Repair/Noise reduction (time consuming)", 1);	
			img->Repair();
			}
		break;

		case PROC_TEMPLATE:
			{
			ShowStatus("Rendering Template", 1);	
			img->Template();
			}
		break;

		case PROC_DILATE:
			{
			ShowStatus("Rendering Dilate", 1);	
			img->Dilate();
			}
		break;

		case PROC_CONTOUR:
			{
			ShowStatus("Rendering Contour", 1);	
			img->Contour();
			}
		break;

		case PROC_JITTER:
			{
			ShowStatus("Rendering Jitter", 1);	
			img->Jitter();
			}
		break;

		case PROC_EDGE:
			{
			ShowStatus("Rendering Edge", 1);	
			img->Edge();
			}
		break;
		
	case PROC_CIRCLETRANSFORM_PINCH:
			{
			ShowStatus("Rendering Pinch", 1);	
			img->CircleTransform(1,0,100);
			}
		break;

	case PROC_CIRCLETRANSFORM_PUNCH:
			{
			ShowStatus("Rendering Punch", 1);	
			img->CircleTransform(0,0,100);
			}
		break;

	case PROC_LIGHT:
			{
			ShowStatus("Rendering Brightness/Contrast", 1);	
			img->Copy(*doc->img);
			img->Light(doc->arg1, doc->arg2);
			}
		break;

	case PROC_COLORIZE:
			{
			ShowStatus("Rendering Colorize", 1);	
			img->Copy(*doc->img);
			img->ShiftRGB(doc->arg1, doc->arg2, doc->arg3);
			}
		break;

	default:
			{
			ShowStatus("Unknown processing opcode", 0);	
			Sleep(300);
			processed = false;
			}
		break;
	}

	HideStatus();	
	
	dlg->BusyCursor(false);

	if(processed)
		{
		// Aborted
		if(img->GetEscape())
			{
			img->SetEscape(0);
			}
		else
			{
			doc->changed = true;
			}

		//doc->NewRender();
		//doc->changed = true;

		CShadesDlg *shadesdlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);
		ASSERT(dlg->magic == SHADE_MAGIC);

		if(shadesdlg->current >= 0)
			{
			shadesdlg->Invalidate(false);
			support.YieldToWinEx();
			}
		}
	else
		{
		//changed--;
		P2N("Invalid opcode\r\n");
		}
		
	doc->thhandle = 0;
	_endthread();
    return;
}

//////////////////////////////////////////////////////////////////////////
#if 0

void	RunPreviewThread(void *lpParam)

{
	CXrayDoc *doc = (CXrayDoc*)lpParam;
	
	if(!doc)
		return;
	
	ASSERT(doc->magic == DOC_MAGIC);

	 // Prepare for elaboration
    doc->img->SetProgress(0);
    doc->img->SetEscape(0);

	int processed = true;

	switch(doc->command)
		{
		case PROC_LIGHT:
				{
				ShowStatus("Rendering Brightness/Contrast", 0);	
				doc->render->Copy(*doc->img);
				doc->render->Light(doc->arg1, doc->arg2);
				}
			break;

		default:
				{
				ShowStatus("Unknown processing opcode", 1);	
				processed = false;
				}
			break;
		}

	HideStatus();	
		
	if(processed)
		{
		CShadesDlg *shadesdlg = (CShadesDlg* )AfxGetApp()->m_pMainWnd;
		ASSERT(shadesdlg->magic == SHADE_MAGIC);

		if(shadesdlg->current >= 0)
			{
			shadesdlg->Invalidate(false);
			YieldToWinEx();
			}
		}
	else
		{
		P2N("Invalid opcode\r\n");
		}

	doc->thhandle = 0;
	_endthread();

    return;
}

#endif

/////////////////////////////////////////////////////////////////////////////
// CProcDlg dialog

CProcDlg::CProcDlg(CWnd* pParent /*=NULL*/)
	: CGrayDlg(CProcDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProcDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	changed = 0;

	windowname = "CProcDlg"; //support.GetFileNameExt(__FILE__);
}

void CProcDlg::DoDataExchange(CDataExchange* pDX)

{
	CGrayDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProcDlg)
	DDX_Control(pDX, IDOK, m_OK);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	DDX_Control(pDX, IDC_BUTTON1, m_Soften);
	DDX_Control(pDX, IDC_BUTTON2, m_Sharpen);
	DDX_Control(pDX, IDC_BUTTON3, m_Reset);
	DDX_Control(pDX, IDC_BUTTON4, m_Erode);
	DDX_Control(pDX, IDC_BUTTON5, m_Emboss);
	DDX_Control(pDX, IDC_BUTTON6, m_Log);
	DDX_Control(pDX, IDC_BUTTON7, m_Root);
	DDX_Control(pDX, IDC_BUTTON8, m_Reverse);
	DDX_Control(pDX, IDC_BUTTON9, m_Punch);
	DDX_Control(pDX, IDC_BUTTON10, m_Equalize);
	DDX_Control(pDX, IDC_BUTTON11, m_Normalize);
	DDX_Control(pDX, IDC_BUTTON12, m_Stretch);
	DDX_Control(pDX, IDC_BUTTON13, m_Pinch);
	DDX_Control(pDX, IDC_BUTTON14, m_GrayScale);
	DDX_Control(pDX, IDC_BUTTON15, m_Dilate);
	DDX_Control(pDX, IDC_BUTTON16, m_Contour);
	DDX_Control(pDX, IDC_BUTTON17, m_Jitter);
	DDX_Control(pDX, IDC_BUTTON18, m_Edge);
	DDX_Control(pDX, IDC_BUTTON19, m_GrayScale2);
	DDX_Control(pDX, IDC_BUTTON20, m_Gamma);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CProcDlg, CGrayDlg)
	//{{AFX_MSG_MAP(CProcDlg)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON8, OnButton8)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	ON_BN_CLICKED(IDC_BUTTON10, OnButton10)
	ON_BN_CLICKED(IDC_BUTTON11, OnButton11)
	ON_BN_CLICKED(IDC_BUTTON12, OnButton12)
	ON_BN_CLICKED(IDC_BUTTON13, OnButton13)
	ON_BN_CLICKED(IDC_BUTTON14, OnButton14)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON15, OnButton15)
	ON_BN_CLICKED(IDC_BUTTON16, OnButton16)
	ON_BN_CLICKED(IDC_BUTTON17, OnButton17)
	ON_BN_CLICKED(IDC_BUTTON18, OnButton18)
	ON_BN_CLICKED(IDC_BUTTON9, OnButton9)
	ON_WM_SETFOCUS()
	ON_WM_MBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON21, OnButton21)
	ON_BN_CLICKED(IDC_BUTTON19, OnButton19)
	ON_BN_CLICKED(IDC_BUTTON20, OnButton20)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProcDlg message handlers

BOOL CProcDlg::OnInitDialog() 

{
	CGrayDlg::OnInitDialog();
	
	CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);
	ASSERT(dlg->magic == SHADE_MAGIC);

	m_title = "Image Processing Dialog";

	P2N("CProcDlg::OnInitDialog IDD=%d\r\n", IDD);

	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);
	
	WINDOWPLACEMENT pwp;
	((CWnd *)parent)->GetWindowPlacement(&pwp);

	if(!posload)
		{
		// Flush Right
		want_x = pwp.rcNormalPosition.right - 
					(wp.rcNormalPosition.right - wp.rcNormalPosition.left) - 80;

		// Middle of vertical
		want_y = (pwp.rcNormalPosition.bottom - pwp.rcNormalPosition.top) / 2 - 
					(wp.rcNormalPosition.bottom - wp.rcNormalPosition.top) / 2;
		}
	else
		{
		want_x = pre_want_x;
		want_y = pre_want_y;
		}

	SetWindowPos(NULL, want_x, want_y, 0, 0,  SWP_NOZORDER | SWP_NOSIZE );  

	CShadesDlg *shadesdlg = (CShadesDlg* )AfxGetApp()->m_pMainWnd;
	ASSERT(shadesdlg->magic == SHADE_MAGIC);

#if 0
	if(shadesdlg->current >= 0)
		{
		CXrayDoc *doc = (CXrayDoc*)(shadesdlg->docarr[shadesdlg->current]); 
	
		//ASSERT(doc);
		//ASSERT(doc->magic == DOC_MAGIC);

		if(doc)
			{
			P2N("Making copy of the original\r\n");
			ASSERT(doc->magic == DOC_MAGIC);
			doc->org->Copy(*doc->img);
			}
		}
#endif

	//m_Soften.SetToolTipText();

	m_OK        .SetToolTipText("Apply current processed view to the whole image, \r\nand close processing dialog");
	m_Cancel    .SetToolTipText("Restore image to the point in time this dialog was opened, \r\nand close processing dialog");
	m_Soften    .SetToolTipText("Soften Image DSP Kernel 10101");
	m_Sharpen   .SetToolTipText("Sharpen Image DSP Kernel 10101");
	m_Reset     .SetToolTipText("Restore image to the point in time this dialog was opened");
	m_Erode     .SetToolTipText("Erode/Dull Image");
	m_Emboss    .SetToolTipText("Emboss Image");
	m_Log       .SetToolTipText("Apply log algorythm to image gamma");
	m_Root      .SetToolTipText("Apply root algorythm to image gamma");
	m_Reverse   .SetToolTipText("Black/While to While/Black");
	m_Punch     .SetToolTipText("Distort with Punch");
	m_Equalize  .SetToolTipText("Equalize image gamma");
	m_Normalize .SetToolTipText("Normalize image gamma");
	m_Stretch   .SetToolTipText("Strech image gamma");
	m_Pinch     .SetToolTipText("Distort with Pinch");
	m_GrayScale .SetToolTipText("Convert Image to GrayScale. Color depth is not changed");
	m_Dilate    .SetToolTipText("Distort image with Dilate ");
	m_Contour   .SetToolTipText("Contour Image");
	m_Jitter    .SetToolTipText("Apply Jitter to image");
	m_GrayScale2.SetToolTipText("Placeholder for xxx");
	m_Edge      .SetToolTipText("Edge detect Image");
	m_Gamma     .SetToolTipText("Apply Gamma");

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CProcDlg::OnButton2() 

{
	RunWorkerFunc(SHARPEN);
}

/////////////////////////////////////////////////////////////////////////////

void CProcDlg::OnButton1() 

{
	RunWorkerFunc(SOFTEN);
}

//////////////////////////////////////////////////////////////////////////

void CProcDlg::Show()

{
	CShadesDlg *shadesdlg = (CShadesDlg* )AfxGetApp()->m_pMainWnd;
	ASSERT(shadesdlg->magic == SHADE_MAGIC);
	
	if(!IsWindow(m_hWnd))
		Create(IDD);

	// If it is already visible, just show
	if(IsWindowVisible())
		{
		SetActiveWindow();	
		return;
		}
	
	if(shadesdlg->current >= 0)
		{
		CXrayDoc *doc = (CXrayDoc*)(shadesdlg->docarr[shadesdlg->current]); 
		
		if(doc)
			{
			P2N("Making copy of the original\r\n");
			ASSERT(doc->magic == DOC_MAGIC);
			doc->org->Copy(*doc->img);
			org_changed = doc->changed;
			}
		}

	CGrayDlg::Show();
}

BOOL CProcDlg::PreTranslateMessage(MSG* pMsg) 

{
#if 0

	if(pMsg->message == WM_CHAR)
		{
		CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);
		SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
		return(0);
		}

	if(pMsg->message == WM_KEYUP)
		{
		CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);
		SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
		return(0);
		}

	if(pMsg->message == WM_SYSKEYDOWN)
		{
		CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);
		SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
		return(0);
		}

	if(pMsg->message == WM_SYSKEYUP)
		{
		CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);
		SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
		return(0);
		}
#endif
	
	return CGrayDlg::PreTranslateMessage(pMsg);
}

//////////////////////////////////////////////////////////////////////////

void CProcDlg::OnButton3() 

{	
	P2N("CProcDlg::OnButton3() Reset to original\r\n");

	CShadesDlg *shadesdlg = (CShadesDlg* )AfxGetApp()->m_pMainWnd;
	ASSERT(shadesdlg->magic == SHADE_MAGIC);

	if(shadesdlg->current < 0)
		{
		P2N("No document in CProcDlg::OnButton3\r\n");
		return;
		}

	CXrayDoc *doc = (CXrayDoc*)(shadesdlg->docarr[shadesdlg->current]); 

	if(doc)
		{
		ASSERT(doc->magic == DOC_MAGIC);

		ShowStatus("Restoring image to previous state ...");	

		doc->render->Copy(*doc->img);
		doc->NewRender();

		//doc->changed = true;
		//HideStatus();
		//support.YieldToWinEx();

		shadesdlg->Invalidate(false);
		support.YieldToWinEx();
		}
}


void CProcDlg::OnOK() 

{	
	if(((CShadesDlg* )parent)->current >= 0)
		{
		// Commit image change
		CXrayDoc *doc = (CXrayDoc*)((CShadesDlg* )parent)->docarr[((CShadesDlg* )parent)->current]; 
		
		ShowStatus("Committing Image changes ...");	
		
		doc->AddUndo();
		
		//HideStatus();
		doc->changed = true;
		((CShadesDlg* )parent)->Invalidate();
		}

	CGrayDlg::OnOK();
}

void CProcDlg::OnCancel() 

{
	OnButton3();

	if(((CShadesDlg* )parent)->current >= 0)
		{
		CXrayDoc *doc = (CXrayDoc*)((CShadesDlg* )parent)->docarr[((CShadesDlg* )parent)->current]; 		
		doc->changed = org_changed;
		}

	CGrayDlg::OnCancel();
}

void CProcDlg::OnButton4() 
{
	RunWorkerFunc(PROC_ERODE);
}

void CProcDlg::OnButton5() 

{
	RunWorkerFunc(PROC_EMBOSS);
}

void CProcDlg::OnButton8() 
{
	RunWorkerFunc(PROC_NEGATIVE);
}

void CProcDlg::OnButton6() 
{
	RunWorkerFunc(PROC_HISTOGRAM_LOG);
}

void CProcDlg::OnButton7() 
{
	RunWorkerFunc(PROC_HISTOGRAM_ROOT);
}

void CProcDlg::OnButton10() 
{
	RunWorkerFunc(PROC_HISTOGRAM_EQUALIZE);
}

void CProcDlg::OnButton11() 
{
	RunWorkerFunc(PROC_HISTOGRAM_NORMALIZE);
}

void CProcDlg::OnButton12() 
{
	RunWorkerFunc(PROC_HISTOGRAM_STRETCH);
}

void CProcDlg::OnButton13() 
{
	RunWorkerFunc(PROC_CIRCLETRANSFORM_PINCH);
}

void CProcDlg::OnButton14() 
{
	RunWorkerFunc(PROC_GRAYSCALE);	
}

void CProcDlg::OnButton15() 
{
	RunWorkerFunc(PROC_DILATE);
}

void CProcDlg::OnButton16() 
{
	RunWorkerFunc(PROC_CONTOUR);
}

void CProcDlg::OnButton17() 
{
	RunWorkerFunc(PROC_JITTER);
}

void CProcDlg::OnButton18() 

{
	RunWorkerFunc(PROC_EDGE);
}

void CProcDlg::OnButton9() 

{
	RunWorkerFunc(PROC_CIRCLETRANSFORM_PUNCH);
}

void CProcDlg::OnSetFocus(CWnd* pOldWnd) 

{
	P2N("CProcDlg::OnSetFocus Setting focus\r\n");	
	CGrayDlg::OnSetFocus(pOldWnd);
}

BOOL CProcDlg::PreCreateWindow(CREATESTRUCT& cs) 

{
	cs.hwndParent = AfxGetApp()->m_pMainWnd->m_hWnd;	
	return CGrayDlg::PreCreateWindow(cs);
}

//////////////////////////////////////////////////////////////////////////

void CProcDlg::RunWorkerFunc(int comm)

{
	CShadesDlg *shadesdlg = (CShadesDlg* )AfxGetApp()->m_pMainWnd;
	ASSERT(shadesdlg->magic == SHADE_MAGIC);

	if(shadesdlg->current < 0)
		{
		ShowStatus("Please load a document before starting this function");
		P2N("Worker function with no document %d\r\n", comm);	
		return;
		}

	CXrayDoc *doc = ((CXrayDoc*)shadesdlg->docarr[shadesdlg->current]);

	// Already running stuff ....
	if(doc->thhandle)
		{
		P2N("CProcDlg Reenter thread prevented\r\n");
		return;
		}
	doc->command  = comm;
	doc->thhandle = _beginthread(RunWorkerThread, 0, doc);

	changed++;

	// Get focus back if applicable
	if(IsWindow(m_hWnd))
		SetFocus();
}

void CProcDlg::OnMButtonDown(UINT nFlags, CPoint point) 

{
	CGrayDlg::OnMButtonDown(nFlags, point);
}

void CProcDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CGrayDlg::OnLButtonDown(nFlags, point);
}

void CProcDlg::PreSubclassWindow() 
{
	CGrayDlg::PreSubclassWindow();
}

void CProcDlg::OnPaint() 

{
	CGrayDlg::OnPaint();

	//CClientDC cdc(this);
	// Test Header
	//cdc.SetBkMode(true);
	//CRect rcc(30, 7, 100, 20);
	//cdc.DrawText("Hello", 5, rcc, DT_LEFT);
}

//////////////////////////////////////////////////////////////////////////

void CProcDlg::MyPaint() 

{
	CPaintDC dc(this); // device context for painting
	
	// Header
	dc.SetBkMode(true);
	CRect rcc(30, 7, 100, 20);
	dc.DrawText("Hello", 5, rcc, DT_LEFT);
}	


void CProcDlg::OnButton21() 

{
	CShadesDlg *shadesdlg = (CShadesDlg* )AfxGetApp()->m_pMainWnd;
	ASSERT(shadesdlg->magic == SHADE_MAGIC);

	if(shadesdlg->current < 0)
		{
		ShowStatus("Please load a document before starting this function");
		return;
		}

	CXrayDoc *doc = ((CXrayDoc*)shadesdlg->docarr[shadesdlg->current]);

	// Already running stuff ....
	if(!doc->thhandle)
		{
		ShowStatus("No processing started, cannot stop it.");
		return;
		}

	doc->img->SetEscape(-1);
	support.YieldToWinEx();
	
	ShowStatus("Aborted processing, restoring image to previous state ...");	

	doc->img->Copy(*doc->org);
	doc->NewRender();
	doc->changed = true;

	Sleep(300);

	shadesdlg->Invalidate();
	support.YieldToWinEx();
}


void CProcDlg::OnButton19() 
{
	RunWorkerFunc(PROC_REPAIR);	
}

void CProcDlg::OnButton20()
 
{
	//RunWorkerFunc(PROC_TEMPLATE);

	CShadesDlg *dlg = (CShadesDlg* )AfxGetApp()->m_pMainWnd;
	ASSERT(dlg->magic == SHADE_MAGIC);

	if(dlg->current < 0)
		{
		ShowStatus("Please load a document before starting this function");
		return;
		}

	CXrayDoc *doc = ((CXrayDoc*)dlg->docarr[dlg->current]);

	dlg->BusyCursor();
	support.YieldToWinEx();

	//doc->render->HistogramClip(2);
	//doc->render->Template();
	//doc->render->Dilate();

	
	doc->render->Circle(200, 
				doc->render->GetHeight() - 300, 100, RGB(200,100,100));

	doc->render->Circle8(440, 
				doc->render->GetHeight() - 300, 100, RGB(200,100,100));

	//////////////////////////////////////////////////////////////////////

	doc->render->DrawCircle(660, 
				doc->render->GetHeight() - 300, 100, RGB(200,100,100));
	
	doc->render->Circle(660, 
				doc->render->GetHeight() - 300, 100, RGB(000, 00, 00));

	doc->render->DrawhLine(100, doc->render->GetHeight() - 150, 300, RGB(000, 00, 00) );

	dlg->Invalidate(false);
	dlg->BusyCursor(false);
}


void CProcDlg::UpdateCursor()
{
	//P2N("CProcDlg::UpdateCursor\r\n");
	
	if(IsWindow(m_hWnd))
		SendMessage(WM_SETCURSOR, HTCLIENT, 0);
}
