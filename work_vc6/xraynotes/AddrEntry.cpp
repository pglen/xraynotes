
/* =====[ AddrEntry1.cpp ]====================================================

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
// AddrEntry1.cpp : implementation file
//

// Just for quick reference:  (modified to extract from farr)

#if 0
	CString qqq =
	'create table address (lastname text, midname text, firstname text, prefix text, suffix text, alias text, 
	updatedate text, tel text, telext text, worktel text, workext text, cell text, cext text, fax text, faxext text, 
	email text, email2 text, note text, address text, city text, zip text, state text, country text, ccode text,
	birthfirst text, birthlast text, birthmiddle text, birthplace text, birthcity text, birthzip text, birthnote text, 
	birthcountry text, birthdate text, birthstate text, bcode text, qnote text, prl text, uidstr text, id text, 
	createdate text, active text)'
#endif

/* -------- System includes:  -------------------------------------------- */

#include "stdafx.h"

#include <io.h>
#include <direct.h>

/* -------- Local includes:  -------------------------------------------- */

#include "mxpad.h"
#include "xraynotes.h"
#include "AddrEntry.h"
#include "support.h"
#include "sqlite3.h"
#include "parse.h"
#include "DateSel.h"
#include "EventDlg.h"
#include "UniqID.h"
#include "msgbox.h"
#include "MedReg.h"
#include "FileDialogST.h"
#include "HistoryDlg.h"
#include "xcalDlg.h"
#include "prl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//static	CAddrEntry *lastclass = NULL;
//static	CString  str;

//////////////////////////////////////////////////////////////////////////
// CAddrEntry dialog

CAddrEntry::CAddrEntry(CWnd* pParent /*=NULL*/)
	: CGrayDlg(CAddrEntry::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddrEntry)
	m_lname = _T("");
	m_email = _T("");
	m_qnote = _T("");
	m_office = _T("");
	m_address = _T("");
	m_city = _T("");
	m_zip = _T("");
	m_tel = _T("");
	m_fax = _T("");
	m_faxext = _T("");
	m_ext = _T("");
	m_post = _T("");
	m_state = _T("");
	m_fname = _T("");
	m_email2 = _T("");
	m_cell = _T("");
	m_cext = _T("");
	m_cid = _T("");
	m_filter = _T("");
	m_uniq = _T("");
	m_cdate = _T("");
	m_prl = _T("");
	m_bplace = _T("");
	m_bcity = _T("");
	m_bstate = _T("");
	m_bcountry = _T("");
	m_bdate = _T("");
	m_bzip = _T("");
	m_bnote = _T("");
	m_bcode = _T("");
	m_country = _T("");
	m_ccode = _T("");
	m_udate = _T("");
	m_wtel = _T("");
	m_wext = _T("");
	m_blast = _T("");
	m_bfirst = _T("");
	m_mname = _T("");
	m_bmname = _T("");
	m_note = _T("");
	m_findbox = _T("");
	m_count = _T("");
	m_active = FALSE;
	m_suffix = _T("");
	m_prefix = _T("");
	m_alias = _T("");
	//}}AFX_DATA_INIT

	farrsize = rows = count = 0;
	first = rescan = filling = false;
	
	PleaseSel = "Please select a Patient Entry First.";
	init_array();

	m_centerdlg = true;
	windowname = "CAddrEntry";
	//SetAlpha(245);
}

//////////////////////////////////////////////////////////////////////////

CAddrEntry::~CAddrEntry()

{
	delete pImageList;
}

//////////////////////////////////////////////////////////////////////////

void CAddrEntry::DoDataExchange(CDataExchange* pDX)

{
	CGrayDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddrEntry)
	DDX_Control(pDX, IDC_AB_BUTTON15, m_b15);
	DDX_Control(pDX, IDC_AB_BUTTON3, m_b3);
	DDX_Control(pDX, IDC_AB_BUTTON14, m_hist);
	DDX_Control(pDX, IDC_AB_EDIT6, m_ealias);
	DDX_Control(pDX, IDC_AB_EDIT9, m_efname);
	DDX_Control(pDX, IDC_AB_EDIT8, m_ecity);
	DDX_Control(pDX, IDC_AB_EDIT7, m_eaddress);
	DDX_Control(pDX, IDC_AB_EDIT5, m_eqnote);
	DDX_Control(pDX, IDC_AB_EDIT26, m_eemail2);
	DDX_Control(pDX, IDC_AB_EDIT25, m_ecext);
	DDX_Control(pDX, IDC_AB_EDIT24, m_ecell);
	DDX_Control(pDX, IDC_AB_EDIT22, m_efaxext);
	DDX_Control(pDX, IDC_AB_EDIT21, m_eext);
	DDX_Control(pDX, IDC_AB_EDIT20, m_efax);
	DDX_Control(pDX, IDC_AB_EDIT2, m_eemail);
	DDX_Control(pDX, IDC_AB_OK6, m_ok6);
	DDX_Control(pDX, ID_AB_OK8, m_ok8);
	DDX_Control(pDX, ID_AB_OK7, m_ok7);
	DDX_Control(pDX, IDC_AB_OK3, m_ok3);
	DDX_Control(pDX, IDC_AB_BUTTON6, m_b6);
	DDX_Control(pDX, IDC_AB_BUTTON30, m_b30);
	DDX_Control(pDX, IDC_AB_BUTTON13, m_b13);
	DDX_Control(pDX, IDC_AB_BUTTON12, m_b12);
	DDX_Control(pDX, IDC_AB_EDIT4, m_esuffix);
	DDX_Control(pDX, IDC_AB_EDIT3, m_eprefix);
	DDX_Control(pDX, IDC_AB_EDIT15, m_ed15);
	DDX_Control(pDX, IDC_AB_OK5, m_ok5);
	DDX_Control(pDX, IDC_AB_EDIT1, m_elname);
	DDX_Control(pDX, IDC_AB_OK4, m_ok4);
	DDX_Control(pDX, IDC_AB_BUTTON9, m_b9);
	DDX_Control(pDX, IDC_AB_BUTTON8, m_b8);
	DDX_Control(pDX, IDC_AB_BUTTON29, m_b29);
	DDX_Control(pDX, IDC_AB_BUTTON10, m_b10);
	DDX_Control(pDX, IDC_AB_EDIT30, m_ebcity);
	DDX_Control(pDX, IDC_AB_EDIT12, m_ecdate);
	DDX_Control(pDX, IDC_AB_EDIT17, m_ecid);
	DDX_Control(pDX, IDC_AB_EDIT33, m_ebcountry);
	DDX_Control(pDX, IDC_AB_EDIT32, m_ebzip);
	DDX_Control(pDX, IDC_AB_EDIT41, m_ebfirst);
	DDX_Control(pDX, IDC_AB_EDIT11, m_eblast);
	DDX_Control(pDX, IDC_AB_EDIT55, m_enote);
	DDX_Control(pDX, IDC_AB_EDIT23, m_ebnote);
	DDX_Control(pDX, IDC_AB_EDIT43, m_ebmname);
	DDX_Control(pDX, IDC_AB_EDIT42, m_emname);
	DDX_Control(pDX, IDC_AB_EDIT29, m_ebdate);
	DDX_Control(pDX, IDC_AB_EDIT28, m_ebplace);
	DDX_Control(pDX, IDC_AB_EDIT31, m_ebstate);
	DDX_Control(pDX, IDC_AB_BUTTON22, m_b22);
	DDX_Control(pDX, IDC_AB_BUTTON11, m_b11);
	DDX_Control(pDX, IDC_AB_BUTTON2, m_b2);
	DDX_Control(pDX, IDC_AB_BUTTON5, m_b5);
	DDX_Control(pDX, IDCANCEL, m_can);
	DDX_Control(pDX, IDC_AB_OK2, m_ok2);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDC_AB_EDIT37, m_eprl);
	DDX_Control(pDX, IDC_AB_EDIT38, m_euniq);
	DDX_Control(pDX, IDC_AB_TREE1, m_tree);
	DDX_Text(pDX, IDC_AB_EDIT1, m_lname);
	DDX_Text(pDX, IDC_AB_EDIT2, m_email);
	DDX_Text(pDX, IDC_AB_EDIT5, m_qnote);
	DDX_Text(pDX, IDC_AB_EDIT7, m_address);
	DDX_Text(pDX, IDC_AB_EDIT8, m_city);
	DDX_Control(pDX, IDC_AB_EDIT18,		m_ezip);
	DDX_Text(pDX, IDC_AB_EDIT18, m_zip);
	DDX_Control(pDX, IDC_AB_EDIT19, 	m_etel);
	DDX_Text(pDX, IDC_AB_EDIT19, m_tel);
	DDX_Text(pDX, IDC_AB_EDIT20, m_fax);
	DDX_Text(pDX, IDC_AB_EDIT22, m_faxext);
	DDX_Text(pDX, IDC_AB_EDIT21, m_ext);
	DDX_Control(pDX, IDC_AB_EDIT10, 	m_estate);
	DDX_Text(pDX, IDC_AB_EDIT10, m_state);
	DDX_Text(pDX, IDC_AB_EDIT9, m_fname);
	DDX_Text(pDX, IDC_AB_EDIT26, m_email2);
	DDX_Text(pDX, IDC_AB_EDIT24, m_cell);
	DDX_Text(pDX, IDC_AB_EDIT25, m_cext);
	DDX_Text(pDX, IDC_AB_EDIT17, m_cid);
	DDX_Text(pDX, IDC_AB_EDIT14, m_filter);
	DDX_Text(pDX, IDC_AB_EDIT38, m_uniq);
	DDX_Text(pDX, IDC_AB_EDIT12, m_cdate);
	DDX_Text(pDX, IDC_AB_EDIT37, m_prl);
	DDX_Text(pDX, IDC_AB_EDIT28, m_bplace);
	DDX_Text(pDX, IDC_AB_EDIT30, m_bcity);
	DDX_Text(pDX, IDC_AB_EDIT31, m_bstate);
	DDX_Text(pDX, IDC_AB_EDIT33, m_bcountry);
	DDX_Text(pDX, IDC_AB_EDIT29, m_bdate);
	DDX_Text(pDX, IDC_AB_EDIT32, m_bzip);
	DDX_Text(pDX, IDC_AB_EDIT23, m_bnote);
	DDX_Control(pDX, IDC_AB_EDIT34, m_ebcode);
	DDX_Text(pDX, IDC_AB_EDIT34, m_bcode);
	DDX_Control(pDX, IDC_AB_EDIT35, 	m_ecountry);
	DDX_Text(pDX, IDC_AB_EDIT35, m_country);
	DDX_Control(pDX, IDC_AB_EDIT36, 	m_eccode);
	DDX_Text(pDX, IDC_AB_EDIT36, m_ccode);
	DDX_Control(pDX, IDC_AB_EDIT13, m_eudate);
	DDX_Text(pDX, IDC_AB_EDIT13, m_udate);
	DDX_Control(pDX, IDC_AB_EDIT39, 	m_ewtel);   
	DDX_Text(pDX, IDC_AB_EDIT39, m_wtel);
	DDX_Text(pDX, IDC_AB_EDIT40, m_wext);
	DDX_Control(pDX, IDC_AB_EDIT40, 	m_ewext);
	DDX_Text(pDX, IDC_AB_EDIT11, m_blast);
	DDX_Text(pDX, IDC_AB_EDIT41, m_bfirst);
	DDX_Text(pDX, IDC_AB_EDIT42, m_mname);
	DDX_Text(pDX, IDC_AB_EDIT43, m_bmname);
	DDX_Text(pDX, IDC_AB_EDIT55, m_note);
	DDX_Text(pDX, IDC_AB_EDIT27, m_findbox);
	DDX_Text(pDX, IDC_AB_EDIT15, m_count);
	DDX_Check(pDX, IDC_AB_CHECK2, m_active);
	DDX_Text(pDX, IDC_AB_EDIT4, m_suffix);
	DDX_Text(pDX, IDC_AB_EDIT3, m_prefix);
	DDX_Text(pDX, IDC_AB_EDIT6, m_alias);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAddrEntry, CGrayDlg)
	//{{AFX_MSG_MAP(CAddrEntry)
	//ON_BN_CLICKED(IDC_AB_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_AB_BUTTON2, OnButton2)
	ON_NOTIFY(TVN_SELCHANGED, IDC_AB_TREE1, OnSelchangedTree1)
	ON_BN_CLICKED(IDC_AB_BUTTON5, OnButton5)
	ON_NOTIFY(TVN_SELCHANGING, IDC_AB_TREE1, OnSelchangingTree1)
	ON_NOTIFY(NM_DBLCLK, IDC_AB_TREE1, OnDblclkTree1)
	ON_BN_CLICKED(IDC_AB_BUTTON8, OnButton8)
	ON_BN_CLICKED(IDC_AB_BUTTON9, OnButton9)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_AB_BUTTON10, OnButton10)
	ON_BN_CLICKED(IDC_AB_BUTTON11, OnButton11)
	ON_EN_CHANGE(IDC_AB_EDIT14, OnChangeEdit14)
	//ON_BN_CLICKED(IDOK2, OnOk2)
	ON_WM_KEYDOWN()
	ON_BN_CLICKED(IDC_AB_BUTTON29, OnButton29)
	ON_BN_CLICKED(IDC_AB_BUTTON22, OnButton22)
	ON_EN_KILLFOCUS(IDC_AB_EDIT23, OnKillfocusEdit23)
	ON_EN_CHANGE(IDC_AB_EDIT1, OnChangeEdit1)
	ON_WM_KILLFOCUS()
	ON_BN_CLICKED(IDC_AB_CHECK1, OnCheck1)
	ON_EN_KILLFOCUS(IDC_AB_EDIT9, OnKillfocusAbEdit9)
	ON_BN_CLICKED(IDC_AB_BUTTON12, OnAbButton12)
	ON_NOTIFY(NM_SETFOCUS, IDC_AB_TREE1, OnSetfocusAbTree1)
	//ON_BN_CLICKED(IDOK3, OnOk3)
	ON_BN_CLICKED(IDC_AB_CHECK3, OnCheck3)
	ON_BN_CLICKED(IDC_AB_OK2, OnAbOk2)
	ON_BN_CLICKED(IDC_AB_OK5, OnAbOk5)
	ON_BN_CLICKED(IDC_AB_OK4, OnAbOk4)
	ON_BN_CLICKED(ID_AB_OK7, OnAbOk7)
	ON_BN_CLICKED(ID_AB_OK8, OnAbOk8)
	ON_BN_CLICKED(IDC_AB_BUTTON30, OnAbButton30)
	ON_BN_CLICKED(IDC_AB_BUTTON13, OnAbButton13)
	ON_BN_CLICKED(IDC_AB_OK6, OnAbOk6)
	ON_BN_CLICKED(IDC_AB_CHECK4, OnAbCheck4)
	ON_BN_CLICKED(IDC_AB_CHECK1, OnAbCheck1)
	ON_WM_SYSKEYDOWN()
	ON_BN_CLICKED(IDC_AB_BUTTON14, OnAbButton14)
	ON_BN_CLICKED(IDC_AB_BUTTON3, OnAbButton3)
	ON_BN_CLICKED(IDC_AB_BUTTON15, OnAbButton15)
	ON_BN_CLICKED(IDC_AB_BUTTON6, OnAbButton6)
	ON_NOTIFY(NM_RCLICK, IDC_AB_TREE1, OnRclickAbTree1)
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_TREERCLICK_MEDICALEVENTS, OnButton8)
	ON_COMMAND(ID_TREERCLICK_TRANSACTIONREGISTRY, OnAbButton3)
	ON_COMMAND(ID_TREERCLICK_DELETEITEM, OnButton5)
	ON_COMMAND(ID_TREERCLICK_PATIENTCALENDAR, OnAbButton6)
	ON_COMMAND(ID_TREERCLICK_DATAHISTORY, OnAbButton14)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

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

static CString scemastr;

static int callback_scema(void *NotUsed, int argc, char **argv, char **azColName)

{
	if(scemastr != "")
		{
		scemastr += ", ";
		}
	scemastr += argv[1];

	return 0;
}

static int callback_export(void *NotUsed, int argc, char **argv, char **azColName)

{
	CString str;
	CArchive *ar = (CArchive *)NotUsed;

	// Accumulate arguments
	for(int ii=0; ii < argc; ii++)
		{
		CString str2 = ", ";
		if(argv[ii] != NULL)
			{
			str2 = argv[ii];
			str2 += ", ";
			}
		str += str2;
		}

	str += "\r\n";
	P2N("Dump %s\r\n", str);
	ar->WriteString(str);

	return 0;
}

//////////////////////////////////////////////////////////////////////////

static int callback_fill(void *NotUsed, int argc, char **argv, char **azColName)

{
	CString str;
	CAddrEntry *lastclass = (CAddrEntry *)NotUsed;

	//P2N("callback_fill\r\n");

	ASSERT(argc == 4);

	str = argv[0];	str += ", "; 
	str += argv[1]; str += ", "; 
	str += argv[2];

	HTREEITEM item = lastclass->m_tree.InsertItem(str, 1, 0);

	DWORD id = atoi(argv[3]);
	lastclass->m_tree.SetItemData(item, id);

	lastclass->m_count.Format("%d", atoi(lastclass->m_count) + 1);
	lastclass->UpdateData(false);
	//P2N("Got item id: %s %d\r\n", argv[3], id);

	return 0;
}

//////////////////////////////////////////////////////////////////////////

static	int		callback_select(void *NotUsed, int argc, char **argv, char **azColName)

{
	CString  tmp, ttmp;

	CAddrEntry *lastclass = (CAddrEntry *)NotUsed;

	//P2N("callback_select\r\n");
	//P2N("Lastclass %p\r\n", lastclass);

	lastclass->rows++;

	for(int ii=0; ii < argc; ii++)
		{
		ttmp.Format("%s=%s  ",  azColName[ii], argv[ii] ? argv[ii] : "NULL");

		if(ii >= lastclass->farrsize)
			{
			//P2N("Overflow into flags %s\r\n", argv[ii]);
				
			if(argv[ii])
				{
				*lastclass->flarr[ii - lastclass->farrsize].var   = atoi(argv[ii]);
				lastclass->flarr[ii - lastclass->farrsize].value  = atoi(argv[ii]);
				}
			else
				{
				*lastclass->flarr[ii - lastclass->farrsize].var   = 0;
				lastclass->flarr[ii - lastclass->farrsize].value  = 0;
				}
			}
		else
			{
			if(argv[ii])
				{
				*lastclass->farr[ii].var   = argv[ii];
				lastclass->farr[ii].value  = argv[ii];
				}
			else
				{
				lastclass->farr[ii].value = "";
				}
			tmp += ttmp;
			}
		}

  tmp += "\r\n\r\n";
  lastclass->sumstr += tmp;

  return 0;
}


//////////////////////////////////////////////////////////////////////////

static int		callback3(void *NotUsed, int argc, char **argv, char **azColName)

{
	CString  tmp, ttmp;

	CAddrEntry *lastclass = (CAddrEntry *)NotUsed;

	//P2N("Callback3\r\n");

	lastclass->rows ++;
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

	CAddrEntry *lastclass = (CAddrEntry *)NotUsed;

	//P2N("Callback4\r\n");

	for(int ii=0; ii < argc; ii++)
		{
		if(argv[ii])
			str = argv[ii];
		else
			str = "";

		str = support.EscapeComma(str);
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
	CAddrEntry *lastclass = (CAddrEntry *)NotUsed;

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
	CGrayDlg::OnInitDialog();

	m_title = "Person Demographics";
	
	dbfile   = dataroot + "db\\address.sqlt";
	dbfile2  = dataroot + "db\\address_history.sqlt";

	//dbfile = abook_root + "address.sqlt";

	// Init the data array
	//memset(farr, 0, sizeof(farr));

	LoadToolTips();

	//////////////////////////////////////////////////////////////////////
	// Button tooltips

	m_ok4.SetToolTipText("Copy 'Names' fields to 'Birth Names' fields");

	//////////////////////////////////////////////////////////////////////
	CBitmap	 bitmap;

	pImageList = new CImageList();
	pImageList->Create(16, 16, ILC_COLOR8, 3, 4);

	bitmap.LoadBitmap(IDB_AB_BITMAP1);
	pImageList->Add(&bitmap, &bitmap);
	bitmap.DeleteObject();

	bitmap.LoadBitmap(IDB_AB_BITMAP2);
	pImageList->Add(&bitmap, &bitmap);
	bitmap.DeleteObject();

	bitmap.LoadBitmap(IDB_AB_BITMAP2);
	pImageList->Add(&bitmap, &bitmap);
	bitmap.DeleteObject();

	m_tree.SetImageList(pImageList, TVSIL_NORMAL);

	//////////////////////////////////////////////////////////////////////
	// Disable uneditables

	m_eprl.Disable();
	m_euniq.Disable();
	m_ed15.Disable();
	m_ecid.Disable();
	m_ecdate.Disable();
	m_eudate.Disable();

	// Mark default disabled
	for(int ii=0; ii < farrsize ; ii++)
		{
		if(farr[ii].ctrl != NULL)
			{
			//P2N("loop %d disabled %d\r\n", ii, farr[ii].ctrl->disabled);
			farr[ii].olddis = farr[ii].ctrl->disabled;
			}
		}

	// Disable All
	for(ii=0; ii < farrsize ; ii++)
		{
		if(farr[ii].ctrl != NULL)
			{
			//P2N("About to dissable %s\r\n", farr[ii].name);
			farr[ii].ctrl->Disable();
			}
		}

	SetFocus();

	//YieldToWinEx();

	SetTimer(1, 300, NULL);

	((CButton*)GetDlgItem(IDC_AB_CHECK1))->SetCheck(true);

	//((CButton*)GetDlgItem(IDC_CHECK2))->SetCheck(true);
	//Preload tree's right click menu
	//hrcmenu.LoadMenu(IDR_ADDR_MENU);
	//hrclickmenu = (BCMenu *)hrcmenu.GetSubMenu(0);
	//hrclickmenu->InsertMenu(0,  MF_BYPOSITION  | MF_STRING, 1000, "Name of Patient");


	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

void CAddrEntry::Fill(const char *str, int find)

{
	rescan = 0;
	filling = true;

	m_count = "0";

	//P2N("Filling '%s'\r\n", str);

	// Preserve selection
	//HTREEITEM old = m_tree.GetSelectedItem();

	m_tree.DeleteAllItems();
	m_tree.Invalidate();

	int active = ((CButton*)GetDlgItem(IDC_AB_CHECK3))->GetCheck();

	//P2N("CAddrEntry::Fill Active %d\r\n", active);

	// Clear all fields
	for(int ii = 0; ii <  farrsize; ii++)
		{
		farr[ii].value = "";
		*farr[ii].var = "";
		}

	BeginWaitCursor();

	//YieldToWinEx();

	CString qqq = "select lastname, midname, firstname, id from address ";

	int filtered = 0;

	if (command != "")
		{
		CString comm; 

		//MBOX("command");

		comm.Format("where uidstr = '%s'", command);
		qqq += comm;
		}
	else if(str != NULL)
		{
		if(strlen(str))
			{
			CString filter; 

			if(find)
				{
				filter.Format(	"where   (lastname like '%%%s%%') or "
										"(firstname like '%%%s%%') or "
										"(tel like '%%%s%%') or "
										"(email like '%%%s%%') or "
										"(city  like '%%%s%%') or "
										"(state like '%%%s%%') or "
										"(address like '%%%s%%') "
										, str, str, str, str, str, str, str);
				}
			else
				{
				filter.Format("where lastname like '%%%s%%' ",  str);
				}	

			filtered = true;
			qqq += filter;
			}
		}

	if(active)
		{
		if(filtered)
			qqq += " and active = 1 ";
		else
			qqq += " where active = 1 ";
		}

	qqq += " order by updatedate desc";

	P2N("AddrEntry::Fill qqq=%s\r\n", qqq);

	//rc = sqlite3_exec(db, qqq, callback_fill, this, &zErrMsg);
	//if( rc != SQLITE_OK )
	//	{
	//	MBOXP("SQL error: %s", sqlite3_errmsg(db));
	//	sqlite3_free(zErrMsg);
	//	}
	//
	//	sqlite3_close(db);

	 ExecSql(dbfile, qqq, callback_fill);

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

	filling = false;

	// Select entry if command is passed
	if (command != "")
		{
		m_tree.Select(m_tree.GetNextItem(NULL, TVGN_CHILD), TVGN_CARET);
		}
}

//////////////////////////////////////////////////////////////////////////

void CAddrEntry::OnOK()

{
	// See if prev focus was on filter

	HTREEITEM hItem  = m_tree.GetSelectedItem();
	if(hItem)
		{
		UpdateDB(m_tree.GetItemData(hItem));
		m_currp = m_tree.GetItemText(hItem);
		}
	CGrayDlg::OnOK();
}

//////////////////////////////////////////////////////////////////////////

void CAddrEntry::OnButton1()

{
	HTREEITEM hItem  = m_tree.GetSelectedItem();
	if(hItem)
		{
		UpdateDB(m_tree.GetItemData(hItem));
		}

	sumstr = "";
	rows = 0;
	CString qqq("insert into address (lname, fname) values('");

	qqq +=  "New', '";
	qqq +=  "Entry')";

	int rowid = ExecSql(dbfile, qqq, callback_select);

	Fill();

	SelEntryInt(rowid);

	GetDlgItem(IDC_AB_EDIT1)->SetFocus();
	((CEdit *)GetDlgItem(IDC_AB_EDIT1))->SetSel(0, -1);

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
		xMsgBox("cannot open SQL");
		sqlite3_close(db);
		return;
		//exit(1);
		}

	//str = "";
	sumstr = ""; rows = 0;

	CString qqq = "select ";
	
	// Fill in names of fields
	for(int ii = 0; ii <  farrsize; ii++)
		{
		if(ii > 0)
			qqq += ", ";
		qqq += farr[ii].name;
		}

	// Fill in names of flags
	for(int iii = 0; iii < flarrsize; iii++)
		{
		qqq += ", ";
		qqq += flarr[iii].name;
		}

	qqq += " from address where id = ";

	CString tmp; tmp.Format("%d", getid);
	qqq += tmp;

	//P2N("CAddrEntry::Qdata qqq=%s\r\n", qqq);

	rc = sqlite3_exec(db, qqq, callback_select, this, &zErrMsg);

	if( rc != SQLITE_OK )
		{
		MBOXP("SQL err: %d %s", rc, sqlite3_errmsg(db));
		sqlite3_free(zErrMsg);
		}

	sqlite3_close(db);

	//P2N("m_uniq = %s\r\n", m_uniq);

	//if(m_uniq == "")
	//	{
	//	CUniqID uid;
	//	m_uniq = uid.GetStr();
	//	}

	UpdateData(false);
}


//////////////////////////////////////////////////////////////////////////

void CAddrEntry::OnButton2()

{
	FlushDB();
	Fill(m_filter);
}

//////////////////////////////////////////////////////////////////////////

void CAddrEntry::OnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult)

{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	*pResult = 0;

	//P2N("CAddrEntry::OnSelchangedTree1\r\n");

	HTREEITEM hItem  = m_tree.GetSelectedItem();

	if(!hItem)
		{
		//P2N("Changed tree no selection\r\n");
		return;
		}

	FillOne();
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
		{
		MBOX(PleaseSel);
		return;
		}

	int item = m_tree.GetItemData(hItem);
	str =  m_tree.GetItemText(hItem);

	// Select next one when deleting
	HTREEITEM item3 = m_tree.GetNextItem(hItem, TVGN_NEXT);

	if(str == "")
		{
		MBOX(PleaseSel);
		return;
		}

	CString str2;
	if(item3)
		str2 = m_tree.GetItemText(item3);

	CString tmpbox;
	tmpbox.Format("Deleting Patient. This action will \r\nAre you sure you want to delete:\r\n\r\n'%s' ?", str);

	if(xMsgBox(tmpbox, "Patient Delete Prompt", MBTYPE_YESNO, DEFNO) != MBOXYES)
		return ;

	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	rc = sqlite3_open(dbfile, &db);
	if( rc )
		{
		MBOXP("cannot open SQL %s", dbfile);
		sqlite3_close(db);
		return;
		}

	CString qqq;
	qqq.Format("delete from address where id==%d", item);
	P2N("Calling sqlite select\r\n");
	rc = sqlite3_exec(db, qqq, callback_noop, this, &zErrMsg);

	if( rc != SQLITE_OK )
		{
		MBOXP("SQL error: %s", sqlite3_errmsg(db));
		sqlite3_free(zErrMsg);
		}
	sqlite3_close(db);

	// Clear all fields
	for(int ii=0; ii < farrsize ; ii++)
		{
		farr[ii ].value = "";
		*farr[ii ].var = "";
		}

	UpdateData(false);

	Fill(m_filter);

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

void CAddrEntry::SelEntryInt(int num, int focus)

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
			if(focus)
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
	
	//P2N("CAddrEntry::OnSelchangingTree1\r\n");

	if(filling)
		{
		//P2N("Not filling\r\n");		
		return;
		}

	HTREEITEM hItem  = m_tree.GetSelectedItem();

	if(!hItem)
		{	
		//P2N("No selection\r\n");		
		return;
		}

	CString str =  m_tree.GetItemText(hItem);
	str = m_lname + ", " + m_mname + ", " + m_fname;
	m_tree.SetItemText(hItem, str);

	rescan = UpdateDB(m_tree.GetItemData(hItem));
}

//////////////////////////////////////////////////////////////////////////

int		CAddrEntry::UpdateDB(int id)

{
	UpdateData();

	if(!IsDataChanged())
		return 1;

	FillPRL();

	CTime ct = CTime::GetCurrentTime();
	CString datestr = ct.Format("%Y/%m/%d %H:%M:%S");

	m_udate = datestr;
	UpdateData(false);

	sqlite3 	*db;
	char 		*zErrMsg = 0;
	int 		rc;

	//////////////////////////////////////////////////////////////////////
	// Backup old data first

	rc = sqlite3_open(dbfile2, &db);

	if( rc )
		{
		MBOXP("Cannot open SQL %s", dbfile);
		sqlite3_close(db);
		return 0;
		}

	CString tmp; tmp.Format("%d", id);
	CString qqq("insert into address ( ");

	// Fill in names of fields
	for(int iii=0; iii < farrsize; iii++)
		{
		if(iii > 0)
			qqq += ", ";

		qqq += farr[iii].name;
		}

	// Fill in names of flags
	for(iii = 0; iii < flarrsize; iii++)
		{
		qqq += ", ";
		qqq += flarr[iii].name + " ";
		}

	qqq += ") values (";
	
	// Fill in values of fields
	for(iii=0; iii < farrsize; iii++)
		{
		farr[iii].value2 = support.EscapeSql(farr[iii].value);

		if(iii > 0)
			qqq += ", ";

		qqq +=  "'"  + farr[iii].value2 + "'";
		}

	// Fill in names of flags
	for(iii = 0; iii < flarrsize; iii++)
		{
		CString ttt; ttt.Format("%d", flarr[iii].value);
		qqq +=  ", ";
		qqq +=  ttt + " ";
		}

	qqq += ")";

	//qqq += " where id = " + tmp;
	//MBOXP("SQL: '%s'", qqq);
	//P2N("CAddrEntry::UpdateDB backup\r\nqqq=[%s]\r\n", qqq);
	//P2N("Updated record '%s' '%s'\r\n", m_lname, m_fname);
	
	rc = sqlite3_exec(db, qqq, callback_noop, this, &zErrMsg);

	if( rc != SQLITE_OK )
		{
		P2N("SQL error %s\r\n%s\r\n", dbfile2, sqlite3_errmsg(db));
		MBOXP("SQL error %s\r\n%s", dbfile2, sqlite3_errmsg(db));
		sqlite3_free(zErrMsg);
		}
	sqlite3_close(db);
		
	//////////////////////////////////////////////////////////////////////
	// Save real data as well

	rc = sqlite3_open(dbfile, &db);

	if( rc )
		{
		MBOXP("Cannot open SQL %s", dbfile);
		sqlite3_close(db);
		return 0;
		}

	CString tmp2; tmp2.Format("%d", id);
	CString qqq2("update address set ");

	// Fill in names/values of fields
	for(int iiii=0; iiii < farrsize; iiii++)
		{
		farr[iiii].var2 = support.EscapeSql(*farr[iiii].var);

		if(farr[iiii].name != "id")
			{
			if(iiii > 0)
				qqq2 += ", ";

			qqq2 += farr[iiii].name + " =  '"  + farr[iiii].var2 + "'";
			}
		}

	// Fill in names of flags
	for(iiii = 0; iiii < flarrsize; iiii++)
		{
		CString ttt; ttt.Format("%d", *flarr[iiii].var);
		qqq2 += ", ";
		qqq2 += flarr[iiii].name + " = " + ttt + " ";
		}

	qqq2 += " where id = " + tmp2;

	//MBOXP("SQL: '%s'", qqq);
	//P2N("CAddrEntry::UpdateDB %s\r\nqqq=[%s]\r\n", dbfile, qqq2);

	P2N("Updated record '%s' '%s'\r\n", m_lname, m_fname);

	rc = sqlite3_exec(db, qqq2, callback_noop, this, &zErrMsg);

	if( rc != SQLITE_OK )
		{
		P2N("SQL error on %s\r\n%s\r\n", dbfile, sqlite3_errmsg(db));
		MBOXP("SQL error on %s\r\n%s", dbfile, sqlite3_errmsg(db));
		sqlite3_free(zErrMsg);
		}
	sqlite3_close(db);

	//////////////////////////////////////////////////////////////////////
	// If title changed, rewrite it
	HTREEITEM hItem  = m_tree.GetSelectedItem();
	CString str = m_lname + ", " + m_mname + ", " + m_fname;

	m_tree.SetItemText(hItem, str);

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
	CGrayDlg::OnCancel();
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

	str = support.EscapeSql(str);
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
		xMsgBox("cannot open SQL");
		sqlite3_close(db);
		}


	str = "";	sumstr = "";

	rows = 0;

	CString qqq("insert into address (lname, fname, email) values('");

	if(parr.GetSize() > 0)

	qqq +=  rr + "', '";
	qqq +=  ll + "', '";

	if(parr.GetSize() > 1)
		qqq +=  *((CString *)parr.GetAt(1)) + "')";

	P2N("SQL=%s\r\n", qqq);

	rc = sqlite3_exec(db, qqq, callback_select, this, &zErrMsg);

	if( rc != SQLITE_OK )
		{
		MBOXP("SQL error: %s", sqlite3_errmsg(db));
		sqlite3_free(zErrMsg);
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

	str = support.EscapeSql(str);
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
		xMsgBox("cannot open SQL");
		sqlite3_close(db);
		}


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

	rc = sqlite3_exec(db, qqq, callback_select, this, &zErrMsg);

	if( rc != SQLITE_OK )
		{
		MBOXP("SQL error: %s", sqlite3_errmsg(db));
		sqlite3_free(zErrMsg);
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

	str = support.EscapeSql(str);

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
		xMsgBox("cannot open SQL");
		sqlite3_close(db);
		}


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

	rc = sqlite3_exec(db, qqq, callback3, this, &zErrMsg);

	if( rc != SQLITE_OK )
		{
		MBOXP("SQL error: %s", sqlite3_errmsg(db));
		sqlite3_free(zErrMsg);
		return;
		}

	int rowid = (int)sqlite3_last_insert_rowid(db);
	sqlite3_close(db);

	FreeParseStr(&parr);
}

//////////////////////////////////////////////////////////////////////////
// Import

#if 0

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
	//dlgFile.m_ofn.lpstrInitialDir = str;

	BOOL bRet = (dlgFile.DoModal() == IDOK) ? TRUE : FALSE;
	fileName.ReleaseBuffer();

	if(!bRet)
		return;

	CFileException fileException;
	CFile bfp;

	if (!bfp.Open(fileName, CFile::modeRead, &fileException))
		{
		MBOXP("Cannot open import file %s", fileName);
		//P2N("Cannot open import file\r\n", fileName);
		return;
		}

	CString str;
	char chh;
	while(true)
		{
		if(!bfp.Read(&chh, 1))
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

	bfp.Close();

	Fill(m_filter);

	//SelEntryInt(rowid);
	//GetDlgItem(IDC_EDIT1)->SetFocus();
	//((CEdit *)GetDlgItem(IDC_EDIT1))->SetSel(0, -1);
	//UpdateData(false);
}

#endif

#if 0
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
	//dlgFile.m_ofn.lpstrInitialDir = str;

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



	// Query the database for all
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	first = false;

	rc = sqlite3_open(dbfile, &db);

	if( rc )
		{
		MBOXP("SQL error: %s %s", sqlite3_errmsg(db), dbfile);
		sqlite3_close(db);
		}

	rc = sqlite3_exec(db, "select "
		"lname, fname, email, prefix, post, email2, qnote, office, address, city, "
				"zip, state, tel, ext, fax, faxext, cell, cext, note "
				" from address", callback4, this, &zErrMsg);
	if( rc != SQLITE_OK )
		{
		MBOXP("SQL error: %s", sqlite3_errmsg(db));
		sqlite3_free(zErrMsg);
		}

	if( rc != SQLITE_OK )
		{
		//?????
		MBOXP("SQL error: %s", sqlite3_errmsg(db));

		}
	sqlite3_close(db);

	filehand.Close();
}

#endif

//////////////////////////////////////////////////////////////////////////
// Event Form

void CAddrEntry::OnButton8()

{
	// Save First
	FlushDB();

	if(m_tree.GetSelectedItem() == NULL)
		{
		MBOX(PleaseSel);
		return;
		}

	if(m_prl == "")
		{
		xMsgBox("Medical Event details are only present if the entry has a valid PRL.\r\n"
			"If it is a new entry, please fill in birth parameters first.");

		return;
		}

	//P2N("prl: %s\r\n", m_prl);
	CEventDlg ed;

	CString olddir = support.GetCurrentDir();

	ed.header.Format("Mediacal event details for '%s %s %s'", m_lname, m_mname, m_fname);
	ed.m_prl = m_prl;
	ed.m_patid = m_uniq;
	ed.DoModal();

	_chdir(olddir);
}

//////////////////////////////////////////////////////////////////////////

void CAddrEntry::OnButton9()

{
	// Save First
	FlushDB();

	CString approot; support.GetAppRoot(approot);
	CString str, path("umanual\\"), file("abook.html");

#ifdef _DEBUG
	str = "Z:/archive/xraynotes/umanual/" + file;
#else
	str =  approot + path + file;
#endif

	ShellExecute(NULL, "open", str, NULL, NULL, SW_SHOWMAXIMIZED);

#if 0
	HTREEITEM hItem  = m_tree.GetSelectedItem();
	if(hItem)
		{
		UpdateDB(m_tree.GetItemData(hItem));
		}

	CString tmpbox;
	tmpbox.Format("This will delete duplicate records. Continue?");

	if(MBOX(tmpbox, "", MB_YESNO) != IDYES)
		return ;

	CString title;
	GetWindowText(title);

	BeginWaitCursor();

	sqlite3 *db;
	char *zErrMsg = 0;
	int rc, loop;

	rc = sqlite3_open(dbfile, &db);

	if( rc )
		{
		MBOXP("SQL error: %s %s", sqlite3_errmsg(db), dbfile);
		sqlite3_close(db);
		}

	old_lname = "";  old_fname = ""; old_email = "";
	dwarr.RemoveAll();

	rc = sqlite3_exec(db, "select lname, fname, email, id from address order by lname, fname, email",
								callback5, this, &zErrMsg);

	if( rc != SQLITE_OK )
		{
		MBOXP("SQL error: %s", sqlite3_errmsg(db));
		sqlite3_free(zErrMsg);
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

		rc = sqlite3_exec(db, qqq, callback_select, this, &zErrMsg);

		if( rc != SQLITE_OK )
			{
			MBOXP("SQL error: %s", sqlite3_errmsg(db));
			sqlite3_free(zErrMsg);
			}
		}

	dwarr.RemoveAll();
	sqlite3_close(db);

	SetWindowText(title);
	EndWaitCursor();

	Fill(m_filter);

#endif
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

#if 0

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
	//dlgFile.m_ofn.lpstrInitialDir = str;

	BOOL bRet = (dlgFile.DoModal() == IDOK) ? TRUE : FALSE;
	fileName.ReleaseBuffer();

	if(!bRet)
		return;

	CFileException fileException;
	CFile bfp;

	BeginWaitCursor();

	CString title2;
	GetWindowText(title2);

	if (!bfp.Open(fileName, CFile::modeRead, &fileException))
		{
		MBOXP("Cannot open import file %s", fileName);
		//P2N("Cannot open import file\r\n", fileName);
		return;
		}

	CString str;
	char chh;
	while(true)
		{
		if(!bfp.Read(&chh, 1))
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

	bfp.Close();

	SetWindowText(title2);

	EndWaitCursor();

	Fill(m_filter);
}

#endif


void CAddrEntry::OnTimer(UINT nIDEvent)

{
	//P2N("CAddrEntry::OnTimer(UINT nIDEvent (%d))\r\n", nIDEvent);

	// Initial Timer tick
	if(nIDEvent == 1)
		{
		KillTimer(nIDEvent);

		// Create address book scema if not there
		if(!support.IsFile(dbfile))
			{
			CreateABfile(dbfile);
			}

		// Create address book backup scema if not there
		if(!support.IsFile(dbfile2))
			{
			CreateABfile(dbfile2, true);
			}

		//P2N("CAddrEntry::OnTimer: Initial load\r\n");
		Fill(m_filter);

		// Create one if blank
		if(m_tree.GetCount() == 0)
			{
			OnButton10();
			}

		//m_tree.SelectItem(m_tree.GetRootItem());
		//FillOne();

		//shield.ShowWindow(true);
		}

	if(nIDEvent == 2)
		{
		KillTimer(nIDEvent);
		P2N("timer for chaged %d\r\n", m_filter);
		Fill(m_filter);
		FillOne();
		}
	CGrayDlg::OnTimer(nIDEvent);
}

void CAddrEntry::OnButton10()

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
		MBOXP("SQL error: %s %s", sqlite3_errmsg(db), dbfile);
		sqlite3_close(db);
		
		return;
		//exit(1);
		}

	//str = "";
	sumstr = "";
	rows = 0;

	CUniqID uid;
	CString str = uid.GetStr();

	CTime ct = CTime::GetCurrentTime();
	CString datestr = ct.Format("%Y/%m/%d %H:%M:%S");

	CString qqq("insert into address (lastname, firstname, uidstr, createdate, updatedate) values('");

	qqq +=  "New', '";
	qqq +=  "Entry' , '";
	qqq +=   str;
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

	Fill(m_filter);
	SelEntryInt(rowid);
	
	UpdateData(false);

	// Enable editing
	((CButton*)GetDlgItem(IDC_AB_CHECK4))->SetCheck(true);	
	OnAbCheck4();

	// Go to first field
	GetDlgItem(IDC_AB_EDIT1)->SetFocus();
	((CEdit *)GetDlgItem(IDC_AB_EDIT1))->SetSel(0, -1);
	support.YieldToWinEx();

}


void CAddrEntry::OnButton11()

{
	FlushDB();

	P2N("Find  %s\r\n", m_findbox);

	Fill(m_findbox, true);
}

void CAddrEntry::OnChangeEdit14()

{
	UpdateData();
	P2N("CAddrEntry::OnChangeEdit14 %s\r\n", m_filter);

	SetTimer(2, 500, NULL);
}

void CAddrEntry::OnOk2()

{
	P2N("Exporting ..\r\n");
}

//////////////////////////////////////////////////////////////////////////

void CAddrEntry::FillPRL()

{
	// Already has a PRL
	if(m_prl != "")
		return;

	// Record not complete enough to evaluate
	if(m_bfirst == "" || m_blast == "")
		return;

	CString tmp(m_bdate); tmp.Replace("/", "_");

	// Clean it up a little
	m_bcode.TrimLeft();		m_bcode.TrimRight();
	m_bstate.TrimLeft();	m_bstate.TrimRight();
	m_bcity.TrimLeft();		m_bcity.TrimRight();
	m_blast.TrimLeft();		m_blast.TrimRight();
	m_bfirst.TrimLeft();	m_bfirst.TrimRight();

	// Make sure no \\ & : //

	CleanPRLname(m_blast); 
	CleanPRLname(m_bmname);
	CleanPRLname(m_bfirst);

	UpdateData(false);

	CString midname;

	if(m_bmname != "")
		{
		midname = "_" + m_bmname;
		}

	CString tmp2;
	
	// Is prl date valid?
	COleDateTime dd;
	int ret = Str2Date(m_bdate, &dd);
	if(ret)
		{
		MBOXP("Bad PRL format: '%s', please correct birth date.", m_bdate);
		return;
		}

	// Is PRL valid?
	if(	m_bcode	 != "" && m_bstate	!= "" &&
		m_bcity	 != "" && m_blast   != "" &&
		m_bfirst != "" && m_bdate   != "")
		{
		tmp2 = "PRL://" + m_bcode + "_" + m_bstate + "_" + m_bcity + "_" + tmp +
								"_" + m_blast + midname + "_" + m_bfirst ;
		tmp2.MakeUpper();

		//if(m_prl != "")
		//	{
		//	if(m_prl != tmp2)
		//		{
		//		if(MBOX("     This entry already has a 'PRL'  \r\n"
		//			     "   Are you sure you want to change it?    ", MB_YESNO) != IDYES)
		//			{
		//			return;
		//			}
		//		}
		//	}

		m_prl = tmp2;

		// Fill in dir as well
		CString tmp2(m_bdate); tmp2.Replace("/", "\\");


		CString dir(dataroot + "patients\\");
		dir += m_bcode + "\\" + m_bstate + "\\" + m_bcity + "\\" + tmp2 +
								"\\" + m_blast + midname + "_" + m_bfirst ;

		P2N("PRL Directory '%s'\r\n", dir);
		}

	//mkdir(dir);
}

void CAddrEntry::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)

{
	P2N("CAddrEntry::OnkeyDown %d\r\n", nChar);

	if(nChar == VK_RETURN)
		{
		P2N("ENTER Down\r\n");
		}

	if(GetKeyState(VK_CONTROL) & 0x8000)
		{
		if(nChar == 83)
			{
			P2N("Control-S Down\r\n");
			FlushDB();
			}
		}

	CGrayDlg::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CAddrEntry::PreTranslateMessage(MSG* pMsg)

{

#if 1

	//////////////////////////////////////////////////////////////////////
	// Dispatch messages ourselvs
	
	if(pMsg->message == WM_KEYDOWN)
		{
		//P2N("CAddrEntry::PreTranslateMessage WM_KEYDOWN\r\n");
		
		//OnKeyDown(pMsg->wParam, LOWORD(pMsg->lParam), pMsg->lParam);
	
		// Capture ENTER
		if(pMsg->wParam == 13)
			{
			if(GetFocus()->m_hWnd == m_ok.m_hWnd)
				{
				//P2N("Enter on OK\r\n");
				goto endd;
				}
			else
				{
				CRuntimeClass *rtc; rtc = GetFocus()->GetRuntimeClass();
				CString ed("CEdit");
				if(ed.Find(rtc->m_lpszClassName) >= 0)
					{
					LONG ss = GetWindowLong(GetFocus()->m_hWnd, GWL_STYLE);

					if(!(ss & ES_MULTILINE))
						{
						pMsg->wParam = 9;
						goto endd;
						}
					}
				return 0;
				}
			}
		}

	if(pMsg->message == WM_CHAR)
		{
		//OnChar(pMsg->wParam, LOWORD(pMsg->lParam), pMsg->lParam);
		//return 0;
		}

	if(pMsg->message == WM_KEYUP)
		{
		//OnKeyUp(pMsg->wParam, LOWORD(pMsg->lParam), pMsg->lParam);
		//return 0;
		}
#endif

#if 0
	if(pMsg->message == WM_SYSCHAR)
		{
		OnSysChar(pMsg->wParam, LOWORD(pMsg->lParam), pMsg->lParam);
		return 0;
		}
	if(pMsg->message == WM_SYSKEYDOWN)
		{
		OnSysKeyDown(pMsg->wParam, LOWORD(pMsg->lParam), pMsg->lParam);
		return 0;
		}
	if(pMsg->message == WM_SYSKEYUP)
		{
		OnSysKeyUp(pMsg->wParam, LOWORD(pMsg->lParam), pMsg->lParam);
		return 0;
		}
	#endif

endd:

	return CGrayDlg::PreTranslateMessage(pMsg);
}


void CAddrEntry::OnButton29()

{
	CString approot; support.GetAppRoot(approot);
	CString str, file("countrycodes.html");

#ifdef _DEBUG
	str = "Z:/archive/xraynotes/umanual/" + file;
#else
	str =  approot + "umanual\\" + file;
#endif

	ShellExecute(NULL, "open", str, NULL, NULL, SW_SHOWMAXIMIZED);
}


void CAddrEntry::OnButton22()

{
	UpdateData();

	CDateSel ds;

	ds.time_disabled = true;
	ds.result = m_bdate;

	if(ds.DoModal() == IDOK)
		{
		m_bdate = ds.result;
		P2N("Got date %s\r\n", m_bdate);
		UpdateData(false);
		}
}

void CAddrEntry::OnKillfocusEdit23()

{
	UpdateData();

	FillPRL();

	UpdateData(false);
}

//////////////////////////////////////////////////////////////////////////

void CAddrEntry::FillOne()

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

void CAddrEntry::OnChangeEdit1()

{
	//CTreeCtrl
	if(m_tree.GetCount() == 0)
		{
		MBOX("No item in table, please create an item first by clicking on the 'Add Item' button");
		}
}

void CAddrEntry::OnKillFocus(CWnd* pNewWnd)

{
	CGrayDlg::OnKillFocus(pNewWnd);
	//P2N("CAddrEntry::KillFocus\r\n");
}

//////////////////////////////////////////////////////////////////////////

void CAddrEntry::OnCheck1() 

{
	int cc = ((CButton*)GetDlgItem(IDC_AB_CHECK1))->GetCheck();
	
	P2N("Checked %d\r\n", cc);

	// Set All screentip flags
	for(int ii=0; ii < farrsize; ii++)
		{
		if(farr[ii].ctrl != NULL)
			{
			farr[ii].ctrl->tipshow = cc;
			}
		}
}

//////////////////////////////////////////////////////////////////////////

#if 0

void CAddrEntry::OnOk4() 

{
	
}

#endif

void CAddrEntry::OnKillfocusAbEdit9() 

{
	//P2N("CAddrEntry::OnKillfocusAbEdit9\r\n"); 

	HTREEITEM hItem  = m_tree.GetSelectedItem();
	if(hItem)
		{
		P2N("CAddrEntry::OnKillfocusAbEdit9 - Selected Item\r\n");

		// Update (if changed)
		UpdateDB(m_tree.GetItemData(hItem));
		}
	else
		{
		// Add new data to list
		UpdateData();

		P2N("unSelected Item\r\n");

		if(m_lname != "")
			{
			sqlite3 *db;
			char *zErrMsg = 0;
			int rc;

			rc = sqlite3_open(dbfile, &db);

			if( rc )
				{
				MBOXP("SQL error: %s %s", sqlite3_errmsg(db), dbfile);
				sqlite3_close(db);

				return;
				//exit(1);
				}

			//str = "";
			sumstr = "";
			rows = 0;

			CUniqID uid;
			CString str = uid.GetStr();

			CTime ct = CTime::GetCurrentTime();
			CString datestr = ct.Format("%Y/%m/%d %H:%M:%S");

			CString qqq("insert into address (lastname, midname, firstname, uidstr, createdate, updatedate) values('");

			qqq +=  m_lname + "', '";
			qqq +=  m_mname + "', '";
			qqq +=  m_fname  + "' , '";
			qqq +=   str;
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
			SelEntryInt(rowid, false);
			}

		// Set focus to next tabbed entry
		//GetDlgItem(IDC_AB_EDIT19)->SetFocus();

		//((CEdit *)GetDlgItem(IDC_AB_EDIT1))->SetSel(0, -1);
		//UpdateData(false);
		}
}

void CAddrEntry::OnAbButton12() 

{
	UpdateData();
	MBOX(m_email);
}

void CAddrEntry::OnSetfocusAbTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//P2N("CAddrEntry::OnSetfocusAbTree1\r\n");
	*pResult = 0;
}

//////////////////////////////////////////////////////////////////////////
// Return true if data changed

int CAddrEntry::IsDataChanged()

{
	int changed = false;

	// Compare all fields
	for(int ii=0; ii < farrsize; ii++)
		{
		if(farr[ii].value != *farr[ii].var)
			{
			//MBOXP("changed data from %s %s", *arr[i], *arr2[i]);
			changed = true;
			break;
			}
		}

	// Compare all flags
	for(int iii=0; iii < flarrsize; iii++)
		{
		if(flarr[iii].value != *flarr[iii].var)
			{
			//MBOXP("changed flags from %s %s", *arr[i], *arr2[i]);
			changed = true;
			break;
			}
		}

	return changed;
}

void CAddrEntry::FlushDB()

{
	// Force it
	UpdateData();

	P2N("CAddrEntry::FlushDB()\r\n");

	UpdateData();

	HTREEITEM hItem  = m_tree.GetSelectedItem();

	if(!hItem)
		return;

	rescan = UpdateDB(m_tree.GetItemData(hItem));
	
	CString str1 = m_tree.GetItemText(hItem);
	CString str2 = m_lname + ", " + m_mname + ", " + m_fname;

	FillPRL();
	UpdateData(false);

	// See if the main tree needs refresh
	if(str1 != str2)
		m_tree.SetItemText(hItem, str2);
}

#if 0

void CAddrEntry::OnOk5() 
{
	P2N("Clear filters\r\n");
}
#endif


void CAddrEntry::OnOk3() 
{
	P2N("Importing...\r\n");	
}

void CAddrEntry::OnCheck3() 

{
	FlushDB();
	Fill();	
}


void CAddrEntry::OnAbOk2() 

{
	//P2N("Exporting ..	\r\n");
	// CFileDialog


	CFileDialogST  fd(false);

	CString dir = support.GetCurrentDir();
			
	CString strFilter;
	
	strFilter += "Comma Separated Values (*.csv)";	strFilter += (TCHAR)NULL;	
	strFilter += "*.csv";				strFilter += (TCHAR)NULL;

	strFilter += "All files (*.*)";	strFilter += (TCHAR)NULL;	
	strFilter += "*.*";				strFilter += (TCHAR)NULL;

	// Terminating NULL
	strFilter += (TCHAR)NULL;

	fd.m_ofn.lpstrInitialDir = dir;

	fd.m_ofn.lpstrFilter	 = strFilter; 
	fd.m_ofn.nFilterIndex    = 1;

	if(fd.DoModal() != IDOK)
		{
		return;
		}

	P2N("Selected file '%s'\r\n", fd.GetPathName());

	CFile fp;
	if(!fp.Open(fd.GetPathName(), CFile::modeCreate || CFile::modeWrite))
		{
		P2N("Cannot create file %s %s\r\n", fd.GetPathName(), strerror(errno));
		MBOX("Cannot create file ");
		return;
		}

	CString str, str2, str3;
	int count = 0;

	CArchive ar(&fp, CArchive::store);

	//str = "XrayNotes Address Book Archive\r\n";
	//ar.WriteString(str);

	
	// Dump CSV file
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	rc = sqlite3_open(dbfile, &db);

	if( rc )
		{
		MBOXP("SQL error: %s %s", sqlite3_errmsg(db), dbfile);
		sqlite3_close(db);
		sqlite3_free(zErrMsg);
		return;
		}

	// Output SCEME
	scemastr = "";
	CString qqqq = "PRAGMA table_info(address)";

	rc = sqlite3_exec(db, qqqq, callback_scema, &ar, &zErrMsg);
	if( rc != SQLITE_OK )
		{
		MBOXP("SQL error: %s", sqlite3_errmsg(db));
		sqlite3_free(zErrMsg);
		}

	scemastr += "\r\n";
	ar.WriteString(scemastr);


	// Output DATA
	CString qqq = "select * from address ";

	rc = sqlite3_exec(db, qqq, callback_export, &ar, &zErrMsg);
	if( rc != SQLITE_OK )
		{
		MBOXP("SQL error: %s", sqlite3_errmsg(db));
		sqlite3_free(zErrMsg);
		}

	sqlite3_close(db);

	//fp.Close();
}

int CAddrEntry::CreateABfile(const char *dbname, int backup)

{
	P2N("Creating address book %s\r\n", dbfile);
			
	CString qqq("create table address (");

	// Fill in names/values of fields to create
	for(int iii=0; iii < farrsize; iii++)
		{
		if(iii != 0)
			qqq += ", ";

		if(farr[iii].name == "id" && !backup)
			qqq += farr[iii].name + " integer primary key autoincrement";
		else	 
			qqq += farr[iii].name + " text";	
		}

	// Fill in names of flags
	for(iii = 0; iii < flarrsize; iii++)
		{
		qqq += ", ";
		qqq += flarr[iii].name + " text";
		}

	qqq += ")";

	//MBOXP("SQL: '%s'", qqq);
	
	P2N("CAddrEntry::CreateABfile %s\r\nqqq='%s'\r\n", dbname, qqq);

	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	//////////////////////////////////////////////////////////////////////
	// Create main address file

	rc = sqlite3_open(dbname, &db);
	if( rc )
		{
		MBOXP("SQL error: %s %s", sqlite3_errmsg(db), dbfile);
		sqlite3_close(db);
		sqlite3_free(zErrMsg);
		return 0;
		}

	rc = sqlite3_exec(db, qqq, callback_noop, this, &zErrMsg);
	if( rc != SQLITE_OK )
		{
		MBOXP("SQL error on database %s sql %s", dbname, sqlite3_errmsg(db));
		sqlite3_free(zErrMsg);
		}

	sqlite3_close(db);

	return 0;
}

void CAddrEntry::OnAbOk5() 

{
	m_filter = "";
	UpdateData(false);
	Fill();
}

void CAddrEntry::OnAbOk4() 
{

	P2N("Copy birth items\r\n");	

	UpdateData();

	if(m_bfirst !=  "")	
		{
		if(MBOX(" This entry already has a birthname filled in.  \r\n"
				"    Are you sure you want to replace it?    ", "", MB_YESNO) != IDYES)
			{
			return;
			}
		}

	m_bfirst	= m_fname;
	m_blast		= m_lname;
	m_bmname	= m_mname;

	UpdateData(false);
		
}

void CAddrEntry::OnAbOk7() 

{
	UpdateData();

	int ret = MBOX("Deleting/Refreshing  PRL. This action will impact the global visibility of "
		"previuous PRL.\r\n\r\nAre you sure you want to refresh the PRL?", "", true, IDNO);

	if(ret != IDYES)
		return;

	P2N("update PRL\r\n");

	m_prl = "";
	FillPRL();
	UpdateData(false);
}

//////////////////////////////////////////////////////////////////////////

int CAddrEntry::ExecSql(const char *fname, const char *sql, sqlite3_callback callb)

{
	char *zErrMsg = 0;
	sqlite3 *db;
	
	//P2N("Opening DB file %s\\%s\r\n", GetCurrentDir(), dbfile);
	
	int rc = sqlite3_open(fname, &db);

	if( rc )
		{
		MBOXP("Cannot open SQL file %s", fname);
		sqlite3_close(db);	
		return -1;
		}

	rc = sqlite3_exec(db, sql, callb, this, &zErrMsg);

	if( rc != SQLITE_OK )
		{
		MBOXP("SQL error: %s", sqlite3_errmsg(db));
		sqlite3_free(zErrMsg);
		return -1;
		}

	int rowid = (int)sqlite3_last_insert_rowid(db);
	sqlite3_close(db);

	return rowid;
}

void CAddrEntry::OnAbOk8() 
{
	MBOXP("The PATID is assigned permanently to this entry.");
}

void CAddrEntry::OnAbButton30() 
{
	OnButton29();
}

void CAddrEntry::OnAbButton13() 
{
	UpdateData();
	MBOX(m_email2);
}

void CAddrEntry::OnAbOk6() 
{
	m_filter = "";
	UpdateData(false);
	Fill();	
}

void CAddrEntry::OnAbCheck4() 

{
	int allow = ((CButton*)GetDlgItem(IDC_AB_CHECK4))->GetCheck();			

	//P2N("Allow button %d\r\n", allow);

	// Disable All
	for(int ii=0; ii < farrsize; ii++)
		{
		if(farr[ii].ctrl != NULL)
			{
			if(allow)
				{
				// Only enable the ones that has no initial disable
				if(!farr[ii].olddis)
					farr[ii].ctrl->Enable();
				}
			else
				{
				farr[ii].ctrl->Disable();
				}
			}
		}

	m_elname.SetFocus();
	m_elname.SetSel(-1,-1);
}

void CAddrEntry::OnAbCheck1() 

{
	int tooltip = ((CButton*)GetDlgItem(IDC_AB_CHECK1))->GetCheck();			
	P2N("Tooltip button %d\r\n", tooltip);
}

void CAddrEntry::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 

{
	P2N("CAddrEntry::OnSysKeyDown(UINT nChar=%d %c\r\n", nChar, nChar);

#if 0
	CString str2; str2.Format("&%c", nChar);

	CWnd *win = NULL, *win2 = NULL;

	win2 = win = GetNextDlgTabItem(NULL);

	while(true)
		{
		win2 = GetNextDlgTabItem(win2);
		if(win == win2)
			{
			break;
			}
		CRuntimeClass *runclass;
		runclass = win2->GetRuntimeClass();
	
		if(strcmp("CButton", runclass->m_lpszClassName) == 0)
			{
			CString str; win2->GetWindowText(str);
			str.MakeUpper();

			if(str.Find(str2) >= 0)
				{
				P2N("Dialog item %d %s classname=%s\r\n", win2, str, runclass->m_lpszClassName);	
				return;
				break;
				}
			}		
		}
#endif
	
	CGrayDlg::OnSysKeyDown(nChar, nRepCnt, nFlags);
}

//////////////////////////////////////////////////////////////////////////

void CAddrEntry::OnAbButton14() 

{	
	if(filling)
		return;

	HTREEITEM hItem  = m_tree.GetSelectedItem();

	if(!hItem)
		{
		MBOX(PleaseSel);
		return;
		}

	CString dir, str, num;

	UpdateData();
	UpdateDB(m_tree.GetItemData(hItem));

	CString tmp; tmp.Format("'%s'", m_uniq);
	
	str =  m_tree.GetItemText(hItem);

	if(str == "")
		return;

	//////////////////////////////////////////////////////////////////////
	CString qqq = "select ";
	
	// Fill in names of fields
	for(int ii = 0; ii <  farrsize; ii++)
		{
		if(ii > 0)
			qqq += ", ";
		qqq += farr[ii].name;
		}

	// Fill in names of flags
	for(int iii = 0; iii < flarrsize; iii++)
		{
		qqq += ", ";
		qqq += flarr[iii].name;
		}

	qqq += " from address where uidstr = ";
	qqq += tmp;

	P2N("CAddrEntry::History qqq=%s\r\n", qqq);
	
	//////////////////////////////////////////////////////////////////////
	CString qqq2 = "select ";
	
	// Fill in names of fields
	for(ii = 0; ii <  farrsize; ii++)
		{
		if(ii > 0)
			qqq2 += ", ";
		qqq2 += farr[ii].name;
		}

	// Fill in names of flags
	for(iii = 0; iii < flarrsize; iii++)
		{
		qqq2 += ", ";
		qqq2 += flarr[iii].name;
		}

	qqq2 += " from address where uidstr = ";
	qqq2 += tmp;
	qqq2 += " order by updatedate desc";


	P2N("CAddrEntry::History qqq2=%s\r\n", qqq2);

	//////////////////////////////////////////////////////////////////////

	CHistoryDlg hist;

	hist.m_grid.first_bold = true;

	hist.sql = qqq;
	hist.dbfile = dbfile;

	hist.sql2 = qqq2;
	hist.dbfile2 = dbfile2;

	hist.header.Format("History for '%s %s %s'", m_lname, m_mname, m_fname);

	hist.DoModal();
}

//////////////////////////////////////////////////////////////////////////

void	CAddrEntry::init_array()

{
	for(int loop = 0; loop < sizeof(farr)/sizeof(fields); loop++)
		{
		farr[loop].name		= farr[loop].value	= farr[loop].value2  = farr[loop].var2	= "";
		farr[loop].var		= NULL;
		farr[loop].ctrl		= NULL;
		farr[loop].tipid	= farr[loop].olddis	= 0;
		}

	int fs = 0;
	
	farr[fs].name =  "LastName";	farr[fs].var = &m_lname; 	farr[fs].ctrl = &m_elname; farr[fs].tipid = 0; fs++;
	farr[fs].name =  "MidName"; 	farr[fs].var = &m_mname;    farr[fs].ctrl = &m_emname; farr[fs].tipid = 0; fs++;
	farr[fs].name =  "FirstName"; 	farr[fs].var = &m_fname;    farr[fs].ctrl = &m_efname; farr[fs].tipid = 0; fs++;
	farr[fs].name =  "Prefix"; 		farr[fs].var = &m_prefix;   farr[fs].ctrl = &m_eprefix; farr[fs].tipid = 0; fs++;
	farr[fs].name =  "Suffix"; 		farr[fs].var = &m_suffix;   farr[fs].ctrl = &m_esuffix; farr[fs].tipid = 0; fs++;
	farr[fs].name =  "Alias"; 		farr[fs].var = &m_alias;    farr[fs].ctrl = &m_ealias; farr[fs].tipid = 0; fs++;
	farr[fs].name =  "UpdateDate"; 	farr[fs].var = &m_udate;    farr[fs].ctrl = &m_eudate; farr[fs].tipid = 0; fs++;
	farr[fs].name =  "Tel"; 		farr[fs].var = &m_tel; 	    farr[fs].ctrl = &m_etel; farr[fs].tipid = 0; fs++;
	farr[fs].name =  "TelExt"; 		farr[fs].var = &m_ext; 	    farr[fs].ctrl = &m_eext; farr[fs].tipid = 0; fs++;
	farr[fs].name =  "WorkTel"; 	farr[fs].var = &m_wtel;     farr[fs].ctrl = &m_ewtel; farr[fs].tipid = 0; fs++;
	farr[fs].name =  "WorkExt"; 	farr[fs].var = &m_wext;     farr[fs].ctrl = &m_ewext; farr[fs].tipid = 0; fs++;
	farr[fs].name =  "Cell";  		farr[fs].var = &m_cell;     farr[fs].ctrl = &m_ecell; farr[fs].tipid = 0; fs++;
	farr[fs].name =  "CellExt";  		farr[fs].var = &m_cext;     farr[fs].ctrl = &m_ecext; farr[fs].tipid = 0; fs++;
	farr[fs].name =  "Fax"; 		farr[fs].var = &m_fax; 	    farr[fs].ctrl = &m_efax; farr[fs].tipid = 0; fs++;
	farr[fs].name =  "FaxExt"; 		farr[fs].var = &m_faxext;   farr[fs].ctrl = &m_efaxext; farr[fs].tipid = 0; fs++;
	farr[fs].name =  "Email";		farr[fs].var = &m_email;    farr[fs].ctrl = &m_eemail; farr[fs].tipid = 0; fs++;
	farr[fs].name =  "Email2"; 		farr[fs].var = &m_email2;   farr[fs].ctrl = &m_eemail2; farr[fs].tipid = 0; fs++;
	farr[fs].name =  "Note"; 		farr[fs].var = &m_note;     farr[fs].ctrl = &m_enote; farr[fs].tipid = 0; fs++;
	farr[fs].name =  "Address";		farr[fs].var = &m_address;  farr[fs].ctrl = &m_eaddress; farr[fs].tipid = 0; fs++;
	farr[fs].name =  "City"; 		farr[fs].var = &m_city;     farr[fs].ctrl = &m_ecity; farr[fs].tipid = 0; fs++;
	farr[fs].name =  "Zip"; 		farr[fs].var = &m_zip; 	    farr[fs].ctrl = &m_ezip; farr[fs].tipid = 0; fs++;
	farr[fs].name =  "State";		farr[fs].var = &m_state;    farr[fs].ctrl = &m_estate; farr[fs].tipid = 0; fs++;
	farr[fs].name =  "Country";		farr[fs].var = &m_country;  farr[fs].ctrl = &m_ecountry; farr[fs].tipid = 0; fs++;
	farr[fs].name =  "cCode";  		farr[fs].var = &m_ccode;    farr[fs].ctrl = &m_eccode; farr[fs].tipid = 0; fs++;
	farr[fs].name =  "BirthFirst"; 	farr[fs].var = &m_bfirst;   farr[fs].ctrl = &m_ebfirst; farr[fs].tipid = IDS_BFIRST; fs++;
	farr[fs].name =  "BirthLast"; 	farr[fs].var = &m_blast;    farr[fs].ctrl = &m_eblast;  farr[fs].tipid = IDS_BLAST;  fs++;
	farr[fs].name =  "BirthMddle";	farr[fs].var = &m_bmname;   farr[fs].ctrl = &m_ebmname; farr[fs].tipid = IDS_BMNAME; fs++;
	farr[fs].name =  "BrthPace";  farr[fs].var = &m_bplace;   farr[fs].ctrl = &m_ebplace; farr[fs].tipid = 0; fs++;
	farr[fs].name =  "BirthCity";  	farr[fs].var = &m_bcity;    farr[fs].ctrl = &m_ebcity; farr[fs].tipid = IDS_BCITY; fs++;
	farr[fs].name =  "BirthZip";  	farr[fs].var = &m_bzip;     farr[fs].ctrl = &m_ebzip; farr[fs].tipid = 0; fs++;
	farr[fs].name =  "BirthNote";  	farr[fs].var = &m_bnote;    farr[fs].ctrl = &m_ebnote; farr[fs].tipid = 0; fs++;
	farr[fs].name =  "BirthCountry";farr[fs].var = &m_bcountry; farr[fs].ctrl = &m_ebcountry; farr[fs].tipid = 0; fs++;
	farr[fs].name =  "BirthDate";  	farr[fs].var = &m_bdate;    farr[fs].ctrl = &m_ebdate; farr[fs].tipid = 0; fs++;
	farr[fs].name =  "BirthState";  farr[fs].var = &m_bstate;   farr[fs].ctrl = &m_ebstate; farr[fs].tipid = 0; fs++;
	farr[fs].name =  "BcCode";  	farr[fs].var = &m_bcode;    farr[fs].ctrl = &m_ebcode; farr[fs].tipid = 0; fs++;
	farr[fs].name =  "Qnote"; 		farr[fs].var = &m_qnote;    farr[fs].ctrl = &m_eqnote; farr[fs].tipid = 0; fs++;
	farr[fs].name =  "prl";  		farr[fs].var = &m_prl; 	    farr[fs].ctrl = &m_eprl; farr[fs].tipid = 0; fs++;
	farr[fs].name =  "uidstr";		farr[fs].var = &m_uniq;     farr[fs].ctrl = &m_euniq; farr[fs].tipid = 0; fs++;
	farr[fs].name =  "id";  		farr[fs].var = &m_cid; 	    farr[fs].ctrl = &m_ecid; farr[fs].tipid = 0; fs++;
	farr[fs].name =  "CreateDate";  farr[fs].var = &m_cdate;    farr[fs].ctrl = &m_ecdate; farr[fs].tipid = 0; fs++;
	
	farrsize = fs;

	//P2N("Number of edit fields: %d max=%d\r\n", farrsize, sizeof(farr) / sizeof(fields));

	// Make sure it is still smaller then the number of fields
	ASSERT(farrsize < sizeof(farr) / sizeof(fields)) ;

	// Load All screentips (if any)
	for(int ii=0; ii < farrsize ; ii++)
		{
		if(farr[ii].ctrl != NULL)
			{
			if(farr[ii].tipid)
				farr[ii].ctrl->LoadTip(farr[ii].tipid);
			}
		}

	//////////////////////////////////////////////////////////////////////
	// Decorate flags array
	int fls = 0;

	flarr[fls].name =  "active"; flarr[fls].var = &m_active; flarr[fls].ctrl = NULL; flarr[fls].tipid = 0; fls++;

	flarrsize = fls;
}




void CAddrEntry::OnAbButton3() 

{
	HTREEITEM hItem  = m_tree.GetSelectedItem();
	if(!hItem)
		{
		MBOX(PleaseSel);
		return;
		}	

	CMedReg  mr;

	mr.DoModal();

}

void CAddrEntry::OnAbButton15() 

{
	UpdateDB(-1);	
}

void CAddrEntry::OnAbButton6() 

{
	HTREEITEM hItem  = m_tree.GetSelectedItem();
	if(!hItem)
		{
		MBOX(PleaseSel);
		return;
		}	

	CXcalDlg	xc;

	xc.DoModal();
}

void CAddrEntry::OnRclickAbTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//P2N("CAddrEntry::OnRclickAbTree1\r\n");	

	*pResult = 0;

	// Make the right clic selected item current
	UINT	flags;
	HTREEITEM item = m_tree.HitTest(m_tree.currentmouse, &flags );

	if(item)
		{
		m_tree.SelectItem(item);

		support.YieldToWinEx();

		POINT pt(m_tree.currentmouse);
		m_tree.ClientToScreen(&pt);
		
		CString name = m_tree.GetItemText(item);

		//hrclickmenu->DeleteMenu(0,  MF_BYPOSITION);
		//hrclickmenu->InsertMenu(0,  MF_BYPOSITION  | MF_STRING | MF_GRAYED , ID_TREERCLICK_DELETEITEM + 1, name);

		//hrclickmenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, pt.x, pt.y, this, NULL);
		}
}

void CAddrEntry::OnMouseMove(UINT nFlags, CPoint point) 

{
	curr = point;
	//P2N("CAddrEntry::OnMouseMove %d %d\r\n", point.x, point.y);
	CGrayDlg::OnMouseMove(nFlags, point);
}


void CAddrEntry::LoadToolTips()
{
	///////////////////////////////////////////////////////////////////////
	// Pre-load tooltip texts

	//m_Lname.tiptext =	"Enter 'Last Name' - 'Middle name' --  'First Name'"; 

	m_ebzip.tiptext =	"This field is NOT used as part of the 'PRL'. "
						"It is for informational purposes only. ";

	m_ebcountry.tiptext =	
						"This field is NOT used as part of the 'PRL'. "
						"It is for informational purposes only. Use CountyCode field to denote country "
						"for the PRL";

	m_ebcountry.tiptext =	
						"This field is used as part of the 'PRL'. "
						"This is the Patient's birth country of origin. For a list of country codes, use the "
						"reference list (button to the right). For compatibility, country codes are identical to the "
						"domain suffix, as assigned by the appropriate internet standards body.";
						
	m_ebdate.tiptext =
						"This field is used as part of the 'PRL'. Fill in the date of birth in the format of "
						"yyyy/mm/dd where 'yyyy' = four digit year,  'mm' = two digit month, 'dd' = two digit day "
						" For example: 21st of nov 1960 will be: 1960/11/21 ";

	m_ebstate.tiptext =
		"This field is used as part of the 'PRL'. Fill in the major subdivision of the country, "
		"known as the state, province, county, district, "
		"territory, land, shire, department, canton, prefecture, oblast or autonomous "
		"region, etc, depending on the country. For the US, use state abbreviations like FL, NJ, MA";

	m_ebplace.tiptext =
		"This field is NOT used as part of the 'PRL'. Fill in for reference only. ";

	m_eprl.tiptext =
		"This field is the Patient Resource Locator ('PRL') It is constructed to be Globally Unique "
		"by virtue of Birth Location, Birth Date, Birth Name";

	m_euniq.tiptext =
		"This field is the Patient ID ('PATID') It is constructed to be Globally Unique "
		"by virtue of Creation Date and Random Sequences. Only the owner of the PATID can link it back to its "
		"originating patient. Thus, records can be made freely available for research and education. ";

	m_ebnote.tiptext =
		"This field is NOT used as part of the 'PRL'. Content may be used for informational purposes. ";

}
