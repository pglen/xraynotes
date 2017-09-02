
/* =====[ label.h ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the label.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  10/27/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#if !defined(AFX_LABEL_H__A4EABEC5_2E8C_11D1_B79F_00805F9ECE10__INCLUDED_)
#define AFX_LABEL_H__A4EABEC5_2E8C_11D1_B79F_00805F9ECE10__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Label.h : header file
//

#define STRTMP	1024

/////////////////////////////////////////////////////////////////////////////
// CLabel window

enum FlashType {None, Text, Background };


class CLabel : public CStatic

{
	DECLARE_DYNAMIC(CLabel)

// Construction
public:

	CLabel();
	virtual ~CLabel();

	CLabel&		SetBkColor(COLORREF crBkgnd);
	COLORREF	GetBkColor();
	
	CLabel& SetTextColor(COLORREF crText);

	CLabel& SetText(const CString& strText , int recalc = true);
	CLabel& SetTextP(const char * strText, ...);

	CLabel& SetFontBold(BOOL bBold);
	CLabel& SetFontName(const CString& strFont);
	CLabel& SetFontUnderline(BOOL bSet);
	CLabel& SetFontItalic(BOOL bSet);
	CLabel& SetFontSize(int nSize);

	CLabel& SetSunken(BOOL bSet);
	CLabel& SetBorder(BOOL bSet);
	
	CLabel& FlashText(BOOL bActivate);
	CLabel& FlashBk(BOOL bActivate);
	
	CLabel& SetLink(BOOL bLink);
	CLabel& SetLinkCursor(HCURSOR hCursor);
	
	CLabel& operator =(const char *str);
	
protected:

	CSize		textsize;

	int			m_inited, m_transparent;
	int			m_center, m_right;
	int			m_frame, m_edge, m_modframe;

	CString		m_text, m_backup;

	////////////////////////////////////////////////

	void		xInitLab();
	void		xReDraw();
	void		xReDrawParent();
	void		xReconstructFont();
	void		xCalcSize();

	COLORREF	m_bkcolor, m_crText;
	
	HBRUSH		m_hBrush, m_hwndBrush;
	
	LOGFONT		m_lf;
	CFont		m_font;

	BOOL		m_bState;
	BOOL		m_bTimer;
	BOOL		m_bLink;
	FlashType	m_Type;
	HCURSOR		m_hCursor;

	CRect		orig;

public:
	void DumpExStyles();
	void DumpSS();
	void DumpStyles();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLabel)
	//}}AFX_VIRTUAL

// Implementation
		
	void SetTrans(int flag = true);
	void CenterText(int flag = true);
	void GetText(CString &str);

	// Generated message map functions
protected:
	//{{AFX_MSG(CLabel)
	afx_msg BOOL OnEraseBkgnd( CDC* pDC );
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnPaint(void);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LABEL_H__A4EABEC5_2E8C_11D1_B79F_00805F9ECE10__INCLUDED_)
