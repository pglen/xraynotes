// PatDmg.cpp : implementation file
//

#include "stdafx.h"
#include "xraynotes.h"
#include "PatDmg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPatDmg dialog


CPatDmg::CPatDmg(CWnd* pParent /*=NULL*/)
	: CDialog(CPatDmg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPatDmg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPatDmg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPatDmg)
	DDX_Control(pDX, IDC_BUTTON34, m_b34);
	DDX_Control(pDX, IDC_BUTTON33, m_b33);
	DDX_Control(pDX, IDC_BUTTON32, m_b32);
	DDX_Control(pDX, IDC_BUTTON31, m_b31);
	DDX_Control(pDX, IDC_BUTTON30, m_b30);
	DDX_Control(pDX, IDC_BUTTON29, m_b29);
	DDX_Control(pDX, IDC_BUTTON28, m_b28);
	DDX_Control(pDX, IDC_BUTTON27, m_b27);
	DDX_Control(pDX, IDC_BUTTON26, m_b26);
	DDX_Control(pDX, IDC_BUTTON25, m_b25);
	DDX_Control(pDX, IDC_BUTTON24, m_b24);
	DDX_Control(pDX, IDC_BUTTON23, m_b23);
	DDX_Control(pDX, IDC_BUTTON22, m_b22);
	DDX_Control(pDX, IDC_BUTTON21, m_b21);
	DDX_Control(pDX, IDC_BUTTON19, m_b19);
	DDX_Control(pDX, IDC_BUTTON18, m_b18);
	DDX_Control(pDX, IDC_BUTTON17, m_b17);
	DDX_Control(pDX, IDC_BUTTON16, m_b16);
	DDX_Control(pDX, IDC_BUTTON14, m_b14);
	DDX_Control(pDX, IDC_BUTTON13, m_b13);
	DDX_Control(pDX, IDC_BUTTON5, m_b5);
	DDX_Control(pDX, IDC_BUTTON4, m_b4);
	DDX_Control(pDX, IDC_BUTTON3, m_b3);
	DDX_Control(pDX, IDC_BUTTON2, m_b2);
	DDX_Control(pDX, IDC_BUTTON1, m_b1);
	DDX_Control(pDX, IDC_HEAD, m_head);
	DDX_Control(pDX, IDCANCEL, m_can);
	DDX_Control(pDX, IDOK, m_ok);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPatDmg, CDialog)
	//{{AFX_MSG_MAP(CPatDmg)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPatDmg message handlers

void CPatDmg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	RECT rc; GetClientRect(&rc);

	// Draw header
	//dc.FillSolidRect(2, 2, rc.right - 4, 16, RGB(64, 64, 64));

	// Draw corner close
	dc.FillSolidRect(rc.right - 17, 2, rc.right - 4, 16, RGB(32, 32, 32));

	CPen pen; pen.CreatePen(PS_SOLID, 2, RGB(255,255,255));
	CPen *oldpen;

	oldpen = (CPen*)dc.SelectObject(pen);

	dc.MoveTo( rc.right - 14, 14);
	dc.LineTo( rc.right - 4, 4);

	dc.MoveTo( rc.right - 14, 4);
	dc.LineTo( rc.right - 4, 14);
	
	// Do not call CDialog::OnPaint() for painting messages
}



void CPatDmg::OnLButtonDown(UINT nFlags, CPoint point) 

{

	RECT rc; GetClientRect(&rc);
	CRect rect(rc.right - 17, 2, rc.right - 4, 16);

	if(rect.PtInRect(point))
		{
		P2N("Click in close box\r\n");
		EndDialog(IDOK);
		//ShowWindow(SW_HIDE);
		return;
		}

	CRect rect2(rc.left, rc.top, rc.right, 24);
	if(rect2.PtInRect(point))
		{
		P2N("Click in header box\r\n");
		PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));	
		return;
		}	
	CDialog::OnLButtonDown(nFlags, point);
}


BOOL CPatDmg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
		BYTE	bAlpha = 220;

	if (bAlpha < 255)
		{
        //  Set layered style for the dialog
        SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);

        //  Call it with 255 as alpha - opacity
		if(g_pSetLayeredWindowAttributes)
			g_pSetLayeredWindowAttributes(m_hWnd, 0, bAlpha, LWA_ALPHA);
		}
	

	m_head.SetTrans();
	m_head.SetFontSize(24);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
