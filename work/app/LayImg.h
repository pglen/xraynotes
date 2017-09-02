
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

// CLayImg.h: interface for the CLayImg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLayImg_H__AF6E6E3E_1A1D_4D98_8A70_467E93D2BFE4__INCLUDED_)
#define AFX_CLayImg_H__AF6E6E3E_1A1D_4D98_8A70_467E93D2BFE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "layer.h"
#include "xImg.h"

class CLayImg : public CLayer

{

public:

	friend class CLayer;

protected:

	int Draw(CDC *pDC, float factor, int dummy1 = 0);
	int MouseEvent(UINT event, UINT nFlags, CPoint point);

public:
	void OpenImage();

	int Init(POINT pt, int dummy1 = 0, int dummy2 = 0);
	void InitVars();

	// Rectangle
	COLORREF	rect_color;
	int			rect_trans;
	int			old_rotate;
	CxImg		*image, *org_image;

	CLayImg();
	virtual ~CLayImg();

};

#endif // !defined(AFX_CLayImg_H__AF6E6E3E_1A1D_4D98_8A70_467E93D2BFE4__INCLUDED_)
