
/* =====[ xTracker.h ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the xTracker.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  2/2/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// xTracker.h: interface for the CxTracker class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XTRACKER_H__FDC30791_68E7_4F97_A0A8_892EDD49C6BA__INCLUDED_)
#define AFX_XTRACKER_H__FDC30791_68E7_4F97_A0A8_892EDD49C6BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////////

#define     NO_HIT		0		// No Hit

#define     UL_HIT		1		// Read: Upper Left  
#define     UR_HIT		2		// Read: Upper Right
#define     LL_HIT		3		// Read: Lower Left  
#define     LR_HIT		4		// Read: Lower Right

#define     UM_HIT		5		// Read: Upper Middle
#define     RM_HIT		6
#define     BM_HIT		7
#define     LM_HIT		8

#define     ULC_HIT		9		// Read: upper left circle
#define     LLC_HIT		10
#define     URC_HIT		11
#define     LRC_HIT		12

#define     WHOLE_HIT   16		// Hit all
#define     HH_HIT		32

//////////////////////////////////////////////////////////////////////////

class CxTracker  

{

public:

	void	Draw(CDC *pDC, CRect *rc, int twidth = 12, int circ = 0);
	int		HitTest(CPoint point);
	void	DrawFocus(CDC *pDC, RECT *rc, int solid);
	void	DrawFocEllipse(CDC *pDC, RECT *rc, int solid);

	CxTracker();
	virtual ~CxTracker();

	int		lasthit;
	CRect whole;

protected:

	CRect rc1, rc2, rc3, rc4;
	CRect rc1c, rc2c, rc3c, rc4c;
	CRect rc1m, rc2m, rc3m, rc4m;
	
};

#endif // !defined(AFX_XTRACKER_H__FDC30791_68E7_4F97_A0A8_892EDD49C6BA__INCLUDED_)
