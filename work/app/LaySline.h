
/* =====[ LaySline.h ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the LaySline.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  11/28/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// LaySline.h: interface for the CLaySline class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LAYSLINE_H__4DEF0AB7_45E4_4A0C_9983_609CE1CFF148__INCLUDED_)
#define AFX_LAYSLINE_H__4DEF0AB7_45E4_4A0C_9983_609CE1CFF148__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Layer.h"

typedef int (*pfri_p)(void *, POINT *pt);

class CLaySline : public CLayer 

{
	friend class CLayer;

typedef struct _SLData

{
	int		ver, magic;
	int		arrow_start, arrow_end;
	int		arrow_len, arrow_elen;
	int		line_thickness, line_dashness;
	int		cap_square;

	double	arrow_ang, arrow_eang;

} SLData;

protected:

	int		Prop(int dummy1 = 0, int dummy2 = 0, int dummy3 = 0);
	int		Invalidate(int dummy1 = 0, int dummy2 = 0, int dummy3 = 0);	
	int		HitTest(POINT pt, int hit, int dummy2 = 0);
	int		KeyEvent(UINT event, UINT nFlags, UINT nChar);
	int		MouseEvent(UINT event, UINT nFlags, CPoint point);
	int		Init(POINT pt, int dummy1 = 0, int dummy2 = 0);
	int		Draw(CDC *pDC, float factor, int dummy1 = 0);

public:
	void Dup(void **ptr, int *len);

	// Data
	SLData	sldata, old_sldata;

	CLaySline();
	CLaySline::CLaySline(POINT pt);
	virtual ~CLaySline();

	// Accessed from callback, needs to be public
	int		_TestLinePt(int px, int py, CPoint pt);
	int		_iterhit;
	POINT	_iterpt;
	int		_handlesize;

	
protected:

	
private:

	
	int	 InvalHandle(CWnd *dlg, int xx, int yy);
	void InitVars();

};

#endif // !defined(AFX_LAYSLINE_H__4DEF0AB7_45E4_4A0C_9983_609CE1CFF148__INCLUDED_)
