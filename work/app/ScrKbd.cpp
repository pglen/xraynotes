
/* =====[ CScrKbd.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the OnscreenKeyboardDlg.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  2/26/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#include "stdafx.h"
#include "xraynotes.h"

#include <math.h>

#include "scrkbd.h"
#include "mxpad.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TIMER_ID 101

/////////////////////////////////////////////////////////////////////////////
// CScrKbd dialog

CScrKbd::CScrKbd(CWnd* pParent /*=NULL*/)
	: CGrayDlg(CScrKbd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CScrKbd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	windowname = "On Screen KBD";
	oldwin = NULL;
	sendthis = NULL;
	topgap = 20;

	shift = control = alt = 0;

	if(!smallfont.CreateFont(-7, 0, 0, 0, 0, 0, 0, 0,
				ANSI_CHARSET, 0, 0, 0, FIXED_PITCH, "Ms Serif"))	
		{
		MBOX("Cannot set small font for keyboard, defaulting to system font.");
		}

	if(!bigfont.CreateFont(-20, 0, 0, 0, 0, 0, 0, 0,
				ANSI_CHARSET, 0, 0, 0, FIXED_PITCH, "Ms Sans Serif"))	
		{
		MBOX("Cannot set large font, defaulting to system font.");
		}
}

void CScrKbd::DoDataExchange(CDataExchange* pDX)
{
	CGrayDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScrKbd)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CScrKbd, CGrayDlg)
	//{{AFX_MSG_MAP(CScrKbd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONUP()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_GETMINMAXINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScrKbd message handlers


BOOL CScrKbd::OnInitDialog()

{
	CGrayDlg::OnInitDialog();

	BuildKeyDef(); 
	CalcWidthHeight();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CScrKbd::CalcWidthHeight()

{
	KeyWidth = 0;
	KeyHeight = 0;

	KEYDEF * key;
	POSITION pos = cKeys.GetHeadPosition();
	
	long totalWidth = 0, longest = 0, rows = 0;

	while(pos)
	{
		key = cKeys.GetNext(pos);
		if((key->cNormal == 0x00) && (key->cShifted == 0x00))
		{
			rows++;
			if(totalWidth > longest)
				{
				longest = totalWidth;
				}
			totalWidth = 0;
		}
		else
		{
			totalWidth += abs(key->cWidth);
		}
	}

	// This is for the header
	//rows += 1;

	CRect rect;	GetClientRect(rect);

	rect.bottom -= topgap;

	KeyHeight = rect.Height() / rows;
	KeyWidth = (int)(((double)rect.Width()) / ((((double)(longest)) / 10)));
}

void CScrKbd::AddKey(char pNormal, char pShifted, int pWidth)

{
	KEYDEF * key = new KEYDEF;

	key->cNormal = pNormal;
	key->cShifted = pShifted;
	key->cWidth = pWidth;

	cKeys.AddTail(key);
}


void CScrKbd::DrawKey(CDC * dc, CRect & rc2, KEYDEF * key, BOOL cHilight)

{
	CRect rc = rc2;				// Make a copy of the rect
	
	if(!cHilight)
		{
		rc.DeflateRect(1,1);
		dc->FillSolidRect(rc,GetSysColor(COLOR_3DFACE));
		dc->Draw3dRect(rc,GetSysColor(COLOR_3DSHADOW), GetSysColor(COLOR_3DDKSHADOW));
		rc.DeflateRect(1,1);
		dc->Draw3dRect(rc,GetSysColor(COLOR_3DLIGHT), GetSysColor(COLOR_3DSHADOW));
		}
	else
		{
		rc.DeflateRect(1,1);
		//dc->FillSolidRect(rc,GetSysColor(COLOR_3DLIGHT));
		dc->FillSolidRect(rc,GetSysColor(COLOR_3DHILIGHT));

		

		dc->Draw3dRect(rc,GetSysColor(COLOR_3DLIGHT), GetSysColor(COLOR_3DSHADOW));
		rc.DeflateRect(1,1);
		dc->Draw3dRect(rc,GetSysColor(COLOR_3DSHADOW), GetSysColor(COLOR_3DDKSHADOW));
		}

	rc.DeflateRect(3,1);

	if(key->cNormal == 0x00)
		{
		CString label = " ";
		BOOL special = FALSE;

		if(key->cShifted > 0x6F) //is a function
			{
			int fkeynum = key->cShifted - 0x6F;
			label.Format("F%d",fkeynum);
			}
		else
			{
			switch(key->cShifted)
				{
				case TAB:		// 0x02
					label = " Tab-> ";
					break;
				case CAPSLOCK:	// 0x03
					label = " Caps ";
					break;

				case SHIFT:	 	// 0x04
					label = " Shift ";
					break;

				case CTRL:		// 0x05
					label = " Ctrl ";
					break;

				case BSPACE:    // 0x06
					label = " <-Bspc ";
					break;

				case ENTER:    // 0x07
					label = " Enter ";
					break;

				case ALT:		// 0x08
					label = " Alt ";
					break;

				case WINDOW:    // 0x9
					label = " Win ";
					break;
				case ESC:
					label = " Esc ";
					break;

				case LEFT:    // 0x0A
					special = TRUE;
						{
						int cx = (rc.left + rc.right)/2;
						int cy = (rc.top + rc.bottom)/2;
						int siz = rc.Width()/3;
						dc->MoveTo(cx+siz, cy);
						dc->LineTo(cx-siz, cy);
						dc->LineTo(cx-siz + (siz*4)/5, cy - siz/2);
						dc->MoveTo(cx-siz, cy);
						dc->LineTo(cx-siz + (siz*4)/5, cy + siz/2);
						}
					break;

				case RIGHT:    // 0x0B
					special = TRUE;
						{
						int cx = (rc.left + rc.right)/2;
						int cy = (rc.top + rc.bottom)/2;
						int siz = rc.Width()/3;
						dc->MoveTo(cx-siz, cy);
						dc->LineTo(cx+siz, cy);
						dc->LineTo(cx+siz - (siz*4)/5, cy - siz/2);
						dc->MoveTo(cx+siz, cy);
						dc->LineTo(cx+siz - (siz*4)/5, cy + siz/2);
						}
					break;

				case UP:    // 0x0C
					special = TRUE;
						{
						int cx = (rc.left + rc.right)/2;
						int cy = (rc.top + rc.bottom)/2;
						int siz = rc.Width()/4;
						dc->MoveTo(cx, cy+siz);
						dc->LineTo(cx, cy-siz);
						dc->LineTo(cx - (2*siz)/3, cy-siz + (siz*4)/5);
						dc->MoveTo(cx, cy-siz);
						dc->LineTo(cx + (2*siz)/3, cy-siz + (siz*4)/5);
						}
					break;

				case DOWN:    // 0x0D
					special = TRUE;
						{
						int cx = (rc.left + rc.right)/2;
						int cy = (rc.top + rc.bottom)/2;
						int siz = rc.Width()/4;
						dc->MoveTo(cx, cy-siz);
						dc->LineTo(cx, cy+siz);
						dc->LineTo(cx - (2*siz)/3, cy+siz - (siz*4)/5);
						dc->LineTo(cx, cy+siz);
						dc->LineTo(cx + (2*siz)/3, cy+siz - (siz*4)/5);
						}
					break;

				case PUP: //0x0F
					label = " PgUp ";
					break;

				case PDN: //0x10
					label = " PgDn ";
					break;

				case HOME: //0x11
					label = " Home ";
					break;

				case END: //0x12
					label = " End ";
					break;

				case INS: //0x13
					label = " Ins ";
					break;

				case DEL: //0x14
					label = " Del ";
					break;

				case WWW:
					label = " Web ";
					break;

				default:
					//ASSERT(FALSE);
					label = "#ERR#";
					break;
				}
			}
		// Draw label
		if(!special)
			{
			CSize tsize;
			tsize = dc->GetTextExtent(label);
			dc->TextOut(
				((rc.left + rc.right)/2) - (tsize.cx/2), 
				((rc.top + rc.bottom)/2) - (tsize.cy/2), 
				label);
			}
		}
	else
		{
		if((key->cShifted >= 'A') && (key->cShifted <= 'Z'))
			{
			CSize tsize;
			tsize = dc->GetTextExtent(key->cShifted);
			dc->TextOut(
				((rc.left + rc.right)/2) - (tsize.cx/2), 
				((rc.top + rc.bottom)/2) - (tsize.cy/2), 
				key->cShifted);
			}
		else
			{
			CSize tsize;
			tsize = dc->GetTextExtent('M');
			dc->TextOut(rc.left, rc.top, key->cShifted);
			dc->TextOut(
				rc.right - tsize.cx, 
				rc.bottom - tsize.cy,
				key->cNormal);
			}
		}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CScrKbd::OnPaint() 

{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CGrayDlg::OnPaint();
		DrawKeyboard();
	}
}

//////////////////////////////////////////////////////////////////////////
// Send the key to the last active application window

void CScrKbd::SendKeyToWin(KEYDEF * keydef)

{
	if(oldwin && IsWindow(oldwin->m_hWnd))
		{
		//oldwin->SetFocus();
		//SendKey(keydef);
		//support.YieldToWinEx();
		//SetFocus();	
		}
	else
		{
		// Just send it already
		//SendKey(keydef);
		}
}

//////////////////////////////////////////////////////////////////////////
// Imitate what the driver does except send messages to window

VOID CScrKbd::keybd_event2(
  BYTE bVk,           // virtual-key code
  BYTE bScan,         // hardware scan code
  DWORD dwFlags,      // flags specifying various function options
  DWORD dwExtraInfo   // additional data associated with keystroke
)

{
	//keybd_event(bVk, bScan, dwFlags, dwExtraInfo);

	if(oldwin && IsWindow(oldwin->m_hWnd))
		{
		P2N("keybd_event2 bVk=%x bScan=%x dwFlags=%x\r\n", bVk, bScan, dwFlags);

		DWORD msg, flag = 0;
		
		if(dwFlags & KEYEVENTF_KEYUP)
			msg = WM_KEYUP, flag = (1 << 30) | 1 << 31;
		else
			msg = WM_KEYDOWN;
		
		oldwin->PostMessage(msg, bVk, (DWORD)bScan << 16 | flag);
		}
	else
		{
		//msg beep
		Beep(100, 5);
		}
}

void CScrKbd::SendKey(KEYDEF * keydef)

{
	if(keydef->cNormal == 0x00)
		{
		int vk;
		BOOL uptoo = TRUE;
		if(keydef->cShifted > 0x6F) // is a function key
			{
			vk = keydef->cShifted;
			}
		else
			{
			switch(keydef->cShifted)
				{
				case TAB:    // 0x02
					vk = VkKeyScan(0x09) & 0xFF;
					break;

				case BSPACE:    // 0x06
					vk = VK_BACK;
					break;

				case ENTER:    // 0x07
					vk = VkKeyScan(0x0D) & 0xFF;
					break;
			
				case WINDOW:    // 0x9
					vk = VK_LWIN;
					break;

				case LEFT:    // 0x0A
					vk = VK_LEFT;
					break;

				case RIGHT:    // 0x0B
					vk = VK_RIGHT;
					break;

				case UP:    // 0x0C
					vk = VK_UP;
					break;

				case DOWN:    // 0x0D
					vk = VK_DOWN;
					break;

				case PUP: //0x0F
					vk = VK_PRIOR;
					break;

				case PDN: //0x10
					vk = VK_NEXT;
					break;

				case HOME: //0x11
					vk = VK_HOME;
					break;

				case END: //0x12
					vk = VK_END;
					break;

				case INS: //0x13
					vk = VK_INSERT;
					break;

				case DEL: //0x14
					vk = VK_DELETE;
					break;

//				case WWW:
//					KEYDEF key;
//					key.cNormal = key.cShifted = 'H';
//					SendKey(&key);
//					key.cNormal = key.cShifted = 'T';
//					SendKey(&key);
//					SendKey(&key);
//					key.cNormal = key.cShifted = 'P';
//					SendKey(&key);
//					key.cNormal = key.cShifted = ':';
//					SendKey(&key);
//					key.cNormal = key.cShifted = '/';
//					SendKey(&key);
//					SendKey(&key);
//					key.cNormal = key.cShifted = 'W';
//					SendKey(&key);
//					SendKey(&key);
//					SendKey(&key);
//					vk = VkKeyScan('.') & 0xFF;
//					break;

				case ESC:

					break;

				default:
					//ASSERT(FALSE);
					P2N("CScrKbd::SendKey -- Unkown Key\r\n");
					break;
				}
			}

		keybd_event2(vk,0,0,0);

		if(uptoo)
			{
			keybd_event2(vk, 0, KEYEVENTF_KEYUP, 0);
			}
	}
	else
		{
		char	pChar = keydef->cNormal;
		SHORT	ks = VkKeyScan(pChar);
		BYTE	key = ks & 0xFF;

		keybd_event2(key, 0,0,0);
		keybd_event2(key, 0,KEYEVENTF_KEYUP,0);

#if 1 
		// turn off the control and shift if they were down
		unsigned char vk = VK_SHIFT;
		if(GetMyKeyState(vk) & 0xF000)
			{
			shift = 0;
			//keybd_event2(vk, 0, KEYEVENTF_KEYUP, 0);
			}

		vk = VK_CONTROL;
		if(GetMyKeyState(vk) & 0xF000) //allow toggle
			{
			control = 0;
			//keybd_event2(vk, 0, KEYEVENTF_KEYUP, 0);
			}

		vk = VK_MENU;
		if(GetMyKeyState(vk) & 0xF000) //allow toggle
			{
			alt = 0;
			//keybd_event2(vk, 0, KEYEVENTF_KEYUP, 0);
			}

#endif

		}
}

void CScrKbd::OnLButtonDown(UINT nFlags, CPoint point) 

{
	CRect client; GetClientRect(client);

	int x = 0, y = -KeyHeight + topgap;
	sendthis = NULL;

	KEYDEF *key; 
	POSITION pos = cKeys.GetHeadPosition();
	
	while(pos)
		{
		key = cKeys.GetNext(pos);

		if((key->cNormal == 0x00) && (key->cShifted == 0x00))
			{
			x = 0; y += KeyHeight;
			}
		else
			{
			int width = ((abs(key->cWidth) * KeyWidth) / 10);
			CRect rc(x,y,x + width, y + KeyHeight);
			if(key->cWidth < 0)
				{
				rc.right = client.right;
				}
			x += width;

			if(rc.PtInRect(point))
				{
				cCurrentKeyState = GetKBState();

				sendthis = key;
				sendrc = rc;

				UpdateStates(sendthis);
				// Redraw key to reflect press
				CClientDC dc(this);

				//CRect client; GetClientRect(client);
				//CRgn clipRgn; clipRgn.CreateRectRgnIndirect(&client);
				//dc.SelectClipRgn(&clipRgn);

				CFont* oldFont = NULL;
				PickFont(&dc, client, oldFont);
				dc.SetBkMode(TRANSPARENT);

				DrawKey(&dc, sendrc, sendthis, true);

				if(oldFont)
					dc.SelectObject(oldFont);
				}
			}
		}

	CGrayDlg::OnLButtonDown(nFlags, point);
}

void CScrKbd::OnLButtonUp(UINT nFlags, CPoint point) 

{
	if(sendthis)
		{
		//P2N("Sending key '%c'\r\n", sendthis->cNormal);

		// Redraw key to reflect release
		CClientDC dc(this);
		CRect client; GetClientRect(client);

		//CRgn clipRgn; clipRgn.CreateRectRgnIndirect(&client);
		//dc.SelectClipRgn(&clipRgn);
		
		CFont* oldFont = NULL;
		PickFont(&dc, client, oldFont);		
		dc.SetBkMode(TRANSPARENT);

		DrawKey(&dc, sendrc, sendthis, false);

		if(oldFont)
			dc.SelectObject(oldFont);

		SendKey(sendthis);
		//SendKeyToWin(sendthis);		
		}

	// Repaint whole if changed shift/ctrl states
	if(GetKBState() != cCurrentKeyState)
		{
		DrawKeyboard();
		}
	CGrayDlg::OnLButtonUp(nFlags, point);
}


void CScrKbd::OnRButtonDown(UINT nFlags, CPoint point) 
{
	OnLButtonDown(nFlags, point);
	CGrayDlg::OnRButtonDown(nFlags, point);
}

void CScrKbd::OnRButtonUp(UINT nFlags, CPoint point) 
{
	OnLButtonUp(nFlags, point);
	CGrayDlg::OnRButtonUp(nFlags, point);
}

void CScrKbd::OnSize(UINT nType, int cx, int cy) 

{
	//P2N("CScrKbd::OnSize nType=%x, cx=%d, cy=%d\r\n", nType, cx, cy); 

	CGrayDlg::OnSize(nType, cx, cy);

	if(IsWindow(m_hWnd))
	{
		if(cKeys.GetCount())
		{
			CalcWidthHeight();
			Invalidate();
		}
	}
}

DWORD CScrKbd::GetKBState() 

{
	int state = 0;
	short ks;

	ks = GetMyKeyState(VK_CAPITAL);

	if(ks & 0xF000) 
		state |= 0x01;

	ks = GetMyKeyState(VK_SHIFT);
	if(ks & 0xF000) 
		state |= 0x02;
	
	ks = GetMyKeyState(VK_CONTROL);
	if(ks & 0xF000) 
		state |= 0x04;
	
	ks = GetMyKeyState(VK_MENU);
	if(ks & 0xF000) 
		state |= 0x08;
	
	P2N("GetKBState %x\r\n", state);

	return state;
}

void CScrKbd::DrawKeyboard()

{
	CClientDC dc(this);

	CRect client; GetClientRect(client);

	//CRgn clipRgn; clipRgn.CreateRectRgnIndirect(&client);
	//dc.SelectClipRgn(&clipRgn);

	CFont* oldFont = NULL;

	PickFont(&dc, client, oldFont);

	dc.SetBkMode(TRANSPARENT);
	int x = 0, y = -KeyHeight + topgap;

	KEYDEF *key;
	
	POSITION pos = cKeys.GetHeadPosition();

	int state = GetKBState();
	
	while(pos)
		{
		key = cKeys.GetNext(pos);
		if((key->cNormal == 0x00) && (key->cShifted == 0x00))
			{
			x = 0;
			y += KeyHeight;
			}
		else
			{
			int width = ((abs(key->cWidth) * KeyWidth) / 10);
			CRect rc(x,y,x + width, y + KeyHeight);
			x += width;
			if(key->cWidth < 0)
				{
				rc.right = client.right; 
				}

			BOOL hilight = FALSE;
			if(key->cNormal == 0x00)
				{
				switch(key->cShifted)
					{
					case CAPSLOCK:
						if(state & 0x01)
							{
							hilight = TRUE;
							}
						break;

					case SHIFT:
						if(state & 0x02)
							{
							hilight = TRUE;
							}
						break;

					case CTRL:
						if(state & 0x04)
							{
							hilight = TRUE;
							}
						break;

					case ALT:
						if(state & 0x08)
							{
							hilight = TRUE;
							}
						break;

					default:
						break;
					}
				}
			DrawKey(&dc, rc, key, hilight);
			}
		}

	if(oldFont)
		dc.SelectObject(oldFont);

}

void CScrKbd::OnDestroy()

{
	P2N("CScrKbd::OnDestroy()\r\n");
	
	// Free allocated resources
	POSITION pos = cKeys.GetHeadPosition();

	while(pos)
		{
		delete cKeys.GetNext(pos);
		}

	// In case key state is not consistent
	int state = GetKBState();

	if(state & 0x01) keybd_event2(VK_CAPITAL, 0, KEYEVENTF_KEYUP,0);
	if(state & 0x02) keybd_event2(VK_SHIFT, 0, KEYEVENTF_KEYUP,0);
	if(state & 0x04) keybd_event2(VK_CONTROL, 0, KEYEVENTF_KEYUP,0);

	
	CGrayDlg::OnDestroy();
}

//////////////////////////////////////////////////////////////////////////
// Build the keyboard definition table

void CScrKbd::BuildKeyDef()

{

	//////////////////////////////////////////////////////////////////////////
	// Control row

	AddKey(0x00,0x00,0); //signal row change
	AddKey(0x00,ESC,10);
	for(int fnkey = 0x70; fnkey < 0x7C; fnkey++)
		{
		AddKey(0x00,fnkey & 0xFF, 11);
		}
	AddKey(0x00,INS,14);
	AddKey(0x00,DEL,-10);

	//////////////////////////////////////////////////////////////////////////
	// First row
	
	AddKey(0x00,0x00,1); //signal row change
	AddKey('`','~',10);
	AddKey('1','!',10);
	AddKey('2','@',10);
	AddKey('3','#',10);
	AddKey('4','$',10);
	AddKey('5','%',10);
	AddKey('6','^',10);
	AddKey('7','&',10);
	AddKey('8','*',10);
	AddKey('9','(',10);
	AddKey('0',')',10);
	AddKey('-','_',10);
	AddKey('=','+',10);
	AddKey(0x00,BSPACE,18); //backspace
	AddKey(0x00,PUP,-12);

	//////////////////////////////////////////////////////////////////////////
	// Second row
	
	AddKey(0x00,0x00,2); //signal row change
	AddKey(0x00,TAB,14); //tab
	AddKey('q','Q',10);
	AddKey('w','W',10);
	AddKey('e','E',10);
	AddKey('r','R',10);
	AddKey('t','T',10);
	AddKey('y','Y',10);
	AddKey('u','U',10);
	AddKey('i','I',10);
	AddKey('o','O',10);
	AddKey('p','P',10);
	AddKey('[','{',10);
	AddKey(']','}',10);
	AddKey('\\','|',10);
	AddKey(0x00,PDN,-12);

	//////////////////////////////////////////////////////////////////////////
	// Third row
	
	AddKey(0x00,0x00,3);		//signal row change
	AddKey(0x00,CAPSLOCK,18);	//caps lock
	AddKey('a','A',10);
	AddKey('s','S',10);
	AddKey('d','D',10);
	AddKey('f','F',10);
	AddKey('g','G',10);
	AddKey('h','H',10);
	AddKey('j','J',10);
	AddKey('k','K',10);
	AddKey('l','L',10);
	AddKey(';',':',10);
	AddKey('\'','\"',10);
	AddKey(0x00,ENTER,16);		//enter
	AddKey(0x00,HOME,-10);

	//////////////////////////////////////////////////////////////////////////
	// Fourth row

	AddKey(0x00,0x00,4);		//signal row change
	AddKey(0x00,SHIFT,22);		//shift
	AddKey('z','Z',10);
	AddKey('x','X',10);
	AddKey('c','C',10);
	AddKey('v','V',10);
	AddKey('b','B',10);
	AddKey('n','N',10);
	AddKey('m','M',10);
	AddKey(',','<',10);
	AddKey('.','>',10);
	AddKey('/','?',10);
	AddKey(0x00,SHIFT,22);		//shift
	AddKey(0x00,END,-10);

	//////////////////////////////////////////////////////////////////////////
	// Fifth row

	AddKey(0x00,0x00,5);		//signal row change
	AddKey(0x00,CTRL,12);		//ctrl
	//AddKey(0x00,WINDOW,12);	//window
	AddKey(0x00,ALT,12);		//alt
	AddKey(' ',' ',62);
	AddKey(0x00,ALT,12);		//alt
	AddKey(0x00,CTRL,12);		//ctrl
	AddKey(0x00,LEFT,12);		//left
	AddKey(0x00,UP,12);			//up
	AddKey(0x00,DOWN,12);		//down
	AddKey(0x00,RIGHT,12);		//right

	//AddKey(0x00,WWW,-10);		//http://WWW
}

void CScrKbd::OnSetFocus(CWnd* pOldWnd) 

{
	CGrayDlg::OnSetFocus(pOldWnd);
	oldwin = pOldWnd;

	//P2N("CScrKbd::OnSetFocus %p\r\n", pOldWnd);	
}

void CScrKbd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 

{
	lpMMI->ptMinTrackSize.y = 150;
	lpMMI->ptMinTrackSize.x = 250;	
	
	CGrayDlg::OnGetMinMaxInfo(lpMMI);
}

short	CScrKbd::GetMyKeyState(DWORD key)

{
	int ret = 0;

	switch(key)
		{
		case VK_CAPITAL:
			if(cap)
				ret = 0x8000;
			break;

		case VK_SHIFT:
			if(shift)
				ret = 0x8000;
			break;

		case VK_CONTROL:
			if(control)
				ret = 0x8000;
			break;

		case VK_MENU:
			if(alt)
				ret = 0x8000;
			break;
		}

	return ret;
}

//////////////////////////////////////////////////////////////////////////
// Pick a font for display

void CScrKbd::PickFont(CClientDC *pDC, CRect &rc, CFont *oldFont)

{
	if(rc.right > 600)
		{
		//P2N("Drawing larger font\r\n");
		oldFont = pDC->SelectObject(&bigfont);
		}
	else if(rc.right > 300)
		{
		//P2N("Drawing regular font\r\n");
		pDC->SelectObject(GetStockObject(DEFAULT_GUI_FONT));
		}
	else
		{	
		//P2N("Drawing smaller font\r\n");
		oldFont = pDC->SelectObject(&smallfont);
		}

}


//////////////////////////////////////////////////////////////////////////
// Set Shift states

void	CScrKbd::UpdateStates(KEYDEF *keydef)

{
	P2N("CScrKbd::UpdateStates %d\r\n", keydef->cShifted);

	if(keydef->cNormal == 0x00)
		{
		switch(keydef->cShifted)
			{	
			case CAPSLOCK:    // 0x03
				cap = !cap;
				break;

			case SHIFT:    // 0x04
				shift = !shift;
				break;

			case CTRL:    // 0x05
				control = !control;
				break;

			case ALT:    // 0x08
				alt = !alt;
				break;
			}
		}
}
