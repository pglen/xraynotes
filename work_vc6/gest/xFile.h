#if !defined(AFX_XFILE_H__FDC7FBE5_FED1_4576_BED9_4116A699A714__INCLUDED_)
#define AFX_XFILE_H__FDC7FBE5_FED1_4576_BED9_4116A699A714__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// xFile.h : header file
//

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
