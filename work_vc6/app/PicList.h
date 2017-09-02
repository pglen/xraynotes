#if !defined(AFX_PICLIST_H__1541F698_EB8C_4FE2_8899_0DCBA739B636__INCLUDED_)
#define AFX_PICLIST_H__1541F698_EB8C_4FE2_8899_0DCBA739B636__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PicList.h : header file
//

#define ListItem_Magic 0x12345

#include "basewnd.h"
#include "xstr.h"

class ListItem

{

public:
		int		magic;
		int		type;
		int		cellcol, cellrow;
		int		bold, italic;
		int		xx, yy;
		int		ww, hh;
		int		color;
		int		trans;
		CxStr	*content;

		ListItem() { magic = ListItem_Magic;
					 type = cellcol = cellrow = bold = italic = 0;
					 xx = yy = ww = hh = color = trans = 0;	}

		~ListItem() { delete content; }

} ;

/////////////////////////////////////////////////////////////////////////////
// CPicList window

class CPicList : public CBaseWnd
{

// Data
public:

	CScrollBar  vs, hs;

	CRect		charrc; 
	COLORREF	back, highlite, high_nofocus;
	CFont		m_font, m_fontb;

	int		sw, sh;
	int		leftspacer, topgap;
	int		curr_row;
	int		startrow, startcol;
	int		vrows, vcols;
	int		maxlen, visrows;

protected:

	int firstpaint;

private:
	CPtrArray	rowarr;
	
// Implementation
public:

	void	OnInitControl();
	void	UpdateBars(int state);
	int		KeyEvent(UINT event, UINT nChar, UINT nFlags);
	void	SetCurrRow(int crow);
	void	AddStr(const char *str);
	void	FirstPaint();
	
// Construction
public:

	CPicList();
	virtual ~CPicList();
	
	
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPicList)
	public:
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CPicList)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PICLIST_H__1541F698_EB8C_4FE2_8899_0DCBA739B636__INCLUDED_)
