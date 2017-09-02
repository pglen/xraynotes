// DiagList.cpp : implementation file
//

#include "stdafx.h"
#include "dbtest.h"
#include "sqlite3.h"
#include "DiagList.h"
#include "support.h"

#include "mxpad.h"
#include "pcSelection.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static	int		callback_select(void *NotUsed, int argc, char **argv, char **azColName)

{
	CDiagList *lastclass = (CDiagList *)NotUsed;

	//ASSERT(lastclass->magic == PLIST_MAGIC);

	P2N("CDiagList -> callback_select\r\n");
	
	lastclass->rows++;

	CString  tmp, ttmp;

	for(int ii=0; ii < argc; ii++)
		{
		ttmp.Format("%s=%s  ",  azColName[ii], argv[ii] ? argv[ii] : "NULL");

		if(argv[ii])
			{
			*lastclass->farr[ii].var  = argv[ii];
			lastclass->farr[ii].value  = argv[ii];
			}
		else
			{
			lastclass->farr[ii].value = "";
			}
		tmp += ttmp;
		}

	tmp += "\r\n\r\n";

	//lastclass->sumstr += tmp;
	
	P2N("tmp=%s\r\n", tmp);

	lastclass->UpdateData(false);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CDiagList dialog


CDiagList::CDiagList(CWnd* pParent /*=NULL*/)
	: CDialog(CDiagList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDiagList)
	m_diag = _T("");
	m_diagd = _T("");
	m_diag1 = _T("");
	m_diag1d = _T("");
	m_diag1n = _T("");
	m_diag2 = _T("");
	m_diag2d = _T("");
	m_diag2n = _T("");
	m_diag3 = _T("");
	m_diag3d = _T("");
	m_diag3n = _T("");
	m_diagn = _T("");
	//}}AFX_DATA_INIT

	dbfile = "events.sqlt";

	init_array();
}

void CDiagList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDiagList)
	DDX_Text(pDX, IDC_DL_EDIT44, m_diag);
	DDX_Text(pDX, IDC_DL_EDIT47, m_diagd);
	DDX_Text(pDX, IDC_DL_EDIT45, m_diag1);
	DDX_Text(pDX, IDC_DL_EDIT48, m_diag1d);
	DDX_Text(pDX, IDC_DL_EDIT4, m_diag1n);
	DDX_Text(pDX, IDC_DL_EDIT46, m_diag2);
	DDX_Text(pDX, IDC_DL_EDIT49, m_diag2d);
	DDX_Text(pDX, IDC_DL_EDIT5, m_diag2n);
	DDX_Text(pDX, IDC_DL_EDIT50, m_diag3);
	DDX_Text(pDX, IDC_DL_EDIT51, m_diag3d);
	DDX_Text(pDX, IDC_DL_EDIT6, m_diag3n);
	DDX_Text(pDX, IDC_DL_EDIT3, m_diagn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDiagList, CDialog)
	//{{AFX_MSG_MAP(CDiagList)
	ON_BN_CLICKED(IDC_DL_BUTTON2, OnDlButton2)
	ON_BN_CLICKED(IDC_DL_BUTTON23, OnDlButton23)
	ON_BN_CLICKED(IDC_DL_BUTTON30, OnDlButton30)
	ON_BN_CLICKED(IDC_DL_BUTTON24, OnDlButton24)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDiagList message handlers

void CDiagList::OnDlButton2() 
{
	UpdateData();

	CpcSelection pcs;	

	pcs.table = "diag_codes";
	pcs.title = "Dignostics Code Picker";

	if(pcs.DoModal() == IDOK)
		{
		m_diag	= pcs.diagcode;
		m_diagd = pcs.description;

		UpdateData(false);
		}	
}

BOOL CDiagList::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetTimer(1, 100, NULL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDiagList::OnDlButton23() 

{
	UpdateData();

	CpcSelection pcs;	

	pcs.table = "diag_codes";
	pcs.title = "Dignostics Code Picker";

	if(pcs.DoModal() == IDOK)
		{
		m_diag1	= pcs.diagcode;
		m_diag1d = pcs.description;

		UpdateData(false);
		}		
}

void CDiagList::OnDlButton30() 
{
	UpdateData();

	CpcSelection pcs;	

	pcs.table = "diag_codes";
	pcs.title = "Dignostics Code Picker";

	if(pcs.DoModal() == IDOK)
		{
		m_diag2	= pcs.diagcode;
		m_diag2d = pcs.description;

		UpdateData(false);
		}		
}

void CDiagList::OnDlButton24() 
{
	UpdateData();

	CpcSelection pcs;	

	pcs.table = "diag_codes";
	pcs.title = "Dignostics Code Picker";

	if(pcs.DoModal() == IDOK)
		{
		m_diag3	= pcs.diagcode;
		m_diag3d = pcs.description;

		UpdateData(false);
		}		
}

void CDiagList::OnOK() 

{	
	UpdateDB();

	CDialog::OnOK();
}

void CDiagList::init_array()

{
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

	farr[farrsize].name =  "Diagnosis1";	farr[farrsize].var = &m_diag1;   farrsize++;
	farr[farrsize].name =  "Diag1Desc";		farr[farrsize].var = &m_diag1d;	 farrsize++;
	farr[farrsize].name =  "Diag1Note";		farr[farrsize].var = &m_diag1n;  farrsize++;

	farr[farrsize].name =  "Diagnosis2";	farr[farrsize].var = &m_diag2;   farrsize++;
	farr[farrsize].name =  "Diag2Desc";		farr[farrsize].var = &m_diag2d;  farrsize++;
	farr[farrsize].name =  "Diag2Note";		farr[farrsize].var = &m_diag2n;  farrsize++;

	farr[farrsize].name =  "Diagnosis3";	farr[farrsize].var = &m_diag3;   farrsize++;
	farr[farrsize].name =  "Diag3Desc";		farr[farrsize].var = &m_diag3d;  farrsize++;
	farr[farrsize].name =  "Diag3Note";		farr[farrsize].var = &m_diag3n;  farrsize++;

	// This is to make sure that we did not exceed array bounds
	ASSERT(farrsize < sizeof(farr)/sizeof(fields));

}


void CDiagList::LoadData(int getid)

{
	//P2N("CDiagList::LoadData\r\n");

	sqlite3 *db;
	char *zErrMsg = 0;
	int	rc;

	rc = sqlite3_open(dbfile, &db);

	if( rc )
		{
		AfxMessageBox("cannot open SQL");
		sqlite3_close(db);
		exit(1);
		}

	rows = 0;

	CString qqq = "select ";
	
	for(int ii = 0; ii <  farrsize; ii++)
		{
		//if(farr[ii].var)
			{
			if(ii > 0)
				qqq += ", ";

			qqq += farr[ii].name;
			}
		}

	qqq += " from events where EventIDstr = ";

	CString tmp; tmp.Format("'%s' limit 1", tid);
	qqq += tmp;

	P2N("CDiagList::LoadData qqq=[%s]\r\n", qqq);

	rc = sqlite3_exec(db, qqq, callback_select, this, &zErrMsg);

	if( rc != SQLITE_OK )
		{
		MBOXP("SQL error in %s line %d %s ", "EventdDlg", __LINE__, sqlite3_errmsg(db));
		P2N("SQL error qqq=[%s]\r\n", qqq);
		sqlite3_free(zErrMsg);
		}
	sqlite3_close(db);
}


int	CDiagList::UpdateDB()

{
	UpdateData();

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

	P2N("CDiagList::UpdateDB -- updating '%s'\r\n", tid);

	CTime ct = CTime::GetCurrentTime();
	CString datestr = ct.Format("%Y/%m/%d %H:%M:%S");

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

	CString tmp; tmp.Format("'%s'", tid);
	CString qqq("update events set ");

	for(int iii=0; iii < farrsize; iii++)
		{
		if(farr[iii].var)
			{
			farr[iii].var2 = support.EscapeSql(*farr[iii].var);
		
			if(iii > 0)
				qqq += ", ";

			qqq += farr[iii].name + " = '"  +  farr[iii].var2 + "'";			
			}
		}

	qqq += " where EventIDstr = " + tmp;

	P2N("CDiagList::UpdateDB SQL: [%s]\r\n", qqq);

	//P2N("Updated record '%s'\r\n", m_name);

	rc = sqlite3_exec(db, qqq, NULL, this, &zErrMsg);

	if( rc != SQLITE_OK )
		{
		MBOXP("SQL error in %s line %d %s ", "EventdDlg", __LINE__, sqlite3_errmsg(db));
		
		//MBOXP("SQL error on '%s' --> %s", qqq, sqlite3_errmsg(db));

		sqlite3_free(zErrMsg);
		}
	
	sqlite3_close(db);

	return 0;
}

void CDiagList::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent == 1)
		{
		KillTimer(nIDEvent);
		LoadData();
		}
	CDialog::OnTimer(nIDEvent);
}

void CDiagList::OnCancel() 
{
	UpdateDB();
	
	CDialog::OnCancel();
}
