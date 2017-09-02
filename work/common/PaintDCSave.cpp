// ClientDCSave.cpp: implementation of the CPaintDCSave class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "PaintDCSave.h"
#include "../common/mxpad.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPaintDCSave::CPaintDCSave(CWnd* pWnd) : CPaintDC(pWnd)

{
	//P2N("CPaintDCSave(CWnd* pWnd)\r\n");
	save = SaveDC();
}
	

CPaintDCSave::~CPaintDCSave()
{
	RestoreDC(save);
	//P2N("~CPaintDCSave(CWnd* pWnd)\r\n");
}
