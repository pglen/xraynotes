// XFX.h: Definition of the XFX class
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XFX_H__AF9595DD_2151_4BB9_A637_031192588510__INCLUDED_)
#define AFX_XFX_H__AF9595DD_2151_4BB9_A637_031192588510__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// XFX

class XFX : 
	public IDispatchImpl<IXFX, &IID_IXFX, &LIBID_XSXLib>, 
	public ISupportErrorInfo,
	public CComObjectRoot,
	public CComCoClass<XFX,&CLSID_XFX>
{
public:
	XFX() {}
BEGIN_COM_MAP(XFX)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IXFX)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()
//DECLARE_NOT_AGGREGATABLE(XFX) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation. 

DECLARE_REGISTRY_RESOURCEID(IDR_XFX)
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IXFX
public:
};

#endif // !defined(AFX_XFX_H__AF9595DD_2151_4BB9_A637_031192588510__INCLUDED_)
