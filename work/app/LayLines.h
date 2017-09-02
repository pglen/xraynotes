
/* =====[ LayLines.h ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the LayLines.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  1/16/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// LayLines.h: interface for the CLayLines class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LAYLINES_H__74886FCD_21E9_4498_B3EB_FE37F5159F7C__INCLUDED_)
#define AFX_LAYLINES_H__74886FCD_21E9_4498_B3EB_FE37F5159F7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Layer.h"

class CLayLines : public CLayer 

{
public:

	friend class CLayer;

protected:

	int		Prop(int dummy1 = 0, int dummy2 = 0, int dummy3 = 0);
	int		HitTest(POINT pt, int dummy1 = 0, int dummy2 = 0);
	int		Invalidate(int dummy1, int dummy2, int dummy3);
	int		MouseEvent(UINT event, UINT nFlags, CPoint point);
	int		Draw(CDC *pDC, float factor, int dummy1 = 0);

public:

	int		Init(POINT pt, int dummy1 = 0, int dummy2 = 0);

	// Data:
	CPtrList	points;

	int		num_lines;	
	//int		line_thickness, 
	int		line_dashness, line_init;
	int		handlesize;

	//COLORREF	line_color;
	COLORREF	dash_color;
	COLORREF 	pen_color;

	CDWordArray	line_x, line_y;

	CLayLines();
	virtual ~CLayLines();

private:

	void	InitVars();
	void	AddLinePoint(int px, int py);
};

#endif // !defined(AFX_LAYLINES_H__74886FCD_21E9_4498_B3EB_FE37F5159F7C__INCLUDED_)
