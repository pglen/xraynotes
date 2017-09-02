// clientDlg.h : header file
//

#if !defined(AFX_CLIENTDLG_H__86AFB8C8_2FD2_45BB_99C4_60392BDC5F54__INCLUDED_)
#define AFX_CLIENTDLG_H__86AFB8C8_2FD2_45BB_99C4_60392BDC5F54__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CClientDlg dialog

#include "label.h"
#include "xSocket.h"
#include "LogWin.h"
#include "xCircular.h"
#include "xTargetButton.h"

// Do not change this, must be in sync with the server for encryption
#define BUFFSIZE	4096

class CClientDlg : public CDialog

{

// Construction

public:
	int Decrypt(char *buf, int len);

	char *buffpass;

	CString echal, ekey;

	int		lprog;
	char	cryptbuf[BUFFSIZE];

	void AllButtons(int enable);
	//int			wassock;

	int			GetRemLine(char *line, int maxlen, int *serr);
	int			GetRemChar(char *chr, int *succ);
	void		GetOverFlow(char *chr, int *len);

	CxCircular	circ;

	CxSocket	m_sock1;

	char	buffer[_MAX_PATH + 1];
	int		buffprog, rprog, fsm;

	int		in_recv, in_send;
	int		abort;

	char	buf[BUFFSIZE + 1];

	int		CClientDlg::PingPong(CString &str);

	//CAsyncSocket m_sock1;

	CClientDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CClientDlg)
	enum { IDD = IDD_CLIENT_DIALOG };
	CxTargetButton	m_b17;
	CxTargetButton	m_b16;
	CxTargetButton	m_b15;
	CxTargetButton	m_b14;
	CLogWin	m_log;
	CxTargetButton	m_con;
	CxTargetButton	m_Exec			;
	CxTargetButton	m_Exit          ;
	CxTargetButton	m_Cancel        ;
	CxTargetButton	m_ClearLog     ;
	CxTargetButton	m_Disconnect    ;
	CxTargetButton	m_Login         ;
	CxTargetButton	m_LS            ;
	CxTargetButton	m_Slst          ;
	CxTargetButton	m_Recv          ;
	CxTargetButton	m_Abort         ;
	CxTargetButton	m_Help          ;
	CxTargetButton	m_Stat          ;
	CxTargetButton	m_PWD           ;
	CxTargetButton	m_eKey          ;
	CLabel	m_status;
	CString	m_host;
	CString	m_com;
	CString	m_port;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CClientDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	afx_msg void OnButton6();
	afx_msg void OnButton7();
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg void OnEndSession(BOOL bEnding);
	afx_msg BOOL OnQueryEndSession();
	virtual void OnOK();
	afx_msg void OnButton8();
	afx_msg void OnButton9();
	afx_msg void OnButton10();
	afx_msg void OnButton11();
	afx_msg void OnButton12();
	afx_msg void OnButton13();
	afx_msg void OnButton15();
	afx_msg void OnButton16();
	afx_msg void OnButton17();
	afx_msg void OnButton18();
	afx_msg void OnButton14();
	afx_msg void OnButton19();
	afx_msg void OnButton20();
	afx_msg void OnButton21();
	afx_msg void OnButton22();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTDLG_H__86AFB8C8_2FD2_45BB_99C4_60392BDC5F54__INCLUDED_)
