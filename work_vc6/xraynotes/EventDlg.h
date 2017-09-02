
/* =====[ EventDlg.h ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the EventDlg.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  12/11/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#if !defined(AFX_EVENTDLG_H__85A90678_10CA_4506_B8E0_C2181E1C611A__INCLUDED_)
#define AFX_EVENTDLG_H__85A90678_10CA_4506_B8E0_C2181E1C611A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EventDlg.h : header file
//

#include "EditEx.h"
#include "PGEdit.h"

#include "dbdef.h"

#define EVENT_MAGIC		0x12345678

/////////////////////////////////////////////////////////////////////////////
// CEventDlg dialog

class CEventDlg : public CGrayDlg

{
// Construction

public:
	
	CString thisname;
	
	CEventDlg(CWnd* pParent = NULL);   // standard constructor

	int		magic, rescan, filling, rows, backup;
	int		farrsize;

	fields	farr[30];		// Make sure it will hold all of constructor's additions

	CString header, old_header;
	CString dbfile, dbfile2, calfile;
	CString dbtable, str;

protected:

	void	SetAppoint();
	void	CreateDBfile();
	void	SelLast();
	void	Fill(const char *str = NULL, int find = false);
	void	FillOne();
	void	SelEntry(const char * estr);
	void	init_array();

	int		UpdateDB(int id = -1);
	
	int check_table();

	void SelEntryInt(int num);
	void Qdata(int getid);

public:

// Dialog Data
	//{{AFX_DATA(CEventDlg)
	enum { IDD = IDD_EVENT_DLG };
	CPGedit	m_eid;
	CPGedit	m_ed5;
	CPGedit	m_ed4;
	CPGedit	m_ed1;
	CPGedit	m_ed3;
	CPGedit	m_emod;
	CPGedit	m_endate;
	CPGedit	m_tide;
	CPGedit	m_prle;
	CPGedit	m_epatid;
	CTreeCtrl	m_tree;
	CString	m_name;
	CString	m_tid;
	CString	m_patid;
	CString	m_diagnosis;
	CString	m_proc;
	CString	m_symptom;
	CString	m_diagcode;
	CString	m_pcode1;
	CString	m_pc1desc;
	CString	m_dcdesc;
	CString	m_bdate;
	CString	m_edate;
	CString	m_pcode3;
	CString	m_ndate;
	CString	m_note;
	CString	m_prl;
	CString	m_chdate;
	CString	m_appoint;
	CString	m_mdate;
	CString	m_id;
	CString	m_filter;
	CString	m_phys;
	CString	m_phys2;
	CString	m_trans;
	CString	m_res;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEventDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEventDlg)
	afx_msg void OnOk4();
	virtual BOOL OnInitDialog();
	afx_msg void OnOk5();
	afx_msg void OnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangingTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnOk6();
	afx_msg void OnButton2();
	afx_msg void OnButton23();
	afx_msg void OnButton22();
	afx_msg void OnButton38();
	afx_msg void OnSetfocusTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButton6();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButton7();
	afx_msg void OnButton14();
	afx_msg void OnButtonEd15();
	afx_msg void OnButtonEd24();
	afx_msg void OnButtonEd40();
	afx_msg void OnButtonEd39();
	afx_msg void OnEd7();
	afx_msg void OnButtonEd16();
	afx_msg void OnButtonEd17();
	afx_msg void OnKillfocusEditEd2();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnEd2();
	afx_msg void OnChangeEditEd50();
	afx_msg void OnEd9();
	afx_msg void OnEd12();
	afx_msg void OnEd8();
	afx_msg void OnKillfocusEditEd31();
	afx_msg void OnEd14();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EVENTDLG_H__85A90678_10CA_4506_B8E0_C2181E1C611A__INCLUDED_)
