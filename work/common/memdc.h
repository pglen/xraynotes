
/* =====[ memdc.h ]======================================================= 
                                                                             
   Description:     The xraynotes project, implementation of the memdc.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  1/27/2009  Peter Glen   Initial version.                         
                                                                             
   ==================================================================== */

#ifndef _MEMDC_H_
#define _MEMDC_H_

//////////////////////////////////////////////////////////////////////////
// CMemDC - memory DC
//
// Originally by Keith Rule.
//
// You may freely use or modify this code provided this
// Copyright is included in all derived versions.
//
// This class implements and automates a 'Memory Device Context' which 
// allows flicker free drawing.
//
//////////////////////////////////////////////////////////////////////////

#include "../common/mxpad.h"

class CMemDC : public CDC 

{

public:

	// Construct:
	CMemDC(CDC* pDC, int clear = false) 
		{ pDC->GetClipBox(&m_rect); Construct(pDC, clear); }

	CMemDC(CDC* pDC, const RECT& rect) : CDC() 
		{ m_rect = rect; Construct(pDC); }

	// Destruct:
	virtual ~CMemDC() { Destruct(); };

	// Allow usage as a pointer    
	CMemDC* operator->()    { return this; }    
	operator CMemDC*()		{ return this; }

protected:

	int		 save;
	CBitmap  m_bitmap;       // Offscreen bitmap
	CBitmap* m_oldBitmap;    // Bitmap originally found in CMemDC
	CDC*     m_pDC;          // Saves CDC passed in constructor
	CRect    m_rect;         // Rectangle of the drawing area.
	BOOL     m_bMemDC;       // TRUE if CDC really is a Memory DC.
    
private:

	void Construct(CDC* pDC, int clear = false);
	void Destruct();	
};

//////////////////////////////////////////////////////////////////////////

#if 0

class CMemPaintDC : public CPaintDC 

{

public:

	// Construct:
	CMemPaintDC(CWnd* pWnd, int clear = false) : CPaintDC(pWnd) 
		{ GetClipBox(&m_rect); Construct(clear); }

	CMemPaintDC(CWnd* pWnd, const RECT& rect) : CPaintDC(pWnd) 
		{ m_rect = rect; Construct(pDC); }

	// Destruct:
	virtual ~CMemPaintDC() { Destruct(); };

	// Allow usage as a pointer    
	CMemPaintDC* operator->()   { return this; }    
	operator CMemPaintDC*()		{ return this; }

protected:

	CBitmap		m_bitmap;       // Offscreen bitmap
	CBitmap*	m_oldBitmap;    // Bitmap originally found in CMemDC
	CPaintDC*	m_pDC;          // Saves CDC passed in constructor
	CRect		m_rect;         // Rectangle of the drawing area.
	BOOL		m_bMemDC;       // TRUE if CDC really is a Memory DC.
    
private:

	void Construct(CPaintDC* pDC, int clear = false);
	void Destruct();	
};

#endif

//////////////////////////////////////////////////////////////////////////
// Save DC context
// Make sure you include the block nesting brackets
// { 
// CSaveDC sdc(dc);
// dc changing code here
// }

class CSaveDC 

{

public:

   CSaveDC(CDC & dc) { sdc = &dc; saved = dc.SaveDC(); }
   CSaveDC(CDC * dc) { sdc = dc; saved = dc->SaveDC(); }
   virtual ~CSaveDC() { sdc->RestoreDC(saved); }

protected:

   CDC * sdc;
   int saved;
};

#endif
