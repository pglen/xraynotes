
/* =====[ BmpDC.h ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the BmpDC.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  4/3/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#if !defined(AFX_BMPDC_H__E37E8DF2_5AC5_438F_BCA7_241718A1A55C__INCLUDED_)
#define AFX_BMPDC_H__E37E8DF2_5AC5_438F_BCA7_241718A1A55C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBmpDC  
{
public:
	//CBmpDC();
	CBmpDC::CBmpDC(HDC oldhdc, int ww, int hh);

	virtual ~CBmpDC();

	int save;
	HDC hdc;
	HBITMAP hBmp, oldBmp;

	// Casting operator overloading
	 operator HDC () { return hdc; }
};

#endif // !defined(AFX_BMPDC_H__E37E8DF2_5AC5_438F_BCA7_241718A1A55C__INCLUDED_)
