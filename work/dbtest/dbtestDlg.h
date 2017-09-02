// dbtestDlg.h : header file
//

#if !defined(AFX_DBTESTDLG_H__6411225D_1B6C_4245_B74B_8E1F7C0CBCD3__INCLUDED_)
#define AFX_DBTESTDLG_H__6411225D_1B6C_4245_B74B_8E1F7C0CBCD3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SimpleGrid.h"

/////////////////////////////////////////////////////////////////////////////
// CDbtestDlg dialog

class CDbtestDlg : public CDialog
{
// Construction
public:

	int table_offset;
	int	qcount;
	CString table_name;
	int	m_offset;

	//virtual void OnCancel();

	int rrr;
	void ExecSQL(const char *str);
	CDbtestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDbtestDlg)
	enum { IDD = IDD_DBTEST_DIALOG };
	CComboBox	m_combo;
	CSimpleGrid	m_grid;
	CString	m_table;
	CString	m_sql;
	CString	m_sql2;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDbtestDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDbtestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	afx_msg void OnButton12();
	afx_msg void OnButton13();
	afx_msg void OnButton14();
	afx_msg void OnButton15();
	afx_msg void OnButton16();
	afx_msg void OnButton17();
	afx_msg void OnButton18();
	afx_msg void OnButton19();
	afx_msg void OnButton20();
	afx_msg void OnButton21();
	afx_msg void OnButton2();
	afx_msg void OnButton24();
	afx_msg void OnButton23();
	afx_msg void OnButton25();
	afx_msg void OnButton26();
	afx_msg void OnButton27();
	afx_msg void OnButton28();
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DBTESTDLG_H__6411225D_1B6C_4245_B74B_8E1F7C0CBCD3__INCLUDED_)
