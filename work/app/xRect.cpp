
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
#include "xraynotes.h"
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

void	CxRect::clear_rect(RECT *rc)

{
	rc->left = 0;
	rc->top =  0;
	rc->right = 0;
	rc->bottom = 0;
}


//////////////////////////////////////////////////////////////////////////
//
// Function name:    norm_rect
// Description:      Normalize rect (Swap top/buttom left rigth if needed)
// Return type:      
// Argument:         RECT *rc
//
//////////////////////////////////////////////////////////////////////////

void	CxRect::norm_rect(RECT *rc)

{
	if(rc->left > rc->right)
		{
		int tmp = rc->left;
		rc->left = rc->right;	
		rc->right = tmp;
		}

	if(rc->top >  rc->bottom)
		{
		int tmp = rc->top;
		rc->top = rc->bottom;	
		rc->bottom = tmp;
		}
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

void	CxRect::pad_rect(RECT *rc, int pad)

{
	rc->left -= pad;
	rc->top -= pad;
	rc->right += pad;
	rc->bottom += pad;
}
