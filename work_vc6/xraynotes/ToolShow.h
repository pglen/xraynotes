#if !defined(AFX_TOOLSHOW_H__F644DAC8_F0EA_40EC_BBB1_4FF1F59E15AD__INCLUDED_)
#define AFX_TOOLSHOW_H__F644DAC8_F0EA_40EC_BBB1_4FF1F59E15AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolShow.h : header file
//

#include "persistdlg.h"
#include "GrayDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CToolShow dialog

class CToolShow : public CGrayDlg
{
// Construction
public:
	CToolShow(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CToolShow)
	enum { IDD = IDD_DIALOG20 };
	CxTargetButton	m_ok;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolShow)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CToolShow)
	afx_msg void OnCheck1();
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnCheck4();
	afx_msg void OnCheck6();
	afx_msg void OnCheck7();
	afx_msg void OnCheck5();
	afx_msg void OnCheck8();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLSHOW_H__F644DAC8_F0EA_40EC_BBB1_4FF1F59E15AD__INCLUDED_)
