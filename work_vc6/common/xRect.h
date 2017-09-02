
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
	CxRect(int ll, int tt, int rr, int bb);
	CxRect(int ww, int hh);

	virtual ~CxRect();

	CxRect&	Clear();
	CxRect&	Normalize();
	CxRect&	Pad(int pad);
	
	// Return Width/Height

	inline int		Width() {	return abs(right - left);};
	inline int		Height() {	return abs(bottom - top);};

	//////////////////////////////////////////////////////////////////////

	inline operator RECT* ()	{ return (this); };

	inline void operator=(const RECT& srcRect)
								{ ::CopyRect(this, &srcRect); }

	inline operator += (int offs) {left -= offs; right += offs; 
								top -= offs; bottom += offs; };

	inline operator += (POINT pt) {left -= pt.x; right += pt.x; 
								top -= pt.y; bottom += pt.y; };

	inline BOOL	operator==(const RECT& rect) const
							{ return ::EqualRect(this, &rect); }

	inline BOOL	operator!=(const RECT& rect) const
							{ return !::EqualRect(this, &rect); }

	inline void operator&=(const RECT& rect)
						{ ::IntersectRect(this, this, &rect); }

	inline void operator|=(const RECT& rect)
						{ ::UnionRect(this, this, &rect); }

};

#endif // !defined(AFX_XRECT_H__C2A590C0_29CE_4016_A034_CA8CEC496B58__INCLUDED_)
