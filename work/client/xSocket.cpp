
/* =====[ MySocket.cpp ]========================================== 
                                                                             
   Description:     The client project, implementation of the MySocket.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  3/30/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// MySocket.cpp : implementation file
//

#include "stdafx.h"
#include "client.h"
#include "xSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CxSocket

CxSocket::CxSocket()

{
	busy = sbusy = cbusy = xbusy = false;
	status = INITIAL;
	lasterr = 0;
	timeout = 3000;

    //hGlobalWriteEvent = CreateEvent( 
	harr[0] = CreateEvent( 

        NULL,			// no security attributes
        TRUE,			// manual-reset event
        FALSE,			// initial state is non signaled
        NULL			// object name
        ); 

	ASSERT(harr[0]);
    //ASSERT(hGlobalWriteEvent);

	//hGlobalSendEvent = CreateEvent( 
	harr[1] = CreateEvent( 

        NULL,			// no security attributes
        TRUE,			// manual-reset event
        FALSE,			// initial state is non signaled
        NULL			// object name
        ); 

	ASSERT(harr[1]);
    //ASSERT(hGlobalSendEvent);
}

CxSocket::~CxSocket()

{
	//CloseHandle (hGlobalWriteEvent);

	CloseHandle (harr[0]);
	CloseHandle (harr[1]);
}

// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CxSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CxSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CxSocket member functions

void CxSocket::OnConnect(int nErrorCode) 

{
	//AP2N("CxSocket::OnConnect err=%d\r\n", nErrorCode);

	lasterr = nErrorCode;
	cbusy = false;
	CAsyncSocket::OnConnect(nErrorCode);
}

void CxSocket::OnClose(int nErrorCode) 

{
	//AP2N("CxSocket::OnClose err=%d\r\n", nErrorCode);

	lasterr = nErrorCode;
	xbusy = false;
	CAsyncSocket::OnClose(nErrorCode);
}

void CxSocket::OnSend(int nErrorCode) 

{
	//AP2N("CxSocket::OnSend err=%d\r\n", nErrorCode);
	
	// Only get the async stuff if we are actually waiting
	if(sbusy && (nErrorCode == 0))
		{
		lasterr = CAsyncSocket::Send(sbuf, slen, sflags);
		sbusy = false;

		//SetEvent(hGlobalWriteEvent);	

		SetEvent(harr[1]);	
		}	
	CAsyncSocket::OnSend(nErrorCode);
}

//////////////////////////////////////////////////////////////////////////

void CxSocket::OnReceive(int nErrorCode) 

{
	//AP2N("CxSocket::OnReceive err=%d\r\n", nErrorCode);
	
	// Only get the async stuff if we are actually waiting
	if(busy && (nErrorCode == 0))
		{
		lasterr = CAsyncSocket::Receive(rbuf, rlen, rflags);
		busy = false;

		//SetEvent(hGlobalWriteEvent);
		SetEvent(harr[0]);
		//PulseEvent(hGlobalWriteEvent);
		}

	CAsyncSocket::OnReceive(nErrorCode);
}

//////////////////////////////////////////////////////////////////////////
// Wait for the messages to go thru

int CxSocket::PumpMsgs(int ret, volatile int *flag)

{
	//P2N("Op completed with %d %d %s\r\n", ret, GetLastError(), sockerr2str(GetLastError()) );

	// We need to make this arrangement before another system call,
	// otherwise the async fires while we are not listening for async
	// Async wait is requested
	busy = true;

	// Normal operation, just return
	if(ret > 0)
		{
		*flag = false;
		return ret;
		}

	if(GetLastError() != WSAEWOULDBLOCK)
		{
		// Reset and ret err if not waiting for async op
		*flag = false;
		return ret;
		}

	//P2N("CxSocket::PumpMsgs\r\n");

	int start = clock();
	while(TRUE)
		{ 
		MSG msg;

		// Do our own version of message loop
		while(true)
			{
			if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
				{
				TranslateMessage(&msg); DispatchMessage(&msg);
				
				// App exit
				if(msg.message == WM_QUIT)
					break;

				// Window down
				if(msg.message == WM_DESTROY)
					break;		
				}
			else
				{
				break;
				}
			}

		// Do this so the process can breathe
		Sleep(0);

		// This is to save processor power while the transfer is going
		DWORD rr = MsgWaitForMultipleObjects(
			  2,						// number of handles in the handle array
			  //&hGlobalWriteEvent,		// pointer to the object-handle array
			  harr,						// pointer to the object-handle array
			  FALSE,					// wait for all or wait for one
			  timeout,		  			// time-out interval in milliseconds
			  QS_ALLINPUT				// type of input events to wait for
			);

		//P2N("released event ret=%d\r\n", rr);
		//ResetEvent(hGlobalWriteEvent);
		ResetEvent(harr[0]); ResetEvent(harr[1]);

		if(!*flag)
			{
			// got data, stop wait
			ret = lasterr;
			break;
			}

		if((clock() - start) > timeout)
			{
			P2N("Timeout in pump message\r\n");
			break;
			}

		//Sleep(0);
		}
	return ret;
}

int CxSocket::Send(CString &str)

{
	return Send(str, str.GetLength());
}

int CxSocket::Send(const void* lpBuf, int nBufLen, int nFlags) 

{
	//P2N("CxSocket::Sending '%s'\r\n", lpBuf);

		// Remeber params for async operation
	sbuf	= lpBuf;
	slen	= nBufLen;
	sflags	= nFlags;

	int ret = CAsyncSocket::Send(lpBuf, nBufLen, nFlags);
	sbusy = true;
	ret = PumpMsgs(ret, &sbusy);

	//P2N("CxSocket::Sent %d bytes\r\n", ret);
	
	//YieldToWinEx();

	return ret;
}

int CxSocket::Receive(void* lpBuf, int nBufLen, int nFlags) 

{
	//P2N("CxSocket::Receive: want %d\r\n", nBufLen);

	// Remeber params for async operation
	rbuf	= lpBuf;
	rlen	= nBufLen;
	rflags	= nFlags;

	int ret = CAsyncSocket::Receive(lpBuf, nBufLen, nFlags);
	busy = true;
	ret = PumpMsgs(ret, &busy);
	
	//if((ret > 0) && (ret < nBufLen))
	//	((char *)lpBuf)[ret] = 0;

	//P2N("CxSocket::Received %d bytes\r\n", ret);
	//P2N("'%s'\r\n", lpBuf);
	
	return ret;		
}

BOOL CxSocket::Connect(LPCTSTR lpszHostAddress, UINT nHostPort)

{
	//P2N("CxSocket::Connect to: %s port=%d\r\n", lpszHostAddress, nHostPort);

	BOOL ret = CAsyncSocket::Connect(lpszHostAddress, nHostPort);
	ret = PumpMsgs(ret, &cbusy);

	if(ret != 0)
		status = CONNECTED;

	return ret;
}

BOOL CxSocket::Create(UINT nSocketPort, int nSocketType, long lEvent, LPCTSTR lpszSocketAddress)

{
	//P2N("CxSocket::Create(UINT nSocketPort=%d\r\n", nSocketPort);

	BOOL ret = CAsyncSocket::Create(nSocketPort, nSocketType, lEvent, lpszSocketAddress);

	return ret;
}

void CxSocket::Close()

{
	status = INITIAL;
	busy = false;	
	CAsyncSocket::Close();
}

int CxSocket::DrainRecv(void *buf, int maxlen)

{
	u_long amount = 0;

	while(true)
		{
		int ret = ioctlsocket(m_hSocket,  FIONREAD,  &amount);
		
		P2N("Socket data Amount pending: %d\r\n", amount);

		if(amount)
			{
			char *mem = (char *)malloc(amount);
			int ret = CAsyncSocket::Receive(mem, amount);
			free(mem);
			}
		else
			{	
			break;
			}
		}
	return 0;
}
