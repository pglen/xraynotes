#if !defined(AFX_WINDOW_H__09647520_31F7_48BB_B5A4_0F587B2E6205__INCLUDED_)
#define AFX_WINDOW_H__09647520_31F7_48BB_B5A4_0F587B2E6205__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Window.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// Window command target

class Window : public CCmdTarget
{
	DECLARE_DYNCREATE(Window)

	Window();           // protected constructor used by dynamic creation

	HWND	win;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Window)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~Window();

	// Generated message map functions
	//{{AFX_MSG(Window)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINDOW_H__09647520_31F7_48BB_B5A4_0F587B2E6205__INCLUDED_)
