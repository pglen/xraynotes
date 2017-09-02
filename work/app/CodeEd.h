#if !defined(AFX_CODEED_H__8CCB9FE9_1391_42C3_BA84_2A0EE2EB49BD__INCLUDED_)
#define AFX_CODEED_H__8CCB9FE9_1391_42C3_BA84_2A0EE2EB49BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CodeEd.h : header file
//

#include "editex.h"
#include "xtargetbutton.h"

/////////////////////////////////////////////////////////////////////////////
// CCodeEd dialog

class CCodeEd : public CGrayDlg

{

public:

	CString	m_id2;
	CString	m_code2;
	CString	m_desc2;
	CString	m_title2;
	CString	m_notes2;

	int id;	
	CString table;

// Construction
public:
	CCodeEd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCodeEd)
	enum { IDD = IDD_CODE_DIALOG };
	CEditEx	m_ced1;
	CEditEx	m_ced2;
	CEditEx	m_ced3;
	CEditEx	m_ced4;
	CEditEx	m_ced5;

	CString	m_id;
	CString	m_code;
	CString	m_desc;
	CString	m_title;
	CString	m_notes;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCodeEd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCodeEd)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CODEED_H__8CCB9FE9_1391_42C3_BA84_2A0EE2EB49BD__INCLUDED_)
