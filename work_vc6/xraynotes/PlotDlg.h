#if !defined(AFX_PLOTDLG_H__6E02F02D_A5AF_4030_8BD4_5AA357793441__INCLUDED_)
#define AFX_PLOTDLG_H__6E02F02D_A5AF_4030_8BD4_5AA357793441__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlotDlg.h : header file
//

#include "plot.h"

/////////////////////////////////////////////////////////////////////////////
// CPlotDlg dialog

class CPlotDlg : public CDialog

{
// Construction
public:

	int created;
	void Show();
	CPlotDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPlotDlg)
	enum { IDD = IDD_DIALOG14 };
	CPlot	m_plot;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlotDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPlotDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLOTDLG_H__6E02F02D_A5AF_4030_8BD4_5AA357793441__INCLUDED_)
