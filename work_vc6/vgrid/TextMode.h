
/* =====[ TextMode.h ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the TextMode.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  1/26/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#if !defined(AFX_TEXTMODE_H__9C9C42EA_90E3_4D3B_AF90_9A1D915B142F__INCLUDED_)
#define AFX_TEXTMODE_H__9C9C42EA_90E3_4D3B_AF90_9A1D915B142F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTextMode  
{
	CDC			*gl_pCDC;
	int			oldmode;
	COLORREF	oldbgcolor, oldfgcolor;

public:

	CTextMode(CClientDC *pDC);
	CTextMode(CPaintDC *pDC);
	CTextMode(CDC *pDC);

	CTextMode();
	virtual ~CTextMode();

protected:

	void	CTextMode::Restore();
	void	CTextMode::Save();
};

#endif // !defined(AFX_TEXTMODE_H__9C9C42EA_90E3_4D3B_AF90_9A1D915B142F__INCLUDED_)
