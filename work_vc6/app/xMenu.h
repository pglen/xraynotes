// xMenu.h: interface for the CxMenu class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMENU_H__D392C68A_CEB7_4016_877E_38A5BB7AEFE3__INCLUDED_)
#define AFX_XMENU_H__D392C68A_CEB7_4016_877E_38A5BB7AEFE3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CxMenu : public CMenu  
{
public:
	CxMenu();

	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	virtual ~CxMenu();

};

#endif // !defined(AFX_XMENU_H__D392C68A_CEB7_4016_877E_38A5BB7AEFE3__INCLUDED_)
