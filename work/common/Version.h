// Version.h: interface for the CVersion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VERSION_H__858DCCBA_3D17_4D97_8C5C_18DB0D84FD94__INCLUDED_)
#define AFX_VERSION_H__858DCCBA_3D17_4D97_8C5C_18DB0D84FD94__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CVersion : OSVERSIONINFO 
{
public:
	CVersion();
	virtual ~CVersion();

	CVersion::Dump();
	CVersion::DumpEx();

protected:

	OSVERSIONINFOEX osvx; 

};

#endif // !defined(AFX_VERSION_H__858DCCBA_3D17_4D97_8C5C_18DB0D84FD94__INCLUDED_)
