
/* =====[ memdc.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the memdc.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  1/27/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#include "stdafx.h"
#include "memdc.h"

//////////////////////////////////////////////////////////////////////////
// The constructor makes a copy of the orignal DC pointer, 
// sets up a new Mem DC

void CMemDC::Construct(CDC* pDC, int clear)

{
    ASSERT(pDC != NULL); 

	//P2N("CMemDC::Construct: "); P2N_RECT(m_rect);

	// Copy original pointer to DC
    m_pDC = pDC;
    
	// Some initialization
    m_oldBitmap = NULL;
    m_bMemDC = !pDC->IsPrinting();

    if (m_bMemDC) 
		{
        // Real Memory DC, dup it
        CreateCompatibleDC(pDC); pDC->LPtoDP(&m_rect);

		save = SaveDC();
        m_bitmap.CreateCompatibleBitmap(pDC, m_rect.Width(), m_rect.Height());

        m_oldBitmap = SelectObject(&m_bitmap);
        
        SetMapMode(pDC->GetMapMode());
        pDC->DPtoLP(&m_rect);
        SetWindowOrg(m_rect.left, m_rect.top);

		//BeginPath();
		//EndPath();
		//SelectClipRgn(NULL);

		//CRgn rgn; rgn.CreateRectRgn(m_rect.left, m_rect.top,
		//			m_rect.right, m_rect.bottom);		
		//pDC->SelectClipRgn(&rgn);	
		} 
	else 
		{
		// Printer DC, copy the relevent parts 
        m_bPrinting = pDC->m_bPrinting;
        m_hDC       = pDC->m_hDC;
        m_hAttribDC = pDC->m_hAttribDC;
		}

    // Fill background, if requested
    if(clear)
		FillSolidRect(m_rect, pDC->GetBkColor());
}

//////////////////////////////////////////////////////////////////////////
// The destructor puts the newly drawn DC back into the orignal DC

void	CMemDC::Destruct()

{
	//P2N("CMemDC::Destruct\r\n");

	if (m_bMemDC) 
		{
		// Make sure all is allowed to copy
		//BeginPath(); EndPath();

		//SelectClipRgn(NULL);

		// Copy the offscreen bitmap onto the screen.
		m_pDC->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),
			this, m_rect.left, m_rect.top, SRCCOPY);            

		// Swap back the original bitmap.
		SelectObject(m_oldBitmap);  
		
		RestoreDC(save);
		} 
	else 
		{
		// All we need to do is replace the DC with an illegal value,
		// this keeps us from accidently deleting the handles associated with
		// the CDC that was passed to the constructor.            
		m_hDC = m_hAttribDC = NULL;
		}    
}

#if 0

//////////////////////////////////////////////////////////////////////////
// The constructor makes a copy of the orignal DC pointer, 
// sets up a new Mem DC

void CMemPaintDC::Construct(CPaintDC* pDC, int clear)

{
    ASSERT(pDC != NULL); 

	//P2N("CMemDC::Construct\r\n");

	// Copy original pointer to DC
    m_pDC = pDC;
    
	// Some initialization
    m_oldBitmap = NULL;
    m_bMemDC = !pDC->IsPrinting();

    if (m_bMemDC) 
		{
        // Real Memory DC, dup it
        CreateCompatibleDC(pDC); pDC->LPtoDP(&m_rect);

        m_bitmap.CreateCompatibleBitmap(pDC, m_rect.Width(), m_rect.Height());

        m_oldBitmap = SelectObject(&m_bitmap);
        
        SetMapMode(pDC->GetMapMode());
        pDC->DPtoLP(&m_rect);
        SetWindowOrg(m_rect.left, m_rect.top);

		//BeginPath();
		//EndPath();
		//SelectClipRgn(NULL);

		//CRgn rgn; rgn.CreateRectRgn(m_rect.left, m_rect.top,
		//			m_rect.right, m_rect.bottom);		
		//pDC->SelectClipRgn(&rgn);	
		} 
	else 
		{
		// Printer DC, copy the relevent parts 
        m_bPrinting = pDC->m_bPrinting;
        m_hDC       = pDC->m_hDC;
        m_hAttribDC = pDC->m_hAttribDC;
		}

    // Fill background, if requested
    if(clear)
		FillSolidRect(m_rect, pDC->GetBkColor());
}

//////////////////////////////////////////////////////////////////////////
// The destructor puts the newly drawn DC back into the orignal DC

void	CMemPaintDC::Destruct()

{
	//P2N("CMemDC::Destruct\r\n");

	if (m_bMemDC) 
		{
		// Make sure all is allowed to copy
		//BeginPath(); EndPath();

		//SelectClipRgn(NULL);

		// Copy the offscreen bitmap onto the screen.
		m_pDC->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),
			this, m_rect.left, m_rect.top, SRCCOPY);            

		// Swap back the original bitmap.
		SelectObject(m_oldBitmap);        
		} 
	else 
		{
		// All we need to do is replace the DC with an illegal value,
		// this keeps us from accidently deleting the handles associated with
		// the CDC that was passed to the constructor.            
		m_hDC = m_hAttribDC = NULL;
		}    
}

#endif
