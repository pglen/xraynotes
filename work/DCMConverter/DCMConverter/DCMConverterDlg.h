// DCMConverterDlg.h : header file
//

#if !defined(AFX_DCMCONVERTERDLG_H__14805F24_FAF6_4CE0_B459_5B8E7964DD8F__INCLUDED_)
#define AFX_DCMCONVERTERDLG_H__14805F24_FAF6_4CE0_B459_5B8E7964DD8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "xImageDCM.h"
/////////////////////////////////////////////////////////////////////////////
// CDCMConverterDlg dialog

class CDCMConverterDlg : public CDialog
{
// Construction
public:
	CDCMConverterDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDCMConverterDlg)
	enum { IDD = IDD_DCMCONVERTER_DIALOG };
	CString	m_sDcmSrcFile;
	CString	m_sBmpSrcFile;
	CString	m_sJPGDestFile;
	CString	m_sDcmDestFile;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDCMConverterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDCMConverterDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonConvert();
	afx_msg void OnButtonConvert2dcm();
	afx_msg void OnButtonOpenDcm();
	afx_msg void OnButtonOpenBmp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CxImageDCM dcm;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DCMCONVERTERDLG_H__14805F24_FAF6_4CE0_B459_5B8E7964DD8F__INCLUDED_)
