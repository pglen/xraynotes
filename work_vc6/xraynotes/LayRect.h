// LayRect.h: interface for the CLayRect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LAYRECT_H__AF6E6E3E_1A1D_4D98_8A70_467E93D2BFE3__INCLUDED_)
#define AFX_LAYRECT_H__AF6E6E3E_1A1D_4D98_8A70_467E93D2BFE3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "layer.h"

class CLayRect : public CLayer

{
	friend class CLayer;

protected:

	int dstW, dstH;
	
	int	Invalidate(int dummy1, int dummy2, int dummy3);
	int	Prop(int dummy1 = 0, int dummy2 = 0, int dummy3 = 0);
	int Draw(CDC *pDC, float factor, int dummy1 = 0);
	int MouseEvent(UINT event, UINT nFlags, CPoint point);
	
public:

	//int	rect_transparent;

	CLayRect();
	virtual ~CLayRect();

};

#endif // !defined(AFX_LAYRECT_H__AF6E6E3E_1A1D_4D98_8A70_467E93D2BFE3__INCLUDED_)
