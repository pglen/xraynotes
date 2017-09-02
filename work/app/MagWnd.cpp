
/* =====[ MagWnd.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the MagWnd.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  11/18/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// MagWnd.cpp : implementation file
//

#include "stdafx.h"
#include "xraynotes.h"
#include "MagWnd.h"

#include "xScroll.h"
#include "yScroll.h"

extern CxScroll    xscroll;
extern CyScroll    yscroll;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static int in_move = 0;

/////////////////////////////////////////////////////////////////////////////
// CMagWnd dialog

CMagWnd::CMagWnd(CWnd* pParent /*=NULL*/)
	: CGrayDlg(CMagWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMagWnd)
	//}}AFX_DATA_INIT

	//want_x = want_y = 100;
	
	moving = false;
	mag = 2;
	bAlpha = 255;
	lastmove = fired = showtext = 0;
	lastmouse.x = lastmouse.y = 0;
	scantime = 500;

	windowname = "CMagWnd";
}

void CMagWnd::DoDataExchange(CDataExchange* pDX)

{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMagWnd)
	DDX_Control(pDX, IDC_BUTTON6, m_b6);
	DDX_Control(pDX, IDC_BUTTON5, m_b5);
	DDX_Control(pDX, IDC_BUTTON4, m_b4);
	DDX_Control(pDX, IDC_BUTTON3, m_b3);
	DDX_Control(pDX, IDC_BUTTON2, m_b2);
	DDX_Control(pDX, IDC_BUTTON1, m_b1);
	DDX_Control(pDX, IDC_BUTTON7, m_b7);
	DDX_Control(pDX, IDC_BUTTON8, m_b8);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMagWnd, CGrayDlg)
	//{{AFX_MSG_MAP(CMagWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	//ON_WM_NCHITTEST() //TODO
	ON_WM_GETMINMAXINFO()
	ON_WM_WINDOWPOSCHANGED()
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	ON_BN_CLICKED(IDC_BUTTON8, OnButton8)
	ON_BN_CLICKED(IDC_BUTTON9, OnButton9)
	ON_WM_TIMER()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_MAGRCLICK_MAGNIFICATIONFACTOR2, OnMagrclickMagnificationfactor2)
	ON_COMMAND(ID_MAGRCLICK_MAGNIFICATIONFACTOR3, OnMagrclickMagnificationfactor3)
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_MAGRCLICK_MAGNIFICATIONFACTOR4, OnMagrclickMagnificationfactor4)
	ON_COMMAND(ID_MAGRCLICK_MAGNIFICATIONFACTOR6, OnMagrclickMagnificationfactor6)
	ON_COMMAND(ID_MAGRCLICK_MAGNIFICATIONFACTOR8, OnMagrclickMagnificationfactor8)
	ON_COMMAND(ID_MAGRCLICK_NORMALIZEMAGVIEW, OnMagrclickNormalizemagview)
	ON_COMMAND(ID_MAGRCLICK_SHARPENMAGVIEW, OnMagrclickSharpenmagview)
	ON_COMMAND(ID_MAGRCLICK_MAGNIFICATIONFACTOR1, OnMagrclickMagnificationfactor1)
	ON_COMMAND(ID_MAGRCLICK_SHOWHIDEMAGVIEWBUTTONS, OnMagrclickShowhidemagviewbuttons)
	ON_COMMAND(ID_MAGRCLICK_HIDEMAGVIEWWINDOW, OnMagrclickHidemagviewwindow)
	ON_COMMAND(ID_MAGRCLICK_CAPTUREMAGVIEW, OnMagrclickCapturemagview)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMagWnd message handlers

BOOL CMagWnd::OnInitDialog() 
{
	CGrayDlg::OnInitDialog();

	m_title = "Magnifier";

	CWnd *win, *win2; win = win2 = GetNextDlgTabItem(NULL);
	while(true)
		{		
		//P2N("CMagWnd::DialogItem %p\r\n", win2);	
		
		controls.Add(win2);
		win2 = GetNextDlgTabItem(win2);

		// Back to the beginning of list
		if(win2 == win)
			break;
		}

	// Pre Hide them
	for(int loop = 0; loop < controls.GetSize(); loop++)
		{
		((CWnd*)controls[loop])->ShowWindow(SW_HIDE);
		}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMagWnd::OnPaint() 

{
	CPaintDC dc(this); // device context for painting
	//CClientDC dc(this); // device context for painting

	RECT rc; GetClientRect(&rc);

#if 0
	// Get memory image from background
	CDC  dcMem;
	dcMem.CreateCompatibleDC(&dc);

	// CreateCompatibleBitmap does not work on color/memory device!
	HBITMAP bm =::CreateCompatibleBitmap(dc.GetSafeHdc(), rc.right, rc.bottom);
	HBITMAP oldbm = (HBITMAP)::SelectObject(dcMem, bm);

	//P2N("Draw image\r\n");
	img.Draw(dcMem.GetSafeHdc() );

	::StretchBlt(dc.GetSafeHdc(),	
				rc.left, rc.top + titlerect.bottom, 
						rc.right, rc.bottom,
							dcMem, img.GetWidth()/(2*mag), img.GetHeight()/(2*mag), 
								img.GetWidth()/mag, img.GetHeight()/mag, SRCCOPY);

	::SelectObject(dcMem, oldbm);
	::DeleteObject(bm);
	dcMem.DeleteDC();
#endif

	//titlerect.bottom

	img.DrawX(dc.GetSafeHdc(),  0, 0, rc.right, rc.bottom,
							NULL, false, 0, 0);

	// Draw header
	//rc.bottom = titlerect.bottom;
	//dc.FillSolidRect(&rc, ((CShadesApp*)AfxGetApp())->dlgbackcol);

	// Draw corner close	
	dc.FillSolidRect(closerect, RGB(32, 32, 32));

	CPen pen, *oldpen;
	pen.CreatePen(PS_SOLID, 2, RGB(255,255,255));
	oldpen = (CPen*)dc.SelectObject(pen);

	// Draw cross
	dc.MoveTo( closerect.left + 2, closerect.top + 2);
	dc.LineTo( closerect.right - 2, closerect.bottom -2);

	dc.MoveTo( closerect.left + 2, closerect.bottom - 2);
	dc.LineTo( closerect.right - 2, closerect.top + 2);

	CPen pen2; pen2.CreatePen(PS_SOLID, 1, RGB(128,128,128));
	dc.SelectObject(pen2);

	// Draw har cross
	if(moving)
		{
		int loop;
		int hh = rc.bottom - rc.top;
		int ww = rc.right - rc.left;

		dc.MoveTo( ww / 2, hh/4);
		dc.LineTo( ww / 2, 3*hh/4);

		dc.MoveTo( ww/4,  hh/2);
		dc.LineTo( 3*ww/4, hh/2);

		// Draw horiz spikes
		int xx = ww/2, yy = hh/2;
		int xxx = ww/2, yyy = hh/2;
		for(loop = 0; loop < 10; loop++)
			{
			dc.MoveTo( xx,  yy - 2);
			dc.LineTo( xx , yy + 2);
			xx += ww / 40;

			dc.MoveTo( xxx,  yy - 2);
			dc.LineTo( xxx , yy + 2);
			xxx -= ww / 40;
			}

		// Draw vert spikes
		xx = ww/2, yy = hh/2;
		xxx = ww/2, yyy = hh/2;
		for(loop = 0; loop < 10; loop++)
			{
			dc.MoveTo( xx - 2,  yy);
			dc.LineTo( xx + 2 , yy);
			yy += hh / 40;

			dc.MoveTo( xx - 2,  yyy);
			dc.LineTo( xx + 2 , yyy);
			yyy -= hh / 40;
			}
		}

	CFont *pOldFont, *pFont = GetFont();
	ASSERT(pFont);
	pOldFont = dc.SelectObject(pFont);

	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(0,0,0));

	if(invalid)
		{
		CString tmp = "No undelying image";
		dc.DrawText(tmp, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);  
		}

	if(fired < 3)
		{
		RECT xrc;	CopyRect(&xrc, &rc);
		xrc.top = 5; xrc.left = 5;

		CString tmp = "Right click on window for magnification menu";
		dc.DrawText(tmp, &xrc, DT_LEFT);  

		xrc.top += 1; xrc.left += 1;
		dc.SetTextColor(RGB(255,255,255));
		dc.DrawText(tmp, &xrc, DT_LEFT);  	
		}

	// Select old font
	dc.SelectObject(pOldFont);

	//CGrayDlg::OnPaint();
	// Do not call CGrayDlg::OnPaint() for painting messages
}

BOOL CMagWnd::OnEraseBkgnd(CDC* pDC) 

{	
	return true;
}

void CMagWnd::SimMove(int fine, int outside)

{
	CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);
	ASSERT(dlg->magic == SHADE_MAGIC);

	CXrayDoc *doc = dlg->GetCurrDoc();
	if(!doc)
		return;

	lastmove = outside;

	if(in_move)
		return;

	in_move = true;

	  // Get update from scroll bars
    int hscroll = xscroll.m_scroll.GetScrollPos();
    int vscroll = yscroll.m_scroll.GetScrollPos();

	CxImg *src = doc->render;
	
	if(doc->IsDicomFile())
		{
		if(doc->currframe >= 0)
			src = (CxImg *)doc->imgarr[doc->currframe];
		}

	// Calc scroll dimentions
    RECT    disrc;   dlg->GetClientRect(&disrc);

    int xdiff = src->GetWidth() - (disrc.right - disrc.left);
    int ydiff = src->GetHeight() - (disrc.bottom - disrc.top);

    xdiff = MAX(xdiff, 0);   ydiff = MAX(ydiff, 0);

	CRect  rc;	GetClientRect(&rc);
	CRect wrc;	GetWindowRect(&wrc);

	//RECT dwrc;	dlg->GetWindowRect(&dwrc);

	//P2N("CMagWnd::SimMove xpos=%d ypos=%d\r\n", dlg->xpos, dlg->ypos);
	//P2N("CMagWnd::SimMove ww=%d hh=%d\r\n", wrc.Width(), wrc.Height());
	//P2N("CMagWnd::SimMove xdiff=%d ydiff=%d hsb=%d vsb=%d\r\n", 
	//						xdiff, ydiff, hscroll, vscroll);

	// Calc dialog to image position
	wrc.left -= dlg->xpos; 
	wrc.top -= dlg->ypos;

	// Calc to mag pos, start from middle
	int ymid = wrc.top +  rc.Height() / 2;
	int xmid = wrc.left + rc.Width() / 2;

	if(outside)
		{
		// Recenter capture rect 
		xmid = appmouse.x; ymid = appmouse.y;

		xmid -= dlg->xpos; 
		ymid -= dlg->ypos;
		}

	ymid += ydiff * vscroll / 100;
	xmid += xdiff * hscroll / 100;
	
	wrc.top = ymid -  rc.Height() / (2*mag);
	wrc.bottom = ymid +  rc.Height() / (2*mag);

	wrc.left = xmid - rc.Width() / (2*mag);
	wrc.right = xmid + rc.Width() / (2*mag);

	// New image acquired
	//P2N("CMagWnd::SimMove l=%d t=%d r=%d b=%d\r\n", 
	//			wrc.left, wrc.top, wrc.right, wrc.bottom);

	src->Crop(wrc, &img);

	//P2N("CMagWnd::SimMove ww=%d hh=%d\r\n", 
	//			img.GetWidth(), img.GetHeight());

	img.SetTransTresh(255);

	if(!img.IsValid())
		{
		P2N("CMagWnd::SimMove() - Invalid image on resample\r\n");
		invalid = true;
		img.Create(rc.Width(), rc.Height(), 24);
		img.Clear(128);
		}
	else
		{
		img.Resample2(rc.right, rc.bottom, (CxImage::InterpolationMethod)fine);
		}

	Invalidate();
	in_move = false;
}

void CMagWnd::OnLButtonDown(UINT nFlags, CPoint point) 

{
	//P2N("CMagWnd::OnLButtonDown\r\n");
	
	if(!closerect2.PtInRect(point))
		{
		PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));	
		}

	CGrayDlg::OnLButtonDown(nFlags, point);
}

void CMagWnd::OnShowWindow(BOOL bShow, UINT nStatus) 

{
	CGrayDlg::OnShowWindow(bShow, nStatus);

	fired = 0;

	if(bShow)
		{
		SimMove(gl_resamp_rough);	
		SetTimer(2, scantime, NULL);
		}
	else
		{
		KillTimer(2);
		}

	//P2N("CMagWnd::OnShowWindow\r\n");	
}

LRESULT CMagWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 

{
	// Got a message from the main window
	// This does not work well on pretranslatemessage

	if(message == WM_DOCCHANGE)
		{
		//P2N("CMagWnd::WindowProc WM_DOCCHANGE\r\n");
				
		// Only process if needs to
		if(IsWindowVisible())
			{
			CShadesDlg	*dlg;	GETMAINDIALOG(dlg);
			CXrayDoc	*doc;	GETCURRDOC(doc, dlg)
			if(doc)
				{
				if(doc->IsDicomSeries())
					{
					//m_b10.ShowWindow(SW_SHOW);
					//m_b11.ShowWindow(SW_SHOW);
					}
				else
					{
					//m_b10.ShowWindow(SW_HIDE);
					//m_b11.ShowWindow(SW_HIDE);
					}
				}
			SimMove(gl_resamp_rough, lastmove);
			Invalidate();
			SetTimer(1, 500, NULL);
			}
		}
	return CGrayDlg::WindowProc(message, wParam, lParam);
}

void CMagWnd::OnButton1() 

{
	ChangeMag(2);
}

void CMagWnd::OnButton2() 

{
	ChangeMag(3);
}

void CMagWnd::OnButton3() 

{
	ChangeMag(4);
}

void CMagWnd::OnButton4() 

{
	ChangeMag(5);
}

void CMagWnd::OnButton5() 

{
	ChangeMag(6);
}

void CMagWnd::OnButton6() 

{
	ChangeMag(8);
}

UINT CMagWnd::OnNcHitTest(CPoint point) 

{
	CRect  rc;	GetWindowRect(&rc);

	// Fake in a 10x10 box on LR corner
	rc.left = rc.right - 10; rc.top = rc.bottom - 10;

	//P2N(" CMagWnd::OnNcHitTest  x=%d y=%d\r\n", point.x, point.y);

	if(rc.PtInRect(point))
		{		
		return(HTBOTTOMRIGHT);
		}
	return CGrayDlg::OnNcHitTest(point);
}

void CMagWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 

{
	lpMMI->ptMinTrackSize.x = 300;
	lpMMI->ptMinTrackSize.y = 120;
	
	CGrayDlg::OnGetMinMaxInfo(lpMMI);
}


void CMagWnd::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 

{
	CGrayDlg::OnWindowPosChanged(lpwndpos);
	
	//P2N("CMagWnd::OnWindowPosChanged cx=%d cy=%d\r\n", lpwndpos->cx, lpwndpos->cy);
	
	xdrag = GetSystemMetrics(SM_CXDRAG);
	ydrag = GetSystemMetrics(SM_CYDRAG);

	if(	abs(oldpos.x - lpwndpos->x) >	xdrag	||
		abs(oldpos.y - lpwndpos->y) >	ydrag	||
		abs(oldpos.cx - lpwndpos->cx) > xdrag	||
		abs(oldpos.cy - lpwndpos->cy) > ydrag )
		{	
		oldpos = *lpwndpos;
		moving = true;
		SimMove(gl_resamp_rough);
		SetTimer(1, 500, NULL);
		}
}

void CMagWnd::OnButton7() 

{	
	Sharpen();
}

void CMagWnd::OnButton8() 

{
	Norm();
}

//////////////////////////////////////////////////////////////////////////

void CMagWnd::ChangeMag(int mm)

{
	P2N("MagWnd::ChangeMag %d\r\n", mm);

	mag = mm;
	BeginWaitCursor();
	SimMove(gl_resamp_fine);
	EndWaitCursor();
	Invalidate();
}


void CMagWnd::OnButton9() 

{	
	CRect wrc;	GetWindowRect(&wrc);

	CShadesDlg	*dlg;	GETMAINDIALOG(dlg);
	CXrayDoc	*doc;	GETCURRDOCRET(doc, dlg)

	BeginWaitCursor();    
	
	P2N("Fetching original left=%d top=%d \r\n", wrc.left, wrc.top);
	P2N("xpos=%d ypos=%d \r\n", dlg->xpos, dlg->ypos);

	EndWaitCursor();
	Invalidate();
}

BOOL CMagWnd::PreTranslateMessage(MSG* pMsg) 

{
	int processed = false;

	// Moved to winproc ... it does not work here
	//if(pMsg->message == WM_DOCCHANGE)
	//	{
	//	//P2N("CMagWnd::PreTranslateMessage WM_DOCCHANGE\r\n");
	//	}

	if(pMsg->message == WM_KEYDOWN)
		{
		CRect rc; GetWindowRect(&rc);
				
		//P2N("CMagWnd::PreTranslateMessage WM_KEYDOWN wParam=%d\r\n", pMsg->wParam);

		switch(pMsg->wParam)
			{
			case VK_LEFT:
				processed = true;
				if(misc.IsCtrlDown())
					rc.OffsetRect(-50, 0);
				else if(misc.IsShiftDown())
					rc.OffsetRect(-10, 0);
				else
					rc.OffsetRect(-1, 0);
				break;

			case VK_RIGHT: 
				processed = true;
				if(misc.IsCtrlDown())
					rc.OffsetRect(50, 0);
				else if(misc.IsShiftDown())
					rc.OffsetRect(10, 0);
				else
					rc.OffsetRect(1, 0);
				break;

			case VK_UP:
				processed = true;
				if(misc.IsCtrlDown())
					rc.OffsetRect(0, -50);
				else if(misc.IsShiftDown())
					rc.OffsetRect(0, -10);
				else
					rc.OffsetRect(0, -1);
				break;
	
			case VK_DOWN: 
				processed = true;
				if(misc.IsCtrlDown())
					rc.OffsetRect(0, 50);
				else if(misc.IsShiftDown())
					rc.OffsetRect(0, 10);
				else
					rc.OffsetRect(0, 1);
				break;
			}
		if(processed)
			{
			MoveWindow(&rc);
			}
		}

	if(processed)
		return true;

	return CGrayDlg::PreTranslateMessage(pMsg);
}

//////////////////////////////////////////////////////////////////////////

void CMagWnd::OnTimer(UINT nIDEvent) 

{
	switch(nIDEvent)
		{
		case 1:
			{
			fired++;
			KillTimer(nIDEvent);
			moving = invalid = false;
			SimMove(gl_resamp_fine, lastmove);
			}
		break;

		case 2:
			{
			//P2N("CMagWnd::OnTimer (2)\r\n");

			CShadesDlg	*dlg;	GETMAINDIALOG(dlg);
			CRect rc; GetWindowRect(&rc);
			CPoint point(appmouse.x, appmouse.y);

			//P2N("Rect: .left=%d  .top=%d appmouse: .x=%d .y=%d\r\n", 
			//		rc.left, rc.right, appmouse.x, appmouse.y);

			if(GetForegroundWindow() == this)
				{
				//P2N("Foregroundwindow dialog\r\n");

				// See if mouse outside
				if(!rc.PtInRect(point))
					{
					//P2N("Point outside dialog\r\n");

					// Create doc rect
					CRect rc2(dlg->xpos, dlg->ypos, dlg->xpos + 
									dlg->renx, dlg->ypos + dlg->reny);
				
					if(rc2.PtInRect(point))
						{
						//P2N("Point inside image\r\n");
						if(point != lastmouse)
							{
							//P2N("Point inside image and moved\r\n");
							lastmouse = point;
							SimMove(gl_resamp_rough, true);
							SetTimer(1, scantime, NULL);
							}
						}
					}	
				}
			}
		break;
		}

	CGrayDlg::OnTimer(nIDEvent);
}


void CMagWnd::OnRButtonDown(UINT nFlags, CPoint point) 

{
	CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);

	// Right click for menu
	ClientToScreen(&point);

	// Get the menu body
	CXrayM *omenu = new CXrayM; ASSERT(omenu);
	omenu->LoadMenu(IDR_MENU14, 1); 
	omenu->sendto = this;


	// Handle menu states

	CString magstr; magstr.Format("Factor %d", mag);
	int setit = omenu->FindMenuItem(magstr);
	if(setit >= 0)
		omenu->SetCheck(setit, true);

	omenu->Show(point.x, point.y);

	CGrayDlg::OnRButtonDown(nFlags, point);
}

void CMagWnd::OnMagrclickMagnificationfactor2() 
{
	ChangeMag(2);	
}

void CMagWnd::OnMagrclickMagnificationfactor3() 
{
	ChangeMag(3);		
}

void CMagWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	currmouse = point;	
	CGrayDlg::OnMouseMove(nFlags, point);
}

void CMagWnd::OnMagrclickMagnificationfactor4() 
{
	ChangeMag(4);
}

void CMagWnd::OnMagrclickMagnificationfactor6() 
{
	ChangeMag(6);	
}

void CMagWnd::OnMagrclickMagnificationfactor8() 
{
	ChangeMag(8);	
}

void CMagWnd::OnMagrclickNormalizemagview() 
{
	Norm();
}

void CMagWnd::Sharpen()
{
	long kernel[]={-1,-1,-1,-1,15,-1,-1,-1,-1};

	BeginWaitCursor();
    img.Filter(kernel,3,7,0);
	EndWaitCursor();
	Invalidate();
}

void CMagWnd::Norm()
{
	BeginWaitCursor();
    img.HistogramNormalize();	
	EndWaitCursor();
	Invalidate();
}

void CMagWnd::OnMagrclickSharpenmagview() 
{
	Sharpen();	
}

void CMagWnd::OnMagrclickMagnificationfactor1() 
{
	ChangeMag(1);
}


void CMagWnd::OnMagrclickShowhidemagviewbuttons() 

{
	//P2N("CMagWnd::OnMagrclickShowhidemagviewbuttons\r\n");

	// Toggle last command
	//if(lastcmd == SW_HIDE)
	//	lastcmd = SW_SHOW;
	//else
	//	lastcmd = SW_HIDE;

	// This will hide the right click message
	fired = 5;

	// Toggle visibility state
	for(int loop = 0; loop < controls.GetSize(); loop++)
		{
		int cmd = ((CWnd*)controls[loop])->IsWindowVisible();
		
		if(cmd)
			cmd = SW_HIDE;
		else
			cmd = SW_SHOW;
		
		((CWnd*)controls[loop])->ShowWindow(cmd);
		}
	Invalidate();
}

void CMagWnd::OnMagrclickHidemagviewwindow() 
{
	Hide();	
}

void CMagWnd::OnMagrclickCapturemagview() 

{
	CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);

	CXrayDoc *doc = ((CShadesDlg* )AfxGetApp()->m_pMainWnd)->GetCurrDoc();
	if(!doc) return;

	CXrayDoc *ndoc = new CXrayDoc;
	// Create uniq 3D file (for this directory)
	while(true)
		{
		int uniq2 = CXrayDoc::gl_enum++;
		ndoc->filename.Format("%sUntitled_3d_%d.png", tmproot, uniq2);
		if(support.IsFile(ndoc->filename) == 0)
			break;
		}

	// New document is born
	int current =  dlg->docarr.Add(ndoc);

	//dlg->current = current;
	
	P2N("Got Filename %s\r\n", ndoc->filename);

	RECT rc; GetClientRect(&rc);

	ndoc->img->Copy(img);

	//ndoc->img->Create(rc.right - rc.left, rc.bottom - rc.top, 24); 

	Hide();

	dlg->SelDoc(current);
}
