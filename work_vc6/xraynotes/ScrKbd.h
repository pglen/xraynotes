
/* =====[ OnscreenKeyboardDlg.h ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the OnscreenKeyboardDlg.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  2/26/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#if !defined(AFX_KEYBOARDDLG_H__EFE558FB_FB84_11D3_8D1D_00C0F0405B24__INCLUDED_)
#define AFX_KEYBOARDDLG_H__EFE558FB_FB84_11D3_8D1D_00C0F0405B24__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CScrKbd dialog

#include "AfxTempl.h"
#include "GrayDlg.h"


#define TAB			0x02
#define CAPSLOCK	0x03
#define SHIFT		0x04
#define CTRL		0x05
#define BSPACE		0x06
#define ENTER		0x07
#define ALT			0x08
#define WINDOW		0x9
#define LEFT		0x0A
#define RIGHT		0x0B
#define UP			0x0C
#define DOWN		0x0D
#define ESC			0x0E
#define PUP			0x0F
#define PDN			0x10
#define HOME		0x11
#define END			0x12
#define INS			0x13
#define DEL			0x14
#define WWW			0x15

struct KEYDEF
	{
	char	cNormal, cShifted;
	int		cWidth;
	};

class CScrKbd : public CGrayDlg

{

public:
	
	int		cap, shift, control, alt;
	int		KeyWidth, KeyHeight;
	
	CTypedPtrList<CPtrList, KEYDEF*> cKeys;

	void	OnDestroy();

	CFont	smallfont, bigfont;
	CWnd	*oldwin;

	CScrKbd(CWnd* pParent = NULL);	// standard constructor

protected:

	int		topgap;
	DWORD	cCurrentKeyState;
	KEYDEF	*sendthis;
	CRect 	sendrc;

	DWORD	GetKBState();

	void	keybd_event2(
				BYTE bVk, BYTE bScan, DWORD dwFlags, DWORD dwExtraInfo);

	short	GetMyKeyState(DWORD key);

	void	UpdateStates(KEYDEF *keydef);
	void	PickFont(CClientDC *pDC, CRect &rc, CFont *oldFont);
	void	BuildKeyDef();
	void	SendKeyToWin(KEYDEF * keydef);
	void	AddKey(char pNormal, char pShifted, int pWidth);
	void	CalcWidthHeight();
	void	SendKey(KEYDEF * key);
	void	DrawKey(CDC * dc, CRect & rc2, KEYDEF * key, BOOL cHilight=FALSE);
	void	DrawKeyboard();

// Dialog Data
	//{{AFX_DATA(CScrKbd)
	enum { IDD = IDD_ONSCREENKEYBOARD_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScrKbd)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CScrKbd)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KEYBOARDDLG_H__EFE558FB_FB84_11D3_8D1D_00C0F0405B24__INCLUDED_)
