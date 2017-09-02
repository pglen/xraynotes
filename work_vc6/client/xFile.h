
/* =====[ xFile.h ]========================================== 
                                                                             
   Description:     The client project, implementation of the xFile.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  3/31/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#if !defined(AFX_XFILE_H__FDC7FBE5_FED1_4576_BED9_4116A699A714__INCLUDED_)
#define AFX_XFILE_H__FDC7FBE5_FED1_4576_BED9_4116A699A714__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// xFile.h : header file
//

#if 0

	//example for CFile::CFile
	char* pFileName = "test.dat";
	TRY
	{
	   CFile f( pFileName, CFile::modeCreate | CFile::modeWrite );
	}
	CATCH( CFileException, e )
	{
	   #ifdef _DEBUG
		  afxDump << "File could not be opened " << e->m_cause << "\n";
	   #endif
	}
	END_CATCH

#endif

// CFile::modeCreate   		
// CFile::modeNoTruncate   	
// CFile::modeRead   			
// CFile::modeReadWrite   		
// CFile::modeWrite   			
// CFile::modeNoInherit   		
// CFile::shareDenyNone   		
// CFile::shareDenyRead   		
// CFile::shareDenyWrite   	
// CFile::shareExclusive   	
// CFile::shareCompat   		
// CFile::typeText   			
// CFile::typeBinary   		

/////////////////////////////////////////////////////////////////////////////
// CxFile dialog

class CxFile : public CFile
{
	DECLARE_DYNAMIC(CxFile)

public:
	CxFile::CxFile();
	CxFile::~CxFile();

	int	CxFile::ReadString(CString &str);
	int	CxFile::WriteString(CString &str);
	int	CxFile::WriteString(const char *str);

	char	*CxFile::ExceptionToStr(int exc);

protected:
	//{{AFX_MSG(CxFile)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XFILE_H__FDC7FBE5_FED1_4576_BED9_4116A699A714__INCLUDED_)
