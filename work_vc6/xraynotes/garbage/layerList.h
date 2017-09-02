// layerList.h: interface for the ClayerList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LAYERLIST_H__FEBA5E47_28A7_485E_AB06_7308FEB76D62__INCLUDED_)
#define AFX_LAYERLIST_H__FEBA5E47_28A7_485E_AB06_7308FEB76D62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class ClayerList  
{
public:

	CPtrList	list;

	//CPtrList list();

	ClayerList();
	virtual ~ClayerList();

};

#endif // !defined(AFX_LAYERLIST_H__FEBA5E47_28A7_485E_AB06_7308FEB76D62__INCLUDED_)
