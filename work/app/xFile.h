
/* =====[ xFile.h ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the xFile.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  10/20/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// xCFile.h: interface for the CxFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XFILE_H__9CFC8272_5523_49A3_B688_6BB76ED3DD5F__INCLUDED_)
#define AFX_XFILE_H__9CFC8272_5523_49A3_B688_6BB76ED3DD5F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define TransExc TranslateFileException

class CxCFile : public CFile  

{

public:

	CxCFile();
	//virtual ~CxCFile();

	int		ReadString(CString &str);
	char	*TranslateFileException(int exc);

	void	WriteString(CString &str);

};

#endif // !defined(AFX_XFILE_H__9CFC8272_5523_49A3_B688_6BB76ED3DD5F__INCLUDED_)
