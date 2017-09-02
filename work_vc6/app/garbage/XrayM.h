
/* =====[ XrayM.h ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the XrayM.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  10/31/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#if !defined(AFX_XRAYM_H__07A7BC02_F12C_4376_86D4_89F1AD2BADD9__INCLUDED_)
#define AFX_XRAYM_H__07A7BC02_F12C_4376_86D4_89F1AD2BADD9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XrayM.h : header file
//

#include "label.h"
#include "transslider.h"
#include "GrayDlg.h"
#include "popup.h"

#define MENU_MAGIC		0x345345
#define MENUITEM_MAGIC	0x342245

// The compiler needs a hint
class CXrayM;

class CXrayMitem 

{
public:
	void InitVars();

	CXrayMitem();
	CXrayMitem(const char *ss);
	~CXrayMitem();

	int			magic;
	int			selected;
	CXrayM		*submenu;
	int			command;
	int			status;
	int			checked;
	//char		str[128];

	CString		*itemstr;
	CString		*tipstr;
	CString		*tooltip;

	HBITMAP		bitmap;
	RECT		rect;	
	char		key[32];
	
};

/////////////////////////////////////////////////////////////////////////////
/// CXrayM Dialog. 
///

/// The windows menu got replaced becase of some ugly inconsistancies. <br>
/// Ex: Popup did not inherit color scheme, dialog lifetime 
/// did not allow retrieval after menu disappeared, leaky resource management
/// ...<br>
/// This replacement menu is eazy ... 
/// just call LoadAndShow(Resoure, skip, pointX, pointY) to work it.
///
/// This menu obey/load correct menu font and assume the XrayNotes 
/// theme correctly. This file is open sourced during development.
///

class CXrayM : public CGrayDlg

{	
	static		CPtrArray wndarr;

public:

	void		LoadAndShow(int nResource, int skip = 0, int xx = -1, int yy = -1);
	void		DelMenuItem(int idx);
	void		GetMenuItemText(int idx, CString &txt);
	
	BOOL		LoadMenu(int nResource, int skip = 0);
	BOOL		LoadMenu(LPCTSTR lpszResourceName, int skip = 0);
	
	void		SetToolTip(const char *str, const char *tipstr);
	void		SetToolTip(int idx, const char *tipstr);
	void		SetItemKey(int idx, const char *newstr);
	void		SetItemText(int idx, const char *newstr);

	int			SetItemStatus(int idx, int status);

	CXrayM		*FindSubmenu(const char *str);
	CXrayM		*FindSubmenu(int idx);
	
	int			FindMenuItem(const char *str);
	int			FindMenuItem(int command);

	int			GetNumItems();
	void		DumpMenu(CXrayM *menu, CString indent = "");
	void		SetCheck(int idx, int check);
	void		Clear();
	void		Hide(int special = false);
	int			AppendMenuItem(const char *str, int command = -1, int status = MF_ENABLED, int checked = false);
	int			AppendSubMenu(const char *str, CXrayM *submenu, int status = MF_ENABLED);
	void		Show(int xx, int yy, int sub = 0);

	CWnd		*sendto;
	BOOL		m_loaded;
	COLORREF	highlite, dlgbackcol, checkmark;

	COLORREF	disabled, grayed;

	int			magic, closeonup;

	// Construction
	CXrayM(CWnd* pParent = NULL);   // standard constructor
	CXrayM::~CXrayM();

	// Expose this as public, so it can be maniputated directly
	CPtrArray	arr;

protected:

	int			GotoPrev();
	int			GotoNext();

	int			GetSelected();
	int			OpenSubMenu(CXrayMitem *item, CPoint &point);
	int			ExecItem(CXrayMitem *item);
	int			IsAnySelected();
	int			IsItemSelectAble(CXrayMitem *item);
	
	DWORD		UnicodeToAnsi(LPWSTR pszW, CString &str);
	CPopup	tip;
	int		fired;
	int		OldLoop, textheight, gap, leftgap, topgap;
	
	POINT	pp[4];

	CXrayM	*subopen, *mparent;
	CPoint	lastmouse, capmouse;

	void	ScanForAction(CPoint &point, int NoClick);
	void	MeasureMenu(int *ww, int *hh);
	int		GetMenuTextSize(const char *str, SIZE &size);
	
	CWnd	*SafeGetMainWnd();
	CFont	m_fontMenu;
	
// Dialog Data
	//{{AFX_DATA(CXrayM)
	enum { IDD = IDD_MENUBASE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXrayM)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CXrayM)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XRAYM_H__07A7BC02_F12C_4376_86D4_89F1AD2BADD9__INCLUDED_)
