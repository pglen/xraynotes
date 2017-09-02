// CodeEd.cpp : implementation file
//

#include "stdafx.h"
#include "dbtest.h"
#include "CodeEd.h"

#include "dbtest.h"
#include "mxpad.h"
#include "support.h"
#include "sqlite3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static int callback_update(void *NotUsed, int argc, char **argv, char **azColName)

{
	P2N("CCodeEd::OnOK() callback_update\r\n");
	return 0;
}


static int callback_fill(void *NotUsed, int argc, char **argv, char **azColName)

{
	CCodeEd *lastclass = (CCodeEd *)NotUsed;

	P2N("CCodeEd::OnOK() callback_fill\r\n");

	//ASSERT(argc == 5);
	//P2N("Got item: %d %s %s\r\n", atoi(argv[0]), argv[1], argv[2]);

	lastclass->m_id2    = lastclass->m_id		= argv[0];
	lastclass->m_code2  = lastclass->m_code		= argv[1];
	lastclass->m_title2 = lastclass->m_title	= argv[2];
	lastclass->m_desc2  = lastclass->m_desc		= argv[3];
	lastclass->m_notes2 = lastclass->m_notes	= argv[4];

	lastclass->UpdateData(false);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CCodeEd dialog

CCodeEd::CCodeEd(CWnd* pParent /*=NULL*/)
	: CDialog(CCodeEd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCodeEd)
	m_id = _T("");
	m_code = _T("");
	m_desc = _T("");
	m_title = _T("");
	m_notes = _T("");
	//}}AFX_DATA_INIT

	id = 0;
}

void CCodeEd::DoDataExchange(CDataExchange* pDX)

{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCodeEd)
	DDX_Control(pDX, IDC_EDIT_CED_1, m_ced1);
	DDX_Control(pDX, IDC_EDIT_CED_2, m_ced2);
	DDX_Control(pDX, IDC_EDIT_CED_3, m_ced3);
	DDX_Control(pDX, IDC_EDIT_CED_4, m_ced4);
	DDX_Control(pDX, IDC_EDIT_CED_5, m_ced5);
	
	DDX_Text(pDX, IDC_EDIT_CED_1, m_id);
	DDX_Text(pDX, IDC_EDIT_CED_2, m_code);
	DDX_Text(pDX, IDC_EDIT_CED_3, m_desc);
	DDX_Text(pDX, IDC_EDIT_CED_4, m_title);
	DDX_Text(pDX, IDC_EDIT_CED_5, m_notes);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCodeEd, CDialog)
	//{{AFX_MSG_MAP(CCodeEd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCodeEd message handlers

BOOL CCodeEd::OnInitDialog() 

{
	CDialog::OnInitDialog();
	
	BeginWaitCursor();

	// Fill in customer name tree
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	CString dbfile = droot + "db\\codes.sqlt";
	rc = sqlite3_open(dbfile, &db);

	if( rc )
		{
		MBOXP("SQL error: %s", sqlite3_errmsg(db));
		sqlite3_close(db);
		sqlite3_free(zErrMsg);

		EndWaitCursor();
		return TRUE;
		}

	CString qqq;
	qqq.Format("select id,  desc, code, longdesc, note from %s where id = %d limit 1", table, id);

	P2N("qqq=%s\r\n", qqq);

	rc = sqlite3_exec(db, qqq, callback_fill, this, &zErrMsg);
	if( rc != SQLITE_OK )
		{
		MBOXP("SQL error: %s", sqlite3_errmsg(db));
		sqlite3_free(zErrMsg);
		}

	m_ced1.Disable();
	m_ced4.Disable();

	sqlite3_close(db);
	EndWaitCursor();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCodeEd::OnOK()
 
{
	UpdateData();

	if(	m_id2    != m_id		||
		m_code2  != m_code		||
		m_title2 != m_title		||
		m_desc2  != m_desc		||
		m_notes2 != m_notes	)
		{
		P2N("CCodeEd::OnOK() Changed\r\n");

		sqlite3 *db;
		char *zErrMsg = 0;
		int rc;

		
		CString dbfile = droot + "codes.sqlt";
		rc = sqlite3_open(dbfile, &db);

		if( rc )
			{
			MBOXP("SQL error: %s", sqlite3_errmsg(db));
			sqlite3_close(db);
			sqlite3_free(zErrMsg);

			EndWaitCursor();
			goto endd;
			}
			{
			m_title = support.EscapeSql(m_title);
			m_desc	= support.EscapeSql(m_desc);
			m_notes = support.EscapeSql(m_notes);

			CString qqq;
			qqq.Format("update %s set desc = '%s', longdesc = '%s', note = '%s'  where id = %d", 
					table, m_title, m_desc, m_notes, id);

			P2N("CCodeEd::OnOK() qqq=%s\r\n", qqq);

			BeginWaitCursor();
			rc = sqlite3_exec(db, qqq, callback_update, this, &zErrMsg);
			if( rc != SQLITE_OK )
				{
				MBOXP("SQL error: %s", sqlite3_errmsg(db));
				sqlite3_free(zErrMsg);
				}
			sqlite3_close(db);
			EndWaitCursor();
			}
		}
		
endd:

	CDialog::OnOK();
}
