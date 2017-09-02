#if !defined(AFX_ALADLG_H__20A1F573_0224_4579_A50E_629B53E2A16D__INCLUDED_)
#define AFX_ALADLG_H__20A1F573_0224_4579_A50E_629B53E2A16D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AlaDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAlaDlg dialog

//typedef struct _fields
//		{
//		CString name;
//		CString value;
//		//CString value2;
//		CString	*var;
//		} fields;
//

class CAlaDlg : public CDialog
{
// Construction
public:
	void FillOne(int id);

	CString calfile, abfile;

	CString headstr;

	COleDateTime date;
	COleDateTime date2;

	CAlaDlg(CWnd* pParent = NULL);   // standard constructor

	CString m_id;

//	int		farrsize;
//	fields	farr[60];			// Make sure it hold all of initdialog's additions

// Dialog Data
	//{{AFX_DATA(CAlaDlg)
	enum { IDD = IDD_ALA_DIALOG };
	CxTargetButton	m_ok;
	CxTargetButton	m_can;
	CTreeCtrl	m_tree;
	CString	m_title;
	CString	m_num;
	CString	m_note;
	//}}AFX_DATA

//	CDWordArray dwarr;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAlaDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAlaDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSelchangedTreeAla1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonAla2();
	afx_msg void OnButtonAla1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALADLG_H__20A1F573_0224_4579_A50E_629B53E2A16D__INCLUDED_)
