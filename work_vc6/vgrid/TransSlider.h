#if !defined(AFX_MYSLIDERCONTROL_H__C76FA857_51CC_4EB6_A8E2_8323BBEF10BD__INCLUDED_)
#define AFX_MYSLIDERCONTROL_H__C76FA857_51CC_4EB6_A8E2_8323BBEF10BD__INCLUDED_
#pragma once   // speed up compiling with MSVC++, file will only be read once
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define SCROLL_START	1
#define SCROLL_TRACK	2
#define SCROLL_END		3

//
// FEATURES:
//
// Transparent background.
// Custom Transparent Channel
// Custom thumb with optional colours..
// CStatic objects can be auto sized.
// Derived from CSlider

/////////////////////////////////////////////////////////////////////////////
// CTransSlider window

class CTransSlider : public CSliderCtrl

{
	HDC			m_dcBk;

	HBITMAP		m_bmpBk, m_bmpBkOld;
	COLORREF	m_crThumbColor, m_crThumbColorSelected;

	int oldpos;

// Construction
public:
	CTransSlider();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTransSlider)
	//}}AFX_VIRTUAL

// Implementation
public: 
	virtual ~CTransSlider();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTransSlider)
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	int message;
	
	virtual void OnScroll(int opcode, int pos);

	void SetThumbColors(COLORREF face, COLORREF highlight);
	void DrawTransparent(BOOL bRepaint);
	
private:
	void DrawFocus(CDC *pDC, RECT *r, DWORD color, int border);
	void DrawChannel(CDC *pDC, LPNMCUSTOMDRAW lpcd);
	void DrawThumb(CDC *pDC, LPNMCUSTOMDRAW lpcd);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSLIDERCONTROL_H__C76FA857_51CC_4EB6_A8E2_8323BBEF10BD__INCLUDED_)
