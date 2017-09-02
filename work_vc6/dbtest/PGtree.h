#if !defined(AFX_PGTREE_H__CC39B5FD_668E_4AD0_B859_344070CC331E__INCLUDED_)
#define AFX_PGTREE_H__CC39B5FD_668E_4AD0_B859_344070CC331E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PGtree.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPGtree window

class CPGtree : public CTreeCtrl
{
// Construction
public:
	CPGtree();

	POINT currentmouse;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPGtree)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPGtree();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPGtree)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PGTREE_H__CC39B5FD_668E_4AD0_B859_344070CC331E__INCLUDED_)
