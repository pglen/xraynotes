
/* =====[ prl.cpp ]========================================== 
                                                                             
   Description:     The dbtest project, implementation of the prl.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  5/6/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#include "stdafx.h"
#include "dbtest.h"


#include "mxpad.h"
#include "prl.h"
#include "direct.h"

#include "support.h"
#include "msgbox.h"


int		ParsePRL(const char *prl, CPtrArray *arr)

{
	int ret = 0;
	CString tmp, *tmp2;
	int pos = 0, start = 0, count = 0;

	//P2N("ParsePRL %s\r\n", prl);

	// Is it a PRL?
	if(strnicmp(prl, "PRL://", 6) != 0)
		{
		P2N("Not a PRL\r\n");
		return ret;
		}

	CString text = prl;
	text = text.Mid(6);
	
	//P2N("Stipped %s\r\n", text);

	while(true)
		{
		pos = text.Find("_", pos);

		if(pos < 0)
			{
			tmp = text.Mid(start, text.GetLength() - start);
			tmp2 = 	new CString(tmp);

			arr->Add(tmp2);
			//P2N("text='%s' (last)\r\n", *tmp2);

			ret = true;
			break;
			}

		// Exclude new line
		CString tmp = text.Mid(start, (pos - start));
		tmp2 = new CString(tmp);

		pos++;
		start = pos;

		//P2N("text='%s'\r\n", *tmp2);
		arr->Add(tmp2);

		// Safety catch
		if(count++ > MAXPRLCOMP)
			break;
		}

	return ret;
}

///////////////////////////////////////////////////////////////////////////
//
//  firstname_midname_lastname_year_month_day_country_state_city
//
#if 0

int     createpatient(char *patient)

{
    char *dup = strdup(patient);

    P2N("Creating patient '%s'", patient);

    int loop,  ret = GOOD_PRI;

    // Init array
    char *arr[MAX_COMP_PRI];
    for(loop = 0 ; loop < MAX_COMP_PRI; loop ++)
        {
        arr[loop] = "";
        }

    // Split patient into components
    char *ptr = dup;
    for(loop = 0 ; loop < MAX_COMP_PRI; loop ++)
        {
        arr[loop] =  ptr;

        ptr = strchr(ptr, '_');
        if(ptr == NULL)
            break;

        *ptr = 0;
        ptr++;
        }

    // Dump and check
    for(loop = 0 ; loop < MAX_COMP_PRI; loop++)
        {
        debugprintf("patient component '%s'", arr[loop]);

        if(*arr[loop] == 0)
            {
            debugprintf("blank component '%s'", arr[loop]);
            ret = PARTIAL_PRI;
            }

        // Check for non_printable
        int loop2, len = strlen(arr[loop]);
        for(loop2 = 0; loop2 < len; loop2++)
            {
            if(!isprint(arr[loop][loop2]))
                ret = INVALID_PRI;
            }
        //break;
        }


    // Create dir
    char    path[MAXPATHLEN + 1];
    strcpy(path, pw->pw_dir);
    strcat(path, "/");

    for(loop = 0 ; loop < MAX_COMP_PRI; loop++)
        {
        if(*arr[loop] != 0)
            {
            strcat(path, arr[loop]);
            strcat(path, "/");

            debugprintf("patient path: '%s'", path);

            //mkdir(path);
            }
        //break;
        }

    free (dup);
    return ret;
}

#endif


//////////////////////////////////////////////////////////////////////////
//
// Create PRL directory
//

int		MakePRLDir(CString &prl, CString &str)

{
	// Create PRL directory

	int ret_val = true;
	CPtrArray arr;
	int ret = ParsePRL(prl, &arr);

	if(ret)
		{
		// Assemble/Create path component
		for(int loop = 0; loop < MAXPRDEPTH; loop++)
			{
			CString *pstr = ( (CString *)arr.GetAt(loop));

			//P2N("PRL Dump %d '%s'\r\n", loop, *pstr);
			pstr->MakeLower();

			if(_chdir(*pstr) < 0)
				{
				if(_mkdir(*pstr) < 0)
					{
					MBOXP("Cannot create PRL path component '%s', event data is not available.", *pstr);
					ret_val = 0;
					}
				else
					{
					if(_chdir(*pstr) < 0)
						{
						MBOXP("Cannot change to PRL path component '%s', event data is not available.", *pstr);
						ret_val = 0;
						}
					}
				}
			}

		// Assemble name component
		CString name; int first = 0;
		for(loop = MAXPRDEPTH; loop <  arr.GetSize() ; loop++)
			{
			CString *pstr = ( (CString *)arr.GetAt(loop));

			pstr->MakeLower();

			//P2N("PRL name %d '%s'\r\n", loop, *pstr);

			if(first++)
				name += "_";
			
			name += *pstr;	
			}

		if(_chdir(name) < 0)
			{
			if(_mkdir(name) < 0)
				{
				MBOXP("Cannot create PRL name component '%s'", name);
				ret_val = 0;
				}
			else
				{
				if(_chdir(name) < 0)
					{
					MBOX("Cannot change to PRL name component '%s'", name);
					ret_val = 0;
					}
				}
			}
		}

	// Free PRL parser strings
	for(int loop = 0; loop < arr.GetSize(); loop++)
		{
		delete (CString *)arr.GetAt(loop);
		}

	return ret_val;
}

void	CleanPRLname(CString &str)

{
	CString tmp;

	for(int loop = 0; loop < str.GetLength(); loop++)
		{
		char chh = str.GetAt(loop);

		if(isalnum(chh))
			tmp +=  chh;
		else if (chh == '\'')
			tmp +=  chh;
		}
	str = tmp;
}

