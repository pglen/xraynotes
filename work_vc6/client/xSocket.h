#if !defined(AFX_MYSOCKET_H__65A9C092_1AF7_4522_AF71_C55D135E2C68__INCLUDED_)
#define AFX_MYSOCKET_H__65A9C092_1AF7_4522_AF71_C55D135E2C68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MySocket.h : header file
//

#define		INITIAL		0
#define		CONNECTED	1

/////////////////////////////////////////////////////////////////////////////
// CxSocket command target

class CxSocket : public CAsyncSocket
{
// Attributes
public:

	//HANDLE hGlobalWriteEvent;
	//HANDLE hGlobalSendEvent;

	HANDLE	harr[2];

	const void	*sbuf;

	void	*rbuf;
	int		slen, sflags, rlen, rflags;

	volatile int		busy, sbusy, cbusy, xbusy;
		
	int		status, lasterr;

	int		timeout;   // MILLISEC

	BOOL Create(UINT nSocketPort = 0, int nSocketType=SOCK_STREAM,
		long lEvent = FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE,
			LPCTSTR lpszSocketAddress = NULL);

	int Send(CString &str);

// Operations
public:
	CxSocket();
	virtual ~CxSocket();

// Overrides
public:
	int DrainRecv(void *buf, int maxlen);
	void Close();
	BOOL Connect(LPCTSTR lpszHostAddress, UINT nHostPort);

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CxSocket)
	public:
	virtual void OnConnect(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual int Send(const void* lpBuf, int nBufLen, int nFlags = 0);
	virtual int Receive(void* lpBuf, int nBufLen, int nFlags = 0);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CxSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:

	int	PumpMsgs(int ret, volatile int *flag);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSOCKET_H__65A9C092_1AF7_4522_AF71_C55D135E2C68__INCLUDED_)
