// xScroll.cpp : implementation file
//

#include "stdafx.h"
#include "xraynotes.h"

#include "yScroll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CyScroll dialog

CyScroll::CyScroll(CWnd* pParent /*=NULL*/)
	: CGrayDlg(CyScroll::IDD, pParent)
{
	//{{AFX_DATA_INIT(CyScroll)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	noheader = true;
	windowname = "CyScroll"; //support.GetFileNameExt(__FILE__);
}

void CyScroll::DoDataExchange(CDataExchange* pDX)
{
	CGrayDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CyScroll)
	DDX_Control(pDX, IDC_SCROLLBAR1, m_scroll);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CyScroll, CGrayDlg)
	//{{AFX_MSG_MAP(CyScroll)
	ON_WM_MOUSEMOVE()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_NCLBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_NCHITTEST()
	ON_WM_NCMOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_NCRBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CyScroll message handlers

BOOL CyScroll::OnInitDialog()

{
	CGrayDlg::OnInitDialog();

	m_scroll.SetScrollRange(0, 100);
	m_scroll.SetScrollPos(50);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CyScroll::OnMouseMove(UINT nFlags, CPoint point)

{
		//P2N("CxScroll::OnMouseMove %d %d\r\n", point.x, point.y);
	if(sizing)
		{
		ClientToScreen(&point);

		//AP2N("CxScroll::OnMouseMove sizing: xx=%d yy=%d\r\n", point.x, point.y);

		RECT rect;
		GetWindowRect(&rect);

		MINMAXINFO mmi; 
		mmi.ptMinTrackSize.x = 10;
		mmi.ptMaxTrackSize.x = 32000; 

		mmi.ptMinTrackSize.y = 10;
		mmi.ptMaxTrackSize.y = 32000; 

		OnGetMinMaxInfo(&mmi);

		rect.right = point.x; rect.bottom = point.y;

		//////////////////////////////////////////////////////////////////
		if(rect.right - rect.left < mmi.ptMinTrackSize.x)
			{
			rect.right =  rect.left + mmi.ptMinTrackSize.x;
			}
		if(rect.bottom - rect.top < mmi.ptMinTrackSize.y)
			{
			rect.bottom =  rect.top + mmi.ptMinTrackSize.y;
			}

		//////////////////////////////////////////////////////////////////
		if(rect.right - rect.left > mmi.ptMaxTrackSize.x)
			{
			rect.right =  rect.left + mmi.ptMaxTrackSize.x;
			}
		if(rect.bottom - rect.top > mmi.ptMaxTrackSize.y)
			{
			rect.bottom =  rect.top + mmi.ptMaxTrackSize.y;
			}
		MoveWindow(&rect);
		}
	CGrayDlg::OnMouseMove(nFlags, point);
}

BOOL CyScroll::PreTranslateMessage(MSG* pMsg)

{
	if(pMsg->message == WM_CHAR)
		{
		((CShadesDlg* )parent)->SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
		P2N("CyScroll::PreTranslateMessage WM_CHAR %c\r\n", pMsg->wParam);
		return(0);
		}

	if(pMsg->message == WM_KEYDOWN)
		{
		switch(pMsg->wParam)
			{
			case VK_HOME:
				m_scroll.SetScrollPos(0);
				((CShadesDlg* )parent)->vscroll = m_scroll.GetScrollPos();
				((CShadesDlg* )parent)->helper->OnDocChange();
				((CShadesDlg* )parent)->Invalidate(false);
				break;

			case VK_END:
				m_scroll.SetScrollPos(100);
				((CShadesDlg* )parent)->vscroll = m_scroll.GetScrollPos();
				((CShadesDlg* )parent)->helper->OnDocChange();
				((CShadesDlg* )parent)->Invalidate(false);
				break;

			default:
				((CShadesDlg* )parent)->SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
				//P2N("CyScroll::mag pretrans keydown %c (%d) %x\r\n", pMsg->wParam, pMsg->wParam, pMsg->lParam);
			}
		return(0);
		}

	return CGrayDlg::PreTranslateMessage(pMsg);
}

void CyScroll::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)

{
	static UINT old_pos = 0;

	//P2N("CyScroll::Ver scroll code=%d pos=%d\r\n", nSBCode, nPos);

	switch(nSBCode)
	{
	case SB_THUMBTRACK   :
		
		//P2N("CyScroll::thumb track\r\n", nPos);

		m_scroll.SetScrollPos(nPos);
		if(ABS((int)(old_pos - nPos)) > 3 || nPos == 0 || nPos == 100)
			{
			old_pos = nPos;
			//P2N("CyScroll::thumb track invalidate\r\n", nPos);
			((CShadesDlg* )parent)->vscroll = m_scroll.GetScrollPos();
			((CShadesDlg* )parent)->helper->OnDocChange();
			((CShadesDlg* )parent)->Invalidate(false);
			}
		m_scroll.SetScrollPos(nPos);
	break;

	case SB_THUMBPOSITION   :
		P2N("CyScroll::thumbpos\r\n", nPos);
		m_scroll.SetScrollPos(nPos);
	break;

	case SB_LINELEFT:
		P2N("CyScroll::line up\r\n", nPos);
		m_scroll.SetScrollPos(m_scroll.GetScrollPos() - 1);
	break;

	case SB_LINERIGHT   :
			P2N("CyScroll::line down\r\n", nPos);
		m_scroll.SetScrollPos(m_scroll.GetScrollPos() + 1);
	break;

	case SB_PAGELEFT   :
			P2N("CyScroll::page up\r\n", nPos);
		m_scroll.SetScrollPos(m_scroll.GetScrollPos() - 10);
	break;

	case SB_PAGERIGHT   :
		P2N("CyScroll::page down\r\n", nPos);
		m_scroll.SetScrollPos(m_scroll.GetScrollPos() + 10);
	break;

	case	SB_ENDSCROLL  :
		//P2N("CyScroll::End Scroll at %d\r\n", m_scroll.GetScrollPos());
		((CShadesDlg* )parent)->vscroll = m_scroll.GetScrollPos();
		((CShadesDlg* )parent)->helper->OnDocChange();
		((CShadesDlg* )parent)->Invalidate(false);
		break;
	}

	CGrayDlg::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CyScroll::ExtMouseWheel(UINT nFlags, short zDelta, CPoint pt) 

{
	return OnMouseWheel(nFlags, zDelta, pt); 
}

BOOL CyScroll::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 

{
	//P2N("CyScroll::Mouse wheel yscroll flags=%d delta=%d px=%d py=%d\r\n", nFlags, zDelta, pt.x, pt.y);

	int pos = m_scroll.GetScrollPos();
	if(zDelta < 0)
		pos += 10;
	else
		pos -= 10;

	m_scroll.SetScrollPos(pos);
			
	((CShadesDlg* )parent)->vscroll = m_scroll.GetScrollPos();
	((CShadesDlg* )parent)->helper->OnDocChange();
	((CShadesDlg* )parent)->Invalidate(false);

	if( ((CShadesDlg* )parent)->current >= 0)
		{
		CXrayDoc *doc = ((CXrayDoc*) ((CShadesDlg* )parent)->docarr[((CShadesDlg* )parent)->current]); 
		if(doc)
			{
			if(doc->showstate == STATE_VARI)
				{
				doc->finestate = false;
				doc->NewRender();
				}
			}
		}
	
	return CGrayDlg::OnMouseWheel(nFlags, zDelta, pt);
}

//DEL void CyScroll::OnLButtonDblClk(UINT nFlags, CPoint point) 
//DEL {
//DEL 	//ShowWindow(SW_HIDE);	
//DEL 	CGrayDlg::OnLButtonDblClk(nFlags, point);
//DEL }


void CyScroll::MoveWidth(int want_x, int want_y, int want_h)

{
	WINDOWPLACEMENT wp; GetWindowPlacement(&wp);
	SetWindowPos( NULL, want_x, want_y, wp.rcNormalPosition.right - wp.rcNormalPosition.left, want_h, SWP_NOZORDER );
}

//////////////////////////////////////////////////////////////////////////

void CyScroll::OnSize(UINT nType, int cx, int cy) 

{
	CGrayDlg::OnSize(nType, cx, cy);
	
	if(::IsWindow(m_hWnd))
		{
		WINDOWPLACEMENT wp;
		GetWindowPlacement(&wp);

		if(::IsWindow(m_scroll.m_hWnd))
			{
			WINDOWPLACEMENT wp2;

			m_scroll.GetWindowPlacement(&wp2);

			ScreenToClient(&wp.rcNormalPosition);
			//wp2.rcNormalPosition.right - wp2.rcNormalPosition.left

			m_scroll.SetWindowPos(NULL, 
				wp.rcNormalPosition.left + 14, wp.rcNormalPosition.top + 24, 
					wp.rcNormalPosition.right - wp.rcNormalPosition.left - 28, 
						wp.rcNormalPosition.bottom - wp.rcNormalPosition.top - 38, SWP_NOZORDER  );  
			}
		}
}

//////////////////////////////////////////////////////////////////////////

void CyScroll::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 

{
	lpMMI->ptMinTrackSize.x = 45;
	lpMMI->ptMaxTrackSize.x = 80;
	lpMMI->ptMinTrackSize.y = 130;

	CGrayDlg::OnGetMinMaxInfo(lpMMI);
}

void CyScroll::OnNcLButtonUp(UINT nHitTest, CPoint point) 
{
	if(sizing)
		{
		P2N("CxScroll::Sizing off\r\n");
		ReleaseCapture();
		sizing = false;
		}	
	CGrayDlg::OnNcLButtonUp(nHitTest, point);
}

void CyScroll::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CGrayDlg::OnLButtonDown(nFlags, point);
}

UINT CyScroll::OnNcHitTest(CPoint point) 
{
int ret;
	RECT corner, lcorner, rect; GetWindowRect(&rect);

	//P2N("CxScroll::OnNcHitTest %d xx=%d yy=%d xxx=%d yyy=%d\r\n", 
	//		nchit, point.x, point.y, rect.right, rect.bottom);
	
	corner.left = rect.right - 10; corner.right = rect.right + 10;
	corner.top = rect.bottom - 10; corner.bottom = rect.bottom + 10;

	lcorner.left = rect.left - 10; lcorner.right = rect.left + 10;
	lcorner.top = rect.bottom - 10; lcorner.bottom = rect.bottom + 10;

	if(::PtInRect(&corner, point))
		{
		ret = HTBOTTOMRIGHT;
		}
	else if(::PtInRect(&lcorner, point))
		{
		ret = HTBOTTOMLEFT;
		}
	else
		{
		// Get a clue where it is at according to W32
		ret = CGrayDlg::OnNcHitTest(point);
		}
	return ret;
	
	//return CGrayDlg::OnNcHitTest(point);
}

void CyScroll::OnNcMouseMove(UINT nHitTest, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CGrayDlg::OnNcMouseMove(nHitTest, point);
}

BOOL CyScroll::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if(nHitTest == HTBOTTOMRIGHT)
		{
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENWSE));
		return true;
		}
	if(nHitTest == HTBOTTOMLEFT)
		{
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENESW));
		return true;
		}
	else
		return CGrayDlg::OnSetCursor(pWnd, nHitTest, message);
	
	//return CGrayDlg::OnSetCursor(pWnd, nHitTest, message);
}

void CyScroll::OnNcRButtonDown(UINT nHitTest, CPoint point) 
{
	if(nHitTest == HTBOTTOMRIGHT)
		{
		ncold = point;
		ScreenToClient(&ncold);
		P2N("CxScroll::Sizing on\r\n");
		sizing = true;
		SetCapture();
		}	
	CGrayDlg::OnNcRButtonDown(nHitTest, point);
}

void CyScroll::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(sizing)
		{
		P2N("CxScroll::OnLButtonUp Sizing off\r\n");
		ReleaseCapture();
		sizing = false;
		ShowWindow(SW_HIDE);
		support.YieldToWinEx();
		ShowWindow(SW_SHOW);
		}	

	CGrayDlg::OnLButtonUp(nFlags, point);
}
