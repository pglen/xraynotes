// PlotDlg.cpp : implementation file
//

#include "stdafx.h"
#include "xraynotes.h"
#include "shades.h"
#include "shadesdlg.h"

#include "PlotDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlotDlg dialog


CPlotDlg::CPlotDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPlotDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPlotDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	created = false;
}


void CPlotDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPlotDlg)
	DDX_Control(pDX, IDC_PLOT, m_plot);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPlotDlg, CDialog)
	//{{AFX_MSG_MAP(CPlotDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlotDlg message handlers

void CPlotDlg::Show()
{
	if(!IsWindow(m_hWnd))
		{
		Create(IDD);
		created = true;
		}

	ShowWindow(SW_SHOW);
}
