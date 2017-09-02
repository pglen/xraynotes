// lines.h: interface for the Clines class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINES_H__F6604ED0_786E_4F0B_8C80_45FADCADA42F__INCLUDED_)
#define AFX_LINES_H__F6604ED0_786E_4F0B_8C80_45FADCADA42F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "label.h"

class Clines : public CStatic  

{
	DECLARE_DYNAMIC(Clines)

public:
	Clines();
	virtual ~Clines();

	CPoint		lstart;
	CPoint		lend;
	int			thick;
	COLORREF	color;

	//{{AFX_VIRTUAL(Clines)
	public:
	
	//}}AFX_VIRTUAL


	// Generated message map functions
protected:

public:

	//{{AFX_MSG(Clines)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_LINES_H__F6604ED0_786E_4F0B_8C80_45FADCADA42F__INCLUDED_)
