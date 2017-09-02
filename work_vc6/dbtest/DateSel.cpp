// DateSel.cpp : implementation file
//

#include "stdafx.h"
#include "dbtest.h"
#include "DateSel.h"
#include "mxpad.h"
#include "parse.h"
#include "support.h"
#include "msgbox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDateSel dialog

CDateSel::CDateSel(CWnd* pParent /*=NULL*/)
	: CDialog(CDateSel::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDateSel)
	m_str = _T("");
	//}}AFX_DATA_INIT

	// Init it to today
	COleDateTime today = COleDateTime::GetCurrentTime();
	today.SetDateTime(today.GetYear(), today.GetMonth(), today.GetDay(), 
						today.GetHour(), today.GetMinute(), today.GetSecond());

	result = today.Format("%Y/%m/%d");
	time_disabled = false;
	date_disabled = false;

	reenter = false;
}


void CDateSel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDateSel)
	DDX_Control(pDX, IDC_DS_BUTTON31, m_b31);
	DDX_Control(pDX, IDC_DS_BUTTON30, m_b29);
	DDX_Control(pDX, IDC_DS_BUTTON28, m_b28);
	DDX_Control(pDX, IDC_DS_BUTTON27, m_b27);
	DDX_Control(pDX, IDC_DS_BUTTON26, m_b26);
	DDX_Control(pDX, IDC_DS_BUTTON25, m_b25);
	DDX_Control(pDX, IDC_DS_BUTTON24, m_b24);
	DDX_Control(pDX, IDC_DS_EDIT2, m_tme);
	DDX_Control(pDX, IDC_TTT, m_ttt);
	DDX_Control(pDX, IDC_TIME, m_time);
	DDX_Control(pDX, IDC_CAL, m_cal);
	DDX_Control(pDX, IDC_DS_EDIT1, m_edit);
	DDX_Control(pDX, IDC_DS_BUTTON23, m_n);
	DDX_Control(pDX, IDC_DS_BUTTON2, m_p);
	DDX_Control(pDX, IDCANCEL, m_can2);
	DDX_Control(pDX, IDDS_CANCEL2, m_can);
	DDX_Control(pDX, IDOK, m_ok);
	//DDX_Text(pDX, IDC_EDIT2, m_str);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDateSel, CDialog)
	//{{AFX_MSG_MAP(CDateSel)
	ON_BN_CLICKED(IDDS_CANCEL2, OnCancel2)
	ON_BN_CLICKED(IDC_DS_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_DS_BUTTON23, OnButton23)
	ON_BN_CLICKED(IDC_DS_BUTTON24, OnButton24)
	ON_BN_CLICKED(IDC_DS_BUTTON25, OnButton25)
	ON_BN_CLICKED(IDC_DS_BUTTON26, OnButton26)
	ON_BN_CLICKED(IDC_DS_BUTTON30, OnButton30)
	ON_BN_CLICKED(IDC_DS_BUTTON27, OnButton27)
	ON_BN_CLICKED(IDC_DS_BUTTON28, OnButton28)
	ON_BN_CLICKED(IDC_DS_BUTTON31, OnButton31)
	ON_EN_CHANGE(IDC_DS_EDIT2, OnChangeEdit2)
	ON_EN_CHANGE(IDC_DS_EDIT1, OnChangeDsEdit1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDateSel message handlers

void CDateSel::OnCancel2() 
{
	CString approot; support.GetAppRoot(approot);
	CString str, file("dateformat.html");

#ifdef _DEBUG
	str = "Z:/archive/xraynotes/umanual/" + file;
#else
	str =  approot + "umanual\\" + file;
#endif

	ShellExecute(NULL, "open", str, NULL, NULL, SW_SHOWMAXIMIZED);	
}

void CDateSel::OnButton2() 

{
	//P2N("Prev Year\r\n");
	
	COleDateTimeSpan ts(365, 0, 0, 0);
	COleDateTime refDateTime;
	m_cal.GetDate( refDateTime);
	refDateTime -= ts;
	m_cal.SetDate(refDateTime);
}

void CDateSel::OnButton23() 

{
	//P2N("Next year\r\n");	
	COleDateTimeSpan ts(365, 0, 0, 0);
	COleDateTime refDateTime;

	m_cal.GetDate( refDateTime);
	refDateTime += ts;
	m_cal.SetDate(refDateTime);

}

void CDateSel::OnButton24() 

{
	COleDateTimeSpan ts(10 * 365, 0, 0, 0);
	COleDateTime refDateTime;

	m_cal.GetDate( refDateTime);
	refDateTime -= ts;
	m_cal.SetDate(refDateTime);
}

void CDateSel::OnButton25() 

{
	COleDateTimeSpan ts(10 * 365, 0, 0, 0);
	COleDateTime refDateTime;

	m_cal.GetDate( refDateTime);
	refDateTime += ts;
	m_cal.SetDate(refDateTime);//m_date.GetCurSel(refDateTime);
}

void CDateSel::OnButton26() 

{
	m_cal.SetToday();
	m_time.SetToday();
}

BOOL CDateSel::PreTranslateMessage(MSG* pMsg) 

{
	if(pMsg->message == EN_CHANGE)
		{
		//P2N("CDateSel::PreTranslateMessage EN_CHANGE\r\n");
		//	//return 0;
		}

	if(pMsg->message == WM_CALCHANGE)
		{
		COleDateTime refDateTime;
		m_cal.GetDate(refDateTime);

		//CString datestr = refDateTime.Format("%Y/%m/%d");

		CString datestr = ((COleDateTime*)pMsg->lParam)->Format("%Y/%m/%d");

		//P2N("CDateSel::PreTranslateMessage Changed calendar control to %s\r\n", datestr);

		CString datestr2 = refDateTime.Format("%Y/%m/%d");
		m_edit.SetWindowText(datestr2);
		}
	
	if(pMsg->message == WM_CALCDBLCLICK)
		{
		OnOK();
		}

	if(pMsg->message == WM_TCHANGE)
		{
		//P2N("CDateSel::PreTranslateMessage time change on %d\r\n", pMsg->wParam);

		int hh = m_time.hour1.num * 10 + m_time.hour2.num;
		int mm = m_time.minute1.num * 10 + m_time.minute2.num;
	
		// Update Edit box
		CString str; str.Format("%02d:%02d", hh, mm);
		m_tme.SetWindowText(str);

		// Do the 12 hour format:
		CString str3("AM"); if(hh > 11) str3 = "PM";
		if(hh > 12) hh -= 12;

		CString str2; str2.Format("%02d:%02d %s", hh, mm, str3);
		m_ttt.SetWindowText(str2);
	
		//UpdateData(false);
		}

	return CDialog::PreTranslateMessage(pMsg);
}

#if 0
void CDateSel::OnSelectMonthcalendar1(NMHDR* pNMHDR, LRESULT* pResult) 

{
	// TODO: Add your control notification handler code here

	P2N("Select Month Caledar\r\n");
	*pResult = 0;
}
#endif


void CDateSel::OnButton30() 

{
	COleDateTimeSpan ts(30, 0, 0, 0);
	COleDateTime refDateTime;

	m_cal.GetDate( refDateTime);
	refDateTime -= ts;
	m_cal.SetDate(refDateTime);	
}

void CDateSel::OnButton27() 
{
	P2N("Button 27\r\n");

	COleDateTimeSpan ts(30, 0, 0, 0);
	COleDateTime refDateTime;

	m_cal.GetDate( refDateTime);
 	refDateTime += ts;
	m_cal.SetDate(refDateTime);		
}

void CDateSel::OnButton28() 
{
	COleDateTimeSpan ts(4 * 30, 0, 0, 0);
	COleDateTime refDateTime;

	m_cal.GetDate( refDateTime);
	refDateTime -= ts;
	m_cal.SetDate(refDateTime);		
}

void CDateSel::OnButton31() 
{
	COleDateTimeSpan ts(4 * 30, 0, 0, 0);
	COleDateTime refDateTime;

	m_cal.GetDate( refDateTime);
	refDateTime += ts;
	m_cal.SetDate(refDateTime);		
}

void CDateSel::OnOK() 

{
	P2N("CDateSel::OnOK()\r\n");	
	
	if(m_cal.selday.GetStatus() == COleDateTime::invalid)
		{
		MBOX("Invalid date entered, please correct it.");
		return;
		}

	CString result1, result2;

	result = "";

	m_edit.GetWindowText(result1);
	m_tme.GetWindowText(result2);
	
	if(!date_disabled)
		result += result1;
	
	if(!time_disabled)
		{
		// Add space for clarity
		if(!date_disabled)
			result += " ";
		result += result2;
		}
	
	CDialog::OnOK();
}

BOOL CDateSel::OnInitDialog() 

{
	CDialog::OnInitDialog();
	
	CString arr[6];
	ParseDateStr(result, arr, 6);

	COleDateTime dt(atoi(arr[0]), atoi(arr[1]), atoi(arr[2]), atoi(arr[3]), atoi(arr[4]), atoi(arr[5]) );

	if(dt.GetStatus() == COleDateTime::invalid)
		{
		dt = COleDateTime::GetCurrentTime();
		}

	if(time_disabled)
		{
		m_time.Disable();
		m_tme.Disable();

		m_time.bg = RGB(110, 110, 110);
		m_time.fg = RGB(128, 128, 128);
		}

	if(date_disabled)
		{
		m_cal.Disable();
		m_edit.Disable();
		}

	m_cal.SetDate(dt);
	m_time.SetTime(dt);
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//////////////////////////////////////////////////////////////////////////

void CDateSel::OnChangeEdit2() 

{
	//UpdateData();

	CString str;
	GetDlgItem(IDC_DS_EDIT2)->GetWindowText(str);

	if(str.GetLength() < 3)
		return;

	if(str[2] != ':')
		return;

	reenter = true;

	int hh = atoi(str);
	int mm = atoi(str.Mid(3));
	
	//P2N("CDateSel::OnChangeEdit2 %d %d\r\n", str);

	// Disable time_change message  generation
	m_time.hour1.nomessage = true;
	m_time.hour2.nomessage = true;
	m_time.minute1.nomessage = true;
	m_time.minute2.nomessage = true;

	m_time.hour1.num = hh / 10;
	m_time.hour2.num = hh % 10;

	m_time.minute1.num = mm / 10;
	m_time.minute2.num = mm % 10;

	m_time.Invalidate();

	// Do the 12 hour format:
		CString str3("AM"); if(hh > 11) str3 = "PM";
		if(hh > 12) hh -= 12;

		CString str2; str2.Format("%02d:%02d %s", hh, mm, str3);
		m_ttt.SetWindowText(str2);


	//YieldToWinEx();

	//m_time.hour1.nomessage = false;
	//m_time.hour2.nomessage = false;
	//m_time.minute1.nomessage = false;
	//m_time.minute2.nomessage = false;
}

//DEL void CDateSel::OnChangeEdit3() 
//DEL 
//DEL {
//DEL 	P2N("CDateSel::OnChangeEdit3\r\n");
//DEL 
//DEL 	// send this notification unless you override the CDialog::OnInitDialog()
//DEL 	// function and call CRichEditCtrl().SetEventMask()
//DEL 	// with the ENM_CHANGE flag ORed into the mask.
//DEL 	
//DEL 	// TODO: Add your control notification handler code here
//DEL }
			
void CDateSel::OnChangeDsEdit1() 

{
	CString str;
	((CEdit *)GetDlgItem(IDC_DS_EDIT1))->GetWindowText(str);

	CString arr[3];
	ParsePathStr(str, arr, 3);

	if(arr[0] == "") arr[0] = "1";
	if(arr[1] == "") arr[1] = "1";
	if(arr[2] == "") arr[2] = "1";

	//P2N("CDateSel::OnChangeDsEdit1 %s  -- %s/%s/%s\r\n", str, arr[0], arr[1], arr[2]); 	

	COleDateTime dt(atoi(arr[0]), atoi(arr[1]), atoi(arr[2]), 0,0,0);

	// No need to error check, the calendar will show it
	//(dt.GetStatus() == COleDateTime::invalid)
	//	{
	//	//dt = COleDateTime::GetCurrentTime();
	//	}

	m_cal.SetDate(dt, true);  // send it with nopost
}

//DEL void CDateSel::OnKillfocusDsEdit1() 
//DEL {
//DEL 	P2N("CDateSel::OnKillfocusDsEdit1\r\n");
//DEL }

//DEL void CDateSel::OnSetfocusDsEdit1() 
//DEL {
//DEL 	P2N(" CDateSel::OnSetfocusDsEdit1\r\n");
//DEL }
