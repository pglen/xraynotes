// Window.cpp : implementation file
//

#include "stdafx.h"
#include "xraynotes.h"
#include "Window.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Window

IMPLEMENT_DYNCREATE(Window, CCmdTarget)

Window::Window()

{
	P2N("Window::Window\r\n"); 

	HINSTANCE res  = AfxGetInstanceHandle( );
	//FindResource

	HRSRC rc = FindResource(res, "DIALOG1", RT_DIALOG);
	HGLOBAL hg = LoadResource(res, rc);

	win = CreateWindow("EDIT", "Win", WS_POPUP, 100, 100, 100, 100, 
		NULL, NULL, AfxGetInstanceHandle(), NULL);

	ASSERT(win);

#if 0
	LPCTSTR lpClassName,  // pointer to registered class name
	LPCTSTR lpWindowName, // pointer to window name
	DWORD dwStyle,        // window style
	int x,                // horizontal position of window
	int y,                // vertical position of window
	int nWidth,           // window width
	int nHeight,          // window height
	HWND hWndParent,      // handle to parent or owner window
	HMENU hMenu,          // handle to menu or child-window identifier
	HANDLE hInstance,     // handle to application instance
	LPVOID lpParam        // pointer to window-creation data
#endif

}

Window::~Window()

{
	P2N("Window::~Window\r\n"); 
}


BEGIN_MESSAGE_MAP(Window, CCmdTarget)
	//{{AFX_MSG_MAP(Window)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Window message handlers
