
/* =====[ parse.h ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the parse.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  10/28/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

//////////////////////////////////////////////////////////////////////
//
// Parse.h
//

void	ParseCSVStr(CString &text, CPtrArray *ret);
void	ParseStr(CString &text, CPtrArray *ret);
void	FreeParseStr(CPtrArray  *ret);
int		ParseDateStr(const char *dstr, CString *arr, int lim);
int		ParsePathStr(const char *cmdline, CString *arr, int lim);
int		Str2Date(const char *str, COleDateTime *dd, int repair = 0);
void	str2date(const char *str, COleDateTime *ddd);
int		DLookup(const char *dbfile, const char *cond, CString *res, int arrsize);
