#if !defined(AFX_MYSLIDERCONTROL_H__C76FA857_51CC_4EB6_A8E2_8323BBEF10CD__INCLUDED_)
#define AFX_MYSLIDERCONTROL_H__C76FA857_51CC_4EB6_A8E2_8323BBEF10CD__INCLUDED_
#pragma once   // speed up compiling with MSVC++, file will only be read once
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//
// FEATURES:
//
// Transparent background.
// Custom Transparent Channel
// Custom thumb with optional colours..
// CStatic objects can be auto sized.
// Derived from CSlider

/////////////////////////////////////////////////////////////////////////////
// CTransProg window

class CTransProg : public CProgressCtrl

{
	HDC			m_dcBk;
	HBITMAP		m_bmpBk;
	HBITMAP     m_bmpBkOld;
	COLORREF	m_crThumbColor;
	COLORREF	m_crThumbColorSelected;

// Construction
public:
	CTransProg();

	//bool m_bRedraw;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTransProg)
	//}}AFX_VIRTUAL

// Implementation
public: 
	virtual ~CTransProg();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTransProg)
		afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	void SetThumbColors(COLORREF face, COLORREF highlight);
	//void SetllPos(LONGLONG nPos);
	void DrawTransparent(BOOL bRepaint);

	
private:
	//void DrawFocus(CDC *pDC, RECT *r, DWORD color, int border, int width, int height, int offset);
	void DrawFocus(CDC *pDC, RECT *r, DWORD color, int border);
	void DrawChannel(CDC *pDC, LPNMCUSTOMDRAW lpcd);
	void DrawThumb(CDC *pDC, LPNMCUSTOMDRAW lpcd);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSLIDERCONTROL_H__C76FA857_51CC_4EB6_A8E2_8323BBEF10CD__INCLUDED_)
