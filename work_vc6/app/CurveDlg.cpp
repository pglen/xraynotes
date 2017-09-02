// CurveDlg.cpp : implementation file
//

#include "stdafx.h"
#include "xraynotes.h"
#include "CurveDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCurveDlg dialog


CCurveDlg::CCurveDlg(CWnd* pParent /*=NULL*/)
	: CGrayDlg(CCurveDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCurveDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	windowname = "CCurveDlg"; //support.GetFileNameExt(__FILE__);
}

void CCurveDlg::DoDataExchange(CDataExchange* pDX)
{
	CGrayDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCurveDlg)
	DDX_Control(pDX, IDC_BUTTON3, m_b3);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_can);
	DDX_Control(pDX, IDC_BUTTON2, m_b2);
	DDX_Control(pDX, IDC_BUTTON1, m_b1);
	DDX_Control(pDX, IDC_TRI2, m_tri2);
	DDX_Control(pDX, IDC_CURVE, m_cur);
	DDX_Control(pDX, IDC_TRI, m_tri);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCurveDlg, CGrayDlg)
	//{{AFX_MSG_MAP(CCurveDlg)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_WM_ACTIVATE()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCurveDlg message handlers

BOOL CCurveDlg::OnInitDialog() 

{
	CGrayDlg::OnInitDialog();
	
	m_title = "Color Curve / Brighness Curve Dialog";

	m_tri2.upside  = true;
	m_tri2.tiptext = "Input Range Selection";
	m_tri.tiptext  = "Output Range Selection";

	((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(false);
	((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(true);
	((CButton*)GetDlgItem(IDC_RADIO3))->SetCheck(false);

	CenterWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCurveDlg::PreTranslateMessage(MSG* pMsg) 
{
	//if(pMsg->message != WM_PAINT && pMsg->message != WM_MOUSEMOVE)
	//	P2N("CCurveDlg::PreTranslateMessage %p %s\r\n", this, mxpad.num2msg(pMsg->message));

	//if(pMsg->message == WM_SHOWWINDOW)
	//	{
	//	P2N("Show Window\r\n");
	//	}

	if(pMsg->message == WM_TRIREL1)
		{
		switch(pMsg->wParam)
			{
			case IDC_TRI:
				P2N("WM_TRIREL1  TRI id=%d pos=%d\r\n", pMsg->wParam, pMsg->lParam);
				m_cur.lowlim = pMsg->lParam;
				m_cur.SetHist();
				break;

			case IDC_TRI2:
				P2N("WM_TRIREL1 TRI2 id=%d pos=%d\r\n", pMsg->wParam, pMsg->lParam);
				m_cur.low = pMsg->lParam;
				m_cur.SetHist();
				break;
			}
		
		}

	if(pMsg->message == WM_TRIREL2)
		{
		//P2N("WM_TRIREL2 id=%d pos=%d\r\n", pMsg->wParam, pMsg->lParam);

		switch(pMsg->wParam)
			{
			case IDC_TRI:
				P2N("WM_TRIREL2  TRI id=%d pos=%d\r\n", pMsg->wParam, pMsg->lParam);
				m_cur.highlim = pMsg->lParam;
				m_cur.SetHist();
				break;

			case IDC_TRI2:
				P2N("WM_TRIREL2 TRI2 id=%d pos=%d\r\n", pMsg->wParam, pMsg->lParam);
				m_cur.high = pMsg->lParam;
				m_cur.SetHist();
				break;
			}
		}
	return CGrayDlg::PreTranslateMessage(pMsg);
}

void CCurveDlg::OnPaint() 

{
	//CPaintDC dc(this); // device context for painting
	CGrayDlg::OnPaint();
}

void CCurveDlg::OnLButtonDown(UINT nFlags, CPoint point) 

{
	P2N("CCurveDlg::OnLButtonDown x=%d y=%d\r\n", point.x, point.y );
	CGrayDlg::OnLButtonDown(nFlags, point);
}

void CCurveDlg::OnButton1() 

{
	m_cur.ResetCurve();
	
	m_tri.Reset();
	m_tri.InvalidateParent();

	m_tri2.Reset();
	m_tri2.InvalidateParent();

	m_cur.low = m_cur.lowlim = 0;
	m_cur.high = m_cur.highlim = 255;

	m_cur.ResetImg();

	P2N("CCurveDlg::OnButton1 => Reset to original\r\n");
}

void CCurveDlg::OnButton2() 

{
	m_cur.SmoothCurve();
	m_cur.SetHist();
}

void CCurveDlg::OnButton3() 

{

}

void CCurveDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 

{
	CGrayDlg::OnActivate(nState, pWndOther, bMinimized);
	//P2N("CCurveDlg::OnActivate\r\n");	
}

void CCurveDlg::OnShowWindow(BOOL bShow, UINT nStatus) 

{
	//P2N("CCurveDlg::OnShowWindow bShow=%d\r\n", bShow);	
	CGrayDlg::OnShowWindow(bShow, nStatus);
	
	CShadesDlg *shadesdlg = (CShadesDlg* )AfxGetApp()->m_pMainWnd;
	ASSERT(shadesdlg->magic == SHADE_MAGIC);

	if(shadesdlg->current >= 0)
		{	
		CXrayDoc *doc = ((CXrayDoc*)shadesdlg->docarr[shadesdlg->current]);

		if(doc)
			{
			doc->org->Copy(*doc->render);
			}
		}
}


void CCurveDlg::OnButton4() 
{

	
}
