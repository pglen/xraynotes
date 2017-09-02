// ExcRep.cpp : implementation file
//

#include "stdafx.h"
#include "ftext.h"
#include "ExcRep.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExcRep dialog


CExcRep::CExcRep(CWnd* pParent /*=NULL*/)
	: CDialog(CExcRep::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExcRep)
	//}}AFX_DATA_INIT
}


void CExcRep::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExcRep)
	DDX_Control(pDX, IDC_EXC_TEXT, m_txt);
	DDX_Control(pDX, IDC_EXC_HEAD, m_head);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExcRep, CDialog)
	//{{AFX_MSG_MAP(CExcRep)
	ON_BN_CLICKED(IDOK2, OnOk2)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExcRep message handlers

BOOL CExcRep::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_head.SetFontSize(-24);
	m_head.CenterText();
	
	m_head.SetTrans();
	m_txt.SetTrans();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CExcRep::OnOk2() 

{
	EndDialog(-1);	
}

BOOL CExcRep::OnEraseBkgnd(CDC* pDC) 
{
	RECT rc; GetClientRect(&rc);
	
	pDC->FillSolidRect(&rc, RGB(255, 155, 155));
	
	return true;

	//return CDialog::OnEraseBkgnd(pDC);
}
