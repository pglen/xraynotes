
/* =====[ parse.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the parse.cpp                
                                                                             
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
// Parse.cpp: implementation of the string parser (not used)
//

#include "stdafx.h"
#include "xraynotes.h"

//include "demo.h"

//////////////////////////////////////////////////////////////////////////
//
//

void	ParseCSVStr(CString &text, CPtrArray *ret)

{
	CString tmp, *tmp2;

	//P2N("Parsing:%s\r\n", text);

	int pos = 0, start = 0, count = 0;

	while(true)
		{
		pos = text.Find(",", pos);

		if(pos < 0)
			{
			tmp = text.Mid(start, text.GetLength() - start);
			tmp2 = 	new CString(tmp);

			ret->Add(tmp2);
			//P2N("text2='%s'\r\n", *tmp2);
			break;
			}

		// Exclude new line
		tmp = text.Mid(start, (pos - start));
		tmp2 = new CString(tmp);

		pos++;
		start = pos;

		//P2N("text='%s'\r\n", *tmp2);
		ret->Add(tmp2);

		if(count ++ > 30)
			break;
		}
}

//////////////////////////////////////////////////////////////////////
// Remember to destroy the array after it is done
//

void	ParseStr(CString &text, CPtrArray *ret)

{
	CString tmp, *tmp2;

	P2N("Parsing:%s\r\n", text);

	int pos = 0, start = 0, count = 0;

	while(true)
		{
		pos = text.Find("\n", pos);

		if(pos < 0)
			{
			tmp = text.Mid(start, text.GetLength() - start);
			tmp2 = 	new CString(tmp);

			ret->Add(tmp2);
			P2N("text2='%s'\r\n", *tmp2);
			break;
			}

		// Exclude new line
		tmp = text.Mid(start, (pos - start));
		tmp2 = new CString(tmp);

		pos++;
		start = pos;

		P2N("text='%s'\r\n", *tmp2);
		ret->Add(tmp2);

		if(count ++ > 10)
			break;
		}
}

//////////////////////////////////////////////////////////////////////

void	FreeParseStr(CPtrArray  *ret)

{
	int loop;
	int size = ret->GetSize();

	for(loop = 0; loop < size; loop++)
		{
		CString *tmp = (CString *)ret->GetAt(loop);
		//PrintToNotepad("del %d-%s\r\n", loop, *tmp);
		delete tmp;
		}
}



//////////////////////////////////////////////////////////////////////////
//
// Function name:    ParsePathStr
// Description:      Parse string by separator into a string array
//					 Obeys '\ ' '\"' "str" escapes -- similar to W32 file name parse
// Return type:      
// Argument:         const char *cmdline
// Argument:         const char *sep
// Argument:         CString *arr
// Argument:         int lim
//
//////////////////////////////////////////////////////////////////////////

int		ParseDateStr(const char *dstr, CString *arr, int lim)

{
	int in_quote = 0, esc = 0, idx = 0;
	int loop, len = strlen(dstr);

	const char *cmd = dstr;

	char prev = 0;

	for(loop = 0; loop < len; loop++)
		{
		char  chh = *dstr++;

		if(isdigit(chh))
			{
			arr[idx] += chh;
			}
		else
			{
			if(isdigit(prev))
				idx ++;
			}
		prev = chh;

		// Terminating condition
		if(idx >= lim)
			break;
		}

	
#if 0
	// For testing purposes
	P2N("ParseDateStr Dump:\r\n");
	for(loop = 0; loop < lim; loop++)
		P2N(" %d='%s' ", loop, arr[loop]);
	P2N("\r\nParseDateStr Dump End.\r\n");
#endif

	return 0;
}


int		ParsePathStr(const char *cmdline, CString *arr, int lim)

{
	int in_quote = 0, esc = 0, idx = 0;
	int loop, len = strlen(cmdline);

	const char *cmd = cmdline;

	char prev = 0;

	for(loop = 0; loop < len; loop++)
		{
		char  chh = *cmdline++;

		switch(chh)
			{
			case '/':
				if(prev != '/')
					idx ++;
				break;

			default:
				arr[idx] += chh;
			}

		prev = chh;

		// Terminating condition
		if(idx >= lim)
			break;
		}

	// For testing purposes
#if 0
	for(loop = 0; loop < lim; loop++)
		{
		P2N("String array %d '%s'\r\n", loop, arr[loop]);
		}
#endif

	return 0;
}


//////////////////////////////////////////////////////////////////////////
// Convert a string to date
// 
// IN:		str
// OUT:		date 
//
// Return:	true if date is bad
//

int		Str2Date(const char *str, COleDateTime *dd, int repair)

{
	int ret = 0;

	CString arr[6];	ParseDateStr(str, arr, 6);

	COleDateTime dt(atoi(arr[0]), atoi(arr[1]), atoi(arr[2]), 
					atoi(arr[3]), atoi(arr[4]), atoi(arr[5]) );

	ret = dt.GetStatus() == COleDateTime::invalid;

	if(ret)
		{
		P2N("Invalid date passed to Str2Date %s\r\n", str);

		if(repair)
			{
			dt = COleDateTime::GetCurrentTime();
			}
		}

	*dd = dt;

	return ret;
}



void	str2date(const char *str, COleDateTime *ddd)

{
	CString sarr[6];
	int	prog = 0;

	while(true)
		{
		char chh = *str++;

		if(!chh)
			break;

		if(isdigit(chh))
			{
			sarr[prog] += chh;
			}
		else
			{
			prog++;

			// Safety net
			if(prog >= 6)
				break;
			}
		}

	// Test: Dump parsed array
	//for(int loop = 0; loop < 6; loop++)
	//	{
	//	P2N(" %s ", sarr[loop]);
	//	}
	//P2N("\r\n");

	ddd->SetDateTime(atoi(sarr[0]), atoi(sarr[1]), atoi(sarr[2]), 
			atoi(sarr[3]), atoi(sarr[4]), atoi(sarr[5]));
}

#include "sqlite3.h"


typedef struct _callb

{
	int			num_entry;
	int			arrsize;
	CString		*arr;

} DlookCallb;
	

static int callback_lookup(void *NotUsed, int argc, char **argv, char **azColName)

{
	CString *res = (CString *)NotUsed;
	DlookCallb  *pcb = (DlookCallb *)NotUsed;

	int cnt = pcb->arrsize;
	cnt = max(cnt, argc);

	for(int loop = 0; loop < cnt; loop++)
		{
		pcb->arr[loop] = argv[loop];
		pcb->num_entry	+= 1;
		}

	return 0;
}

//////////////////////////////////////////////////////////////////////////

int		DLookup(const char *dbfile, const char *cond, CString *res, int arrsize)

{
	int ret = true;

	//P2N("DLookup %s [%s] \r\n", dbfile, cond);
	
	DlookCallb  cb;

	cb.arrsize		= arrsize;
	cb.num_entry	= 0;
	cb.arr			= res;

	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	rc = sqlite3_open(dbfile, &db);

	if( rc )
		{
		//MBOXP("DLookup: SQL error in: %s ", sqlite3_errmsg(db));
		res[0] = "No Database";
		sqlite3_close(db);
		sqlite3_free(zErrMsg);
		return false;
		}
			
	//MBOXP("SQL: '%s'", cond);
	//P2N("dlookup: cond=[%s]\r\n", cond);

	rc = sqlite3_exec(db, cond, callback_lookup, &cb, &zErrMsg);

	if( rc != SQLITE_OK )
		{
		//MBOXP("SQL error %s", sqlite3_errmsg(db));
		sqlite3_free(zErrMsg);
		ret = false;
		res[0] = "Error";
		}

	if(cb.num_entry == 0)
		{
		res[0] = "No Data";
		}

	sqlite3_close(db);

	return ret;
}
