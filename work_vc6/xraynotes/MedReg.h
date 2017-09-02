#if !defined(AFX_MEDREG_H__A2DAE651_E684_4D87_BDC5_B127FE42A13A__INCLUDED_)
#define AFX_MEDREG_H__A2DAE651_E684_4D87_BDC5_B127FE42A13A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MedReg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMedReg dialog

class CMedReg : public CDialog
{
// Construction
public:
	CMedReg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMedReg)
	enum { IDD = IDD_REG_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMedReg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMedReg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MEDREG_H__A2DAE651_E684_4D87_BDC5_B127FE42A13A__INCLUDED_)
