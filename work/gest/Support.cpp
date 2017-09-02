// Support.cpp: implementation of the CSupport class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "malloc.h"
#include "direct.h"
#include "aclapi.h"


#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>

#include "mxpad.h"
#include "gest.h"
#include "Support.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CSupport support;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSupport::CSupport()

{
	int	init_perf = 0;
	g_pSetLayeredWindowAttributes = NULL;
}

CSupport::~CSupport()
{

}

void CSupport::Init()

{
	GetAppRoot(approot);
	GetAppData(appdata);

	HMODULE hUser32 = GetModuleHandle(_T("USER32.DLL"));
	g_pSetLayeredWindowAttributes = (lpfnSetLayeredWindowAttributes)
                        GetProcAddress(hUser32, "SetLayeredWindowAttributes");
}

void CSupport::HeapCheck(const char *str, int showok)

{
	CString str2 = str;
	
	int heapstatus = _heapchk();

	//if(str2 == "")
	//	{
	//	str2.Format("%s -- line :: %d ", __FILE__ , __LINE__);
	//	}

   //P2N("Heap check ret = %d\r\n", _heapchk());

	switch( heapstatus )
		{
		case _HEAPOK:
			if(showok)
				P2N("%s Heap Check -- OK - heap is fine\r\n", str2);
			break;

		case _HEAPEMPTY:
			P2N("%s Heap Check -- OK - heap is empty\r\n", str2);
			break;

		case _HEAPBADBEGIN:
			P2N( "%s Heap Check --ERROR - bad start of heap\r\n", str2);
			break;

		case _HEAPBADNODE:
			P2N( "%s Heap Check --ERROR - bad node in heap\r\n", str2);
			break;

		default:
			P2N("%s UNKNOWN - heap ret code\r\n", str2);
			break;
		}
}

//////////////////////////////////////////////////////////////////////////
// Performace counter routines

int		CSupport::MarkPerf()

{
	if(!init_perf)
		{
		init_perf = true;
		QueryPerformanceFrequency(&Frequency);

		//P2N("freq=%d - %d\r\n", Frequency.HighPart, Frequency.LowPart);
		}

	QueryPerformanceCounter(&PerformanceCount);
	//P2N("count = %d - %d\r\n",	PerformanceCount.HighPart, PerformanceCount.LowPart);

	return(0);
}

/////////////////////////////////////////////////////////////////////////////
// Get number of milliseconds since last call to mark

int		CSupport::QueryPerf()

{
	LARGE_INTEGER lastcount;
	LARGE_INTEGER lastcount2;

	QueryPerformanceCounter(&lastcount);
	QueryPerformanceCounter(&lastcount2);

	int elapsed = ((lastcount.LowPart - PerformanceCount.LowPart) * 1000) / Frequency.LowPart;

	// Just to check latency (can be ignored in msec range)
	//int elapsed2 = ((lastcount2.LowPart - PerformanceCount.LowPart) * 1000) / Frequency.LowPart;
	//P2N("Time elapsed: %d msec elapsed2: %d msec\r\n", elapsed, elapsed2); 

	return(elapsed);
}

//////////////////////////////////////////////////////////////////////////
// Get where the application lives

void	CSupport::GetAppRoot(CString &aroot)

{
	char *tmp = aroot.GetBuffer(MAX_PATH + 1); ASSERT(tmp);	
	GetModuleFileName(AfxGetInstanceHandle(), tmp, MAX_PATH);
	aroot.ReleaseBuffer();

	// Get application root
	int idx = aroot.ReverseFind('\\');
	if(idx >= 0)
		aroot = aroot.Mid(0, idx + 1);
}

//////////////////////////////////////////////////////////////////////////
// Get space for tmp data

void	CSupport::GetAppData(CString &adata)

{
	GetSpecialFolder(CSIDL_APPDATA, adata);
	RepairPath(adata);

	adata += appdirname + "\\";

	if(_access(adata, 0) < 0)
		{
		_mkdir(adata);
		}

	if(_access(adata, 0) < 0)
		{
		AfxMessageBox("Cannot access application data storage, switching to c:\\tmp\\");
		appdata = "c:\\tmp\\";
		}
}

void	CSupport::GetSharedData(CString &sdata)

{	
	char *tmp = sdata.GetBuffer(MAX_PATH + 1); ASSERT(tmp);
	GetEnvironmentVariable("ALLUSERSPROFILE", tmp, _MAX_PATH);
	sdata.ReleaseBuffer();

	RepairPath(sdata);
	sdata += appdirname + "\\";

	if(_access(sdata, 0) < 0)
		{
		_mkdir(sdata);
		chmod(sdata, _S_IREAD | _S_IWRITE);
		}

	if(_access(sdata, 0) < 0)
		{
		AfxMessageBox("Cannot access application common data storage, switching.");
		//sdata = "c:\\tmp\\";
		}
}



bool CSupport::CreateDirectoryUserFullAccess(LPCTSTR lpPath)
{
#if 1
	int f = CreateDirectory(lpPath,NULL);
	if(!f)
		return false;
	
	HANDLE hDir = CreateFile(lpPath,READ_CONTROL|WRITE_DAC,0,NULL,OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS,NULL);
	if(hDir == INVALID_HANDLE_VALUE)
		return true;
	
	ACL* pOldDACL=NULL;
	SECURITY_DESCRIPTOR* pSD = NULL;
	GetSecurityInfo(hDir,SE_FILE_OBJECT,DACL_SECURITY_INFORMATION,NULL,NULL,&pOldDACL,NULL,(void**)&pSD);
	
	EXPLICIT_ACCESS ea={0};
	ea.grfAccessMode = GRANT_ACCESS;
	ea.grfAccessPermissions = GENERIC_ALL;
	ea.grfInheritance = CONTAINER_INHERIT_ACE|OBJECT_INHERIT_ACE;
	ea.Trustee.TrusteeType = TRUSTEE_IS_GROUP;
	ea.Trustee.TrusteeForm = TRUSTEE_IS_NAME;
	ea.Trustee.ptstrName = TEXT("Users");
	
	ACL* pNewDACL = NULL;
	SetEntriesInAcl(1,&ea,pOldDACL,&pNewDACL);
	
	SetSecurityInfo(hDir,SE_FILE_OBJECT,DACL_SECURITY_INFORMATION,NULL,NULL,pNewDACL,NULL);
	
	LocalFree(pSD);
	LocalFree(pNewDACL);
	CloseHandle(hDir);
#endif

	return true;
}

//////////////////////////////////////////////////////////////////////////

bool CSupport::ChMod(LPCTSTR lpPath, int mod)

{	
	HANDLE hDir = CreateFile(lpPath,READ_CONTROL|WRITE_DAC,0,NULL,OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS,NULL);

	if(hDir == INVALID_HANDLE_VALUE)
		{
		P2N("Cannot open dir for access control\r\n");
		return true;
		}
	
	ACL* pOldDACL=NULL;
	SECURITY_DESCRIPTOR* pSD = NULL;

	GetSecurityInfo(hDir, SE_FILE_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL,&pOldDACL,NULL,(void**)&pSD);
	
	EXPLICIT_ACCESS ea={0};

	ea.grfAccessMode = GRANT_ACCESS;
	ea.grfAccessPermissions = GENERIC_ALL;
	ea.grfInheritance = CONTAINER_INHERIT_ACE | OBJECT_INHERIT_ACE;
	ea.Trustee.TrusteeType = TRUSTEE_IS_GROUP;
	ea.Trustee.TrusteeForm = TRUSTEE_IS_NAME;
	ea.Trustee.ptstrName = TEXT("Users");
	
	ACL* pNewDACL = NULL;
	SetEntriesInAcl(1, &ea, pOldDACL, &pNewDACL);
	
	SetSecurityInfo(hDir, SE_FILE_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL, pNewDACL, NULL);
	
	LocalFree(pSD);
	LocalFree(pNewDACL);
	CloseHandle(hDir);

	return true;
}


//////////////////////////////////////////////////////////////////////////
// Get where the application data lives

void	CSupport::GetDataRoot(CString &droot)

{
	//char *tmp = droot.GetBuffer(MAX_PATH + 1); ASSERT(tmp);	
	//GetModuleFileName(AfxGetInstanceHandle(), tmp, MAX_PATH);
	//droot.ReleaseBuffer();

	droot = "C:\\xraynotes\\";

	// Get application root
	//int idx = droot.ReverseFind('\\');
	//if(idx >= 0)
	//	dataroot = root.Mid(0, idx + 1);
}

//////////////////////////////////////////////////////////////////////////

long    CSupport::HashString(const char  *name)

{
    unsigned long    ret_val = 0;

    while(*name != '\0')
        {
        ret_val ^= (long)*name;
        ret_val  = ROTATE_LONG_RIGHT(ret_val, 3);          /* rotate right */
        *name++;
        }
    return((long)ret_val);
}

///////////////////////////////////////////////////////////////////////////
// YieldToWinEx
// Let every accumulated message go through
// Return 1 if ESC | Ctrl-Break pressed -- 2 if EXITING

int     CSupport::YieldToWinEx(HWND hWnd)

{
    int ret_val = 0;
    while(TRUE)
        {
        MSG msg;
        if(!PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE))
           break;

        // Ctrl-Break
        if(msg.message == WM_KEYDOWN)
            {
            if(msg.wParam == VK_CANCEL)
                {
                ret_val = 1; break;
                }
            }
        // Esc
        if(msg.message == WM_CHAR)
            {
            if(msg.wParam == VK_ESCAPE)
                {
                ret_val = 1; break;
                }
            }
        // App exit
        if(msg.message == WM_QUIT)
                {
                //wait_exit = TRUE;
                ret_val = 2; break;
                }
        // App kill
        if(msg.message == WM_DESTROY)
                {
                //wait_exit = TRUE;
                ret_val = 2; break;
                }
        // Every other message is processed
        TranslateMessage(&msg); DispatchMessage(&msg);
        }
    return(ret_val);
}

/////////////////////////////////////////////////////////////////////////////////////
//
// This routine is a helper that finds the path to the special folder:
//
// \param SpecialFolder			- an UINT-define (See #defines above or (MSDN))\n
// \param SpecialFolderString	- Reference to a CString that receives the path to the special folder
//
// Returns a BOOL - Found or not \n
//
// CSIDL_APPDATA		File system directory that serves as a common repository for application-specific data.
// CSIDL_ALTSTARTUP  	File system directory that corresponds to the user's nonlocalized Startup program group.
// CSIDL_APPDATA  		File system directory that serves as a common repository for application-specific data.
// CSIDL_BITBUCKET  	File system directory containing file objects in the user's Recycle Bin. The location of this directory is not in the registry; it is marked with the hidden and system attributes to prevent the user from moving or deleting it.
// CSIDL_COMMON_ALTSTARTUP  File system directory that corresponds to the nonlocalized Startup program group for all users.
// CSIDL_COMMON_DESKTOPDIRECTORY  File system directory that contains files and folders that appear on the desktop for all users.
// CSIDL_COMMON_FAVORITES  File system directory that serves as a common repository for all users' favorite items.
// CSIDL_COMMON_PROGRAMS  File system directory that contains the directories for the common program groups that appear on the Start menu for all users.
// CSIDL_COMMON_STARTMENU  File system directory that contains the programs and folders that appear on the Start menu for all users.
// CSIDL_COMMON_STARTUP  File system directory that contains the programs that appear in the Startup folder for all users.
// CSIDL_CONTROLS  		Virtual folder containing icons for the Control Panel applications.
// CSIDL_COOKIES  		File system directory that serves as a common repository for Internet cookies.
// CSIDL_DESKTOP  		Windows Desktop virtual folder at the root of the namespace.
// CSIDL_DESKTOPDIRECTORY  File system directory used to physically store file objects on the desktop (not to be confused with the desktop folder itself).
// CSIDL_DRIVES  		My Computer  virtual folder containing everything on the local computer: storage devices, printers, and Control Panel. The folder may also contain mapped network drives.
// CSIDL_FAVORITES  	File system directory that serves as a common repository for the user's favorite items.
// CSIDL_FONTS  		Virtual folder containing fonts.
// CSIDL_HISTORY  		File system directory that serves as a common repository for Internet history items.
// CSIDL_INTERNET  		Virtual folder representing the Internet.
// CSIDL_INTERNET_CACHE  File system directory that serves as a common repository for temporary Internet files.
// CSIDL_NETHOOD  		File system directory containing objects that appear in the network neighborhood.
// CSIDL_NETWORK  		Network Neighborhood Folder—virtual folder representing the top level of the network hierarchy.
// CSIDL_PERSONAL  		File system directory that serves as a common repository for documents.
// CSIDL_PRINTERS  		Virtual folder containing installed printers.
// CSIDL_PRINTHOOD  	File system directory that serves as a common repository for printer links.
// CSIDL_PROGRAMS  		File system directory that contains the user's program groups (which are also file system directories).
// CSIDL_RECENT  		File system directory that contains the user's most recently used documents.
// CSIDL_SENDTO  		File system directory that contains Send To menu items.
// CSIDL_STARTMENU  	File system directory containing Start menu items.
// CSIDL_STARTUP  		File system directory that corresponds to the user's Startup program group. The system starts these programs whenever any user logs onto Windows NT or starts Windows 95.
// CSIDL_TEMPLATES  	File system directory that serves as a common repository for document templates.
//

BOOL	CSupport::GetSpecialFolder(UINT SpecialFolder, CString &SpecialFolderString)

{
	HRESULT hr;

	LPITEMIDLIST pidl;

	hr = SHGetSpecialFolderLocation(NULL, SpecialFolder, &pidl);

    if(SUCCEEDED(hr))
		{
		// Convert the item ID list's binary representation into a file system path
		char szPath[_MAX_PATH];

		if(SHGetPathFromIDList(pidl, szPath))
			{
			// Allocate a pointer to an IMalloc interface
			LPMALLOC pMalloc;

			// Get the address of our task allocator's IMalloc interface
			hr = SHGetMalloc(&pMalloc);

			// Free the item ID list allocated by SHGetSpecialFolderLocation
			pMalloc->Free(pidl);

			// Free our task allocator
			pMalloc->Release();

			// Work with the special folder's path (contained in szPath)
			SpecialFolderString = szPath;	SpecialFolderString += "\\";
			return TRUE;
			}
		}
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
// Make sure initdir has backslash at the end

void	CSupport::RepairPath(CString &dir)

{
	if(dir.GetLength())
		if(dir.GetAt(dir.GetLength() - 1) != '\\')
			dir += "\\";
}

/////////////////////////////////////////////////////////////////////////////
// Wrapper for split path

void	CSupport::SplitPath(const char *full,  CString &drive,
					CString &dir, CString &fname, CString &ext)

{
	char sdrive[_MAX_DRIVE];
	char sdir[_MAX_DIR];
	char sfname[_MAX_FNAME];
	char sext[_MAX_EXT];
	
	//Parse it
	_splitpath(full, sdrive, sdir, sfname, sext);

	P2N("path components sdrive=%s sdir=%s  sfname=%s sext=%s\r\n",
							sdrive, sdir, sfname, sext);

	drive = sdrive;
    dir   = sdir;
    fname = sfname;
    ext   = sext;
}

//////////////////////////////////////////////////////////////////////////////
// Convert full path to file extension

CString CSupport::GetExtension(const CString& name)

{
	int ii, len = name.GetLength();
	
	for (ii = len-1; ii >= 0; ii--)
		{
		if (name[ii] == '.')
			{
			return name.Mid(ii + 1);
			}
		}
	return CString("");
}

////////////////////////////////////////////////////////////////////////////////
// Convert full path to file name and extension

CString CSupport::GetFileNameExt(const CString& name)

{
	int len = name.GetLength();
	int i;
	for (i = len-1; i >= 0; i--)
		{
		if (name[i] == '\\')
			{
			return name.Right((len - i) -1 );
			}
		}
	return CString(name);
}

////////////////////////////////////////////////////////////////////////////////
// Convert full path to directory

CString CSupport::GetDir(const CString& name)

{
	int len = name.GetLength();
	int i;
	for (i = len-1; i >= 0; i--)
		{
		if (name[i] == '\\')
			{
			return name.Left(i+1);
			}
		}
	return CString("");
}

static char    asztmp[512];

int     CSupport::AfxPrintf(const char *Format, ...)

{
    char    *asc = NULL;
    va_list ArgList;

    va_start(ArgList, Format);
    _vsnprintf(asztmp, sizeof(asztmp), Format, ArgList);

    AfxMessageBox(asztmp);

    return(0);
}

void CSupport::SetTrans(HWND m_hWnd, BYTE bAlpha)

{
	SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);

	if (bAlpha < 255)
		{
        //  Call it with 255 as alpha - opacity
		if(g_pSetLayeredWindowAttributes)
			g_pSetLayeredWindowAttributes(m_hWnd, 0, bAlpha, LWA_ALPHA);
		}
}
