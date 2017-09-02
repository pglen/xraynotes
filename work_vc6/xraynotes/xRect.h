
/* =====[ xRect.h ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the xRect.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  1/19/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// xRect.h: interface for the CxRect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XRECT_H__C2A590C0_29CE_4016_A034_CA8CEC496B58__INCLUDED_)
#define AFX_XRECT_H__C2A590C0_29CE_4016_A034_CA8CEC496B58__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CxRect : public RECT

{

public:
	CxRect();
	virtual ~CxRect();

	int		top, left, bottom, right;
	void	CxRect::norm_rect(RECT *rc);
	void	CxRect::pad_rect(RECT *rc, int pad);
	void	CxRect::clear_rect(RECT *rc);

};

#endif // !defined(AFX_XRECT_H__C2A590C0_29CE_4016_A034_CA8CEC496B58__INCLUDED_)
