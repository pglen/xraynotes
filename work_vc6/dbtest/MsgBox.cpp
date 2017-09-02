// MsgBox.cpp : implementation file
//

#include "stdafx.h"
#include "dbtest.h"
#include "mxpad.h"

#include "MsgBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMsgBox dialog

CMsgBox::CMsgBox(CWnd* pParent /*=NULL*/)
	: CGrayDlg(CMsgBox::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMsgBox)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	drag		= 0;
	yesno		= YESOK;
	idret		= 0;
	tocount		= 0;
	timeout		= AUTOCLOSE;
}

void CMsgBox::OnCancel()

{
	//AfxMessageBox("Cancel");
}


void CMsgBox::OnOK()

{
	//AfxMessageBox("OK");
	//CGrayDlg::OnOK();

	if(yesno)
		{
		idret = IDYES;
		}
	else
		{
		idret = true;
		}
	EndDialog(idret);
}

void CMsgBox::DoDataExchange(CDataExchange* pDX)

{
	CGrayDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMsgBox)
	DDX_Control(pDX, ID_MBOK4, m_stop);
	DDX_Control(pDX, ID_MBOK3, m_no);
	DDX_Control(pDX, ID_MBOK2, m_yes);
	DDX_Control(pDX, IDOK, m_ok);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMsgBox, CGrayDlg)
	//{{AFX_MSG_MAP(CMsgBox)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_BN_CLICKED(ID_MBOK2, OnOk2)
	ON_BN_CLICKED(ID_MBOK3, OnOk3)
	ON_WM_CHAR()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_BN_CLICKED(ID_MBOK4, OnOk4)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMsgBox message handlers

BOOL CMsgBox::OnInitDialog() 

{
	CGrayDlg::OnInitDialog();
	
	//m_title		= "XrayNotes Prompt";

	//P2N("Messagebox showing with: '%s'\r\n", m_text);

#if 0
	m_bitmap.LoadBitmap(IDB_BITMAP7);

	BITMAP BitMap;
	m_bitmap.GetBitmap(&BitMap);
	m_nWidth  = BitMap.bmWidth;
	m_nHeight = BitMap.bmHeight;
#endif

	LOGFONT		m_lf;

	::GetObject((HFONT)GetStockObject(DEFAULT_GUI_FONT), sizeof(m_lf), &m_lf);
	m_lf.lfHeight = -14;
	m_font.CreateFontIndirect(&m_lf);

	m_lf.lfWeight  = FW_BOLD;
	m_lf.lfHeight = -16;
	m_fontb.CreateFontIndirect(&m_lf);

	RECT	rc; GetClientRect(&rc); 
	
	int adjust = false;

	// Collect text sizes	
	titlesss =  GetTextSize(m_header, &m_fontb);
	textsss	 =  GetTextSize(m_text, &m_font);

	// Horizontal
	if(titlesss.cx > rc.right - rc.left)
		{
		adjust = true;
		rc.right = rc.left + titlesss.cx;
		}
	if(textsss.cx > rc.right - rc.left)
		{
		adjust = true;
		rc.right = rc.left + textsss.cx;
		}
	
	// Vertical
	if(titlesss.cy + textsss.cy > (rc.bottom - rc.top) - 100)
		{
		adjust = true;
		rc.bottom = rc.top + titlesss.cy + textsss.cy + 100;
		}
	
	// Execute it
	if(adjust)
		{
		if(textsss.cx  > 800)
			{
			P2N("too big, reformatting string\r\n");
			}
		SetWindowPos(NULL, 0, 0, rc.right  + 50, rc.bottom - rc.top, SWP_NOMOVE | SWP_NOOWNERZORDER);
		}

	// Center and show buttons
	RECT	rc3; GetClientRect(&rc3);
	int		www = rc3.right - rc3.left;

	if(yesno == YESNO || yesno == MB_YESNO)
		{
		WINDOWPLACEMENT wp;
		m_no.GetWindowPlacement(&wp);

		int ww = wp.rcNormalPosition.right - wp.rcNormalPosition.left;
		int hh = wp.rcNormalPosition.bottom  - wp.rcNormalPosition.top;

		m_no.SetWindowPos(NULL, (rc3.right - rc3.left)/2 + 4 , 		
				rc3.bottom - (hh + 10), 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);

		m_yes.GetWindowPlacement(&wp);

		ww = wp.rcNormalPosition.right - wp.rcNormalPosition.left;
		hh = wp.rcNormalPosition.bottom  - wp.rcNormalPosition.top;

		m_yes.SetWindowPos(NULL, (rc3.right - rc3.left)/2 - (ww + 4) , 		
				rc3.bottom - (hh + 10), 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);
	
		m_ok.ShowWindow(false);
		m_stop.ShowWindow(false);
		}
	else if(yesno == YESNOSTOP)
		{
		WINDOWPLACEMENT wp;
		m_no.GetWindowPlacement(&wp);

		// No in Middle
		int ww = wp.rcNormalPosition.right - wp.rcNormalPosition.left;
		int hh = wp.rcNormalPosition.bottom  - wp.rcNormalPosition.top;

		m_no.SetWindowPos(NULL, www/2 - ww/2 , 		
				rc3.bottom - (hh + 10), 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);

		// Yes on left
		WINDOWPLACEMENT wp2;
		m_yes.GetWindowPlacement(&wp2);

		int ww2 = wp2.rcNormalPosition.right - wp2.rcNormalPosition.left;
		int hh2 = wp2.rcNormalPosition.bottom  - wp2.rcNormalPosition.top;

		m_yes.SetWindowPos(NULL, www/2 - (ww2 + 4 +  ww/2) , 		
				rc3.bottom - (hh + 10), 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);

		// Stop on right
		m_stop.GetWindowPlacement(&wp2);

		ww2 = wp2.rcNormalPosition.right - wp2.rcNormalPosition.left;
		hh2 = wp2.rcNormalPosition.bottom  - wp2.rcNormalPosition.top;

		m_stop.SetWindowPos(NULL, www/2 + (ww/2 + 4) , 		
				rc3.bottom - (hh + 10), 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);

		m_ok.ShowWindow(false);
		}
	else
		{
		WINDOWPLACEMENT wp;
		m_ok.GetWindowPlacement(&wp);

		int ww = wp.rcNormalPosition.right - wp.rcNormalPosition.left;
		int hh = wp.rcNormalPosition.bottom  - wp.rcNormalPosition.top;

		//GetClientRect(&rc);

		m_ok.SetWindowPos(NULL, (rc3.right - rc3.left)/2 - ww/2 , 		
				rc3.bottom - (hh + 10), 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);

		m_stop.ShowWindow(false);
		m_yes.ShowWindow(false);
		m_no.ShowWindow(false);
		}

	// Set def button CDialog
	switch(idret)
		{
		case 0:
			SetDefID(ID_MBOK3);
			GetDlgItem(ID_MBOK3)->SetFocus();
			Invalidate();
			break;

		case 1:
			SetDefID(ID_MBOK2);
			GetDlgItem(ID_MBOK2)->SetFocus();
			Invalidate();
			break;

		default:
			SetDefID(ID_MBOK4);
			GetDlgItem(ID_MBOK4)->SetFocus();
			Invalidate();
			break;
		}

	// No choice default on OK
	if (yesno == YESOK)
		SetDefID(IDOK);

	SetTimer(1, 1000, NULL);

	return TRUE;  
}

//////////////////////////////////////////////////////////////////////////
// Override to provide enddialog

void CMsgBox::OnLButtonDown(UINT nFlags, CPoint point) 

{
	if(closerect.PtInRect(point))
		{
		
		P2N("CMsgBox::OnLButtonDown - Click in Close Box\r\n");
		//EndDialog(IDOK);
		
		//if(yesno == YESNO)
		//		ret = false;
		//	if(yesno == YESNOSTOP)
		//		ret = -1;
		//	else
		//		ret = false;
		//	

		EndDialog(idret);
		return;
		}

	// Reset timer
	tocount = AUTOWARN;

	CGrayDlg::OnLButtonDown(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////

void CMsgBox::OnLButtonUp(UINT nFlags, CPoint point) 

{
#if 0
	drag_x = point.x; drag_y = point.y;

	//focus = false;
	if(drag)
		{
		drag = 0; 
		ReleaseCapture();
		InvalidateRect(NULL, TRUE);
		}	
#endif

	CGrayDlg::OnLButtonUp(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////

void CMsgBox::OnMouseMove(UINT nFlags, CPoint point) 

{

#if 0

	RECT rc;
	if(drag)
		{
		GetWindowRect(&rc);
				SetWindowPos( NULL, (point.x +  rc.left) - drag_x , 
						(point.y + rc.top) - drag_y, 0, 0, 
								SWP_NOSIZE | SWP_NOZORDER ) ;  
		goto endd;
		}

	// Moved?
	if(drag_x != point.x || drag_y != point.y)
		{
		// Is drag treshold?

		if((abs(drag_x - point.x)  > 3 ) || (abs(drag_y - point.y) > 3) )
			{
			//P2N("Moved\r\n");

			drag_x = point.x;
			drag_y = point.y;

			// Button?
			if(nFlags & MK_LBUTTON)
				{
				SetCapture();
				drag = 1;
				}
			}
		goto endd;
		}
#endif

//endd:	
	CGrayDlg::OnMouseMove(nFlags, point);
}

BOOL CMsgBox::OnEraseBkgnd(CDC* pDC) 

{
#if 0
	RECT rect,m_rDataBox;
	
	GetClientRect(&rect);
	CopyRect(&m_rDataBox,&rect);

	//CBrush NewBrush(0x00555555) ; 
	CBrush NewBrush(RGB(64, 64, 64)) ; 
	pDC->SetBrushOrg(0,0) ;
	CBrush* pOldBrush = (CBrush*)pDC->SelectObject(&NewBrush);   
	pDC->PatBlt(rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top ,PATCOPY);
	pDC->SelectObject(pOldBrush);

	CDC  dcMem;
	dcMem.CreateCompatibleDC(pDC);
	
	CBitmap *pOldBitmap  =  dcMem.SelectObject(&m_bitmap);
	CClientDC dc(this);

	int xx = 0, yy = 0, ww = rect.right-rect.left, hh = rect.bottom-rect.top;

	int loopx = 0, loopy = 0;

	for (loopy = 0; loopy < hh; loopy += m_nHeight)
		{
		for (loopx = 0; loopx < ww; loopx += m_nWidth)
			{
			pDC->BitBlt(loopx, loopy, m_nWidth, m_nHeight, &dcMem, 0,0, SRCCOPY ); 		
			}
		}

	dcMem.SelectObject(pOldBitmap);

	return true;
#endif

	return CGrayDlg::OnEraseBkgnd(pDC); 
}

//////////////////////////////////////////////////////////////////////////

void CMsgBox::OnPaint() 

{
	CGrayDlg::OnPaint();

	//CPaintDC dc(this); // device context for painting
	CClientDC dc(this);
	
	dc.SetBkMode(TRANSPARENT);

	RECT	rc;
	GetClientRect(&rc);

	// Vert
	RECT	rc3;
	GetClientRect(&rc3);

	//dc.SetTextColor(m_crText);
	dc.SetBkMode(TRANSPARENT);

	// Header
	RECT rc4 = rc; 
	rc4.top = 24;
	CFont* oldFont = dc.SelectObject(&m_fontb);
	dc.DrawText(m_header, m_header.GetLength(), &rc4, DT_CENTER);
	dc.SelectObject(oldFont);

	// Text
	RECT rc2 = rc;  rc2.top = 52;
	oldFont = dc.SelectObject(&m_font);
	dc.DrawText(m_text, m_text.GetLength(), &rc2, DT_CENTER);
	
	// Autoclose
	RECT rc5 = rc; rc5.top = rc5.bottom - 56;
	dc.DrawText(m_timer, m_timer.GetLength(), &rc5, DT_CENTER);

	//////////////////////////////////////////////////////////////////////
	dc.SelectObject(oldFont);
}

//////////////////////////////////////////////////////////////////////////

CSize CMsgBox::GetTextSize(const char *str, CFont *font)

{
	CPaintDC dc(this); // device context for painting
	
	//P2N("GetTextSize(): Sizing: '%s'\r\n", str);

	CFont *oldFont = dc.SelectObject(font);

	CSize sst(0,0), sst2(0, 0), sss(0, 0);
	CString tmp, text2 = str;

	int flen = text2.GetLength();
	int pos = 0, start = 0, count = 0, text_lines = 0, oneheight = 0;	
	int	ww = 0, hh = 0;
	
	// Draw box to this size ....
	while(true)
		{
		pos = text2.Find("\r", pos);
		if(pos < 0)
			{
			tmp = text2.Mid(start, flen - start);
			sst2 = dc.GetTextExtent(tmp);
			ww =  max(sst2.cx, ww);
			oneheight = max(oneheight, sst2.cy);

			if(!sst2.cy)
				sst2.cy = oneheight;

			hh += sst2.cy;

			//P2N("GetTextSize() last line: ww=%d hh=%d - text2='%s'\r\n", sst2.cx, sst2.cy, tmp);

			break;
			}
		text_lines++;

		// Exclude new line
		tmp = text2.Mid(start, (pos - start));

		pos++; start = pos;
		sst = dc.GetTextExtent(tmp);

		oneheight = max(oneheight, sst.cy);

		ww =  max(sst.cx, ww);

		if(!sst.cy)
			sst.cy = oneheight;

		hh += sst.cy;

		//P2N("GetTextSize() line: ww=%d hh=%d - text1='%s'\r\n", sst.cx, sst.cy, tmp);

		//if(count ++ > 50)
		//	break;
		}

	dc.SelectObject(oldFont);

	//P2N("GetTextSize(): ww=%d hh=%d\r\n", ww, hh);
		
	sss.cx = ww; sss.cy = hh;

	return(sss);
}

void CMsgBox::OnOk2() 

{
	if(yesno)
		{
		idret = IDYES;
		}
	else
		{
		idret = true;
		}

	EndDialog(idret);
}

void CMsgBox::OnOk3() 
{
	if(yesno)
		{
		idret = IDNO;
		}
	else
		{
		idret = false;
		}
	EndDialog(idret);
}

void CMsgBox::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	P2N("	onchar %d\r\n", nChar);
	CGrayDlg::OnChar(nChar, nRepCnt, nFlags);
}


BOOL CMsgBox::PreTranslateMessage(MSG* pMsg) 

{
	if(pMsg->message == WM_KEYDOWN)
		{
		//P2N("mag pretrans keydown %c (%d) %x\r\n", pMsg->wParam, pMsg->wParam, pMsg->lParam);

		if(pMsg->wParam == 27)
			{
			P2N("CMsgBox::PreTranslateMessage Esc Pressed\r\n");
			EndDialog(false);
			}
		}

	if(pMsg->message == WM_CHAR)
		{
		if(tolower(pMsg->wParam)  == 'n')
			{
			EndDialog(IDNO);
			}

		if(tolower(pMsg->wParam) == 'y')
			{
			EndDialog(IDYES);
			}
		}
	
	return CGrayDlg::PreTranslateMessage(pMsg);
}

int CMsgBox::DoModal() 

{
	// Break up long line into segments
	

	//((CShadesApp*)AfxGetApp())->inmodal = true;
	int ret = CGrayDlg::DoModal();
	//((CShadesApp*)AfxGetApp())->inmodal = false;
	return ret;
}


void CMsgBox::OnTimer(UINT nIDEvent) 

{	
	if(tocount++ > timeout)
		{
		EndDialog(IDOK);
		}
	if(tocount > AUTOWARN)
		{
		m_timer.Format("AutoClosing Dialog in %d sec", (timeout + 2) - tocount);

		RECT rc5; GetClientRect(&rc5);

		// Calculate Invalidate 
		rc5.top = rc5.bottom - 56; rc5.bottom = rc5.top + 20;

		InvalidateRect(&rc5);
		}
	else
		{
		}

	CGrayDlg::OnTimer(nIDEvent);
}

void CMsgBox::OnSize(UINT nType, int cx, int cy) 
{
	CGrayDlg::OnSize(nType, cx, cy);
}


void CMsgBox::OnOk4() 

{
	idret = -1;
	EndDialog(idret);
}

void CMsgBox::OnShowWindow(BOOL bShow, UINT nStatus) 

{
	switch(idret)
		{
		case 0:
			GetDlgItem(ID_MBOK3)->SetFocus();
			break;

		case 1:
			GetDlgItem(ID_MBOK2)->SetFocus();
			break;

		default:
			GetDlgItem(ID_MBOK4)->SetFocus();
			break;
		}
	CGrayDlg::OnShowWindow(bShow, nStatus);
}

int		xMsgBox(const char *str, const char *title, int yesno, int defa) 

{
	CMsgBox mb;	

	if(title == NULL || *title == '\0')
		mb.m_header = "XrayNotes MessageBox";
	else
		mb.m_header = title;

	mb.m_text = str;
	mb.yesno = yesno;
	mb.idret = defa;

	return mb.DoModal();
}
