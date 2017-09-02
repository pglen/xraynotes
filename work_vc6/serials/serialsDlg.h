// serialsDlg.h : header file
//

#if !defined(AFX_SERIALSDLG_H__F25A87B0_92A3_414C_A087_EE3099457758__INCLUDED_)
#define AFX_SERIALSDLG_H__F25A87B0_92A3_414C_A087_EE3099457758__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSerialsDlg dialog

class CSerialsDlg : public CDialog
{
// Construction
public:
	int saved;
	CSerialsDlg(CWnd* pParent = NULL);	// standard constructor

	void	CSerialsDlg::Str2Clip(const char *str);

// Dialog Data
	//{{AFX_DATA(CSerialsDlg)
	enum { IDD = IDD_SERIALS_DIALOG };
	CString	m_name;
	CString	m_cust;
	CString	m_serial;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSerialsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSerialsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	virtual void OnCancel();
	afx_msg void OnOk2();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	afx_msg void OnButton6();
	afx_msg void OnButton7();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERIALSDLG_H__F25A87B0_92A3_414C_A087_EE3099457758__INCLUDED_)
