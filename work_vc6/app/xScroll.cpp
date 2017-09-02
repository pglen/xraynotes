
/* =====[ xScroll.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the xScroll.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  11/26/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// xScroll.cpp : implementation file
//

#include "stdafx.h"
#include "xraynotes.h"

#include "xScroll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CxScroll dialog


/*! 
  	\page dialog_keystrokes Dialog Specific Keyboard Shortcuts	

  		Keystrokes and hotkeys description for individual dialogs

	\section Scroll Dialog (Horizontal/Vertical)

  	<table>
	<tr><th>Key<th>Purpose<th>Notes
	<tr><td>Home<td>Move to beginning<td>(Vertical: upmost Horizontal: Leftmost
	<tr><td>End<td>Move to end<td>(Vertical: bottom Horizontal: Rigthtmost
	<tr><td>End<td>Move one line up<td>(Vertical: Up Arrow Horizontal: Up Arrow
	</table>

*/


CxScroll::CxScroll(CWnd* pParent /*=NULL*/)
	: CGrayDlg(CxScroll::IDD, pParent)
{
	//{{AFX_DATA_INIT(CxScroll)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	//want_x = 0, want_y = 0; created = false;

	old_focus = NULL;
	noheader = true;
	sizing = false;
	windowname = "CxScroll"; //support.GetFileNameExt(__FILE__);
}

void CxScroll::DoDataExchange(CDataExchange* pDX)
{
	CGrayDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CxScroll)
	DDX_Control(pDX, IDC_SCROLLBAR1, m_scroll);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CxScroll, CGrayDlg)
	//{{AFX_MSG_MAP(CxScroll)
			ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_HSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_NCHITTEST()
	ON_WM_SETCURSOR()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCLBUTTONUP()
	ON_WM_NCMOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CxScroll message handlers


BOOL CxScroll::OnInitDialog() 

{
	CGrayDlg::OnInitDialog();

	m_scroll.SetScrollRange(0, 100);
	m_scroll.SetScrollPos(50);

	// reflect

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CxScroll::OnLButtonUp(UINT nFlags, CPoint point) 

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

void CxScroll::OnMouseMove(UINT nFlags, CPoint point) 

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

BOOL CxScroll::PreTranslateMessage(MSG* pMsg) 

{
	//P2N("xScroll PreTranslatetrans %x %s\r\n", pMsg->message, mxpad.num2msg(pMsg->message) );

	if(pMsg->message == WM_CHAR)
		{
		((CShadesDlg* )parent)->SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
		P2N("CxScroll::PreTranslateMessage WM_CHAR %c\r\n", pMsg->wParam);
		return(0);
		}

	if(pMsg->message == WM_KEYDOWN)
		{
		switch(pMsg->wParam)
			{
			case VK_HOME:
				m_scroll.SetScrollPos(0);
				((CShadesDlg* )parent)->hscroll = m_scroll.GetScrollPos();
				((CShadesDlg* )parent)->Invalidate(false);
				break;

			case VK_END:
				m_scroll.SetScrollPos(100);
				((CShadesDlg* )parent)->hscroll = m_scroll.GetScrollPos();
				((CShadesDlg* )parent)->Invalidate(false);
				break;

			default:
				((CShadesDlg* )parent)->SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
				//P2N("CxScroll:: PreTranslatetrans keydown %c (%d) %x\r\n", pMsg->wParam, pMsg->wParam, pMsg->lParam);
			}
		return(0);
		}

#if 0
	if(pMsg->message == WM_MOUSELEAVE)
		{
		//((CShadesDlg* )parent)->SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);

		P2N("CxScroll mouseleave.\r\n");

		if(old_focus)
			{
			::SetFocus(old_focus->m_hWnd);
			old_focus = NULL;
			}

		//CPoint pt;
		//CGestureDlg::OnLButtonUp(0, pt); 
		//return(0);
		}
#endif

	return CGrayDlg::PreTranslateMessage(pMsg);
}

void CxScroll::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 

{
	static UINT old_pos = 0;

	//P2N("Hor scroll code=%d pos=%d\r\n", nSBCode, nPos); 	

	switch(nSBCode)
	{
	case SB_THUMBTRACK   :
		//P2N("CxScroll::thumb track\r\n", nPos);
		m_scroll.SetScrollPos(nPos);
		if(ABS((int)(old_pos - nPos)) > 3)
			{
			old_pos = nPos;
			//P2N("thumb track invalidate\r\n", nPos);
			((CShadesDlg* )parent)->hscroll = m_scroll.GetScrollPos();
			((CShadesDlg* )parent)->helper->OnDocChange();
			((CShadesDlg* )parent)->Invalidate(false);
			}
	break;

	case SB_THUMBPOSITION   :
		P2N("CxScroll::thumbpos left\r\n", nPos);
		m_scroll.SetScrollPos(nPos);
	break;

	case SB_LINELEFT:   
		P2N("CxScroll::line left\r\n", nPos);
		m_scroll.SetScrollPos(m_scroll.GetScrollPos() - 1);
	break;

	case SB_LINERIGHT   :
			P2N("CxScroll::line right\r\n", nPos);
		m_scroll.SetScrollPos(m_scroll.GetScrollPos() + 1);
	break;
	
	case SB_PAGELEFT   :
			P2N("CxScroll::page left\r\n", nPos);
		m_scroll.SetScrollPos(m_scroll.GetScrollPos() - 10);
	break;
	
	case SB_PAGERIGHT   :
		P2N("CxScroll::page right\r\n", nPos);
		m_scroll.SetScrollPos(m_scroll.GetScrollPos() + 10);
	break;

	case	SB_ENDSCROLL  :
		//P2N("CxScroll::End Scroll at %d\r\n", m_scroll.GetScrollPos());
		((CShadesDlg* )parent)->hscroll = m_scroll.GetScrollPos();
		((CShadesDlg* )parent)->helper->OnDocChange();
		((CShadesDlg* )parent)->Invalidate(false);
		break;
	}

	CGrayDlg::OnHScroll(nSBCode, nPos, pScrollBar);
}

BOOL CxScroll::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 

{
	//P2N("CxScroll::Mouse wheel xcroll flags=%d delta=%d px=%d py=%d\r\n", nFlags, zDelta, pt.x, pt.y);

	int pos = m_scroll.GetScrollPos();
	if(zDelta < 0)
		pos += 10;
	else
		pos -= 10;

	m_scroll.SetScrollPos(pos);
			
	((CShadesDlg* )parent)->hscroll = m_scroll.GetScrollPos();
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


void CxScroll::OnSize(UINT nType, int cx, int cy) 

{
	CGrayDlg::OnSize(nType, cx, cy);
	
	//P2N("CxScroll::OnSize\r\n");

	if(::IsWindow(m_hWnd))
		{
		WINDOWPLACEMENT wp;
		GetWindowPlacement(&wp);

		if(::IsWindow(m_scroll.m_hWnd))
			{
			WINDOWPLACEMENT wp2;

			//SetFocus();
			m_scroll.GetWindowPlacement(&wp2);

			ScreenToClient(&wp.rcNormalPosition);
			//wp2.rcNormalPosition.right - wp2.rcNormalPosition.left

			m_scroll.SetWindowPos(NULL, 
				wp.rcNormalPosition.left + 14, wp.rcNormalPosition.top + 14, 
					wp.rcNormalPosition.right - wp.rcNormalPosition.left - 38, 
						wp.rcNormalPosition.bottom - wp.rcNormalPosition.top - 28, SWP_NOZORDER  );  

			//m_scroll.SetFocus();
			}
		}		
}

void CxScroll::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 

{
	//P2N("CxScroll::OnGetMinMaxInfo\r\n");

	lpMMI->ptMinTrackSize.y = 40;
	lpMMI->ptMaxTrackSize.y = 80;
	lpMMI->ptMinTrackSize.x = 130;	
	
	CGrayDlg::OnGetMinMaxInfo(lpMMI);
}


UINT CxScroll::OnNcHitTest(CPoint point) 

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
}

BOOL CxScroll::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 

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
}

void CxScroll::OnNcLButtonDown(UINT nHitTest, CPoint point) 

{
	if(nHitTest == HTBOTTOMRIGHT)
		{
		ncold = point;
		ScreenToClient(&ncold);
		P2N("CxScroll::Sizing on\r\n");
		sizing = true;
		SetCapture();
		}
	CGrayDlg::OnNcLButtonDown(nHitTest, point);
}

void CxScroll::OnNcLButtonUp(UINT nHitTest, CPoint point) 

{
	if(sizing)
		{
		P2N("CxScroll::Sizing off\r\n");
		ReleaseCapture();
		sizing = false;
		}	
	CGrayDlg::OnNcLButtonUp(nHitTest, point);
}

void CxScroll::OnNcMouseMove(UINT nHitTest, CPoint point) 
{
	//P2N("CxScroll::OnNcMouseMove(hit=%d  xx=%d yy=%d\r\n", nHitTest, point.x, point.y);
	CGrayDlg::OnNcMouseMove(nHitTest, point);
}
