
/* =====[ LayCirc.h ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the LayCirc.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  11/28/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// LayRect.h: interface for the CLayCirc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LAYRECT_H__AF6E6E3E_1A1D_4D98_8A70_467E93D2BFE4__INCLUDED_)
#define AFX_LAYRECT_H__AF6E6E3E_1A1D_4D98_8A70_467E93D2BFE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "layer.h"

class CLayCirc : public CLayer

{

public:

	friend class CLayer;

protected:

	//int	pentype;
	//int brushtype;
	//int hatchtype;

	int dstW, dstH;
	
	int Draw(CDC *pDC, float factor, int dummy1 = 0);
	int MouseEvent(UINT event, UINT nFlags, CPoint point);
	int	Prop(int dummy1 = 0, int dummy2 = 0, int dummy3 = 0);
	int	Invalidate(int dummy1, int dummy2, int dummy3);

public:

	int Init(POINT pt, int dummy1 = 0, int dummy2 = 0);
	void InitVars();

	// Rectangle
	//COLORREF	rect_color;
	//int			rect_trans;

	CLayCirc();
	virtual ~CLayCirc();

private:

};

#endif // !defined(AFX_LAYRECT_H__AF6E6E3E_1A1D_4D98_8A70_467E93D2BFE4__INCLUDED_)
