
/* =====[ xFile.cpp ]========================================== 
                                                                             
   Description:     The client project, implementation of the xFile.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  3/31/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// xFile.cpp : implementation file
//

#include "stdafx.h"
#include "io.h"
#include "client.h"
#include "xFile.h"
#include "xpad.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CxFile

IMPLEMENT_DYNAMIC(CxFile, CFile)

CxFile::CxFile()
{
}


CxFile::~CxFile()
{

}

//////////////////////////////////////////////////////////////////////////
//
// Function name:    CxFile::WriteString
// Description:      <empty description>
// Return type:      return number of bytes writter
// Argument:         CString &str
//
//////////////////////////////////////////////////////////////////////////

int	CxFile::WriteString(CString &str)

{
	ULONGLONG ret = 0;
	int len = str.GetLength();

	ret = Seek(0, CFile::current);
	Write(str, (int)len);
	ret = Seek(0, CFile::current) - ret;
	return (int)ret;
}

int	CxFile::WriteString(const char *str)

{
	ULONGLONG ret = 0;
	int len = strlen(str);

	ret = Seek(0, CFile::current);
	Write(str, len);
	ret = Seek(0, CFile::current) - ret;
	return (int)ret;
}

///////////////////////////////////////////////////////////////////////////
//
// Read a string from CFile
// Return false for EOF
// Simple and SLOW implementation (no buffering ...)
//

int	CxFile::ReadString(CString &str)

{
	//ASSERT(fp);
	int ret_val = true;

	while(true)
		{
		char chh;

		int ret = Read(&chh, sizeof(char));

		if(ret < sizeof(char))
			{
			ret_val = false;
			break;
			}	

		//if(_eof(m_hFile))
		//	{
		//	P2N("CxFile::ReadString End of file reached\r\n");
		//	ret_val = false;
		//	break;
		//	}

		// Handle CRLF
		if(chh == '\n')
			break;

		if(chh != '\r')
			str += chh;
		}
	return ret_val;
}


char	*CxFile::ExceptionToStr(int exc)

{
	char *str = "None";

	switch(exc)
		{
	 	case CFileException::none:		       str = "none"; break;
	 	case CFileException::generic:          str = "Generic Error";break;
	 	case CFileException::fileNotFound:     str = "File Not Found"; break;
	 	case CFileException::badPath:          str = "Bad Path"; break;
	 	case CFileException::tooManyOpenFiles: str = "Too Many Open Files"; break;
	 	case CFileException::accessDenied:     str = "Access Denied (Read only file?)"; break;
	 	case CFileException::invalidFile:      str = "Invalid File"; break;
	 	case CFileException::removeCurrentDir: str = "Remove Current Dir"; break;
	 	case CFileException::directoryFull:    str = "Directory Full (Disk full?)"; break;
	 	case CFileException::badSeek:          str = "Bad Seek"; break;
	 	case CFileException::hardIO:           str = "Hard IO Error"; break;
	 	case CFileException::sharingViolation: str = "Sharing Violation"; break;
	 	case CFileException::lockViolation:    str = "Lock Violation"; break;
	 	case CFileException::diskFull:         str = "Disk Full"; break;
	 	case CFileException::endOfFile:        str = "End of File"; break;
	 	}
	return(str);
}

