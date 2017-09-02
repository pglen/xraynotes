
/* =====[ helperdlgs.h ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the helperdlgs.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  1/28/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#include "chevron.h"
#include "Flyout.h"
#include "ColDlg.h"
#include "MagDlg.h"
#include "xScroll.h"
#include "yScroll.h"
#include "gestureDlg.h"
#include "ProcDlg.h"
#include "BriCon.h"
#include "DicomWin.h"
#include "PyGlg.h"
#include "dbmain.h"
#include "xyzdlg.h"
#include "Status.h"
#include "CurveDlg.h"
#include "doclist.h"
#include "ToolShow.h"
#include "dicomfile.h"
#include "doclist.h"
#include "patdmg.h"
#include "AddrEntry.h"
#include "CurveDlg.h"
#include "TestDlg.h"

extern	CMagDlg     magdlg;
extern	CGestureDlg gesdlg;
extern	CProcDlg    procdlg;
extern	CBriCon		bcondlg;
extern	CColDlg     coldlg;
extern	CDicomWin   dicomwin;
extern	CPyGlg		pydlg;
//extern	CDBMain		dbmain;
extern	CMagWnd		magwin;
extern	CxyzDlg		xyzdlg;
extern	CCurveDlg	cdlg;

// Flyout Windows
extern CFlyout nwfo;
extern CFlyout swfo;
extern CFlyout nefo;
extern CFlyout sefo;

// Chevron Windows
extern CChevron  ne;
extern CChevron  se;
extern CChevron  nw;
extern CChevron  sw;

extern CxScroll    xscroll;
extern CyScroll    yscroll;
extern CMagDlg     magdlg;
extern CGestureDlg gesdlg;

extern CStatus     *statusdlg;
