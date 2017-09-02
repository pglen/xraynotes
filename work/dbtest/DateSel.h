#if !defined(AFX_DATESEL_H__C94F2ECA_BC0D_4920_BF23_D5BFAD9EA6FB__INCLUDED_)
#define AFX_DATESEL_H__C94F2ECA_BC0D_4920_BF23_D5BFAD9EA6FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DateSel.h : header file
//

#include "xTargetButton.h"
#include "EditEx.h"
#include "CalCtrl.h"
#include "cTimeCtrl.h"

#define CHANGED	 (WM_USER + 5)

/////////////////////////////////////////////////////////////////////////////
// CDateSel dialog

class CDateSel : public CDialog

{
// Construction
public:
	CDateSel(CWnd* pParent = NULL);   // standard constructor

	CString result;
	int	time_disabled;
	int date_disabled;

	int reenter;

// Dialog Data
	//{{AFX_DATA(CDateSel)
	enum { IDD = IDD_DIALOG4 };
	CxTargetButton	m_b31;
	CxTargetButton	m_b29;
	CxTargetButton	m_b28;
	CxTargetButton	m_b27;
	CxTargetButton	m_b26;
	CxTargetButton	m_b25;
	CxTargetButton	m_b24;
	CEditEx	m_tme;
	CStatic	m_ttt;
	CTimeCtrl	m_time;
	CCalCtrl	m_cal;
	CEditEx		m_edit;
	CxTargetButton	m_n;
	CxTargetButton	m_p;
	CxTargetButton	m_can2;
	CxTargetButton	m_can;
	CxTargetButton	m_ok;
	CString	m_str;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDateSel)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDateSel)
	afx_msg void OnCancel2();
	afx_msg void OnButton2();
	afx_msg void OnButton23();
	afx_msg void OnButton24();
	afx_msg void OnButton25();
	afx_msg void OnButton26();
	afx_msg void OnButton30();
	afx_msg void OnButton27();
	afx_msg void OnButton28();
	afx_msg void OnButton31();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEdit2();
	afx_msg void OnChangeDsEdit1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATESEL_H__C94F2ECA_BC0D_4920_BF23_D5BFAD9EA6FB__INCLUDED_)
