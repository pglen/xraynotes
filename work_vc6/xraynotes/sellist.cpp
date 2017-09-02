// CSelList.cpp : implementation file
//

#include "stdafx.h"
#include "xraynotes.h"
#include "SelList.h"
#include "mxpad.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelList

CSelList::CSelList()
{
	P2N("CSelList::CSelList\r\n");
}

CSelList::~CSelList()
{
}


BEGIN_MESSAGE_MAP(CSelList, CCheckListBox)
	//{{AFX_MSG_MAP(CSelList)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelList message handlers

static first = 0;

void CSelList::OnPaint() 
{
	//CPaintDC dc(this); // device context for painting
	
	//if(!first)
	//	{
	//	ModifyStyle(0, LBS_HASSTRINGS |  LBS_OWNERDRAWFIXED);
	//	}
	//P2N("CSelList::OnPaint\r\n");
		
	CCheckListBox::OnPaint();

	// Do not call CCheckListBox::OnPaint() for painting messages
}

void CSelList::PreSubclassWindow() 

{
	ASSERT(m_hWnd != NULL);

	::SetWindowLong(m_hWnd, GWL_STYLE, 
			::GetWindowLong(m_hWnd, GWL_STYLE) | LBS_HASSTRINGS | LBS_OWNERDRAWFIXED);  	
	
	CCheckListBox::PreSubclassWindow();
}
