
/* =====[ PicSel.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the PicSel.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  12/10/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// PicSel.cpp : implementation file
//

#include "stdafx.h"
#include "xraynotes.h"

#include "xtargetbutton.h"
#include "label.h"
#include "lines.h"

#include "PicSel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPicSel dialog


CPicSel::CPicSel(CWnd* pParent /*=NULL*/)
	: CGrayDlg(CPicSel::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPicSel)
	m_r = _T("");
	m_g = _T("");
	m_b = _T("");
	//}}AFX_DATA_INIT

	labcount = 0;
	m_lines = 0;
	m_showalt = 1;
	slider = false;
	uppd = PIC_APPLY_NONE;
	m_centerdlg = true;
	liveptr = NULL;
	lay		= NULL;

	windowname = "CPicSel";
}

void CPicSel::DoDataExchange(CDataExchange* pDX)
{
	CGrayDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPicSel)
	DDX_Control(pDX, IDC_BUTTON1, m_b1);
	DDX_Control(pDX, IDC_CURR2, m_curr2);
	DDX_Control(pDX, IDC_SLIDER3, m_slider3);
	DDX_Control(pDX, IDC_SLIDER2, m_slider2);
	DDX_Control(pDX, IDC_SLIDER1, m_slider1);
	DDX_Control(pDX, IDC_CURR, m_curr);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Text(pDX, IDC_EDIT1, m_r);
	DDX_Text(pDX, IDC_EDIT2, m_g);
	DDX_Text(pDX, IDC_EDIT3, m_b);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPicSel, CGrayDlg)
	//{{AFX_MSG_MAP(CPicSel)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
	ON_BN_CLICKED(IDC_CHECK5, OnCheck5)
	ON_BN_CLICKED(IDC_CHECK4, OnCheck4)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_EN_KILLFOCUS(IDC_EDIT1, OnKillfocusEdit1)
	ON_EN_KILLFOCUS(IDC_EDIT2, OnKillfocusEdit2)
	ON_EN_KILLFOCUS(IDC_EDIT3, OnKillfocusEdit3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPicSel message handlers

BOOL CPicSel::OnInitDialog() 

{
	SetAlpha(245);
	m_title = "Color Selection";

	CGrayDlg::OnInitDialog();
	
	memset(cr, 0, 256 * sizeof(COLORREF));
	memset(labels, 0, sizeof(labels));

	((CSliderCtrl*)GetDlgItem(IDC_SLIDER1))->SetRange(0, 255);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER2))->SetRange(0, 255);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER3))->SetRange(0, 255);
	
	CreatePics();

	if(liveptr)
		m_color = *liveptr;

	UpdateUI(true);

	old_col = m_color;

	m_curr2.SetBkColor(old_col);	m_curr2	.Invalidate();
		
	return TRUE;
}

//////////////////////////////////////////////////////

void CPicSel::OnLButtonDown(UINT nFlags, CPoint point) 

{
	//P2N("CPicSel::OnLButtonDown: x=%d y=%d\r\n", point.x, point.y);

	int found = 0;

	for(int loop1 = 0; loop1 < labcount; loop1++)
		{	
		CRect rc = ra[loop1];
		if(rc.PtInRect(point))
			{
			m_color = cr[loop1];
			
			UpdateUI(true);
			LiveUpdate();
		
			found = true;
			break;
			}
		}

	if(!found)
		{
		CRect rc; m_curr2.GetWindowRect(&rc); ScreenToClient(&rc);

		if(rc.PtInRect(point))
			{
			//P2N("Click in org rect\r\n");
			m_color = old_col;
			UpdateUI(true);	LiveUpdate();	
			}
		}

	CGrayDlg::OnLButtonDown(nFlags, point);
}

void CPicSel::OnLButtonDblClk(UINT nFlags, CPoint point) 

{
	//CGrayDlg::OnLButtonDblClk(nFlags, point);

	// Double click color
	for(int loop1 = 0; loop1 < labcount; loop1++)
		{	
		CRect rc = ra[loop1];
		if(rc.PtInRect(point))
			{
			OnOK();
			break;
			}
		}
}

CPicSel::~CPicSel()

{
	//P2N("Destruct\r\n");
}

void CPicSel::OnOK() 

{
	//m_color = RGB(atoi(m_r), atoi(m_g), atoi(m_b));
	//UpdateData(true);
	
	LiveUpdate();

	CGrayDlg::OnOK();
}

BOOL CPicSel::DestroyWindow() 

{
	P2N("CPicSel::Destroy\r\n");

	// Free resources
	for(int loop1 = 0; loop1 < labcount; loop1++)
		if(labels[loop1])
			delete labels[loop1];

	return CGrayDlg::DestroyWindow();
}

//////////////////////////////////////////////////////////////////////////

void CPicSel::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 

{
	P2N("CPicSel::OnHScroll\r\n");

	if(nSBCode == SB_ENDSCROLL || nSBCode == SB_THUMBTRACK) //	|| nSBCode == SB_THUMBPOSITION)
		{
		// Timer scroll
		int rr = ((CSliderCtrl*)GetDlgItem(IDC_SLIDER1))->GetPos();
		int gg = ((CSliderCtrl*)GetDlgItem(IDC_SLIDER2))->GetPos();
		int bb = ((CSliderCtrl*)GetDlgItem(IDC_SLIDER3))->GetPos();
		
		m_color = RGB(rr, gg, bb);

		UpdateUI(false);
		LiveUpdate();
		}

	CGrayDlg::OnHScroll(nSBCode, nPos, pScrollBar);
}

//////////////////////////////////////////////////////////////////////////
//

void CPicSel::LiveUpdate()

{	
	P2N("CPicSel::LiveUpdate()\r\n");

	if(!lay || !liveptr)
		{
		P2N("CPicSel::LiveUpdate Missing layer or live pointer\r\n");
		return;
		}

	*liveptr = (int)m_color;
	lay->Invalidate();	

	// Also update other selected objects
	if(uppd == 	PIC_APPLY_NONE)
		return;

	CShadesDlg *dlg; GMD(dlg); CXrayDoc *doc; GCDR(doc, dlg);
	
	int lsize = doc->m_layers.GetSize();
	for(int looplay = 0; looplay < lsize; looplay++)
		{
		CLayer *xlay = (CLayer*)doc->m_layers[looplay];
		ASSERT(xlay->magic == LAYER_MAGIC);

	
		if(xlay->gdata.kind == LAYER_TEXT && xlay->focus)
			{
				P2N("liveupdate %d %p\r\n", uppd, xlay);

			switch(uppd)
				{
				case PIC_APPLY_BG:
					((CLayText*)xlay)->gdata.bg_color = m_color;
					break;

				case PIC_APPLY_FG:
					((CLayText*)xlay)->gdata.fg_color = m_color;		
					break;
				}
			xlay->Invalidate();
			}
		}
}

//////////////////////////////////////////////////////////////////////////

void CPicSel::OnCancel()

{
	P2N("CPicSel::Cancel\r\n");

	m_color = old_col;

	LiveUpdate();

	CGrayDlg::OnCancel();
}

void CPicSel::OnCheck2() 
{
	LiveUpdate();
}

void CPicSel::OnCheck5() 
{
	LiveUpdate();	
}

void CPicSel::OnCheck4() 
{
	LiveUpdate();	
}

//DEL void CPicSel::OnChangeEdit4() 
//DEL {
//DEL 	LiveUpdate();
//DEL }


void CPicSel::ShowCol(RECT &rc, int &loop1, int &cnt)

{
	int leftgap2 = 30;
	int topgap2 = 20;

	// Display colored boxes
	for(int loop2 = 0; loop2 <= 16; loop2++)
		{
		rc.top = topgap2 + (loop1 * 20);
		rc.bottom = rc.top + 19;

		rc.left = leftgap2 + (loop2 * 20);
		rc.right = rc.left + 19;

		ra[cnt] = rc;

		CLabel *pRec = new CLabel;
		//ss.Format("%d", cnt);
		pRec->Create("", WS_VISIBLE, rc, this);
		labels[cnt] = pRec;

		if(cr[cnt] == m_color)
			{
			pRec->SetBorder(true);
			}
		pRec->SetBkColor(cr[cnt]);

		cnt++;
		}
}

//////////////////////////////////////////////////////////////////////////

void	CPicSel::CreatePics()

{
	CLabel		*pRec;
	RECT	rc;
	int		loop1, loop2, loop3, loop4;

	int cnt = 0;
	// Create array of colors
	for(loop1 = 0; loop1 < 256; loop1+= 51)
		for(loop2 = 0; loop2 < 256; loop2 += 51)
			for(loop3 = 0; loop3 < 256; loop3 +=51)
				cr[cnt++] = RGB(loop3, loop2, loop1);

	int graystart = cnt;
	P2N("cnt = %d\r\n", cnt);

	// Add gray scale 
	for(loop4 = 0; loop4 <= 256; loop4 += 256/16)
		cr[cnt++] = RGBLIM(loop4, loop4, loop4);
	
	// Add colors
	for(loop4 = 0; loop4 <= 256; loop4+= 256/16)
		cr[cnt++] = RGBLIM(loop4, 0, 0);
	
	for(loop4 = 0; loop4 <= 256; loop4+= 256/16)
		cr[cnt++] = RGBLIM(0, loop4, 0);
	
	for(loop4 = 0; loop4 <= 256; loop4+= 256/16)
		cr[cnt++] = RGBLIM(0, 0, loop4);

	for(loop4 = 0; loop4 <= 256; loop4+= 256/16)
		cr[cnt++] = RGBLIM(0, loop4, loop4);

	for(loop4 = 0; loop4 <= 256; loop4+= 256/16)
		cr[cnt++] = RGBLIM(loop4, 0, loop4);

	for(loop4 = 0; loop4 <= 256; loop4+= 256/16)
		cr[cnt++] = RGBLIM(loop4, loop4, 0);

	for(loop4 = 0; loop4 <= 256; loop4+= 256/16)
		cr[cnt++] = RGBLIM(loop4, loop4/2, 0);

	ASSERT(cnt < MAXLABELS);

	// Display squares:
	int toppad = 30, leftpad = 20;
	
	cnt = 0;
	for(loop1 = 0; loop1 < 15; loop1++)
		{
		for(loop2 = 0; loop2 < 18; loop2++)
			{
			rc.top =  toppad + (loop1 * 20);
			rc.bottom = rc.top + 19;

			rc.left = leftpad + (loop2 * 20);
			rc.right = rc.left + 19;

			ra[cnt] = rc;

			if(cnt < 216)
				{				
				pRec = new CLabel;
				//ss.Format("%d", cnt);
				pRec->Create("", WS_VISIBLE, rc, this);
				labels[cnt] = pRec;

				if(cr[cnt] == m_color)
					{
					//pRec->SetWindowText(" x");
					pRec->SetBorder(true);
					}

				pRec->SetBkColor(cr[cnt]);

				cnt++;
				}
			else
				{
				goto doublebreak;
				//break;
				}
			}
		}

doublebreak:

	cnt = graystart;

	// Make space for it ...
	loop1++;
	
	ShowCol(rc, loop1, cnt); loop1++;
	ShowCol(rc, loop1, cnt); loop1++;
	ShowCol(rc, loop1, cnt); loop1++;
	ShowCol(rc, loop1, cnt); loop1++;
	ShowCol(rc, loop1, cnt); loop1++;
	ShowCol(rc, loop1, cnt); loop1++;
	ShowCol(rc, loop1, cnt); loop1++;
	ShowCol(rc, loop1, cnt); loop1++;
	
	labcount = cnt;

	P2N("Created %d labels\r\n", labcount);
}

//////////////////////////////////////////////////////////////////////////

void	CPicSel::UpdateUI(int updscr)

{	
	if(updscr)
		{
		((CSliderCtrl*)GetDlgItem(IDC_SLIDER1))->SetPos(GetRValue(m_color));
		((CSliderCtrl*)GetDlgItem(IDC_SLIDER2))->SetPos(GetGValue(m_color));
		((CSliderCtrl*)GetDlgItem(IDC_SLIDER3))->SetPos(GetBValue(m_color));
		}

	m_curr.SetBkColor(m_color);	m_curr.Invalidate();

	m_r.Format("%d", GetRValue(m_color));
	m_g.Format("%d", GetGValue(m_color));
	m_b.Format("%d", GetBValue(m_color));

	UpdateData(false);
}

void CPicSel::OnButton1() 

{
	CColorDialog cdlg(m_color, CC_ANYCOLOR | CC_RGBINIT | CC_FULLOPEN);

	cdlg.m_cc.rgbResult = m_color;

	if(cdlg.DoModal())
		{
		m_color = cdlg.m_cc.rgbResult;
		UpdateUI(true);
		LiveUpdate();
		}
}

void CPicSel::OnKillfocusEdit1() 

{
	UpdateData();

	int rr = atoi(m_r); rr = MIN(255, MAX(0,rr));
	m_color = RGB(rr, GetGValue(m_color), GetBValue(m_color));

	UpdateUI(true);	LiveUpdate();
}

void CPicSel::OnKillfocusEdit2() 

{
	UpdateData();

	int rr = atoi(m_g); rr = MIN(255, MAX(0,rr));
	m_color = RGB(GetRValue(m_color), rr, GetBValue(m_color));

	UpdateUI(true);	LiveUpdate();	
}

void CPicSel::OnKillfocusEdit3() 
{
	UpdateData();

	int rr = atoi(m_b); rr = MIN(255, MAX(0,rr));
	m_color = RGB(GetRValue(m_color), GetBValue(m_color), rr);

	UpdateUI(true);	LiveUpdate();		
}

