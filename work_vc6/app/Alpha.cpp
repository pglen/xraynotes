// Alpha.cpp : implementation file
//

#include "stdafx.h"
#include "xraynotes.h"
#include "Alpha.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAlpha dialog


CAlpha::CAlpha(CWnd* pParent /*=NULL*/)
	: CGrayDlg(CAlpha::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAlpha)
	m_ed1 = _T("");
	//}}AFX_DATA_INIT
}


void CAlpha::DoDataExchange(CDataExchange* pDX)
{
	CGrayDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlpha)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_can);
	DDX_Control(pDX, IDC_SLIDER2, m_slider1);
	DDX_Text(pDX, IDC_EDIT1, m_ed1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAlpha, CGrayDlg)
	//{{AFX_MSG_MAP(CAlpha)
	//}}AFX_MSG_MAP

	ON_MESSAGE(WM_SLIDE1, OnSlide1)
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAlpha message handlers

BOOL CAlpha::OnInitDialog() 

{
	CGrayDlg::OnInitDialog();
	
	m_slider1.message = WM_SLIDE1;
	m_slider1.SetRange(0, 255);
	
	CShadesDlg *dlg; GMD(dlg); CXrayDoc *doc; GCD(doc, dlg);

	SetWindowText("Object Transparency");
	int pos = 128;
	
	if(doc && doc->m_currlay)
		pos = doc->m_currlay->gdata.alpha;
	
	m_ed1.Format("%d", pos); UpdateData(false);
	m_slider1.SetPos(pos);
	
	org = pos;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void	CAlpha::OnSlide1(int wParam, int lParam)

{
	//P2N("CAlpha::OnSlide1 %d pos=%d op=%d\r\n", wParam, LOWORD(lParam), HIWORD(lParam));

	int pos = LOWORD(lParam); 
	
	m_ed1.Format("%d", pos); UpdateData(false);
	Apply(pos);
}

void CAlpha::Apply(int val)

{
	CShadesDlg *dlg; GMD(dlg); CXrayDoc *doc; GCDR(doc, dlg);
		
	int lsize = doc->m_layers.GetSize();
	for(int looplay = 0; looplay < lsize; looplay++)
		{
		CLayer *xlay = (CLayer*)doc->m_layers[looplay];
		ASSERT(xlay->magic == LAYER_MAGIC);

		if(xlay->focus)
			{
			((CLayText*)xlay)->gdata.alpha = val;
			xlay->Invalidate();
			}
		}
}

void	CAlpha::OnCancel()

{
	//P2N("CAlpha::OnCancel\r\n");
	Apply(org);
	CGrayDlg::OnCancel();
}

