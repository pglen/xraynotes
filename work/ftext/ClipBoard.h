// ClipBoard.h: interface for the CClipBoard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIPBOARD_H__F5B2D147_E641_43D4_B217_2D5D6CCA60E0__INCLUDED_)
#define AFX_CLIPBOARD_H__F5B2D147_E641_43D4_B217_2D5D6CCA60E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CClipBoard  
{
public:
	CClipBoard();
	virtual ~CClipBoard();

	int CClipBoard::GetText(CString &str);
	int CClipBoard::PutText(const char *str);

};

extern CClipBoard clip;

#endif // !defined(AFX_CLIPBOARD_H__F5B2D147_E641_43D4_B217_2D5D6CCA60E0__INCLUDED_)
