
/* =====[ Support.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the Support.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  5/20/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// Support.cpp: implementation of the CSupport class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include <malloc.h>
#include <direct.h>
#include <aclapi.h>
#include <winsvc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include "support.h"
#include "../common/mxpad.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CSupport support;

CString appdirname, appdata;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSupport::CSupport()

{
	int	init_perf = 0;
}

CSupport::~CSupport()
{

}

void CSupport::HeapDump(const char *str2, int showok)

{
	// LEAK
	_HEAPINFO hi;

	malloc(12);

	memset(&hi, 0, sizeof(hi));

	P2N("Heap Walk\r\n");

	int cnt = 0;
	while (true)
		{
		int ret = _heapwalk(&hi);

		switch( ret )
			{
			case _HEAPOK:
				P2N("%p %4d %s, ", hi._pentry, hi._size, hi._useflag == _USEDENTRY ? "USED" : "FREE" );

				if(++cnt % 6 == 0)
					P2N("\r\n");
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

			case _HEAPEND:
				P2N( "\r\n%s Heap Check --END\r\n", str2);
				goto endd;
				break;

			default:
				P2N("%s UNKNOWN - heap ret code\r\n", str2);
				break;
			}
		}
endd:
	//P2N("\r\nEnd heap walk.\r\n");
	;
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
	GetEnvStr("ALLUSERSPROFILE", sdata);

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

//

void	CSupport::GetTmpDir(CString &tmproot)

{
	CString newdir; 
	
	// Get temp
	char *tmp = newdir.GetBuffer(_MAX_PATH + 1); ASSERT(tmp);	
	GetTempPath(_MAX_PATH, tmp);
	newdir.ReleaseBuffer();
	
	// Convert it
	char *tmp2 = tmproot.GetBuffer(_MAX_PATH + 1); ASSERT(tmp2);	
	GetLongPathName(newdir, tmp2, _MAX_PATH);
	tmproot.ReleaseBuffer();
}

//////////////////////////////////////////////////////////////////////////
// Get where the application data lives

void	CSupport::GetDataRoot(CString &droot)

{
	//char *tmp = droot.GetBuffer(MAX_PATH + 1); ASSERT(tmp);	
	//GetModuleFileName(AfxGetInstanceHandle(), tmp, MAX_PATH);
	//droot.ReleaseBuffer();

	droot = "c:\\XrayNotes\\";

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

int CSupport::IsAdmin( )

{
    SC_HANDLE hSC;

	// Try an Admin Privileged API - if it works, return TRUE, else FALSE
    hSC = OpenSCManager(
        NULL,
        NULL,
        GENERIC_READ | GENERIC_WRITE | GENERIC_EXECUTE  );

    if ( hSC == NULL ) 
        return FALSE;

    CloseServiceHandle( hSC );
    return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// Shorten path to directory name

void	CSupport::PathToDir(CString &docname)

{
	int idx;
	CString filename;

	if((idx = docname.ReverseFind('\\')) != -1)
		{
		filename = docname.Left(idx+1);
		docname = filename;
		}
}

/////////////////////////////////////////////////////////////////////////////
// Shorten path to file name 

void	CSupport::PathToFname(CString &docname)

{
	int idx;
	CString filename;

	if((idx = docname.ReverseFind('\\')) != -1)
		{
		filename = docname.Right(docname.GetLength() - (idx + 1));
		docname = filename;
		}
}


void	CSupport::PathToExt(CString  &fname)

{
	int idx;
	CString filename;

	if((idx = fname.ReverseFind('.')) != -1)
		{
		filename = fname.Right(fname.GetLength() - (idx + 1));
		fname = filename;
		}
}


//////////////////////////////////////////////////////////////////////////////
// 

CString CSupport::GetExtension(const CString& name)

{
	int len = name.GetLength();
	int i;
	for (i = len-1; i >= 0; i--){
		if (name[i] == '.'){
			return name.Mid(i+1);
		}
	}
	return CString("");
}

//////////////////////////////////////////////////////////////////////////////

CString CSupport::GetFilename(const CString& name)

{
	int len = name.GetLength();
	int i;
	for (i = len-1; i >= 0; i--){
		if (name[i] == '.')
			{
			return name.Left(i);
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
// Convert full path to directory name

CString CSupport::GetDirName(const CString& name)

{
	int len = name.GetLength();
	int i;
	for (i = len-1; i >= 0; i--)
		{
		if (name[i] == '\\')
			{
			return name.Left(i);
			}
		}
	return CString("");
}


/////////////////////////////////////////////////////////////////////////////
// Wrapper for split path

void	CSupport::SplitPath(const CString &full,  CString &drive,
					CString &dir, CString &fname, CString &ext)

{
	//char sbuffer[_MAX_PATH + 1];
	char sdrive[_MAX_DRIVE];
	char sdir[_MAX_DIR];
	char sfname[_MAX_FNAME];
	char sext[_MAX_EXT];
	
	//strncpy(sbuffer, full, MAX_PATH);

	//Parse it
	_splitpath(full, sdrive, sdir, sfname, sext);

	//P2N("path components sdrive=%s sdir=%s  sfname=%s sext=%s\r\n",
	//						sdrive, sdir, sfname, sext);

	drive = sdrive;
    dir   = sdir;
    fname = sfname;
    ext   = sext;
}


void	CSupport::Help()

{
	CString approot; GetAppRoot(approot);
	P2N("Approot='%s'\r\n", approot);

	// Execute winhelp
	CString str, file("index.html");

#ifdef _DEBUG
	str = "Z:/archive/xraynotes/umanual/" + file;
#else
	str =  approot + "umanual\\" + file;
#endif

	ShellExecute(NULL, "open", str, NULL, NULL, SW_SHOWMAXIMIZED);
}


/////////////////////////////////////////////////////////////////////////
// return true if it is a directory 

int		CSupport::IsDir(const char *str)

{
	int ret = 0;

	struct _stat buff; 
	if(_stat( str, &buff) < 0)
		{
		//P2N("Not stat\r\n");
		goto endd;
		}

	if(buff.st_mode & _S_IFDIR)
		{
		//P2N("Not dir\r\n");
		ret = true;
		}
endd:

	return ret;
}

/////////////////////////////////////////////////////////////////////////
// Return true if it is a file

int		CSupport::IsFile(const char *str)

{
	int ret = 0;

	struct _stat buff; 
	if(_stat( str, &buff) < 0)
		{
		//P2N("Not stat\r\n");
		goto endd;
		}

	if(buff.st_mode & _S_IFREG)
		{
		//P2N("Not dir\r\n");
		ret = true;
		}
endd:

	return ret;
}

//////////////////////////////////////////////////////////////////////////

int		CSupport::GetRegRootInt(const char *str, const char *key)

{
	HKEY	Result;
	DWORD	oldtime = 0;
	DWORD	plen = sizeof(DWORD);
	DWORD	Type = 0;

	LONG ret2 = RegOpenKey(HKEY_CLASSES_ROOT, str, &Result);

	if(ret2 == ERROR_SUCCESS)
		{
		RegQueryValueEx(Result, key, NULL, &Type, 
							(unsigned char*)&oldtime, &plen);
		RegCloseKey(Result);
		}
	return oldtime;
}

//////////////////////////////////////////////////////////////////////////

void	CSupport::SetRegRootInt(const char *str, const char *key, int val)

{
	HKEY	Result;
	DWORD	oldtime = 0;
	DWORD	plen = sizeof(DWORD);

	LONG ret2 = RegOpenKey(HKEY_CLASSES_ROOT, str, &Result);

	if(ret2 != ERROR_SUCCESS)
		{
		/// Create key
		ret2 = RegCreateKeyEx(HKEY_CLASSES_ROOT, str, 0,
									NULL, 0, KEY_ALL_ACCESS, NULL, &Result, NULL);
		}

	if(ret2 == ERROR_SUCCESS)
		{
		RegSetValueEx(Result, key, NULL, REG_DWORD, 
								(unsigned char*)&val, sizeof(DWORD));
		RegCloseKey(Result);
		}
}

//////////////////////////////////////////////////////////////////////////

int		CSupport::GetLocMachineInt(const char *str, const char *key)

{
	HKEY	Result;
	DWORD	oldtime = 0;
	DWORD	plen = sizeof(DWORD);
	DWORD	Type = 0;

	LONG ret2 = RegOpenKey(HKEY_LOCAL_MACHINE, str, &Result);

	if(ret2 == ERROR_SUCCESS)
		{
		RegQueryValueEx(Result, key, NULL, &Type, 
							(unsigned char*)&oldtime, &plen);
		RegCloseKey(Result);
		}
	return oldtime;
}

void	CSupport::SetLocMachineInt(const char *str, const char *key, int val)

{
	HKEY	Result;
	DWORD	oldtime = 0;
	DWORD	plen = sizeof(DWORD);

	LONG ret2 = RegOpenKey(HKEY_LOCAL_MACHINE, str, &Result);

	if(ret2 != ERROR_SUCCESS)
		{
		/// Create key
		ret2 = RegCreateKeyEx(HKEY_LOCAL_MACHINE, str, 0,
									NULL, 0, KEY_ALL_ACCESS, NULL, &Result, NULL);
		}

	if(ret2 == ERROR_SUCCESS)
		{
		RegSetValueEx(Result, key, NULL, REG_DWORD, 
								(unsigned char*)&val, sizeof(DWORD));
		RegCloseKey(Result);
		}
}

////////////////////////////////////////////////////////////////////////////

CString CSupport::EscapeSql(CString &str)

{
	CString  res;

	int len = str.GetLength();

	for (int loop = 0; loop < len; loop++)
		{
		char chh = str.GetAt(loop);
		res +=  chh;
		if(chh == '\'')
			res +=  chh;
		}
	return res;
}


CString CSupport::EscapeComma(CString &str)

{
	CString  res;

	int len = str.GetLength();

	for (int loop = 0; loop < len; loop++)
		{
		char chh = str.GetAt(loop);

		switch(chh)
			{
			case '\r':
				res +=  "\\r";
				break;

			case '\n':
				res +=  "\\n";
				break;

			case ',':
				res +=  ";";
				break;

			default:
				res +=  chh;
				break;
			}
		}
	return res;
}


////////////////////////////////////////////////////////////
// Find how many characters in starting

int	 	CSupport::FindNumOfChar(CString &str, CString &mus, int start)

{
	int ret = 0, loop;
	int lim = str.GetLength();

	for(loop = start; loop < lim; loop++)
		{
		if(mus.Find(str.GetAt(loop)) >=0)
			{
			ret++;
			}
		}
	return(ret);
}


/////////////////////////////////////////////////////////////////////////////
//  void	ShortenPath(CString &str, int len)
// Shorten path to len

void	CSupport::ShortenPath(CString &str, int len)

{
	int slen = str.GetLength();

	// Nothing to do
	if(slen <= len)
		return;

	CString lstr = str.Left ((len-5)/2);
	CString rstr = str.Right((len-5)/2);
	str = lstr + " ... " + rstr;
}


/////////////////////////////////////////////////////////////////////////////
// Wrapper for get current dir

CString CSupport::GetCurrentDir()

{
	CString dir;
	char buffer[_MAX_PATH + 4];

	getcwd(buffer, _MAX_PATH);

	// Fix up path for split
	if(strlen(buffer) > 3 )
	 	strcat(buffer, "\\");

	dir = buffer;
	return(dir);
}

#if 0

	CString cenv; GetEnvStr("PATH", cenv);

	int len = cenv.GetLength();

	int prog = 0;
	while(true)
		{
		int idx = cenv.Find(";", prog);

		if(idx < 0)
			{
			CString sub = cenv.Mid(prog);
			P2N("last sub: %s\r\n", sub);
			TryExec(sub, fstr);
			break;
			}

		CString sub = cenv.Mid(prog, idx - prog);
		//P2N("sub: %s\r\n", sub);
		if(TryExec(sub, fstr))
			break;

		prog =  idx + 1;

		// Safety net
		if(prog >= len)
			break;
		}

tryexec:

	CString cstr(str);
	
	cstr += "\\notepad.exe";

	//P2N("TryExec %s %s\r\n", cstr, arg);

	if(_access(cstr, 00) >= 0)
		{
		P2N("TryExec OK %s %s\r\n", cstr, arg);

		int ret = _spawnl(_P_NOWAIT, cstr, arg, NULL);
		P2N("spawn returned %d\r\n", ret);

		return 1;
		}

	return 0;
#endif

//////////////////////////////////////////////////////////////////////////
// Convert the error value into a string 

const char *CSupport::Err2Str(int errcode)

{
	LPVOID lpMsgBuf = NULL;

	if(errcode == 0)
		errcode = GetLastError();

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | 
					FORMAT_MESSAGE_FROM_SYSTEM | 
						FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, errcode,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
					(LPTSTR) &lpMsgBuf,	0, NULL );

	// Kill last new line
	char *zz = strrchr((char*)lpMsgBuf, '\r');
	if(zz) 
		*zz = '\0';

	errstr = (char*)lpMsgBuf;
	LocalFree( lpMsgBuf );

	return errstr;
}

void CSupport::CheckPoint()

{

#ifdef _DEBUG
	
	P2N("CSupport::CheckPoint\r\n");
	oldMemState.Checkpoint();

#endif

}

void	CSupport::ShowLeaks()

{

	//malloc(10);

#ifdef _DEBUG
    
	P2N("memstate dump\r\n");

	newMemState.Checkpoint();
    
	if(diffMemState.Difference( oldMemState, newMemState ) )
		{
        TRACE( "Memory leaked!\n" );
		}
	
	diffMemState.DumpAllObjectsSince();

	P2N("dump end\r\n");

#endif

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

void	CSupport::GetModuleBaseName(CString &bname)

{
	CString temp;

	char *tmp = temp.GetBuffer(MAX_PATH + 1); ASSERT(tmp);	
	GetModuleFileName(AfxGetInstanceHandle(), tmp, MAX_PATH);
	temp.ReleaseBuffer();
	
	temp = GetFileNameExt(temp);
	temp = GetFilename(temp);

	bname = temp;
}

void	CSupport::GetSharedDocDir(CString &sdata)

{	
	CString ttt;

#if 0
	GetEnvStr("ALLUSERSPROFILE", sdata);
	RepairPath(sdata);
	CString base; GetModuleBaseName(base);
	ttt += "Documents\\" + base + "\\";
	if(_access(ttt, 0) < 0)
		{
		_mkdir(ttt);
		chmod(ttt, _S_IREAD | _S_IWRITE);
		}

	if(_access(ttt, 0) < 0)
		{
		AfxMessageBox("Cannot access application common document storage.");
		//sdata = "c:\\tmp\\";
		}
	sdata = ttt;
#endif

	sdata = "c:\\xraynotes\\";
}

//////////////////////////////////////////////////////////////////////////
// Get space for tmp data

void	CSupport::GetAppDataDir(CString &adata)

{
	GetSpecialFolder(CSIDL_APPDATA, adata);
	RepairPath(adata);
	
	CString base;
	GetModuleBaseName(base);

	//P2N("got module base name %s\r\n", temp);

	adata += base + "\\";
	
	if(_access(adata, 0) < 0)
		{
		_mkdir(adata);
		}

	if(_access(adata, 0) < 0)
		{
		AfxMessageBox("Cannot access application data storage, switching to c:\\tmp\\");
		adata = "c:\\tmp\\";
		}
}

void CSupport::GetEnvStr(const char *env, CString &str)

{
	char tmp[12];
	
	int ret = GetEnvironmentVariable(env, tmp, sizeof(tmp));	

	if(ret > 0)
		{
		char *pstr = str.GetBuffer(ret); ASSERT(pstr);
		GetEnvironmentVariable("PATH", pstr, ret);	
		}

	//P2N("CSupport::GetEnvStr(): %s\r\n", str);
}


int		CSupport::GetDaysUsed()

{
	return 10;
}
