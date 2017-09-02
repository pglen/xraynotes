#if !defined(AFX_TRANSCRIPT_H__0329DD86_D9E3_41CD_9AEF_A6ED2DF9EE46__INCLUDED_)
#define AFX_TRANSCRIPT_H__0329DD86_D9E3_41CD_9AEF_A6ED2DF9EE46__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Transcript.h : header file
//

#include "editex.h"
#include "PGedit.h"

/////////////////////////////////////////////////////////////////////////////
// CTranscript dialog

class CTranscript : public CDialog
{
// Construction
public:
	void	Resize();

	void OnOK();
	CTranscript(CWnd* pParent = NULL);   // standard constructor

	int changed;

	int ok_x, ok_y, can_x, can_y;
	int	b1_x, b1_y, b2_x, b2_y, b32_x, b32_y;
	int c1_x, c1_y, s1_x, s1_y, e2_x, e2_y;
	int ed1_gapx, ed1_gapy, ed1_left, ed1_top;

	int moved;
	//const char	*str;
	CString		*ref, *head;

// Dialog Data
	//{{AFX_DATA(CTranscript)
	enum { IDD = IDD_TRANSCR_DIALOG };
	CPGedit	m_edit1;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTranscript)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTranscript)
	virtual BOOL OnInitDialog();
	afx_msg void OnButton2();
	afx_msg void OnButton30();
	afx_msg void OnButton32();
	afx_msg void OnCheck1();
	virtual void OnCancel();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRANSCRIPT_H__0329DD86_D9E3_41CD_9AEF_A6ED2DF9EE46__INCLUDED_)
