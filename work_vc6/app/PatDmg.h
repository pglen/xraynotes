#if !defined(AFX_PATDMG_H__78F3C945_1886_4CB0_A714_12D6FF31CAFA__INCLUDED_)
#define AFX_PATDMG_H__78F3C945_1886_4CB0_A714_12D6FF31CAFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PatDmg.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CPatDmg dialog

class CPatDmg : public CDialog
{
// Construction
public:
	CPatDmg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPatDmg)
	enum { IDD = IDD_DIALOG19 };
	CxTargetButton	m_b34;
	CxTargetButton	m_b33;
	CxTargetButton	m_b32;
	CxTargetButton	m_b31;
	CxTargetButton	m_b30;
	CxTargetButton	m_b29;
	CxTargetButton	m_b28;
	CxTargetButton	m_b27;
	CxTargetButton	m_b26;
	CxTargetButton	m_b25;
	CxTargetButton	m_b24;
	CxTargetButton	m_b23;
	CxTargetButton	m_b22;
	CxTargetButton	m_b21;
	CxTargetButton	m_b19;
	CxTargetButton	m_b18;
	CxTargetButton	m_b17;
	CxTargetButton	m_b16;
	CxTargetButton	m_b14;
	CxTargetButton	m_b13;
	CxTargetButton	m_b5;
	CxTargetButton	m_b4;
	CxTargetButton	m_b3;
	CxTargetButton	m_b2;
	CxTargetButton	m_b1;
	CLabel	m_head;
	CxTargetButton	m_can;
	CxTargetButton	m_ok;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPatDmg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPatDmg)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PATDMG_H__78F3C945_1886_4CB0_A714_12D6FF31CAFA__INCLUDED_)
