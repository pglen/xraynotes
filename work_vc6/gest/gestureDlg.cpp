
/* =====[ gestureDlg.cpp ]========================================== 
                                                                             
   Description:     The gest project, implementation of the gestureDlg.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  3/25/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// MagDlg.cpp : implementation file
//

#include "stdafx.h"

#include <math.h>
#include <stdio.h>

#include "gestureDlg.h"
#include "FileDialogST.h"
#include "mxpad.h"
#include "support.h"
#include "GetText.h"
#include "xfile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define NORMALIZE_TO	500		// Normalize stroke list to this many entries
#define ENTRYPERLINE	20
#define	DIST_MULTI		50		// Distance multiplier
#define	STROKE_PAUSE	500		// Msecs to change letter

#define MINSIZE		215
#define MAXSIZE		470

char	*gfhead = "Gesture File 1.00";

typedef	struct _coms

{
	char	*str;
	void   (*func)(void*);
} coms;

//////////////////////////////////////////////////////////////////////////

coms	varcoms[] = 

{
#if 0
	"Open File",		OpenFileCb,
	"Prev Document",	NULL,
	"Next Document",	NULL,
	"Size to Width",	NULL,
	"Size to Height",	NULL,
	"Close File",		NULL,
	"Load Last Doc",	NULL,
	"Exit",				NULL,
	"Brightness",		NULL,

#endif

	NULL, NULL

};

/////////////////////////////////////////////////////////////////////////////
// CGestureDlg dialog

CGestureDlg::CGestureDlg(CWnd* pParent /*=NULL*/)
	: CGrayDlg(CGestureDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGestureDlg)
	//}}AFX_DATA_INIT

	in_draw = false;
	want_x = 0, want_y = 0; created = false;
	dark = 0;

	more = true;
	push = false;
	changed = false;
	newtrain = false;
	lastmdown = lastmup = clock();

}

void CGestureDlg::DoDataExchange(CDataExchange* pDX)

{
	CGrayDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGestureDlg)
	DDX_Control(pDX, IDC_BUTTON7, m_b7);
	DDX_Control(pDX, IDC_BUTTON15, m_b15);
	DDX_Control(pDX, IDC_EDIT2, m_edit);
	DDX_Control(pDX, IDC_BUTTON1, m_butt1);
	DDX_Control(pDX, IDC_BUTTON2 ,   m_Add		);	
	DDX_Control(pDX, IDC_BUTTON3 ,   m_Delete	);	
	DDX_Control(pDX, IDC_BUTTON4 ,   m_Load		);	
	DDX_Control(pDX, IDC_BUTTON6 ,   m_Save		);	
	DDX_Control(pDX, IDC_BUTTON8 ,   m_Dump		);	
	DDX_Control(pDX, IDC_BUTTON9 ,   m_Play		);	
	DDX_Control(pDX, IDC_BUTTON10,   m_Place	);	
	DDX_Control(pDX, IDC_BUTTON11,   m_ClearLast);	
	DDX_Control(pDX, IDC_BUTTON5 ,   m_Copy		);	
	DDX_Control(pDX, IDC_BUTTON12,   m_Paste	);	
	DDX_Control(pDX, IDC_BUTTON13,   m_Clear2	);	
	DDX_Control(pDX, IDC_BUTTON14,   m_Recall	);	
	DDX_Control(pDX, IDC_PROG, m_prog);
	DDX_Control(pDX, IDC_LIST1, m_list);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGestureDlg, CGrayDlg)
	//{{AFX_MSG_MAP(CGestureDlg)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_SYSCHAR()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_WM_LBUTTONDBLCLK()
	ON_LBN_DBLCLK(IDC_LIST1, OnDblclkList1)
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	ON_BN_CLICKED(IDC_BUTTON8, OnButton8)
	ON_BN_CLICKED(IDC_BUTTON9, OnButton9)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON11, OnButton11)
	ON_BN_CLICKED(IDC_BUTTON15, OnButton15)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGestureDlg message handlers

void CGestureDlg::Show()

{
	if(!created)
		{
		Create(IDD);

		if(pre_want_x)	want_x = pre_want_x;
		if(pre_want_y)	want_y = pre_want_y;

		SetWindowPos( NULL, want_x, want_y, 0, 0, SWP_NOSIZE | SWP_NOZORDER );  	
		}

	created = true;

	// Show based upon loaded settings
	if(pre_showstate)
		{
		pre_showstate = false;
		}
	else
		{
		ShowWindow(SW_SHOW);
		}
}

///////////////////////////////////////////////////////////////////////////

BOOL CGestureDlg::OnInitDialog() 

{
	CGrayDlg::OnInitDialog();
		
	CenterWindow();

	BYTE	bAlpha = 200;

#if 0
	if (bAlpha < 255)
		{
        //  Set layered style for the dialog
        SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);

        //  Call it with 255 as alpha - opacity
		if(g_pSetLayeredWindowAttributes)
			g_pSetLayeredWindowAttributes(m_hWnd, 0, bAlpha, LWA_ALPHA);
		}
#endif

	//m_list.AddString("Prev Document");
	//m_list.AddString("Next Document");
	//m_list.AddString("Size to Width");
	//m_list.AddString("Size to Height");
	//m_list.AddString("Open file");

	// Load last gesture
	//CString fname;
	//fname = appdata + "default.gesture2";
	//LoadSave(true, fname);
		
	m_prog.SetTrans(true);
	
	//WINDOWPLACEMENT wp;
	//GetWindowPlacement(&wp);
	//wp.rcNormalPosition.right = 215;
	//SetWindowPlacement(&wp);

	OnButton1();

	( (CButton*)GetDlgItem(IDC_RADIO2) )->SetCheck(true) ;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

///////////////////////////////////////////////////////////////////////////

void CGestureDlg::OnRButtonDown(UINT nFlags, CPoint point) 

{
#if 0
	xarr.RemoveAll(); yarr.RemoveAll();
	
	in_draw = true;
	
	//P2N("Rigth button down\r\n");

	TRACKMOUSEEVENT te;

	te.cbSize = sizeof(TRACKMOUSEEVENT);
	te.dwFlags = TME_LEAVE;
	te.hwndTrack = this->m_hWnd;
	te.dwHoverTime = 0;

	::_TrackMouseEvent(&te);

	old_x = point.x; old_y = point.y;
#endif

	CGrayDlg::OnRButtonDown(nFlags, point);
}

void CGestureDlg::OnRButtonUp(UINT nFlags, CPoint point) 

{

#if 0
	TRACKMOUSEEVENT te;

	te.cbSize = sizeof(TRACKMOUSEEVENT);
	te.dwFlags = TME_CANCEL;
	te.hwndTrack = this->m_hWnd;
	te.dwHoverTime = 0;

	::_TrackMouseEvent(&te);

	//P2N("rbuttom up\r\n");
	in_draw = false;
	

	//Invalidate();
	InvalDrawing();

	SetTimer(2, 100, NULL);
	SetTimer(1, 600, NULL);

#endif

	CGrayDlg::OnRButtonDown(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////

void CGestureDlg::OnLButtonUp(UINT nFlags, CPoint point) 

{
	drag_x = point.x; drag_y = point.y;

	//P2N("rbuttom up\r\n");
	
	lastmup = clock();

	TRACKMOUSEEVENT te;

	te.cbSize = sizeof(TRACKMOUSEEVENT);
	te.dwFlags = TME_CANCEL;
	te.hwndTrack = this->m_hWnd;
	te.dwHoverTime = 0;

	::_TrackMouseEvent(&te);

	//focus = false;
	if(drag)
		{
		drag = 0; 
		ReleaseCapture();
		InvalidateRect(NULL, TRUE);
		}
	else
		{
		in_draw = false;
		
		//Invalidate();
		//InvalDrawing();

		SetTimer(2, STROKE_PAUSE, NULL);
		}

	CGrayDlg::OnLButtonUp(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////

void CGestureDlg::OnLButtonDown(UINT nFlags, CPoint point) 

{
	drag_x = point.x; drag_y = point.y;

	lastmdown = clock();

	P2N("CGestureDlg::OnLButtonDown xx=%d yy=%d\r\n", point.x, point.y);

	// If less than letter time space, continue this letter
	int diff = lastmdown - lastmup;
	P2N("diff lastmouse time %d\r\n", diff);

	RECT rc; GetClientRect(&rc);
	CRect rect(rc.right - 17, 2, rc.right - 4, 16);
	CRect rect2(rc.left, rc.top, rc.right, 12);

	if(rect.PtInRect(point))
		{
		P2N("Click in close box\r\n");
		EndDialog(IDOK);
		//ShowWindow(SW_HIDE);
		goto endd;
		//return;
		}

	if(rect2.PtInRect(point))
		{
		P2N("Click in header box\r\n");
		PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));	
		goto endd;
		//return;
		}

	TRACKMOUSEEVENT te;

	te.cbSize = sizeof(TRACKMOUSEEVENT);
	te.dwFlags = TME_LEAVE;
	te.hwndTrack = this->m_hWnd;
	te.dwHoverTime = 0;

	::_TrackMouseEvent(&te);

	if(diff < STROKE_PAUSE)
		{
		KillTimer(2);
		marr.Add(0);
		}
	else
		{
		// Start fresh
		xarr.RemoveAll();
		yarr.RemoveAll();	
		marr.RemoveAll();

		Invalidate();

		//Drawing();
		marr.Add(1);
		}

	// Insert into point array
	xarr.Add((int)point.x);
	yarr.Add((int)point.y);

endd:
	CGrayDlg::OnLButtonDown(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////

void CGestureDlg::OnMouseMove(UINT nFlags, CPoint point) 

{
	CPoint point2 = point;	ClientToScreen( &point2 ); 

	RECT wrc; GetWindowRect(&wrc);
	
	CRect rc(wrc);
	CRect head(2, 2, wrc.right - 4, 16);

	// In header?
	if(head.PtInRect(point))
		{
		// Button?
		if(nFlags & MK_LBUTTON)
			{		
			// Moved?
			if(drag_x != point.x || drag_y != point.y)
				{
				// Is drag treshold?
				if((abs(drag_x - point.x)  > 3 ) || (abs(drag_y - point.y) > 3) )
					{
					//P2N("Moved\r\n");
					drag_x = point.x;
					drag_y = point.y;

					SetCapture();
					drag = 1;
					}
				}
			}
		}
	else	// Not in header
		{
		// Button?
		if( /*( nFlags & MK_RBUTTON) ||*/ (nFlags & MK_LBUTTON))
			{
			if(!drag)
				{
				//P2N("Adding mouse gesture x=%d y=%d\r\n", old_x - point.x, old_y - point.y);
				
				// Previous point
				int xsizediff = 4, ysizediff =  4;

				// Calculate invalidate region
				if(xarr.GetSize() > 0)
					{
					xsizediff = ABS((int)xarr.ElementAt(xarr.GetUpperBound()) - point.x) + 1;
					ysizediff = ABS((int)yarr.ElementAt(yarr.GetUpperBound()) - point.y) + 1;
					//P2N("xdiff=%d ydiff=%d\r\n", xsizediff, ysizediff);
					}
 		
				RECT rc;
				rc.left		= point.x - xsizediff;
				rc.right	= point.x + xsizediff;
				rc.top		= point.y - ysizediff;
				rc.bottom	= point.y + ysizediff;

				InvalidateRect(&rc);

				// Insert into point array
				xarr.Add((int)point.x);
				yarr.Add((int)point.y);
				marr.Add(1);
				}
			}
		}

	if(nFlags & MK_LBUTTON)
		{
		if(!drag)
			{
			//ReleaseCapture();
			}
		else
			{
			// Actual drag
			GetWindowRect(&rc);
			SetWindowPos( NULL, (point.x +  rc.left) - drag_x , 
							(point.y + rc.top) - drag_y, 0, 0, 
									SWP_NOSIZE | SWP_NOZORDER ) ;  
			}		
		}

	CGrayDlg::OnMouseMove(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////

void CGestureDlg::OnSysChar(UINT nChar, UINT nRepCnt, UINT nFlags) 

{
	P2N("Mag Syschar %c %d\r\n", nChar, nChar);	
	CGrayDlg::OnSysChar(nChar, nRepCnt, nFlags);
}


BOOL CGestureDlg::PreTranslateMessage(MSG* pMsg) 
{

#if 0
	if(pMsg->message == WM_KEYDOWN)
		{
		((CShadesDlg* )parent)->SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
		P2N("CGestureDlg pretrans keydown %c (%d) %x\r\n", pMsg->wParam, pMsg->wParam, pMsg->lParam);
		return(0);
		}

	if(pMsg->message == WM_KEYUP)
		{
		((CShadesDlg* )parent)->SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
		//P2N("mag pretrans keyup %c (%d) %x\r\n", pMsg->wParam, pMsg->wParam, pMsg->lParam);
		return(0);
		}
#endif

	// Stop escape's effect
	if(pMsg->message == WM_KEYDOWN)
		{
		//P2N("CGestureDlg::PreTranslateMessage keydown %c (%d) %x\r\n", pMsg->wParam, pMsg->wParam, pMsg->lParam);
		if(pMsg->wParam == 27)
			return(0);
		}

	//if(pMsg->message == WM_CHAR)
	//	{
	//	//((CShadesDlg* )parent)->SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
	//	P2N("CGestureDlg::PreTranslateMessage Char %c\r\n", pMsg->wParam);
	//	//return(0);
	//	}

#if 0
	
	if(pMsg->message == WM_SYSKEYDOWN)
		{
		((CShadesDlg* )parent)->SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
		//P2N("mag pretrans syskeydown %c (%d) %x\r\n", pMsg->wParam, pMsg->wParam, pMsg->lParam);
		//return(0);
		}

	if(pMsg->message == WM_SYSKEYUP)
		{
		((CShadesDlg* )parent)->SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
		//P2N("mag pretrans syskeyup %c (%d) %x\r\n", pMsg->wParam, pMsg->wParam, pMsg->lParam);
		//return(0);
		}

	if(pMsg->message == WM_MOUSELEAVE)
		{
		//((CShadesDlg* )parent)->SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
		P2N("mag mouseleave wParam=%d lParam=0x%x\r\n", pMsg->wParam, pMsg->lParam);

		//CPoint pt;
		//CGestureDlg::OnLButtonUp(0, pt); 
		//return(0);
		}
#endif

	return CGrayDlg::PreTranslateMessage(pMsg);
}

//////////////////////////////////////////////////////////////////////////

void CGestureDlg::OnPaint() 

{
	//P2N("Paint gesture\r\n");

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
		//dc.DrawIcon(x, y, m_hIcon);

		return;
		}
	else
		{
		CClientDC dc(this); // device context for painting

#if 0
		RECT rc; GetClientRect(&rc);

		// Draw header
		dc.FillSolidRect(2, 2, rc.right - 4, 17, RGB(64, 64, 64));

		// Draw corner close
		dc.FillSolidRect(rc.right - 17, 2, rc.right - 4, 16, RGB(32, 32, 32));

		CPen pen; pen.CreatePen(PS_SOLID, 2, RGB(255,255,255));
		CPen *oldpen;

		oldpen = (CPen*)dc.SelectObject(pen);
	
		dc.MoveTo( rc.right - 14, 14);
		dc.LineTo( rc.right - 4, 4);

		dc.MoveTo( rc.right - 14, 4);
		dc.LineTo( rc.right - 4, 14);

		//dc.SelectObject(oldpen);

		CPen pen2; pen2.CreatePen(PS_SOLID, 1, RGB(0,0,0));
		dc.SelectObject(pen2);

#endif
		///////////////////////////////////////////////////////////////////
		int loop =	0; 
		int len = xarr.GetSize();
		if(len)
			{
			dc.MoveTo(xarr[0], yarr[0]);

			for(loop = 0; loop < len; loop++)
				{
				if(marr[loop])
					dc.LineTo(xarr[loop], yarr[loop]);
				else
					dc.MoveTo(xarr[loop], yarr[loop]);
				}
			}
		else
			{
			
			}

		//dc.SelectObject(oldpen);
		}
 
	// Paint background and controls
	CGrayDlg::OnPaint();
}

/////////////////////////////////////////////////////////////////////////

void CGestureDlg::OnTimer(UINT nIDEvent) 

{
	//P2N("CGestureDlg::OnTimer fired\r\n");

	switch(nIDEvent)
		{
		case 1:
			{
			KillTimer(nIDEvent);
			if(!in_draw)
				{
				InvalDrawing();

				xarr.RemoveAll();
				yarr.RemoveAll();
				marr.RemoveAll();

				normalarr.RemoveAll();
				m_prog.SetText("");		
				}
			break;
			}

		case 2:
			{
			KillTimer(nIDEvent);

			SetTimer(1, STROKE_PAUSE, NULL);
			Recognize();
			}
			break;
	
		default:
			P2N("CGestureDlg::OnTimer Invalid Case\r\n");
			break;
		}
				
	CGrayDlg::OnTimer(nIDEvent);
}


void CGestureDlg::OnOK() 

{
	CGrayDlg::OnOK();
}

//////////////////////////////////////////////////////////////////////////

void CGestureDlg::OnButton1() 

{
	//P2N("More button\r\n");

	int	xwidth;

	if(more)
		{
		m_butt1.SetWindowText("More >>");
		xwidth = MINSIZE;
		more = false;
		}
	else
		{
		m_butt1.SetWindowText("<< Less");
		xwidth = MAXSIZE;
		more = true;

		// As a safety measure, switch to recognize mode
		( (CButton*)GetDlgItem(IDC_RADIO2) )->SetCheck(true);
		( (CButton*)GetDlgItem(IDC_RADIO1) )->SetCheck(false);
		( (CButton*)GetDlgItem(IDC_RADIO3) )->SetCheck(false);
		}
	
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);
	wp.rcNormalPosition.right = wp.rcNormalPosition.left + xwidth;
	SetWindowPlacement(&wp);
}


void CGestureDlg::OnButton2() 

{
	CGetText gt; 

	if(gt.DoModal() != IDOK)
		return;

	int idx = m_list.AddString(gt.m_edit);

	if(idx >= 0)
		m_list.SetCurSel(idx);
}

void CGestureDlg::OnButton3() 

{
	CString	str;
	int		idx = m_list.GetCurSel();

	if(idx < 0)
		{
		AfxMessageBox("Please select an item to delete."); //, "Cannot Delete");
		return;
		}

	// CListBox
	m_list.GetText(idx, str);
	P2N("Removing %s\r\n", str);

	int len = traindata.GetUpperBound();
	int count = 0;
	for(int zloop = len; zloop >=0; zloop--)
		{
		tdata *ptr = (tdata*)traindata[zloop];

		if(ptr->str == str)
			{
			//P2N("Deleting %d %s\r\n", zloop, str);
			traindata.RemoveAt(zloop);
			}
		}	
	m_list.DeleteString(idx);
}

int CGestureDlg::RecFuzzy(CDWordArray *arr) 

{
	int len = traindata.GetSize();
	int idx = 0;
	int minerr = 0x8000000;

	//P2N("Fuzzy Compare %d items\r\n", len);

	// Dump train data
	for(int zloop = 0; zloop < len; zloop++)
		{
		int err = 0;
		int ilen = ((tdata*)traindata[zloop])->arr.GetSize();

		for(int iloop = 0; iloop < ilen; iloop++)
			{
			//P2N("%3d  ", ((tdata*)traindata[zloop])->arr[iloop]);
			err += abs(((tdata*)traindata[zloop])->arr[iloop] - arr->GetAt(iloop));
			}

		err /= ilen;

		if(abs(err) < abs(minerr))
			{
			P2N("Candidate: %s %d\r\n", ((tdata*)traindata[zloop])->str, err);

			minerr = err;
			idx  = zloop;
			}
		}

	TRACKMOUSEEVENT te;

	te.cbSize = sizeof(TRACKMOUSEEVENT);
	te.dwFlags = TME_CANCEL;
	te.hwndTrack = this->m_hWnd;
	te.dwHoverTime = 0;

	::_TrackMouseEvent(&te);

	if(minerr < 70)
		{
		P2N("Possible match: %s \r\n", ((tdata*)traindata[idx])->str);
		CString str;
		str.Format("%s", ((tdata*)traindata[idx])->str);
		m_prog.SetText(str);
		support.YieldToWinEx();

		int tchecked = ( (CButton*)GetDlgItem(IDC_RADIO3) )->GetState();
		tchecked = tchecked & 0x03 ? true : false;

		if(!tchecked)
			{
			CString str = ((tdata*)traindata[idx])->str;
			
			P2N("Exec %s\r\n", str);
			ExecGesture(str);
			}
		}
	else
		{
		m_prog.SetText("No Match");
		}

	return 0;
}

//////////////////////////////////////////////////////////////////////////

void CGestureDlg::InvalDrawing()

{
	// Calculate boundaries of drawing ...
	RECT rc; 

	int len = xarr.GetSize();
	if(len)
		{
		rc.bottom = rc.top = xarr[0];
		rc.left = rc.right = yarr[0];

		for(int loop = 0; loop < len; loop++)
			{
			rc.bottom	= MAX(rc.bottom, (int)yarr[loop]);
			rc.top		= MIN(rc.top, (int)yarr[loop]);

			rc.right	= MAX(rc.right, (int)xarr[loop]);
			rc.left		= MIN(rc.left, (int)xarr[loop]);												
			}

		// Just to make sure it is in ....
		rc.bottom += 2;	rc.top -= 2;	
		rc.left -= 2;  rc.right += 2;

		InvalidateRect(&rc);		
		}
}

void CGestureDlg::OnButton6() 

{
	CString str;
	LoadSave(false, str);		
}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
// Function name:    CGestureDlg::LoadSave
// Description:      <empty description>
// Return type:      int 
// Argument:         int loadflag		true for load
// Argument:         CString &fname		pass empty name for prompt
//
//////////////////////////////////////////////////////////////////////////

int CGestureDlg::LoadSave(int loadflag, CString &fname)

{
	CString fileName;

	P2N("Gesture Open/Save '%s'\r\n", fname);

	// If no file name passed, ASK
	if(fname == "")
		{
		BOOL bRet = 0;

		//MBOX("Open");
		int	bOpenFileDialog = loadflag;
		int	dwFlags = 0;
		int *pType = NULL;

		//CFileDialog dlgFile(bOpenFileDialog);
		CFileDialogST dlgFile(bOpenFileDialog);

		CString title;
		
		if (bOpenFileDialog) 
			title = "Open Gesture File"; 
		else 
			title = "Save Gesture File";

		dlgFile.m_ofn.Flags |= dwFlags | OFN_EXPLORER ; //OFN_ALLOWMULTISELECT | ;

		int nDocType = (pType != NULL) ? *pType : 0;
		int nIndex = 0;

		dlgFile.m_ofn.nFilterIndex = 0; //nIndex + 1;
		CString strDefExt = "gesture";
		dlgFile.m_ofn.lpstrDefExt = strDefExt;
			
		CString strFilter;
		
		strFilter += "Gesture Data file (gesture)";
		strFilter += (TCHAR)NULL;
		strFilter += "*.gesture";
		strFilter += (TCHAR)NULL;
		
		strFilter += "*.*";
		strFilter += (TCHAR)NULL;
		strFilter += "*.*";
		strFilter += (TCHAR)NULL;
		strFilter += (TCHAR)NULL;

		dlgFile.m_ofn.lpstrFilter = strFilter;
		dlgFile.m_ofn.lpstrTitle = title;
		dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(1000 * _MAX_PATH);

		CString approot;
		support.GetAppRoot(approot);

		dlgFile.m_ofn.lpstrInitialDir = approot;

		bRet = (dlgFile.DoModal() == IDOK) ? TRUE : FALSE;
		
		fileName.ReleaseBuffer();
	
		if(!bRet)
			return 0;
		}
	else
		{
		fileName = fname;
		}
	
	//MsgBox(fileName, title);

	CxFile fp;

	//CFileException *fileException = new CFileException;
	//fileException->m_strFileName = fileName;
	
	if(loadflag)
		{
		// Load gesture file
		int state = 0, count = 0;
		
		//try
			{
			if(!fp.Open(fileName,  CFile::modeRead, NULL))
				{
				if(fname != "")
					{			
					CString prog = support.approot;
					prog += support.GetFileNameExt(fileName);

					P2N("Attempting copy from %s to %s\r\n", prog, fileName);

					// Attempt copy from installation directory
					CopyFile(prog, fileName, true);
					
					if(!fp.Open(fileName,  CFile::modeRead, NULL))
						{
						CString str; str.Format("Cannot open gesture file '%s'", fileName);
						AfxMessageBox(str); //, "Error Opening / Reading Gesture File");
						return 0;
						}
					}
				}
			}

		//catch(CFileException *e)
		//	{
		//	 if( e->m_cause == CFileException::fileNotFound )
		//			MsgBox("Cannot open gesture file2\r\n", "");
		//
		//	MsgBox("Cannot open gesture file\r\n", "");
		//	
		//	P2N("File Exception\r\n");
		//	
		//	e->ReportError();
		//	
		//	fileException->Delete();
		//	return 0;
		//	}
		//fileException->Delete();
		//END_CATCH_ALL
		
		// Empty old training data
		traindata.RemoveAll();

		//CListBox
		m_list.ResetContent();

		// Read prelude
		
		CString head;
		int ret = fp.ReadString(head);

		if(head != gfhead)
			{
			AfxMessageBox("Invalid header");
			return false;
			}

		while(true)
			{
			CString str;
			
			// Read in one line:
			int ret = fp.ReadString(str);
			int len = str.GetLength();

			//P2N("len=%d line='%s'\r\n", len, str);
			
			switch (state)	
				{
				case 0:
					{
					if(len == 0)		// Empty header - malformed entry or EOF
						{
						state = 0;
						goto endd;						
						break;
						}

					// Add Unique element:
					if(m_list.FindString(0, str) == LB_ERR)
						m_list.AddString(str);

					tdata *element = new tdata;
					element->str = str;
					traindata.Add(element);
					state = 1; count = 0;

					//P2N("Adding to '%s' ====>\r\n", 
					//((tdata*)traindata[traindata.GetUpperBound()])->str);
					}
					break;

				case 1:			
					if(len == 0)
						{
						state = 0;		// Reset state machine
						break;
						}

					// Parse one line:
					int prog = 0;
					for(int loop = 0; loop < ENTRYPERLINE; loop++)
						{
						int prog2 = str.Find(" ", prog);

						if(prog2 < 0)
							{
							prog2 = len;
							}
						CString str2 = str.Mid(prog, prog2 - prog);
						str2.TrimLeft(); str2.TrimRight();
						prog = prog2 +	1;
					
						//P2N("'%s' ", str2);

						((tdata*)traindata[traindata.GetUpperBound()])
							->arr.Add(atoi(str2));

						if(prog >= len)
							break;
						}
					break;
				}

			// EOF condition
			if(!ret)
				break;
			}
endd: 
	;

		}
	else
		{
		// Save new gestures to file
		try
			{
			fp.Open(fileName, CFile::modeCreate | CFile::modeReadWrite,	NULL);
			}
		catch(CFileException* theException )
			{
			theException->ReportError();
			}

		fp.WriteString(gfhead);
		fp.WriteString("\r\n");

		int len = traindata.GetSize();
		
		// Dump/Save train data
		for(int zloop = 0; zloop < len; zloop++)
			{
			fp.WriteString(((tdata*)traindata[zloop])->str);
			fp.WriteString("\r\n");

			//P2N("%s\r\n", ((tdata*)traindata[zloop])->str);

			int ilen = ((tdata*)traindata[zloop])->arr.GetSize();

			for(int iloop = 0; iloop < ilen; iloop++)
				{
				if(iloop > 0 && iloop % ENTRYPERLINE == 0)
					{
					//P2N("\r\n");
					fp.WriteString("\r\n");
					}
				CString str; 
				str.Format("%d ", ((tdata*)traindata[zloop])->arr[iloop]);
				//P2N(str);

				fp.WriteString(str);
				}
			//P2N("\r\n");
			fp.WriteString("\r\n\r\n");
			}
		}

	P2N("Done Gesture Open/Save '%s'\r\n", fname);

	return 0;
}

//////////////////////////////////////////////////////////////////////////

void CGestureDlg::OnButton4() 

{
	CString str;
	
	LoadSave(true, str);
}

//////////////////////////////////////////////////////////////////////////

void CGestureDlg::ExecGesture(CString &str)

{
	ExecGesture2(str);

	CString org;
	m_edit.GetWindowText(org);
	org += str;
	m_edit.SetWindowText(org);

	// CEdit
	int  ee, ss;
	ss =  ee = org.GetLength();
	m_edit.SetSel(ss, ee);


#if 0
	if(str == "Open File")
		{
		OpenFileCb("");
		}
	else if(str == "Prev Document")
		{
		((CShadesDlg* )parent)->SendMessage(WM_SYSKEYDOWN, 'P', 0);
		}
	else if(str == "Next Document")
		{
		((CShadesDlg* )parent)->SendMessage(WM_SYSKEYDOWN, 'N', 0);
		}
	else if(str == "Size to Width")
		{
		if(((CShadesDlg* )parent)->current < 0)
			return;

		CXrayDoc *doc = (CXrayDoc*)((CShadesDlg* )parent)->docarr[((CShadesDlg* )parent)->current]; 
		doc->showstate = STATE_WIDTH;
		((CShadesDlg* )parent)->Invalidate();
		}
	
	else if(str == "Size to Height")
		{
		if(((CShadesDlg* )parent)->current < 0)
			return;

		CXrayDoc *doc = (CXrayDoc*)((CShadesDlg* )parent)->docarr[((CShadesDlg* )parent)->current]; 
		doc->showstate = STATE_HEIGHT;
		((CShadesDlg* )parent)->Invalidate();
		}
	else if(str == "Close File")
		{
		CloseFileCb(parent);
		}

	else if(str == "Load Last Doc")
		{
		((CShadesDlg* )parent)->OpenLastFile();
		}

	else if(str == "Exit")
		{
		((CShadesDlg* )parent)->EndDialog(IDOK, false);
		}

	else if(str == "Brightness")
		{
		//MBOX("Bright", "");
		BrightCb(parent);
		}
#endif

}

//////////////////////////////////////////////////////////////////////////

void CGestureDlg::ExecGesture2(CString &str)

{
	for(int loop = 0; loop < sizeof(varcoms) / sizeof(coms); loop++)
		{
		if(varcoms[loop].str != NULL)
			{
			//if(varcoms[loop].func != NULL)
				{
				P2N("Iterate  %s\r\n", varcoms[loop].str);
				}
			}
		}
}


///////////////////////////////////////////////////////////////////////////

void CGestureDlg::OnLButtonDblClk(UINT nFlags, CPoint point) 

{
	int trchecked = ( (CButton*)GetDlgItem(IDC_RADIO3) )->GetState();
	trchecked = trchecked & 0x03 ? true : false;

	if(trchecked)
		{
		P2N("Double click in test\r\n");
		}

	CGrayDlg::OnLButtonDblClk(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////

void CGestureDlg::OnDblclkList1() 

{

	static int in_proc = 0;

	if(in_proc)
		return;

	in_proc = 1;

	int trchecked = ( (CButton*)GetDlgItem(IDC_RADIO3) )->GetState();
	trchecked = trchecked & 0x03 ? true : false;

	CString str;
				
	if(m_list.GetCurSel() >= 0 )
		{
		m_list.GetText(m_list.GetCurSel(), str);				
		}

	CDWordArray *arr = &normalarr;

	if(trchecked)
		{
		int len = traindata.GetSize();
		int idx = 0;
		int minerr = 0x8000000;

		P2N("Double on list test %s\r\n", str);
		//Invalidate();

		CDC* pdc = GetDC();

		int count = 0;
		for(int zloop = 0; zloop < len; zloop++)
			{
			int err = 0;
			
			if(((tdata*)traindata[zloop])->str == str)
				{
				CString str2;

				str2.Format("%s (%d)", ((tdata*)traindata[zloop])->str, ++count);
				m_prog.SetText(str2);
				support.YieldToWinEx();
				
				P2N("\r\nDumping Train Data %s %x\r\n\r\n", ((tdata*)traindata[zloop])->str, ((tdata*)traindata[zloop])->item);
	
				int ilen = ((tdata*)traindata[zloop])->arr.GetSize();
				double dx =  0, dy = 0;

				CDWordArray xxarr, yyarr;
	
				for(int iloop = 0; iloop < ilen; iloop++)
					{
					int		angle = ((tdata*)traindata[zloop])->arr[iloop] ;
					double	rad = ((MATHPI / 180.0) * (double)(angle % 90));

					//P2N("%d ",  angle);
					
					//////////////////////////////////////////////////////
					//		   270
					//  180  ---|---  0
					//          90
					//////////////////////////////////////////////////////

					// New arrangement 

					//////////////////////////////////////////////////////
					//		    0
					//  270  ---|---  90
					//         180
					//////////////////////////////////////////////////////

					double delta = .6;
					
					// Quadrant
					if (angle >= 270) 
						{
						dx -= delta * sin(rad); dy -= delta * cos(rad);
						}
					else if(angle >= 180) 
						{
						dx += delta * cos(rad); dy -= delta * sin(rad);
						}
					else if(angle >= 90) 
						{
						dx += delta * sin(rad); dy += delta * cos(rad);
						}
					else 
						{
						dx -= delta * cos(rad); dy += delta * sin(rad);
						}

					xxarr.Add((int)dx); yyarr.Add((int)dy);

					//P2N("angle=%d rad=%.2f  dx=%-3d dy=%-3d\t", angle, rad, (int)dx, (int)dy);
					}

				RECT rc; rc.top = 0; rc.bottom = 0; rc.left = 0; rc.right = 0;
				
				GetWindowRect(&rc);
				rc.top = rc.bottom; rc.bottom = 0;		// Swap coordinates
				rc.left = rc.right; rc.right = 0;
				
				// Find boundaries
				for(int loop2 = 0; loop2 < xxarr.GetSize(); loop2++)
					{
					int	dxx = xxarr[loop2];
					int	dyy = yyarr[loop2];

					if(rc.top > dyy) rc.top = (int)dyy;
					if(rc.bottom < dyy) rc.bottom = (int)dyy;

					if(rc.left > dxx) rc.left = (int)dxx;
					if(rc.right < dxx) rc.right = (int)dxx;
					}
			
				//P2N("\r\ntop=%d left=%d bottom=%d right=%d\r\n",
				//			rc.top, rc.left, rc.bottom, rc.right);
			
				int midx = 0, midy = 0;

				// Display 	
				for(int loop = 0; loop < xxarr.GetSize(); loop++)
					{
					//P2N("xx=%-3d yy=%-3d\t", dx, dx);
				
					if(loop == 0)
						pdc->MoveTo(ABS(rc.left) + 40 + xxarr[loop], yyarr[loop] + ABS(rc.top) + 40);

					pdc->LineTo(xxarr[loop] + ABS(rc.left) + 40, yyarr[loop] + ABS(rc.top) + 40);

					if(loop % 15 == 0)
						Sleep(1);
					}

				// Correct bounding rect
				rc.right += ABS(rc.left); rc.left = 40;
				rc.bottom += ABS(rc.top); rc.top = 40;
				
				//break;		// only show one

				//P2N("top=%d left=%d bottom=%d right=%d\r\n",
				//			rc.top, rc.left, rc.bottom, rc.right);

				Sleep(500);

				rc.bottom++; rc.top--;
				rc.left--;   rc.right++;

				InvalidateRect(&rc);
				//Invalidate();

				support.YieldToWinEx();
				}
			}
		}

	Sleep(500);
	m_prog.SetText("");
	support.YieldToWinEx();
	Invalidate();
	in_proc = 0;
}

void CGestureDlg::Move(int want_x, int want_y)

{
	SetWindowPos( NULL, want_x, want_y, 0, 0, SWP_NOSIZE | SWP_NOZORDER );  	
}

CGestureDlg::~CGestureDlg()

{
	//P2N("Destoying training data\r\n");

	int len = traindata.GetSize();

	// Free train data
	for(int zloop = 0; zloop < len; zloop++)
		{
		//P2N("%s %x\r\n", ((tdata*)traindata[zloop])->str, ((tdata*)traindata[zloop])->item);	
		//int ilen = ((tdata*)traindata[zloop])->arr.GetSize();

		//for(int iloop = 0; iloop < ilen; iloop++)
		//	{
		//	//P2N("%3d  ", ((tdata*)traindata[zloop])->arr[iloop]);
		//	}

		delete ((tdata*)traindata[zloop]);

		//P2N("\r\n");
		}
}

void CGestureDlg::OnButton7() 
{
	int idx = m_list.GetCurSel();

	if(idx < 0)
		{
		AfxMessageBox("Please select an item to clear."); //, "Cannot Clear");
		return;
		}

	// CListbox
	CString str;
	m_list.GetText(idx, str);

	int len = traindata.GetUpperBound(); //GetSize();

	int count = 0;
	for(int zloop = len; zloop >= 0; zloop--)
		{
		int err = 0;
		
		if(((tdata*)traindata[zloop])->str == str)
			{
			P2N("Clearing %s (%d) %d", ((tdata*)traindata[zloop])->str, count, zloop);
			traindata.RemoveAt(zloop);
			count++;
			}
		}

	str.Format("Cleared %d items", count);
	m_prog.SetText(str);		
}


void CGestureDlg::DumpGestures()

{
	int len = traindata.GetSize();
	int count = 0;
	for(int zloop = 0; zloop < len; zloop++)
		{
		P2N("Getsure %d '%s' %d\r\n", zloop, 
				((tdata*)traindata[zloop])->str, ((tdata*)traindata[zloop])->arr.GetSize());
		}	
}

void CGestureDlg::OnButton8() 

{
	DumpGestures();		
}

void CGestureDlg::OnButton9() 

{
	P2N("Play Gesture\r\n");
	PlayGesture();
}


void CGestureDlg::PlayGesture()

{
	int idx;

	if((idx = m_list.GetCurSel()) < 0 )
		{
		AfxMessageBox("Please select an Item to play");
		return;
		}
	
	static int in_proc = 0;

	if(in_proc)
		return;

	in_proc = 1;		

	CString str;
	m_list.GetText(idx, str);
	
	CDWordArray *arr = &normalarr;

	//if(trchecked)
		{
		int len = traindata.GetSize();
		int idx = 0;
		int minerr = 0x8000000;

		P2N("Playing '%s'\r\n", str);
		//Invalidate();

		CDC* pdc = GetDC();

		int count = 0;
		for(int zloop = 0; zloop < len; zloop++)
			{
			int err = 0;
		
			tdata *dat = (tdata*)traindata[zloop];

			if(dat->str == str)
				{				
				CString str2;

				str2.Format("%s (%d)", dat->str, ++count);
				m_prog.SetText(str2);

				support.YieldToWinEx();
				
				P2N("\r\nDumping Train Data str=%s item=%x size=%d\r\n\r\n", dat->str, dat->item, dat->arr.GetSize());
	
				int ilen	= dat->arr.GetSize() / 2;
				double dx	= 0, dy = 0;

				CDWordArray xxarr, yyarr;
	
				// Reverse building strokes onto array
				for(int iloop = 0; iloop < ilen; iloop++)
					{
					int		angle = dat->arr[2 * iloop] ;
					int		dist  = dat->arr[2 * iloop + 1] / DIST_MULTI ;

					//P2N("%d %d ",  angle, dist);

					double	delta	= (double)dist;
					double	rad		= ((MATHPI / 180.0) * (double)(angle % 90));

					//////////////////////////////////////////////////////
					//		  -- 0 +-
					//   270 ----|--- 90
					//       -+ 180 ++
					//////////////////////////////////////////////////////

					// Quadrant
					if (angle >= 270) 
						{
						dx -= delta * cos(rad); dy -= delta * sin(rad);
						}
					else if(angle >= 180) 
						{
						dx -= delta * sin(rad); dy += delta * cos(rad);
						}
					else if(angle >= 90) 
						{
						dx += delta * cos(rad); dy += delta * sin(rad);
						}
					else 
						{
						dx += delta * sin(rad); dy -= delta * cos(rad);
						}

					xxarr.Add((int)dx); 
					yyarr.Add((int)dy);

					//P2N("angle=%d rad=%.2f  dx=%-3d dy=%-3d\r\n", angle, rad, (int)dx, (int)dy);
					}

				RECT rc; SetRectEmpty(&rc);

				// Find boundaries and extremes
				int xlen = xxarr.GetSize();
				for(int loop2 = 0; loop2 < xlen; loop2++)
					{
					int yyy = (int)yyarr[loop2];

					if(rc.top > yyy) rc.top = (int)yyy;
					if(rc.bottom < yyy) rc.bottom = (int)yyy;

					int	xxx = (int)xxarr[loop2];
					
					if(rc.left  > xxx) rc.left = (int)xxx;
					if(rc.right < xxx) rc.right = (int)xxx;

					//P2N("xxx=%d yyy=%d\r\n", (int)xxx, (int)yyy);
					}
				
				P2N("surounding top=%d left=%d bottom=%d right=%d\r\n", 
								rc.top, rc.left, rc.bottom, rc.right);

				// Shift it to zero based
				if(rc.left < 0) 
					{
					for(loop2 = 0; loop2 < xlen; loop2++)
						xxarr[loop2] += abs(rc.left);

					rc.right += abs(rc.left); rc.left = 0;
					}

				if(rc.top < 0) 
					{
					for(loop2 = 0; loop2 < xlen; loop2++)
						yyarr[loop2] += abs(rc.top);

					rc.bottom += abs(rc.top); rc.top = 0;
					}

				//////////////////////////////////////////////////////////
				// Scale it / Center it

				P2N("pre-adj: top=%d left=%d bottom=%d right=%d\r\n",
							rc.top, rc.left, rc.bottom, rc.right);

				if( (rc.right > 160) || (rc.bottom > 140) )
					{
					double scale, xscale, yscale;
			
					xscale = (double)rc.right / 160;
					yscale = (double)rc.bottom / 140;

					P2N("xScale=%f yScale=%f\r\n", xscale, yscale);

					scale = max(xscale, yscale);

					for(int sloop = 0; sloop < xxarr.GetSize(); sloop++)
						{
						xxarr[sloop] = int((double)xxarr[sloop] / scale); 
						yyarr[sloop] = int((double)yyarr[sloop] / scale); 
						}
					rc.right = 160; rc.bottom = 140;
					}
				else		 
					{ 
					int ww = rc.right, hh = rc.bottom;
					
					RECT rc2; GetClientRect(&rc2);

					rc2.right	= 200;
					rc2.bottom	= 200;

					//     rc      rc2
					//  ------------
					// |    ---     |
					// |   |   |    |
					// |    ---     |
					//  ------------
					//
					
					rc.left = (rc2.right / 2) - (rc.right / 2);
					rc.top = (rc2.bottom / 2) - (rc.bottom / 2);

					rc.right = rc.left + ww;
					rc.bottom = rc.top + hh;
					}

				P2N("Adj: top=%d left=%d bottom=%d right=%d\r\n",
							rc.top, rc.left, rc.bottom, rc.right);
	
				int xoffset = 10, yoffset = 40;

				// Display 	
				for(int loop = 0; loop < xxarr.GetSize(); loop++)
					{
					//P2N("xx=%-3d yy=%-3d\t", dx, dx);
				
					if(loop == 0)
						pdc->MoveTo(ABS(rc.left) + xoffset + xxarr[loop], yyarr[loop] + ABS(rc.top) + yoffset);

					pdc->LineTo(xxarr[loop] + ABS(rc.left) + xoffset, yyarr[loop] + ABS(rc.top) + yoffset);

					if(loop % 15 == 0)
						{
						Sleep(1);
						//Invalidate();
						//support.YieldToWinEx();
						}
					}

				// Correct bounding rect
				//rc.right += ABS(rc.left); rc.left = 40;
				//rc.bottom += ABS(rc.top); rc.top = 40;
				
				//break;		// only show one

				//P2N("top=%d left=%d bottom=%d right=%d\r\n",
				//			rc.top, rc.left, rc.bottom, rc.right);
			
				// Let the user see it
				Sleep(500);

				// Erase this drawing
				Invalidate();
				support.YieldToWinEx();

				//rc.bottom++; rc.top--; rc.left--;   rc.right++;
				//InvalidateRect(&rc);x
				}
			}
		}

	Sleep(500);
	m_prog.SetText("");
	
	support.YieldToWinEx();
	Invalidate();
	in_proc = 0;
}



//////////////////////////////////////////////////////////////////////////
//
// Function name:    CGestureDlg::Recognize
// Description:      <empty description>
// Return type:      void 
//
//////////////////////////////////////////////////////////////////////////

void CGestureDlg::Recognize()

{			
	CDWordArray anglearr;
	RecBuildArr(&anglearr);
	
	int len2 = anglearr.GetSize();

	// Anything?
	if(len2 < 3)
		return;

	int trchecked = ( (CButton*)GetDlgItem(IDC_RADIO1))->GetState();
	trchecked = trchecked & 0x03 ? true : false;

	// CButton radio
	P2N("Got gesture %d items mode=%d\r\n",  anglearr.GetSize(), trchecked);

#if 0
	// For testing -- Dumping angle array PRE
	P2N("\r\nDumping angle array PRE %d\r\n", len2);
	for(int oloop = 0; oloop < len2 / 2; oloop++)
		{
		P2N("%03d %03d - ", anglearr[2 * oloop], anglearr[2 * oloop + 1]);		
		}
#endif

//////////////////////////////////////////////////////////////////////////
// To switch off distance normalization

#define NORMALIZE

#ifndef NORMALIZE
	// This is without normalisation
	int clen = anglearr.GetSize() ;
	for(int zloop = 0; zloop < clen; zloop++)
		{
		normalarr.Add(anglearr[zloop]);
		}
#else
	// This is with normalisation
	int hlen = anglearr.GetSize() / 2;
	for(int zloop = 0; zloop < NORMALIZE_TO / 2; zloop++)
		{
		int xloop = (zloop * hlen) / (NORMALIZE_TO / 2);
		normalarr.Add(anglearr[2 * xloop]);
		normalarr.Add(anglearr[2 * xloop + 1]);
		}
#endif

//////////////////////////////////////////////////////////////////////////

	// For testing
	//int dlen = normalarr.GetSize();
	//P2N("\r\nDumping angle array POST %d\r\n", dlen);
	//for(int oloop2 = 0; oloop2 < dlen; oloop2++)
	//	{
	//	P2N("%03d  ", normalarr[oloop2]);		
	//	}

	if(trchecked)
		{	
		CString str;
	
		if(m_list.GetCurSel() < 0 )
			{
			AfxMessageBox("Please select an Item to Train");
			return;
			}
		if(m_list.GetCurSel() >= 0 )
			{
			m_list.GetText(m_list.GetCurSel(), str);				
			}
			
		if(str != "")
			{
			tdata *trd = new tdata;
			
			trd->str = str;
			trd->item = support.HashString(str);
			//trd->item = m_list.GetCurSel();			
			int nlen = normalarr.GetSize();
			//for(int zloop = 0; zloop < NORMALIZE_TO; zloop++)
			for(int zloop = 0; zloop < nlen; zloop++)
				{
				trd->arr.Add(normalarr[zloop]);
				}

			// Add to training data
			P2N("Train data added '%s' %x\r\n", str, support.HashString(str));

			traindata.Add(trd);
			newtrain++;
			}
		}
	else
		{
		// Recognize	
		RecFuzzy(&normalarr);
		}
}

void CGestureDlg::RecBuildArr(CDWordArray *arr)

{
	int loop =	0, xold = 0, yold = 0; 
	int len = xarr.GetSize();

	// Nothing to process
	if(len < 3)
		return;

	// New arrangement where up is zero degrees

	//////////////////////////////////////////////////////////////
	//
	//  x ---------->      270  -x -y  | NW | NE |  +x -y   0
	//  |             dy               |----|----|
	//  |    x ----->      180  -x +y  | SW | SE |  +x +y   90
	//  |    |
	//    dx
	//
	//////////////////////////////////////////////////////////////

	xold = xarr[loop];
	yold = yarr[loop];

	for(loop = 0; loop < len; loop++)
		{
		int angle = 0, dx, dx2, dy, dy2;
			
		// Mouse was not down, mark new origin
		if(!marr[loop])
			{
			//xold = xarr[loop];
			//yold = yarr[loop];
			//continue;
			}

		dx2 = dx = xarr[loop] - xold;
		dy2 = dy = yarr[loop] - yold;

#if 1
		// Pre fetch new until we encounter a non zero value
		int step = 1;
		while(true)
			{
			// Limit step also
			if(step > 10)
				break;

			// Lower bound
			if((loop - step) < 0)
				break;

			// Upper bound
			if(loop+step >= len)
				break;

			if(dx == 0 || dy == 0)
				{
				//P2N("Prefech %d\r\n", step);
				//dx = xarr[loop+step] - xold;
				//dy = yarr[loop+step] - yold;
				
				// Expand step symmetric
				dx = xarr[loop+step] - xarr[loop-step];
				dy = yarr[loop+step] - yarr[loop-step];
				}
			step++;
			}
		//loop += step;
#endif
		
		//        dl  /| dy
		//          /  |    angle = asin( dy/dl )
		//        /----|
		//          dx
		
		// Get angle
		double dl2 = SQRT(SQR(dx2) + SQR(dy2));
		double dl  = SQRT(SQR(dx)  + SQR(dy));
		angle  =(int)((180.0 / MATHPI) * asin((double)abs(dy) / dl));
		
		//P2N("dx=%d dy=%d length=%.2f ratio=%.2f sin=%f angle=%d -- ",  
		//			dx, dy, dl, dy / dl, asin(dy / dl), angle);

		//   NW|NE
		//  ---|---
		//   SW|SE

		// Place quadrant
		if(dx == 0 && dy == 0) 
			angle = 0;					// No movement
		
		if(dx == 0)						// Pure y
			{
			if(dy >= 0)
				angle =	180;			// DOWN
			else
				angle =	0;				// UP	
			}
		else if(dy == 0)				// Pure x
			{
			if(dx >= 0)
				angle =	90;				// Down	
			else
				angle =	270;			// UP	
			}
		else if(dx > 0)					// East
			{
			if(dy >= 0)
				angle = angle + 90;		// SE
			else
				angle = 90 - angle;		// NE
			}
		else							// West
			{
			if(dy >= 0)
				angle = 270 - angle;	// SW
			else
				angle = 270 + angle;	// NW
			}

		//P2N("angle=%d dl2=%d  dl=%d step=%d\r\n", angle, (short)dl2, (short)(dl), step);
		
		arr->Add(angle);
		arr->Add((short)(dl2) * DIST_MULTI);
		
	xold = 	xarr[loop]; yold = yarr[loop];
	}

	P2N("%d items\r\n", xarr.GetSize());
}


void CGestureDlg::OnDestroy() 
{
	CGrayDlg::OnDestroy();
	
	if(newtrain)
		{
		if(AfxMessageBox("Training data was added/modified. Save it?", MB_YESNO) == IDYES)
			{
			CString str;
			LoadSave(false, str);
			}
		}	
}

void CGestureDlg::OnButton11() 

{
	int idx = m_list.GetCurSel();

	if(idx < 0)
		{
		AfxMessageBox("Please select an item to clear."); //, "Cannot Clear");
		return;
		}

	// CListbox
	CString str;
	m_list.GetText(idx, str);

	int len = traindata.GetUpperBound(); //GetSize();

	int count = 0;
	for(int zloop = len; zloop >= 0; zloop--)
		{
		int err = 0;
		
		if(((tdata*)traindata[zloop])->str == str)
			{
			P2N("Clearing %s (%d) %d", ((tdata*)traindata[zloop])->str, count, zloop);
			traindata.RemoveAt(zloop);
			count++;
			break;
			}
		}

	str.Format("Cleared %d items", count);
	m_prog.SetText(str);		
}

void CGestureDlg::OnButton15() 
{
	EndDialog(IDOK);	
}
