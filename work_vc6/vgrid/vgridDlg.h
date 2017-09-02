// vgridDlg.h : header file
//

#if !defined(AFX_VGRIDDLG_H__CCFC9CD4_4B95_48EB_8219_08A6FDA63691__INCLUDED_)
#define AFX_VGRIDDLG_H__CCFC9CD4_4B95_48EB_8219_08A6FDA63691__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CVgridDlg dialog

class CVgridDlg : public CDialog
{
// Construction
public:
	void UpdateTitle();
	CVgridDlg(CWnd* pParent = NULL);	// standard constructor

	int mode, grain;

	int red_arr[16];
	int green_arr[16];
	int blue_arr[16];
	int gray_arr[16];

	// Dialog Data
	//{{AFX_DATA(CVgridDlg)
	enum { IDD = IDD_VGRID_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVgridDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CVgridDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNewShowgriddefault();
	afx_msg void OnNewShowcolorpatette();
	afx_msg void OnNewShowredgradient();
	afx_msg void OnNewColorgradientsShowgreengradient();
	afx_msg void OnNewColorgradientsShowbluegradient();
	afx_msg void OnNewShowblahblah();
	afx_msg void OnNewShowbleh();
	afx_msg void OnNewShowfinegrid();
	afx_msg void OnNewShowcoarsegrid();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VGRIDDLG_H__CCFC9CD4_4B95_48EB_8219_08A6FDA63691__INCLUDED_)
