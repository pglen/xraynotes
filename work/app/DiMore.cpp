// DiMore.cpp : implementation file
//

#include "stdafx.h"
#include "xraynotes.h"
#include "DiMore.h"

#include "DicomWin.h"

extern	CDicomWin	dicomwin;


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiMore dialog


CDiMore::CDiMore(CWnd* pParent /*=NULL*/)
	: CGrayDlg(CDiMore::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDiMore)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_title = "XrayNotes Dicom Details";
}


void CDiMore::DoDataExchange(CDataExchange* pDX)
{
	CGrayDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDiMore)
	DDX_Control(pDX, IDC_LIST1, m_list);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDiMore, CGrayDlg)
	//{{AFX_MSG_MAP(CDiMore)
	ON_BN_CLICKED(IDC_DID_BUTTON3, OnDidButton3)
	ON_BN_CLICKED(IDC_DID_BUTTON4, OnDidButton4)
	ON_BN_CLICKED(IDC_DID_BUTTON1, OnDidButton1)
	ON_BN_CLICKED(IDC_DID_BUTTON2, OnDidButton2)
	ON_BN_CLICKED(IDC_DID_BUTTON7, OnDidButton7)
	ON_BN_CLICKED(IDC_DID_BUTTON8, OnDidButton8)
	ON_BN_CLICKED(IDC_DID_BUTTON9, OnDidButton9)
	ON_BN_CLICKED(IDC_DID_BUTTON10, OnDidButton10)
	ON_LBN_SELCHANGE(IDC_LIST1, OnSelchangeList1)
	ON_BN_CLICKED(IDC_DID_BUTTON11, OnDidButton11)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDiMore message handlers

BOOL CDiMore::OnInitDialog() 

{
	CGrayDlg::OnInitDialog();
	
	CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);
	CXrayDoc *doc = ((CShadesDlg* )AfxGetApp()->m_pMainWnd)->GetCurrDoc();
	if(!doc) return TRUE;
	if(!doc->IsDicomFile()) return TRUE;

	// P2N("CDiMore::OnInitDialog \r\n");
	
	int size = doc->dcarr.GetSize();
	for(int loop = 0; loop < size; loop++)
		{
		int ret = m_list.AddString(((DicomFile*)doc->dcarr[loop])->filename);

		m_list.SetCheck(ret, ((DicomFile*)doc->dcarr[loop])->enabled);
		}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDiMore::OnOK()

{
	// Propagate to DICOM array

	CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);
	CXrayDoc *doc = ((CShadesDlg* )AfxGetApp()->m_pMainWnd)->GetCurrDoc();
	if(!doc) goto endd;
	if(!doc->IsDicomFile()) goto endd;

	{
	// Write out the flags

	CString str, str2;
	int size = m_list.GetCount();
	
	for(int loop = 0; loop < size; loop++)
		{
		int cch = m_list.GetCheck(loop);

		m_list.GetText(loop, str);
		
		//P2N("%d %s\r\n", cch, str);
		
		// Set it in the array
		int size2 = doc->dcarr.GetSize();
		for(int loop2 = 0; loop2 < size2; loop2++)
			{
			DicomFile* dc = (DicomFile*)doc->dcarr[loop2];

			if(dc->filename == str)
				{
				dc->enabled = cch;
				break;
				}
			}
		}

	} // scope for goto

endd:

	CGrayDlg::OnOK();
}	



void CDiMore::OnDidButton3() 
{
	int size = m_list.GetCount();
	
	for(int loop = 0; loop < size; loop++)
		{
		m_list.SetCheck(loop, true);
		}
}

void CDiMore::OnDidButton4() 
{
	int size = m_list.GetCount();
	
	for(int loop = 0; loop < size; loop++)
		{
		m_list.SetCheck(loop, false);
		}
}

void CDiMore::OnDidButton1() 
{
	int sel = m_list.GetCurSel();

	if(sel < 0)
		{
		xMsgBox("At least one item must be selected");
		return;
		}
	if(sel == 0)
		{
		xMsgBox("Cannot move item further, already at the beginning.");
		return;
		}
	
	// Swap the two entries
	CString str, str2;	int en, en2;

	// Save to tmp place CCheckListBox
	m_list.GetText(sel, str);		en = m_list.GetCheck(sel);
	m_list.GetText(sel-1, str2);	en2 = m_list.GetCheck(sel-1);

	// Swap
	m_list.DeleteString(sel);	m_list.DeleteString(sel-1);

	int last = m_list.InsertString(sel-1, str2);	m_list.SetCheck(last, en2);
	last = m_list.InsertString(sel-1, str);	m_list.SetCheck(last, en);

	 m_list.SetCurSel(sel-1);
}

//////////////////////////////////////////////////////////////////////////

void CDiMore::OnDidButton2() 

{
	int last, itemcnt = m_list.GetCount(), sel   = m_list.GetCurSel();

	if(sel < 0)
		{
		xMsgBox("At least one item must be selected"); return;
		}
	if(sel == itemcnt - 1)
		{
		xMsgBox("Cannot move item further, already at the end.");
		return;
		}
	
	// Swap the two entries
	CString str, str2;	int en, en2;

	// Save to tmp place CCheckListBox
	m_list.GetText(sel, str);		en = m_list.GetCheck(sel);
	m_list.GetText(sel+1, str2);	en2 = m_list.GetCheck(sel+1);

	// Swap
	m_list.DeleteString(sel);	m_list.DeleteString(sel);

	last = m_list.InsertString(sel, str);	m_list.SetCheck(last, en);
	last = m_list.InsertString(sel, str2);	m_list.SetCheck(last, en2);
	
	m_list.SetCurSel(sel+1);	
}

void CDiMore::OnDidButton7() 

{
	ToggleQarter(0, m_list.GetCount()/4);	
}


void CDiMore::ToggleQarter(int beg, int end)

{
	if(end-beg < 1)
		return;

	int flag = false, val = 0; 
	for(int loop = beg; loop < end; loop++)
		{
		if(m_list.GetCheck(loop))
			val++;
		}

	if(val > (end-beg)/2)
		flag = true;
		
	P2N("val=%d dist=%d flag=%d\r\n", val, end-beg, flag);

	for(int loop = beg; loop < end; loop++)
		{
		m_list.SetCheck(loop, !flag);
		}

}

void CDiMore::OnDidButton8() 
{
	ToggleQarter(m_list.GetCount()/4, m_list.GetCount()/2);		
}

void CDiMore::OnDidButton9() 
{
	ToggleQarter(m_list.GetCount()/2, 3*m_list.GetCount()/4);			
}

void CDiMore::OnDidButton10() 
{
	ToggleQarter(3*m_list.GetCount()/4, m_list.GetCount());			
}

void CDiMore::OnSelchangeList1() 

{
	int sel = m_list.GetCurSel();

	if(sel >= 0)
		dicomwin.SetCurrFrame(sel);	
}

void CDiMore::OnDidButton11() 

{
	int sel = m_list.GetCurSel();

	if(sel >= 0)
		{
		
		CString str;
		m_list.GetText(sel, str);	

		P2N("unload %s\r\n", str);
		}
}
