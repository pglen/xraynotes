// unicodeDlg.h : header file
//

#if !defined(AFX_UNICODEDLG_H__FD32BEED_DF27_448E_B97B_A204DD54FB1C__INCLUDED_)
#define AFX_UNICODEDLG_H__FD32BEED_DF27_448E_B97B_A204DD54FB1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CUnicodeDlgAutoProxy;

/////////////////////////////////////////////////////////////////////////////
// CUnicodeDlg dialog

class CUnicodeDlg : public CDialog
{
	DECLARE_DYNAMIC(CUnicodeDlg);
	friend class CUnicodeDlgAutoProxy;

// Construction
public:
	CUnicodeDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CUnicodeDlg();

// Dialog Data
	//{{AFX_DATA(CUnicodeDlg)
	enum { IDD = IDD_UNICODE_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUnicodeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CUnicodeDlgAutoProxy* m_pAutoProxy;
	HICON m_hIcon;

	BOOL CanExit();

	// Generated message map functions
	//{{AFX_MSG(CUnicodeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UNICODEDLG_H__FD32BEED_DF27_448E_B97B_A204DD54FB1C__INCLUDED_)
