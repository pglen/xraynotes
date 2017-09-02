
/* =====[ xFile.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the xFile.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  10/20/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// xFile.cpp: implementation of the CxFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "xraynotes.h"
#include "xFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CxCFile::CxCFile()

{
	CFile::CFile();
}

//CxCFile::~CxCFile()
//
//{
//	CFile::~CFile();
//}

///////////////////////////////////////////////////////////////////////////

char	*CxCFile::TranslateFileException(int exc)

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

///////////////////////////////////////////////////////////////////////////
//
// Read a string from CFile
// Return false for EOF
// Simple and SLOW implementation (no buffering ...)
//

int	CxCFile::ReadString(CString &str)

{
	int		ret_val = true;	char chh;

	str = "";
	while(true)
		{
		if(Read(&chh, sizeof(char)) < sizeof(char))
			{
			ret_val = false; break;
			}

		// Handle CR,LF and valid chars
		if(chh == '\n')	break;
		if(chh != '\r')	str += chh;
		}

	//P2N("got '%s'\r\n", str);
	return ret_val;
}

void		CxCFile::WriteString(CString &str)

{
	CFile::Write(str, str.GetLength());
}


// EOF

