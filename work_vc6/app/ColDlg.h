#if !defined(AFX_COLDLG_H__9936CFCD_9B00_4400_A84F_2BFCC7C40AD5__INCLUDED_)
#define AFX_COLDLG_H__9936CFCD_9B00_4400_A84F_2BFCC7C40AD5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColDlg.h : header file
//

#include "TransSlider.h"
#include "GrayDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CColDlg dialog

class CColDlg : public CGrayDlg
{
// Construction
public:
	void UpdateCursor();
	void UpdateParam();

	void	Apply();
	void	Show();

	int		org_changed;
	//int		want_x, want_y;
	//int		created;
	int		in_apply;

	void	*parent;
	HCURSOR m_busy;

	CColDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CColDlg)
	enum { IDD = IDD_RGB };
	CTransSlider	m_slide3;
	CTransSlider	m_slide2;
	CTransSlider	m_slide1;

	CxTargetButton	m_OK	;	
	CxTargetButton	m_Cancel;	
	CxTargetButton	m_Reset	;	
	CxTargetButton	m_lt1	;		
	CxTargetButton	m_gt1	;		
	CxTargetButton	m_ltt1	;	
	CxTargetButton	m_gtt1	;	
	CxTargetButton	m_lt2	;		
	CxTargetButton	m_gt2	;		
	CxTargetButton	m_ltt2	;	
	CxTargetButton	m_gtt2	;	
	CxTargetButton	m_lt3	;		
	CxTargetButton	m_gt3	;		
	CxTargetButton	m_gtt3	;	
	CxTargetButton	m_ltt3	;	

	CLabel	m_colb;
	CLabel	m_colg;
	CLabel	m_colr;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CColDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnReleasedcaptureSlider1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSlider2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSlider3(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButton3();
	virtual void OnOK();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimeChange();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	virtual void OnCancel();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	afx_msg void OnButton2();
	afx_msg void OnButton1();
	afx_msg void OnButton6();
	afx_msg void OnButton7();
	afx_msg void OnButton8();
	afx_msg void OnButton9();
	afx_msg void OnButton10();
	afx_msg void OnButton12();
	afx_msg void OnButton11();
	afx_msg void OnButton13();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLDLG_H__9936CFCD_9B00_4400_A84F_2BFCC7C40AD5__INCLUDED_)
