
/* =====[ BmpDC.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the BmpDC.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  4/3/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#include "stdafx.h"
#include "xraynotes.h"
#include "BmpDC.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBmpDC::CBmpDC(HDC oldhdc, int ww, int hh)

{
	//P2N("CBmpDC::CBmpDC %d %d\r\n", ww, hh);
	hdc = CreateCompatibleDC(oldhdc);
	save = SaveDC(hdc);

	hBmp = CreateCompatibleBitmap(oldhdc, ww, hh);
	oldBmp = (HBITMAP)SelectObject(hdc, hBmp);

	// Tried filling, no difference
	//CRect rc( 0, 0, ww, hh);
	//FillRect(hdc, &rc, (HBRUSH)GetStockObject(BLACK_BRUSH)); 
}


CBmpDC::~CBmpDC()

{
	SelectObject(hdc, oldBmp);	DeleteObject(hBmp);
	RestoreDC(hdc, save);
	DeleteDC(hdc);
}
