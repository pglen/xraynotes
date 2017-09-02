// UniqID.cpp: implementation of the CUniqID class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "dbtest.h"
#include "UniqID.h"
#include "mxpad.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUniqID::CUniqID()
{
	Create();	
}

CUniqID::~CUniqID()
{

}

CString CUniqID::GetStr()

{
	CString str;
	char  *ptr = (char*) unique;

	for(int loop = 0; loop < 8 * sizeof(long); loop++)
		{
		str += ptr[loop];

		if((loop + 1) % 4 == 0 && loop <  sizeof(unique) - 1)
			str += "-";
		}

	return str;
}

void CUniqID::Create()

{
	char  *ptr = (char*) unique;
	int loop;

	time_t tme;
	tme = time(NULL);

	//P2N("time %x\r\n", tme);

	CString tmp;

	for(loop = 0; loop < 12; loop++)
		{
		int num = rand(), pre = rand();

		//if(pre % 3 == 0)
	//		ptr[loop] = (num % ('Z' - 'A')) + 'a';

		if (pre % 2 == 1)
			ptr[loop] = (num % ('Z' - 'A')) + 'A';
		else
			ptr[loop] = (num % ('9' - '0')) + '0';			
		}

	for(loop = 19; loop >= 12; loop--)
		{
		char cc = tme % 16;

		if(cc >= 10)
			cc += ('A' - 10); 
		else
			cc += '0';

		ptr[loop] =  cc;

		tme /= 16;
		}

	for(loop = 20; loop < sizeof(unique); loop++)
		{
		int num = rand(), pre = rand();

		//if(pre % 3 == 0)
		//	ptr[loop] = (num % ('Z' - 'A')) + 'a';

		if (pre % 2 == 1)
			ptr[loop] = (num % ('Z' - 'A')) + 'A';
		else
			ptr[loop] = (num % ('9' - '0')) + '0';			
		}
}


void CUniqID::Create(const char *str)

{
	char  *ptr = (char*) unique;

	int prog = 0, len = strlen(str);

	for(int loop = 0; loop < len; loop++)
		{
		if(*str != '-')
			{
			prog++;
			*ptr++ = *str;
			}
		str++;

		if(prog >= sizeof(unique))
			break;
		}

	
}

void	CUniqID::Create(CUniqID *id)

{
	memcpy(unique, id->unique, sizeof(unique));
}
