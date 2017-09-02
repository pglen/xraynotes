#if !defined(AFX_CSimpleGrid_H__D7F8E078_7CAF_433D_82EF_99F635B77B04__INCLUDED_)
#define AFX_CSimpleGrid_H__D7F8E078_7CAF_433D_82EF_99F635B77B04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CSimpleGrid.h : header file
//

#include "popup.h"
//include "BCMenu.h"        // BCMenu class declaration

#define CELL_MAGIC	0x1234

/////////////////////////////////////////////////////////////////////////////
// CSimpleGrid window

class CSimpleGrid : public CWnd

{
	typedef struct _cell
		{
		int		magic;
		int		type;
		int		cellcol;
		int		cellrow;
		int		bold;
		int		italic;
		int		ww,hh;
		int		color;
		int		trans;
		void	*content;
		} cell;
	
// Construction

public:

	int		first_bold;

protected:

	int		maxwidth;
	int		wasbold;
	int		currrow, currcol;
	int		head_size;

	int		init_row, init_col;
	int		cell_www, cell_hhh;

	int		capped, fired;
	int		sw, sh;

	cell	*cccc;
	
	//BCMenu	hrcmenu;
	//BCMenu	*hrclickmenu;

	RECT	charrc; 

	CPoint	mouse, capmouse;

	CPopup	tip;

	CPtrArray	rowarr;
	
	CFont m_font, m_fontb;

	CScrollBar  vs, hs;

// Attributes
public:

protected:
	int	rows, cols;
	int	startrow, startcol;
	int	vrows, vcols;

// Operations
public:

	CSimpleGrid();
	virtual ~CSimpleGrid();

	void AddStr(int qrow, int qcol, const char *str, int inval = true);
	void AutoSize();
	void Clear();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSimpleGrid)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_VIRTUAL

// Implementation
public:

protected:

	void Resized();
	void Pt2Cell(CPoint point, int *rowp, int *colp, CRect *rc);
	void SetCurr(CPoint  point);
	void ReAlloc(int newrow, int newcol);
	void InvalidateGrid();
	void FirstPaint();	
	void InvalidateCell(int row, int col);
	
	int		init_alloc;
	int		firstpaint;

	// Generated message map functions
protected:
	//{{AFX_MSG(CSimpleGrid)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CSimpleGrid_H__D7F8E078_7CAF_433D_82EF_99F635B77B04__INCLUDED_)
