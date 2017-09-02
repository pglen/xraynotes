#if !defined(AFX_DOCLIST_H__409B4859_FDB0_4A9C_A312_9B0A68D13AD1__INCLUDED_)
#define AFX_DOCLIST_H__409B4859_FDB0_4A9C_A312_9B0A68D13AD1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DocList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDocList dialog

class CDocList : public CDialog
{
// Construction
public:
	void Closing(int reason);
	void OnOK();
	void OnCancel();

	CDWordArray	 xarr;
	CDWordArray	 yarr;

	CDocList(CWnd* pParent = NULL);   // standard constructor

	HBITMAP		m_hBitmap;
    DWORD		m_dwWidth;			// Width of bitmap
	DWORD		m_dwHeight;			// Height of bitmap
	CShadesDlg *dlg;
	CRect		m_hitrec;
// Dialog Data
	//{{AFX_DATA(CDocList)
	enum { IDD = IDD_DIALOG11 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDocList)
	public:
	virtual int DoModal();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDocList)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOCLIST_H__409B4859_FDB0_4A9C_A312_9B0A68D13AD1__INCLUDED_)
