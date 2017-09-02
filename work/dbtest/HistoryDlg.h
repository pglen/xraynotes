#if !defined(AFX_HISTORYDLG_H__E74A9897_4518_4EFE_A734_6B921AE89F1B__INCLUDED_)
#define AFX_HISTORYDLG_H__E74A9897_4518_4EFE_A734_6B921AE89F1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HistoryDlg.h : header file
//
#include "SimpleGrid.h"

/////////////////////////////////////////////////////////////////////////////
// CHistoryDlg dialog

class CHistoryDlg : public CDialog
{
// Construction
public:
	CString sql, dbfile, header;
	CString sql2, dbfile2;

	int reccnt;

	int ok_x, ok_y, can_x, can_y;
	int ed1_gapx, ed1_gapy, ed1_left, ed1_top;

	void	Resize();

	void Fill();
	CHistoryDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CHistoryDlg)
	enum { IDD = IDD_HIST_DIALOG };
	CSimpleGrid	m_grid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHistoryDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHistoryDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HISTORYDLG_H__E74A9897_4518_4EFE_A734_6B921AE89F1B__INCLUDED_)
