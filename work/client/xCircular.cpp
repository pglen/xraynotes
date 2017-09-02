// xCircular.cpp: implementation of the CxCircular class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "client.h"
#include "xCircular.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CxCircular::CxCircular()

{
	size = CIRC_DEF_SIZE;
	InitVars();
}

CxCircular::CxCircular(int bufsize)

{
	size = bufsize;
	InitVars();
}

int CxCircular::InitVars()

{
	head = tail = 0;

	buff = (char *)malloc(size);
	ASSERT(buff);

	return 0;
}

CxCircular::~CxCircular()

{
	if(buff) 
		free (buff);
}

//////////////////////////////////////////////////////////////////////////
//
//  ---------------------------------
//	    ^tail        ^head
//
//  ---------------------------------
//	           ^head       ^tail 
//

int CxCircular::Add(char chh)

{
	// Full?
	if (head == tail - 1)
		return false;
	
	// Full? - special case
	if((tail == 0) && (head == size - 1))
		return false;

	buff[head] = chh;

	head++;

	if(head >= size)
		head = 0;

	return true;
}

//////////////////////////////////////////////////////////////////////////

int CxCircular::Get(char *chh)

{
	// Empty?
	if (head == tail)
		return false;

	*chh = 	buff[tail];

	tail++;

	if(tail >= size)
		tail = 0;

	return true;
}

