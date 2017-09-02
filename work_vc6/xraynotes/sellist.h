#if !defined(AFX_CSelList_H__A07972E4_F794_11D2_B398_525400D994D7__INCLUDED_)
#define AFX_CSelList_H__A07972E4_F794_11D2_B398_525400D994D7__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// CSelList.h : header file
//
			
/////////////////////////////////////////////////////////////////////////////
// CSelList window

class CSelList : public CCheckListBox

{

// Construction
public:
	CSelList();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelList)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSelList();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSelList)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CSelList_H__A07972E4_F794_11D2_B398_525400D994D7__INCLUDED_)
