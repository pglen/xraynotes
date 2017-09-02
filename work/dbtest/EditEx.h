#if !defined(AFX_EDITEX_H__C720E1F7_C999_4200_AA7D_8B80744DAAE5__INCLUDED_)
#define AFX_EDITEX_H__C720E1F7_C999_4200_AA7D_8B80744DAAE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditEx.h : header file
//

#include "popup.h"

/////////////////////////////////////////////////////////////////////////////
// CEditEx window

class CEditEx : public CEdit
{
// Construction
public:
	CEditEx();
		
	int		first;
	CPopup	tip;
	int		tipshow;
	int		capped, fired;
	CPoint	mouse;
	CPoint	capmouse;
	CString tiptext;

	CStringArray sarr, rsarr;

// Attributes
public:

	COLORREF	grey;
	CBrush		NewBrush;
	int			disabled ;

protected:
	
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditEx)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	void Enable();
	void ShowTip();
	void LoadTip(int id);
	void StopTip();
	void Disable();
	virtual ~CEditEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEditEx)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnChange();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITEX_H__C720E1F7_C999_4200_AA7D_8B80744DAAE5__INCLUDED_)
