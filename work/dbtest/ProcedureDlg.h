#if !defined(AFX_PROCEDUREDLG_H__CE9A042B_2AE0_413A_857F_E0F80FB847E9__INCLUDED_)
#define AFX_PROCEDUREDLG_H__CE9A042B_2AE0_413A_857F_E0F80FB847E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProcedureDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProcedureDlg dialog

class CProcedureDlg : public CDialog
{
// Construction
public:
	CProcedureDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProcedureDlg)
	enum { IDD = IDD_PROC_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProcedureDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProcedureDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROCEDUREDLG_H__CE9A042B_2AE0_413A_857F_E0F80FB847E9__INCLUDED_)
