// xMenu.cpp: implementation of the CxMenu class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "xraynotes.h"
#include "xMenu.h"

#include "mxpad.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CxMenu::CxMenu()
{
	P2N("Created menu %p\r\n", this);
}

CxMenu::~CxMenu()
{

}

void CxMenu::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)

{
	P2N("Called CxMenu::DrawItem %p\r\n", this);
}
