// demo.cpp : Defines the class behaviors for the application.

#include "stdafx.h"
#include "AFXRES.H"

#include <ole2.h>

#include <direct.h>
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <direct.h>
#include <time.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h> 

#include "dbtest.h"

#include "mxpad.h"
#include "support.h"

//include "demo.h"

///////////////////////////////////////////////////////////////////////////
// YieldToWinEx
// Let every accumulated message go through
// Return 1 if ESC | Ctrl-Break pressed -- 2 if EXITING

int     YieldToWinEx()

{
    int ret_val = 0;
    while(TRUE)
        {
        MSG msg;
        if(!PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
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

/////////////////////////////////////////////////////////////////////////////
// Shorten path to directory name

void	PathToDir(CString &docname)

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

void	PathToFname(CString &docname)

{
	int idx;
	CString filename;

	if((idx = docname.ReverseFind('\\')) != -1)
		{
		filename = docname.Right(docname.GetLength() - (idx + 1));
		docname = filename;
		}
}


void	PathToExt(CString  &fname)

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

CString GetExtension(const CString& name)

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

CString GetFilename(const CString& name)

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

CString GetFileNameExt(const CString& name)

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

CString GetDirName(const CString& name)

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

//////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Find how many characters in starting

int	 	FindNumOfChar(CString &str, CString &mus, int start)

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

void	ShortenPath(CString &str, int len)

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
// int		HashString(const char *name)

int		HashString(const char *name)

{
	unsigned int ret_val = 0;

    while(*name != '\0')
        {
        ret_val ^= (int)*name;
        ret_val  = ROTATE_LONG_RIGHT(ret_val, 3);          /* rotate right */
        *name++;
        }
    return((int)ret_val);
}

/////////////////////////////////////////////////////////////////////////////
// Wrapper for current dir

CString GetCurrentDir()

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


/////////////////////////////////////////////////////////////////////////////
// Wrapper for split path

void	SplitPath(CString &full,  CString &drive,
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

//////////////////////////////////////////////////////////////////////////

CString		RepSpecChar(CString str)

{
	int ii;
	int len = str.GetLength();
	CString result;

	//P2N("RREPSPEC Command line %s\r\n", str); 

	for (ii = 0; ii < len; ii++)
		{
		char chh = str.GetAt(ii);

        if ((chh <= ' ') || (chh >= '~') || (chh == '&'))
			{
            // Replace with hex code.
            CString hx; hx.Format("%02x", chh);
            result +=  "%" +  hx;
			}
        else
			{
            // Leave unchanged.
            result += chh;
			}
		}

	//P2N("RREPSPEC result %s\r\n", result); 

	return	result;
}


/////////////////////////////////////////////////////////////////////////////////////
//
// This routine is a helper that finds the path to the special folder:
//
// \param SpecialFolder			- an UINT-define (See #defines above or (MSDN))\n
// \param SpecialFolderString	- Reference to a CString that receives the\n
//									path to the special folder
//
//\returns a BOOL - Found or not \n
// CSIDL_APPDATA 

BOOL	GetSpecialFolder(UINT SpecialFolder, CString &SpecialFolderString)

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

#include "msgbox.h"

static char    asztmp[512];

int     AfxPrintf(const char *Format, ...)

{
    char    *asc = NULL;
    va_list ArgList;

    va_start(ArgList, Format);
    _vsnprintf(asztmp, sizeof(asztmp), Format, ArgList);

    MBOX(asztmp);

    return(0);
}

/////////////////////////////////////////////////////////////////////////
// return true if it is a directory 

int		IsDir(const char *str)

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

int		IsFile(const char *str)

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
// Make sure initdir has backslash at the end

void	RepairPath(CString &dir)

{
	if(dir.GetLength())
		if(dir.GetAt(dir.GetLength() - 1) != '\\')
			dir += "\\";
}

//////////////////////////////////////////////////////////////////////////
// Remove all subdirs and file  recursive
// Return string if error

CString		rmrf(const char * str)

{
	P2N("Remove -rf dir: %s\r\n", str);

	CString fret = "";

	if(access(str, 0) < 0)
		{
		P2N("No dir by this name: %s\r\n", str);
		return fret;
		}

	CString buffer(str);

	CFileFind  ff;
	ff.FindFile(buffer + "\\" + "*.*");

	int ret = 1;
	while(TRUE)
		{
		if(!ret)
			break;
		ret = ff.FindNextFile();

		if(ff.IsDots() )
			continue;

		CString fname = ff.GetFileName();

		if(ff.IsDirectory())
			{
			P2N("Recursing dir %s\r\n", buffer + "\\" + fname);
			rmrf(buffer + "\\" + fname);
			P2N("Removing dir %s\r\n", buffer + "\\" + fname);

			if(rmdir(buffer + "\\" + fname) < 0)
				{
				fret = buffer + "\\" + fname + " -> " +  strerror(errno);
				}
			}
		else
			{
			P2N("Removing file %s\r\n", buffer + "\\" + fname);
			if(unlink(buffer + "\\" + fname) < 0)
				{
				// See if access denied?
				_chmod( buffer + "\\" + fname, _S_IWRITE );

				if(unlink(buffer + "\\" + fname) < 0)
					{
					fret = fret = buffer + "\\" + fname + " -> " +  strerror(errno);
					}
				}
			}
		}
	return fret;
}



///////////////////////////////////////////////////////////////////////////////
// Return sum of string

int		SumStr(const char *str)

{
    CString str2 = str;

	int sum = 0, loop, lim = str2.GetLength();

	str2.MakeUpper();

	for(loop = 0; loop < lim; loop++)
		{
		char chh = str2.GetAt(loop);

		if(isalnum(chh))
			sum += chh;
		}
	return(sum);
}

///////////////////////////////////////////////////////////////////////////////
// Return TRUE if OK

int		CheckLic(const char *str, int *res)

{
	int sum2;

	if(res)
		*res = 0;
	
	sum2 = SumStr(str);

	if(*str == 0)
		return(false);

	if(res)
		*res = sum2;
	
	// Terminal condition
	if(!(sum2 % 111))
		{
		return true;
		}
	else
		{
		return false;
		}			
}

///////////////////////////////////////////////////////////////////////////////
// Return number of months in licence
// 0 for inifinite

int		LicMonth(const char *cstr)

{
	CString str = cstr;

	str.MakeLower(); str.TrimRight();

	int mon = 0;

	P2N("Lic month check '%s'\r\n", str);

	int idx = str.ReverseFind(' ');

	if(idx < 0)
		return 0;

	CString end = str.Right(str.GetLength() - (idx + 1));
		
	//P2N("End str '%s'\r\n", end);

	// Look for identifier
	int idx2 = end.Find("month");

	if(idx2 < 0)
		return 0;

	CString rem = str.Left(idx );
	rem.TrimRight();

	//P2N("Rem '%s'\r\n", rem);

	int idx3 = rem.ReverseFind(' ');
	CString num = rem.Right(rem.GetLength() - (idx3 + 1));

	//P2N("Num '%s'\r\n", num);
	
	mon = atoi(num);

	P2N("Lic month check: %d\r\n", mon);
	return(mon);
}

//////////////////////////////////////////////////////////////////////////

int		GetRegRootInt(const char *str, const char *key)

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

void	SetRegRootInt(const char *str, const char *key, int val)

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

int		GetLocMachineInt(const char *str, const char *key)

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

void	SetLocMachineInt(const char *str, const char *key, int val)

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

CString EscapeSql(CString &str)

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


CString EscapeComma(CString &str)

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

//////////////////////////////////////////////////////////////////////////
// Get where the application lives

void	GetAppRoot(CString &approot)

{
	char *tmp = approot.GetBuffer(MAX_PATH + 1); ASSERT(tmp);	
	GetModuleFileName(AfxGetInstanceHandle(), tmp, MAX_PATH);
	approot.ReleaseBuffer();

	// Get application root
	int idx = approot.ReverseFind('\\');
	if(idx >= 0)
		approot = approot.Mid(0, idx + 1);
}

//////////////////////////////////////////////////////////////////////////
// Get where the application lives

#if 0

void	Help()

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

#endif

#if 0

#include "winsvc.h"

int IsAdmin( )
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

#endif
