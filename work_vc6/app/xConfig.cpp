
/* =====[ xConfig.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the xConfig.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  11/19/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// xConfig.cpp : implementation file
//

#include "stdafx.h"
#include "xraynotes.h"
#include "xConfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CxConfig dialog

#if 0

IM_NEAREST_NEIGHBOUR 	
IM_BILINEAR 	
IM_BSPLINE 	
IM_BICUBIC 	
IM_BICUBIC2 	
IM_LANCZOS 	
IM_BOX 	
IM_HERMITE 	
IM_HAMMING 	
IM_SINC 	
IM_BLACKMAN 	
IM_BESSEL 	
IM_GAUSSIAN 	
IM_QUADRATIC 	
IM_MITCHELL 	
IM_CATROM 

#endif


CxConfig::CxConfig(CWnd* pParent /*=NULL*/)
	: CGrayDlg(CxConfig::IDD, pParent)
{
	//{{AFX_DATA_INIT(CxConfig)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	windowname = "CxConfig";
}


void CxConfig::DoDataExchange(CDataExchange* pDX)
{
	CGrayDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CxConfig)
	DDX_Control(pDX, IDCANCEL, m_can);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDC_BUTTON3, m_b3);
	DDX_Control(pDX, IDC_BUTTON2, m_b2);
	DDX_Control(pDX, IDC_BUTTON1, m_b1);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CxConfig, CGrayDlg)
	//{{AFX_MSG_MAP(CxConfig)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO2, OnSelchangeCombo2)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CxConfig message handlers

BOOL CxConfig::OnInitDialog() 
{
	CGrayDlg::OnInitDialog();

	m_title = "XrayNotes Configuration Dialog";	

	SelCombo(gl_resamp_rough, GetDlgItem(IDC_COMBO1));
	SelCombo(gl_resamp_fine, GetDlgItem(IDC_COMBO2));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//////////////////////////////////////////////////////////////////////////

int CxConfig::SelCombo(int resamp, CWnd *combo) 

{
	const char *str = "none";

	switch(resamp)
		{
		case CxImage::IM_NEAREST_NEIGHBOUR 	:  str = "Nearest Neighbour"; break;
		case CxImage::IM_BILINEAR  			:  str = "Bilinear"         ; break;
		case CxImage::IM_BSPLINE   			:  str = "Bspline"          ; break;
		case CxImage::IM_BICUBIC   			:  str = "Bicubic"          ; break;
		case CxImage::IM_BICUBIC2  			:  str = "Bicubic2"         ; break;
		case CxImage::IM_LANCZOS   			:  str = "Lanczos"          ; break;
		case CxImage::IM_BOX       			:  str = "Box"              ; break;
		case CxImage::IM_HERMITE   			:  str = "Hermite"          ; break;
		case CxImage::IM_HAMMING   			:  str = "Hamming"          ; break;
		case CxImage::IM_SINC      			:  str = "Sinc"             ; break;
		case CxImage::IM_BLACKMAN  			:  str = "Blackman"         ; break;
		case CxImage::IM_BESSEL    			:  str = "Bessel"           ; break;
		case CxImage::IM_GAUSSIAN  			:  str = "Gaussian"         ; break;
		case CxImage::IM_QUADRATIC 			:  str = "Quadratic"        ; break;
		case CxImage::IM_MITCHELL  			:  str = "Mitchell"         ; break;
		case CxImage::IM_CATROM    			:  str = "Catrom"           ; break;
		}

	P2N("Found str %s\r\n", str);

	((CComboBox*)combo)->SelectString(0, str);

	return 0;
}


int CxConfig::unSelCombo(CString &str, CWnd *combo) 

{
	int ret = 0;

		if(str.Find("Nearest Neighbour") >= 0)	ret = CxImage::IM_NEAREST_NEIGHBOUR;
	else if(str.Find("Bilinear" ) >= 0)		ret = CxImage::IM_BILINEAR ; 			
	else if(str.Find("Bspline"  ) >= 0)		ret = CxImage::IM_BSPLINE  ; 			
	else if(str.Find("Bicubic"  ) >= 0)		ret = CxImage::IM_BICUBIC  ; 			
	else if(str.Find("Bicubic2" ) >= 0)		ret = CxImage::IM_BICUBIC2 ; 			
	else if(str.Find("Lanczos"  ) >= 0)		ret = CxImage::IM_LANCZOS  ; 			
	else if(str.Find("Box"      ) >= 0)		ret = CxImage::IM_BOX      ; 			
	else if(str.Find("Hermite"  ) >= 0)		ret = CxImage::IM_HERMITE  ; 			
	else if(str.Find("Hamming"  ) >= 0)		ret = CxImage::IM_HAMMING  ; 			
	else if(str.Find("Sinc"     ) >= 0)		ret = CxImage::IM_SINC     ; 			
	else if(str.Find("Blackman" ) >= 0)		ret = CxImage::IM_BLACKMAN ; 			
	else if(str.Find("Bessel"   ) >= 0)		ret = CxImage::IM_BESSEL   ; 			
	else if(str.Find("Gaussian" ) >= 0)		ret = CxImage::IM_GAUSSIAN ; 			
	else if(str.Find("Quadratic") >= 0)		ret = CxImage::IM_QUADRATIC; 			
	else if(str.Find("Mitchell" ) >= 0)		ret = CxImage::IM_MITCHELL ; 			
	else if(str.Find("Catrom"   ) >= 0)		ret = CxImage::IM_CATROM   ; 			
	else 									ret = CxImage::IM_NEAREST_NEIGHBOUR   ; 			

	//P2N("CxConfig::unSelCombo Found  %d\r\n", ret);

	return ret;
}

void CxConfig::OnSelchangeCombo1() 

{
	CString str;
	CWnd *combo = GetDlgItem(IDC_COMBO1);

	((CComboBox*)combo)->GetLBText(((CComboBox*)combo)->GetCurSel(), str);
	gl_resamp_rough	= unSelCombo(str, combo);
}

void CxConfig::OnSelchangeCombo2() 

{
	CString str;
	CWnd *combo = GetDlgItem(IDC_COMBO2);

	((CComboBox*)combo)->GetLBText(((CComboBox*)combo)->GetCurSel(), str);
	gl_resamp_fine	= unSelCombo(str, combo);
}

void CxConfig::OnButton1() 
{
	((CShadesApp* )AfxGetApp())->Help("resample.html");
}
