#if !defined(AFX_TRIANGLE_H__83A44C52_E8E1_4049_BA7D_991E936ECAFF__INCLUDED_)
#define AFX_TRIANGLE_H__83A44C52_E8E1_4049_BA7D_991E936ECAFF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Triangle.h : header file
//

#include "BaseWnd.h"

// Make sure these messages are not used in your application already
// If in doubt, use RegisterWindowMessages

#define WM_TRICHANGE1		(WM_USER + 2)
#define WM_TRICHANGE2		(WM_USER + 3)

#define WM_TRIREL1			(WM_USER + 4)
#define WM_TRIREL2			(WM_USER + 5)

/////////////////////////////////////////////////////////////////////////////
// CTriangle CBaseWnd

class CTriangle : public CBaseWnd

{
	int	pos1, pos2;

	POINT arr[3], uarr[3];
	POINT arr1[3], arr2[3];

	int leftcap;
	int rightcap;
	int erase;
	int trisize;

	// Construction
public:
	
	CString tiptext;
	int	upside;

	CTriangle();

	void	OnMouseLeave();
	void	OnMouseEnter();
	void	OnGetTipText(CString &str);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTriangle)
	public:
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	void Reset();
	void OnInitControl();
	virtual ~CTriangle();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTriangle)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRIANGLE_H__83A44C52_E8E1_4049_BA7D_991E936ECAFF__INCLUDED_)
