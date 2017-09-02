
/* =====[ pcSelection.cpp ]========================================== 
                                                                             
   Description:     The dbtest project, implementation of the pcSelection.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  5/14/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// pcSelection.cpp : implementation file

#include "stdafx.h"
#include "xraynotes.h"
#include "mxpad.h"
#include "support.h"
#include "sqlite3.h"
#include "CodeEd.h"
#include "pcSelection.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static int callback_load(void *NotUsed, int argc, char **argv, char **azColName)

{
	CpcSelection *lastclass = (CpcSelection *)NotUsed;

	//P2N("callback_load %s '%s'\r\n", argv[0], argv[1]);
	lastclass->numcol++;
	if(argv[1])
		{
		lastclass->m_recent.AddString(argv[1]);
		}
	return 0;
}


static int callback_select(void *NotUsed, int argc, char **argv, char **azColName)

{
	CpcSelection *lastclass = (CpcSelection *)NotUsed;

	//P2N("callback_select count=%s '%s'\r\n", argv[0], argv[1]);
	
	lastclass->numcol++;
	lastclass->zcount = atoi(argv[0]);

	return 0;
}

static int callback_fill(void *NotUsed, int argc, char **argv, char **azColName)

{
	CpcSelection *lastclass = (CpcSelection *)NotUsed;

	//P2N("callback_fill item: %d %s %s\r\n", argv[0], argv[1], argv[2]);

	CString str; str.Format("%-10s %s", argv[1], argv[2]);
	
	// Clistbox
	DWORD id = atoi(argv[0]);
	int ent = lastclass->m_list.AddString(str);
	if(ent >= 0)
		{
		lastclass->m_list.SetItemData(ent, id);
		}

	return 0;
}

static int callback_longdesc(void *NotUsed, int argc, char **argv, char **azColName)

{
	CpcSelection *lastclass = (CpcSelection *)NotUsed;

	//P2N("callback_longdesc\r\n");
	//P2N("Got item: %d %s %s\r\n", argv[0], argv[1], argv[2]);
	
	// Clistbox
	lastclass->m_desc  = argv[1];
	lastclass->m_notes = argv[2];

	lastclass->UpdateData(false);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CpcSelection dialog


CpcSelection::CpcSelection(CWnd* pParent /*=NULL*/)
	: CDialog(CpcSelection::IDD, pParent)
{
	//{{AFX_DATA_INIT(CpcSelection)
	m_filter = _T("");
	m_desc = _T("");
	m_notes = _T("");
	//}}AFX_DATA_INIT

	maxfill = 50;
	maxsize = 50;

	// Just in case no one is setting this
	table	= "proc_codes";  
	title	= "Procedural Code Picker";
}


void CpcSelection::DoDataExchange(CDataExchange* pDX)

{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CpcSelection)
	DDX_Control(pDX, IDC_EDIT_CS_3, m_enotes);
	DDX_Control(pDX, IDC_EDIT_CS_2, m_edesc);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_can);
	DDX_Control(pDX, IDC_BUTTON_CS_2, m_b2);
	DDX_Control(pDX, IDC_BUTTON_CS_3, m_b3);
	DDX_Control(pDX, IDC_BUTTON_CS_4, m_b4);
	//DDX_Control(pDX, IDC_BUTTON_CS_5, m_b5);
	//DDX_Control(pDX, IDC_BUTTON_CS_6, m_b6);
	//DDX_Control(pDX, IDC_BUTTON_CS_7, m_b7);
	//DDX_Control(pDX, IDC_BUTTON_CS_8, m_b8);
	DDX_Control(pDX, IDC_LIST_CS_2, m_recent);
	DDX_Control(pDX, IDC_LIST_CS_1, m_list);
	DDX_Text(pDX, IDC_EDIT_CS_1, m_filter);
	DDX_Text(pDX, IDC_EDIT_CS_2, m_desc);
	DDX_Text(pDX, IDC_EDIT_CS_3, m_notes);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CpcSelection, CDialog)
	//{{AFX_MSG_MAP(CpcSelection)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_EDIT_CS_1, OnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON_CS_2, OnButton2)
	ON_LBN_DBLCLK(IDC_LIST_CS_1, OnDblclkList1)
	ON_LBN_DBLCLK(IDC_LIST_CS_2, OnDblclkList2)
	ON_LBN_SELCHANGE(IDC_LIST_CS_1, OnSelchangeList1)
	ON_BN_CLICKED(IDC_BUTTON_CS_4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON_CS_3, OnButtonCs3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CpcSelection message handlers

BOOL CpcSelection::OnInitDialog() 

{
	CDialog::OnInitDialog();
	
	dbfile =	dataroot;
	dbfile += "db\\codes.sqlt";

	dbfile2 =	dataroot;
	dbfile2 += "db\\recentcodes.sqlt";

	SetWindowText(title);

	m_edesc.Disable();
	m_enotes.Disable();

	LoadRecent();

	SetTimer(1, 100, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CpcSelection::OnTimer(UINT nIDEvent) 

{
	if(nIDEvent == 1)
		{
		KillTimer(nIDEvent);
		Fill();
		}
	else if (nIDEvent == 2)
		{
		KillTimer(nIDEvent);

		CString str, code;
		UpdateData();

		int sel = m_list.GetCurSel();
		m_list.GetText(sel, str);

		if(str == "")
			return;

		int idx = str.Find(' ');

		if(idx >= 0)
			code = str.Mid(0, idx);
		
		if(code == "")
			return;

		//P2N("code='%s' Filling '%s'\r\n", code, str);

		//BeginWaitCursor();

		// Fill in description
		sqlite3 *db;
		char *zErrMsg = 0;
		int rc;

		//P2N("CpcSelection::OnTimer opening %s\r\n", dbfile);

		rc = sqlite3_open(dbfile, &db);

		if( rc )
			{
			MBOXP("SQL error: %s", sqlite3_errmsg(db));
			sqlite3_close(db);
			sqlite3_free(zErrMsg);
			return;
			}

		CString qqq;
		qqq.Format("select code, longdesc, note from %s where code = '%s'", table, code);

		//P2N("qqq=%s\r\n", qqq);

		rc = sqlite3_exec(db, qqq, callback_longdesc, this, &zErrMsg);
		if( rc != SQLITE_OK )
			{
			MBOXP("SQL error: %s", sqlite3_errmsg(db));
			sqlite3_free(zErrMsg);
			}
		sqlite3_close(db);
		//EndWaitCursor();
		}


	CDialog::OnTimer(nIDEvent);
}


void CpcSelection::Fill()

{
	m_list.ResetContent();

	UpdateData();
	//P2N("Filling '%s'\r\n", str);

	BeginWaitCursor();

	// Fill in customer name tree
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	rc = sqlite3_open(dbfile, &db);

	if( rc )
		{
		MBOXP("SQL error: %s", sqlite3_errmsg(db));
		sqlite3_close(db);
		sqlite3_free(zErrMsg);
		return;
		}

	CString qqq;
	qqq.Format("select id, code, desc from %s where (desc like '%%%s%%') or (code like '%%%s%%') limit %d", 
					table, m_filter, m_filter, maxfill);

	//P2N("qqq=%s\r\n", qqq);

	rc = sqlite3_exec(db, qqq, callback_fill, this, &zErrMsg);
	if( rc != SQLITE_OK )
		{
		MBOXP("SQL error: %s", sqlite3_errmsg(db));
		sqlite3_free(zErrMsg);
		}

	sqlite3_close(db);
	EndWaitCursor();
}

void CpcSelection::OnChangeEdit1() 

{
	maxfill = 50;
	SetTimer(1, 1000, NULL);
}

void CpcSelection::OnButton2() 

{
	maxfill = 500;
	SetTimer(1, 100, NULL);
}

void CpcSelection::OnDblclkList1() 

{
	int sel = m_list.GetCurSel();

	if(sel >= 0)
		{
		m_list.GetText(sel, result);
		OnOK();
		}
}

void CpcSelection::OnOK() 

{	
	// Anything Selected?

	if(m_list.GetCurSel() >= 0)
		{
		m_list.GetText(m_list.GetCurSel(), result);
		}
	else if(m_recent.GetCurSel() >= 0)
		{
		m_recent.GetText(m_recent.GetCurSel(), result);	
		}
	else
		{
		MBOX("Please Select an Item or use the 'Cancel' button");
		return;
		}

	SaveRecent();

	// Parse CPC
	int idx = result.FindOneOf(" \t\r\n");
	if(idx >= 0)
		diagcode  = result.Left(idx);
	else
		diagcode  = result.Left(5);

	diagcode.TrimLeft();

	if(idx >= 0)
		description = result.Mid(idx + 1);
	else
		description = result.Mid(8);

	description.TrimLeft();

	CDialog::OnOK();
}

void CpcSelection::OnDblclkList2() 

{
	m_recent.GetText(m_recent.GetCurSel(), result);
	OnOK();
}

void CpcSelection::OnSelchangeList1() 

{
	CString str, code;
	SetTimer(2, 300, NULL);
}

void CpcSelection::OnButton4() 

{
	int loop, size = m_recent.GetCount();
	size = min(50, size);
	m_recent.ResetContent();
	
	for(loop = 0; loop < size; loop++)
		{
		CString str;
		
		CString key; key.Format("%d", loop);
		AfxGetApp()->WriteProfileString(table, key, "");
		//P2N("Combo = %s\r\n", str);
		}
}



void CpcSelection::OnButtonCs3() 

{
	int curr = m_list.GetCurSel();
	if(curr < 0)
		{
		MBOX("Please select an entry to Edit.");
		return;
		}

	CCodeEd ced;

	ced.table = table;
	ced.id = m_list.GetItemData(curr);
	
	//P2N("Editing entry %d\r\n", ced.id);

	ced.DoModal();

	SetTimer(2, 300, NULL);
}

void CpcSelection::SaveRecent()

{
	//P2N("Save Recent %s\r\n", result);

	// New: maintain proper usage count
	
	CString result2 = support.EscapeSql(result);

	// See if we have an entry for this one
	CString qqq;
	qqq.Format("select frequency, code from %s where code = '%s'", table, result2);

	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	//////////////////////////////////////////////////////////////////////
	// Save recent

	numcol = 0;

	rc = sqlite3_open(dbfile2, &db);
	if( rc )
		{
		MBOXP("SQL error: %s %s", sqlite3_errmsg(db), dbfile2);
		sqlite3_close(db);
		sqlite3_free(zErrMsg);
		return;
		}

	//P2N("CpcSelection::SaveRecent [%s]\r\n", qqq);

	rc = sqlite3_exec(db, qqq, callback_select, this, &zErrMsg);

	if( rc != SQLITE_OK )
		{
		P2N("Error on SQL [%s]\r\n", qqq);
		MBOXP("SQL error on database %s - %s", dbfile2, sqlite3_errmsg(db));
		sqlite3_free(zErrMsg);
		}

	//P2N("NUMCOL %d\r\n", numcol);

	if(numcol == 0)
		{
		qqq.Format("insert into %s (frequency, code) values (%d, '%s')", 
				table, 1, result2);
		}
	else
		{
		qqq.Format("update %s set frequency = %d where code = '%s'", 
				table, zcount+1, support.EscapeSql(result));
		}

	rc = sqlite3_exec(db, qqq, callback_select, this, &zErrMsg);

	if( rc != SQLITE_OK )
		{
		P2N("Error on SQL [%s]\r\n", qqq);
		MBOXP("SQL error on database %s - %s", dbfile2, sqlite3_errmsg(db));
		sqlite3_free(zErrMsg);
		}
	
	sqlite3_close(db);
}


void CpcSelection::LoadRecent()

{
	// Load list
	CString qqq;
	qqq.Format("select frequency, code from %s order by frequency desc limit %d", table, maxsize);

	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	//////////////////////////////////////////////////////////////////////
	// Save recent

	numcol = 0;

	rc = sqlite3_open(dbfile2, &db);
	if( rc )
		{
		MBOXP("SQL error: %s %s", sqlite3_errmsg(db), dbfile2);

		sqlite3_close(db);
		sqlite3_free(zErrMsg);
		return;
		}

	//P2N("CpcSelection::LoadRecent [%s]\r\n", qqq);

	rc = sqlite3_exec(db, qqq, callback_load, this, &zErrMsg);
	
	if( rc != SQLITE_OK )
		{
		CString errmsg = zErrMsg;
		errmsg.MakeLower();
	
		if(errmsg.Find("no such table") >= 0)
			{
			//P2N("Creating new table %s\r\n", table);

			// Attempt to create it
			CreateDBfile(dbfile2, table);
			rc = sqlite3_exec(db, qqq, callback_load, this, &zErrMsg);
			}
		}

	if( rc != SQLITE_OK )
		{
		P2N("Error %d (%s) on SQL [%s]\r\n", rc, zErrMsg, qqq);
		MBOXP("SQL error on database %s - %s", dbfile2, sqlite3_errmsg(db));
		sqlite3_free(zErrMsg);
		}

	sqlite3_close(db);
}

//////////////////////////////////////////////////////////////////////////

void CpcSelection::CreateDBfile(const char *str, const char *newtable)

{
	CString qqq;
	qqq.Format("create table %s "
		"(id integer primary key autoincrement, frequency integer, code text) ", newtable);

	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	//////////////////////////////////////////////////////////////////////
	// Create recent

	rc = sqlite3_open(str, &db);
	if( rc )
		{
		MBOXP("SQL error: %s %s", sqlite3_errmsg(db), dbfile);
		sqlite3_close(db);
		sqlite3_free(zErrMsg);
		return;
		}

	//P2N("CpcSelection::CreateDBfile [%s]\r\n", qqq);

	rc = sqlite3_exec(db, qqq, NULL, this, &zErrMsg);

	if( rc != SQLITE_OK )
		{
		P2N("Error on SQL [%s]\r\n", qqq);
		MBOXP("SQL error on database %s - %s", str, sqlite3_errmsg(db));
		sqlite3_free(zErrMsg);
		}
	sqlite3_close(db);
}
