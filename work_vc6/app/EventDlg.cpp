
/* =====[ EventDlg.cpp ]========================================== 
                                                                             
   Description:     The dbtest project, implementation of the EventDlg.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  1/9/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// EventDlg.cpp : implementation file
//

#include "stdafx.h"
#include <direct.h>
#include "xraynotes.h"

#include "PRocList.h"
#include "EventDlg.h"
#include "support.h"
#include "mxpad.h"
#include "DateSel.h"
#include "uniqid.h"
#include "sqlite3.h"
#include "parse.h"
#include "DiagList.h"
#include "prl.h"
#include "pcSelection.h"
#include "Transcript.h"
#include "calen.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString EventPleaseSel = "Please select an event entry first.";
CString EventNoItem = "No event item is currently active, please select one.";


static int callback_table(void *NotUsed, int argc, char **argv, char **azColName)

{
	CEventDlg *lc = (CEventDlg *)NotUsed;

#if 0
	P2N("callback_table:\r\n");
	for(int ii = 0;  ii < argc; ii++)
		P2N("%s=%s  ", azColName[ii], argv[ii] ? argv[ii] : "NULL");
	P2N("\r\n");
#endif

	// Iterate array for match
	for(int iii = 0; iii <  lc->farrsize; iii++)
		{
		if(lc->farr[iii].name.CompareNoCase(argv[1]) == 0)
			{
			//P2N("checked off %s\r\n", argv[1]);
			lc->farr[iii].checked = true;
			}
		}

	return 0;
}


static int callback_noop(void *NotUsed, int argc, char **argv, char **azColName)

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


static int callback_cal(void *NotUsed, int argc, char **argv, char **azColName)

{
	int *cnt = (int *)NotUsed;

	P2N("callback_cal: Got cal item item id: %s \r\n", argv[0]);

	*cnt += 1;

	return 0;
}

static int callback_fill(void *NotUsed, int argc, char **argv, char **azColName)

{
	CString str;
	CEventDlg *lastclass = (CEventDlg *)NotUsed;


	ASSERT(argc == 2);
	ASSERT(lastclass->magic == EVENT_MAGIC);

	str = argv[0];	

	//P2N("callback_fill %x %s %s\r\n", lastclass->magic, str, argv[1]);

	HTREEITEM item = lastclass->m_tree.InsertItem(str, 1, 0);

	DWORD id = atoi(argv[1]);
	lastclass->m_tree.SetItemData(item, id);

	//P2N("Got item id: %s %d\r\n", argv[2], id);
	return 0;
}

//////////////////////////////////////////////////////////////////////////

static	int		callback_select(void *NotUsed, int argc, char **argv, char **azColName)

{
	//CString  tmp, ttmp;

	CEventDlg *lastclass = (CEventDlg *)NotUsed;

	ASSERT(lastclass->magic == EVENT_MAGIC);

	//P2N("callback_select\r\n");
	
	lastclass->rows ++;
	
	for(int ii = 0; ii < argc; ii++)
		{
		//ttmp.Format("%s=%s  ",  azColName[ii], argv[ii] ? argv[ii] : "NULL");

		if(argv[ii])
			{
			// Only assign if there is any vars
			if(lastclass->farr[ii].var)
				*lastclass->farr[ii].var   = argv[ii];

			lastclass->farr[ii].value  = argv[ii];
			}
		else
			{
			lastclass->farr[ii].value = "";
			}

		//tmp += ttmp;
		}

	//tmp += "\r\n\r\n";

	//lastclass->sumstr += tmp;

	lastclass->UpdateData(false);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CEventDlg dialog


CEventDlg::CEventDlg(CWnd* pParent /*=NULL*/)
	: CGrayDlg(CEventDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEventDlg)
	m_name = _T("");
	m_tid = _T("");
	m_patid = _T("");
	m_diagnosis = _T("");
	m_proc = _T("");
	m_symptom = _T("");
	m_diagcode = _T("");
	m_pcode1 = _T("");
	m_pc1desc = _T("");
	m_dcdesc = _T("");
	m_bdate = _T("");
	m_edate = _T("");
	m_pcode3 = _T("");
	m_ndate = _T("");
	m_note = _T("");
	m_prl = _T("");
	m_chdate = _T("");
	m_appoint = _T("");
	m_mdate = _T("");
	m_id = _T("");
	m_filter = _T("");
	m_phys = _T("");
	m_phys2 = _T("");
	m_trans = _T("");
	m_res = _T("");
	//}}AFX_DATA_INIT
	

	//title_changed = false;
	rows = backup = farrsize = 0;
	filling = rescan = false;

	// Specific to this database and dialog
	dbtable = "events";
	thisname = "EventdDlg";
	magic = EVENT_MAGIC;

	// Data files to use:
	dbfile		= "events.sqlt";
	dbfile2		= "codes.sqlt";
	
	support.GetDataRoot(calfile);
	calfile		+= "calendar.sqlt";
	
	windowname = "CEventDlg";
	SetAlpha(245);
	m_centerdlg = true;
	init_array();
}


void CEventDlg::DoDataExchange(CDataExchange* pDX)
{
	CGrayDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEventDlg)
	DDX_Control(pDX, IDC_EDIT_ED_55, m_eid);
	DDX_Control(pDX, IDC_EDIT_ED_5, m_ed5);
	DDX_Control(pDX, IDC_EDIT_ED_4, m_ed4);
	DDX_Control(pDX, IDC_EDIT_ED_1, m_ed1);
	DDX_Control(pDX, IDC_EDIT_ED_3, m_ed3);
	DDX_Control(pDX, IDC_EDIT_ED_54, m_emod);
	DDX_Control(pDX, IDC_EDIT_ED_53, m_endate);
	DDX_Control(pDX, IDC_EDIT_ED_41, m_tide);
	DDX_Control(pDX, IDC_EDIT_ED_42, m_prle);
	DDX_Control(pDX, IDC_EDIT_ED_38, m_epatid);
	DDX_Control(pDX, IDC_TREE_ED_1, m_tree);
	DDX_Text(pDX, IDC_EDIT_ED_2, m_name);
	DDX_Text(pDX, IDC_EDIT_ED_41, m_tid);
	DDX_Text(pDX, IDC_EDIT_ED_38, m_patid);
	DDX_Text(pDX, IDC_EDIT_ED_1, m_diagnosis);
	DDX_Text(pDX, IDC_EDIT_ED_4, m_proc);
	DDX_Text(pDX, IDC_EDIT_ED_3, m_symptom);
	DDX_Text(pDX, IDC_EDIT_ED_44, m_diagcode);
	DDX_Text(pDX, IDC_EDIT_ED_45, m_pcode1);
	DDX_Text(pDX, IDC_EDIT_ED_49, m_pc1desc);
	DDX_Text(pDX, IDC_EDIT_ED_47, m_dcdesc);
	DDX_Text(pDX, IDC_EDIT_ED_29, m_bdate);
	DDX_Text(pDX, IDC_EDIT_ED_30, m_edate);
	DDX_Text(pDX, IDC_EDIT_ED_53, m_ndate);
	DDX_Text(pDX, IDC_EDIT_ED_5, m_note);
	DDX_Text(pDX, IDC_EDIT_ED_42, m_prl);
	DDX_Text(pDX, IDC_EDIT_ED_33, m_chdate);
	DDX_Text(pDX, IDC_EDIT_ED_31, m_appoint);
	DDX_Text(pDX, IDC_EDIT_ED_54, m_mdate);
	DDX_Text(pDX, IDC_EDIT_ED_55, m_id);
	DDX_Text(pDX, IDC_EDIT_ED_50, m_filter);
	DDX_Text(pDX, IDC_EDIT_ED_51, m_phys);
	DDX_Text(pDX, IDC_EDIT_ED_52, m_phys2);
	DDX_Text(pDX, IDC_EDIT_ED_58, m_trans);
	DDX_Text(pDX, IDC_EDIT_ED_57, m_res);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEventDlg, CGrayDlg)
	//{{AFX_MSG_MAP(CEventDlg)
	ON_BN_CLICKED(IDOK_ED_4, OnOk4)
	ON_BN_CLICKED(IDOK_ED_5, OnOk5)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_ED_1, OnSelchangedTree1)
	ON_NOTIFY(TVN_SELCHANGING, IDC_TREE_ED_1, OnSelchangingTree1)
	ON_BN_CLICKED(IDOK_ED_6, OnOk6)
	ON_BN_CLICKED(IDC_BUTTON_ED_2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON_ED_23, OnButton23)
	ON_BN_CLICKED(IDC_BUTTON_ED_22, OnButton22)
	ON_BN_CLICKED(IDC_BUTTON_ED_38, OnButton38)
	ON_NOTIFY(NM_SETFOCUS, IDC_TREE_ED_1, OnSetfocusTree1)
	ON_BN_CLICKED(IDC_BUTTON_ED_6, OnButton6)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_ED_7, OnButton7)
	ON_BN_CLICKED(IDC_BUTTON_ED_14, OnButton14)
	ON_BN_CLICKED(IDC_BUTTON_ED_15, OnButtonEd15)
	ON_BN_CLICKED(IDC_BUTTON_ED_24, OnButtonEd24)
	ON_BN_CLICKED(IDC_BUTTON_ED_40, OnButtonEd40)
	ON_BN_CLICKED(IDC_BUTTON_ED_39, OnButtonEd39)
	ON_BN_CLICKED(IDOK_ED_7, OnEd7)
	ON_BN_CLICKED(IDC_BUTTON_ED_16, OnButtonEd16)
	ON_BN_CLICKED(IDC_BUTTON_ED_17, OnButtonEd17)
	ON_EN_KILLFOCUS(IDC_EDIT_ED_2, OnKillfocusEditEd2)
	ON_WM_KILLFOCUS()
	ON_BN_CLICKED(IDOK_ED_2, OnEd2)
	ON_EN_CHANGE(IDC_EDIT_ED_50, OnChangeEditEd50)
	ON_BN_CLICKED(IDOK_ED_9, OnEd9)
	ON_BN_CLICKED(IDOK_ED_12, OnEd12)
	ON_BN_CLICKED(IDOK_ED_8, OnEd8)
	ON_EN_KILLFOCUS(IDC_EDIT_ED_31, OnKillfocusEditEd31)
	ON_BN_CLICKED(IDOK_ED_14, OnEd14)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEventDlg message handlers

void CEventDlg::OnOk4() 

{
	BeginWaitCursor();

	//dbfile2 = "codes.sqlt";

	//YieldToWinEx();

	// Fill in customer name tree
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	P2N("Emitting codes\r\n");

	CFile fp;
	if(!fp.Open("proc_codes.txt", CFile::modeRead))
		{
		P2N("Cannot open file\r\n");
		return;
		}

	CString str, str2, str3;
	int count = 0;

	CArchive ar(&fp, CArchive::load);

	//return;

	rc = sqlite3_open(dbfile2, &db);

	if( rc )
		{
		MBOXP("SQL open error: %s", sqlite3_errmsg(db));
		sqlite3_close(db);
		sqlite3_free(zErrMsg);
		return;
		}

	CString qqq("insert into proc_codes (code, desc) values ('");
	CString qqqq;
	
	while(true)
		{
		if(!ar.ReadString(str))
			break;

		str2 = str.Left(4);
		str3 = str.Mid(5);

		//P2N("Archive read in: '%s' '%s'\r\n", str2, str3);

		str3 = support.EscapeSql(str3);

		qqqq = qqq;
		qqqq +=  str2 + "', '";
		qqqq +=  str3 + "')";
		
		//P2N("qqqq = '%s'\r\n", qqqq);

		rc = sqlite3_exec(db, qqqq, callback_noop, this, &zErrMsg);

		if( rc != SQLITE_OK )
			{
			MBOXP("SQL query error: %s", sqlite3_errmsg(db));
			sqlite3_free(zErrMsg);
			break;
			}

		//if(count++ > 20)
		//	break;
		}

	sqlite3_close(db);

	//m_tree.SortChildren(NULL);
	//if(!old)
	//	m_tree.SelectItem(m_tree.GetRootItem());

	EndWaitCursor();
}

//////////////////////////////////////////////////////////////////////////

BOOL CEventDlg::OnInitDialog() 

{
	CGrayDlg::OnInitDialog();
		
	P2N("Looking into customer data file %s\r\n", dbfile);

	SetWindowText(header);
	
	m_epatid.aDisable();
	m_prle.aDisable();
	m_tide.aDisable();

	m_endate.aDisable();
	m_emod.aDisable();
	m_eid.aDisable();

	m_title = "Medical Events Dialog";

	SetTimer(1, 300, NULL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//////////////////////////////////////////////////////////////////////////

void CEventDlg::Fill(const char *str, int find)

{
	//P2N("Filling '%s'\r\n", str);

	filling = TRUE;

	P2N("Opening DB file %s%s\r\n", support.GetCurrentDir(), dbfile);

	HTREEITEM old = m_tree.GetSelectedItem();

	m_tree.DeleteAllItems();
	m_tree.Invalidate();

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

	CString qqq("select EventName, id from events ");
	
	if(str != NULL)
		{
		CString tmp; tmp.Format("where EventName like '%%%s%%' ", str);
		qqq += tmp;
		}

	qqq += " order by newdate desc";

	P2N("qqq=%s\r\n", qqq);

	rc = sqlite3_exec(db, qqq, callback_fill, this, &zErrMsg);
	if( rc != SQLITE_OK )
		{
		MBOXP("SQL error: %s", sqlite3_errmsg(db));
		sqlite3_free(zErrMsg);
		}

	sqlite3_close(db);

	// Select old entry
	if(old)
		{
		m_tree.SelectItem(old);
		}

	EndWaitCursor();

	filling = false;
}

//////////////////////////////////////////////////////////////////////////

void CEventDlg::OnOk5() 

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
		AfxMessageBox("cannot open SQL");
		sqlite3_close(db);
		exit(1);
		}

	//str = "";	sumstr = "";
	//rows = 0;

	CUniqID uid;
	CString str = uid.GetStr();

	CTime ct = CTime::GetCurrentTime();
	CString datestr  = ct.Format("%Y/%m/%d %H:%M:%S");
	CString datestr2 = ct.Format("%Y/%m/%d %H:%M");
	CString timestr  = ct.Format("%H:%M:%S");
	CString date     = ct.Format("%Y/%m/%d");

	CString qqq("insert into events (eventname, eventidstr, newdate, modifydate, begindate, patid) values('");

	qqq +=  "New Event at ";
	qqq +=  datestr2 + "' , '";
	qqq +=   str;
	qqq +=  "', '";
	qqq +=   datestr;
	qqq +=  "', '";
	qqq +=   datestr;
	qqq +=  "', '";
	qqq +=   datestr;
	qqq +=  "', '";
	qqq +=   m_patid;
	qqq +=  "')";

	rc = sqlite3_exec(db, qqq, callback_select, this, &zErrMsg);

	if( rc != SQLITE_OK )
		{
		MBOXP("SQL error: %s", sqlite3_errmsg(db));
		sqlite3_free(zErrMsg);
		return;
		}

	int rowid = (int)sqlite3_last_insert_rowid(db);
	sqlite3_close(db);

	Fill();
	SelEntryInt(rowid);
	FillOne();

	GetDlgItem(IDC_EDIT_ED_2)->SetFocus();

	((CEdit *)GetDlgItem(IDC_EDIT_ED_2))->SetSel(0, -1);

	UpdateData(false);	
}

void CEventDlg::OnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult) 

{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	//P2N("CEventDlg::OnSelchangedTree1\r\n");

	if(filling)
		return;

	CString dir, str, num;
	HTREEITEM hItem  = m_tree.GetSelectedItem();

	if(!hItem)
		return;

	*pResult = 0;

	// Clear all fields
	for(int ii = 0; ii <  farrsize; ii++)
		{
		farr[ii].value = "";
		if(farr[ii].var)
			*farr[ii].var = "";
		}

	UpdateData(false);

	if(!hItem)
		return;

	str =  m_tree.GetItemText(hItem);

	if(str == "")
		return;

	Qdata(m_tree.GetItemData(hItem));

	//FillPRL();

	UpdateData(false);

	*pResult = 0;
}

//////////////////////////////////////////////////////////////////////////

void CEventDlg::Qdata(int getid)

{
	P2N("CEventDlg::Qdata\r\n");

	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	rc = sqlite3_open(dbfile, &db);

	if( rc )
		{
		AfxMessageBox("cannot open SQL");
		sqlite3_close(db);
		exit(1);
		}

	str = "";	
	//sumstr = ""; 
	rows = 0;

	CString qqq = "select ";
	for(int ii = 0; ii <  farrsize; ii++)
		{
		if(farr[ii].var)
			{
			if(ii > 0)
				qqq += ", ";

			qqq += farr[ii].name;
			}
		}

	qqq += " from events where id = ";

	CString tmp; tmp.Format("%d", getid);
	qqq += tmp;

	P2N("Qdata qqq=[%s]\r\n", qqq);

	rc = sqlite3_exec(db, qqq, callback_select, this, &zErrMsg);

	if( rc != SQLITE_OK )
		{
		MBOXP("SQL error in %s line %d %s ", thisname, __LINE__, sqlite3_errmsg(db));
		P2N("SQL error qqq=[%s]\r\n", qqq);
		sqlite3_free(zErrMsg);
		}
	sqlite3_close(db);
}

//////////////////////////////////////////////////////////////////////////

void CEventDlg::OnSelchangingTree1(NMHDR* pNMHDR, LRESULT* pResult) 

{
	*pResult = 0;

	//P2N("CEventDlg::OnSelchangingTree1\r\n");

	if(filling)
		return;

	HTREEITEM hItem  = m_tree.GetSelectedItem();

	if(!hItem)
		return;

	CString str =  m_tree.GetItemText(hItem);

	//str = m_name;
	//m_tree.SetItemText(hItem, str);

	rescan =  UpdateDB(m_tree.GetItemData(hItem));

	UpdateData(false);

	*pResult = 0;
}

//////////////////////////////////////////////////////////////////////////

int		CEventDlg::UpdateDB(int id)

{
	rescan = false;

	UpdateData();

	if(id == -1)
		{
		// If nothing slected, return
		HTREEITEM hItem  = m_tree.GetSelectedItem();
		if(!hItem)
			return 0;

		id = m_tree.GetItemData(hItem);
		}
	
	int changed = false;
	
	// Compare all fields
	for(int ii=0; ii < farrsize - 1; ii++)
		{
		if(farr[ii].var)
			{
			if(farr[ii].value != *farr[ii].var)
				{
				//MBOXP("changed data from %s %s", *arr[i], *arr2[i]);
				changed = true;
				break;
				}
			}
		}

	if(!changed)
		return 0;

	//P2N("CEventDlg::UpdateDB -- updating %d\r\n", id);

	// Set title change flag
	if(farr[0].value != *farr[0].var)
		{
		//title_changed = true;
		rescan = true;
		}

	CTime ct = CTime::GetCurrentTime();
	CString datestr = ct.Format("%Y/%m/%d %H:%M:%S");

	//m_mdate = datestr;
	UpdateData(false);

	sqlite3 	*db;
	char 		*zErrMsg = 0;
	int 		rc;

	rc = sqlite3_open(dbfile, &db);

	if( rc )
		{
		MBOXP("Cannot open SQL file %s", dbfile);
		sqlite3_close(db);
		return 0;
		}

	CString tmp; tmp.Format("%d", id);
	CString qqq("update events set ");

	for(int iii=0; iii < farrsize; iii++)
		{
		if(farr[iii].var)
			{
			farr[iii].var2 = support.EscapeSql(*farr[iii].var);
		
			// Generate all except id
			if(farr[iii].name != "id")
				{
				if(iii > 0)
					qqq += ", ";

				qqq += farr[iii].name + " = '"  +  farr[iii].var2 + "'";			
				}
			}
		}

	qqq += " where id = " + tmp;

	P2N("UpdateDB SQL: [%s]\r\n", qqq);
	//P2N("Updated record '%s'\r\n", m_name);

	rc = sqlite3_exec(db, qqq, callback_noop, this, &zErrMsg);

	if( rc != SQLITE_OK )
		{
		MBOXP("SQL error in %s line %d %s ", thisname, __LINE__, sqlite3_errmsg(db));
		
		//MBOXP("SQL error on '%s' --> %s", qqq, sqlite3_errmsg(db));
		sqlite3_free(zErrMsg);
		}
	sqlite3_close(db);

	// If title changed, rewrite it
	HTREEITEM hItem  = m_tree.GetSelectedItem();
	m_tree.SetItemText(hItem, m_name);

	return rescan;
}

void CEventDlg::OnOk6() 
{
	//BeginWaitCursor();
	//YieldToWinEx();

	// Fill in customer name tree
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	P2N("Emitting codes\r\n");

	CFile fp;
	if(!fp.Open("diag_codes.txt", CFile::modeRead))
		{
		P2N("Cannot open file\r\n");
		return;
		}

	CString str, str2, str3;
	int count = 0;

	CArchive ar(&fp, CArchive::load);

	//return;

	rc = sqlite3_open(dbfile2, &db);

	if( rc )
		{
		MBOXP("SQL open error: %s", sqlite3_errmsg(db));
		sqlite3_close(db);
		sqlite3_free(zErrMsg);
		return;
		}

	CString qqq("insert into diag_codes (code, desc) values ('");
	CString qqqq;
	
	sqlite3_exec(db, "PRAGMA synchronous = OFF", callback_noop, this, &zErrMsg);

	while(true)
		{
		if(!ar.ReadString(str))
			break;

		str2 = str.Left(5);
		str3 = str.Mid(6);

		str2.TrimRight();

		//P2N("Archive read in: '%s' '%s'\r\n", str2, str3);

		str3 = support.EscapeSql(str3);

		qqqq = qqq;
		qqqq +=  str2 + "', '";
		qqqq +=  str3 + "')";
		
		//P2N("qqqq = '%s'\r\n", qqqq);

		rc = sqlite3_exec(db, qqqq, callback_noop, this, &zErrMsg);

		if( rc != SQLITE_OK )
			{
			MBOXP("SQL error in %s line %d %s ", thisname, __LINE__, sqlite3_errmsg(db));
		
			//MBOXP("SQL query error: %s", sqlite3_errmsg(db));
			sqlite3_free(zErrMsg);
			break;
			}

		//if(count++ > 10)
		//	break;

		if(count++ % 30 == 0)
			{
			P2N("Processing record %d\r\n", count);
			}
		}

	sqlite3_exec(db, "PRAGMA synchronous = FULL", callback_noop, this, &zErrMsg);

	sqlite3_close(db);

	EndWaitCursor();
	P2N("Ended Processing. %d records\r\n", count);
}


void CEventDlg::OnButton2() 

{
	UpdateData();

	if(!m_tree.GetSelectedItem())
		{
		MBOX(EventPleaseSel);
		return;
		}

	CpcSelection pcs;	

	P2N("CEventDlg::OnButtonEd2\r\n");

	pcs.table = "diag_codes";
	pcs.title = "Dignosis Code Picker";

	if(pcs.DoModal() == IDOK)
		{
		m_diagcode  = pcs.diagcode;
		m_dcdesc = pcs.description;

		UpdateData(false);
		}
}

//////////////////////////////////////////////////////////////////////////

void CEventDlg::OnButton23() 

{
	UpdateData();

	if(!m_tree.GetSelectedItem())
		{
		MBOX(EventPleaseSel);
		return;
		}

	CpcSelection pcs;	

	P2N("CEventDlg::OnButtonEd23\r\n");

	if(pcs.DoModal() == IDOK)
		{
		m_pcode1  = pcs.diagcode;
		m_pc1desc = pcs.description;

		UpdateData(false);
		}
}


void CEventDlg::OnButton22() 

{
	UpdateData();

	if(!m_tree.GetSelectedItem())
		{
		MBOX(EventPleaseSel);
		return;
		}

	CDateSel ds;

	P2N("CEventDlg::OnButtonEd22\r\n");

	ds.time_disabled = true;
	ds.result = m_bdate;

	if(ds.DoModal() == IDOK)
		{
		m_bdate = ds.result;
		UpdateData(false);
		}	

}

void CEventDlg::OnButton38() 

{
	UpdateData();

	if(!m_tree.GetSelectedItem())
		{
		MBOX(EventPleaseSel);
		return;
		}

	CDateSel ds;

	ds.time_disabled = true;
	ds.result = m_edate;

	P2N("CEventDlg::OnButtonEd38\r\n");

	if(ds.DoModal() == IDOK)
		{
		m_edate = ds.result;
		UpdateData(false);
		}	
	
}

void CEventDlg::SelEntryInt(int num)

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

void CEventDlg::OnSetfocusTree1(NMHDR* pNMHDR, LRESULT* pResult) 

{
	*pResult = 0;
} 

#if 0

void CEventDlg::SelEntryInt(int num)

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
			YieldToWinEx();
			break;
			}
		item = m_tree.GetNextItem(item, TVGN_NEXT );
		}
}

#endif

void CEventDlg::OnButton6() 
{

	BeginWaitCursor();

	//dbfile2 = "codes.sqlt";

	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	P2N("Emitting codes\r\n");

	CFile fp;
	if(!fp.Open("diseases.txt", CFile::modeRead))
		{
		P2N("Cannot open file\r\n");
		return;
		}

	//CFile fp2;
	//if(!fp2.Open("diseases_edited.txt", CFile::modeCreate | CFile::modeWrite))
	//	{
	//	P2N("Cannot open file for writing\r\n");
	//	return;
	//	}

	CString str, str2, str3;
	int count = 0;

	CArchive ar(&fp, CArchive::load);
	//CArchive ar2(&fp2, CArchive::store);

	//return;

	rc = sqlite3_open(dbfile2, &db);

	if( rc )
		{
		MBOXP("SQL error in %s line %d %s ", thisname, __LINE__, sqlite3_errmsg(db));
		
		//MBOXP("SQL open error: %s", sqlite3_errmsg(db));

		sqlite3_close(db);
		sqlite3_free(zErrMsg);
		return;
		}

	// Delete old data, throw away the error code
	sqlite3_exec(db, "DELETE FROM diag_codes", callback_noop, this, &zErrMsg);

	// Faster for lots of ops
	sqlite3_exec(db, "PRAGMA synchronous = OFF", callback_noop, this, &zErrMsg);

	CString qqq("insert into diag_codes (code, desc, longdesc) values ('");
	CString qqqq;
	
	CString cumm;
	while(true)
		{
		if(!ar.ReadString(str))
			break;

		// Short process for testing 
		//if(count > 40 )
		//	break;

		str.TrimLeft();

		int idx1 = str.FindOneOf(" \t\r\n");
		if(idx1 > 0)
			{
			str2 = str.Mid(0, idx1);
			str2.TrimLeft(); str2.TrimRight();


			if(str2.GetLength() > 2)
				{
				// Starts wit 0000 or E000
				if( (str2[0] == 'E' && isdigit(str2[1]) ) 
					||
					(isdigit(str2[0]) && isdigit(str2[1]))
					)
					{
					str3 = str.Mid(idx1);
					str3.TrimLeft(); str3.TrimRight();

					//P2N("Archive num in: '%s' '%s'\r\n", str2, str3);
					
					//if(cumm != "")
					//	{
					//	CString result2; result2.Format("Cumm:------'%s'-----\r\n", cumm);
					//	ar2.WriteString(result2);
					//	}

					//CString result; result.Format("%s -- %s\r\n", str2, str3);
					//P2N("%s", result);
				
					//ar2.WriteString(result);

					str2 = support.EscapeSql(str2);
					str3 = support.EscapeSql(str3);
					cumm = support.EscapeSql(cumm);

					qqqq = qqq;
					qqqq +=  str2 + "', '";
					qqqq +=  str3 + "', '";
					qqqq +=  cumm + "')";
		
					//P2N("qqqq = '%s'\r\n", qqqq);

					rc = sqlite3_exec(db, qqqq, callback_noop, this, &zErrMsg);

					if(count++ % 30 == 0)
						{
						P2N("Processing record %d\r\n", count);
						}
					cumm = "";
					}
				else
					{
					cumm += str + "\r\n";
					}
				}			
			}
	
		if( rc != SQLITE_OK )
			{
			
			//MBOXP("SQL error in %s %d %s ", thisname, __LINE__, sqlite3_errmsg(db));
			MBOXP("SQL query error: %s", sqlite3_errmsg(db));

			sqlite3_free(zErrMsg);
			break;
			}
		}

	sqlite3_exec(db, "PRAGMA synchronous = FULL", callback_noop, this, &zErrMsg);

	sqlite3_close(db);

	//m_tree.SortChildren(NULL);
	//if(!old)
	//	m_tree.SelectItem(m_tree.GetRootItem());

	P2N("Ended conversion\r\n");

	EndWaitCursor();
}

void CEventDlg::OnTimer(UINT nIDEvent) 

{
	// Initial Timer tick
	if(nIDEvent == 1)
		{
		KillTimer(nIDEvent);

		//P2N("CEventDlg::OnTimer initial load\r\n");

		CString drootp; drootp = dataroot + "patients\\";

		if(_chdir(drootp) < 0)
			{
			if(_mkdir(drootp) < 0)
				{
				MBOX("Cannot make patient system's root directory, event data is not available. (read only directory or disk full)\r\n");
				}

			if(_chdir(drootp) < 0)
				{
				MBOX("Cannot change to patient's root directory, event data is not available. (read only directory or disk full)\r\n");
				}
			}
		
		// Create our home dir
		CString str;
		int ret = MakePRLDir(m_prl, str);

		// See if there is no eventdb, create if none
		if(!support.IsFile(dbfile))
			{
			CreateDBfile();
			}
		
		// Make sure table is up to snuff:
		check_table();

		// Populate with data
		Fill();
		
		// Select first entry (commed out)
		//m_tree.SelectItem(m_tree.GetRootItem());
		//FillOne();
		}	

	//////////////////////////////////////////////////////////////////////

	if(nIDEvent == 2)
		{
		KillTimer(nIDEvent);
		P2N("timer for chaged %d\r\n", m_filter);
		Fill(m_filter);
		FillOne();
		}

	if(nIDEvent == 4)
		{
		KillTimer(nIDEvent);
		P2N("Timer tick for header\r\n");
		SetWindowText(old_header);
		old_header = "";
		}

	CGrayDlg::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////

void CEventDlg::FillOne()

{
	if(filling)
		return;

	HTREEITEM hItem  = m_tree.GetSelectedItem();

	if(!hItem)
		return;

	CString dir, str, num;
	
	// Clear all fields
	for(int ii = 0; ii <  farrsize; ii++)
		{
		farr[ii].value = "";
		if(farr[ii].var)
			*farr[ii].var = "";
		}

	UpdateData(false);

	str =  m_tree.GetItemText(hItem);

	if(str == "")
		return;

	Qdata(m_tree.GetItemData(hItem));

	//FillPRL();

	UpdateData(false);
}

void CEventDlg::OnButton7() 
{
	BeginWaitCursor();

	dbfile2 = "codes.sqlt";

	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	P2N("Emitting codes\r\n");

	CFile fp;
	if(!fp.Open("procedures.txt", CFile::modeRead))
		{
		P2N("Cannot open file\r\n");
		return;
		}

	//CFile fp2;
	//if(!fp2.Open("diseases_edited.txt", CFile::modeCreate | CFile::modeWrite))
	//	{
	//	P2N("Cannot open file for writing\r\n");
	//	return;
	//	}

	CString str, str2, str3;
	int count = 0;

	CArchive ar(&fp, CArchive::load);
	//CArchive ar2(&fp2, CArchive::store);

	//return;

	rc = sqlite3_open(dbfile2, &db);

	if( rc )
		{
		MBOXP("SQL open error: %s", sqlite3_errmsg(db));
		sqlite3_close(db);
		sqlite3_free(zErrMsg);
		return;
		}

	// Delete old data, throw away the error code
	sqlite3_exec(db, "DELETE FROM proc_codes", callback_noop, this, &zErrMsg);

	// Faster for lots of ops
	sqlite3_exec(db, "PRAGMA synchronous = OFF", callback_noop, this, &zErrMsg);

	CString qqq("insert into proc_codes (code, desc, longdesc) values ('");
	CString qqqq;
	
	CString cumm;
	while(true)
		{
		if(!ar.ReadString(str))
			break;

		// Short process for testing 
		//if(count > 40)
		//	break;

		str.TrimLeft();

		int idx1 = str.FindOneOf(" \t\r\n");
		if(idx1 > 0)
			{
			str2 = str.Mid(0, idx1);
			str2.TrimLeft(); str2.TrimRight();


			if(str2.GetLength() > 2)
				{
				// Starts wit 0000 or E000
				if( (str2[0] == 'E' && isdigit(str2[1]) ) 
					||
					(isdigit(str2[0]) && isdigit(str2[1]))
					)
					{
					str3 = str.Mid(idx1);
					str3.TrimLeft(); str3.TrimRight();

					//P2N("Archive num in: '%s' '%s'\r\n", str2, str3);
					
					//if(cumm != "")
					//	{
					//	CString result2; result2.Format("Cumm:------'%s'-----\r\n", cumm);
					//	ar2.WriteString(result2);
					//	}

					//CString result; result.Format("%s -- %s\r\n", str2, str3);
					//P2N("%s", result);
				
					//ar2.WriteString(result);

					str2 = support.EscapeSql(str2);
					str3 = support.EscapeSql(str3);
					cumm = support.EscapeSql(cumm);

					qqqq = qqq;
					qqqq +=  str2 + "', '";
					qqqq +=  str3 + "', '";
					qqqq +=  cumm + "')";
		
					//P2N("qqqq = '%s'\r\n", qqqq);

					rc = sqlite3_exec(db, qqqq, callback_noop, this, &zErrMsg);

					if(count++ % 30 == 0)
						{
						P2N("Processing record %d\r\n", count);
						}
					cumm = "";
					}
				else
					{
					cumm += str + "\r\n";
					}
				}			
			}
	
		if( rc != SQLITE_OK )
			{
			//MBOXP("SQL query error: %s", sqlite3_errmsg(db));
			MBOXP("SQL error in %s line %d %s ", thisname, __LINE__, sqlite3_errmsg(db));
		

			sqlite3_free(zErrMsg);
			break;
			}
		}

	sqlite3_exec(db, "PRAGMA synchronous = FULL", callback_noop, this, &zErrMsg);

	sqlite3_close(db);

	//m_tree.SortChildren(NULL);
	//if(!old)
	//	m_tree.SelectItem(m_tree.GetRootItem());

	P2N("Ended conversion\r\n");

	EndWaitCursor();	
}

void CEventDlg::OnButton14() 

{
	UpdateData();

	if(!m_tree.GetSelectedItem())
		{
		MBOX(EventPleaseSel);
		return;
		}

	CDiagList dl;

	dl.tid = m_tid;

	dl.m_diag	=	m_diagcode;
	dl.m_diagd	=	m_dcdesc;
	dl.m_diagn	=	m_diagnosis;

	dl.DoModal();	

	m_diagcode	=	dl.m_diag;
	m_dcdesc	=	dl.m_diagd;
	m_diagnosis	=	dl.m_diagn;

	UpdateData(false);
		
	UpdateDB(-1);
}

void CEventDlg::OnButtonEd15() 

{
	P2N("CEventDlg::OnButtonEd15\r\n");
	
	UpdateData();

	if(!m_tree.GetSelectedItem())
		{
		MBOX(EventPleaseSel);
		return;
		}

	CPRocList pl;

	UpdateDB();

	// Assign already gotten values
	pl.m_pcode	= m_pcode1;
	pl.m_pcoded	= m_pc1desc;
	pl.m_proc	= m_proc;

	pl.tid = m_tid;

	pl.DoModal();

	// Save it back
	m_pcode1	= pl.m_pcode;
	m_pc1desc	= pl.m_pcoded;
	m_proc	= pl.m_proc;

	UpdateData(false);

	UpdateDB(-1);	
}

void CEventDlg::OnButtonEd24() 

{	
	UpdateData();

	if(!m_tree.GetSelectedItem())
		{
		MBOX(EventPleaseSel);
		return;
		}

	CDateSel ds;

	ds.result = m_chdate;

	if(ds.DoModal() == IDOK)
		{
		m_chdate = ds.result;
		P2N("Got date %s\r\n", m_bdate);
		UpdateData(false);
		}
}

void CEventDlg::OnButtonEd40() 
{
	P2N("Scedule\r\n");
}

void CEventDlg::OnButtonEd39() 

{

	UpdateData();

	if(!m_tree.GetSelectedItem())
		{
		MBOX(EventPleaseSel);
		return;
		}

	CDateSel ds;

	//P2N("CEventDlg::OnButtonEd22\r\n");

	ds.result = m_appoint;

	if(ds.DoModal() == IDOK)
		{
		m_appoint = ds.result;
		UpdateData(false);

		SetAppoint();
		}		
}

void CEventDlg::CreateDBfile()

{
	P2N("CEventDlg::CreateDBfile Creating DB file in %s\r\n", support.GetCurrentDir());
			
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	rc = sqlite3_open(dbfile, &db);

	if( rc )
		{
		//MBOXP("SQL error: %s", sqlite3_errmsg(db));

		MBOXP("SQL error in %s line %d %s ", thisname, __LINE__, sqlite3_errmsg(db));
		
		sqlite3_close(db);
		sqlite3_free(zErrMsg);
		return;
		}
			
	CString qqq("create table events (");

	// Fill in names/values of fields to create
	for(int iii=0; iii < farrsize; iii++)
		{
		if(iii != 0)
			qqq += ", ";

		if(farr[iii].name == "id" && backup)
			{
			qqq += farr[iii].name + " integer";
			}
		else
			{
			qqq += farr[iii].name + "  " + farr[iii].type;
			}	
		}

	// These are in the proc detail dialogs
	CPRocList pl;

	for(int loop = 0; loop < pl.farrsize; loop++)
		{
		qqq += ", ";
		qqq +=  pl.farr[loop].name + " " + pl.farr[iii].type;
		}
	
	// These are in the diag detail dialogs
	CDiagList dl;

	for(int loop2 = 0; loop2 < dl.farrsize; loop2++)
		{
		qqq += ", ";
		qqq +=  dl.farr[loop2].name + " " + dl.farr[iii].type;
		}

	qqq += ")";

	//MBOXP("SQL: '%s'", qqq);
	
	P2N("qqq=[%s]\r\n", qqq);

	rc = sqlite3_exec(db, qqq, NULL, this, &zErrMsg);

	if( rc != SQLITE_OK )
		{
		MBOXP("SQL error on Create Table: %s", sqlite3_errmsg(db));
		sqlite3_free(zErrMsg);
		}

	// Add default entries:

	CUniqID uid;
	CString str = uid.GetStr();

	CTime ct = CTime::GetCurrentTime();
	CString datestr  = ct.Format("%Y/%m/%d %H:%M:%S");
	CString datestr2 = ct.Format("%Y/%m/%d %H:%M");
	CString timestr  = ct.Format("%H:%M:%S");
	CString date     = ct.Format("%Y/%m/%d");

	qqq = "insert into events (eventname, eventidstr, newdate, modifydate, begindate, TranNote, patid) values('";

	qqq +=  "Basic Biometrics ";
	qqq +=  "', '";
	qqq +=   str;
	qqq +=  "', '";
	qqq +=   datestr;
	qqq +=  "', '";
	qqq +=   datestr;
	qqq +=  "', '";
	qqq +=   datestr;
	qqq +=  "', '";
	qqq +=   "Fill in Physical Parameters such as Height, Weight,\r\nGender, Ethnicity, Blood Type, Allergies, Sensitivities etc ...";
	qqq +=  "', '";
	qqq +=   m_patid;
	qqq +=  "')";

	rc = sqlite3_exec(db, qqq, NULL, this, &zErrMsg);
	if( rc != SQLITE_OK )
		{
		MBOXP("SQL error: %s", sqlite3_errmsg(db));
		sqlite3_free(zErrMsg);
		}

	sqlite3_close(db);
}


void CEventDlg::OnEd7() 

{
	P2N("Delete Record\r\n");

	HTREEITEM hItem2  = m_tree.GetSelectedItem();
	if(hItem2)
		{
		UpdateDB(m_tree.GetItemData(hItem2));
		}

	CString dir, str, num;
	HTREEITEM hItem  = m_tree.GetSelectedItem();

	if(!hItem)
		{
		MBOX(EventPleaseSel);
		return;
		}

	int item = m_tree.GetItemData(hItem);
	str =  m_tree.GetItemText(hItem);

	// Select next one when deleting
	HTREEITEM item3 = m_tree.GetNextItem(hItem, TVGN_NEXT);

	//if(str == "")
	//	{
	//	MBOX("Please select an entry first.(n)");
	//	return;
	//	}

	CString str2;
	if(item3)
		str2 = m_tree.GetItemText(item3);

	CString tmpbox;
	tmpbox.Format("Are you sure you want to delete:\r\n\r\n'%s' ?", str);

	if(MBOX(tmpbox, "", MB_YESNO, IDNO) != IDYES)
		return ;

	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	rc = sqlite3_open(dbfile, &db);
	if( rc )
		{
		xMsgBox("cannot open SQL");
		sqlite3_close(db);
		return;
		}

	CString qqq;
	qqq.Format("delete from events where id==%d", item);
	P2N("Calling sqlite select\r\n");
	rc = sqlite3_exec(db, qqq, callback_noop, this, &zErrMsg);

	if( rc != SQLITE_OK )
		{
		MBOXP("SQL error: %s", sqlite3_errmsg(db));
		sqlite3_free(zErrMsg);
		}
	sqlite3_close(db);

	// Clear all fields
	for(int ii=0; ii < farrsize - 1; ii++)
		{
		farr[ii ].value = "";
		if(farr[ii].var)
			*farr[ii].var = "";
		}

	UpdateData(false);

	Fill();

	// Select next in line
	if(str2 != "")
		SelEntry(str2);
	else
		SelLast();
	
	HTREEITEM hItem3  = m_tree.GetSelectedItem();

	if(hItem3)
		{
		Qdata(m_tree.GetItemData(hItem3));
		}

	m_tree.SetFocus();
}

void CEventDlg::OnButtonEd16() 

{	
	UpdateData();

	if(!m_tree.GetSelectedItem())
		{
		MBOX(EventPleaseSel);
		return;
		}

	CTranscript ct;

	CString hh = "Transcription Details Dialog";
	ct.head = &hh;
	ct.ref = &m_note;
	ct.DoModal();

	UpdateData(false);
}

void CEventDlg::OnButtonEd17() 

{
	UpdateData();

	if(!m_tree.GetSelectedItem())
		{
		MBOX(EventPleaseSel);
		return;
		}

	CTranscript ct;

	CString hh = "Symptom Details Dialog";
	ct.head = &hh;
	ct.ref = &m_symptom;
	ct.DoModal();

	UpdateData(false);	
}

void CEventDlg::SelLast()

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

void CEventDlg::SelEntry(const char * estr)

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

void CEventDlg::OnKillfocusEditEd2() 

{
	P2N("CEventDlg::OnKillfocusEditEd2\r\n"); 

	HTREEITEM hItem  = m_tree.GetSelectedItem();
	if(hItem)
		{
		P2N("CEventDlg::OnKillfocusEditEd2 - Selected Item\r\n");

		// Update (if changed)
		UpdateDB(m_tree.GetItemData(hItem));
		}
	else
		{
		// Add new data to list
		UpdateData();

		P2N("CEventDlg::OnKillfocusEditEd2 unSelected Item\r\n");

		if(m_name != "")
			{
			sqlite3 *db;
			char *zErrMsg = 0;
			int rc;

			rc = sqlite3_open(dbfile, &db);

			if( rc )
				{
				xMsgBox("Cannot open SQL");
				sqlite3_close(db);

				return;
				//exit(1);
				}

			str = "";
			//sumstr = "";
			rows = 0;

			CUniqID uid;
			CString str = uid.GetStr();

			CTime ct = CTime::GetCurrentTime();
			CString datestr  = ct.Format("%Y/%m/%d %H:%M:%S");
			CString datestr2 = ct.Format("%Y/%m/%d %H:%M");
			CString timestr  = ct.Format("%H:%M:%S");
			CString date     = ct.Format("%Y/%m/%d");

			CString qqq("insert into events (eventname, eventidstr, newdate, modifydate, begindate) values('");

			qqq +=   m_name + "', '";
			qqq +=   str;
			qqq +=  "', '";
			qqq +=   datestr;
			qqq +=  "', '";
			qqq +=   datestr;
			qqq +=  "', '";
			qqq +=   datestr;
			qqq +=  "')";

			rc = sqlite3_exec(db, qqq, callback_select, this, &zErrMsg);

			if( rc != SQLITE_OK )
				{
				MBOXP("SQL error: %s", sqlite3_errmsg(db));
				sqlite3_free(zErrMsg);
				return;
				}

			int rowid = (int)sqlite3_last_insert_rowid(db);
			sqlite3_close(db);

			Fill();
			SelEntryInt(rowid);
			}
		else
			{
			MBOX("No event item is currently active, please select one.");
			}
		}	
}

//////////////////////////////////////////////////////////////////////////

void CEventDlg::OnCancel() 

{	
	UpdateDB(-1);
	
	CGrayDlg::OnCancel();
}

void CEventDlg::OnOK() 

{	
	UpdateDB(-1);

	CGrayDlg::OnOK();
}

//////////////////////////////////////////////////////////////////////////

void CEventDlg::OnKillFocus(CWnd* pNewWnd) 

{
	CGrayDlg::OnKillFocus(pNewWnd);
	
	//P2N("CEventDlg::OnKillFocus\r\n");
		
	UpdateDB(-1);
}

//////////////////////////////////////////////////////////////////////////

void CEventDlg::init_array()

{
	//P2N("sizeof(fields)=%d sizeof(farr)=%d count=%d\r\n", sizeof(fields), sizeof(farr), 
	//							sizeof(farr)/sizeof(fields));

	// Init all fields
	for(int loop = 0; loop < sizeof(farr)/sizeof(fields); loop++)
		{
		farr[loop].name		= farr[loop].value	= farr[loop].value2  = farr[loop].var2	= "";
		farr[loop].type		= "text";
		farr[loop].var		= NULL;
		farr[loop].ctrl		= NULL;
		farr[loop].checked	= farr[loop].tipid	= farr[loop].olddis	= 0;
		}
	farrsize = 0;

	int fs = 0;
	char *txt = "text"; char *tdt = "datetime"; char *ipk = "integer primary key";
	
	// Fill in
	farr[fs].name =  "EventName";	farr[fs].var = &m_name;		  farr[fs].type = txt;  fs++;
	farr[fs].name =  "EventIDstr";	farr[fs].var = &m_tid;		  farr[fs].type = txt;  fs++;
	farr[fs].name =  "Procedure";	farr[fs].var = &m_proc;		  farr[fs].type = txt;  fs++;
	farr[fs].name =  "Diagnosis";	farr[fs].var = &m_diagnosis;  farr[fs].type = txt;  fs++;
	farr[fs].name =  "Symptom";		farr[fs].var = &m_symptom;	  farr[fs].type = txt;  fs++;
	farr[fs].name =  "Diagcode";	farr[fs].var = &m_diagcode;	  farr[fs].type = txt;  fs++;
	farr[fs].name =  "ProcCode";	farr[fs].var = &m_pcode1;	  farr[fs].type = txt;  fs++;
	farr[fs].name =  "PcodeDesc";	farr[fs].var = &m_pc1desc;	  farr[fs].type = txt;  fs++;
	farr[fs].name =  "DcDesc";		farr[fs].var = &m_dcdesc;	  farr[fs].type = txt;  fs++;
	farr[fs].name =  "BeginDate";	farr[fs].var = &m_bdate;	  farr[fs].type = txt;  fs++;
	farr[fs].name =  "TranNote";	farr[fs].var = &m_note;		  farr[fs].type = txt;  fs++;
	farr[fs].name =  "PatID";		farr[fs].var = &m_patid;  	  farr[fs].type = txt;  fs++;
	farr[fs].name =  "Reso";		farr[fs].var = &m_res;	  	  farr[fs].type = txt;  fs++;
	farr[fs].name =  "TranStat";	farr[fs].var = &m_trans;	  farr[fs].type = txt;  fs++;
	farr[fs].name =  "EndDate";		farr[fs].var = &m_edate;	  farr[fs].type = tdt;  fs++;
	farr[fs].name =  "NewDate";		farr[fs].var = &m_ndate;	  farr[fs].type = tdt;	fs++;
	farr[fs].name =  "ModifyDate";	farr[fs].var = &m_mdate;	  farr[fs].type = tdt;  fs++;
	farr[fs].name =  "AppointDate";	farr[fs].var = &m_appoint;	  farr[fs].type = tdt;  fs++;
	farr[fs].name =  "ChangeDate";	farr[fs].var = &m_chdate;	  farr[fs].type = tdt;  fs++;
	farr[fs].name =  "id";			farr[fs].var = &m_id;		  farr[fs].type = ipk;  fs++;

	farrsize = fs;
	
	// This is to make sure that we did not exceed array bounds
	ASSERT(farrsize < sizeof(farr)/sizeof(fields));

	//P2N("Array size %d limit: %d\r\n",  farrsize, sizeof(farr)/sizeof(fields));
}


void CEventDlg::OnEd2() 
{
	//UpdateDB();	


}

void CEventDlg::OnChangeEditEd50() 
{
	
	UpdateData();
	P2N("CEventDlg::OnChangeEditEd50 %s\r\n", m_filter);
	SetTimer(2, 500, NULL);	
}

void CEventDlg::OnEd9() 
{
	SetTimer(3, 100, NULL);	
}

void CEventDlg::OnEd12() 

{
	m_filter = "";
	UpdateData(false);
	Fill();	
}

void CEventDlg::OnEd8() 
{
	MBOX("PHYSICIAN");	
}

void CEventDlg::OnKillfocusEditEd31() 

{
	UpdateData();
	SetAppoint();
}

//////////////////////////////////////////////////////////////////////////

void CEventDlg::SetAppoint()

{
	CString str, str2;
	COleDateTime dd;

	if(m_appoint == "")
		return;

	int ret = Str2Date(m_appoint, &dd);

	if(ret)
		{
		MBOXP("Bad date format: %s, no appointment is set.", m_appoint);
		return;
		}

	str2 = dd.Format("%Y/%m/%d %H:%M:%S");

	P2N("Setting new appointment: %s\r\n", str2);

	if(old_header == "")
		GetWindowText(old_header);

	str.Format("Set Appointment to %s", str2);

	SetWindowText(str);
	SetTimer(4, 2000, NULL);

	CheckCalDBfile(NULL);

	// See if we have an entry like this already:
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	rc = sqlite3_open(calfile, &db);

	if(rc)
		{
		MBOXP("SQL error: %s %s", calfile, sqlite3_errmsg(db));
		sqlite3_close(db);
		sqlite3_free(zErrMsg);
		return;
		}

	CString qqq;
	qqq.Format("select name from calendar where tidstr = '%s'", m_tid);
	P2N("qqq=%s\r\n", qqq);

	int cntt = 0;
	rc = sqlite3_exec(db, qqq, callback_cal, &cntt, &zErrMsg);

	if( rc != SQLITE_OK )
		{
		MBOXP("SQL error: %s %s", CalendarFile, sqlite3_errmsg(db));
		sqlite3_free(zErrMsg);
		}
	
	if(cntt > 0)
		{
		//P2N("Entry already there, updating\r\n");
		qqq =	"update calendar set sdate = '";
		qqq +=	 str2 + "'";
		qqq +=  "where tidstr = '" + m_tid + "'";
		}
	else
		{
		//P2N("No such entry, adding %s\r\n", m_appoint);

		//////////////////////////////////////////////////////////////////
		// Gather data
		CString abdbfile   = dataroot + "db\\address.sqlt";

		CString res[3];
		CString qqq2; 
		qqq2.Format("select FirstName, Midname, Lastname from address where uidstr = '%s' limit 1", m_patid);
		DLookup(abdbfile, qqq2, res, 3);
	
		CString str3; str3 = res[0] + ", " + res[1] + ", " + res[2];

		P2N("Got patient name '%s'\r\n", str3);

		qqq = "insert into calendar (name, sdate, note, tidstr, patid) values ('";
		
		qqq += support.EscapeSql(str3) +   "', '";
		qqq += str2 +   "', '";
		qqq += support.EscapeSql(m_name) + "', '";
		qqq += m_tid +  "', '";
		qqq += m_patid + "')";
		}

	P2N("SQL qqq = [%s]\r\n", qqq);

	rc = sqlite3_exec(db, qqq, NULL, NULL, &zErrMsg);

	if( rc != SQLITE_OK )
		{
		MBOXP("SQL error: %s %s", CalendarFile, sqlite3_errmsg(db));
		sqlite3_free(zErrMsg);
		}
		
	sqlite3_close(db);
}

//////////////////////////////////////////////////////////////////////////
//
// This is to accomodate new fields as the DB grows.
//

int CEventDlg::check_table()

{
	//P2N("CEventDlg::check_table %s%s\r\n", support.GetCurrentDir(), dbfile);

	// Clear all fields
	for(int ii = 0; ii <  farrsize; ii++)
		{
		farr[ii].checked = 0;
		}

	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	rc = sqlite3_open(dbfile, &db);

	if(rc)
		{
		MBOXP("SQL error: %s %s", calfile, sqlite3_errmsg(db));
		sqlite3_close(db);
		sqlite3_free(zErrMsg);
		return 0;
		}

	CString qqq; qqq.Format("PRAGMA table_info(%s)", dbtable);

	rc = sqlite3_exec(db, qqq, callback_table, this, &zErrMsg);

	if( rc != SQLITE_OK )
		{
		MBOXP("SQL error: %s %s", dbfile, sqlite3_errmsg(db));
		sqlite3_free(zErrMsg);
		}
		
	// Check array for unchecked items, add new columns
	for(int iii = 0; iii <  farrsize; iii++)
		{
		if(farr[iii].checked == 0)
			{
			//P2N("***Unchecked=%s\r\n", farr[iii].name);
			
			qqq.Format("ALTER TABLE %s ADD COLUMN %s %s", dbtable, farr[iii].name, farr[iii].type);

			P2N("sql=[%s]\r\n", qqq);
	
			rc = sqlite3_exec(db, qqq, NULL, NULL, &zErrMsg);

			if( rc != SQLITE_OK )
				{
				MBOXP("SQL error: %s %s", dbfile, sqlite3_errmsg(db));
				sqlite3_free(zErrMsg);
				}		
			}
		}

	sqlite3_close(db);

	return 0;
}



void CEventDlg::OnEd14() 
{

	P2N("Help wanted\r\n");

	CString approot; support.GetAppRoot(approot);
	CString str, path("umanual\\"), file("event.html");

#ifdef _DEBUG
	str = "Z:/archive/xraynotes/umanual/" + file;
#else
	str =  approot + path + file;
#endif

	ShellExecute(NULL, "open", str, NULL, NULL, SW_SHOWMAXIMIZED);
}
