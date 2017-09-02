// xScrollBar.cpp : implementation file
//

#include "stdafx.h"
#include "xScrollBar.h"
#include "mxpad.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CxScrollBar

CxScrollBar::CxScrollBar()
{
	P2N("CxScrollBar::CxScrollBar\r\n");
}

CxScrollBar::~CxScrollBar()
{
}


BEGIN_MESSAGE_MAP(CxScrollBar, CScrollBar)
	//{{AFX_MSG_MAP(CxScrollBar)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CxScrollBar message handlers

void CxScrollBar::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	P2N("CxScrollBar::OnHScroll\r\n");
	
	CScrollBar::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CxScrollBar::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	P2N("CxScrollBar::OnVScroll\r\n");	
	
	CScrollBar::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CxScrollBar::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	P2N("CxScrollBar::OnNotify\r\n");
	return CScrollBar::OnNotify(wParam, lParam, pResult);
}

BOOL CxScrollBar::PreTranslateMessage(MSG* pMsg) 
{
	//if(!mxpad.is_junk(pMsg->message)) 
	//	P2N("CxScrollBar::PreTranslateMessage %s\r\n", mxpad.num2msg(pMsg->message));	

	return CScrollBar::PreTranslateMessage(pMsg);
}
