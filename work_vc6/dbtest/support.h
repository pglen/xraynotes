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

/////////////////////////////////////////////////////////////////////////////
// suport.h : Services for applications
//

////////////////////////////////////////////////////////////////
// General defines

#ifndef MIN
#define MIN(a,b) (((a)<(b))?(a):(b))
#endif

#ifndef MAX
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif

#ifndef ABS
#define ABS(a) (((a)<0) ? -(a) : (a))
#endif

#ifndef TRUE
#define TRUE (1 == 1)
#endif

#ifndef FALSE
#define FALSE (1 != 1)
#endif

#define		CASTINT(val) ((int)(val))
#define		MATHPI  3.1415926535

#ifndef MBOX
#define MBOX		xMsgBox
#define MBOXP		support.AfxPrintf
#endif


#ifndef WS_EX_LAYERED
#define WS_EX_LAYERED           0x00080000
#define LWA_COLORKEY            0x00000001
#define LWA_ALPHA               0x00000002
#endif // ndef WS_EX_LAYERED

// Preparation for the function we want to import from USER32.DLL
typedef BOOL (WINAPI *lpfnSetLayeredWindowAttributes)(HWND hWnd, 
                                  COLORREF crKey, BYTE bAlpha, DWORD dwFlags);

extern	lpfnSetLayeredWindowAttributes m_pSetLayeredWindowAttributes;


//void	Help();

class CSupport  

{
public:

	void	GetTmpDir(CString &tmproot);
	void	RepairPath(CString &dir);
	bool	ChMod(LPCTSTR lpPath, int mod);
	bool	CreateDirectoryUserFullAccess(LPCTSTR lpPath);
	
	void	GetSharedDocDir(CString &sdata);
	void	GetSharedDataDir(CString &sdata);
	void	GetAppDataDir(CString &adata);
	void	GetAppRoot(CString &aroot);

	void	GetDataRoot(CString &droot);
	long    HashString(const char  *name);
	int		QueryPerf();
	int		MarkPerf();
	void	HeapCheck(const char *str, int showok);
	int     YieldToWinEx(HWND hWnd = NULL);
	int		IsAdmin( );
	void	GetModuleBaseName(CString &bname);

	BOOL	GetSpecialFolder(UINT SpecialFolder, CString &SpecialFolderString);
	

	CSupport();
	virtual ~CSupport();

	void	PathToDir(CString &docname);
	void	PathToFname(CString &docname);
	void	PathToExt(CString  &fname);

	CString GetExtension(const CString& name);
	CString GetFilename(const CString& name);
	CString GetFileNameExt(const CString& name);
	CString GetDirName(const CString& name);

	void	SplitPath(CString &full,  CString &drive,
					CString &dir, CString &fname, CString &ext);

	void	CSupport::Help();
	int		CSupport::IsDir(const char *str);
	int		CSupport::IsFile(const char *str);

	CString CSupport::EscapeComma(CString &str);
	CString CSupport::EscapeSql(CString &str);
	void	CSupport::SetLocMachineInt(const char *str, const char *key, int val);
	int		CSupport::GetLocMachineInt(const char *str, const char *key);
	void	CSupport::SetRegRootInt(const char *str, const char *key, int val);
	int		CSupport::GetRegRootInt(const char *str, const char *key);
	int	 	CSupport::FindNumOfChar(CString &str, CString &mus, int start);
	void	CSupport::ShortenPath(CString &str, int len);
	CString CSupport::GetCurrentDir();
	int     CSupport::AfxPrintf(const char *Format, ...);

protected:

	LARGE_INTEGER Frequency, PerformanceCount;
	int	init_perf;
};

extern CSupport support;

#endif // !defined(AFX_SUPPORT_H__79D62B02_F5B5_4F15_BF58_4C808471BEB7__INCLUDED_)
