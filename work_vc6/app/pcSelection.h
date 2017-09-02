#if !defined(AFX_PCSELECTION_H__2A101415_FB79_48C2_B834_86AEC3137D90__INCLUDED_)
#define AFX_PCSELECTION_H__2A101415_FB79_48C2_B834_86AEC3137D90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// pcSelection.h : header file
//

#include "xtargetbutton.h"
#include "editex.h"

/////////////////////////////////////////////////////////////////////////////
// CpcSelection dialog

class CpcSelection : public CDialog
{
// Construction
public:
	void CreateDBfile(const char *str, const char *newtable);
	void LoadRecent();
	void SaveRecent();
	
	int	maxsize, maxfill, numcol, zcount;

	CString result, diagcode, description;
	CString table, title;
	CString dbfile, dbfile2;

	void Fill();
	CpcSelection(CWnd* pParent = NULL);   // standard constructor

	
// Dialog Data
	//{{AFX_DATA(CpcSelection)
	enum { IDD = IDD_CODESEL };
	CEditEx	m_enotes;
	CEditEx	m_edesc;
	CxTargetButton	m_ok;
	CxTargetButton	m_can;
	CxTargetButton	m_b2;
	CxTargetButton	m_b3;
	CxTargetButton	m_b4;
	CxTargetButton	m_b5;
	CxTargetButton	m_b6;
	CxTargetButton	m_b7;
	CxTargetButton	m_b8;
	CListBox	m_recent;
	CListBox	m_list;
	CString	m_filter;
	CString	m_desc;
	CString	m_notes;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CpcSelection)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CpcSelection)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnChangeEdit1();
	afx_msg void OnButton2();
	afx_msg void OnDblclkList1();
	virtual void OnOK();
	afx_msg void OnDblclkList2();
	afx_msg void OnSelchangeList1();
	afx_msg void OnButton4();
	afx_msg void OnButtonCs3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PCSELECTION_H__2A101415_FB79_48C2_B834_86AEC3137D90__INCLUDED_)
