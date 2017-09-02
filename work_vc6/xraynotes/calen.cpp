
/* =====[ calen.cpp ]========================================== 
                                                                             
   Description:     The dbtest project, implementation of the calen.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  7/22/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "xraynotes.h"

//include "dbtest.h"

#include "mxpad.h"
#include "sqlite3.h"
#include "support.h"
#include "calen.h"

CString CalendarFile("calendar.sqlt");

int		CheckCalDBfile(const char *str)

{
	CString calfile;
	support.GetDataRoot(calfile);

	if(str == NULL)
		str = CalendarFile;

	calfile += str;

	//P2N("CheckCalDBfile %s\r\n", calfile);

	// Create scema if not there
	if(!support.IsFile(calfile))
		{
		CreateCalDBfile(calfile);
		}

	return 0;
}

int		CreateCalDBfile(const char *str)

{
	P2N("CXcalDlg::CreateDBfile %s\r\n", str);
			
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	rc = sqlite3_open(str, &db);

	if( rc )
		{
		//MBOXP("SQL error: %s", sqlite3_errmsg(db));

		MBOXP("SQL error in %s line %d %s ", "CXcalDlg", __LINE__, sqlite3_errmsg(db));
		
		sqlite3_close(db);
		sqlite3_free(zErrMsg);
		return 0;
		}
	
	CString qqq = 	"CREATE TABLE calendar ( id INTEGER PRIMARY KEY, name TEXT,  "
					"desc text, longdesc text, sdate datetime,  edate datetime, "
					"note text, tidstr text, patid text)";

	//MBOXP("SQL: [%s]", qqq);

	P2N("Creating CALENDAR -> qqq=[%s]\r\n", qqq);

	rc = sqlite3_exec(db, qqq, NULL, NULL, &zErrMsg);

	if( rc != SQLITE_OK )
		{
		MBOXP("SQL error on Creating table: %s -- %s", str, sqlite3_errmsg(db));
		sqlite3_free(zErrMsg);
		}

	sqlite3_close(db);

	return true;
}

