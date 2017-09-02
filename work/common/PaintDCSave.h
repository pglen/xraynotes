// ClientDCSave.h: interface for the CPaintDCSave class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIENTDCSAVE_H__83AD4E07_0218_4A0E_BFF7_BD623AC6C553__INCLUDED_)
#define AFX_CLIENTDCSAVE_H__83AD4E07_0218_4A0E_BFF7_BD623AC6C553__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//include <afxwin.h>

class CPaintDCSave : public CPaintDC  

{

public:
	
	CPaintDCSave(CWnd* pWnd);
	virtual ~CPaintDCSave();

protected:

	int save;
};

#endif // !defined(AFX_CLIENTDCSAVE_H__83AD4E07_0218_4A0E_BFF7_BD623AC6C553__INCLUDED_)
