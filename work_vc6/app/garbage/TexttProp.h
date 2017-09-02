#if !defined(AFX_TEXTTPROP_H__A287E6AC_B722_4363_8428_91FA3DEE6D30__INCLUDED_)
#define AFX_TEXTTPROP_H__A287E6AC_B722_4363_8428_91FA3DEE6D30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TexttProp.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTextProp dialog

class CTextProp : public CGrayDlg
{
// Construction
public:
	CTextProp(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTextProp)
	enum { IDD = IDD_DIALOG28};
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTextProp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTextProp)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTTPROP_H__A287E6AC_B722_4363_8428_91FA3DEE6D30__INCLUDED_)
