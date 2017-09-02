#if !defined(AFX_PROCLIST_H__038FE1BD_F770_4805_B745_415CFD4C8856__INCLUDED_)
#define AFX_PROCLIST_H__038FE1BD_F770_4805_B745_415CFD4C8856__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PRocList.h : header file
//

#include "pgedit.h"
#include "dbdef.h"

#define PLIST_MAGIC	0x13579

/////////////////////////////////////////////////////////////////////////////
// CPRocList dialog

class CPRocList : public CDialog
{
// Construction
public:
	int UpdateDB();
	void SaveData();
	
	int magic;

	CPRocList(CWnd* pParent = NULL);   // standard constructor
	void CPRocList::init_array();

	void CPRocList::LoadData(int getid = -1);

	int		farrsize;
	int		backup;
	int		rows;

	CString tid, dbfile;

	fields	farr[60];			// Make sure it will hold all of constructor's additions

// Dialog Data
	//{{AFX_DATA(CPRocList)
	enum { IDD = IDD_PROC_LIST };
	CPGedit	m_tide;
	CString	m_pcode;
	CString	m_pcoded;
	CString	m_proc2;
	CString	m_proc2d;
	CString	m_proc3;
	CString	m_proc3d;
	CString	m_proc4;
	CString	m_proc4d;
	CString	m_proc1d;
	CString	m_proc1;
	CString	m_proc;
	CString	m_proc1n;
	CString	m_proc2n;
	CString	m_proc3n;
	CString	m_proc4n;
	CString	m_tid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPRocList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPRocList)
	afx_msg void OnPlButton2();
	afx_msg void OnPlButton23();
	afx_msg void OnPlButton30();
	afx_msg void OnPlButton24();
	afx_msg void OnPlButton25();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROCLIST_H__038FE1BD_F770_4805_B745_415CFD4C8856__INCLUDED_)
