
/* =====[ xStr.h ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the xStr.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  12/22/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// xStr.h: interface for the CxStr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XSTR_H__2F27CD81_DD5E_42A2_B64F_72E22BF7C2F7__INCLUDED_)
#define AFX_XSTR_H__2F27CD81_DD5E_42A2_B64F_72E22BF7C2F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STR_MAGIC	0x987654

#define DIM(x) ( sizeof((x)) / sizeof((x)[0]) )
#define ROUND(x, y) (((x)+(y-1))&~(y-1))

#define CSTR  const char *

//pragma pack (1)

// Make sure the first data is strptr. This is because when there is no 
// casting, this is the value the class returns. (like sprintf - ellipsis)
// Also have no virtual destructor as the vtable goes ahead of this
// tested on VC6 only

// ATTENTION: the note above is obsolete
//

class CxStr   

{

public:
	
	//void		CxStr::DumpAll();

	CxStr();
	CxStr(LPCSTR str);
	CxStr(CxStr &str);

	virtual ~CxStr();

	void	Dump();
	char	*Alloc_buffer(int len, int force = false);
	void	Release_buffer();
	int		Compact();

	// Assigment operators
	const CxStr& operator=(LPCSTR lpsz)
		{ _Assign(lpsz); return *this; }

	//const CxStr& operator=(CString cstr)
	//	{ _Assign(cstr); return *this; }

	// Concat operators
	const CxStr& operator=(CxStr str)
		{ _Assign(str); return *this; }

	const CxStr& operator+=(LPCSTR lpsz)
		{ _Append(lpsz); return *this; }

	// Cast operators
	//operator unsigned char * ()	const { return (unsigned char *)_buffer;	}
	//operator char * ()	const { return _buffer;			}

	operator LPCSTR ()	const { return _buffer; }

	//operator ... ()	const { return _buffer; }
	//operator void * ()	const { return (void *)_buffer;	}
	//operator const void * () const	{ return (void *)_buffer;	}
	
	//////////////////////////////////////////////////////////////////////////
	// Return length if casted to int or long
	operator int  () const	{ return (int)_bufflen;	}	
	operator long () const	{ return (long)_bufflen;	}	
	
	//operator unsigned int  () const { return ( unsigned int)_bufflen;}	
	//operator unsigned long () const { return (unsigned long)_bufflen;}	
	
	// String manipulation
	void	SetAt(int idx, TCHAR chh);

	// String searching
	int		Find(TCHAR ch, int nStart = 0) const;

	// Properties
	int		GetLength() { return _bufflen; }

	// String extraction
	CxStr&	SafeMid(int start);
	CxStr&	SafeMid(int start, int len);

	
	int		LoadString(int id);
	CxStr&	Truncate(int len);

	void ClearLastErr() {lasterror = ""; }

	// Data members

public:

	TCHAR	oldtruncate;

protected:


	//LPTSTR	_buffer;	
	TCHAR	*_buffer;			
	int		_bufflen, _alloclen;
	
	CxStr&	_Append(LPCSTR str);
	CxStr&	_Assign(LPCSTR str) ;
	
	void	_InitVars();

	#include "xStr.inl"

	

// Static members

protected:

	static	char		empty[4];
	static  CPtrArray	CxStr_UseArr;

public:

	static	void		DumpAll();
	static	char		*lasterror;

	int		magic;
};

//pragma pack ()

#endif // !defined(AFX_XSTR_H__2F27CD81_DD5E_42A2_B64F_72E22BF7C2F7__INCLUDED_)
