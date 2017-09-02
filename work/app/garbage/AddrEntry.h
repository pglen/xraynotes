#if !defined(AFX_ADDRENTRY_H__64A47214_9591_423D_A688_50B264EBABD8__INCLUDED_)
#define AFX_ADDRENTRY_H__64A47214_9591_423D_A688_50B264EBABD8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddrEntry.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAddrEntry dialog

class CAddrEntry : public CDialog
{
// Construction
public:
	void SelLast();

	//int CAddrEntry::callback2(void *NotUsed, int argc, char **argv, char **azColName);

	void	CAddrEntry::Fill();
	void	CAddrEntry::Qdata(int getid);
	void	CAddrEntry::SelEntry(const char * estr) ;
	void	CAddrEntry::SelEntryInt(int num) ;
	int		CAddrEntry::UpdateDB(int id) ;
	void	CAddrEntry::ParseCSV(CString &str);
	void	CAddrEntry::ParseCSV_NS(CString &str);
	void	CAddrEntry::ParseDumpCSV(CString &str);
	CString CAddrEntry::EscapeSql(CString &str);
	CString CAddrEntry::EscapeComma(CString &str);

	CImageList  *pImageList;
	CFile	filehand;
	int		first;

	CString *arr[20];
	CString *arr2[20];

	CString sumstr;
	CString dbfile;
	int		count;

	CAddrEntry(CWnd* pParent = NULL);   // standard constructor

	DWORD	itemdata;
	int		rescan;

// Dialog Data
	//{{AFX_DATA(CAddrEntry)
	enum { IDD = IDD_DIALOG31 };
	CTreeCtrl	m_tree;
	CString	m_lname;
	CString	m_email;
	CString	m_prefix;
	CString	m_qnote;
	CString	m_office;
	CString	m_address;
	CString	m_city;
	CString	m_zip;
	CString	m_tel;
	CString	m_fax;
	CString	m_faxext;
	CString	m_note;
	CString	m_ext;
	CString	m_post;
	CString	m_state;
	CString	m_fname;
	CString	m_email2;
	CString	m_cell;
	CString	m_cext;
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
	afx_msg void OnButton3();
	afx_msg void OnButton2();
	afx_msg void OnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButton5();
	afx_msg void OnSelchangingTree1(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnCancel();
	afx_msg void OnDblclkTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButton6();
	afx_msg void OnButton7();
	afx_msg void OnButton8();
	afx_msg void OnButton9();
	afx_msg void OnSetfocusTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButton37();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDRENTRY_H__64A47214_9591_423D_A688_50B264EBABD8__INCLUDED_)
