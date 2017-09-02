// PGtree.cpp : implementation file
//

#include "stdafx.h"
#include "dbtest.h"
#include "PGtree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPGtree

CPGtree::CPGtree()
{
}

CPGtree::~CPGtree()
{
}


BEGIN_MESSAGE_MAP(CPGtree, CTreeCtrl)
	//{{AFX_MSG_MAP(CPGtree)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPGtree message handlers

void CPGtree::OnMouseMove(UINT nFlags, CPoint point) 
{
	currentmouse = point;	
	CTreeCtrl::OnMouseMove(nFlags, point);
}

void CPGtree::OnLButtonDown(UINT nFlags, CPoint point) 
{
	currentmouse = point;	
	
	CTreeCtrl::OnLButtonDown(nFlags, point);
}

void CPGtree::OnRButtonDown(UINT nFlags, CPoint point) 
{
	currentmouse = point;	
	
	CTreeCtrl::OnRButtonDown(nFlags, point);
}
