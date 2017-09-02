// lines.cpp: implementation of the Clines class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "xraynotes.h"
#include "lines.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(Clines, CStatic)
		
BEGIN_MESSAGE_MAP(Clines, CStatic)
	//{{AFX_MSG_MAP(Clines)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Clines::Clines()

{
	lstart.x	= 0;
	lend.x		= 0;
	thick		= 1;
	color		= RGB(0,0,0);
}


Clines::~Clines()
{

}

void Clines::OnLButtonDown(UINT nFlags, CPoint point)

{
	P2N("Left b down in lines\r\n");
	CStatic::OnLButtonDown(nFlags, point);
}

void Clines::OnPaint()

{
	//P2N("paint in lines\r\n");

	CPen Pen(PS_SOLID, thick, color);

	PAINTSTRUCT ps;
	CDC *pdc = BeginPaint(&ps);

	CPen* pOldPen = (CPen*)pdc->SelectObject(&Pen);

	pdc->MoveTo(lstart);
	pdc->LineTo(lend);

	// Free resource(s)
	pdc->SelectObject(pOldPen);

	EndPaint(&ps);
}

