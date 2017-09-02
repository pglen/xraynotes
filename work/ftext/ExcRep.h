#if !defined(AFX_EXCREP_H__908271DB_E1DE_4697_9A13_7BCB8F26A49D__INCLUDED_)
#define AFX_EXCREP_H__908271DB_E1DE_4697_9A13_7BCB8F26A49D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../common/Label.h"
#include "../common/mxpad.h"

// ExcRep.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CExcRep dialog

class CExcRep : public CDialog
{
// Construction
public:
	CExcRep(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CExcRep)
	enum { IDD = IDD_DIALOG1 };
	CLabel	m_txt;
	CLabel	m_head;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExcRep)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CExcRep)
	virtual BOOL OnInitDialog();
	afx_msg void OnOk2();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXCREP_H__908271DB_E1DE_4697_9A13_7BCB8F26A49D__INCLUDED_)
