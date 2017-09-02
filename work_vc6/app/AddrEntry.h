
/* =====[ AddrEntry.h ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the AddrEntry.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  12/11/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#if !defined(AFX_ADDRENTRY_H__64A47214_9591_423D_A688_50B264EBABD8__INCLUDED_)
#define AFX_ADDRENTRY_H__64A47214_9591_423D_A688_50B264EBABD8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EditEx.h"
#include "xTargetButton.h"
#include "HShield.h"
#include "label.h"
#include "sqlite3.h"
#include "PGtree.h"
#include "xCalen.h"
#include "dbdef.h"

/////////////////////////////////////////////////////////////////////////////
// CAddrEntry dialog

class CAddrEntry : public CGrayDlg

{
	
// Construction
public:
	void LoadToolTips();

	//BCMenu	hrcmenu;
	//BCMenu	*hrclickmenu;

	CString		command;

	int		ExecSql(const char *fname, const char *sql, sqlite3_callback callb);
	int		CreateABfile(const char *db, int backup = false);
	void	FlushDB();
	int		IsDataChanged();
	void	CAddrEntry::init_array();

	POINT	curr;

	char *PleaseSel;
	//CHShield shield;

	void	FillOne();
	void	FillPRL();
	void	SelLast();

	CString m_currp;
	int		rows;

	//int CAddrEntry::callback2(void *NotUsed, int argc, char **argv, char **azColName);

	void	Fill(const char *str = NULL, int find = 0);
	void	Qdata(int getid);
	void	SelEntry(const char * estr) ;
	void	SelEntryInt(int num,  int focus = true) ;
	int		UpdateDB(int id) ;
	void	ParseCSV(CString &str);
	void	ParseCSV_NS(CString &str);
	void	ParseDumpCSV(CString &str);

	CImageList  *pImageList;
	CFile	filehand;
	int		first;

	int		farrsize, flarrsize;

	// There is an ASSERT macro checking this. One time only init, no need to make it dynamic
	fields	farr[50];			// Make sure it hold all of initdialog's additions
	flags	flarr[10];			// Make sure it hold all of initdialog's additions

	CString sumstr;
	CString dbfile, dbfile2;
	int		count;

	CAddrEntry(CWnd* pParent = NULL);   // standard constructor
	~CAddrEntry();

	DWORD	itemdata;
	int		rescan, filling;

// Dialog Data
	//{{AFX_DATA(CAddrEntry)
	enum { IDD = IDD_ADDR_DIALOG };
	CxTargetButton	m_b15;
	CxTargetButton	m_b3;
	CxTargetButton	m_hist;
	CEditEx	m_ealias;
	CEditEx	m_efname;
	CEditEx	m_ecity;
	CEditEx	m_eaddress;
	CEditEx	m_eqnote;
	CEditEx	m_eemail2;
	CEditEx	m_ecext;
	CEditEx	m_ecell;
	CEditEx	m_efaxext;
	CEditEx	m_eext;
	CEditEx	m_efax;
	CEditEx	m_eemail;
	CxTargetButton	m_ok6;
	CxTargetButton	m_ok8;
	CxTargetButton	m_ok7;
	CxTargetButton	m_ok3;
	CxTargetButton	m_b6;
	CxTargetButton	m_b30;
	CxTargetButton	m_b13;
	CxTargetButton	m_b12;
	CEditEx	m_esuffix;
	CEditEx	m_eprefix;
	CEditEx	m_ed15;
	CxTargetButton	m_ok5;
	CEditEx	m_elname;
	CxTargetButton	m_ok4;
	CxTargetButton	m_b9;
	CxTargetButton	m_b8;
	CxTargetButton	m_b29;
	CxTargetButton	m_b10;
	CEditEx	m_ebcity;
	CEditEx	m_ecdate;
	CEditEx	m_ecid;
	CEditEx	m_ebcountry;
	CEditEx	m_ebzip;
	CEditEx	m_ebfirst;
	CEditEx	m_eblast;
	CEditEx	m_enote;
	CEditEx	m_ebnote;
	CEditEx	m_ebmname;
	CEditEx	m_emname;
	CEditEx	m_ebdate;
	CEditEx	m_ebplace;
	CEditEx	m_ebstate;
	CxTargetButton	m_b22;
	CxTargetButton	m_b11;
	CxTargetButton	m_b2;
	CxTargetButton	m_b5;
	CxTargetButton	m_can;
	CxTargetButton	m_ok2;
	CxTargetButton	m_ok;
	CEditEx	m_eprl;
	CEditEx	m_euniq;
	CPGtree	m_tree;
	CString	m_lname;
	CString	m_email;
	CString	m_qnote;
	CString	m_office;
	CString	m_address;
	CString	m_city;
	CEditEx	m_ezip;
	CString	m_zip;
	CEditEx	m_etel;
	CString	m_tel;
	CString	m_fax;
	CString	m_faxext;
	CString	m_ext;
	CEditEx	m_epost;
	CString	m_post;
	CEditEx	m_estate;
	CString	m_state;
	CString	m_fname;
	CString	m_email2;
	CString	m_cell;
	CString	m_cext;
	CString	m_cid;
	CString	m_filter;
	CString	m_uniq;
	CString	m_cdate;
	CString	m_prl;
	CString	m_bplace;
	CString	m_bcity;
	CString	m_bstate;
	CString	m_bcountry;
	CString	m_bdate;
	CString	m_bzip;
	CString	m_bnote;
	CEditEx	m_ebcode;
	CString	m_bcode;
	CEditEx	m_ecountry;
	CString	m_country;
	CEditEx	m_eccode;
	CString	m_ccode;
	CEditEx	m_eudate;
	CString	m_udate;
	CEditEx	m_ewtel;	
	CString	m_wtel;
	CString	m_wext;
	CEditEx	m_ewext;
	CString	m_blast;
	CString	m_bfirst;
	CString	m_mname;
	CString	m_bmname;
	CString	m_note;
	CString	m_findbox;
	CString	m_count;
	BOOL	m_active;
	CString	m_suffix;
	CString	m_prefix;
	CString	m_alias;
	//}}AFX_DATA

	CString	m_lname2;
	CString	m_email22;
	CString	m_prefix2;
	CString	m_qnote2;
	CString	m_office2;
	CString	m_address2;
	CString	m_city2;
	CString	m_zip2;
	CString	m_tel2;
	CString	m_fax2;
	CString	m_faxext2;
	CString	m_note2;
	CString	m_ext2;
	CString	m_post2;
	CString	m_state2;
	CString	m_fname2;
	CString	m_email222;
	CString	m_cell2;
	CString	m_cext2;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddrEntry)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAddrEntry)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButton5();
	afx_msg void OnSelchangingTree1(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnCancel();
	afx_msg void OnDblclkTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButton8();
	afx_msg void OnButton9();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButton10();
	afx_msg void OnButton11();
	afx_msg void OnChangeEdit14();
	afx_msg void OnOk2();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnButton29();
	afx_msg void OnButton22();
	afx_msg void OnKillfocusEdit23();
	afx_msg void OnChangeEdit1();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnCheck1();
	afx_msg void OnKillfocusAbEdit9();
	afx_msg void OnAbButton12();
	afx_msg void OnSetfocusAbTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnOk3();
	afx_msg void OnCheck3();
	afx_msg void OnAbOk2();
	afx_msg void OnAbOk5();
	afx_msg void OnAbOk4();
	afx_msg void OnAbOk7();
	afx_msg void OnAbOk8();
	afx_msg void OnAbButton30();
	afx_msg void OnAbButton13();
	afx_msg void OnAbOk6();
	afx_msg void OnAbCheck4();
	afx_msg void OnAbCheck1();
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnAbButton14();
	afx_msg void OnAbButton3();
	afx_msg void OnAbButton15();
	afx_msg void OnAbButton6();
	afx_msg void OnRclickAbTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDRENTRY_H__64A47214_9591_423D_A688_50B264EBABD8__INCLUDED_)
