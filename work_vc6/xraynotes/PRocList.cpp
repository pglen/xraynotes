
/* =====[ PRocList.cpp ]========================================== 
                                                                             
   Description:     The dbtest project, implementation of the PRocList.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  6/16/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// PRocList.cpp : implementation file
//

#include "stdafx.h"
#include "xraynotes.h"
#include "PRocList.h"
#include "sqlite3.h"
#include "support.h"
#include "mxpad.h"

#include "pcSelection.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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

static	int		callback_select(void *NotUsed, int argc, char **argv, char **azColName)

{
	CPRocList *lastclass = (CPRocList *)NotUsed;

	ASSERT(lastclass->magic == PLIST_MAGIC);

	P2N("CPRocList -> callback_select\r\n");
	
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
// CPRocList dialog


CPRocList::CPRocList(CWnd* pParent /*=NULL*/)
	: CDialog(CPRocList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPRocList)
	m_pcode = _T("");
	m_pcoded = _T("");
	m_proc2 = _T("");
	m_proc2d = _T("");
	m_proc3 = _T("");
	m_proc3d = _T("");
	m_proc4 = _T("");
	m_proc4d = _T("");
	m_proc1d = _T("");
	m_proc1 = _T("");
	m_proc = _T("");
	m_proc1n = _T("");
	m_proc2n = _T("");
	m_proc3n = _T("");
	m_proc4n = _T("");
	m_tid = _T("");
	//}}AFX_DATA_INIT

	dbfile = "events.sqlt";

	magic = PLIST_MAGIC;

	farrsize = 0;
	init_array();

	//windowname = "CPRocList";
}


void CPRocList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPRocList)
	DDX_Control(pDX, IDC_PL_EDIT54, m_tide);
	DDX_Text(pDX, IDC_PL_EDIT44, m_pcode);
	DDX_Text(pDX, IDC_PL_EDIT3, m_pcoded);
	DDX_Text(pDX, IDC_PL_EDIT46, m_proc2);
	DDX_Text(pDX, IDC_PL_EDIT49, m_proc2d);
	DDX_Text(pDX, IDC_PL_EDIT50, m_proc3);
	DDX_Text(pDX, IDC_PL_EDIT51, m_proc3d);
	DDX_Text(pDX, IDC_PL_EDIT52, m_proc4);
	DDX_Text(pDX, IDC_PL_EDIT53, m_proc4d);
	DDX_Text(pDX, IDC_PL_EDIT48, m_proc1d);
	DDX_Text(pDX, IDC_PL_EDIT45, m_proc1);
	DDX_Text(pDX, IDC_PL_EDIT4, m_proc);
	DDX_Text(pDX, IDC_PL_EDIT5, m_proc1n);
	DDX_Text(pDX, IDC_PL_EDIT6, m_proc2n);
	DDX_Text(pDX, IDC_PL_EDIT7, m_proc3n);
	DDX_Text(pDX, IDC_PL_EDIT8, m_proc4n);
	DDX_Text(pDX, IDC_PL_EDIT54, m_tid);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPRocList, CDialog)
	//{{AFX_MSG_MAP(CPRocList)
	ON_BN_CLICKED(IDC_PL_BUTTON2, OnPlButton2)
	ON_BN_CLICKED(IDC_PL_BUTTON23, OnPlButton23)
	ON_BN_CLICKED(IDC_PL_BUTTON30, OnPlButton30)
	ON_BN_CLICKED(IDC_PL_BUTTON24, OnPlButton24)
	ON_BN_CLICKED(IDC_PL_BUTTON25, OnPlButton25)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPRocList message handlers

void CPRocList::OnPlButton2() 

{
	UpdateData();

	CpcSelection pcs;	

	P2N("CEventDlg::OnButtonEd23\r\n");

	if(pcs.DoModal() == IDOK)
		{
		m_pcode  = pcs.diagcode;
		m_pcoded = pcs.description;

		UpdateData(false);
		}
}

void CPRocList::OnPlButton23() 

{
	UpdateData();

	CpcSelection pcs;	

	if(pcs.DoModal() == IDOK)
		{
		m_proc1  = pcs.diagcode;
		m_proc1d = pcs.description;

		UpdateData(false);
		}	
}

void CPRocList::OnPlButton30() 

{
	CpcSelection pcs;	

	if(pcs.DoModal() == IDOK)
		{
		m_proc2  = pcs.diagcode;
		m_proc2d = pcs.description;
		
		UpdateData(false);
		}		
}

void CPRocList::OnPlButton24() 

{
	UpdateData();

	CpcSelection pcs;	

	if(pcs.DoModal() == IDOK)
		{
		m_proc3 = pcs.diagcode;
		m_proc3d = pcs.description;

		UpdateData(false);
		}	
	
}

void CPRocList::OnPlButton25() 

{
	UpdateData();

	CpcSelection pcs;	

	if(pcs.DoModal() == IDOK)
		{
		m_proc4 = pcs.diagcode;
		m_proc4d = pcs.description;

		UpdateData(false);
		}	
	
}

void CPRocList::init_array()

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
	
	farr[farrsize].name =  "ProcCode1";		farr[farrsize].var = &m_proc1;  farrsize++;
	farr[farrsize].name =  "ProcCode1Desc";	farr[farrsize].var = &m_proc1d; farrsize++;
	farr[farrsize].name =  "ProcCode1Note";	farr[farrsize].var = &m_proc1n; farrsize++;

	farr[farrsize].name =  "ProcCode2";		farr[farrsize].var = &m_proc2;  farrsize++;
	farr[farrsize].name =  "ProcCode2Desc";	farr[farrsize].var = &m_proc2d; farrsize++;
	farr[farrsize].name =  "ProcCode2Note";	farr[farrsize].var = &m_proc2n; farrsize++;

	farr[farrsize].name =  "ProcCode3";		farr[farrsize].var = &m_proc3;  farrsize++;
	farr[farrsize].name =  "ProcCode3Desc";	farr[farrsize].var = &m_proc3d; farrsize++;
	farr[farrsize].name =  "ProcCode3Note";	farr[farrsize].var = &m_proc3n; farrsize++;

	farr[farrsize].name =  "ProcCode4";		farr[farrsize].var = &m_proc4;  farrsize++;
	farr[farrsize].name =  "ProcCode4Desc";	farr[farrsize].var = &m_proc4d; farrsize++;
	farr[farrsize].name =  "ProcCode4Note";	farr[farrsize].var = &m_proc4n; farrsize++;

	// This is to make sure that we did not exceed array bounds
	ASSERT(farrsize < sizeof(farr)/sizeof(fields));

}

//////////////////////////////////////////////////////////////////////////

BOOL CPRocList::OnInitDialog() 

{
	CDialog::OnInitDialog();
	
	P2N("CPRocList::OnInitDialog tid= '%s'\r\n", tid);

	m_tid = tid;
	m_tide.aDisable();
	
	SetTimer(1, 100, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPRocList::OnTimer(UINT nIDEvent) 

{
	if(nIDEvent == 1)
		{
		KillTimer(nIDEvent);
		P2N("CPRocList::OnTimer -- Fill timer\r\n");
		LoadData();
		}
	CDialog::OnTimer(nIDEvent);
}

void CPRocList::SaveData()
{

}

void CPRocList::LoadData(int getid)

{
	P2N("CPRocList::LoadData\r\n");

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

	CString tmp; tmp.Format("'%s'", tid);
	qqq += tmp;

	P2N("CPRocList::LoadData qqq=[%s]\r\n", qqq);

	rc = sqlite3_exec(db, qqq, callback_select, this, &zErrMsg);

	if( rc != SQLITE_OK )
		{
		MBOXP("SQL error in %s line %d %s ", "EventdDlg", __LINE__, sqlite3_errmsg(db));
		P2N("SQL error qqq=[%s]\r\n", qqq);
		sqlite3_free(zErrMsg);
		}
	sqlite3_close(db);
}


int	CPRocList::UpdateDB()

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

	P2N("CPRocList::UpdateDB -- updating '%s'\r\n", tid);

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

	P2N("CPRocList::UpdateDB SQL: [%s]\r\n", qqq);

	//P2N("Updated record '%s'\r\n", m_name);

	rc = sqlite3_exec(db, qqq, callback_noop, this, &zErrMsg);

	if( rc != SQLITE_OK )
		{
		MBOXP("SQL error in %s line %d %s ", "EventdDlg", __LINE__, sqlite3_errmsg(db));
		
		//MBOXP("SQL error on '%s' --> %s", qqq, sqlite3_errmsg(db));

		sqlite3_free(zErrMsg);
		}
	
	sqlite3_close(db);

	return 0;
}

//////////////////////////////////////////////////////////////////////////

void CPRocList::OnOK() 

{
	UpdateDB();	
	CDialog::OnOK();
}

void CPRocList::OnCancel() 
{
	UpdateDB();
	CDialog::OnCancel();
}
