// Transcript.cpp : implementation file
//

#include "stdafx.h"
#include "xraynotes.h"
#include "Transcript.h"

#include "support.h"
#include "mxpad.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTranscript dialog


CTranscript::CTranscript(CWnd* pParent /*=NULL*/)
	: CDialog(CTranscript::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTranscript)
	//}}AFX_DATA_INIT

	moved = false;

	head = ref = NULL;
	P2N("CTranscript::CTranscript\r\n");
}

void CTranscript::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTranscript)
	DDX_Control(pDX, IDC_TR_EDIT1, m_edit1);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTranscript, CDialog)
	//{{AFX_MSG_MAP(CTranscript)
	ON_BN_CLICKED(IDC_TR_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_TR_BUTTON30, OnButton30)
	ON_BN_CLICKED(IDC_TR_BUTTON32, OnButton32)
	ON_BN_CLICKED(IDC_TR_CHECK1, OnCheck1)
	ON_WM_SIZE()
	ON_WM_LBUTTONUP()
	ON_WM_NCLBUTTONUP()
	ON_WM_NCCALCSIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void	AutoWrap(CString &str, int width)

{
	// Autowrap to size
	int idx = 0;
	int len = str.GetLength();

	while(true)
		{
		idx += width;

		if(idx >= len)
			break;

		// Wobble no more than quarter length either way
		for(int loop = 0; loop < width / 4; loop++)
			{
			if(idx - loop <= 0)
				break;

			if(isspace(str.GetAt(idx - loop)) )
				{
				idx -= loop - 1;
				break;
				}

			if(idx + loop < len )
				{
				if(isspace(str.GetAt(idx - loop)) )
					{
					idx += loop - 1;
					break;
					}
				}
			}
		str.Insert(idx, "\r\n");
		}
}

/////////////////////////////////////////////////////////////////////////////
// CTranscript message handlers

BOOL CTranscript::OnInitDialog() 

{
	CDialog::OnInitDialog();
	
	//P2N("CTranscript::OnInitDialog()\r\n");

	CPGedit  *ed = (CPGedit*)GetDlgItem(IDC_TR_EDIT1);

	if(ref)
		{
		ed->anoautoselect = true;
		ed->SetWindowText(*ref);
		}

	if(head)
		SetWindowText(*head);

	((CButton*)GetDlgItem(IDC_TR_CHECK1))->SetCheck(ed->acheckon);

	RECT rc; GetClientRect(&rc);

	// Geometry for buttons
	WINDOWPLACEMENT wp;

	GetDlgItem(IDOK)->GetWindowPlacement(&wp);
	ok_x = rc.right - wp.rcNormalPosition.left;	
	ok_y = rc.bottom - wp.rcNormalPosition.top;	

	GetDlgItem(IDCANCEL)->GetWindowPlacement(&wp);
	can_x = rc.right - wp.rcNormalPosition.left;	
	can_y = rc.bottom - wp.rcNormalPosition.top;	

	GetDlgItem(IDC_TR_BUTTON30)->GetWindowPlacement(&wp);
	b1_x = rc.right - wp.rcNormalPosition.left;	
	b1_y = rc.bottom - wp.rcNormalPosition.top;	

	GetDlgItem(IDC_TR_BUTTON2)->GetWindowPlacement(&wp);
	b2_x = rc.right - wp.rcNormalPosition.left;	
	b2_y = rc.bottom - wp.rcNormalPosition.top;	

	GetDlgItem(IDC_TR_BUTTON32)->GetWindowPlacement(&wp);
	b32_x = rc.right - wp.rcNormalPosition.left;	
	b32_y = rc.bottom - wp.rcNormalPosition.top;	

	GetDlgItem(IDC_TR_CHECK1)->GetWindowPlacement(&wp);
	c1_x = rc.right - wp.rcNormalPosition.left;	
	c1_y = rc.bottom - wp.rcNormalPosition.top;	

	GetDlgItem(IDC_TR_STATIC)->GetWindowPlacement(&wp);
	s1_x = rc.right - wp.rcNormalPosition.left;	
	s1_y = rc.bottom - wp.rcNormalPosition.top;	

	GetDlgItem(IDC_TR_EDIT2)->GetWindowPlacement(&wp);
	e2_x = rc.right - wp.rcNormalPosition.left;	
	e2_y = rc.bottom - wp.rcNormalPosition.top;	

	GetDlgItem(IDC_TR_EDIT1)->GetWindowPlacement(&wp);
	ed1_left	= wp.rcNormalPosition.left;	
	ed1_top		= wp.rcNormalPosition.top;	
	ed1_gapx    = rc.right  - wp.rcNormalPosition.right;	
	ed1_gapy    = rc.bottom - wp.rcNormalPosition.bottom;	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTranscript::OnButton2() 

{		
	CString str, line;;

	CStringArray arr;

	((CEdit*)GetDlgItem(IDC_TR_EDIT1))->GetWindowText(str);

	//P2N("Dumping str\r\n");	D2N(str, 128);

	int prog = 0, cnt = 0;
	int len, idx1, idx2, lastout;

	lastout = len = str.GetLength(),

	// Leading line
	idx2 = idx1 = str.Find("\r");

	if(idx1 >= 0)
		{
		line = str.Mid(0, idx1-1); 
		line.Remove('\n'); line.Remove('\r');
		//P2N("%d %d iline='%s'\r\n", idx1, idx2, line);
		arr.Add(line);
		}
	else
		{
		//P2N("Only one line %s\r\n", str);
		}

	while (true)
		{
		
		idx1 = str.Find("\r", idx2);

		if(idx1 >= 0)
			{
			idx2 = str.Find("\r", idx1 + 1);
			
			if(idx2 >= 0)
				{
				line = str.Mid(idx1, (idx2 - idx1)); 
				line.Remove('\n'); line.Remove('\r');
				//P2N("%d %d mline='%s'\r\n", idx1, idx2, line);

				arr.Add(line);
				lastout = idx2;
				}
			else
				{
				break;
				}
			}
		else
			{
			break;
			}

		// For testing
		//if(cnt++ > 300)
		//	break;
		}

	if(lastout < len)
		{
		line = str.Mid(lastout, len - lastout);
		line.Remove('\n'); line.Remove('\r');
		arr.Add(line);
		//P2N("%d %d tline='%s'\r\n", idx1, idx2, line);
		}
		
	// Dump array
	CString str2;
	int arrs = arr.GetSize();

	for(int loop = 0; loop < arrs; loop++)
		{
		AutoWrap(arr[loop], 120);
		str2 += arr[loop];
		str2 += "\r\n";
		}	 

	((CEdit*)GetDlgItem(IDC_TR_EDIT1))->SetWindowText(str2);
}

//////////////////////////////////////////////////////////////////////////

void CTranscript::OnOK()
{
	if(ref)
		{
		((CEdit*)GetDlgItem(IDC_TR_EDIT1))->GetWindowText(*ref);
		P2N("%s OK\r\n", *ref);
		}
	else
		{
		P2N("OK\r\n");
		}

	CDialog::OnOK();
}

//void CTranscript::OnButton23() 
//{
//		
//}

void CTranscript::OnButton30() 
{	
	P2N("Setsel\r\n");

	//if(m_edit1.disabled)
	//	m_edit1.Enable();
	//else
	//	m_edit1.Disable();
		
	CPGedit  *ed = (CPGedit*)GetDlgItem(IDC_TR_EDIT1);

	//ed->SetSel(0, ed->GetWindowTextLength());

	ed->SetFocus();
	ed->SetSel(0, -1);
}

//////////////////////////////////////////////////////////////////////////

void CTranscript::OnButton32() 

{
	CString str, fff;
	int ss, ee;

	CPGedit  *ed = (CPGedit*)GetDlgItem(IDC_TR_EDIT1);
	CPGedit  *ff = (CPGedit*)GetDlgItem(IDC_TR_EDIT2);

	ff->GetWindowText(fff);

	//P2N("Searching for fff=%s\r\n", fff);

	ed->GetSel(ss, ee);
	ed->GetWindowText(str);

	int idx = str.Find(fff, ss + 1);
	if(idx >= 0)
		{
		ed->SetFocus();
		// Highlite
		ed->SetSel(idx, idx + fff.GetLength());
		}
	else
		{
		MBOX("No more matches found.");
		ed->SetFocus();
		}
}

void CTranscript::OnCheck1() 

{
	int cc;

	CPGedit  *ed = (CPGedit*)GetDlgItem(IDC_TR_EDIT1);
	cc = ((CButton*)GetDlgItem(IDC_TR_CHECK1))->GetCheck();

	ed->acheckon = cc;

	ed->SetFocus();
	ed->Invalidate();
}

void CTranscript::OnCancel() 
{
	CPGedit  *ed = (CPGedit*)GetDlgItem(IDC_TR_EDIT1);

	if(ed->GetModify())
		{
		if(MBOX("Are you sure you want to abandon changes to contents?", "", MB_YESNO) != IDYES )
			return;
		}

	CDialog::OnCancel();
}

//////////////////////////////////////////////////////////////////////////

void CTranscript::OnSize(UINT nType, int cx, int cy) 

{

	CDialog::OnSize(nType, cx, cy);
	
	//P2N("CTranscript::OnSize %d %d flag=%d\r\n", cx, cy, IsWindow(m_hWnd));

	CPGedit  *ed = (CPGedit*)GetDlgItem(IDC_TR_EDIT1);
		{
		if(ed)
			if(IsWindow(ed->m_hWnd))
				Resize();
		}		
}

void	CTranscript::Resize()

{
	RECT rc; GetClientRect(&rc);

	//WINDOWPLACEMENT wp;	GetWindowPlacement(&wp);

	// Right aligned
	GetDlgItem(IDOK)->SetWindowPos(NULL, rc.right - ok_x,  rc.bottom - ok_y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	GetDlgItem(IDCANCEL)->SetWindowPos(NULL, rc.right - can_x,  rc.bottom - can_y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	GetDlgItem(IDC_TR_BUTTON30)->SetWindowPos(NULL, rc.right - b1_x,  rc.bottom - b1_y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	GetDlgItem(IDC_TR_BUTTON2)->SetWindowPos(NULL, rc.right - b2_x,  rc.bottom - b2_y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	GetDlgItem(IDC_TR_BUTTON32)->SetWindowPos(NULL, rc.right - b32_x,  rc.bottom - b32_y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	GetDlgItem(IDC_TR_CHECK1)->SetWindowPos(NULL, rc.right - c1_x,  rc.bottom - c1_y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	GetDlgItem(IDC_TR_STATIC)->SetWindowPos(NULL, rc.right - s1_x,  rc.bottom - s1_y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	GetDlgItem(IDC_TR_EDIT2)->SetWindowPos(NULL, rc.right - e2_x,  rc.bottom - e2_y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	
	GetDlgItem(IDC_TR_EDIT1)->SetWindowPos(NULL, ed1_left, ed1_top, 
								rc.right - ed1_gapx - ed1_left,  rc.bottom - ed1_gapy - ed1_top, SWP_NOZORDER);
	
	SetTimer(1, 300, NULL);
}

//////////////////////////////////////////////////////////////////////////

void CTranscript::OnLButtonUp(UINT nFlags, CPoint point) 

{
	//P2N("CTranscript::OnLButtonUp\r\n");
	CDialog::OnLButtonUp(nFlags, point);
}

void CTranscript::OnNcLButtonUp(UINT nHitTest, CPoint point) 
{
	//P2N("CTranscript::OnNcLButtonUp\r\n");
	CDialog::OnNcLButtonUp(nHitTest, point);
}

void CTranscript::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	//P2N("CTranscript::OnNcCalcSize\r\n");
	CDialog::OnNcCalcSize(bCalcValidRects, lpncsp);
}

void CTranscript::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	//P2N("CTranscript::OnGetMinMaxInfo\r\n");
	
	lpMMI->ptMinTrackSize.x = 700;
	lpMMI->ptMinTrackSize.y = 150;

	CDialog::OnGetMinMaxInfo(lpMMI);
}

void CTranscript::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent == 1)
		{
		KillTimer(nIDEvent);
		P2N("CTranscript::OnTimer -- Invalidate timer\r\n");
		Invalidate();	
		}
	CDialog::OnTimer(nIDEvent);
}
