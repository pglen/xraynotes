
/* =====[ misc.h ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the misc.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  10/20/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#if !defined(AFX_SHADES_H__10CE72D1_1678_4810_8D52_1EA02C2176E1__INCLUDED_)
#define AFX_SHADES_H__10CE72D1_1678_4810_8D52_1EA02C2176E1__INCLUDED_

#ifndef ROTATE_LONG_RIGHT
#define ROTATE_LONG_RIGHT(xx, rot) ( (xx << rot) | (xx >> (32 - rot)) )
#endif

class CMisc

{

public:

	UUID	str2uuid(CString &str);
	CString	uuid2str(UUID *id);

	void	CurrentMonRect(POINT pt, RECT *rc);

	long	hash_string(const char  *name);
	bool	IsCtrlDown();
	bool	IsShiftDown();
	bool	IsAltDown();
};

extern CMisc misc;

#endif
