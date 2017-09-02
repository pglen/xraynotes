
/* =====[ GestureDlg.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the gestureDlg.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  10/21/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// MagDlg.cpp : implementation file
//

#include "stdafx.h"
#include <direct.h>
#include "xraynotes.h"
#include "gestureDlg.h"
#include "GetText.h"
#include "FileDialogST.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define NORMALIZE_TO	500		// Normalize stroke list to this many entries

//////////////////////////////////////////////////////////////////////////

CGestureDlg::coms	CGestureDlg::varcoms[] = 

{
	"Open File",		NULL,
	"Prev Document",	NULL,
	"Next Document",	NULL,
	"Size to Width",	NULL,
	"Size to Height",	NULL,
	"Close File",		NULL,
	"Load Last Doc",	NULL,
	"Exit",				NULL,
	"Brightness",		NULL,

	NULL, NULL
};

/////////////////////////////////////////////////////////////////////////////
// CGestureDlg dialog

CGestureDlg::CGestureDlg(CWnd* pParent /*=NULL*/)
	: CGrayDlg(CGestureDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGestureDlg)
	//}}AFX_DATA_INIT

	more = true;
	
	want_x = 0, want_y = 0; dark = 0;
	addto = in_draw = created = push = false;
	busy = loaded = changed = trigger = false;
	lbutton = false;

	windowname = "CGestureDlg"; //support.GetFileNameExt(__FILE__);
}

void CGestureDlg::DoDataExchange(CDataExchange* pDX)

{
	CGrayDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGestureDlg)
	DDX_Control(pDX, IDC_COM, m_com);
	DDX_Control(pDX, IDC_BUTTON9, m_b9);
	DDX_Control(pDX, IDC_BUTTON8, m_b8);
	DDX_Control(pDX, IDC_BUTTON7, m_b7);
	DDX_Control(pDX, IDC_BUTTON6, m_b6);
	DDX_Control(pDX, IDC_BUTTON4, m_b4);
	DDX_Control(pDX, IDC_BUTTON3, m_b3);
	DDX_Control(pDX, IDC_BUTTON10, m_b10);
	DDX_Control(pDX, IDC_BUTTON2, m_b2);
	DDX_Control(pDX, IDC_BUTTON1, m_butt1);
	DDX_Control(pDX, IDC_PROG, m_prog);
	DDX_Control(pDX, IDC_LIST1, m_list);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGestureDlg, CGrayDlg)
	//{{AFX_MSG_MAP(CGestureDlg)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
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
	ON_BN_CLICKED(IDC_BUTTON10, OnButton10)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGestureDlg message handlers

//DEL void CGestureDlg::Show()
//DEL 
//DEL {
//DEL 	if(!IsWindow(m_hWnd))
//DEL 		{
//DEL 		Create(IDD);
//DEL 
//DEL 		if(pre_want_x)	want_x = pre_want_x;
//DEL 		if(pre_want_y)	want_y = pre_want_y;
//DEL 
//DEL 		SetWindowPos( NULL, want_x, want_y, 0, 0, SWP_NOSIZE | SWP_NOZORDER );  	
//DEL 		}
//DEL 
//DEL 	created = true;
//DEL 
//DEL 	// Show based upon loaded settings
//DEL 	if(pre_showstate)
//DEL 		{
//DEL 		pre_showstate = false;
//DEL 		}
//DEL 	else
//DEL 		{
//DEL 		ShowWindow(SW_SHOW);
//DEL 		}
//DEL }


BOOL CGestureDlg::OnInitDialog() 

{
	CGrayDlg::OnInitDialog();
		
	BYTE	bAlpha = 200;

	if (bAlpha < 255)
		{
        //  Set layered style for the dialog
        SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);

        //  Call it with 255 as alpha - opacity
		if(g_pSetLayeredWindowAttributes)
			g_pSetLayeredWindowAttributes(m_hWnd, 0, bAlpha, LWA_ALPHA);
		}
	m_prog.SetTrans(true);
	//m_prog.SetTextColor(RGB(20,20,20));
	ShowProg("Gesture recognition ready.");
	
	m_com.SetTrans(true);
	m_com.SetText("Command");

	OnButton1();

	( (CButton*)GetDlgItem(IDC_RADIO2) )->SetCheck(true) ;

	pencil = AfxGetApp()->LoadCursor(IDC_CURSOR7);
    ASSERT(pencil);

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CGestureDlg::OnLButtonDown(UINT nFlags, CPoint point) 

{
	//drag_x = point.x; drag_y = point.y;

	lbutton = true;

	RECT rc; GetClientRect(&rc);
	CRect rect(rc.right - 17, 2, rc.right - 4, 16);

	if(rect.PtInRect(point))
		{
		//P2N("Click in close box\r\n");
		ShowWindow(SW_HIDE);
		return;
		}

	CRect rect2(rc.left, rc.top, rc.right, 12);

	if(rect2.PtInRect(point))
		{
		P2N("Click in header box\r\n");
		PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));	
		return;
		}

	//P2N("Mouse down: Drag_x=%d drag_y=%d\r\n", drag_x, drag_y);
	
	CGrayDlg::OnLButtonDown(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////

void CGestureDlg::OnMouseMove(UINT nFlags, CPoint point) 

{
	CPoint point2 = point;	ClientToScreen( &point2 ); 

	lastflags = nFlags, lastpoint = point;

	//SetTimer(4, 300, NULL);

	// If pending recogntion, add to it
	//if(addto)
	//	KillTimer(2);

	if( (nFlags & MK_LBUTTON))
		{
		lbutton = true;
		in_draw = true;

		// If pending recogntion, add to it
		KillTimer(2);

		//busy = false;
		
		SetCursor(pencil);

		//P2N("Adding mouse gesture x=%d y=%d\r\n", old_x - point.x, old_y - point.y);
		
		// Previous point
		int xsizediff = 4, ysizediff =  4;

		// Calculate the invalidate region
		if(xarr.GetSize() > 0)
			{
			xsizediff = ABS(xarr.ElementAt(xarr.GetUpperBound()) - point.x) + 1;
			ysizediff = ABS(yarr.ElementAt(yarr.GetUpperBound()) - point.y) + 1;
			//P2N("xdiff=%d ydiff=%d\r\n", xsizediff, ysizediff);
			}
		else
			{
			lbutton = false;
			}

		CRect rc(point.x, point.y, point.x, point.y);
		rc.InflateRect(xsizediff, ysizediff);

		if(xarr.GetSize() > 0)
			{
			if(xarr.ElementAt(xarr.GetUpperBound()) == 0)
				{
				InvalDrawing();
				}
			else
				{
				InvalidateRect(&rc);
				}
			}
		
		// Insert into point array
		if(addto)
			{
			addto = false;
			P2N("Draw in addto periood\r\n");
			KillTimer(1); KillTimer(2);

			// Fake boundary by adding nulls
			xarr.Add(0);
			yarr.Add(0);	
			}

		xarr.Add((WORD)point.x);
		yarr.Add((WORD)point.y);
		}

	if(!trigger)
		{
		trigger = true;
		TRACKMOUSEEVENT te;

		te.cbSize = sizeof(TRACKMOUSEEVENT);
		te.dwFlags = TME_LEAVE;
		te.hwndTrack = this->m_hWnd;
		te.dwHoverTime = 0;

		::_TrackMouseEvent(&te);
		}	

	CGrayDlg::OnMouseMove(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////

BOOL CGestureDlg::PreTranslateMessage(MSG* pMsg) 

{
	if(pMsg->message == WM_MOUSELEAVE)
		{
		//P2N("CGestureDlg::PreTrans WM_MOUSELEAVE\r\n");
		lbutton = trigger = false;
		SynLButtonUp(300);
		}

#if 0
	if(pMsg->message == WM_SYSKEYDOWN)
		{
		//((CShadesDlg* )parent)->SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
		P2N("CGestureDlg::PreTrans WM_SYSKEYDOWN %c (%d) %x\r\n", pMsg->wParam, pMsg->wParam, pMsg->lParam);
		//return(0);
		}

	if(pMsg->message == WM_SYSKEYDOWN)
		{	
		//((CShadesDlg* )parent)->SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
		
		P2N("CGestureDlg::PreTrans WM_SYSKEYDOWN %c (%d) %x\r\n", pMsg->wParam, pMsg->wParam, pMsg->lParam);
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

		CPen pen; pen.CreatePen(PS_SOLID, 1,  RGB(20, 20,20));
		CPen* pOldPen = dc.SelectObject(&pen);

		int loop =	0; 
		int len = xarr.GetSize();
		if(len)
			{
			dc.MoveTo(xarr[0], yarr[0]);

			for(loop = 0; loop < len; loop++)
				{
				if(xarr[loop] == 0 && xarr[loop] == 0)
					{
					// Move and skip agead
					if(loop < len - 1)
						{
						dc.MoveTo(xarr[loop + 1], yarr[loop+1]);
						loop++;
						}
					}
				else
					dc.LineTo(xarr[loop], yarr[loop]);
				}
			}

		dc.SelectObject(pOldPen);
		}
 
	// Paint background and controls
	CGrayDlg::OnPaint();
}

/////////////////////////////////////////////////////////////////////////

void CGestureDlg::OnTimer(UINT nIDEvent) 

{
	//P2N("CGestureDlg::OnTimer fired %d\r\n", nIDEvent);
	
	switch(nIDEvent)
		{
		// Clear gesture from window
		case 1:
			P2N("Clear gesture\r\n");
			KillTimer(nIDEvent);
			if(!in_draw)
				{
				busy = false;
				SetCursor(LoadCursor(NULL, IDC_ARROW));
				//InvalDrawing();
				Invalidate();

				//xarr.RemoveAll(); yarr.RemoveAll();
				//normalarr.RemoveAll();
				}
			break;

		// Recognize
		case 2:
			P2N("Recognize gesture\r\n");
			KillTimer(nIDEvent);

			addto = false;
			if(!in_draw)
				{
				busy = true;
				SetCursor(LoadCursor(NULL, IDC_WAIT));
				DoMagic();
				}

			SetTimer(1, 300, NULL);
			break;

		// Clear progress window
		case 3:
			//P2N("Clear gesture progress\r\n");
			KillTimer(nIDEvent);
			ShowProg("");
			break;

		case 4:
			// Mouse idle
			KillTimer(nIDEvent);
			//P2N("Mouse move stopped\r\n");
			//addto = true;
			//SynLButtonUp(10);
			//SetTimer(2, 200, NULL);
			break;

		default:
			//P2N("CGestureDlg::OnTimer Invalid Case\r\n");
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
		m_butt1.SetWindowText("&More >>");
		//xwidth = 215;

		CButton *butt = (CButton*)GetDlgItem(IDC_BUTTON1);
		WINDOWPLACEMENT wp;
		butt->GetWindowPlacement(&wp);		
		xwidth = wp.rcNormalPosition.right + 15;
		more = false;

		if(isTrainChecked())
			ShowProg("Switched to recognize mode");
		}
	else
		{
		m_butt1.SetWindowText("<< &Less");

		CButton *butt = (CButton*)GetDlgItem(IDC_BUTTON6);
		WINDOWPLACEMENT wp;
		butt->GetWindowPlacement(&wp);		
		xwidth = wp.rcNormalPosition.right + 15;
		
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

	Invalidate();
}


void CGestureDlg::OnButton2() 

{
	CGetText gt; 

	if(gt.DoModal() != IDOK)
		return;

	if(m_list.FindString(0, gt.m_edit) != LB_ERR)
		{
		CString tmp; tmp.Format("Cannot add duplicate item: '%s'", gt.m_edit);	
		ShowProg("Duplicate item not added");
		MBOX(tmp);
		return;
		}

	m_list.AddString(gt.m_edit);
	m_list.SelectString(0, gt.m_edit);

	CString tmp; tmp.Format("Added: '%s'", gt.m_edit);
	ShowProg(tmp);
	changed = true;
}

void CGestureDlg::OnButton3() 

{
	int idx = m_list.GetCurSel();

	if(idx < 0)
		{
		xMsgBox("Please select an item to delete.", "Cannot Delete");
		return;
		}
	m_list.DeleteString(idx);

	changed = true;

	// ClistBox
	m_list.SetCurSel(m_list.GetCount()-1);
}

int CGestureDlg::Fuzzy(CWordArray *arr) 

{
	int len = traindata.GetSize();
	int idx = 0;
	int minerr = 0x8000000;

	//P2N("Fuzzy Compare %d items\r\n", len);

	// Eval against train data
	for(int zloop = 0; zloop < len; zloop++)
		{
		int err = 0;
 
		//P2N("%s %x\r\n", ((tdata*)traindata[zloop])->str, ((tdata*)traindata[zloop])->item);

		int ilen = ((tdata*)traindata[zloop])->arr.GetSize();

		for(int iloop = 0; iloop < ilen; iloop++)
			{
			//P2N("%3d  ", ((tdata*)traindata[zloop])->arr[iloop]);
			err += abs(((tdata*)traindata[zloop])->arr[iloop] - arr->GetAt(iloop));
			}

		if(abs(err) < abs(minerr))
			{
			//P2N("Err sum=%d %s\r\n", err, ((tdata*)traindata[zloop])->str);

			minerr = err;
			idx  = zloop;
			}
		}

	//P2N("Min sum=%d idx=%d '%s'\r\n", minerr, idx, ((tdata*)traindata[idx])->str);

	TRACKMOUSEEVENT te;

	te.cbSize = sizeof(TRACKMOUSEEVENT);
	te.dwFlags = TME_CANCEL;
	te.hwndTrack = this->m_hWnd;
	te.dwHoverTime = 0;

	::_TrackMouseEvent(&te);

	if(minerr < NORMALIZE_TO * 60)
		{
		//P2N("Match: %s \r\n", ((tdata*)traindata[idx])->str);

		CString str; str.Format("%s", ((tdata*)traindata[idx])->str);
		ShowProg(str);
		
		if(!isTrainChecked())
			{
			CString str = ((tdata*)traindata[idx])->str;
			//P2N("Exec %s\r\n", str);
			ExecGesture(str);
			}
		}
	else
		{
		ShowProg("No Match");
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
			rc.bottom	= MAX(rc.bottom, yarr[loop]);
			rc.top		= MIN(rc.top, yarr[loop]);

			rc.right	= MAX(rc.right, xarr[loop]);
			rc.left		= MIN(rc.left, xarr[loop]);												
			}

		// Just to make sure it is in ....
		rc.bottom += 2;	rc.top -= 2;	
		rc.left -= 2;  rc.right += 2;

		InvalidateRect(&rc);		
		}
}

void CGestureDlg::OnButton6() 

{
	CString str(currfile);
	ShowProg("Saved Gestures");
	LoadSave(false, str);	
	SetFocus();
}

//////////////////////////////////////////////////////////////////////////

int CGestureDlg::LoadSave(int loadflag, CString &fname)

{
	CString fileName;

	//P2N("Gesture Open/Save '%s'\r\n", fname);

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

	if(loadflag)
		{
		LoadGestures(fileName);
		}
	else
		{
		SaveGestures(fileName);
		}

	return 0;
}

//////////////////////////////////////////////////////////////////////////

void CGestureDlg::OnButton4() 

{
	CString str;
	
	LoadSave(true, str);
	SetFocus();
}

//////////////////////////////////////////////////////////////////////////

void CGestureDlg::ExecGesture(CString &str)

{
	ExecGesture2(str);

	if(str == "Open File")
		{
		//OpenFileCb("");
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
		//CloseFileCb(parent);
		}

	//else if(str == "Load Last Doc")
	//	{
	//	((CShadesDlg* )parent)->OpenLastFile();
	//	}

	else if(str == "Exit")
		{
		((CShadesDlg* )parent)->EndDialog(IDOK, false);
		}

	else if(str == "Brightness")
		{
		//MBOX("Bright", "");
		//BrightCb(parent);
		}
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
				//P2N("Iterate  %s\r\n", varcoms[loop].str);
				}
			}
		}
}

void CGestureDlg::OnLButtonDblClk(UINT nFlags, CPoint point) 

{
	int trchecked = ( (CButton*)GetDlgItem(IDC_RADIO3) )->GetState();
	trchecked = trchecked & 0x03 ? true : false;

	if(trchecked)
		{
		//P2N("Double click in test\r\n");
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

	CWordArray *arr = &normalarr;

	if(trchecked)
		{
		int len = traindata.GetSize();
		int idx = 0;
		int minerr = 0x8000000;

		//P2N("Double on list test %s\r\n", str);
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
				ShowProg(str2);
				
				//P2N("\r\nDumping Train Data %s %x\r\n\r\n", ((tdata*)traindata[zloop])->str, ((tdata*)traindata[zloop])->item);
	
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
	ShowProg("");
	Invalidate();
	in_proc = 0;
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

		((tdata*)traindata[zloop])->arr.RemoveAll();
		delete ((tdata*)traindata[zloop]);

		//P2N("\r\n");
		}
}

void CGestureDlg::OnButton7() 

{
	int idx = m_list.GetCurSel();

	if(idx < 0)
		{
		xMsgBox("Please select an item to clear.", "Cannot Clear");
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
			//P2N("Clearing %s (%d) %d", ((tdata*)traindata[zloop])->str, count, zloop);
			traindata.RemoveAt(zloop);
			count++;
			}
		}

	str.Format("Cleared %d items", count);
	ShowProg(str);		
}

void CGestureDlg::DumpGestures()

{
	int len = traindata.GetSize();
	int count = 0;
	P2N("Dumping gestures:\r\n");

	for(int zloop = 0; zloop < len; zloop++)
		{
		P2N("Gesture %d '%s' %d items.\r\n", zloop, 
				((tdata*)traindata[zloop])->str, ((tdata*)traindata[zloop])->arr.GetSize());
		}	
}

void CGestureDlg::OnButton8() 

{
	int idx = m_list.GetCurSel();

	if(idx < 0)
		{
		xMsgBox("Please select an item to clear.", "Cannot Clear Gesture Item");
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
			//P2N("Clearing %s (%d) %d", ((tdata*)traindata[zloop])->str, count, zloop);
			traindata.RemoveAt(zloop);
			count++;
			break;
			}
		}
	str.Format("Cleared %d items", count);
	ShowProg(str);			

}

void CGestureDlg::OnButton9() 

{
	//P2N("Play Gesture\r\n");
	PlayGesture();
}


void CGestureDlg::PlayGesture()

{
	int idx;

	if((idx = m_list.GetCurSel()) < 0 )
		{
		xMsgBox("Please select an Item to play");
		return;
		}
	
	static int in_proc = 0;

	if(in_proc)
		return;

	in_proc = 1;		

	CString str;
	m_list.GetText(idx, str);
	
	CWordArray *arr = &normalarr;

	//if(trchecked)
		{
		int len = traindata.GetSize();
		int idx = 0;
		int minerr = 0x8000000;

		//P2N("Playing '%s'\r\n", str);
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
				ShowProg(str2);
				
				//P2N("\r\nDumping Train Data %s %x\r\n\r\n", ((tdata*)traindata[zloop])->str, ((tdata*)traindata[zloop])->item);
	
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
					//		  -- 0 +-
					//   270 ----|--- 90
					//       -+ 180 ++
					//////////////////////////////////////////////////////

					double delta = .6;
					
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

					//int move = false;
					//if(loop < xxarr.GetSize() - 1)
					//	{
					//	if(xxarr[loop] == 0 && yyarr[loop] == 0)
					//		move = true;
					//	}

					//if(move)
					//	pdc->MoveTo(xxarr[loop] + ABS(rc.left) + 40, yyarr[loop] + ABS(rc.top) + 40);
					//else //draw
					
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

				//InvalidateRect(&rc);
				Invalidate();

				support.YieldToWinEx();
				}
			}
		}

	Sleep(500);
	ShowProg("");
	Invalidate();
	in_proc = 0;
}

#if 0

static in_train;

	if(in_train)
		return;

	in_train = true;

	int len = traindata.GetSize();

	//P2N("Dump of training data:\r\n");

	// Dump train data
	//for(int zloop = 0; zloop < len; zloop++)
	//	{
	//	P2N("%s %x\r\n", ((tdata*)traindata[zloop])->str, ((tdata*)traindata[zloop])->item);
//
//		int ilen = ((tdata*)traindata[zloop])->arr.GetSize();
//
//		for(int iloop = 0; iloop < ilen; iloop++)
//			{
//			P2N("%3d  ", ((tdata*)traindata[zloop])->arr[iloop]);
//			}
//		P2N("\r\n");
//		}

	for(int tloop = 0; tloop < len; tloop++)
		{
		int	outarr[32];
		int item = ((tdata*)traindata[tloop])->item;
		
		// ---------------------------------------------------------------
		// 11111111 0000000 0000000 0000000 

		for(int oloop = 0; oloop < 4; oloop++)
			{
			unsigned int mask = 0xff000000 >> (oloop * 8);
			
			//P2N("mask - %x %x \r\n", mask, mask & item );
			gneunet->Byte2Bits( (mask & item) >> (24 - (8 * oloop)), 10000, 100, &outarr[oloop * 8]);	
			}
		
		//for(int dloop = 0; dloop < 32; dloop++)
		//	{
		//	P2N("%06d  ", outarr[dloop]);
		//	}
		
		//P2N("Training: %s %x\r\n", ((tdata*)traindata[tloop])->str, item);

		gneunet->TrainOneFromArray( &((tdata*)traindata[tloop])->arr, outarr, 32);

		//P2N("Ref: %s %x\r\n", ((tdata*)traindata[tloop])->str, item);
		//for(int dloop = 0; dloop < 32; dloop++)
		//	{
		//	P2N("%06d  ", outarr[dloop]);
		//	}

		//P2N("\r\nTrained: %s %x\r\n", ((tdata*)traindata[tloop])->str, item);
		//for(dloop = 0; dloop < 32; dloop++)
		//	{
		//	P2N("%06d  ", gneunet->GetOutVal(dloop));
		//	}

		//P2N("Deviation: %f\r\n", gneunet->SumDeviation(outarr, 32));
		}

	P2N("Ended training session\r\n");

	in_train = false;

#endif


void CGestureDlg::WriteNL(CxCFile *fp)

{
	fp->Write("\r\n", 2);
}

void CGestureDlg::SaveGestures(CString &fname)

{
	CxCFile fp; 	// CFile
	
	// Save new gestures to file
	CFileException theException;

	if(!fp.Open(fname, CFile::modeCreate | CFile::modeReadWrite,
				&theException))
		{
		CString tmp;
		tmp.Format("Error on saving file: '%s' -- %s", fname, 
							fp.TransExc(theException.m_cause));
		MBOX(tmp);	

		//P2N("Error on saving file: '%s' -- %s", fname, 
		//					fp.TransExc(theException.m_cause));
		return;
		}

	int len = traindata.GetSize();
	
	// Dump/Save train data
	for(int zloop = 0; zloop < len; zloop++)
		{
		int ilen = ((tdata*)traindata[zloop])->arr.GetSize();

		// Anything to write?
		if(ilen > 0)
			{
			CString tmp2; 

			//P2N("Saving gesture: '%s' size=%d\r\n", ((tdata*)traindata[zloop])->str, ilen);

			tmp2.Format("Gesture: %s\r\n", ((tdata*)traindata[zloop])->str);
			fp.WriteString(tmp2);
			//WriteNL(&fp);

			tmp2.Format("Length: %d\r\n", ilen);
			fp.WriteString(tmp2);
			//WriteNL(&fp);

			for(int iloop = 0; iloop < ilen; iloop++)
				{
				if(iloop > 0 && iloop % 10 == 0)
					{
					//P2N("\r\n");
					WriteNL(&fp);
					}
				CString str; 
				str.Format("%3d  ", ((tdata*)traindata[zloop])->arr[iloop]);
				//P2N(str);	
				fp.WriteString(str);
				}

			//P2N("\r\n");
			WriteNL(&fp);
			}
		}	

	changed = false;
}

void CGestureDlg::LoadGestures(CString &fname)

{
	// Load gesture file
	int state = 0, count = 0;

	//P2N("Loading gesture file: '%s'\r\n", fname);

	CxCFile fp;
	if(!fp.Open(fname, CFile::modeRead, NULL))
		{
		ShowProg("No gesture file");
		CString tmp; tmp.Format("Cannot open gesturefile: '%s'", fname);
		MBOX(tmp);

		// Was none loaded, so assume name anyway
		if(currfile == "")
			currfile = fname;

		loaded = true;
		return;
		}
			
	// Empty old training data
	traindata.RemoveAll();
	m_list.ResetContent();

	tdata *element;
	CString str, tmp2;
	int	 records = 0;
		
	while(true)
		{
		int ret = fp.ReadString(str);

		// EOF condition
		if(!ret)
			break;

		//P2N("line: '%s'\r\n", str);

		if(str.GetLength() == 0)
			continue;

		switch (state)	
			{
			case 0:
				if(str.Mid(0, 9) != "Gesture: ")
					{
					P2N("Invalid gesture file format (head)\r\n");
					// Keep state macine on 0 to seach for new head
					break;
					}
				tmp2 = str.Mid(9);
				//P2N("Loading gesture: '%s'\r\n", tmp2);

				// Add Unique element to list box
				if(m_list.FindString(0, tmp2) == LB_ERR)
					{
					//P2N("Duplicate element %s\r\n", tmp2);
					m_list.AddString(tmp2);
					}

				// Add it to array (always, so dups are in)
				element = new tdata;
				element->str = tmp2;
				traindata.Add(element);
				state = 1; count = 0;
				records = 0;
				break;

			case 1:
				if(str.Mid(0, 8) != "Length: ")
					{
					P2N("Invalid gesture file format (len)\r\n");
					//  Set state machine to 0 to seach for new head
					state = 0;
					break;
					}
				tmp2 = str.Mid(8);	records = atoi(tmp2);
				state = 2;

				//P2N("Record count: %d\r\n", records);

				// Sanity check, go fetch another record if false
				if(records == 0)
					state = 0;
				break;

			case 2:		
				//P2N("Adding to %s %s\r\n", 
				//	((tdata*)traindata[traindata.GetUpperBound()])->str, 
				//		str	);

				// Read in one line:
				for(int loop = 0; loop < 10; loop++)
					{
					//P2N("'%3d',", atoi(str.Mid(loop * 5, 3)));

					if(str.GetLength() > loop * 5)
						{
						((tdata*)traindata[traindata.GetUpperBound()])
							->arr.Add(atoi(str.Mid(loop * 5, 3)));
						}
					count++;
					}
				if(count >= records)
					{
					state = 0;		// Reset state machine
					}
				break;
			}
		}

	// Loaded file, reflect it
	currfile = fname;
	loaded = true;
}

//

void CGestureDlg::OnDestroy() 

{
	CGrayDlg::OnDestroy();
	
	//P2N("CGestureDlg::OnDestroy changed=%d %s\r\n", changed, currfile);
	if(changed)
		{
		if(xMsgBox("Gestures changed, would you like to save it?", "", true) == (int)true)
			{
			LoadSave(false, currfile);
			}
		}
}

void CGestureDlg::OnButton10() 

{
	CString str, dir = support.GetDirName(currfile);
	//P2N("chdir %s\r\n", dir);

	_chdir(dir);

	LoadSave(false, str);
	SetFocus();
}

void CGestureDlg::ShowProg(const char *str)

{
	m_prog.SetText(str);
	support.YieldToWinEx();

	// When str is empty, do not set the timer 
	if(str != "")
		SetTimer(3, 1500, NULL);
}

// Return true for checked button

int CGestureDlg::isTrainChecked()

{
	int tchecked = ( (CButton*)GetDlgItem(IDC_RADIO1) )->GetState();
	
	tchecked = tchecked & 0x03 ? true : false;

	return tchecked;
}

void CGestureDlg::OnRadio1() 

{
	if(isTrainChecked())
		ShowProg("Switched to training mode.");
}

void CGestureDlg::DoMagic()

{
	int loop =	0, xold = 0, yold = 0; 
	int len = xarr.GetSize();

	// Nothing to process
	if(len < 3)
		{
		busy = false;
		return;
		}

	if(!loaded)
		{
		busy = false;
		return;
		}	

	CWordArray anglearr;

	//////////////////////////////////////////////////////////////
	//
	//  x ---------->     180  -x -y  | NW | NE |  +x -y   270
	//  |             dy              |----|----|
	//  |    x ----->      90  -x +y  | SW | SE |  +x +y   0
	//  |    |
	//    dx
	//
	//////////////////////////////////////////////////////////////

	// New arrangement where up is zero

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
		int angle = 0;
		
		int dx = xarr[loop] - xold, dy = yarr[loop] - yold;

#if 0
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
				
				// Expand step simmetric
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
		//          dy
		
		// Get angle
		double dl = SQRT(SQR(dx) + SQR(dy));
		angle  =(int)((180.0 / MATHPI) * asin((double)abs(dy) / dl));
		
		//P2N("dx=%3d dy=%3d len=%2.2f rat=%2.2f sin=%2.2f ang=%3d\r\n",  
		//			dx, dy, dl, dy / dl, asin(dy / dl), angle);

		//if(loop % 2 == 0)
		//	P2N("\r\n");

		//   NW|NE
		//  ---|---
		//   SW|SE

		// Place quadrant
		if(dx == 0 && dy == 0) 
			{
			angle = 0;				// No movement
			}
		if(dx == 0) 
			{
			if(dy >= 0)
				{
				angle =	180;		//DOWN
				}
			else
				{
				angle =	0;			//UP	
				}
			}
		else if(dy == 0)
			{
			if(dx >= 0)
				{
				angle =	90;			//UP	
				}
			else
				{
				angle =	270;		//UP	
				}
			}
		else if(dx > 0)  // East
			{
			if(dy >= 0)
				angle = angle + 90;		// SE
			else
				angle = 90 - angle;		// NE
			}
		else		// West
			{
			if(dy >= 0)
				angle = 270 - angle;	// SW
			else
				angle = 270 + angle;	// NW
			}

		//P2N("angle=%d ", angle);

		anglearr.Add(angle);

		// Compensate for long strokes
		anglearr.Add(angle);

		double ll = dl - 1;
		while(ll > 0)
			{
			anglearr.Add(angle);
			ll -= 1;
			}		
	xold = 	xarr[loop]; yold = yarr[loop];
	}

	// CButton radio
	P2N("\r\nEnd Gesture %d items mode=%d\r\n",  xarr.GetSize(), isTrainChecked());

	if(len = anglearr.GetSize())
		{
		for(int zloop = 0; zloop < NORMALIZE_TO; zloop++)
			{
			int xloop = zloop * (len - 1) / NORMALIZE_TO;
			xloop = MIN(xloop, len - 1);
			//P2N("%3d  ", anglearr[xloop]);		
			normalarr.Add(anglearr[xloop]);
			}
		}

	if(isTrainChecked())
		{	
		Train();
		}
	else
		{
		// Recognize	
		Fuzzy(&normalarr);
		}

	xarr.RemoveAll(); yarr.RemoveAll();	normalarr.RemoveAll();
	addto = false;

	//busy = false;
	//SetCursor(LoadCursor(NULL, IDC_ARROW));
}

void CGestureDlg::OnLButtonUp(UINT nFlags, CPoint point)
 
{
	//SetCursor(LoadCursor(NULL, IDC_WAIT));
	//drag_x = point.x; drag_y = point.y;
  	//SynLButtonUp(nFlags, point);

	lbutton = false;
	SynLButtonUp(500);

	//addto = true;
	CGrayDlg::OnLButtonUp(nFlags, point);
}

void CGestureDlg::SynLButtonUp(int timeout)

{
	//P2N("CGestureDlg::SynLButtonUp lbutton=%d\r\n", lbutton);

	if(trigger)
		{
 		TRACKMOUSEEVENT te;
 
 		te.cbSize = sizeof(TRACKMOUSEEVENT);
 		te.dwFlags = TME_CANCEL;
 		te.hwndTrack = this->m_hWnd;
 		te.dwHoverTime = 0;
 
 		::_TrackMouseEvent(&te);

		trigger = false;
		}
 
 	in_draw = false;	

	// Anything to process?
	if(xarr.GetSize() > 3)
		{
		if(!lbutton)
			{
 			addto = true;
			//busy = true;
			//SetCursor(LoadCursor(NULL, IDC_WAIT));
			SetTimer(2, timeout, NULL);
			}
		}
}

//////////////////////////////////////////////////////////////////////////

void CGestureDlg::DumpMouArr()

{
	P2N("Dumping mouse array:\r\n");
	for(int loop = 1; loop < xarr.GetSize(); loop++)
		{
		P2N("  %3d %3d -> %3d %3d  ", xarr[loop], yarr[loop], 
							xarr[loop] - xarr[loop - 1],
								yarr[loop] - yarr[loop - 1]);
		if(loop % 5 == 0)
			P2N("\r\n");
		}
	P2N("\r\n");
}

BOOL CGestureDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 

{
	if(busy)
		{
		SetCursor(LoadCursor(NULL, IDC_WAIT));
		return true;
		}
	else
		{
		//SetCursor(LoadCursor(NULL, IDC_WAIT));
		//SetCursor(LoadCursor(NULL, IDC_ARROW));
		}
	return CGrayDlg::OnSetCursor(pWnd, nHitTest, message);
}

void CGestureDlg::Train()

{
	CString str;
	
	if(m_list.GetCurSel() >= 0 )
		{
		m_list.GetText(m_list.GetCurSel(), str);				
		}
		
	if(str != "")
		{
		changed = true;
		tdata *trd = new tdata;
		
		trd->str = str;
		trd->item = misc.hash_string(str);

		//trd->item = m_list.GetCurSel();			

		for(int zloop = 0; zloop < NORMALIZE_TO; zloop++)
			{
			trd->arr.Add(normalarr[zloop]);
			}

		// Add to training data
		//P2N("Train data added '%s' %x\r\n", str, misc.hash_string(str));

		traindata.Add(trd);

		CString tmp; tmp.Format("Added to '%s'", str);
		ShowProg(tmp);
		}
	else
		{
		ShowProg("Must select an item to train");
		}
}

void CGestureDlg::DumpAngleArray()

{
	//if(len)
		{
		//P2N("Dumping angle array\r\n");
		//
		//for(int oloop = 0; oloop < NORMALIZE_TO; oloop++)
		//	{
		//	P2N("%03d  ", normalarr[oloop]);		
		//	}
		}
}

