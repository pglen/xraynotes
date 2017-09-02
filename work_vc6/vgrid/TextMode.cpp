
/* =====[ TextMode.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the TextMode.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  1/26/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

//////////////////////////////////////////////////////////////////////
// Convenience class to save/restore CDC text parameters

#include "stdafx.h"
#include "vgrid.h"
#include "TextMode.h"
#include "mxpad.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTextMode::CTextMode()

{
	P2N("CTextMode::CTextMode Constructor\r\n");
	gl_pCDC = NULL;
}

CTextMode::CTextMode(CClientDC *pDC)

{
	gl_pCDC = (CDC*)pDC;
	Save();
}

CTextMode::CTextMode(CDC *pDC)

{
	gl_pCDC = (CDC*)pDC;
	Save();
}

CTextMode::CTextMode(CPaintDC *pDC)

{
	gl_pCDC = (CDC*)pDC;
	Save();
}

void	CTextMode::Save()

{
	oldmode		= gl_pCDC->GetBkMode();
	oldfgcolor	= gl_pCDC->GetTextColor();
	oldbgcolor	= gl_pCDC->GetBkColor();
}

CTextMode::~CTextMode()

{
	//P2N("CTextMode::~CTextMode() mode=%d fg=%d bg=%d\r\n", 
	//			oldmode, oldfgcolor, oldbgcolor);
	Restore();
}

void	CTextMode::Restore()

{
	ASSERT(gl_pCDC);

	gl_pCDC->SetBkMode(oldmode);
	gl_pCDC->SetTextColor(oldfgcolor);
	gl_pCDC->SetBkColor(oldbgcolor);
}