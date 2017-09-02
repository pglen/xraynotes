
/* =====[ do3dp.h ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the do3dp.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  11/27/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

//////////////////////////////////////////////////////////////////////////
// Macros to set/reset RGB

// Set all three RGB components
#define RGBSET(bg, r,g,b) bg.rgbBlue = b, bg.rgbGreen = g, bg.rgbRed = r;

// Reset RGB to black/white
#define RGBSETBLACK(bg) bg.rgbBlue = 0, bg.rgbGreen = 0, bg.rgbRed = 0;
#define RGBSETWHITE(bg) bg.rgbBlue = 255, bg.rgbGreen = 255, bg.rgbRed = 255;

// Add the tree components together
#define RGBSUM(bg) ((int)bg.rgbBlue + (int)bg.rgbGreen + (int)bg.rgbRed)

// Mix new pixel and old pixel in proportion to alpha (0-255)
#define ALPHABLEND(pix, old, alp) (((old * (255 - alp)) >> 8) + ((pix * alp) >> 8))

//void	Do3Dp(int rev);
//void	Do3Dp(int rev, int midx = 0, int midy = 0);
//void	Do3Dp(int rev, CXrayDoc *xdoc = NULL, int midx = 0, int midy = 0);

void	Do3Dp(int rev, CXrayDoc *sdoc, CXrayDoc *tdoc = NULL, int midx = 0, int midy = 0);


