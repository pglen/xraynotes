#if !defined(AFX_DIMORE_H__BF615E3B_3FDA_4DFA_AA49_B0F642E70772__INCLUDED_)
#define AFX_DIMORE_H__BF615E3B_3FDA_4DFA_AA49_B0F642E70772__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiMore.h : header file
//

#include "label.h"
#include "transslider.h"
#include "GrayDlg.h"
#include "SelList.h"

/////////////////////////////////////////////////////////////////////////////
// CDiMore dialog

class CDiMore : public CGrayDlg
{
// Construction
public:
	void ToggleQarter(int beg, int end);
	void OnOK();
	CDiMore(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDiMore)
	enum { IDD = IDD_DIALOG26 };
	//CListBox	m_list;
	CSelList	m_list;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiMore)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDiMore)
	virtual BOOL OnInitDialog();
	afx_msg void OnDidButton3();
	afx_msg void OnDidButton4();
	afx_msg void OnDidButton1();
	afx_msg void OnDidButton2();
	afx_msg void OnDidButton7();
	afx_msg void OnDidButton8();
	afx_msg void OnDidButton9();
	afx_msg void OnDidButton10();
	afx_msg void OnSelchangeList1();
	afx_msg void OnDidButton11();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIMORE_H__BF615E3B_3FDA_4DFA_AA49_B0F642E70772__INCLUDED_)
