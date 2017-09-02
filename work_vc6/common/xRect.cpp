
/* =====[ xRect.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the xRect.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  1/19/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// xRect.cpp: implementation of the CxRect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "xRect.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CxRect::CxRect()

{
	Clear();
}

CxRect::CxRect(int ll, int tt, int rr, int bb)

{
	left = ll; top = tt;
	right = rr; bottom = bb;
}

CxRect::CxRect(int ww, int hh)

{
	left = top = 0;	right = ww; bottom = hh;
}

CxRect::~CxRect()

{

}

//////////////////////////////////////////////////////////////////////////
//
// Function name:    clear_rect
// Description:      <empty description>
// Return type:      
// Argument:         RECT *rc
//
//////////////////////////////////////////////////////////////////////////

CxRect&	CxRect::Clear()

{
	left = 0;
	top =  0;
	right = 0;
	bottom = 0;

	return *this;
}

//////////////////////////////////////////////////////////////////////////
//
// Function name:    norm_rect
// Description:      Normalize rect (Swap top/buttom left rigth if needed)
// Return type:      
// Argument:         RECT *rc
//
//////////////////////////////////////////////////////////////////////////

CxRect&	CxRect::Normalize()

{
	if(left > right)
		{
		int tmp = left;
		left = right;	
		right = tmp;
		}

	if(top >  bottom)
		{
		int tmp = top;
		top = bottom;	
		bottom = tmp;
		}

	return *this;
}

//////////////////////////////////////////////////////////////////////////
//
// Function name:    pad_rect
// Description:      <empty description>
// Return type:      
// Argument:         RECT *rc
// Argument:         int pad
//
//////////////////////////////////////////////////////////////////////////

CxRect&	CxRect::Pad(int pad)

{
	Normalize();

	left -= pad;
	top -= pad;
	right += pad;
	bottom += pad;

	return *this;
}

