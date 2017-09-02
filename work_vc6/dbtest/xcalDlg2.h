// xcalDlg2.h : header file
//

#if !defined(AFX_XCALDLG_H__66E650F0_5437_4F52_9F43_16500C844C25__INCLUDED_)
#define AFX_XCALDLG_H__66E650F0_5437_4F52_9F43_16500C844C25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//include "vSplit.h"
#include "xCalen.h"

/////////////////////////////////////////////////////////////////////////////
// CXcalDlg dialog

class CXcalDlg : public CDialog
{
// Construction
public:
	void SetItem(int day, int hour, const char *str);
	void Resize();

	int ok_x, ok_y, can_x, can_y;
	int	b1_x, b1_y, b2_x, b2_y, b3_x, b3_y;
	int b4_x, b4_y, b5_x, b5_y, b6_x, b6_y;
	int l1_x, l1_y;

	CXcalDlg(CWnd* pParent = NULL);	// standard constructor

	//CvSplit split;

// Dialog Data
	//{{AFX_DATA(CXcalDlg)
	enum { IDD = IDD_XCAL_DIALOG2 };
	CxCalen	m_split;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXcalDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CXcalDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	afx_msg void OnButton6();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XCALDLG_H__66E650F0_5437_4F52_9F43_16500C844C25__INCLUDED_)
