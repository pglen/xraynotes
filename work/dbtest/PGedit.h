#if !defined(AFX_PGEDIT_H__D65965D6_7BE0_456C_B4E7_9C40E632D0E0__INCLUDED_)
#define AFX_PGEDIT_H__D65965D6_7BE0_456C_B4E7_9C40E632D0E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PGedit.h : header file
//

#define MAX_READ 128

/////////////////////////////////////////////////////////////////////////////
// CPGedit window

class CPGedit : public CEdit

{

protected:

	static	int	dic_good;
	
	static	FILE	*fp1;
	static	FILE	*fp2;
	static	FILE	*fp3;

	static	CDWordArray widx;
	static	CDWordArray wmedidx;

public:

	CStringArray sarr, rsarr;

	// User ajustble parts
	int		amaxsuggest;
	int		anoautoselect;
	int		ameddic, amsdelay;

	int		acheckon;
	
	void	aDisable();
	void	aEnable();

protected:

	UINT	m_uiTimerID;
	DWORD	m_dwLastTick;
	int		m_Drawn;
	char    buffer[MAX_READ];
    
	COLORREF	grey;
	CBrush		NewBrush;

	CString dicidx, dicmedidx, currword;
	CString dicmain, dicmed, dicuser;

	CStringList         ignore;
	CStringList         learn;
	
	CStringArray replace, replacemed;

	int		oldline, firsttime, disabled, in_spell;
	int		oldcheckon;

	void	zAddWordToDic();
	void	zIgnoreWord();
	void	MoreSuggest();
	void	ReplaceSpellingError(UINT uid);
	void	ReplaceMedError(UINT uid);
	void	Suggest(CMenu &menu, CString str);
	void	zInit_Spellcheck();
	int		SpellWord(CString &str);
	int		ProcessLine(int line, CString str, CRect &rect, CDC &dc);
	void	ProcessWord(CString &word, int pos, CDC &dc);
	void	zDrawSquigly(CDC &dc, int xx, int yy, int iWidth);
	int		zIndexDic(CString &dicf, CString &idxf);;
	void	zLoadIndex(CString &idxf, CDWordArray &warr);

	void	SelAll();
	void	zCheckLineChange();
	void	GetLine(int idx, CString &str);
	
	
// Construction
public:

	CPGedit();



// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPGedit)
	public:
	
	protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	
	virtual ~CPGedit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPGedit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnPaint();
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnChange();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PGEDIT_H__D65965D6_7BE0_456C_B4E7_9C40E632D0E0__INCLUDED_)
