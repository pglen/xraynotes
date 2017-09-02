
/* =====[ calen.h ]========================================== 
                                                                             
   Description:     The dbtest project, implementation of the calen.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  7/22/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */


extern CString CalendarFile;


int		CreateCalDBfile(const char *str);
int		CheckCalDBfile(const char *str);

