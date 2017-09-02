// XFX.cpp : Implementation of CXSXApp and DLL registration.

#include "stdafx.h"
#include "XSX.h"
#include "XFX.h"

/////////////////////////////////////////////////////////////////////////////
//

STDMETHODIMP XFX::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IXFX,
	};

	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}
