
/* =====[ lineProp.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the lineProp.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  1/22/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// lineProp.cpp : implementation file
//

#include "stdafx.h"
#include "xraynotes.h"
#include "lineProp.h"

#include "PicSel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ClineProp dialog


ClineProp::ClineProp(CWnd* pParent /*=NULL*/)
	: CGrayDlg(ClineProp::IDD, pParent)
{
	//{{AFX_DATA_INIT(ClineProp)
	m_ed3 = _T("");
	m_ed5 = _T("");
	m_ed6 = _T("");
	m_ed7 = _T("");
	m_ed11 = _T("");
	//}}AFX_DATA_INIT
}


void ClineProp::DoDataExchange(CDataExchange* pDX)
{
	CGrayDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ClineProp)
	DDX_Control(pDX, IDC_BUTTON5, m_b5);
	DDX_Control(pDX, IDC_BUTTON4, m_b4);
	DDX_Control(pDX, IDCANCEL, m_can);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Text(pDX, IDC_EDIT3, m_ed3);
	DDX_Text(pDX, IDC_EDIT5, m_ed5);
	DDX_Text(pDX, IDC_EDIT6, m_ed6);
	DDX_Text(pDX, IDC_EDIT7, m_ed7);
	DDX_Text(pDX, IDC_EDIT11, m_ed11);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(ClineProp, CGrayDlg)
	//{{AFX_MSG_MAP(ClineProp)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_CHECK4, OnCheck4)
	ON_BN_CLICKED(IDC_CHECK5, OnCheck5)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ClineProp message handlers

//////////////////////////////////////////////////////////////////////////
// Set all text that has focus
	
void ClineProp::Apply(int offset, int val1, int val2)

{
	CShadesDlg *dlg; GMD(dlg); CXrayDoc *doc; GCDR(doc, dlg);
		
	int lsize = doc->m_layers.GetSize();
	for(int looplay = 0; looplay < lsize; looplay++)
		{
		CLayer *xlay = (CLayer*)doc->m_layers[looplay];
		ASSERT(xlay->magic == LAYER_MAGIC);

		if( ((xlay->gdata.kind == LAYER_SLINE || xlay->gdata.kind == LAYER_LINES) 
				&& xlay->focus) ||
				xlay == lay
				)
			{
			switch(offset)
				{
				case APPLY_TRANS:
					((CLayer*)xlay)->gdata.alpha = val1;
					break;

				case APPLY_LWIDTH:
					((CLaySline*)xlay)->sldata.line_thickness = val1;
					break;

				case APPLY_BG:
					((CLayer*)xlay)->gdata.bg_color = val1;
					break;

				case APPLY_FG:
					((CLayer*)xlay)->gdata.fg_color = val1;
					break;

				case APPLY_START:
					((CLaySline*)xlay)->sldata.arrow_start = val1;
					break;

				case APPLY_END:
					((CLaySline*)xlay)->sldata.arrow_end = val1;
					break;	

				case APPLY_LROUND:
					((CLaySline*)xlay)->sldata.cap_square = val1;
					break;	

				case APPLY_ARRLEN:
					((CLaySline*)xlay)->sldata.arrow_len = val1;
					break;	
			
				case APPLY_ARRANG:
					((CLaySline*)xlay)->sldata.arrow_ang = val1;
					break;	
					
				case APPLY_ARRLEN2:
					((CLaySline*)xlay)->sldata.arrow_elen = val1;
					break;	
			
				case APPLY_ARRANG2:
					((CLaySline*)xlay)->sldata.arrow_eang = val1;
					break;	
				


				}
			xlay->Invalidate();
			}
		}
}

void ClineProp::OnButton4() 
{
	CPicSel ps;	

	ps.lay = lay;
	ps.liveptr = &lay->gdata.fg_color;

	ps.DoModal();		
}

void ClineProp::OnButton5() 
{
	CPicSel ps;	

	ps.lay = lay;
	ps.liveptr = &lay->gdata.bg_color;
	ps.uppd = PIC_APPLY_BG;

	ps.DoModal();

	Apply(APPLY_BG, lay->gdata.bg_color);	
}

void ClineProp::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 

{
	//P2N("ClineProp::OnHScroll  %p %d\r\n", pScrollBar, ((CSliderCtrl*)pScrollBar)->GetPos());

	if(pScrollBar == GetDlgItem(IDC_SLIDER9))
		{
		// Line thick
		int pos = ((CSliderCtrl*)GetDlgItem(IDC_SLIDER9))->GetPos();
		m_ed3.Format("%d", pos); UpdateData(false);
		Apply(APPLY_LWIDTH, pos);	
		}
	if(pScrollBar == GetDlgItem(IDC_SLIDER2))
		{
		// Arrow Length
		int pos = ((CSliderCtrl*)GetDlgItem(IDC_SLIDER2))->GetPos();
		m_ed5.Format("%d", pos); UpdateData(false);
		Apply(APPLY_ARRLEN, pos);	
		}
	if(pScrollBar == GetDlgItem(IDC_SLIDER3))
		{
		// Arrow Angle
		int pos = ((CSliderCtrl*)GetDlgItem(IDC_SLIDER3))->GetPos();
		m_ed6.Format("%d", pos); UpdateData(false);
		Apply(APPLY_ARRANG, pos);	
		}

	if(pScrollBar == GetDlgItem(IDC_SLIDER7))
		{
		// Arrow Angle
		int pos = ((CSliderCtrl*)GetDlgItem(IDC_SLIDER7))->GetPos();
		m_ed7.Format("%d", pos); UpdateData(false);
		Apply(APPLY_ARRLEN2, pos);	
		}

	if(pScrollBar == GetDlgItem(IDC_SLIDER8))
		{
		// Arrow Angle
		int pos = ((CSliderCtrl*)GetDlgItem(IDC_SLIDER8))->GetPos();
		m_ed11.Format("%d", pos); UpdateData(false);
		Apply(APPLY_ARRANG2, pos);	
		}

	CGrayDlg::OnHScroll(nSBCode, nPos, pScrollBar);
}


BOOL ClineProp::OnInitDialog() 

{
	CGrayDlg::OnInitDialog();
	
	m_title = "Line Properites Dialog";
	
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER9))->SetPos(((CLaySline*)lay)->sldata.line_thickness);
	m_ed3.Format("%d", ((CLaySline*)lay)->sldata.line_thickness);

	((CSliderCtrl*)GetDlgItem(IDC_SLIDER2))->SetPos(((CLaySline*)lay)->sldata.arrow_len);
	m_ed5.Format("%d", ((CLaySline*)lay)->sldata.arrow_len);
	
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER3))->SetPos((int)((CLaySline*)lay)->sldata.arrow_ang);
	m_ed6.Format("%d", (int)((CLaySline*)lay)->sldata.arrow_ang);
	
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER7))->SetPos(((CLaySline*)lay)->sldata.arrow_elen);
	m_ed7.Format("%d", ((CLaySline*)lay)->sldata.arrow_elen);
	
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER8))->SetPos((int)((CLaySline*)lay)->sldata.arrow_eang);
	m_ed11.Format("%d", ((CLaySline*)lay)->sldata.arrow_eang);
	
	if(((CLaySline*)lay)->sldata.arrow_start)
		{
		((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(true);
		}
	if(((CLaySline*)lay)->sldata.arrow_end)
		{
		((CButton*)GetDlgItem(IDC_CHECK4))->SetCheck(true);
		}
	if(((CLaySline*)lay)->sldata.cap_square)
		{
		((CButton*)GetDlgItem(IDC_CHECK5))->SetCheck(true);
		}

	((CLaySline*)lay)->old_sldata = ((CLaySline*)lay)->sldata;

	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void ClineProp::OnCheck1() 
{
	Apply(APPLY_START, ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck());
}

void ClineProp::OnCheck4() 
{
	Apply(APPLY_END, ((CButton*)GetDlgItem(IDC_CHECK4))->GetCheck());
}

void ClineProp::OnCheck5() 
{
	Apply(APPLY_LROUND, ((CButton*)GetDlgItem(IDC_CHECK5))->GetCheck());
}

void ClineProp::OnCancel()

{
	//P2N("ClineProp::OnCancel\r\n");

	int arr_len = MAX(lay->sldata.arrow_len, lay->sldata.arrow_elen);
	int endsize = MAX(lay->_handlesize, lay->sldata.line_thickness + arr_len);

	CRect oldrc; lay->GetRect(&oldrc);
	oldrc.InflateRect(endsize, endsize);

	((CLaySline*)lay)->sldata = ((CLaySline*)lay)->old_sldata;
	CRect newrc; lay->GetRect(&newrc);

	lay->updaterect = oldrc | newrc;

	((CLayer *)lay)->Invalidate();

	CGrayDlg::OnCancel();
}
