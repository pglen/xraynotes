
/* =====[ xStr.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the xStr.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  12/22/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// xStr.cpp: implementation of the CxStr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "xraynotes.h"
#include "xStr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#pragma comment( compiler )
#pragma comment( user, "Compiled on " __DATE__ " at " __TIME__  )

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

char		CxStr::empty[4]		= {0,0,0,0};
char		*CxStr::lasterror	= "";

CPtrArray	CxStr::CxStr_UseArr;

char	*err1 = "CxStr::_Assign - Insufficent memory";
char	*err2 = "CxStr::_Append - Insufficent memory";

CxStr::CxStr()

{
	_InitVars();

	CxStr::_PostConstruct();

	P2N("CxStr::CxStr() %p %.12s\r\n", this, _buffer);
}

CxStr::CxStr(CxStr &str) 

{
	_InitVars();
	_Assign(str);

	CxStr::_PostConstruct();

	P2N("CxStr::CxStr() %p %.12s\r\n", this, _buffer);
}

CxStr::CxStr(LPCSTR str) 

{
	_InitVars();
	_Assign(str);
	
	CxStr::_PostConstruct();

	P2N("CxStr::CxStr() %p %.12s\r\n", this, _buffer);
}

CxStr&	CxStr::_Assign(LPCSTR str) 

{
	if(str == NULL)
		return *this;

	int	len = strlen(str);
	int	rlen = ROUND(len + 1, 32);
	
	Alloc_buffer(rlen);

	memcpy(_buffer, str, len);
	_buffer[len] = 0;
	_bufflen = len;

	//P2N("CxStr::_Assign this=%x buff=%x\r\n", this, _buffer);

	return *this;
}

CxStr&	CxStr::_Append(LPCSTR str) 

{
	if(str == NULL)
		return *this;

	int len = strlen(str);
	int	rlen = ROUND(len + 1, 16);
	
	char * buff2 = NULL;
	buff2 = (char*) malloc (_bufflen + len + 1); 
	
	if(buff2 == NULL)
		{
		lasterror = err2;
		P2N("%s\r\n", err1);	
		return *this;
		}

	// Append string back to back
	memcpy(buff2, _buffer, _bufflen);
	memcpy(buff2 + _bufflen, str, len);
	
	buff2[len + _bufflen] = 0;
	_Assign(buff2);
	free (buff2);

	return *this;
}

CxStr::~CxStr()

{
	P2N("CxStr::~CxStr() %p\r\n", this);

	ASSERT(magic == STR_MAGIC);

	if(_buffer != empty)
		{
		free (_buffer);
		_buffer = empty;
		}

	_PostDestruct();
}

CxStr& CxStr::SafeMid(int start)

{
	ASSERT(magic == STR_MAGIC);

	CxStr ss;

	start = min(start, _bufflen);
	ss = _buffer + start;

	#pragma warning( disable : 4172)

	return ss; 

	#pragma warning( default : 4172)
}

CxStr& CxStr::SafeMid(int start, int len)

{
	ASSERT(magic == STR_MAGIC);

	CxStr ss;

	start = min(start, _bufflen);
	len	  = min(len, _bufflen);

	ss = _buffer + start;
	ss.Truncate(len);

	#pragma warning( disable : 4172)

	return ss; 

	#pragma warning( default : 4172)
}

void CxStr::_InitVars()

{
	_buffer = empty;
	_bufflen = _alloclen = 0;
	oldtruncate = _T(' ');
	
	magic = STR_MAGIC;
}

int CxStr::Find(TCHAR ch, int nStart) const

{
	if (nStart >= _bufflen)
		return -1;

	// Find first single character
	LPTSTR lpsz = _tcschr(_buffer + nStart, (_TUCHAR)ch);

	// return -1 if not found and index otherwise
	return (lpsz == NULL) ? -1 : (int)(lpsz - _buffer);
}

//////////////////////////////////////////////////////////////////////////
// Set the string length

CxStr & CxStr::Truncate(int len)

{
	ASSERT(magic == STR_MAGIC);

	ASSERT(len >= 0);

	len = max(len, 0);	len = min(len, _bufflen);

	// Save truncated character
	oldtruncate = _buffer[len];

	_buffer[len] = '\0';

	return *this;
}

//////////////////////////////////////////////////////////////////////////

char	*CxStr::Alloc_buffer(int len, int force)

{
	ASSERT(magic == STR_MAGIC);

	if(len == 0)
		{
		return NULL;
		}

	int	rlen = ROUND(len + 1, 32);

	if( ((len + 1) >= _alloclen) || (_buffer == empty) || force)
		{
		char *buff2 = NULL;
		buff2 = (char *)malloc(rlen);	

		if(buff2 == NULL)
			{
			lasterror = err1;
			P2N("%s\r\n", err1);
			return 0;
			}

		// Preserve old contents:
		if(_buffer != empty)
			{
			memcpy(buff2, _buffer, min(len, _bufflen) + 1);
			free(_buffer);
			}

		_buffer = buff2;
		_alloclen =  rlen;
		}
	return _buffer;
}

int CxStr::LoadString(int id)

{
	ASSERT(magic == STR_MAGIC);

	int ret = 0;

	char	*str = Alloc_buffer(1024);
	ret = ::LoadString(NULL, id, str, 1024);
	
	if(ret >= 0)
		{
		_bufflen = ret;

		}

	Release_buffer();

	Compact();
	
	//*this = str;

	#if 0

	LPCTSTR lpszResourceName = MAKEINTRESOURCE(id);
	ASSERT(lpszResourceName != NULL);

	// Find the Resource:
	HINSTANCE	hInst =	 AfxFindResourceHandle(lpszResourceName, RT_STRING);
	HRSRC		hRsrc =  ::FindResource(hInst,lpszResourceName, RT_STRING);

	if (hRsrc == NULL)
		{
		hInst = NULL;
		hRsrc = ::FindResource(hInst, lpszResourceName, RT_STRING);
		}

	if(hRsrc == NULL)
		{	
		P2N("Could not open resource %d\r\n", lpszResourceName);
		return FALSE;
		}
	
	// Load and assign the Resource:	
	HGLOBAL hGlobal = LoadResource(hInst, hRsrc);
	if(hGlobal == NULL)
		{
		MBOXP("Could not load  resource %d", lpszResourceName);
		return FALSE;
		}	

	LPCSTR str = (LPCSTR) LockResource(hGlobal);
	ASSERT(str);

	if(!str)
		return false;

#endif

	return ret != 0;
}

//////////////////////////////////////////////////////////////////////////
// Get rid of access allocation
// Return how much memory it compacted

int	CxStr::Compact()

{
	if(_bufflen == 0)
		return 0;
		
	if(_alloclen < (_bufflen + 16))
		return 0;

	int old = _alloclen;
	Alloc_buffer(_bufflen, true);

	return old - _alloclen;
}

void	CxStr::DumpAll()

{
#ifdef _DEBUG

	int len = CxStr_UseArr.GetSize();
	for (int loop = 0; loop < len; loop++)
		{
		CxStr *str = (CxStr *)CxStr_UseArr[loop];	
		//ASSERT(str->magic == STR_MAGIC);

		P2N(
		"CxStr::DumpAll this=%p len=%-4d alloc=%-4d buff=%p str='%.12s'...\r\n",
			str, str->_bufflen, str->_alloclen, str->_buffer, str->_buffer);
		}
#endif
}

void	CxStr::Dump()

{
	P2N("CxStr::Dump this=%p len=%d alloc=%d\r\n", 
			this, _bufflen, _alloclen);

	P2N("contents='%s'\r\n\r\n", _buffer);		
}

void	CxStr::Release_buffer()

{
	// NOOP
}

void	CxStr::SetAt(int idx, TCHAR chh)

{
	if(idx < 0 || idx > _bufflen)
		{
		P2N("CxStr::SetAt Index out of bounds\r\n");
		return;
		}
	_buffer[idx] = chh;
}


