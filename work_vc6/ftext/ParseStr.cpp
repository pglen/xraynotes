
/* =====[ ParseStr.cpp ]========================================== 
                                                                             
   Description:     The ftext project, implementation of the ParseStr.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  7/7/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// ParseStr.cpp: implementation of the CParseStr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ftext.h"
#include "mxpad.h"
#include "ParseStr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CParseStr::CParseStr()
{

}

CParseStr::CParseStr(const char *text)

{
	CString str = text;
	ParseStr(str);
}

CParseStr::~CParseStr()
{
	CParseStr::FreeParseStr();
}


//////////////////////////////////////////////////////////////////////
// Remember to destroy the array after it is done with FreeParseStr
//

void	CParseStr::ParseStr(CString &text)

{
	CString tmp, *tmp2;

	P2N("Parsing: '%s'\r\n\r\n", text);

	int pos = 0, start = 0, count = 0;

	while(true)
		{
		pos = text.Find("\r", pos);

		if(pos < 0)
			{
			tmp = text.Mid(start, text.GetLength() - start);
			tmp2 = 	new CString(tmp);

			arr.Add(tmp2);
			P2N("text2='%s'\r\n", *tmp2);
			break;
			}

		// Exclude new line
		tmp = text.Mid(start, (pos - start));
		tmp2 = new CString(tmp);

		// Skip \r and \n if present
		pos++;
		if(text[pos] == '\n')
			pos++;

		start = pos;

		P2N("text='%s'\r\n", *tmp2);
		arr.Add(tmp2);

		// Safety net:
		//if(count ++ > 10)
		//	break;
		}
}

//////////////////////////////////////////////////////////////////////////

CString *CParseStr::GetAt(int idx)

{
	ASSERT(idx >= 0);	ASSERT(idx < GetCount());
	
	return (CString *)arr[idx];
}


int		CParseStr::GetCount()

{
	return arr.GetSize();
}


void	CParseStr::FreeParseStr()

{
	int loop;
	int size = arr.GetSize();

	for(loop = 0; loop < size; loop++)
		{
		CString *tmp = (CString *)arr.GetAt(loop);
		//PrintToNotepad("del %d-%s\r\n", loop, *tmp);
		delete tmp;
		}
}

