#if !defined(AFX_HSHIELD_H__95B33A78_4B2F_4D5E_8020_EEF11C232FE7__INCLUDED_)
#define AFX_HSHIELD_H__95B33A78_4B2F_4D5E_8020_EEF11C232FE7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HShield.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHShield dialog

class CHShield : public CDialog

{
public:
	//static	int disable;
	static	CString strhipaa, strflag;

// Construction
public:
	CHShield(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CHShield)
	enum { IDD = IDD_HIPAA };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHShield)
	public:
	virtual int DoModal();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHShield)
	afx_msg void OnHip();
	afx_msg void OnCheckHip1();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnCheckHip2();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HSHIELD_H__95B33A78_4B2F_4D5E_8020_EEF11C232FE7__INCLUDED_)
