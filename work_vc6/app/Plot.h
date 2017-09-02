#if !defined(AFX_PLOT_H__596E6728_5FF8_4D09_BC8A_E784A01707F2__INCLUDED_)
#define AFX_PLOT_H__596E6728_5FF8_4D09_BC8A_E784A01707F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Plot.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPlot window

class CPlot : public CStatic
{

	double	mag;

	int		old_max;
	int		scroll;
	int		inited;
	int		step;
	int		changed;
	int		lastmove;
	int		ytimer;

	CPoint old;

	unsigned int lastclock;
	CPoint lastpoint;

	CDWordArray int_arr;

	// Construction
public:
	CPlot();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlot)
	//}}AFX_VIRTUAL

// Implementation
public:
	void ClearAll();
	void AddMarker();
	void AddLine(int hight);

	virtual ~CPlot();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPlot)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLOT_H__596E6728_5FF8_4D09_BC8A_E784A01707F2__INCLUDED_)
