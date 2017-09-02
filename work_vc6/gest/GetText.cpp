// GetText.cpp : implementation file
//

#include "stdafx.h"
#include "gest.h"
#include "GetText.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGetText dialog


CGetText::CGetText(CWnd* pParent /*=NULL*/)
	: CDialog(CGetText::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGetText)
	m_edit = _T("");
	//}}AFX_DATA_INIT
}

void CGetText::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGetText)
	DDX_Text(pDX, IDC_EDIT1, m_edit);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGetText, CDialog)
	//{{AFX_MSG_MAP(CGetText)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGetText message handlers
