#if !defined(AFX_ALPHA_H__2B514532_DC79_4CD1_9287_A8AE80FC934E__INCLUDED_)
#define AFX_ALPHA_H__2B514532_DC79_4CD1_9287_A8AE80FC934E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Alpha.h : header file
//

#define WM_SLIDE1	WM_USER + 15

#include "label.h"
#include "transslider.h"
#include "GrayDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CAlpha dialog

class CAlpha : public CGrayDlg
{
// Construction
public:

	int		org;
	void	OnCancel();
	void	Apply(int val);

	void	OnSlide1(int wParam, int lParam);

	CAlpha(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAlpha)
	enum { IDD = IDD_DIALOG29 };
	CxTargetButton	m_ok;
	CxTargetButton	m_can;
	CTransSlider	m_slider1;
	CString	m_ed1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAlpha)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAlpha)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALPHA_H__2B514532_DC79_4CD1_9287_A8AE80FC934E__INCLUDED_)
