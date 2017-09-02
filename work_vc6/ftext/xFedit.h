// xFedit.h: interface for the CxFedit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XFEDIT_H__511C91C6_BC5B_4D70_AF75_443D0F8AEC87__INCLUDED_)
#define AFX_XFEDIT_H__511C91C6_BC5B_4D70_AF75_443D0F8AEC87__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../common/BaseWnd.h"
#include "../common/xRect.h"
#include "../common/mxpad.h"

//include "xScrollBar.h"
#include "../common/xptrarr.h"

#define LIMAGIC 0x23452345
#define CHMAGIC 0x22341345
#define FOMAGIC 0x23454321

#define A_LEFT	0
#define A_MID	1
#define A_RIGHT	2

class CxFedit : public CBaseWnd  

{

public:

	void	DumpText();
	
	void	Paste();
	void	Copy();

	int		SetAlign(int align);
	int		SetTextColor(int col);			// Set Color, ret old color
	void	Strikeout(int flag);			// Set font parameters
	void	Underline(int flag);
	void	Italic(int flag);
	void	Bold(int flag);					// Set/reset bold
	void	Size(int val);					// Set new font size
	
	void	Font(const char *str);	
	void	EnsureVisible();				// Ensure current cursor is in window
	
	void	ScrollVer(int num);				// Scroll by num (-N scrolls opposite)
	void	ScrollHor(int num);				// Scroll by num (-N scrolls opposite)

	CxFedit();
	virtual ~CxFedit();

	void	AddChar(char nChar);
	void	AddLine(CString &str);
	void	AddLine(const char *str);

	void	InsertLine(CString &str, int idx);

		// Public data
	CBitmap	m_bitmap;				// Splash Bitmap
	int		m_nWidth, m_nHeight;	// Splash Bitmap's width & height

protected:
	
	//////////////////////////////////////////////////////////////////////
	// Data

	LOGFONT		currlf;
	CxPtrArr	parr, fontarr;			// Array of lines, array of fonts
	CxRect		charrc;
	CScrollBar  vs, hs;
	CPoint	inpoint;

	int		currfgcol, currbgcol;
	int		lastfont, prevcurfont;
	int		mousedown;

	int		linespace;					// space between lines
	int		charspace;					// space between characters

	int		currx, curry, scroll, vscroll;
	
	int		bgcolor, bgnofocus, sw, sh, cnt;
	int		leftgap, rightgap, topgap, bottomgap;
	int		ssrow, sscol, serow, secol;

	int		oldx, oldy;
	int		align, in_sel;
	int		xdrag, ydrag;
	int		selstart, selend;

	//////////////////////////////////////////////////////////////////////
	// Internal methods

	int		RowCol2Linear(int row, int col, int *lin = NULL);
	void	Linear2RowCol(int lin, int &row, int &col);
	void	NormalizeSel();
	void	GetEditRect(RECT *rc);
	void	Point2Cur(CPoint point, int &cxx, int &cyy);
	int		FindFontMatch(LOGFONT *lf);
	void	BuildDist(int yy);
	void	InvalLine();
	void	SetCaret(int rgb = RGB(192, 192, 192), int timer = false);
	void	DrawCaret(int rgb, int xx, int yy, int drawline = false);
	void	InvalEd();

	// Overrides 

	virtual void	OnMouseEnter();
	virtual void	OnMouseLeave();
	virtual void	OnGetTipText(CString &str);
	virtual void	OnInitControl();
	virtual int		KeyEvent(UINT event, UINT nChar, UINT nFlags);
	
	//{{AFX_VIRTUAL(CxFedit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	//////////////////////////////////////////////////////////////////////////

class	CFontItem
{
public:

	int magic;
	
	LOGFONT	lf;
	CFont font;
	TEXTMETRIC tm;
	int	color;

	CFontItem() { magic = FOMAGIC;}
	CFontItem(LOGFONT *plf) 
		{ 
		magic = FOMAGIC; lf = *plf; color = RGB(0,0,0);
		font.CreateFontIndirect(&lf);
		
		CPaintDC dc(GetDesktopWindow());
		int sss = dc.SaveDC();
		CFont *old = (CFont*)dc.SelectObject(font);
		dc.GetTextMetrics(&tm);
		dc.RestoreDC(sss);

		//P2N("tm.tmDescent=%d tmAscent=%d\r\n", tm.tmDescent, tm.tmAscent ); 
		}
};

//////////////////////////////////////////////////////////////////////////

class	CCharItem
{
public:

	int		magic;				// Sanity check value

	TCHAR 	chh;				// Actual character
	DWORD	fg;					// fg color
	DWORD	bg;					// bg color
	DWORD	ww;					// width
	DWORD	ff;					// enum fonts numbers
	
	CCharItem()	 {magic = CHMAGIC; fg=ww=ff=0, bg=RGB(255,255,255);}
	~CCharItem() {ASSERT(magic == CHMAGIC);}
};

//////////////////////////////////////////////////////////////////////////

class	CLineItem
{
public:

	int			magic;			// Sanity check value
	int			num;			// Line number
	int			height;			// Calculated height of largest
	int			base;			// Baseline of largest 
	int			align;			// Alignment of the line
	int			start;			// Alignment start value
	
	CString		line;			// Actual line string 
	CxPtrArr	charr;			// Array of char descriptors

	CLineItem() {magic = LIMAGIC; num = 0; height = 0; 
					align = 0; base = 0; start = 0; }

	CLineItem(const char *str) 
					{magic = LIMAGIC; num = 0; height = 0; 
					align = 0; base = 0; start = 0; 
					line = str; }

	~CLineItem() 
		{
		ASSERT(magic == LIMAGIC);
		for(int loop = 0; loop < charr.GetSize(); loop++)
			{
			delete (CCharItem*)charr[loop];
			}
		}
};


	//{{AFX_MSG(CxFedit)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_XFEDIT_H__511C91C6_BC5B_4D70_AF75_443D0F8AEC87__INCLUDED_)

