
/* =====[ Support.h ]========================================== 
                                                                             
   Description:     The gest project, implementation of the Support.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  3/20/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#if !defined(AFX_SUPPORT_H__79D62B02_F5B5_4F15_BF58_4C808471BEB7__INCLUDED_)
#define AFX_SUPPORT_H__79D62B02_F5B5_4F15_BF58_4C808471BEB7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef ROTATE_LONG_RIGHT
#define ROTATE_LONG_RIGHT(xx, rot) ( (xx << rot) | (xx >> (32 - rot)) )
#endif

//////////////////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////////////////

#ifndef TRUE
#define TRUE (1 == 1)
#endif

#ifndef FALSE
#define FALSE (1 != 1)
#endif

//////////////////////////////////////////////////////////////////////////

// Limit number between two boudaries
#define LIM(llim, hlim, num)  ( (num < llim) ? llim : (num > hlin) ? hlim : num )

#define SQR(aa) (aa * aa)		// Square
#define SQRT sqrt				// Square root

#define	CASTINT(val) ((int)(val))
#define	MATHPI  3.1415926535

#define WS_EX_LAYERED 0x00080000 
#define LWA_COLORKEY 1 // Use color as the transparency color.
#define LWA_ALPHA    2 // Use bAlpha to determine the opacity of the layer

#define MBOXP		support.AfxPrintf

//  Function pointer for lyering API in User32.dll
typedef BOOL (WINAPI *lpfnSetLayeredWindowAttributes)
            (HWND hWnd, COLORREF cr, BYTE bAlpha, DWORD dwFlags);

extern	lpfnSetLayeredWindowAttributes g_pSetLayeredWindowAttributes ;

//////////////////////////////////////////////////////////////////////////

class CSupport  

{
public:

	void CSupport::SetTrans(HWND m_hWnd, BYTE bAlpha);

	CString approot;
	CString appdata;

	void CSupport::Init();

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
	
	void	CSupport::SplitPath(const char *full,  CString &drive,
					CString &dir, CString &fname, CString &ext);
	CString CSupport::GetDir(const CString& name);
	CString CSupport::GetFileNameExt(const CString& name);
	CString CSupport::GetExtension(const CString& name);

	int     CSupport::AfxPrintf(const char *Format, ...);

	CSupport();
	virtual ~CSupport();

protected:

	lpfnSetLayeredWindowAttributes g_pSetLayeredWindowAttributes;

	LARGE_INTEGER Frequency, PerformanceCount;
	int	init_perf;
};

extern CSupport support;

#endif // !defined(AFX_SUPPORT_H__79D62B02_F5B5_4F15_BF58_4C808471BEB7__INCLUDED_)
