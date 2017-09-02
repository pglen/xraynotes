
/* =====[ prl.h ]========================================== 
                                                                             
   Description:     The dbtest project, implementation of the prl.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  5/6/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */



#define MAXPRLCOMP	9
#define MAXPRDEPTH	6
	
int		ParsePRL(const char *prl, CPtrArray *arr);
int		MakePRLDir(CString &prl, CString &str);
void	CleanPRLname(CString &str);
