// AlaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "xcal.h"
#include "AlaDlg.h"

#include "mxpad.h"
#include "sqlite3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
		P2N(" '%s' ", argv[loop]);	
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
	//}}AFX_DATA_INIT

	dbfile = "calendar.sqlt";
	dwarr.RemoveAll();
}


void CAlaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlaDlg)
	DDX_Control(pDX, IDC_TREE1, m_tree);
	DDX_Text(pDX, IDC_EDIT1, m_title);
	DDX_Text(pDX, IDC_EDIT2, m_num);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAlaDlg, CDialog)
	//{{AFX_MSG_MAP(CAlaDlg)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAlaDlg message handlers

BOOL CAlaDlg::OnInitDialog() 

{
	CDialog::OnInitDialog();

	SetWindowText(headstr);

	// Decorate the data array
	//farr[farrsize].name =  "lname";		farr[farrsize].var = &m_lname; farrsize++;

	// Fill in customer name tree
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	rc = sqlite3_open(dbfile, &db);

	if( rc )
		{
		CString tmp;tmp.Format("SQL error: %s", sqlite3_errmsg(db));
		AfxMessageBox(tmp);

		sqlite3_close(db);
		sqlite3_free(zErrMsg);
		return true;
		}

	for (int loop = 0; loop < dwarr.GetSize(); loop++)
		{	
		CString tmp; tmp.Format("%d", dwarr[loop]);
		//CString qqq = "select name, sdate, edate, note, id from calendar where id = " + tmp;

		CString qqq = "select name, id from calendar where id = " + tmp;

		//if(str != NULL)
		//	{
		//	if(strlen(str))
		//		{
		//		CString filter; filter.Format(" where lname like '%%%s%%'",  str);
		//		qqq += filter;
		//		}
		//	}
		//qqq += " order by udate desc";

		P2N("qqq= '%s'\r\n", qqq);

		rc = sqlite3_exec(db, qqq, callback_fill, this, &zErrMsg);
		if( rc != SQLITE_OK )
			{
			CString tmp; tmp.Format("SQL error: %s", sqlite3_errmsg(db));
			AfxMessageBox(tmp);
		
			sqlite3_free(zErrMsg);
			}
		}

	sqlite3_close(db);

	//m_tree.SortChildren(NULL);

	//if(!old)
	//	{
	//	m_tree.SelectItem(m_tree.GetRootItem());
	//	}
	//else
	//	{
	//	SelEntryInt(m_tree.GetItemData(old));
	//	}

	EndWaitCursor();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAlaDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	for(int loop = 0; loop < dwarr.GetSize(); loop++)
		{
		P2N("dwarr %d\r\n", dwarr[loop]);
		}
}
