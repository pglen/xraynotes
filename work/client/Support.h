// Support.h: interface for the CSupport class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SUPPORT_H__79D62B02_F5B5_4F15_BF58_4C808471BEB7__INCLUDED_)
#define AFX_SUPPORT_H__79D62B02_F5B5_4F15_BF58_4C808471BEB7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef ROTATE_LONG_RIGHT
#define ROTATE_LONG_RIGHT(xx, rot) ( (xx << rot) | (xx >> (32 - rot)) )
#endif

void	Help();

class CSupport  

{
public:
	
	void	HeapDump();
	void	RepairPath(CString &dir);
	bool	ChMod(LPCTSTR lpPath, int mod);
	bool	CreateDirectoryUserFullAccess(LPCTSTR lpPath);
	void	GetSharedData(CString &sdata);
	void	GetAppData(CString &appdata);
	BOOL	GetSpecialFolder(UINT SpecialFolder, CString &SpecialFolderString);
	void	GetAppRoot(CString &approot);
	void	GetDataRoot(CString &droot);
	long    HashString(const char  *name);
	int		QueryPerf();
	int		MarkPerf();
	void	HeapCheck(const char *str, int showok);
	int     YieldToWinEx(HWND hWnd = NULL);
	
	CString appdirname, appdata;

	CSupport();
	virtual ~CSupport();

protected:

	LARGE_INTEGER Frequency, PerformanceCount;
	int	init_perf;
};

extern CSupport support;

#endif // !defined(AFX_SUPPORT_H__79D62B02_F5B5_4F15_BF58_4C808471BEB7__INCLUDED_)
