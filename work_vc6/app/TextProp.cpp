
/* =====[ TextProp.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the TextProp.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  12/9/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// TexttProp.cpp : implementation file
//

#include "stdafx.h"
#include "xraynotes.h"
#include "TextProp.h"
#include "PicSel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_SLIDE1	WM_USER + 10
#define WM_SLIDE2	WM_USER + 11
#define WM_SLIDE3	WM_USER + 12
#define WM_SLIDE4	WM_USER + 13

/////////////////////////////////////////////////////////////////////////////
// CTextProp dialog

CTextProp::CTextProp(CWnd* pParent /*=NULL*/)
	: CGrayDlg(CTextProp::IDD, pParent)
{
	SetAlpha(245);

	in_timer = 0;

	m_centerdlg = true;
	noremember = true;

	m_slide1.message = WM_SLIDE1;
	m_slide2.message = WM_SLIDE2;
	m_slide3.message = WM_SLIDE3;
	m_slide4.message = WM_SLIDE4;

	//{{AFX_DATA_INIT(CTextProp)
	m_main = _T("");
	m_tip = _T("");
	m_ed5 = _T("");
	m_ed6 = _T("");
	m_ed7 = _T("");
	m_ed11 = _T("");
	m_ed12 = _T("");
	//}}AFX_DATA_INIT

	windowname = "CTextProp";
}

void CTextProp::DoDataExchange(CDataExchange* pDX)

{
	CGrayDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTextProp)
	DDX_Control(pDX, IDC_SLIDER6, m_slide6);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_can);
	DDX_Control(pDX, IDC_BUTTON9, m_b9);
	DDX_Control(pDX, IDC_BUTTON8, m_b8);
	DDX_Control(pDX, IDC_BUTTON2, m_b2);
	DDX_Control(pDX, IDC_BUTTON1, m_b1);
	DDX_Control(pDX, IDC_SLIDER4, m_slide4);
	DDX_Control(pDX, IDC_SLIDER3, m_slide3);
	DDX_Control(pDX, IDC_SLIDER2, m_slide2);
	DDX_Control(pDX, IDC_SLIDER1, m_slide1);
	DDX_Control(pDX, IDC_EDIT2, m_edit2);
	DDX_Control(pDX, IDC_EDIT1, m_edit1);
	DDX_Text(pDX, IDC_EDIT1, m_main);
	DDX_Text(pDX, IDC_EDIT2, m_tip);
	DDX_Text(pDX, IDC_EDIT5, m_ed5);
	DDX_Text(pDX, IDC_EDIT6, m_ed6);
	DDX_Text(pDX, IDC_EDIT7, m_ed7);
	DDX_Text(pDX, IDC_EDIT11, m_ed11);
	DDX_Text(pDX, IDC_EDIT12, m_ed12);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTextProp, CGrayDlg)
	//{{AFX_MSG_MAP(CTextProp)
	ON_WM_HSCROLL()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON8, OnButton8)
	ON_EN_CHANGE(IDC_EDIT1, OnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON9, OnButton9)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_CHECK4, OnCheck4)
	//}}AFX_MSG_MAP
	
	ON_MESSAGE(WM_SLIDE1, OnSlide1)
	ON_MESSAGE(WM_SLIDE2, OnSlide2)
	ON_MESSAGE(WM_SLIDE3, OnSlide3)
	ON_MESSAGE(WM_SLIDE4, OnSlide4)

END_MESSAGE_MAP()


void	CTextProp::OnSlide1(int wParam, int lParam)

{
	//P2N("CTextProp::OnSlide1 %d pos=%d op=%d\r\n", wParam, LOWORD(lParam), HIWORD(lParam));
}

void	CTextProp::OnSlide2(int wParam, int lParam)

{
	//P2N("CTextProp::OnSlide2 %d pos=%d op=%d\r\n", wParam, LOWORD(lParam), HIWORD(lParam));
}

void	CTextProp::OnSlide3(int wParam, int lParam)

{
	//P2N("CTextProp::OnSlide3 %d pos=%d op=%d\r\n", wParam, LOWORD(lParam), HIWORD(lParam));
}

void	CTextProp::OnSlide4(int wParam, int lParam)

{
	//P2N("CTextProp::OnSlide4 %d pos=%d op=%d\r\n", wParam, LOWORD(lParam), HIWORD(lParam));
}

/////////////////////////////////////////////////////////////////////////////
// CTextProp message handlers

BOOL CTextProp::OnInitDialog() 

{
	CGrayDlg::OnInitDialog();

	orgm  = m_main = lay->text;
	orgt = m_tip = lay->tiptext;

	lay->old_txdata = lay->txdata;
	lay->old_gdata	= lay->gdata;

	//orgb = lay->gdata.bg_color;
	//orgf = lay->gdata.fg_color;

	((CSliderCtrl*)GetDlgItem(IDC_SLIDER1))->SetRange(0, 100);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER2))->SetRange(0, 100);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER3))->SetRange(0, 255);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER4))->SetRange(0, 511);

	// Adapt the slider to 
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER6))->SetRange(0, 2 * lay->gdata.ww);
	
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER6))->SetPos(lay->txdata.text_round);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER3))->SetPos(lay->gdata.alpha);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER2))->SetPos(lay->txdata.text_paddingx);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER1))->SetPos(lay->txdata.text_paddingy);
	
	m_ed5.Format("%d", lay->txdata.text_paddingx);
	m_ed6.Format("%d", lay->txdata.text_paddingy);
	m_ed11.Format("%d", lay->gdata.alpha);
	m_ed12.Format("%d", lay->txdata.text_round);

	// Find closest color ???
	m_ed7.Format("%d", 0);

	// Update checks
	((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(lay->txdata.text_3d);
	((CButton*)GetDlgItem(IDC_CHECK4))->SetCheck(lay->gdata.protect);

	UpdateData(false);
	
	// Calulate popup position
	int left = true;
	CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);
	
	RECT  rc; dlg->GetWindowRect(&rc);
	
	//	-----------------
	//	|	|	|	|	|
	//	|	|	|	|	|
	//	|	|	|	|	|
	//      q1x		q3x

	int midx = (rc.right-rc.left)/2;
	int midy = (rc.bottom-rc.top)/2;

	RECT  clrc; GetWindowRect(&clrc);
	int clw		= clrc.right - clrc.left;
	int clmidy	= (clrc.bottom - clrc.top) / 2;

	CXrayDoc *doc = ((CShadesDlg* )AfxGetApp()->m_pMainWnd)->GetCurrDoc();
	if(doc && doc->m_currlay)
		{
		int midlayx = doc->m_currlay->gdata.xx + doc->m_currlay->gdata.ww / 2;

		if(midlayx > midx)
			{		
			P2N("Popping CTextProp to the left\r\n");
			SetWindowPos(NULL, doc->m_currlay->gdata.xx - clw - 20, midy - clmidy,
							0, 0,	SWP_NOSIZE| SWP_NOZORDER );
			}
		else
			{	
			P2N("Popping CTextProp to the right\r\n");
			SetWindowPos(NULL, doc->m_currlay->gdata.xx + 
								doc->m_currlay->gdata.ww + 20, midy - clmidy,
							0, 0,	SWP_NOSIZE| SWP_NOZORDER );
			}
		}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTextProp::OnOK()

{
	UpdateData();

	//P2N("m_main=%s\r\n", m_main);
	
	lay->SetNewText(m_main);
	lay->tiptext = m_tip;

	lay->Invalidate();

	CGrayDlg::OnOK();
}

void CTextProp::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 

{
	//P2N("CTextProp::OnHScroll %d\r\n", nPos);

	if(nSBCode == SB_ENDSCROLL || nSBCode == SB_THUMBTRACK) //	|| nSBCode == SB_THUMBPOSITION)
		{
		if(pScrollBar == GetDlgItem(IDC_SLIDER6))
			{
			int pos = ((CSliderCtrl*)GetDlgItem(IDC_SLIDER6))->GetPos();
			
			m_ed12.Format("%d", pos);
			UpdateData(false);

			Apply(APPLY_ROUND, pos);	
			}

		if(pScrollBar == GetDlgItem(IDC_SLIDER3))
			{
			int pos = ((CSliderCtrl*)GetDlgItem(IDC_SLIDER3))->GetPos();

			m_ed11.Format("%d", pos);
			UpdateData(false);
			
			Apply(APPLY_TRANS, pos);
			}

		if(pScrollBar == GetDlgItem(IDC_SLIDER4))
			{
			int pos = ((CSliderCtrl*)GetDlgItem(IDC_SLIDER4))->GetPos();

			m_ed7.Format("%d", pos);
			UpdateData(false);

			lay->gdata.bg_color = ColorLookup(pos);
			lay->Invalidate();
			}

		if(pScrollBar == GetDlgItem(IDC_SLIDER1) || 
			pScrollBar == GetDlgItem(IDC_SLIDER2) )
			{
			int posx, posy;

			if(misc.IsShiftDown())
				{
				if(pScrollBar == GetDlgItem(IDC_SLIDER1))
					{
					posx = posy = ((CSliderCtrl*)GetDlgItem(IDC_SLIDER1))->GetPos();
					}
				else
					{
					posx = posy = ((CSliderCtrl*)GetDlgItem(IDC_SLIDER2))->GetPos();
					}
				((CSliderCtrl*)GetDlgItem(IDC_SLIDER1))->SetPos(posx);
				((CSliderCtrl*)GetDlgItem(IDC_SLIDER2))->SetPos(posx);
				}
			else
				{
				posx = ((CSliderCtrl*)GetDlgItem(IDC_SLIDER2))->GetPos();
				posy = ((CSliderCtrl*)GetDlgItem(IDC_SLIDER1))->GetPos();
				}

			m_ed5.Format("%d", posx);
			m_ed6.Format("%d", posy);

			UpdateData(false);

			Apply(APPLY_PAD, posx, posy);
			}
		}

	CGrayDlg::OnHScroll(nSBCode, nPos, pScrollBar);
}



void CTextProp::OnTimer(UINT nIDEvent) 

{
	KillTimer(nIDEvent);

	if(in_timer)
		return;

	in_timer = true;
	UpdateData();
	//P2N("Idle timer %s\r\n", m_main);
	lay->SetNewText(m_main);
	lay->Invalidate();

	CGrayDlg::OnTimer(nIDEvent);

	in_timer = false;

}

//////////////////////////////////////////////////////////////////////////

BOOL CTextProp::PreTranslateMessage(MSG* pMsg) 

{
	// Correct the dead static mechanism

#if 0

	if(pMsg->message == WM_SYSKEYDOWN)
		{
		//P2N("CTextProp::PreTranslateMessage %d (%c)\r\n", pMsg->wParam, pMsg->wParam);
	
		if(pMsg->wParam == 'M')
			{
			m_edit1.SetFocus();
			return true;
			}

		if(pMsg->wParam == 'T')
			{
			m_edit2.SetFocus();
			return true;
			}
		
		if(pMsg->wParam == 'R')
			{
			m_slide3.SetFocus();
			return true;
			}
	
		if(pMsg->wParam == 'W')
			{
			m_slide2.SetFocus();
			return true;
			}
		}
#endif
	
	return CGrayDlg::PreTranslateMessage(pMsg);
}

void CTextProp::OnCancel()

{
	CRect oldrc; lay->GetRect(&oldrc);

	lay->txdata = lay->old_txdata;
	lay->gdata = lay->old_gdata;
	
	lay->SetNewText(orgm);
	lay->tiptext = orgt;

	CRect newrc; lay->GetRect(&newrc);

	lay->updaterect = oldrc | newrc;

	lay->Invalidate();

	CGrayDlg::OnCancel();
}


COLORREF	CTextProp::ColorLookup(int idx)

{
	int		rr,gg,bb;

	//for(int loop = 0; loop < 85; loop++)
	
	rr = ((idx >> 0) & 0x7) * 32;
	gg = ((idx >> 3) & 0x7) * 32;
	bb = ((idx >> 6) & 0x7) * 32;

	//P2N("idx=%d rr=%d gg=%d bb=%d\r\n", idx, rr, gg, bb);

	return( RGB(rr,gg,bb) );
}

//////////////////////////////////////////////////////////////////////////

void CTextProp::OnButton2() 

{
	CFontDialog	dlg;

	//m_font->GetLogFont(&m_logfont);

	dlg.m_cf.lpLogFont = &lay->text_logfont;
	dlg.m_cf.rgbColors = lay->gdata.fg_color;

	dlg.m_cf.Flags |= CF_INITTOLOGFONTSTRUCT;

	if (dlg.DoModal() == IDOK)
		{
		lay->gdata.fg_color = dlg.m_cf.rgbColors;
		lay->SetNewFont();
		lay->Invalidate();
		}	
}

void CTextProp::OnButton1() 

{
	CPicSel ps;	

	ps.lay = lay;
	ps.liveptr = &lay->gdata.bg_color;
	ps.uppd = PIC_APPLY_BG;

	ps.DoModal();

	Apply(APPLY_BG, lay->gdata.bg_color);
}


void CTextProp::OnButton8() 

{
	CPicSel ps;	

	ps.lay = lay;
	ps.liveptr = &lay->gdata.bg_color;

	ps.DoModal();	
}

void CTextProp::OnChangeEdit1() 

{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CGrayDlg::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here

	SetTimer(1, 300, NULL);
}

void CTextProp::OnButton9() 

{
	//lay->gdata.bg_color = orgb;
	//lay->gdata.bg_color = orgf;
	
	lay->txdata = lay->old_txdata;

	lay->Invalidate();
}

void CTextProp::OnCheck1() 

{
	Apply(APPLY_3D, ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck());

	//lay->Invalidate();
}


void CTextProp::OnButton3() 

{
	m_main = orgm;

	UpdateData(false);
	lay->SetNewText(orgm);
	lay->Invalidate();
	
	m_edit1.SetFocus();

}

void CTextProp::OnButton4() 

{
	m_tip = orgt;

	UpdateData(false);

	lay->tiptext = orgt;
	lay->Invalidate();

	m_edit2.SetFocus();
}

void CTextProp::OnCheck4() 

{
	if( ((CButton*)GetDlgItem(IDC_CHECK4))->GetCheck())	
		{
		lay->gdata.protect = true;
		}
	else 
		{
		lay->gdata.protect = false;
		}

	lay->Invalidate();
}

//////////////////////////////////////////////////////////////////////////
// Set all text that has focus
	
void CTextProp::Apply(int offset, int val1, int val2)

{
	CShadesDlg *dlg; GMD(dlg); CXrayDoc *doc; GCDR(doc, dlg);
		
	int lsize = doc->m_layers.GetSize();
	for(int looplay = 0; looplay < lsize; looplay++)
		{
		CLayer *xlay = (CLayer*)doc->m_layers[looplay];
		ASSERT(xlay->magic == LAYER_MAGIC);

		if((xlay->gdata.kind == LAYER_TEXT && xlay->focus) || xlay == lay)
			{
			switch(offset)
				{
				case APPLY_TRANS:
					((CLayText*)xlay)->gdata.alpha = val1;
					break;

				case APPLY_ROUND:
					((CLayText*)xlay)->txdata.text_3d = false;						
					((CLayText*)xlay)->txdata.text_round = val1;
					break;

				case APPLY_3D:
					if(val1)
						{
						((CLayText*)xlay)->txdata.text_round = 0;
						((CLayText*)xlay)->txdata.text_3d = true;
						}
					else 
						{
						int pos = ((CSliderCtrl*)GetDlgItem(IDC_SLIDER6))->GetPos();
						if(pos)
							((CLayText*)xlay)->txdata.text_round = pos;

						((CLayText*)xlay)->txdata.text_3d = false;
						}
					break;

				case APPLY_PAD:
					((CLayText*)xlay)->SetNewPadding(val1, val2);
					break;

				case APPLY_BG:
					((CLayText*)xlay)->gdata.bg_color = val1;
					break;

				case APPLY_FG:
					((CLayText*)xlay)->gdata.bg_color = val1;
					break;
				}
			xlay->Invalidate();
			}
		}
}

