// HistoryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "xraynotes.h"
#include "HistoryDlg.h"

#include "mxpad.h"

#include "support.h"
#include "sqlite3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////

static	int		callback_history(void *NotUsed, int argc, char **argv, char **azColName)

{
	CString  tmp, ttmp;

	CHistoryDlg *lastclass = (CHistoryDlg *)NotUsed;

	//P2N("CHistoryDlg::callback_history\r\n");

	if(lastclass->reccnt == 0)
		{
		for(int ii=0; ii < argc; ii++)
			{
			CString str = azColName[ii];
			
			str.SetAt(0, toupper(str[0]));

			lastclass->m_grid.AddStr(lastclass->reccnt, ii, str, false);
			}
		lastclass->reccnt++;
		}	
	
	lastclass->m_grid.Invalidate();
	
	for(int iii=0; iii < argc; iii++)
		{
		lastclass->m_grid.AddStr(lastclass->reccnt, iii, argv[iii], false);
		//P2N("%s=%s  ",  azColName[iii], argv[iii] ? argv[iii] : "NULL");
		}

	lastclass->m_grid.AutoSize();
	lastclass->m_grid.Invalidate();

	lastclass->reccnt++;

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CHistoryDlg dialog


CHistoryDlg::CHistoryDlg(CWnd* pParent /*=NULL*/)
	: CGrayDlg(CHistoryDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHistoryDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	reccnt = 0;

	windowname = "CHistoryDlg";
	m_title = "DataBase History Dialog";
	
	SetAlpha(245);
}


void CHistoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CGrayDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHistoryDlg)
	DDX_Control(pDX, IDC_HIST_GRID, m_grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHistoryDlg, CGrayDlg)
	//{{AFX_MSG_MAP(CHistoryDlg)
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHistoryDlg message handlers

BOOL CHistoryDlg::OnInitDialog() 

{
	CGrayDlg::OnInitDialog();
	
	SetWindowText(header);

	// Geometry for buttons
	WINDOWPLACEMENT wp;
	RECT rc; GetClientRect(&rc);


	GetDlgItem(IDOK)->GetWindowPlacement(&wp);
	ok_x = rc.right - wp.rcNormalPosition.left;	
	ok_y = rc.bottom - wp.rcNormalPosition.top;	

	GetDlgItem(IDCANCEL)->GetWindowPlacement(&wp);
	can_x = rc.right - wp.rcNormalPosition.left;	
	can_y = rc.bottom - wp.rcNormalPosition.top;	

	GetDlgItem(IDC_HIST_GRID)->GetWindowPlacement(&wp);
	ed1_left	= wp.rcNormalPosition.left;	
	ed1_top		= wp.rcNormalPosition.top;	
	ed1_gapx    = rc.right  - wp.rcNormalPosition.right;	
	ed1_gapy    = rc.bottom - wp.rcNormalPosition.bottom;	

	SetTimer(1, 100, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CHistoryDlg::OnOK() 
{
	CGrayDlg::OnOK();
}

void CHistoryDlg::Fill()

{
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	reccnt = 0;

	//////////////////////////////////////////////////////////////////////

	if(sql != "")
		{
		rc = sqlite3_open(dbfile, &db);

		if( rc )
			{
			xMsgBox("Cannot open SQL");
			sqlite3_close(db);
			return;
			//exit(1);
			}
		rc = sqlite3_exec(db, sql, callback_history, this, &zErrMsg);

		if( rc != SQLITE_OK )
			{
			MBOXP("SQL err: %d %s", rc, sqlite3_errmsg(db));
			sqlite3_free(zErrMsg);
			}
		sqlite3_close(db);
		}

	//////////////////////////////////////////////////////////////////////

	if(sql2 != "")
		{
		rc = sqlite3_open(dbfile2, &db);

		if( rc )
			{
			xMsgBox("Cannot open SQL2");
			sqlite3_close(db);
			return;
			//exit(1);
			}
		rc = sqlite3_exec(db, sql2, callback_history, this, &zErrMsg);

		if( rc != SQLITE_OK )
			{
			MBOXP("SQL err: %d %s", rc, sqlite3_errmsg(db));
			sqlite3_free(zErrMsg);
			}
		sqlite3_close(db);
		}
}


void CHistoryDlg::OnTimer(UINT nIDEvent) 

{
	//KillTimer(nIDEvent);

	if(nIDEvent == 1)
		{
		KillTimer(nIDEvent);
		P2N("CHistoryDlg::OnTimer -- Fill timer\r\n");
		Fill();
		}

	if(nIDEvent == 2)
		{
		KillTimer(nIDEvent);
		P2N("CHistoryDlg::OnTimer -- Invalidate timer\r\n");		
		//((CSimpleGrid*)GetDlgItem(IDC_HIST_GRID))->Resized();
		Invalidate();
		}

	CGrayDlg::OnTimer(nIDEvent);
}

void CHistoryDlg::OnSize(UINT nType, int cx, int cy)
 
{
	CGrayDlg::OnSize(nType, cx, cy);
	
	//P2N("CTranscript::OnSize %d %d flag=%d\r\n", cx, cy, IsWindow(m_hWnd));

	CWnd  *ww = GetDlgItem(IDOK);
		{
		if(ww)
			if(IsWindow(ww->m_hWnd))
				Resize();
		}	
		
}

void	CHistoryDlg::Resize()

{
	RECT rc; GetClientRect(&rc);

	//WINDOWPLACEMENT wp;	GetWindowPlacement(&wp);

	// Right aligned
	GetDlgItem(IDOK)->SetWindowPos(NULL, rc.right - ok_x,  rc.bottom - ok_y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	GetDlgItem(IDCANCEL)->SetWindowPos(NULL, rc.right - can_x,  rc.bottom - can_y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	GetDlgItem(IDC_HIST_GRID)->SetWindowPos(NULL, ed1_left, ed1_top, 
								rc.right - ed1_gapx - ed1_left,  rc.bottom - ed1_gapy - ed1_top, SWP_NOZORDER);

	SetTimer(2, 100, NULL);
}

void CHistoryDlg::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 

{
	lpMMI->ptMinTrackSize.x = 200;
	lpMMI->ptMinTrackSize.y = 150;
	
	CGrayDlg::OnGetMinMaxInfo(lpMMI);
}
