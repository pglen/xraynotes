
/* =====[ CommLine.h ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the CommLine.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  11/7/2007  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// CommLine.h: interface for the CCommLine class.
//
//////////////////////////////////////////////////////////////////////

#define ARGARRSIZE		12
#define	MAXARGARRSIZE	1000

#if !defined(AFX_COMMLINE_H__65DF4529_7966_4123_9795_CB03937102E1__INCLUDED_)
#define AFX_COMMLINE_H__65DF4529_7966_4123_9795_CB03937102E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCommLine  
{
public:

	int		lim;
	CStringArray argarr;

	int		CCommLine::ParseComandLine(const char *cmdline);
	int		CCommLine::InterpretComandLine();

public:
	CCommLine();
	virtual ~CCommLine();

};

#endif // !defined(AFX_COMMLINE_H__65DF4529_7966_4123_9795_CB03937102E1__INCLUDED_)
