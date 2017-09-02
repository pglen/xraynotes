// AlaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "xraynotes.h"
#include "AlaDlg.h"

#include "support.h"
#include "parse.h"
#include "mxpad.h"
#include "sqlite3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static int callback_fillone(void *NotUsed, int argc, char **argv, char **azColName)

{
	CAlaDlg *lastclass = (CAlaDlg *)NotUsed;

	P2N("Data: ");
	for(int loop = 0; loop < argc; loop++)
		P2N(" %s = '%s' ", azColName[loop], argv[loop]);	
	P2N("\r\n");

	lastclass->m_title = argv[0];
	lastclass->m_num = argv[2];

	lastclass->UpdateData(false);

	return 0;
}


static int callback_fill(void *NotUsed, int argc, char **argv, char **azColName)

{
	CString str;
	CAlaDlg *lastclass = (CAlaDlg *)NotUsed;

	//P2N("callback_fill\r\n");
	//ASSERT(argc == 3);

	//str = argv[0];	str += ", "; str += argv[1];
	//HTREEITEM item = lastclass->m_tree.InsertItem(str, 1, 0);
	//DWORD id = atoi(argv[2]);
	//lastclass->m_tree.SetItemData(item, id);
	//P2N("Got item name: '%s' date: '%s'  id: %s\r\n", argv[0], argv[1], argv[2]);

	P2N("Data: ");
	for(int loop = 0; loop < argc; loop++)
		{
		P2N(" %s = '%s' ", azColName[loop], argv[loop]);	
		}
	P2N("\r\n");

	HTREEITEM item;
	if(argv[0])
		{
		item = lastclass->m_tree.InsertItem(argv[0]);
		}
	else
		{
		item = lastclass->m_tree.InsertItem("UnNamed");
		}

	if(item)
		{
		lastclass->m_tree.SetItemData(item, atoi(argv[1]));
		}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CAlaDlg dialog


CAlaDlg::CAlaDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAlaDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAlaDlg)
	m_title = _T("");
	m_num = _T("");
	m_note = _T("");
	//}}AFX_DATA_INIT


	//dwarr.RemoveAll();
}

void CAlaDlg::DoDataExchange(CDataExchange* pDX)

{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlaDlg)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_can);
	DDX_Control(pDX, IDC_TREE_ALA_1, m_tree);
	DDX_Text(pDX, IDC_EDIT_ALA_1, m_title);
	DDX_Text(pDX, IDC_EDIT_ALA_2, m_num);
	DDX_Text(pDX, IDC_EDIT_ALA_3, m_note);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAlaDlg, CDialog)
	//{{AFX_MSG_MAP(CAlaDlg)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_ALA_1, OnSelchangedTreeAla1)
	ON_BN_CLICKED(IDC_BUTTON_ALA_2, OnButtonAla2)
	ON_BN_CLICKED(IDC_BUTTON_ALA_1, OnButtonAla1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAlaDlg message handlers

BOOL CAlaDlg::OnInitDialog() 

{
	CDialog::OnInitDialog();

	support.GetDataRoot(calfile);
	calfile		+= "calendar.sqlt";

	abfile   = dataroot + "db\\address.sqlt";

	P2N("CAlaDlg::OnInitDialog dbfile=%s\r\n", calfile);

	SetWindowText(headstr);

	SetTimer(1, 100, NULL);

	// Decorate the data array
	//farr[farrsize].name =  "lname";		farr[farrsize].var = &m_lname; farrsize++;

	//CString res[3];
	//CString qqq2; 
	//qqq2.Format("select FirstName, Midname, Lastname from address where uidstr = '%s' limit 1", m_patid);
	//DLookup(abdbfile, qqq2, res, 3);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CAlaDlg::OnShowWindow(BOOL bShow, UINT nStatus) 

{
	CDialog::OnShowWindow(bShow, nStatus);
	
	//for(int loop = 0; loop < dwarr.GetSize(); loop++)
	//	{
	//	P2N("dwarr %d\r\n", dwarr[loop]);
	//	}
}

//////////////////////////////////////////////////////////////////////////

void CAlaDlg::OnTimer(UINT nIDEvent) 

{	
	if(nIDEvent == 1)
		{
		KillTimer(nIDEvent);

		// Fill in customer name tree
		sqlite3 *db;
		char *zErrMsg = 0;
		int rc;

		CString dstr;	dstr	= date.Format ("%Y/%m/%d %H:%M");
		CString dstr2;	dstr2	= date2.Format("%Y/%m/%d %H:%M");

		rc = sqlite3_open(calfile, &db);

		if( rc )
			{
			MBOXP("SQL error: %s", sqlite3_errmsg(db));
			sqlite3_close(db);
			sqlite3_free(zErrMsg);
			return ;
			}
		CString qqq;
		qqq.Format("select name, id, sdate from calendar "
					"where sdate >= '%s' and sdate < '%s'", dstr, dstr2);

		//P2N("qqq= '%s'\r\n", qqq);

		rc = sqlite3_exec(db, qqq, callback_fill, this, &zErrMsg);

		if( rc != SQLITE_OK )
			{
			MBOXP("SQL error: %s", sqlite3_errmsg(db));
			sqlite3_free(zErrMsg);
			}
		sqlite3_close(db);

		if(m_tree.GetCount() > 0)
			{
			//P2N("mtree getcount %d\r\n", m_tree.GetCount());

			m_tree.Select(m_tree.GetNextItem(NULL, TVGN_CHILD), TVGN_CARET);
			m_tree.SetFocus();
			}
		}

	CDialog::OnTimer(nIDEvent);
}


void CAlaDlg::OnSelchangedTreeAla1(NMHDR* pNMHDR, LRESULT* pResult) 

{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	HTREEITEM hItem  = m_tree.GetSelectedItem();

	if(!hItem)
		{
		//P2N("Changed tree no selection\r\n");
		return;
		}

	FillOne(m_tree.GetItemData(hItem));

	*pResult = 0;
}

void CAlaDlg::FillOne(int id)

{
	// Fill in customer name tree
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	rc = sqlite3_open(calfile, &db);

	if( rc )
		{
		MBOXP("SQL error: %s", sqlite3_errmsg(db));
		sqlite3_close(db);
		sqlite3_free(zErrMsg);
		return ;
		}

	CString dstr;	dstr	= date.Format ("%Y/%m/%d %H:%M:%S");
	CString dstr2;	dstr2	= date2.Format("%Y/%m/%d %H:%M:%S");

	CString qqq;

	qqq.Format("select name, note, sdate, tidstr, patid from calendar "
				"where id = %d", id);

	P2N("qqq= '%s'\r\n", qqq);

	rc = sqlite3_exec(db, qqq, callback_fillone, this, &zErrMsg);

	if( rc != SQLITE_OK )
		{
		MBOXP("SQL error: %s", sqlite3_errmsg(db));
		sqlite3_free(zErrMsg);
		}
	sqlite3_close(db);	
}

#include "AddrEntry.h"

void CAlaDlg::OnButtonAla2() 

{
	HTREEITEM hItem  = m_tree.GetSelectedItem();
	if(!hItem)
		{
		return;	
		}
	int id = m_tree.GetItemData(hItem);

	CString res[3];
	CString qqq; 
	qqq.Format("select patid from calendar where id = %d limit 1", id);
	P2N("qqq=[%s]\r\n", qqq);
	DLookup(calfile, qqq, res, 1);

	P2N("show patient %s\r\n", res[0]);	

	CAddrEntry ae;

	ae.command = res[0];

	ae.DoModal();
}

void CAlaDlg::OnButtonAla1() 

{
	HTREEITEM hItem  = m_tree.GetSelectedItem();
	if(!hItem)
		{
		return;	
		}
	int id = m_tree.GetItemData(hItem);

	CString res[1];
	CString qqq; 
	qqq.Format("select prl from address where id = %d limit 1", id);
	P2N("qqq=[%s]\r\n", qqq);
	DLookup(abfile, qqq, res, 1);

	P2N("prl for patient: %s\r\n", res[0]);	
}

