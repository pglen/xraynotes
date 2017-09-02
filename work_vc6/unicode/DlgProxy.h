// DlgProxy.h : header file
//

#if !defined(AFX_DLGPROXY_H__05A80CEC_D8C9_45AA_9E05_22C912E78778__INCLUDED_)
#define AFX_DLGPROXY_H__05A80CEC_D8C9_45AA_9E05_22C912E78778__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CUnicodeDlg;

/////////////////////////////////////////////////////////////////////////////
// CUnicodeDlgAutoProxy command target

class CUnicodeDlgAutoProxy : public CCmdTarget
{
	DECLARE_DYNCREATE(CUnicodeDlgAutoProxy)

	CUnicodeDlgAutoProxy();           // protected constructor used by dynamic creation

// Attributes
public:
	CUnicodeDlg* m_pDialog;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUnicodeDlgAutoProxy)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CUnicodeDlgAutoProxy();

	// Generated message map functions
	//{{AFX_MSG(CUnicodeDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CUnicodeDlgAutoProxy)

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CUnicodeDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPROXY_H__05A80CEC_D8C9_45AA_9E05_22C912E78778__INCLUDED_)
