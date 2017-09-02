
/* =====[ Support.h ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the Support.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  11/26/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

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

// Limit number between two boudaries
#define LIM(llim, hlim, num)  ( (num < llim) ? llim : (num > hlin) ? hlim : num )

#define SQR(aa) (aa * aa)		// Square
#define SQRT sqrt				// Square root

#define	CASTINT(val) ((int)(val))
#define	MATHPI  3.1415926535

#define	XDELETE(aa) {if(aa != NULL) { delete aa; aa = NULL;}}

#define WS_EX_LAYERED 0x00080000 
//#define LWA_COLORKEY 1 // Use color as the transparency color.
//#define LWA_ALPHA    2 // Use bAlpha to determine the opacity of the layer

//  Function pointer for lyering API in User32.dll
typedef BOOL (WINAPI *lpfnSetLayeredWindowAttributes)
            (HWND hWnd, COLORREF cr, BYTE bAlpha, DWORD dwFlags);

//void	Help();

class CSupport  

{

#ifdef _DEBUG
	CMemoryState oldMemState, newMemState, diffMemState;
#endif

public:

	int		GetDaysUsed();
	void	GetEnvStr(const char *env, CString &str);
	void	GetAppDataDir(CString &adata);
	void	GetSharedDocDir(CString &sdata);
	void	GetModuleBaseName(CString &bname);
	int     AfxPrintf(const char *Format, ...);

	void	ShowLeaks();
	void	CheckPoint();
	const char	*Err2Str(int errcode = 0);
	void	GetTmpDir(CString &tmproot);
	void	RepairPath(CString &dir);
	bool	ChMod(LPCTSTR lpPath, int mod);
	bool	CreateDirectoryUserFullAccess(LPCTSTR lpPath);
	void	GetSharedData(CString &sdata);
	void	CSupport::GetAppData(CString &adata);
	BOOL	GetSpecialFolder(UINT SpecialFolder, CString &SpecialFolderString);
	void	GetAppRoot(CString &aroot);
	void	GetDataRoot(CString &droot);
	long    HashString(const char  *name);
	int		QueryPerf();
	int		MarkPerf();
	void	HeapCheck(const char *str, int showok = false);
	void	HeapDump(const char *str2, int showok = false);
	int     YieldToWinEx(HWND hWnd = NULL);
	int		IsAdmin( );

	CSupport();
	virtual ~CSupport();

	void	PathToDir(CString &docname);
	void	PathToFname(CString &docname);
	void	PathToExt(CString  &fname);

	CString GetExtension(const CString& name);
	CString GetFilename(const CString& name);
	CString GetFileNameExt(const CString& name);
	CString GetDirName(const CString& name);

	void	SplitPath(const CString &full,  CString &drive,
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
	
protected:

	//char	err[_MAX_PATH];
	CString errstr;

	LARGE_INTEGER Frequency, PerformanceCount;
	int	init_perf;
};

extern CSupport support;

#endif // !defined(AFX_SUPPORT_H__79D62B02_F5B5_4F15_BF58_4C808471BEB7__INCLUDED_)
