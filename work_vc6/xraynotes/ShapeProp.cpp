
/* =====[ ShapeProp.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the ShapeProp.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  1/29/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// ShapeProp.cpp : implementation file
//

#include "stdafx.h"
#include "xraynotes.h"
#include "ShapeProp.h"
#include "PicSel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShapeProp dialog

static CString strConfig = "LastColors";

CShapeProp::CShapeProp(CWnd* pParent /*=NULL*/)
	: CGrayDlg(CShapeProp::IDD, pParent)
{
	//{{AFX_DATA_INIT(CShapeProp)
	m_ed5 = _T("");
	m_ed3 = _T("");
	//}}AFX_DATA_INIT
}


void CShapeProp::DoDataExchange(CDataExchange* pDX)
{
	CGrayDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShapeProp)
	DDX_Control(pDX, IDC_BUTTON5, m_but5);
	DDX_Control(pDX, IDC_MRUL4, m_l4);
	DDX_Control(pDX, IDC_MRUL3, m_l3);
	DDX_Control(pDX, IDC_MRUB4, m_b4);
	DDX_Control(pDX, IDC_BUTTON4, m_but4);
	DDX_Control(pDX, IDC_MRUB3, m_b3);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_can);
	DDX_Control(pDX, IDC_BUTTON3, m_but3);
	DDX_Control(pDX, IDC_BUTTON2, m_but2);
	DDX_Control(pDX, IDC_MRUB2, m_b2);
	DDX_Control(pDX, IDC_MRUB1, m_b1);
	DDX_Control(pDX, IDC_MRUL2, m_l2);
	DDX_Control(pDX, IDC_MRUL1, m_l1);
	DDX_Text(pDX, IDC_EDIT5, m_ed5);
	DDX_Text(pDX, IDC_EDIT3, m_ed3);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CShapeProp, CGrayDlg)
	//{{AFX_MSG_MAP(CShapeProp)
	ON_BN_CLICKED(IDC_RADIO13, OnRadio13)
	ON_BN_CLICKED(IDC_RADIO14, OnRadio14)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO12, OnRadio12)
	ON_BN_CLICKED(IDC_RADIO6, OnRadio6)
	ON_BN_CLICKED(IDC_RADIO11, OnRadio11)
	ON_BN_CLICKED(IDC_RADIO10, OnRadio10)
	ON_BN_CLICKED(IDC_RADIO9, OnRadio9)
	ON_BN_CLICKED(IDC_RADIO7, OnRadio7)
	ON_BN_CLICKED(IDC_RADIO8, OnRadio8)
	ON_BN_CLICKED(IDC_RADIO18, OnRadio18)
	ON_BN_CLICKED(IDC_RADIO15, OnRadio15)
	ON_BN_CLICKED(IDC_RADIO16, OnRadio16)
	ON_BN_CLICKED(IDC_RADIO17, OnRadio17)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_RADIO19, OnRadio19)
	ON_BN_CLICKED(IDC_MRUL1, OnMrul1)
	ON_BN_CLICKED(IDC_MRUB1, OnMrub1)
	ON_BN_CLICKED(IDC_MRUB2, OnMrub2)
	ON_BN_CLICKED(IDC_MRUB3, OnMrub3)
	ON_BN_CLICKED(IDC_MRUB4, OnMrub4)
	ON_BN_CLICKED(IDC_MRUL2, OnMrul2)
	ON_BN_CLICKED(IDC_MRUL3, OnMrul3)
	ON_BN_CLICKED(IDC_MRUL4, OnMrul4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShapeProp message handlers

void CShapeProp::OnRadio13() 

{
	CButton *chk = 	((CButton*)GetDlgItem(IDC_RADIO13));
	int curr = chk->GetCheck();

	if(!curr)
		{
		Apply(APPLY_LTYPE, PS_SOLID);
		chk->SetCheck(true);
		((CButton*)GetDlgItem(IDC_RADIO15))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RADIO16))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RADIO17))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RADIO19))->SetCheck(false);
		}
}

void CShapeProp::OnRadio14() 

{
	CButton *chk = 	((CButton*)GetDlgItem(IDC_RADIO14));
	int curr = chk->GetCheck();

	if(!curr)
		{
		Apply(APPLY_LTYPE, PS_NULL);

		chk->SetCheck(true);
		((CButton*)GetDlgItem(IDC_RADIO18))->SetCheck(false);
		}
	
}

void CShapeProp::OnRadio1() 
{
	CButton *chk = 	((CButton*)GetDlgItem(IDC_RADIO1));
	int curr = chk->GetCheck();

	if(!curr)
		{
		Apply(APPLY_BRTYPE, BS_SOLID);

		chk->SetCheck(true);
		((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RADIO12))->SetCheck(false);
		}
}

void CShapeProp::OnRadio2() 

{
	CButton *chk = 	((CButton*)GetDlgItem(IDC_RADIO2));
	int curr = chk->GetCheck();

	if(!curr)
		{
		Apply(APPLY_BRTYPE, BS_HATCHED);

		chk->SetCheck(true);
		((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RADIO12))->SetCheck(false);
		}
}

void CShapeProp::OnRadio12() 
{
	CButton *chk = 	((CButton*)GetDlgItem(IDC_RADIO12));
	int curr = chk->GetCheck();

	if(!curr)
		{
		Apply(APPLY_BRTYPE, BS_NULL);

		chk->SetCheck(true);
		((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(false);
		}	
}

void CShapeProp::OnRadio6() 

{
	CButton *chk = 	((CButton*)GetDlgItem(IDC_RADIO6));
	int curr = chk->GetCheck();

	if(!curr)
		{
		Apply(APPLY_BRHATCH, HS_BDIAGONAL);

		chk->SetCheck(true);
		((CButton*)GetDlgItem(IDC_RADIO11))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RADIO10))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RADIO9))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RADIO7))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RADIO8))->SetCheck(false);
		}	
}

void CShapeProp::OnRadio11() 
{
	CButton *chk = 	((CButton*)GetDlgItem(IDC_RADIO11));
	int curr = chk->GetCheck();

	if(!curr)
		{
		Apply(APPLY_BRHATCH, HS_FDIAGONAL);

		chk->SetCheck(true);
		((CButton*)GetDlgItem(IDC_RADIO6))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RADIO10))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RADIO9))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RADIO7))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RADIO8))->SetCheck(false);
		}	
}

void CShapeProp::OnRadio10() 
{
	CButton *chk = 	((CButton*)GetDlgItem(IDC_RADIO10));
	int curr = chk->GetCheck();

	if(!curr)
		{
		Apply(APPLY_BRHATCH, HS_HORIZONTAL);
		
		chk->SetCheck(true);
		((CButton*)GetDlgItem(IDC_RADIO11))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RADIO6))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RADIO9))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RADIO7))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RADIO8))->SetCheck(false);
		}		
}

void CShapeProp::OnRadio9() 
{
	CButton *chk = 	((CButton*)GetDlgItem(IDC_RADIO9));
	int curr = chk->GetCheck();

	if(!curr)
		{
		Apply(APPLY_BRHATCH, HS_VERTICAL);

		chk->SetCheck(true);
		((CButton*)GetDlgItem(IDC_RADIO11))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RADIO10))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RADIO6))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RADIO7))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RADIO8))->SetCheck(false);
		}		
}

void CShapeProp::OnRadio7() 
{
	CButton *chk = 	((CButton*)GetDlgItem(IDC_RADIO7));
	int curr = chk->GetCheck();

	if(!curr)
		{
		Apply(APPLY_BRHATCH, HS_CROSS);

		chk->SetCheck(true);
		((CButton*)GetDlgItem(IDC_RADIO11))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RADIO10))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RADIO9))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RADIO6))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RADIO8))->SetCheck(false);
		}		
}

void CShapeProp::OnRadio8() 

{
	CButton *chk = 	((CButton*)GetDlgItem(IDC_RADIO8));
	int curr = chk->GetCheck();

	if(!curr)
		{
		Apply(APPLY_BRHATCH, HS_DIAGCROSS);

		chk->SetCheck(true);
		((CButton*)GetDlgItem(IDC_RADIO11))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RADIO10))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RADIO9))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RADIO7))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RADIO6))->SetCheck(false);
		}		
}

BOOL CShapeProp::OnInitDialog() 

{
	CGrayDlg::OnInitDialog();
	
	m_l1.SetBkColor(RGB(255,255,255));
	
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER10))->SetRange(0, 255);

	((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(true);
	((CButton*)GetDlgItem(IDC_RADIO18))->SetCheck(true);
	((CButton*)GetDlgItem(IDC_RADIO13))->SetCheck(true);

	LoadColors();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CShapeProp::OnRadio18() 
{
	CButton *chk = 	((CButton*)GetDlgItem(IDC_RADIO18));
	int curr = chk->GetCheck();

	if(!curr)
		{
		Apply(APPLY_LTYPE, PS_SOLID);

		// Check solid as fallback
		((CButton*)GetDlgItem(IDC_RADIO13))->SetCheck(true);
		
		((CButton*)GetDlgItem(IDC_RADIO15))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RADIO16))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RADIO17))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RADIO19))->SetCheck(false);

		chk->SetCheck(true);
		((CButton*)GetDlgItem(IDC_RADIO14))->SetCheck(false);
		}
}

void CShapeProp::OnRadio15() 
{
	CButton *chk = 	((CButton*)GetDlgItem(IDC_RADIO15));
	int curr = chk->GetCheck();

	if(!curr)
		{
		Apply(APPLY_LWIDTH, 1);
		Apply(APPLY_LTYPE, PS_DASH);
	
		chk->SetCheck(true);
		((CButton*)GetDlgItem(IDC_RADIO13))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RADIO16))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RADIO17))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RADIO19))->SetCheck(false);
		
		}
}

void CShapeProp::OnRadio16() 
{
	CButton *chk = 	((CButton*)GetDlgItem(IDC_RADIO16));
	int curr = chk->GetCheck();

	if(!curr)
		{
		Apply(APPLY_LWIDTH, 1);
		Apply(APPLY_LTYPE, PS_DOT);
		
		chk->SetCheck(true);
		((CButton*)GetDlgItem(IDC_RADIO15))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RADIO13))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RADIO17))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RADIO19))->SetCheck(false);
		
		}
}

void CShapeProp::OnRadio17() 
{
	CButton *chk = 	((CButton*)GetDlgItem(IDC_RADIO17));
	int curr = chk->GetCheck();

	if(!curr)
		{
		Apply(APPLY_LWIDTH, 1);
		Apply(APPLY_LTYPE, PS_DASHDOT );
		
		chk->SetCheck(true);
		((CButton*)GetDlgItem(IDC_RADIO13))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RADIO16))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RADIO15))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RADIO19))->SetCheck(false);
		
		}	
}

//////////////////////////////////////////////////////////////////////////
// Set all text that has focus
	
void CShapeProp::Apply(int offset, int val1, int val2)

{
	CShadesDlg *dlg; GMD(dlg); CXrayDoc *doc; GCDR(doc, dlg);
		
	int lsize = doc->m_layers.GetSize();
	for(int looplay = 0; looplay < lsize; looplay++)
		{
		CLayer *xlay = (CLayer*)doc->m_layers[looplay];
		ASSERT(xlay->magic == LAYER_MAGIC);

		if(xlay->focus || xlay == lay)
			{
			switch(offset)
				{
				case APPLY_TRANS:
					xlay->gdata.alpha = val1;
					break;

				case APPLY_FG:
					xlay->gdata.fg_color = val1;
					break;
			
				case APPLY_BG:
					xlay->gdata.bg_color = val1;
					break;
				}

			if(xlay->gdata.kind == LAYER_ELLIPSE || xlay->gdata.kind == LAYER_RECT )
				{	
				switch(offset)
					{
					case APPLY_LWIDTH:
						((CLayCirc*)xlay)->gdata.line_width = val1;
					break;
					
					case APPLY_LTYPE:
						((CLayCirc*)xlay)->gdata.line_type = val1;
						break;
					
					case APPLY_BRTYPE:
						((CLayCirc*)xlay)->gdata.brush_type = val1;
						break;
					
					case APPLY_BRHATCH:
						((CLayCirc*)xlay)->gdata.brush_type = BS_HATCHED;
						((CLayCirc*)xlay)->gdata.brush_hatch = val1;
						OnRadio2();					
						break;
					}
				}
			xlay->Invalidate();
			}
		}
}


void CShapeProp::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	
	if(nSBCode == SB_ENDSCROLL || nSBCode == SB_THUMBTRACK) //	|| nSBCode == SB_THUMBPOSITION)
		{
		if(pScrollBar == GetDlgItem(IDC_SLIDER10))
			{
			int pos = ((CSliderCtrl*)GetDlgItem(IDC_SLIDER10))->GetPos();

			m_ed5.Format("%d", pos);
			UpdateData(false);
			
			Apply(APPLY_TRANS, pos);
			}

		if(pScrollBar == GetDlgItem(IDC_SLIDER9))
			{
			int pos = ((CSliderCtrl*)GetDlgItem(IDC_SLIDER9))->GetPos();

			m_ed3.Format("%d", pos);
			UpdateData(false);
			
			Apply(APPLY_LWIDTH, pos);
			}

		}	

	CGrayDlg::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CShapeProp::OnButton2() 
{
	CPicSel ps;	

	ps.lay = lay;
	ps.liveptr = &lay->gdata.fg_color;

	ps.DoModal();

	CLabel *arr[4] = {&m_l1, &m_l2, &m_l3, &m_l4};

	for(int loop = sizeof(arr)/sizeof(CLabel*) - 1; loop >= 1; loop--)
		{
		arr[loop]->SetBkColor(arr[loop-1]->GetBkColor());
		arr[loop]->Invalidate();
		}
	arr[0]->SetBkColor(lay->gdata.fg_color);
	arr[0]->Invalidate();
}

void CShapeProp::OnButton3() 

{
	CPicSel ps;	

	ps.lay = lay;
	ps.liveptr = &lay->gdata.bg_color;

	ps.DoModal();		

	CLabel *arr[4] = {&m_b1, &m_b2, &m_b3, &m_b4};

	for(int loop = sizeof(arr)/sizeof(CLabel*) - 1; loop >= 1; loop--)
		{
		arr[loop]->SetBkColor(arr[loop-1]->GetBkColor());
		arr[loop]->Invalidate();
		}
	arr[0]->SetBkColor(lay->gdata.bg_color);
	arr[0]->Invalidate();
}

void CShapeProp::OnRadio19() 

{
	CButton *chk = 	((CButton*)GetDlgItem(IDC_RADIO19));
	int curr = chk->GetCheck();

	if(!curr)
		{
		Apply(APPLY_LWIDTH, 1);
		Apply(APPLY_LTYPE, PS_DASHDOTDOT );
		
		chk->SetCheck(true);
		((CButton*)GetDlgItem(IDC_RADIO13))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RADIO16))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RADIO17))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RADIO15))->SetCheck(false);	
		}	
}

void CShapeProp::OnMrul1() 
{
	Apply(APPLY_FG, m_l1.GetBkColor());
}

void	CShapeProp::OnOK()

{
	SaveColors();
	CGrayDlg::OnOK();
}

void	CShapeProp::OnCancel()
{
	SaveColors();
	CGrayDlg::OnCancel();
}

void CShapeProp::OnMrub1() 
{
	Apply(APPLY_BG, m_b1.GetBkColor());
}

void CShapeProp::OnMrub2() 
{
	Apply(APPLY_BG, m_b2.GetBkColor());	
}

void CShapeProp::OnMrub3() 
{
	Apply(APPLY_BG, m_b3.GetBkColor());	
}

void CShapeProp::OnMrub4() 
{
	Apply(APPLY_BG, m_b4.GetBkColor());	
}

void CShapeProp::SaveColors()

{
	int loop;
	CLabel *arr[4] = {&m_b1, &m_b2, &m_b3, &m_b4};
	CLabel *arr2[4] = {&m_l1, &m_l2, &m_l3, &m_l4};

	for(loop = 0; loop < sizeof(arr)/sizeof(CLabel*);  loop++)
		{
		COLORREF cr = arr[loop]->GetBkColor();

		CString tmp; tmp.Format("RecentBgCol%d", loop);
		AfxGetApp()->WriteProfileInt(strConfig, tmp, cr);
		}

	for(loop = 0; loop < sizeof(arr)/sizeof(CLabel*);  loop++)
		{
		COLORREF cr = arr2[loop]->GetBkColor();

		CString tmp; tmp.Format("RecentFgCol%d", loop);
		AfxGetApp()->WriteProfileInt(strConfig, tmp, cr);
		}
}

void CShapeProp::LoadColors()

{
	int loop;
	COLORREF defcol = RGB(128,128,128);

	CLabel *arr[4] = {&m_b1, &m_b2, &m_b3, &m_b4};
	CLabel *arr2[4] = {&m_l1, &m_l2, &m_l3, &m_l4};

	for(loop = 0; loop < sizeof(arr)/sizeof(CLabel*);  loop++)
		{
		COLORREF cr;
		CString tmp; tmp.Format("RecentBgCol%d", loop);
		cr = AfxGetApp()->GetProfileInt(strConfig, tmp, defcol);

		arr[loop]->SetBkColor(cr);
		}

	for(loop = 0; loop < sizeof(arr)/sizeof(CLabel*);  loop++)
		{
		COLORREF cr;
		CString tmp; tmp.Format("RecentFgCol%d", loop);
		cr = AfxGetApp()->GetProfileInt(strConfig, tmp, defcol);

		arr2[loop]->SetBkColor(cr);
		}
}

void CShapeProp::OnMrul2() 
{
	Apply(APPLY_FG, m_l2.GetBkColor());	
}

void CShapeProp::OnMrul3() 
{
	Apply(APPLY_FG, m_l3.GetBkColor());	
}

void CShapeProp::OnMrul4() 
{
	Apply(APPLY_FG, m_l4.GetBkColor());	
}

void CShapeProp::OnButton5() 

{
	CPicSel ps;	

	ps.lay = lay;
	ps.liveptr = &lay->gdata.hatch_color;

	ps.DoModal();		
	
}
