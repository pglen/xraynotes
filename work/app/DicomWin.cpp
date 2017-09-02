
/* =====[ DicomWin.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the DicomWin.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  10/18/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// DicomWin.cpp : implementation file
//

#include "stdafx.h"
#include "xraynotes.h"

#include <process.h>

#include "support.h"
#include "DicomWin.h"
#include "worker.h"
#include "DiMore.h"

#include "do3dp.h"
#include "xyzDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern	CxyzDlg		xyzdlg;

static int in_3d = false;

char *sb2str(int sb)

{
	char *str = "Unknown"; 

	switch(sb)
		{
		case SB_BOTTOM             :    str = "SB_BOTTOM "; break;
		case SB_ENDSCROLL          :    str = "SB_ENDSCROLL "; break;
		case SB_LINEDOWN           :    str = "SB_LINEDOWN  "; break;
		case SB_LINEUP             :    str = "SB_LINEUP    "; break;
		case SB_PAGEDOWN           :    str = "SB_PAGEDOWN  "; break;
		case SB_PAGEUP             :    str = "SB_PAGEUP   "; break;
		case SB_THUMBPOSITION      :    str = "SB_THUMBPOSITION" ; break;
		case SB_THUMBTRACK         :    str = "SB_THUMBTRACK  "; break;
		case SB_TOP                :    str = "SB_TOP  "; break;
		}
	return(str);
}

/////////////////////////////////////////////////////////////////////////////
// CDicomWin dialog


CDicomWin::CDicomWin(CWnd* pParent /*=NULL*/)
	: CGrayDlg(CDicomWin::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDicomWin)
	//}}AFX_DATA_INIT

	delay = 100;
	stop = false;
	created = false;
	//start_base = start_width = 0;
	m_title = "DICOM Controls";
	in_play = false;
	views = 1;
	maxframe = 0;
	oldcurr = 0;

	windowname = "CDicomWin"; //support.GetFileNameExt(__FILE__);
}

void CDicomWin::DoDataExchange(CDataExchange* pDX)
{
	CGrayDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDicomWin)
	DDX_Control(pDX, IDC_BUTTON20, m_b20);
	DDX_Control(pDX, IDC_BUTTON13, m_b13);
	DDX_Control(pDX, IDC_BUTTON22, m_b22);
	DDX_Control(pDX, IDC_BUTTON5, m_b5);
	DDX_Control(pDX, IDC_BUTTON21, m_b21);
	DDX_Control(pDX, IDC_BUTTON23, m_b23);
	DDX_Control(pDX, IDC_BUTTON24, m_b24);
	DDX_Control(pDX, IDC_BUTTON15, m_b15);
	DDX_Control(pDX, IDC_BUTTON14, m_b14);
	DDX_Control(pDX, IDC_BUTTON12, m_b12);
	DDX_Control(pDX, IDC_BUTTON11, m_b11);
	DDX_Control(pDX, IDC_BUTTON16, m_b16);
	DDX_Control(pDX, IDC_BUTTON26, m_b26);
	DDX_Control(pDX, IDC_BUTTON10, m_b10);
	DDX_Control(pDX, IDC_BUTTON25, m_b25);
	DDX_Control(pDX, IDC_BUTTON9, m_b9);
	DDX_Control(pDX, IDC_BUTTON7, m_b7);
	DDX_Control(pDX, IDC_BUTTON35, m_b35);
	DDX_Control(pDX, IDC_BUTTON6, m_b6);
	DDX_Control(pDX, IDC_BUTTON3, m_play);
	DDX_Control(pDX, IDC_BUTTON4, m_stop);
	DDX_Control(pDX, IDC_LAB3, m_lab3);
	DDX_Control(pDX, IDC_LAB5, m_lab5);
	DDX_Control(pDX, IDC_SLIDER5, m_frame);
	DDX_Control(pDX, IDC_LAB4, m_lab4);
	DDX_Control(pDX, IDC_SLIDER3, m_curr);
	DDX_Control(pDX, IDC_BUTTON2, m_b2);
	DDX_Control(pDX, IDC_BUTTON1, m_b1);
	DDX_Control(pDX, IDC_LAB2, m_lab2);
	DDX_Control(pDX, IDC_LAB1, m_lab1);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_can);
	DDX_Control(pDX, IDC_SLIDER2, m_width);
	DDX_Control(pDX, IDC_SLIDER1, m_lev);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDicomWin, CGrayDlg)
	//{{AFX_MSG_MAP(CDicomWin)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEWHEEL()
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_WM_HSCROLL()
	ON_WM_CHAR()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	ON_BN_CLICKED(IDC_BUTTON9, OnButton9)
	ON_BN_CLICKED(IDC_BUTTON10, OnButton10)
	ON_BN_CLICKED(IDC_BUTTON11, OnButton11)
	ON_BN_CLICKED(IDC_BUTTON16, OnButton16)
	ON_BN_CLICKED(IDC_BUTTON12, OnButton12)
	ON_BN_CLICKED(IDC_BUTTON13, OnButton13)
	ON_BN_CLICKED(IDC_BUTTON14, OnButton14)
	ON_BN_CLICKED(IDC_BUTTON15, OnButton15)
	ON_BN_CLICKED(IDC_BUTTON20, OnButton20)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON23, OnButton23)
	ON_BN_CLICKED(IDC_BUTTON24, OnButton24)
	ON_BN_CLICKED(IDC_BUTTON21, OnButton21)
	ON_BN_CLICKED(IDC_BUTTON22, OnButton22)
	ON_COMMAND(ID_DICOM_MOVEZORDERTOWARDSTHEFIRST, OnDicomMovezordertowardsthefirst)
	ON_BN_CLICKED(IDC_BUTTON25, OnButton25)
	ON_BN_CLICKED(IDC_BUTTON26, OnButton26)
	ON_BN_CLICKED(IDC_BUTTON35, OnButton35)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDicomWin message handlers

void CDicomWin::Show()

{
	xCreate();
	ShowWindow(SW_SHOW);

	if(!in_play)
		{
		// Just for gentleness, get curr 
		int oldframe = 0;

		CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);
		CXrayDoc *doc = ((CShadesDlg* )AfxGetApp()->m_pMainWnd)->GetCurrDoc();
		if(doc)
			{
			oldframe = doc->currframe;
			maxframe = doc->imgarr.GetSize();
			m_frame.SetRange(0, maxframe - 1);
			}

		SetCurrFrame(oldframe);
		}

	//UpdateNum();
}

BOOL CDicomWin::OnInitDialog() 

{
	CGrayDlg::OnInitDialog();
	
	m_lev.SetRange(0, 3000);
	m_lev.SetTicFreq(300);			m_lev.SetPageSize(50);
	m_lev.SetPos(0);				m_lev.SetLineSize(5);

	m_width.SetRange(0, 3000);
	m_width.SetTicFreq(300);		m_width.SetPageSize(50);
	m_width.SetPos(0);				m_width.SetLineSize(5);

	m_curr.SetRange(20, 1000);
	m_curr.SetTicFreq(50);			m_curr.SetPageSize(10);
	m_curr.SetPos(delay);			m_curr.SetLineSize(5);

	m_frame.SetRange(0, 20);
	m_frame.SetTicFreq(2);			m_frame.SetPageSize(5);
	m_frame.SetPos(0);				m_frame.SetLineSize(5);

	m_lab1.CenterText();			m_lab1.SetTrans();	
	m_lab2.CenterText();			m_lab2.SetTrans();
	m_lab3.CenterText();			m_lab3.SetTrans();
	m_lab4.CenterText();			m_lab4.SetTrans();
	m_lab5.CenterText();			m_lab5.SetTrans();
	
	UpdateNum();

	m_lab4.SetTextP("Frame delay: %d msec", delay);	
	m_lab3.SetTextP("%d", 1);
	m_lab5.SetTextP("Frame number: %d ", 0);		

	m_b1.SetToolTipText ("Reset Dicom Frame to original Window Width/Height");
	m_b2.SetToolTipText ("Show DICOM file's Original/Current Histogram");
	m_b5.SetToolTipText ("Advance Current Frame by one");
	m_b9.SetToolTipText ("Show 3D rendering with\r\nhighest intensity algorythm");
	m_b10.SetToolTipText("Show 3D rendering with\r\nhighest intensity algorythm,\r\nand reverse chroma sorting");
	m_b23.SetToolTipText("Show all frames on one view");
	m_b25.SetToolTipText("Show Crystal 3D rendering\r\nwith alpha blending algorythm");
	m_b26.SetToolTipText("Show Crystal 3D rendering\r\nwith alpha blending algorythm,\r\and reverse stacking order");
	
	m_b20.SetToolTipText("Move Current Frame backward by one");
	m_b21.SetToolTipText("Got to first frame");
	m_b22.SetToolTipText("Got to last frame");
	m_b35.SetToolTipText("Show More processing options");

	//m_frame.SetFocus();
	//m_frame.Invalidate();
	//m_width.SetFocus();
	//m_lev.SetFocus();
	//support.YieldToWinEx();

	m_ok.SetFocus();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDicomWin::OnPaint() 

{
	//CPaintDC dc(this); // device context for painting
	CGrayDlg::OnPaint();
}

void CDicomWin::OnMouseMove(UINT nFlags, CPoint point) 

{
	CGrayDlg::OnMouseMove(nFlags, point);
}

void CDicomWin::OnLButtonDown(UINT nFlags, CPoint point) 

{
	CGrayDlg::OnLButtonDown(nFlags, point);
}

void CDicomWin::OnLButtonUp(UINT nFlags, CPoint point) 

{
	CGrayDlg::OnLButtonUp(nFlags, point);
}

static int vreent = 0;

void CDicomWin::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 

{
	//P2N("Vertical scroll scode=%d pos=%d ptr=%p %s\r\n", nSBCode, nPos, pScrollBar, sb2str(nSBCode) );
	
	// Get current doc
	CXrayDoc *doc = ((CShadesDlg* )AfxGetApp()->m_pMainWnd)->GetCurrDoc();
	if(!doc)
		return;

	if(!doc->IsDicomFile()) return;

	if(vreent)
		return;

	vreent = true;

	// Do thumbtrack only if in fast mode
	if(nSBCode == SB_ENDSCROLL || 
		(nSBCode == SB_THUMBTRACK && doc->showstate == STATE_FULL) )
		{
		CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);

		int slide1 = ((CSliderCtrl*)GetDlgItem(IDC_SLIDER1))->GetPos();
		int slide2 = ((CSliderCtrl*)GetDlgItem(IDC_SLIDER2))->GetPos();

		UpdateNum();

		dlg->BusyCursor();
		
		((DicomFile*)doc->dcarr[doc->currframe])->wincenter = slide1;
		((DicomFile*)doc->dcarr[doc->currframe])->winwidth  = slide2;

		if(nSBCode == SB_ENDSCROLL)
			{
			doc->oldshowstate = -1;

			((DicomFile*)doc->dcarr[doc->currframe])->changed = true;
			((DicomFile*)doc->dcarr[doc->currframe])->ToImage((CxImg*)
				doc->imgarr[doc->currframe], slide1, slide2);

			if(misc.IsShiftDown())
				{
				for(int loopx = 0; loopx < doc->dcarr.GetSize(); loopx++)
					{
					((DicomFile*)doc->dcarr[loopx])->wincenter = slide1;
					((DicomFile*)doc->dcarr[loopx])->winwidth  = slide2;

					((DicomFile*)doc->dcarr[loopx])->changed = true;
					((DicomFile*)doc->dcarr[loopx])->ToImage((CxImg*)
					doc->imgarr[loopx], slide1, slide2);
					}
				}
			}
		else
			{
			((DicomFile*)doc->dcarr[doc->currframe])->ToImage((CxImg*)
				doc->rendarr[doc->currframe], slide1, slide2);
			}

		dlg->helper->OnDocChange();
		dlg->Invalidate(false);
		dlg->BusyCursor(false);		
		}

	vreent = false;

	CGrayDlg::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CDicomWin::UpdateNum()

{
	int slide1 = ((CSliderCtrl*)GetDlgItem(IDC_SLIDER1))->GetPos();
	int slide2 = ((CSliderCtrl*)GetDlgItem(IDC_SLIDER2))->GetPos();

	m_lab1.SetTextP("%d", slide1);
	m_lab2.SetTextP("%d", slide2);
}

// Reset DICOM to start values

void CDicomWin::OnButton1() 

{
	int		start_base, start_width;

	CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);
	CXrayDoc *doc = ((CShadesDlg* )AfxGetApp()->m_pMainWnd)->GetCurrDoc();
	if(!doc) return;
	if(!doc->IsDicomFile()) return;

	start_base = ((DicomFile*)doc->dcarr[doc->currframe])->start_base;
	start_width = ((DicomFile*)doc->dcarr[doc->currframe])->start_width;

	((CSliderCtrl*)GetDlgItem(IDC_SLIDER1))->SetPos(start_base);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER2))->SetPos(start_width);

	UpdateNum();
	
	((DicomFile*)doc->dcarr[doc->currframe])->wincenter = start_base;
	((DicomFile*)doc->dcarr[doc->currframe])->winwidth  = start_width;

	((DicomFile*)doc->dcarr[doc->currframe])->changed = true;
	((DicomFile*)doc->dcarr[doc->currframe])->ToImage(
		(CxImg*)doc->imgarr[doc->currframe], start_base, start_width);

	doc->oldshowstate = -1;
	dlg->Invalidate(false);
}

void CDicomWin::OnButton2() 

{	
	CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);
	CXrayDoc *doc = ((CShadesDlg* )AfxGetApp()->m_pMainWnd)->GetCurrDoc();

	if(!doc) return;
	if(!doc->IsDicomFile()) return;


	int start, width;

	if(doc->currframe >= 0)
		((DicomFile*) doc->dcarr[doc->currframe])->Histogram(&start, &width, true);

	//doc->dicom->Histogram(&start, &width, true);
}

BOOL CDicomWin::OnEraseBkgnd(CDC* pDC) 

{
	BOOL ret = CGrayDlg::OnEraseBkgnd(pDC);

	//m_width.DrawTransparent(true);
	//m_lev.DrawTransparent(true);
	
	return ret;	
}

BOOL CDicomWin::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 

{
	//P2N("Mouse Wheel %d\r\n", zDelta);
	return CGrayDlg::OnMouseWheel(nFlags, zDelta, pt);
}

void CDicomWin::Hide() 

{
	if(!::IsWindow(m_hWnd))
		return;

	ShowWindow(SW_HIDE);
}

void CDicomWin::UpdateCursor()

{
	//P2N("CDicomWin::UpdateCursor\r\n");
	
	if(IsWindow(m_hWnd))
		SendMessage(WM_SETCURSOR, HTCLIENT, 0);
}

void CDicomWin::OnButton3() 

{
	//m_stop.SetFocus();
	Play(0);	
}


void CDicomWin::OnButton4() 

{
	if(in_play)
		ShowStatus("Stopped DICOM playback ...");

	stop = true;	
	support.YieldToWinEx();
	m_play.SetFocus();
}

static int hreent = 0;

void CDicomWin::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 

{
	//P2N("scrollbar %d scroll3=%d scroll4=%d \r\n", pScrollBar, GetDlgItem(IDC_SLIDER3), GetDlgItem(IDC_SLIDER5));
	
	CXrayDoc *doc = ((CShadesDlg* )AfxGetApp()->m_pMainWnd)->GetCurrDoc();
	if(!doc) return;
	if(!doc->IsDicomFile()) return;

	if(hreent)
		return;

	hreent = true;

	if(nSBCode == SB_ENDSCROLL || nSBCode == SB_THUMBTRACK) //	|| nSBCode == SB_THUMBPOSITION)
		{
		CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);

		// Timer scroll
		if(pScrollBar == GetDlgItem(IDC_SLIDER3))
			{
			delay = ((CSliderCtrl*)GetDlgItem(IDC_SLIDER3))->GetPos();
			
			//P2N("Slider 3 = %d\r\n", delay);
		
			CString tmp; tmp.Format("Frame delay: %d msec", delay);
			m_lab4.SetText(tmp);
			}

		// Image scroll
		if(pScrollBar == GetDlgItem(IDC_SLIDER5))
			{
			int pos = ((CSliderCtrl*)GetDlgItem(IDC_SLIDER5))->GetPos();
			//P2N("Slider 5 = %d\r\n", pos);
		
			CString tmp; tmp.Format("Frame: %d ", pos);
			m_lab5.SetText(tmp);

			stop = true;	
			support.YieldToWinEx();	

			// Set the frame number too:
			SetCurrFrame(pos, nSBCode == SB_ENDSCROLL);
			}
		}

	hreent = false;

	CGrayDlg::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CDicomWin::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	//P2N("Char %c \r\n", nChar);
	
	CGrayDlg::OnChar(nChar, nRepCnt, nFlags);
}

void CDicomWin::OnDestroy() 
{
	CGrayDlg::OnDestroy();
	
	stop = true;
	support.YieldToWinEx();
}

//DEL void CDicomWin::OnShowWindow(BOOL bShow, UINT nStatus) 
//DEL {
//DEL 	CGrayDlg::OnShowWindow(bShow, nStatus);
//DEL 	
//DEL 	//stop = true;
//DEL 	//support.YieldToWinEx();	
//DEL }

void CDicomWin::OnOK() 

{
	//stop = true;	
	//support.YieldToWinEx();	

	CGrayDlg::OnOK();
}

void CDicomWin::OnButton6() 

{
	Play(2);
}

void CDicomWin::OnButton7() 

{
	Play(1);	
}

void CDicomWin::OnCancel() 

{
	//stop = true;	
	//support.YieldToWinEx();	

	CGrayDlg::OnCancel();
}

//////////////////////////////////////////////////////////////////////////

void CDicomWin::SetCurrFrame(int pos, int update)

{
	//P2N("SetCurrFrame: %d\r\n", pos);

	CShadesDlg  *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);
	CXrayDoc    *doc = ((CShadesDlg* )AfxGetApp()->m_pMainWnd)->GetCurrDoc();
	if(!doc) return;

	if(pos == -1)
		pos = doc->imgarr.GetSize() -1;

	UpdateSlider(pos);

	if(pos >= 0 && pos < doc->imgarr.GetSize())
		{
		dlg->showrender = false;
		doc->currframe = pos;

		if(doc->showstate == STATE_MUL)
			{
			if (update)
				{
				doc->oldshowstate = -1;
				dlg->Invalidate();
				}
			}
		else
			dlg->InvalidateSurface();

		((CSliderCtrl*)GetDlgItem(IDC_SLIDER1))->SetPos
			(((DicomFile*)doc->dcarr[doc->currframe])->wincenter);

		((CSliderCtrl*)GetDlgItem(IDC_SLIDER2))->SetPos
			(((DicomFile*)doc->dcarr[doc->currframe])->winwidth);

		UpdateNum();
		}

	dlg->helper->OnDocChange();
}

//////////////////////////////////////////////////////////////////////////

void CDicomWin::OnButton9() 

{	
	if(in_3d)
		return;

	if(in_play)
		return;

	CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);
	CXrayDoc *doc = ((CShadesDlg* )AfxGetApp()->m_pMainWnd)->GetCurrDoc();

	if(!doc) return;
	if(!doc->IsDicomFile())
		{
		xMsgBox("Cannot generate 3D: Not a dicom file / series");
		return;
		}
	if(doc->dcarr.GetSize() < 2) 
		{
		xMsgBox("Cannot generate 3D: Not a dicom series");
		return;
		}

	xyzdlg.m_2d.SetPos(50, 50);;

	in_3d = true;
	Do3Dp(false, doc, NULL, 50, 50);
	in_3d = false;

	xyzdlg.Show();

}

void CDicomWin::OnButton10() 

{
	if(in_3d)
		return;
	
	in_3d = true;
	//Do3Dp(true);
	in_3d = false;
}

void CDicomWin::DoCristal3D(int rev)

{
	CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);

	CXrayDoc *doc = ((CShadesDlg* )AfxGetApp()->m_pMainWnd)->GetCurrDoc();
	if(!doc) return;
	if(!doc->IsDicomFile()) return;

	//P2N("CDicomWin::Do3D\r\n");

	int curr = 0, dcnt = 0, doccnt = doc->imgarr.GetSize();

	// Recalc enabled docs
	for(int loopd = 0; loopd < doccnt; loopd++)
		{	
		if( ((DicomFile*)doc->dcarr[loopd])->enabled)
			dcnt++;
		}
	doccnt = dcnt;

	P2N("Doc count %d\r\n", doccnt);

	if(doccnt < 2)
		{
		MBOXP("Cannot generate 3D Image with less than 2 slices");
		return;
		}

	CXrayDoc *ndoc = new CXrayDoc;
	// Create uniq 3D file (for this directory)
	while(true)
		{
		int uniq2 = CXrayDoc::gl_enum++;
		ndoc->filename.Format("%sUntitled_3d_%d.png", tmproot, uniq2);

		if(support.IsFile(ndoc->filename) == 0)
			break;
		}

	//P2N("Created mp image '%s'\r\n", ndoc->filename);

	ndoc->backlink = doc;
	
	// New document is born
	int current =  dlg->docarr.Add(ndoc);
	dlg->current = current;
	
	//dlg->SelDoc(current);

	RECT rc; dlg->GetClientRect(&rc);

	//P2N("Client rect ww=%d\r\n",  rc.right - rc.left);

	// Fill in image coordinates
	int scrww = rc.right - rc.left;
	int alpha = 255, cnt = 0;
	int imgww = doc->img->GetWidth();
	int imghh = doc->img->GetHeight();
	int dist = 2;

	//50 / doccnt;

	//ndoc->img->Create(doc->img->GetWidth() + doccnt * dist, doc->img->GetHeight()  + doccnt * dist, 24);
	//ndoc->img->Create(rc.right - rc.left, doc->img->GetHeight(), 24);
	//ndoc->img->Create(rc.right - rc.left, rc.bottom - rc.top, 24);

	ndoc->img->Create(imgww + dist * doccnt, imghh + dist * doccnt, 24); 
	ndoc->img->Clear(0);	

	//dlg->Invalidate();

	if(rev)
		curr = doc->imgarr.GetSize();
	else
		curr = -1;

	// How many times can 255 div by fff
	// What divides 255 cnt times
	double ddiv = pow(255, (double)1/(2 * doccnt));
		
	P2N("%g ddiv=%g\r\n", (double)log(255) / log(doccnt), ddiv);

	int part = 255 / doccnt;
	P2N("part=%d\r\n", part);

	while(true)
		{
		cnt++;

		// Back and forth logic
		if(rev)
			curr--;
		else
			curr++;

		if(rev)
			{
			if(curr < 0)
				break;
			}
		else
			{
			if(curr >= doc->imgarr.GetSize())
				break;
			}
		
		// Elect only enabled frames
		if( ((DicomFile*)doc->dcarr[curr])->enabled)
			{
			P2N("Mixing img=%d alpha=%d\r\n", cnt, alpha);

			ShowStatusP("Mixing Frame %d of %d XrayNotes Crystal 3D", cnt, doccnt - 1);

			CxImg timg;	timg.Copy(*((CxImg*)doc->imgarr[curr]) );
			timg.AlphaCreate();	timg.AlphaSet(alpha);

			//timg.Skew(.2, 0, timg.GetHeight()/2, 0);
			//timg.
			//timg.Perspective(timg.GetWidth()/2 /* scrww / doccnt */, timg.GetHeight()/2);

			ndoc->img->Mix(timg, CxImage::OpScreen, - dist * doccnt + cnt * dist, - dist * doccnt + cnt * dist , false);
			
			//ndoc->NewRender();
			//dlg->Invalidate();
			//Sleep(500);

			alpha = int ((double)alpha / ddiv);

			//alpha -= part;

			// Limit the clipping factor of the dividing algorythm
			alpha = max(20, alpha);
			}

		}
		
	// Allow it to do its doc thing
	dlg->helper->OnDocSetFocus();
	
	// Normalize it to look good
	int oldcommit  = ndoc->commit;
	ndoc->commit   = true;
	ndoc->command  = PROC_HISTOGRAM_NORMALIZE;
	ndoc->thhandle = _beginthread(RunWorkerThread, 0, ndoc);

	// This crap does not work ...
	//int retcode = 0;
	//_cwait(&retcode, ndoc->thhandle, NULL);

	// Wait for it ..
	while(true)
		{
		Sleep(100);	support.YieldToWinEx();
		if(!doc->thhandle)
			break;
		}

	ndoc->changed	= true;
	ndoc->commit	= oldcommit;

	HideStatus();
	ndoc->NewRender();
	dlg->Invalidate();
}

#define RGBSET(bg, r,g,b) bg.rgbBlue = b, bg.rgbGreen = g, bg.rgbRed = r;
#define RGBSETBLACK(bg) bg.rgbBlue = 0, bg.rgbGreen = 0, bg.rgbRed = 0;
#define RGBSETWHITE(bg) bg.rgbBlue = 255, bg.rgbGreen = 255, bg.rgbRed = 255;

#define RGBSUM(bg) ((int)bg.rgbBlue + (int)bg.rgbGreen + (int)bg.rgbRed)

//////////////////////////////////////////////////////////////////////////


// Mix new pixel and old pixel in proportion to alpha (0-255)

#define ALPHABLEND(pix, old, alp) (((old * (255 - alp)) >> 8) + ((pix * alp) >> 8))

//////////////////////////////////////////////////////////////////////////

void	CDicomWin::Do3Dpix(int xx, int yy, int rev, CxImg *img)

{
	int putx = 0, puty = 0, putz = 0;

	int imgww = img->GetWidth();
	int imghh = img->GetHeight();

	RGBQUAD maxpix; RGBSET(maxpix,0,0,0);

	if(rev)
		RGBSET(maxpix,255,255,255);

	RGBQUAD pix, prevpix;

	int shift = 2;

	for (int zz = imgs.GetSize() - 1; zz >= 0; zz--)
	//for (int zz = 0; zz < imgs.GetSize(); zz++)
	//for (int zz = 3; zz >= 0; zz--)
		{
		//RGBQUAD pix = ((CxImg*)imgs[zz])->GetPixelColor(xx, yy);

		//int xxx = int((xx - imgww/2) / (factor + (zz * diff)) + imgww/2);//  + zz * 2;
		//int yyy = int((yy - imghh/2) / (factor + (zz * diff)) + imghh/2);//  + zz * 2;

		int xxx = xx;
		int yyy = yy;

		//int xxx = int(xx / (factor + (zz * diff)));
		//int yyy = int(yy / (factor + (zz * diff)));
		
		pix = ((CxImg*)imgs[zz])->GetPixelColorWithOverflow(xxx, yyy, CxImage::OM_COLOR, &bg);

		prevpix = img->GetPixelColorWithOverflow(xx + zz * 2, yy + zz * 2, CxImage::OM_COLOR, &bg);

		//int alpha = 255 - (zz * 255 /imgs.GetSize() );
		int alpha = (zz * 255 /imgs.GetSize() );

		if(xx == 200 && yy  == 200)
			P2N("Alpha %d pix.blue=%d prevpix.blue=%d ", alpha, pix.rgbBlue, prevpix.rgbBlue);
	
		// Alpha blend with current pixel
		pix.rgbBlue  = ALPHABLEND(pix.rgbBlue, prevpix.rgbBlue, alpha);
		pix.rgbGreen = ALPHABLEND(pix.rgbGreen, prevpix.rgbGreen, alpha);
		pix.rgbRed   = ALPHABLEND(pix.rgbRed, prevpix.rgbRed, alpha);

		if(xx == 200 && yy  == 200)
			P2N("pix.blue=%d\r\n", pix.rgbBlue);

		img->SetPixelColor(xx + zz * 2, yy + zz * 2, pix);
	
		//img->SetPixelColor(xx, yy, pix);

		// Find maximum/minimum
		//if(rev)
		//	{
		//	if(RGBSUM(maxpix) > RGBSUM(pix))
		//		{
		//		maxpix = pix;
		//		putx = xx, puty = yy; putz = zz;
		//		}
		//	}
		//else
		//	{
		//	if(RGBSUM(maxpix) < RGBSUM(pix))
		//		{
		//		maxpix = pix;
		//		putx = xx, puty = yy; putz = zz;
		//		}
		//	}

		}

	//putx = xx, puty = yy;

	if(RGBSUM(mask.GetPixelColor(putx, puty)) == 0)
		{
		//mask.SetPixelColor(putx, puty, RGB(255,255,255));

		//img->SetPixelColor(putx, puty, pix);

		//int dim = maxpix.rgbBlue;
		//dim = dim * putz / imgs.GetSize();
		//dim = max(min(255,dim), 0); 

		//maxpix.rgbBlue = maxpix.rgbGreen = maxpix.rgbRed = dim;

		//maxpix.rgbReserved = 255 - (putz * 120 / imgs.GetSize() + 100);
		//maxpix.rgbReserved = 255;
		
		//img->SetPixelColor(putx, puty, maxpix);
		}
}


//////////////////////////////////////////////////////////////////////////

void CDicomWin::OnButton11() 

{
	//P2N("Add 2\r\n");
	SetNumViews(2);
}

void CDicomWin::OnButton16() 

{
	//P2N("Add 1\r\n");	
	SetNumViews(1);
}


void CDicomWin::SetNumViews(int num)

{
	CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);

	// Stop currently playing movie
	stop = true; support.YieldToWinEx();	
	
	// Get current doc
	CXrayDoc *doc = dlg->GetCurrDoc();

	if(!doc)
		return;
	
	// Is it dicom file?
	if(doc->dcarr.GetSize() == 0)
		{
		xMsgBox("Not a dicom file");
		return;
		}
	// Is it dicom series?
	if(doc->dcarr.GetSize() == 1)
		{
		xMsgBox("Not a dicom series.");
		return;
		}

	//SetCurrFrame(0);

	if(num == -1)
		doc->num_to_show = doc->dcarr.GetSize();
	else
		doc->num_to_show = min(num, doc->dcarr.GetSize());

	m_lab3.SetTextP("%d", doc->num_to_show);

	// Set show state
	if(num == 1)
		doc->showstate = STATE_FULL;
	else
		doc->showstate = STATE_MUL;
	
	//views = doc->dcarr.GetSize();

	dlg->Invalidate();
}

void CDicomWin::OnButton12() 
{
	//P2N("Add 4\r\n");	
	SetNumViews(4);	
}

void CDicomWin::OnButton13() 
{
	SetNumViews(6);		
}

void CDicomWin::OnButton14() 
{
	SetNumViews(8);		
}

void CDicomWin::OnButton15() 
{
	SetNumViews(12);
}

void CDicomWin::UpdateSlider(int curr)

{
	int maxframes = 0;
	CXrayDoc *doc = ((CShadesDlg* )AfxGetApp()->m_pMainWnd)->GetCurrDoc();

	if(!doc)
		return;

	if(!doc->IsDicomFile())
		return;

	if(doc) 
		maxframes = doc->imgarr.GetSize() - 1;

	oldcurr = curr;

	CString onx("On");
		
	if(!((DicomFile*)doc->dcarr[curr])->enabled )
		{
		onx = "Off";
		}

	CString str; str.Format("Frame: %d of %d (%s)", 
									curr, maxframes, onx);
	m_lab5.SetText(str); 
	m_frame.SetPos(curr);
}

void CDicomWin::OnButton20() 

{
	// Right jump
	int pos = ((CSliderCtrl*)GetDlgItem(IDC_SLIDER5))->GetPos();
	
	stop = true; support.YieldToWinEx();	

	if(pos < ((CSliderCtrl*)GetDlgItem(IDC_SLIDER5))->GetRangeMax())
		{
		pos++;
		SetCurrFrame(pos);
		}
}

void CDicomWin::OnButton5() 

{
	// Left Jump	
	int pos = ((CSliderCtrl*)GetDlgItem(IDC_SLIDER5))->GetPos();
	stop = true; support.YieldToWinEx();	
	if(pos > 0)
		{
		pos--;
		SetCurrFrame(pos);
		}
}

void	CDicomWin::Play(int rev)

{
	CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);
	CXrayDoc *doc = ((CShadesDlg* )AfxGetApp()->m_pMainWnd)->GetCurrDoc();

	if(!doc) return;
	if(!doc->IsDicomFile()) return;

	if(doc->num_to_show != 1)
		SetNumViews(1);

	//P2N("play %d\r\n", doc->uniq);

	m_stop.SetFocus();

	// Stop Movie
	stop = true; 
	support.YieldToWinEx();	

	int dir = true;
	stop = false;
	HideStatus();

	// Moved to class level for doc switch
	maxframe = doc->imgarr.GetSize() - 1;

	// Calc enabled docs
	int dcnt = 0;
	for(int loopd = 0; loopd <= maxframe; loopd++)
		{	
		if( ((DicomFile*)doc->dcarr[loopd])->enabled)
			dcnt++;
		}
	
	if(dcnt < 2)
		{
		xMsgBox("Cannot play movie with less than two enabled frames.");
		return;
		}

	// Start where it left off
	int curr = doc->currframe;
	
	in_play = true;
	while(true)
		{
		if(stop)
			break;

		//UpdateSlider(curr);

		if( ((DicomFile*)doc->dcarr[curr])->enabled)
			{
			SetCurrFrame(curr);
		
			dlg->InvalidateSurface();
			
			if(stop)
				break;		

			support.YieldToWinEx();
			Sleep(delay);
			support.YieldToWinEx();
			}

		if(rev == 2)
			{
			if(dir)
				{
				curr++;
				if(curr >= maxframe )
					dir = !dir;
				}
			else
				{
				curr--;
				if(curr <= 0)
					dir = !dir;
				}
			}
		else if(rev == 1)
			{
			curr--;

			if(curr < 0)
				curr = maxframe ;
			}
		else
			{
			curr++;

			if(curr > maxframe )
				curr = 0;
			}
		}

	in_play = false;
	
	// Mark where it left off
	doc->currframe = curr;
}

void CDicomWin::OnButton23() 

{
	SetNumViews(-1);		
}

void CDicomWin::OnButton24() 

{
	SetNumViews(16);	
}

void CDicomWin::OnButton21() 

{
	SetCurrFrame(0);	
}

void CDicomWin::OnButton22() 

{
	SetCurrFrame(-1);	
}

void CDicomWin::OnDicomMovezordertowardsthefirst() 

{
	P2N("OnDicomMovezordertowardsthefirst\r\n");

	CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);
	CXrayDoc *doc = ((CShadesDlg* )AfxGetApp()->m_pMainWnd)->GetCurrDoc();
	if(!doc) return;
	if(!doc->IsDicomFile()) return;
	
	//////////////////////////////////////////////////////////////////////
	if(!doc->IsDicomSeries()) 
		{
		P2N("Not a DICOM Series\r\n");
		xMsgBox("Not a DICOM Series");
		return;
		}
}

void CDicomWin::OnButton25() 

{
	if(in_3d)
		return;

	in_3d = true;
	DoCristal3D(false);
	in_3d = false;

}

void CDicomWin::OnButton26() 

{
	if(in_3d)
		return;

	in_3d = true;
	DoCristal3D(true);
	in_3d = false;	
}


void CDicomWin::OnButton35() 

{
	stop = true;
	support.YieldToWinEx();

	CDiMore dm;

	dm.SetCenterFlag(true);
	dm.DoModal();

	UpdateSlider(oldcurr);
}
