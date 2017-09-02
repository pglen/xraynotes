#if !defined(AFX_ALADLG_H__20A1F573_0224_4579_A50E_629B53E2A16D__INCLUDED_)
#define AFX_ALADLG_H__20A1F573_0224_4579_A50E_629B53E2A16D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AlaDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAlaDlg dialog
typedef struct _fields
		{
		CString name;
		CString value;
		//CString value2;
		CString	*var;
		} fields;


class CAlaDlg : public CDialog
{
// Construction
public:

	CString dbfile;
	CString headstr;
	CAlaDlg(CWnd* pParent = NULL);   // standard constructor

	CString m_id;

	int		farrsize;
	fields	farr[60];			// Make sure it hold all of initdialog's additions

// Dialog Data
	//{{AFX_DATA(CAlaDlg)
	enum { IDD = IDD_DIALOG1 };
	CTreeCtrl	m_tree;
	CString	m_title;
	CString	m_num;
	//}}AFX_DATA


	CDWordArray dwarr;

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
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALADLG_H__20A1F573_0224_4579_A50E_629B53E2A16D__INCLUDED_)
