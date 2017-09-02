
/* =====[ AddrEntry.cpp ]====================================================

   Description:     Address book: implementation file

   Compiled:        MS-VC 6.00

   Revisions:

      REV     DATE     BY           DESCRIPTION
      ----  --------  ----------    --------------------------------------
      0.00  xx.xx.05  Peter Glen    Initial version.
	  1.00  10/3/03   Peter Glen    Release 1.00
	  1.01  2/13/06   Peter Glen    Release 1.01, Abook install fix
	  1.03  3/20/06   Peter Glen    Release 1.03, OEM

   ======================================================================= */

//////////////////////////////////////////////////////////////////////////
// AddrEntry.cpp : implementation file
//

/* -------- System includes:  -------------------------------------------- */

#include "stdafx.h"
#include "xraynotes.h"
#include <io.h>
#include <direct.h>

CString abook_root;

/* -------- Local includes:  -------------------------------------------- */

#include "AddrEntry.h"
#include "support.h"
#include "sqlite3.h"

#include "parse.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static	CAddrEntry *lastclass = NULL;
static	CString  str;
static	int rows = 0;

////////////////////////////////////////////////////////////////////////////

CString CAddrEntry::EscapeSql(CString &str)

{
	CString  res;

	int len = str.GetLength();

	for (int loop = 0; loop < len; loop++)
		{
		char chh = str.GetAt(loop);
		res +=  chh;
		if(chh == '\'')
			res +=  chh;
		}
	return res;
}


CString CAddrEntry::EscapeComma(CString &str)

{
	CString  res;

	int len = str.GetLength();

	for (int loop = 0; loop < len; loop++)
		{
		char chh = str.GetAt(loop);

		switch(chh)
			{
			case '\r':
				res +=  "\\r";
				break;

			case '\n':
				res +=  "\\n";
				break;

			case ',':
				res +=  ";";
				break;

			default:
				res +=  chh;
				break;
			}
		}
	return res;
}

//////////////////////////////////////////////////////////////////////////
// CAddrEntry dialog

CAddrEntry::CAddrEntry(CWnd* pParent /*=NULL*/)
	: CDialog(CAddrEntry::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddrEntry)
	m_lname = _T("");
	m_email = _T("");
	m_prefix = _T("");
	m_qnote = _T("");
	m_office = _T("");
	m_address = _T("");
	m_city = _T("");
	m_zip = _T("");
	m_tel = _T("");
	m_fax = _T("");
	m_faxext = _T("");
	m_note = _T("");
	m_ext = _T("");
	m_post = _T("");
	m_state = _T("");
	m_fname = _T("");
	m_email2 = _T("");
	m_cell = _T("");
	m_cext = _T("");
	//}}AFX_DATA_INIT

	count = 0;
	first = false;
	rescan = false;
}

//////////////////////////////////////////////////////////////////////////

void CAddrEntry::DoDataExchange(CDataExchange* pDX)

{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddrEntry)
	DDX_Control(pDX, IDC_TREE1, m_tree);
	DDX_Text(pDX, IDC_EDIT1, m_lname);
	DDX_Text(pDX, IDC_EDIT2, m_email);
	DDX_Text(pDX, IDC_EDIT3, m_prefix);
	DDX_Text(pDX, IDC_EDIT5, m_qnote);
	DDX_Text(pDX, IDC_EDIT6, m_office);
	DDX_Text(pDX, IDC_EDIT7, m_address);
	DDX_Text(pDX, IDC_EDIT8, m_city);
	DDX_Text(pDX, IDC_EDIT18, m_zip);
	DDX_Text(pDX, IDC_EDIT19, m_tel);
	DDX_Text(pDX, IDC_EDIT20, m_fax);
	DDX_Text(pDX, IDC_EDIT22, m_faxext);
	DDX_Text(pDX, IDC_EDIT23, m_note);
	DDX_Text(pDX, IDC_EDIT21, m_ext);
	DDX_Text(pDX, IDC_EDIT4, m_post);
	DDX_Text(pDX, IDC_EDIT10, m_state);
	DDX_Text(pDX, IDC_EDIT9, m_fname);
	DDX_Text(pDX, IDC_EDIT26, m_email2);
	DDX_Text(pDX, IDC_EDIT24, m_cell);
	DDX_Text(pDX, IDC_EDIT25, m_cext);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAddrEntry, CDialog)
	//{{AFX_MSG_MAP(CAddrEntry)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, OnSelchangedTree1)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_NOTIFY(TVN_SELCHANGING, IDC_TREE1, OnSelchangingTree1)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE1, OnDblclkTree1)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	ON_BN_CLICKED(IDC_BUTTON8, OnButton8)
	ON_BN_CLICKED(IDC_BUTTON9, OnButton9)
	ON_NOTIFY(NM_SETFOCUS, IDC_TREE1, OnSetfocusTree1)
	ON_BN_CLICKED(IDC_BUTTON37, OnButton37)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

static int callback(void *NotUsed, int argc, char **argv, char **azColName)

{
#if 0
	P2N("Callback\r\n");

	for(int ii = 0;  ii < argc; ii++)
		{
		P2N("%s=%s  ", azColName[ii], argv[ii] ? argv[ii] : "NULL");
		}
	P2N("\r\n");
#endif

	return 0;
}

//////////////////////////////////////////////////////////////////////////

static int callback0(void *NotUsed, int argc, char **argv, char **azColName)

{
	CString str;

	//P2N("Callback0\r\n");

	ASSERT(argc == 3);

	str = argv[0];	str += ", "; str += argv[1];
	HTREEITEM item = lastclass->m_tree.InsertItem(str, 1, 0);

	DWORD id = atoi(argv[2]);
	lastclass->m_tree.SetItemData(item, id);

	//P2N("Got item id: %s %d\r\n", argv[2], id);

	return 0;
}

//////////////////////////////////////////////////////////////////////////

static	int		callback2(void *NotUsed, int argc, char **argv, char **azColName)

{
	CString  tmp, ttmp;

	//P2N("Callback2\r\n");

	//P2N("Lastcklass %p\r\n", lastclass);

	rows ++;
	int i;
	for(i=0; i < argc; i++)
		{
		ttmp.Format("%s=%s  ",  azColName[i], argv[i] ? argv[i] : "NULL");

		if(argv[i])
			{
			*(lastclass->arr[i])  = argv[i];
			*(lastclass->arr2[i]) = argv[i];
			}
		else
			{
			*(lastclass->arr[i]) = "";
			*(lastclass->arr2[i]) = "";
			}
		tmp += ttmp;
		}

  tmp += "\r\n\r\n";
  lastclass->sumstr += tmp;

  return 0;
}

//////////////////////////////////////////////////////////////////////////

static int		callback3(void *NotUsed, int argc, char **argv, char **azColName)

{
	CString  tmp, ttmp;

	//P2N("Callback3\r\n");

	rows ++;
	int i;
	for(i=0; i < argc; i++)
		{
		ttmp.Format("%s=%s  ",  azColName[i], argv[i] ? argv[i] : "NULL");

		//P2N(ttmp);
		tmp += ttmp;
		}

  tmp += "\r\n\r\n";
  //P2N("Iterated: %s\r\n", tmp);

  return 0;
}

//////////////////////////////////////////////////////////////////////////

static int		callback4(void *NotUsed, int argc, char **argv, char **azColName)

{
	CString str;

	//P2N("Callback4\r\n");

	for(int ii=0; ii < argc; ii++)
		{
		if(argv[ii])
			str = argv[ii];
		else
			str = "";

		str = lastclass->EscapeComma(str);
		str += ",";

		lastclass->filehand.Write(str, str.GetLength());
		}

	lastclass->filehand.Write("\r\n", 2);

	return 0;
}

static	CString old_lname, old_fname, old_email;
static	CDWordArray dwarr;

//////////////////////////////////////////////////////////////////////////

static int callback5(void *NotUsed, int argc, char **argv, char **azColName)

{
	for(int ii = 0; ii < argc; ii++)
		{
		//P2N("%s = %s\r\n", azColName[i], argv[i] ? argv[i] : "NULL");

		if(!argv[ii])
			argv[ii] = "";
		}

	if(argv[0] == old_lname &&
			argv[1] == old_fname &&
				argv[2] == old_email)
		{
		DWORD dw = atoi(argv[3]);

		//P2N("Same lname and fname on %d\r\n", dw);

		dwarr.Add(dw);
		}

	old_lname = argv[0]; old_fname = argv[1]; old_email = argv[2];

	//P2N("\r\n");

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// CAddrEntry message handlers

BOOL CAddrEntry::OnInitDialog()
{
	CDialog::OnInitDialog();

	BYTE	bAlpha = 240;

	if (bAlpha < 255)
		{
        //  Set layered style for the dialog
        SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);

        //  Call it with 255 as alpha - opacity
		if(g_pSetLayeredWindowAttributes)
			g_pSetLayeredWindowAttributes(m_hWnd, 0, bAlpha, LWA_ALPHA);
		}

	lastclass = this;

	//RepairPath(abook_root);
	//dbfile = "c:\\xcpt\\stamps\\address.sqlt";
	dbfile = abook_root + "address.sqlt";
	
#if 0

	if(access(abook_root, 0) < 0)
		{
		MBOX("Cannot access address book path.");
		EndDialog(IDCANCEL);
		return true;
		}

	CString dbfile2 = "c:\\xcpt\\stamps\\blank.sqlt";

	if(access(dbfile, 0) < 0)
		{
		CString dbfile2 = "c:\\xcpt\\stamps\\blank.sqlt";
		CopyFile(dbfile2, dbfile, false);
		}

	if(access(dbfile, 0) < 0)
		{
		MBOX("Address book database file not found, addressbook is not available");
		EndDialog(IDCANCEL);
		return true;
		}
#endif

//	if(!AddrBook_Recover())
//		{
//		EndDialog(IDCANCEL);
//		return true;
//		}

#if 1
	arr[0]	= 	&m_lname;
	arr[1]	= 	&m_email;
	arr[2]	= 	&m_prefix;
	arr[3]	= 	&m_qnote;
	arr[4]	= 	&m_office;
	arr[5]	= 	&m_address;
	arr[6]	= 	&m_city;
	arr[7]	= 	&m_zip;
	arr[8]	= 	&m_tel;
	arr[9]	= 	&m_fax;
	arr[10]	= 	&m_faxext;
	arr[11]	= 	&m_note;
	arr[12]	= 	&m_ext;
	arr[13]	= 	&m_post;
	arr[14]	= 	&m_state;
	arr[15]	= 	&m_fname;
	arr[16]	= 	&m_email2;
	arr[17]	= 	&m_cell;
	arr[18]	= 	&m_cext;
#endif

#if 1
	arr2[0]	= 	&m_lname2;
	arr2[1]	= 	&m_email22;
	arr2[2]	= 	&m_prefix2;
	arr2[3]	= 	&m_qnote2;
	arr2[4]	= 	&m_office2;
	arr2[5]	= 	&m_address2;
	arr2[6]	= 	&m_city2;
	arr2[7]	= 	&m_zip2;
	arr2[8]	= 	&m_tel2;
	arr2[9]	= 	&m_fax2;
	arr2[10]	= 	&m_faxext2;
	arr2[11]	= 	&m_note2;
	arr2[12]	= 	&m_ext2;
	arr2[13]	= 	&m_post2;
	arr2[14]	= 	&m_state2;
	arr2[15]	= 	&m_fname2;
	arr2[16]	= 	&m_email222;
	arr2[17]	= 	&m_cell2;
	arr2[18]	= 	&m_cext2;
#endif

	CBitmap	 bitmap;

	pImageList = new CImageList();
	pImageList->Create(16, 16, ILC_COLOR8, 3, 4);

	bitmap.LoadBitmap(IDB_BITMAP8);
	pImageList->Add(&bitmap, &bitmap);
	bitmap.DeleteObject();

	bitmap.LoadBitmap(IDB_BITMAP7);
	pImageList->Add(&bitmap, &bitmap);
	bitmap.DeleteObject();

	bitmap.LoadBitmap(IDB_BITMAP7);
	pImageList->Add(&bitmap, &bitmap);
	bitmap.DeleteObject();

	m_tree.SetImageList(pImageList, TVSIL_NORMAL);

	Fill();

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

void CAddrEntry::Fill()

{
	rescan = 0;

	m_tree.DeleteAllItems();
	m_tree.Invalidate();

	BeginWaitCursor();
	//support.YieldToWinEx();

	// Fill in customer name tree
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	rc = sqlite3_open(dbfile, &db);

	lastclass = this;

	if( rc )
		{
		MBOXP("SQL error: %s", sqlite3_errmsg(db));
		sqlite3_close(db);
		return;
		}

	rc = sqlite3_exec(db, "select lname, fname, id from address", callback0, 0, &zErrMsg);
	if( rc != SQLITE_OK )
		{
		MBOXP("SQL error: %s", sqlite3_errmsg(db));
		}
	sqlite3_close(db);

	m_tree.SortChildren(NULL);

	EndWaitCursor();

}


//////////////////////////////////////////////////////////////////////////

void CAddrEntry::OnOK()

{
	HTREEITEM hItem  = m_tree.GetSelectedItem();
	if(hItem)
		{
		UpdateDB(m_tree.GetItemData(hItem));
		}
	CDialog::OnOK();
}


//////////////////////////////////////////////////////////////////////////

void CAddrEntry::OnButton1()

{
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	HTREEITEM hItem  = m_tree.GetSelectedItem();
	if(hItem)
		{
		UpdateDB(m_tree.GetItemData(hItem));
		}

	rc = sqlite3_open(dbfile, &db);

	if( rc )
		{
		MBOX("cannot open SQL");
		sqlite3_close(db);
		exit(1);
		}

	lastclass = this;
	str = "";	sumstr = "";

	rows = 0;

	CString qqq("insert into address (lname, fname) values('");

	qqq +=  "New', '";
	qqq +=  "Entry')";

	rc = sqlite3_exec(db, qqq, callback2, 0, &zErrMsg);

	if( rc != SQLITE_OK )
		{
		MBOXP("SQL error: %s", sqlite3_errmsg(db));
		return;
		}

	int rowid = (int)sqlite3_last_insert_rowid(db);
	sqlite3_close(db);

	Fill();
	SelEntryInt(rowid);

	GetDlgItem(IDC_EDIT1)->SetFocus();
	((CEdit *)GetDlgItem(IDC_EDIT1))->SetSel(0, -1);

	UpdateData(false);
}

//////////////////////////////////////////////////////////////////////////

void CAddrEntry::Qdata(int getid)

{
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	rc = sqlite3_open(dbfile, &db);

	if( rc )
		{
		MBOX("cannot open SQL");
		sqlite3_close(db);
		exit(1);
		}

	lastclass = this;
	str = "";	sumstr = "";
	rows = 0;

	CString qqq = "Select "
					"lname, email, prefix, qnote, office, address, city, "
					"zip, tel, fax, faxext, note, ext, post, state, "
					"fname, email2, cell, cext "
					" from address where id = ";

	CString tmp; tmp.Format("%d", getid);
	qqq += tmp;

	rc = sqlite3_exec(db, qqq, callback2, 0, &zErrMsg);

	if( rc != SQLITE_OK )
		{
		MBOXP("err: %s", sqlite3_errmsg(db));
		}
	sqlite3_close(db);

	UpdateData(false);
}

//////////////////////////////////////////////////////////////////////////

void CAddrEntry::OnButton3()

{
	Qdata(count++);
}

//////////////////////////////////////////////////////////////////////////

void CAddrEntry::OnButton2()

{
	Qdata(count--);
}

//////////////////////////////////////////////////////////////////////////

void CAddrEntry::OnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult)

{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	CString dir, str, num;
	HTREEITEM hItem  = m_tree.GetSelectedItem();

	*pResult = 0;

	// Clear all fields
	for(int ii=0; ii < 19; ii++)
		{
		*arr[ii] = "";
		}

	UpdateData(false);

	if(!hItem)
		return;

	str =  m_tree.GetItemText(hItem);

	if(str == "")
		return;

	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	lastclass = this;
	CString ll, rr;

	// Fill in current entry
	rc = sqlite3_open(dbfile, &db);
	if( rc )
		{
		MBOX("cannot open SQL");
		sqlite3_close(db);
		}

	CString qqq;
	qqq = "Select "
		"lname, email, prefix, qnote, office, address, city, "
				"zip, tel, fax, faxext, note, ext, post, state, "
				"fname, email2, cell, cext "
		" from address where id = ";

	CString tmp;
	tmp.Format("%d", m_tree.GetItemData(hItem));
	qqq +=  tmp;

	//MBOX(qqq);

	rc = sqlite3_exec(db, qqq, callback2, 0, &zErrMsg);

	if( rc != SQLITE_OK )
		{
		MBOXP("err: %s", sqlite3_errmsg(db));
		}
	else
		{
		P2N(sumstr);
		//P2N("\r\n");
		}
	sqlite3_close(db);

	UpdateData(false);
}

//////////////////////////////////////////////////////////////////////////

void CAddrEntry::OnButton5()

{
	HTREEITEM hItem2  = m_tree.GetSelectedItem();
	if(hItem2)
		{
		UpdateDB(m_tree.GetItemData(hItem2));
		}

	CString dir, str, num;
	HTREEITEM hItem  = m_tree.GetSelectedItem();

	if(!hItem)
		return;

	int item = m_tree.GetItemData(hItem);
	str =  m_tree.GetItemText(hItem);

	// Select next one when deleting
	HTREEITEM item3 = m_tree.GetNextItem(hItem, TVGN_NEXT);

	if(str == "")
		{
		xMsgBox("Please select an entry first.");
		return;
		}

	CString str2;
	if(item3)
		str2 = m_tree.GetItemText(item3);

	CString tmpbox;
	tmpbox.Format("Are you sure you want to delete: '%s'", str);

	if(xMsgBox(tmpbox, "", MB_YESNO) != IDYES)
		return ;

	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	lastclass = this;

	rc = sqlite3_open(dbfile, &db);
	if( rc )
		{
		MBOX("cannot open SQL");
		sqlite3_close(db);
		return;
		}

	CString qqq;
	qqq.Format("delete from address where id==%d", item);
	P2N("Calling sqlite select\r\n");
	rc = sqlite3_exec(db, qqq, callback, 0, &zErrMsg);

	if( rc != SQLITE_OK )
		{
		MBOXP("SQL error: %s", sqlite3_errmsg(db));
		}
	sqlite3_close(db);

	// Clear all fields
	for(int ii=0; ii < 19; ii++)
		{
		*arr[ii ] = "";
		}

	UpdateData(false);

	Fill();

	// Select next in line
	if(str2 != "")
		SelEntry(str2);
	else
		SelLast();

	m_tree.SetFocus();
}

//////////////////////////////////////////////////////////////////////////

void CAddrEntry::SelEntry(const char * estr)

{
	// Select newly created patient
	HTREEITEM item = m_tree.GetNextItem( m_tree.GetSelectedItem(), TVGN_ROOT );
	while(true)
		{
		if(!item)
			break;

		CString str = m_tree.GetItemText(item);

		//P2N("Item retrieved %s\r\n", str);

		if(str == estr)
			{
			//P2N("Selecting %s\r\n", str);
			m_tree.SelectItem(item);
			m_tree.Invalidate();
			support.YieldToWinEx();
			break;
			}
		item = m_tree.GetNextItem(item, TVGN_NEXT );
		}
}

//////////////////////////////////////////////////////////////////////////

void CAddrEntry::SelEntryInt(int num)

{
	HTREEITEM item = m_tree.GetNextItem( m_tree.GetSelectedItem(), TVGN_ROOT );
	while(true)
		{
		if(!item)
			break;

		int data = m_tree.GetItemData(item);

		//P2N("Item retrieved %d\r\n", data);

		if(data == num)
			{
			//P2N("Selecting %d\r\n", data);
			m_tree.SelectItem(item);
			m_tree.Invalidate();
			m_tree.SetFocus();
			support.YieldToWinEx();
			break;
			}
		item = m_tree.GetNextItem(item, TVGN_NEXT );
		}
}

//////////////////////////////////////////////////////////////////////////

void CAddrEntry::OnSelchangingTree1(NMHDR* pNMHDR, LRESULT* pResult)

{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	*pResult = 0;

	HTREEITEM hItem  = m_tree.GetSelectedItem();

	if(!hItem)
		return;

	CString str =  m_tree.GetItemText(hItem);

	str = m_lname + ", " + m_fname;
	m_tree.SetItemText(hItem, str);

	rescan = UpdateDB(m_tree.GetItemData(hItem));
}

//////////////////////////////////////////////////////////////////////////

int		CAddrEntry::UpdateDB(int id)

{
	UpdateData();

	int changed = false;

	// Compare all fields
	for(int ii=0; ii < 19; ii++)
		{
		if(*arr[ii] != *arr2[ii])
			{
			//MBOXP("changed data from %s %s", *arr[i], *arr2[i]);
			changed = true;
			break;
			}
		}

	lastclass = this;

	if(!changed)
		return 0;

	sqlite3 	*db;
	char 		*zErrMsg = 0;
	int 		rc;

	rc = sqlite3_open(dbfile, &db);
	if( rc )
		{
		MBOXP("Cannot open SQL %s", dbfile);
		sqlite3_close(db);
		return 0;
		}

	for(int iii=0; iii < 19; iii++)
		{
		*arr[iii] = EscapeSql(*arr[iii]);
		}

	CString tmp; tmp.Format("%d", id);
	CString qqq("update address set ");

	qqq +=  "lname = '" +   m_lname +  "', ";
	qqq +=  "email = '" +   m_email +  "', ";
	qqq +=  "prefix = '" +  m_prefix +  "', ";
	qqq +=  "qnote = '" +   m_qnote +  "', ";
	qqq +=  "office = '" +  m_office +  "', ";
	qqq +=  "address = '" + m_address +  "', ";
	qqq +=  "city = '" +    m_city +  "', ";
	qqq +=  "zip = '" +     m_zip +  "', ";
	qqq +=  "tel = '" +     m_tel +  "', ";
	qqq +=  "fax  = '" +    m_fax +  "', ";
	qqq +=  "faxext = '" +  m_faxext +  "', ";
	qqq +=  "note = '" +    m_note +  "', ";
	qqq +=  "ext = '" +     m_ext +  "', ";
	qqq +=  "post = '" +    m_post +  "', ";
	qqq +=  "state = '" +   m_state +  "', ";
	qqq +=  "fname = '" +   m_fname +  "', ";
	qqq +=  "email2 = '" +  m_email2 +  "', ";
	qqq +=  "cell  = '" +   m_cell +  "', ";
	qqq +=  "cext  = '" +   m_cext +  "'";

	qqq += " where id = " + tmp;

	//MBOXP("SQL: '%s'", qqq);

	rc = sqlite3_exec(db, qqq, callback, 0, &zErrMsg);

	if( rc != SQLITE_OK )
		{
		MBOXP("SQL error on '%s' --> %s", qqq, sqlite3_errmsg(db));
		}
	sqlite3_close(db);

	return 1;
}

//////////////////////////////////////////////////////////////////////////

void CAddrEntry::OnCancel()

{
	HTREEITEM hItem  = m_tree.GetSelectedItem();
	if(hItem)
		{
		UpdateDB(m_tree.GetItemData(hItem));
		}
	CDialog::OnCancel();
}

//////////////////////////////////////////////////////////////////////////

void CAddrEntry::OnDblclkTree1(NMHDR* pNMHDR, LRESULT* pResult)

{
	OnOK();
	*pResult = 0;
}

//////////////////////////////////////////////////////////////////////////

CString unquote(CString &str)

{
	int inq = false, len = str.GetLength(), loop;
	CString res;

	for(loop = 0; loop < len; loop++)
		{
		char chh = str.GetAt(loop);

		// Toggle in quote flag
		switch(chh)
			{
			case '\"':
				inq = !inq;
				break;

			case ',':
				if(!inq)
					res += chh;
				break;

			default:
				res += chh;
			}
		}
	return(res);
}

//////////////////////////////////////////////////////////////////////////

void	CAddrEntry::ParseCSV(CString &str)

{
	CString field1, field2, field3;

	if(str == "")
		return;

	str = EscapeSql(str);
	str = unquote(str);

	//MBOXP("Whole: %s", str);

	CPtrArray  parr;

	ParseCSVStr(str, &parr);

#if 0
	int loop;
	int size = parr.GetSize();
	for(loop = 0; loop < size; loop++)
		{
		CString *tmp = (CString *)parr.GetAt(loop);
		PrintToNotepad("csv %s\r\n", *tmp);
		}
#endif

	CString  *name;

	if(parr.GetSize() > 0)
		name = (CString *)parr.GetAt(0);

	CString ll, rr;
	int idx = name->Find(' ');

	if(idx >= 0)
		{
		ll = name->Left(idx);
		rr = name->Mid(idx + 1);
		}
	else
		ll = *name;

	CString tmp2; tmp2.Format("Importing: %s", *name);
	SetWindowText(tmp2);

	//P2N("Name '%s' ll=%s rr=%s\r\n", *name, ll, rr);
	// -------------------------------------------------------------------

	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	rc = sqlite3_open(dbfile, &db);

	if( rc )
		{
		MBOX("cannot open SQL");
		sqlite3_close(db);
		}

	lastclass = this;
	str = "";	sumstr = "";

	rows = 0;

	CString qqq("insert into address (lname, fname, email) values('");

	if(parr.GetSize() > 0)

	qqq +=  rr + "', '";
	qqq +=  ll + "', '";

	if(parr.GetSize() > 1)
		qqq +=  *((CString *)parr.GetAt(1)) + "')";

	rc = sqlite3_exec(db, qqq, callback2, 0, &zErrMsg);

	if( rc != SQLITE_OK )
		{
		MBOXP("SQL error: %s", sqlite3_errmsg(db));
		return;
		}

	int rowid = (int)sqlite3_last_insert_rowid(db);
	sqlite3_close(db);

	FreeParseStr(&parr);
}

//////////////////////////////////////////////////////////////////////////
// Netscape parsing

void	CAddrEntry::ParseCSV_NS(CString &str)

{
	CString field1, field2, field3;

	if(str == "")
		return;

	str = EscapeSql(str);
	str = unquote(str);

	//MBOXP("Whole: %s", str);

	CPtrArray  parr;

	ParseCSVStr(str, &parr);


#if 0
	int loop;
	int size = parr.GetSize();

	for(loop = 0; loop < size; loop++)
		{
		CString *tmp = (CString *)parr.GetAt(loop);
		PrintToNotepad("netsc csv %s\r\n", *tmp);
		}
#endif

	CString  *name;

	if(parr.GetSize() > 2)
		{
		name = (CString *)parr.GetAt(2);
		}

	// Substitute email address
	if(*name == "")
		{
		*name = *((CString *)parr.GetAt(4));
		}


	CString ll, rr;
	int idx = name->Find(' ');

	if(idx >= 0)
		{
		ll = name->Left(idx);
		rr = name->Mid(idx + 1);
		}
	else
		ll = *name;

	CString tmp2; tmp2.Format("Importing: %s", *name);
	SetWindowText(tmp2);

	//P2N("Name '%s' ll=%s rr=%s\r\n", *name, ll, rr);
	// -------------------------------------------------------------------

	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	rc = sqlite3_open(dbfile, &db);

	if( rc )
		{
		MBOX("cannot open SQL");
		sqlite3_close(db);
		}

	lastclass = this;
	str = "";	sumstr = "";

	rows = 0;

	CString qqq("insert into address (fname, lname, email) values('");

	if(parr.GetSize() > 2)
	{
	qqq +=  rr + "', '";
	qqq +=  ll + "', '";
	}

	if(parr.GetSize() > 3)
		qqq +=  *((CString *)parr.GetAt(4)) + "')";

	rc = sqlite3_exec(db, qqq, callback2, 0, &zErrMsg);

	if( rc != SQLITE_OK )
		{
		MBOXP("SQL error: %s", sqlite3_errmsg(db));
		return;
		}

	int rowid = (int)sqlite3_last_insert_rowid(db);
	sqlite3_close(db);

	FreeParseStr(&parr);
}


//////////////////////////////////////////////////////////////////////////

void	CAddrEntry::ParseDumpCSV(CString &str)

{
	CString field1, field2, field3;

	if(str == "")
		return;

	str = EscapeSql(str);

	CPtrArray  parr;

	ParseCSVStr(str, &parr);

	int loop;
	int size = parr.GetSize();

	//for(loop = 0; loop < size; loop++)
	//	{
	//	CString *tmp = (CString *)parr.GetAt(loop);
	//	PrintToNotepad("csv %s\r\n", *tmp);
	//	}

	// -------------------------------------------------------------------

	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	rc = sqlite3_open(dbfile, &db);

	if( rc )
		{
		MBOX("cannot open SQL");
		sqlite3_close(db);
		}

	lastclass = this;
	str = "";	sumstr = "";

	rows = 0;

	CString qqq("insert into address ("
		"lname, fname, email, prefix, post, email2, qnote, office, address, city, "
				"zip, state, tel, ext, fax, faxext, cell, cext, note )"
				" values('" );

	for(loop = 0; loop < size - 2; loop++)
		{
		CString *tmp = (CString *)parr.GetAt(loop);
		qqq +=  *tmp + "', '";
		}

	CString *tmp2 = (CString *)parr.GetAt(size - 2);
	qqq +=	*tmp2 + "')";

	//MBOX(qqq);

	rc = sqlite3_exec(db, qqq, callback3, 0, &zErrMsg);

	if( rc != SQLITE_OK )
		{
		MBOXP("SQL error: %s", sqlite3_errmsg(db));
		return;
		}

	int rowid = (int)sqlite3_last_insert_rowid(db);
	sqlite3_close(db);

	FreeParseStr(&parr);
}

//////////////////////////////////////////////////////////////////////////
// Import

void CAddrEntry::OnButton6()

{
	HTREEITEM hItem  = m_tree.GetSelectedItem();
	if(hItem)
		{
		UpdateDB(m_tree.GetItemData(hItem));
		}

	CFileDialog dlgFile(true);

	CString title = "Import Address Book";
	CString fileName, strFilter;

	strFilter += "Comma Separated Values (*.csv)";
	strFilter += (TCHAR)NULL;
	strFilter += "*.csv";
	strFilter += (TCHAR)NULL;

	strFilter += "Text file (*.txt)";
	strFilter += (TCHAR)NULL;
	strFilter += "*.txt";
	strFilter += (TCHAR)NULL;

	strFilter += "All Files (*.*)";
	strFilter += (TCHAR)NULL;
	strFilter += "*.*";
	strFilter += (TCHAR)NULL;
	strFilter += (TCHAR)NULL;

	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;
	dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);

	dlgFile.m_ofn.nFilterIndex = 0;

	dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);
	dlgFile.m_ofn.lpstrInitialDir = str;

	BOOL bRet = (dlgFile.DoModal() == IDOK) ? TRUE : FALSE;
	fileName.ReleaseBuffer();

	if(!bRet)
		return;

	CFileException fileException;
	CFile xcpt;

	if (!xcpt.Open(fileName, CFile::modeRead, &fileException))
		{
		MBOXP("Cannot open import file %s", fileName);
		//P2N("Cannot open import file\r\n", fileName);
		return;
		}

	CString str;
	char chh;
	while(true)
		{
		if(!xcpt.Read(&chh, 1))
			{
			ParseDumpCSV(str);
			break;
			}

		if(chh != '\n' && chh != '\r')
			str += chh;

		if(chh == '\r')
			{
			ParseDumpCSV(str);
			str = "";
			}
		}

	xcpt.Close();

	Fill();

	//SelEntryInt(rowid);
	//GetDlgItem(IDC_EDIT1)->SetFocus();
	//((CEdit *)GetDlgItem(IDC_EDIT1))->SetSel(0, -1);
	//UpdateData(false);
}

//////////////////////////////////////////////////////////////////////////
// Export to data
//

void CAddrEntry::OnButton7()

{
	HTREEITEM hItem  = m_tree.GetSelectedItem();
	if(hItem)
		{
		UpdateDB(m_tree.GetItemData(hItem));
		}

	CFileDialog dlgFile(false);

	CString title = "Export Address Book";
	CString fileName, strFilter;

	strFilter += "Comma Separated Values (*.csv)";
	strFilter += (TCHAR)NULL;
	strFilter += "*.csv";
	strFilter += (TCHAR)NULL;

	strFilter += "Text file (*.txt)";
	strFilter += (TCHAR)NULL;
	strFilter += "*.txt";
	strFilter += (TCHAR)NULL;

	strFilter += "All Files (*.*)";
	strFilter += (TCHAR)NULL;
	strFilter += "*.*";
	strFilter += (TCHAR)NULL;

	strFilter += (TCHAR)NULL;

	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;
	dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);

	dlgFile.m_ofn.nFilterIndex = 0;

	dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);
	dlgFile.m_ofn.lpstrInitialDir = str;

	BOOL bRet = (dlgFile.DoModal() == IDOK) ? TRUE : FALSE;
	fileName.ReleaseBuffer();

	if(!bRet)
		return;

	if(dlgFile.GetFileExt() == "")
		{
		fileName += ".csv";
		}

	CFileException fileException;

	if (!filehand.Open(fileName,
				CFile::modeCreate | CFile::modeReadWrite, &fileException))
		{
		MBOXP("Cannot create  export file %s", fileName);
		//P2N("Cannot create export file\r\n", fileName);
		return;
		}

	lastclass = this;

	// Query the database for all
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	first = false;

	rc = sqlite3_open(dbfile, &db);

	if( rc )
		{
		MBOXP("SQL error: %s", sqlite3_errmsg(db));
		sqlite3_close(db);
		}

	rc = sqlite3_exec(db, "select "
		"lname, fname, email, prefix, post, email2, qnote, office, address, city, "
				"zip, state, tel, ext, fax, faxext, cell, cext, note "
				" from address", callback4, 0, &zErrMsg);
	if( rc != SQLITE_OK )
		{
		MBOXP("SQL error: %s", sqlite3_errmsg(db));
		}

	if( rc != SQLITE_OK )
		{
		MBOXP("SQL error: %s", sqlite3_errmsg(db));
		}
	sqlite3_close(db);

	filehand.Close();
}

//////////////////////////////////////////////////////////////////////////
// Import from outlook

void CAddrEntry::OnButton8()

{
	HTREEITEM hItem  = m_tree.GetSelectedItem();
	if(hItem)
		{
		UpdateDB(m_tree.GetItemData(hItem));
		}

	CFileDialog dlgFile(true);

	CString title = "Import Address Book";
	CString fileName, strFilter;

	strFilter += "Comma Separated Values (*.csv)";
	strFilter += (TCHAR)NULL;
	strFilter += "*.csv";
	strFilter += (TCHAR)NULL;

	strFilter += "Text file (*.txt)";
	strFilter += (TCHAR)NULL;
	strFilter += "*.txt";
	strFilter += (TCHAR)NULL;

	strFilter += "All Files (*.*)";
	strFilter += (TCHAR)NULL;
	strFilter += "*.*";
	strFilter += (TCHAR)NULL;
	strFilter += (TCHAR)NULL;

	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;
	dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);

	dlgFile.m_ofn.nFilterIndex = 0;

	dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);
	dlgFile.m_ofn.lpstrInitialDir = str;

	BOOL bRet = (dlgFile.DoModal() == IDOK) ? TRUE : FALSE;
	fileName.ReleaseBuffer();

	if(!bRet)
		return;

	CFileException fileException;
	CFile xcpt;

	BeginWaitCursor();

	CString title2;
	GetWindowText(title2);

	if (!xcpt.Open(fileName, CFile::modeRead, &fileException))
		{
		MBOXP("Cannot open import file %s", fileName);
		//P2N("Cannot open import file\r\n", fileName);
		return;
		}

	CString str;
	char chh;
	while(true)
		{
		if(!xcpt.Read(&chh, 1))
			{
			ParseCSV(str);
			break;
			}
		if(chh != '\n' && chh != '\r')
			str += chh;

		if(chh == '\r')
			{
			ParseCSV(str);
			str = "";
			}
		}

	xcpt.Close();

	SetWindowText(title2);
	EndWaitCursor();

	Fill();
}


//////////////////////////////////////////////////////////////////////////

void CAddrEntry::OnButton9()

{
	HTREEITEM hItem  = m_tree.GetSelectedItem();
	if(hItem)
		{
		UpdateDB(m_tree.GetItemData(hItem));
		}

	CString tmpbox;
	tmpbox.Format("This will delete duplicate records. Continue?");

	if(xMsgBox(tmpbox, "", MB_YESNO) != IDYES)
		return ;

	lastclass = this;

	CString title;
	GetWindowText(title);

	BeginWaitCursor();

	sqlite3 *db;
	char *zErrMsg = 0;
	int rc, loop;

	rc = sqlite3_open(dbfile, &db);

	if( rc )
		{
		MBOXP("SQL error: %s", sqlite3_errmsg(db));
		sqlite3_close(db);
		}

	old_lname = "";  old_fname = ""; old_email = "";
	dwarr.RemoveAll();

	rc = sqlite3_exec(db, "select lname, fname, email, id from address order by lname, fname, email",
								callback5, 0, &zErrMsg);

	if( rc != SQLITE_OK )
		{
		MBOXP("SQL error: %s", sqlite3_errmsg(db));
		}

	int size = dwarr.GetSize();

	for(loop = 0; loop < size; loop++)
		{
		CString qqq;
		qqq = "delete from address where id = ";

		CString tmp;
		tmp.Format("%d", dwarr.GetAt(loop));
		qqq +=  tmp;
		//MBOX(qqq);

		CString tmp2; tmp2.Format("Removing duplicate: %d", dwarr.GetAt(loop));
		SetWindowText(tmp2);

		rc = sqlite3_exec(db, qqq, callback2, 0, &zErrMsg);

		if( rc != SQLITE_OK )
			{
			MBOXP("SQL error: %s", sqlite3_errmsg(db));
			}
		}
	dwarr.RemoveAll();
	sqlite3_close(db);

	SetWindowText(title);
	EndWaitCursor();

	Fill();
}

//////////////////////////////////////////////////////////////////////////

void CAddrEntry::SelLast()

{
	// Select Last entry
	HTREEITEM old_item = 0;
	HTREEITEM item = m_tree.GetNextItem( m_tree.GetSelectedItem(), TVGN_ROOT );
	while(true)
		{
		if(!item)
			break;

		old_item = item;
		item = m_tree.GetNextItem(item, TVGN_NEXT );
		}

	if(old_item)
		{
		m_tree.SelectItem(old_item);
		m_tree.Invalidate();
		support.YieldToWinEx();
		}
}

//////////////////////////////////////////////////////////////////////////

void CAddrEntry::OnSetfocusTree1(NMHDR* pNMHDR, LRESULT* pResult)

{
	if(rescan)
		{
		int idd;

		HTREEITEM hItem  = m_tree.GetSelectedItem();

		if(hItem)
			idd = m_tree.GetItemData(hItem);

		Fill();

		// Re-select same one
		if(hItem)
			SelEntryInt(idd);
		}

	*pResult = 0;
}

//////////////////////////////////////////////////////////////////////////

void CAddrEntry::OnButton37()

{

	HTREEITEM hItem  = m_tree.GetSelectedItem();
	if(hItem)
		{
		UpdateDB(m_tree.GetItemData(hItem));
		}

	CFileDialog dlgFile(true);

	CString title = "Import Netscape Address Book";
	CString fileName, strFilter;

	strFilter += "Comma Separated Values (*.csv)";
	strFilter += (TCHAR)NULL;
	strFilter += "*.csv";
	strFilter += (TCHAR)NULL;

	strFilter += "Text file (*.txt)";
	strFilter += (TCHAR)NULL;
	strFilter += "*.txt";
	strFilter += (TCHAR)NULL;

	strFilter += "All Files (*.*)";
	strFilter += (TCHAR)NULL;
	strFilter += "*.*";
	strFilter += (TCHAR)NULL;
	strFilter += (TCHAR)NULL;

	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;
	dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);

	dlgFile.m_ofn.nFilterIndex = 0;

	dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);
	dlgFile.m_ofn.lpstrInitialDir = str;

	BOOL bRet = (dlgFile.DoModal() == IDOK) ? TRUE : FALSE;
	fileName.ReleaseBuffer();

	if(!bRet)
		return;

	CFileException fileException;
	CFile xcpt;

	BeginWaitCursor();

	CString title2;
	GetWindowText(title2);

	if (!xcpt.Open(fileName, CFile::modeRead, &fileException))
		{
		MBOXP("Cannot open import file %s", fileName);
		//P2N("Cannot open import file\r\n", fileName);
		return;
		}

	CString str;
	char chh;
	while(true)
		{
		if(!xcpt.Read(&chh, 1))
			{
			ParseCSV_NS(str);
			break;
			}
		if(chh != '\n' && chh != '\r')
			str += chh;

		if(chh == '\r')
			{
			ParseCSV_NS(str);
			str = "";
			}
		}

	xcpt.Close();

	SetWindowText(title2);
	EndWaitCursor();

	Fill();
}

// EOF


