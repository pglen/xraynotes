// ftextDlg.h : header file
//

#if !defined(AFX_FTEXTDLG_H__0549B73C_9693_4F03_BF20_9550C32CD2E9__INCLUDED_)
#define AFX_FTEXTDLG_H__0549B73C_9693_4F03_BF20_9550C32CD2E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../common/PGedit.h"
#include "xFedit.h"

#include "../common/GrayDlg.h"

#include "ToolStrip.h"
/////////////////////////////////////////////////////////////////////////////
// CFtextDlg dialog

class CFtextDlg : public CGrayDlg

{

// Construction
public:

	CFtextDlg(CWnd* pParent = NULL);	// standard constructor
	~CFtextDlg();
	int		fxcount;

// Dialog Data
	//{{AFX_DATA(CFtextDlg)
	enum { IDD = IDD_FTEXT_DIALOG };
	CEdit	m_ed3;
	CComboBox	m_size;
	CComboBox	m_col;
	CComboBox	m_fonts;
	CxFedit	m_ed2;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFtextDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

protected:

	void	AddFont(const char *str);
	void	Resize();

	// Data:
	int		first;
	WINDOWPLACEMENT wpd, wp_ed2, wp_ok; 
	CToolStrip *ts;

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CFtextDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButton7();
	afx_msg void OnButton8();
	afx_msg void OnButton9();
	afx_msg void OnButton10();
	afx_msg void OnButton11();
	afx_msg void OnButton5();
	afx_msg void OnButton6();
	afx_msg void OnSelchangeCombo2();
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnButton4();
	afx_msg void OnButton12();
	afx_msg void OnButton13();
	afx_msg void OnButton14();
	afx_msg void OnSelchangeCombo3();
	afx_msg void OnButton15();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnButton16();
	afx_msg void OnButton17();
	afx_msg void OnButton18();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FTEXTDLG_H__0549B73C_9693_4F03_BF20_9550C32CD2E9__INCLUDED_)
