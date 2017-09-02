
/* =====[ UniqID.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the UniqID.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  1/19/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// UniqID.cpp: implementation of the CUniqID class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "xraynotes.h"
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

void	CUniqID::Copy(CUniqID *id)

{
	memcpy(unique, id->unique, sizeof(unique));
}


int CUniqID::Compare(CUniqID *other)

{
	return( memcmp(unique, other->unique, sizeof(unique)) == 0);
}

//////////////////////////////////////////////////////////////////////////

void	*CUniqID::Pack()

{
	void *ppp;	
	Pack(&ppp);
	return ppp;
}

//////////////////////////////////////////////////////////////////////////
// Asemble the object into memory. Use unPack to reverse the process.

void CUniqID::Pack(void **ptr)

{
	char *mem = new char [ sizeof(unique) +  2 * sizeof(int)];

	*ptr = (void*)mem;

	ASSERT(mem);
	
	*((int*)mem) = sizeof(unique);
	mem += sizeof(int);

	*((long*)mem) = PACK_MAGIC;
	mem += sizeof(int);

	memcpy(mem, unique, sizeof(unique));
}

//////////////////////////////////////////////////////////////////////////
// Asemble the object into memory

void CUniqID::unPack(void *ptr)

{
	char *mem = (char*)ptr;

	int len = *((long*)mem);
	if(len != sizeof(unique))
		{
		P2N("CUniqID::unPack Unpack with wrong mem size\r\n");
		}
	mem += sizeof(int);

	if(*((long*)mem) != PACK_MAGIC)
		{
		P2N("CUniqID::unPack Unpack with wrong magic number\r\n");
		}
	mem += sizeof(int);

	memcpy(unique, mem, sizeof(unique));
}

//////////////////////////////////////////////////////////////////////////
// Free the allocated storage

void CUniqID::PackFree(void *ptr)

{
	char *mem = (char*)ptr;

	int len = *((long*)mem);
	if(len != sizeof(unique))
		{
		P2N("CUniqID::PackFree Unpack with wrong mem size\r\n");
		}
	mem += sizeof(int);

	if(*((long*)mem) != PACK_MAGIC)
		{
		P2N("CUniqID::PackFree Unpack with wrong magic number\r\n");
		}
	mem += sizeof(int);

	delete [] (char *) ptr;
}

