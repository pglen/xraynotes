
/* =====[ Flag.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the Flag.cpp                
                                                                             
                    An intelligent integer for class variables.
					Will set itself to zero on construction.
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  9/19/2007  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// Flag.cpp: implementation of the CFlag class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "xraynotes.h"
#include "Flag.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFlag::CFlag()

{
	//P2N("inited flag\r\n");
	value = 0;
}

CFlag::~CFlag()

{
}

int CFlag::operator ~()

{
	//P2N("Bitwise %d\r\n", !value);
	return ~value;
}

int CFlag::operator !()

{
	//P2N("Negated %d\r\n", !value);
	return !value;
}

bool CFlag::operator ||(int nval)

{
	//P2N("Logical or %d\r\n", nval);
	return value || nval;
}
