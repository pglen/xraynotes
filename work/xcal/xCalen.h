#if !defined(AFX_XCALEN_H__3102BFCF_D731_4FCE_B137_DA38ECDF2D0D__INCLUDED_)
#define AFX_XCALEN_H__3102BFCF_D731_4FCE_B137_DA38ECDF2D0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// xCalen.h : header file
//

#define HOURSLIGHT	 12			// Calendar const
#define WEEKDAYS	 7			// Calendar const
#define HOURSINDAY	 24			// Calendar const

#define SLOTS		 48			// This many slots in a day
#define SHOWHOURS	 12			// Show this many hours

#define DSTART		8			// Work day start
#define DEND		17			// Work day end

#define INISTART	7			// Init the control with this 

#define	MED_SIZE	350
#define	LARGE_SIZE	600
#define	XLARGE_SIZE 900

#define	CALITEM_DBLCLICK	WM_USER+1
#define	CALITEM_WANTDATA	WM_USER+2
#define	CALITEM_RCLICK		WM_USER+3

#include "Popup.h"

/////////////////////////////////////////////////////////////////////////////
// CxCalen window

class CxCalen : public CWnd
{
	
	// Construction
public:

	int		in_scroll;
	int		nightcolor, daycolor, bgcolor;

	// Tooltip related
	int	buttdown, capped, fired;
	COleDateTimeSpan oneday;
	COleDateTime date, today, selday;

	COleDateTime startdate;			// startdate

	BOOL DoMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	CxCalen();

	void	(*callback)(void*);

	int		starthour;

private:

	
	CScrollBar  vs, hs;

	CString	tooltip;
	CPopup	tip;

	int		sw, sh;	
	CPoint	mouse, capmouse;

	void Fill(const char *str = "");

	// Storage for calendar data
	CString			*data[WEEKDAYS][4 * HOURSINDAY];
	CDWordArray		arr[WEEKDAYS][4 * HOURSINDAY];

	int		firstpaint;
	int		lpad, hpad;
	CFont	m_font, m_fontb, m_fontbh;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CxCalen)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation

public:
	void Invalidate();
	void InvalidateCal();

	void	GetStartDate(COleDateTime *sdate);
	void	ClearContents();
	void	GetTooltip(CPoint point, CString &str);
	void	DrawHeaders(CPaintDC *dc, COleDateTime *sdate);
	void	DrawContent(CPaintDC *dc);
	void	DrawGrid(CPaintDC *dc, COleDateTime *sdate);
	void	SetNewDate(COleDateTime *ndate);
	void	DefHours();

	void	SetStartHour(int hour);
	void	SetItem(int day, int hour, int quart, const char *ste, int id = 0);
	void	SetItem(COleDateTime *ddd, const char *ste, int id = 0);

	int		DayHitTest(CPoint point);
	int		TimeHitTest(CPoint point);

	void	FirstPaint(CPaintDC *pDC);
	virtual ~CxCalen();

	// Generated message map functions
protected:
	//{{AFX_MSG(CxCalen)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XCALEN_H__3102BFCF_D731_4FCE_B137_DA38ECDF2D0D__INCLUDED_)
