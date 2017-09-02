// TestDlg.cpp : implementation file
//

#include "stdafx.h"

#include "xraynotes.h"
#include "TestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestDlg dialog


CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
	: CGrayDlg(CTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestDlg)
	//}}AFX_DATA_INIT
}


void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CGrayDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestDlg)
	DDX_Control(pDX, IDC_LIST3, m_list);
	DDX_Control(pDX, IDC_TEST_LIST, m_tlist);
	DDX_Control(pDX, IDC_EDIT1, m_ed);
	DDX_Control(pDX, IDC_LAB8, m_l8);
	DDX_Control(pDX, IDC_LAB7, m_l7);
	DDX_Control(pDX, IDC_LAB6, m_l5);
	DDX_Control(pDX, IDC_LAB4, m_l4);
	DDX_Control(pDX, IDC_LAB3, m_l3);
	DDX_Control(pDX, IDC_LAB2, m_l2);
	DDX_Control(pDX, IDC_LAB1, m_l1);
	DDX_Control(pDX, IDC_TRI, m_tri);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTestDlg, CGrayDlg)
	//{{AFX_MSG_MAP(CTestDlg)
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_ACTIVATE()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestDlg message handlers

void CTestDlg::OnSetFocus(CWnd* pOldWnd) 
{
	CGrayDlg::OnSetFocus(pOldWnd);
	
	// TODO: Add your message handler code here
	
}

void CTestDlg::OnKillFocus(CWnd* pNewWnd) 

{
	CGrayDlg::OnKillFocus(pNewWnd);
	
}

void CTestDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 

{
	CGrayDlg::OnActivate(nState, pWndOther, bMinimized);
	
}

BOOL CTestDlg::OnInitDialog() 

{
	CGrayDlg::OnInitDialog();
	
	m_tri.upside = true;

	//mxpad.DumpEdS(GetDlgItem(IDC_EDIT1));
	//mxpad.DumpStyles(GetDlgItem(IDC_EDIT1));
	//mxpad.DumpExStyles(GetDlgItem(IDC_EDIT1));

	CString str; 

	for(int loop = 0; loop < 5; loop++)
		{
		str.Format("Loop number %d", loop);
		//m_tlist.AddStr(str);
		m_list.AddString(str);
		}

	//m_tlist.AddStr("Long long  long  long  long  long  long  long loop list string text item");
	m_list.AddString("Long  long  long  long  long  long  long long loop list string text item");
	
	for(loop = 0; loop < 5; loop++)
		{
		str.Format("Loop number %d", loop);
		//m_tlist.AddStr(str);
		m_list.AddString(str);
		}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CTestDlg::PreTranslateMessage(MSG* pMsg) 

{

	if(pMsg->message == WM_TRIREL1)
		{
		switch(pMsg->wParam)
			{
			case IDC_TRI:
				P2N("WM_TRIREL1  TRI id=%d pos=%d\r\n", pMsg->wParam, pMsg->lParam);
				break;

			case IDC_TRI2:
				P2N("WM_TRIREL1 TRI2 id=%d pos=%d\r\n", pMsg->wParam, pMsg->lParam);
				break;
			}
		}

	if(pMsg->message == WM_TRIREL2)
		{
		switch(pMsg->wParam)
			{
			case IDC_TRI:
				P2N("WM_TRIREL2  TRI id=%d pos=%d\r\n", pMsg->wParam, pMsg->lParam);
				break;

			case IDC_TRI2:
				P2N("WM_TRIREL2 TRI2 id=%d pos=%d\r\n", pMsg->wParam, pMsg->lParam);
				break;
			}
		}

	if(pMsg->message == WM_TRICHANGE1)
		{
		switch(pMsg->wParam)
			{
			case IDC_TRI:
				P2N("WM_TRICHANGE1  TRI id=%d pos=%d\r\n", pMsg->wParam, pMsg->lParam);
				break;

			case IDC_TRI2:
				P2N("WM_TRICHANGE1 TRI2 id=%d pos=%d\r\n", pMsg->wParam, pMsg->lParam);
				break;
			}
		}

	if(pMsg->message == WM_TRICHANGE2)
		{
		switch(pMsg->wParam)
			{
			case IDC_TRI:
				P2N("WM_TRICHANGE2  TRI id=%d pos=%d\r\n", pMsg->wParam, pMsg->lParam);
				break;

			case IDC_TRI2:
				P2N("WM_TRICHANGE2 TRI2 id=%d pos=%d\r\n", pMsg->wParam, pMsg->lParam);
				break;
			}
		}
	
	
	return CGrayDlg::PreTranslateMessage(pMsg);
}

void CTestDlg::OnButton1() 

{
	EndDialog(IDOK);	
}
