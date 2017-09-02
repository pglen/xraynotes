
/* =====[ CommLine.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the CommLine.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  11/7/2007  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// CommLine.cpp: implementation of the CCommLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "xraynotes.h"
#include "CommLine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCommLine::CCommLine()

{
	lim = MAXARGARRSIZE;
}

CCommLine::~CCommLine()

{

}

//////////////////////////////////////////////////////////////////////////
//
// Function name:    ParseComandLine
// Description:      Parse command line strings into a string array
//					  Obeys '\ ' '\"' "str" escapes -- similar to W32 cmd
// Return type:      
// Argument:         const char *cmdline
// Argument:         CString *arr
// Argument:         int lim
//
//////////////////////////////////////////////////////////////////////////

int		CCommLine::ParseComandLine(const char *cmdline)

{
	int in_quote = 0, esc = 0, idx = 0;
	int loop, len = strlen(cmdline);

	const char *cmd = cmdline;

	argarr.SetAtGrow(idx, "");

	char prev = 0;

	for(loop = 0; loop < len; loop++)
		{
		char  chh = *cmdline++;

		switch(chh)
			{
			case '\"':
				if(esc)
					{
					esc = false;
					argarr[idx] += chh;
					}
				else
					{
					if(in_quote)
						in_quote = false;
					else
						in_quote = true;
					}
				break;

			case '\\':
				if(esc)
					{
					// Double backslash
					argarr[idx] += chh;
					esc = false;
					}
				else
					{
					esc = true;
					}
				break;
		
			case ' ':
				if(esc)
					argarr[idx] += chh;	
				else
					{
					if(in_quote)
						argarr[idx] += chh;	
					else
						{
						// Only skip on one space
						if(prev != ' ')
							{
							idx ++;
							argarr.SetAtGrow(idx, "");
							}
						}
					}
				break;

			default:
				// See if we have a dangleing escape, output prev
				if(esc)
					{	
					esc = false;
					argarr[idx] += prev; 	
					}
					
				argarr[idx] += chh;
			}

		prev = chh;

		// Terminating condition
		//if(idx >= lim)
		//	break;
		}

	// For testing purposes
#if 0
	for(loop = 0; loop < argarr.GetSize(); loop++)
		{
		P2N("String array %d '%s'\r\n", loop, argarr[loop]);
		}
#endif

	return 0;
}

//////////////////////////////////////////////////////////////////////////
//
// Function name:    InterpretComandLine
// Description:      <empty description>
// Return type:      
// Argument:         CString *arr
// Argument:         int lim
//
//////////////////////////////////////////////////////////////////////////

int		CCommLine::InterpretComandLine()

{	
	CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);
	ASSERT(dlg);

	for(int loop = 0; loop < argarr.GetSize(); loop++)
		{
		if(argarr[loop] == "/reset")
			{
			P2N("Reset autosave settings\r\n");
			dlg->resetpositions = true;
			}

		if(argarr[loop] == "/b")
			{
			//mainwin->m_check2 = true;
			}
		}

	return 0;
}