
/* =====[ PGedit.cpp ]========================================== 
                                                                             
   Description:     The dbtest project, implementation of the PGedit.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  5/20/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

//////////////////////////////////////////////////////////////////////////
// PGedit.cpp : implementation file
//

#include "stdafx.h"
#include "dbtest.h"
#include "mxpad.h"
#include "PGedit.h"
#include "io.h"
#include "direct.h"

#include "sys/stat.h"
#include "support.h"

#include "LevenStein.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define APSTUDIO_INVOKED
#undef APSTUDIO_READONLY_SYMBOLS
#include "resource.h"

#undef APSTUDIO_INVOKED

//////////////////////////////////////////////////////////////////////////

#define ID_SPELL_WORD1		_APS_NEXT_COMMAND_VALUE+1
#define ID_SPELL_WORD2		ID_SPELL_WORD1+1
#define ID_SPELL_WORD3		ID_SPELL_WORD1+2
#define ID_SPELL_WORD4		ID_SPELL_WORD1+3
#define ID_SPELL_WORD5		ID_SPELL_WORD1+4
#define ID_SPELL_WORD6		ID_SPELL_WORD1+5
#define ID_SPELL_WORD7		ID_SPELL_WORD1+6
#define ID_SPELL_WORD8		ID_SPELL_WORD1+7
#define ID_SPELL_WORD9		ID_SPELL_WORD1+8
#define ID_SPELL_WORD10		ID_SPELL_WORD1+9

#define ID_SPELL_IGNORE		ID_SPELL_WORD1+11
#define ID_SPELL_ADD		ID_SPELL_WORD1+12
#define ID_SPELL_MORE		ID_SPELL_WORD1+13

#define ID_SPELL_MED1		ID_SPELL_WORD1+14
#define ID_SPELL_MED2		ID_SPELL_WORD1+15
#define ID_SPELL_MED3		ID_SPELL_WORD1+16
#define ID_SPELL_MED4		ID_SPELL_WORD1+17
#define ID_SPELL_MED5		ID_SPELL_WORD1+18
#define ID_SPELL_MED6		ID_SPELL_WORD1+19
#define ID_SPELL_MED7		ID_SPELL_WORD1+20
#define ID_SPELL_MED8		ID_SPELL_WORD1+21
#define ID_SPELL_MED9		ID_SPELL_WORD1+22
#define ID_SPELL_MED10		ID_SPELL_WORD1+23

//////////////////////////////////////////////////////////////////////////
// Names for the dictinaries

static CString Cuserdic ("userspell.txt");
static CString Cmaindic ("spell.txt");
static CString Cmeddic  ("medspell.txt");

int strdiff(const CString &str1, const CString &str2);
int strdiff2(const CString &str1, const CString &str2);
int strdist(const CString &str1, const CString &str2);

/////////////////////////////////////////////////////////////////////////////
// CPGedit

// Static members of the CPGEdit spell box
// The compiler will init them, but for good measure .... us too

int				CPGedit::dic_good = 0;		// Flag to load dic only once

FILE			*CPGedit::fp1 = NULL;		// FH main dic
FILE			*CPGedit::fp2 = NULL;		// FH med dic
FILE			*CPGedit::fp3 = NULL;		// FH custom dic

CDWordArray     CPGedit::widx;				// Index arrays to dics
CDWordArray     CPGedit::wmedidx;

// Usually, these are external, autofill them before use
//CString approot, dataroot, appdataroot;

//CString appdirname("xraynotes");

CPGedit::CPGedit()

{
	//P2N("CPGedit::CPGedit %p\r\n", this);

	in_spell = false;

	// Became STATIC members, DO NOT init them
	//fp1 = fp2 = fp3 = NULL;

	amsdelay = 500;
	amaxsuggest = 8;

	anoautoselect = firsttime = 0;
	oldcheckon = acheckon = true;
	m_uiTimerID = 0;
	oldline = 0;

	m_Drawn = false;
	ameddic = true;
	disabled = false;
	
	grey = RGB(192, 192, 192);
	NewBrush.CreateSolidBrush(grey);

//	guestuser getusername

	if(approot == "")
		{
		support.GetAppRoot(approot);

		//char *buff = approot.GetBuffer(MAX_PATH + 1);
		//GetModuleFileName(AfxGetInstanceHandle(), buff, MAX_PATH);

		// get application root
		//char *ptr = strrchr(buff, '\\');
		//if(ptr)
		//	*(ptr+1) = '\0';

		//approot.ReleaseBuffer();

		}

	if(droot == "")
		{
		support.GetAppDataDir(droot);

		//if(support.IsAdmin() )
		//	{
		//	dataroot = "c:\\xraynotes\\";
		//	}
		//else
		//	{
		//	support.GetAppDataDir(dataroot);
		//	}	
		}

	if(wdroot == "")
		{
		support.GetAppDataDir(wdroot);

		//CString adata;

		//support.GetSpecialFolder(CSIDL_APPDATA, appdataroot);
		//support.RepairPath(appdataroot);

		//appdataroot += appdirname + "\\";

		//if(_access(appdataroot, 0) < 0)
		//	{
		//	_mkdir(appdataroot);
		//	}

		//if(_access(appdataroot, 2) < 0)
		//	{
		//	xMsgBox("Cannot access application data storage, switching to c:\\tmp\\");
		//	appdataroot = "c:\\tmp\\";
		//	}

		P2N("Got appdataroot: '%s'\r\n", wdroot);
		}
}

CPGedit::~CPGedit()

{
	//P2N("CPGedit::~CPGedit() %p\r\n", this);
}

BEGIN_MESSAGE_MAP(CPGedit, CEdit)
	//{{AFX_MSG_MAP(CPGedit)
	ON_WM_CHAR()
	ON_WM_PAINT()
	ON_WM_KEYUP()
	ON_WM_TIMER()
	//ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
	ON_WM_HSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_SPELL_MED1, ID_SPELL_MED10, ReplaceMedError)
	ON_COMMAND_RANGE(ID_SPELL_WORD1, ID_SPELL_WORD10, ReplaceSpellingError)
	ON_COMMAND(ID_SPELL_ADD, zAddWordToDic)
	ON_COMMAND(ID_SPELL_MORE, MoreSuggest)
	ON_COMMAND(ID_SPELL_IGNORE, zIgnoreWord)
	ON_COMMAND(ID_EDIT_COPY, Copy)
	ON_COMMAND(ID_EDIT_PASTE, Paste)
	ON_COMMAND(ID_EDIT_CUT, Cut)
	ON_COMMAND(ID_EDIT_SELECT_ALL, SelAll)
	
END_MESSAGE_MAP()


void CPGedit::zAddWordToDic()

{
	P2N("Add Word To Dic %fp3 %s\r\n", fp3, currword);

	currword.MakeLower();

	if(fp3)
		{
		fseek(fp3, 0, SEEK_END);
		fprintf(fp3, "%s\n", currword);

		int ss,ee;
		GetSel(ss,ee);
		SetSel(ee,ee);

		m_Drawn = false;
		RedrawWindow();
		}
	//m_dwLastTick = GetTickCount();
}

void CPGedit::zIgnoreWord()

{
	//P2N("Ignore Word %s\r\n", currword);

	currword.MakeLower();
	ignore.AddTail(currword);
	
	int ss,ee;
	GetSel(ss,ee);
	SetSel(ee,ee);

	m_Drawn = false;
	RedrawWindow();

	//m_dwLastTick = GetTickCount();
}

void CPGedit::MoreSuggest()

{
	//P2N("More Suggestions\r\n");

	int offset;
	CStringList strlist;

	Distance dist;
	CString str2(currword);
	str2.MakeLower();

	// Figure out starting letter
    if(widx.GetSize() > (str2[0] - 'a'))
        offset =  widx.GetAt(str2[0] - 'a');

	//P2N("Starting at buffer offset %d\r\n", offset);
	// Look up in dictionary

	if(!fp1)
		return;

    fseek(fp1, offset, SEEK_SET);

	// Fill in selection
	while(TRUE)
		{
		if(!fgets(buffer, MAX_READ, fp1) )
			break;

		int len = strlen(buffer) - 1;
		buffer[len] = '\0';
		 
		for(int loop = 0; loop < len; loop++)
			{
			buffer[loop] = tolower(buffer[loop]);
			}

		//P2N("Distance [%s] against [%s]\r\n'  ", str2, buffer);
		//if(cnt++ > 1000)
		//	break;

		// Is ascii?
		if(tolower(buffer[0]) >= 'a' && tolower(buffer[0]) <= 'z')
			{
			if(tolower(buffer[0]) !=  str2[0])
				break;
			}

		int ddd = dist.LD(buffer, str2);

		if(ddd < 4 )
			{
			//P2N("Suggested %s\r\n", buffer);
			CString tmp; tmp.Format("%d %s", ddd, buffer);
			strlist.AddTail(tmp);
			//P2N("more suggestions: %s\r\n", tmp);
			}	
		}
}

void CPGedit::ReplaceMedError(UINT uid)

{
	// This is a hack, I do not know why and how -- but it works
	uid -= 30;

	P2N("ReplaceMedError iud=%d\r\n", uid - ID_SPELL_MED1);
	
	int idx = min(int(uid - ID_SPELL_MED1), replacemed.GetUpperBound());

	// Propagate capitalisation -- we assume it is caps on more than half
	int loop, upcount = 0, len = currword.GetLength();
	for(loop = 0; loop < len; loop++)
		{
		if(isupper(currword[loop]))
			upcount++;
		}
	if(upcount > len / 2)
		replacemed[idx].MakeUpper();
	else if(isupper(currword[0]))
		replacemed[idx].SetAt(0, toupper(replacemed[idx][0]));

	ReplaceSel(replacemed[idx], true);
}

/////////////////////////////////////////////////////////////////////////////

void CPGedit::ReplaceSpellingError(UINT uid)

{
	// This is a hack, I do not know why and how --
	// I figured this one out -- 1 based + command for separator
	// in med context it is 4

	uid -=2;

	P2N("ReplaceSpellingError iud=%d\r\n", uid - ID_SPELL_WORD1);

	int idx = min(int(uid - ID_SPELL_WORD1), replace.GetUpperBound() );

	//P2N("Clicked Range %d %s\r\n", uid - ID_SPELL_WORD1, replace[idx]);

	// Propagate capitalisation -- we assume it is caps on more than half
	int loop, upcount = 0, len = currword.GetLength();
	for(loop = 0; loop < len; loop++)
		{
		if(isupper(currword[loop]))
			upcount++;
		}

	if(upcount > len / 2)
		replace[idx].MakeUpper();
	else if(isupper(currword[0]))
		replace[idx].SetAt(0, toupper(replace[idx][0]));

	ReplaceSel(replace[idx], true);
}

void CPGedit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 

{
	//P2N("CPGedit WM_CHAR %d\r\n", nChar);

	//if(daysused > DEMO_DAYS && nChar != VK_TAB)
	//	{
		//Coco *cc = ((Coco *)AfxGetApp()->m_pMainWnd);
		//ASSERT(cc);
		//cc->m_prog.SetText(
		//MBOX("Editing Disabed");
		//MessageBox("Editing Disabled.", "YellowMemo Licensing");
		//SetFocus();
		//return;
	//	}

	m_dwLastTick = ::GetTickCount() - 500;
	m_Drawn = false;

	if(isalnum(nChar) || nChar ==  VK_SPACE || nChar ==  8)
		{
		
		}

	switch(nChar)
		{
		case 1: // ctrl-a
			SetSel(0, -1);
			break;

		case 3: // ctrl-c
			//Copy();
			break;

		//case VK_SPACE:
		//	P2N("Space in char\r\n");
		//	m_dwLastTick = ::GetTickCount();
		//	m_Drawn = false;

		break;
		}


		//P2N("CEditEx::OnChar (UINT nChar=%d ctrl=%d)\r\n", nChar, GetKeyState(VK_CONTROL) & 0x80);

	// CTRL-Z Handle undo
	if((GetKeyState(VK_CONTROL) & 0x80) && nChar == 26)
		{
		//P2N("CEditEx::OnChar Undo\r\n");

		if(sarr.GetSize() > 0)
			{
			int startt, endd;

			GetSel(startt, endd);

			//for(int loop = 0; loop < sarr.GetSize(); loop++)
			//	{
			//	CString sss = sarr.GetAt(loop);
			//	P2N("Stack '%s'\r\n", sss);
			//	}

			// Remove the last entry
			sarr.RemoveAt(sarr.GetUpperBound());
			
			CString str, str2;
			if(sarr.GetSize())
				{
				str = sarr.GetAt(sarr.GetUpperBound());
				}
			GetWindowText(str2);
			rsarr.Add(str2);

			SetWindowText(str);

			// Remove the newly changed one
			//if(sarr.GetSize())
			//	sarr.RemoveAt(sarr.GetUpperBound());

			//SetSel(startt, endd);
			//SetSel(-1, -1);

			SetSel(str.GetLength(), str.GetLength());
			return;
			}
		}

	// CTRL-Y Handle redo
	if((GetKeyState(VK_CONTROL) & 0x80) && nChar == 25)
		{
		//P2N("CEditEx::OnChar Redo\r\n");

		//for(int loop = 0; loop < rsarr.GetSize(); loop++)
		//	{
		//	CString sss = rsarr.GetAt(loop);
		//	P2N("rStack '%s'\r\n", sss);
		//	}

		CString str;
		if(rsarr.GetSize())
			{
			str = rsarr.GetAt(rsarr.GetUpperBound());

			// Remove the redone item
			rsarr.RemoveAt(rsarr.GetUpperBound());
			SetWindowText(str);
			sarr.Add(str);
			SetSel(str.GetLength(), str.GetLength());	
			}
		}

	CEdit::OnChar(nChar, nRepCnt, nFlags);
}


void CPGedit::OnPaint() 

{
	CEdit::OnPaint();

	//P2N("PaintedCEdit\r\n");

	CClientDC dc(this);
		
	CRect ClientRect;
	GetClientRect(ClientRect);

	//NOWAY CPaintDC dc(this); // device context for painting

	if(!acheckon)
		return;
	
	if(m_Drawn)
		return;

	//////////////////////////////////////////////////////////////////////

	CFont* pFont = GetFont();
	CFont* pOldFont = NULL;
	CBrush* pNewBrush = NULL;
	CBrush* pOldBrush = NULL;
	CPen* pNewPen = NULL;
	CPen* pOldPen = NULL;
	
	pOldFont = dc.SelectObject(pFont);
	
	pNewBrush = new CBrush;	ASSERT(pNewBrush);
	VERIFY(pNewBrush->CreateSolidBrush(RGB(255,0,0)));
	
	pNewPen = new CPen; ASSERT(pNewPen);
	VERIFY(pNewPen->CreatePen(PS_SOLID, 1, RGB(255,150,150)));

	pOldBrush = dc.SelectObject(pNewBrush);
	pOldPen = dc.SelectObject(pNewPen);

	CRgn rgn;
	rgn.CreateRectRgn(ClientRect.left, ClientRect.top, ClientRect.right, ClientRect.bottom);
	dc.SelectClipRgn(&rgn);
	
	// CEdit
	int iline = GetFirstVisibleLine();
	int	tline = GetLineCount();

	// Determine current line:
	int start, cend;
	GetSel(start, cend);

	//P2N("Start:%d end:%d\r\n", start, cend);

	int currline = LineFromChar(start);

	CString str;
	GetLine(currline , str);
	ProcessLine(currline, str, ClientRect, dc);

#if 0
	// Painted every line
	int curr = 0, prev = 0;
	for(int loop = iline; loop <= tline + 1; loop++)
		{
		curr = LineIndex(loop);
		if((start <= curr) &&  (start >= prev))
			{
			//P2N("Curr in line %d\r\n", loop);
			CString str;
			GetLine(loop - 1, str);
			ProcessLine(loop - 1, str, ClientRect, dc);
			prev = curr;
			break;
			}
		}
#endif

	m_Drawn = TRUE;

	dc.SelectObject(pOldFont);
	dc.SelectObject(pOldBrush);
	dc.SelectObject(pOldPen);

	delete(pNewBrush);
	delete(pNewPen);
	
	//P2N("Painted pgedit\r\n");
}

///////////////////////////////////////////////////////////

int	CPGedit::ProcessLine(int line, CString str, CRect &rect, CDC &dc)

{
	CPoint pt;
	CSize ss;

	//P2N("line='%s'\r\n", str);

	CString word;
	int offs = 0, len = str.GetLength();

	for(int loop = 0; loop < len; loop++)
		{
		if(!isalpha(str[loop]))
			{
			if(word != "")
				{		
				ProcessWord(word, LineIndex(line) + offs, dc);
				}
			offs = loop + 1;
			word = "";
			}
		else
			{
			word += str[loop];
			}
		}
	// Output leftover 
	if(word != "")
		{
		ProcessWord(word, LineIndex(line) + offs, dc);
		}

	//if(pt.y > rect.Height())
	//	return false;
	//else

		return true;
}


void	CPGedit::ProcessWord(CString &word, int pos, CDC &dc)

{
	CSize  ss = dc.GetTextExtent(word);
	CPoint pt = PosFromChar(pos);

	if(!SpellWord(word))
		{
		//P2N("LWord: %s x=%d y=%d ww=%d\r\n\r\n", word, pt.x, pt.y, ss.cx);
		zDrawSquigly(dc, pt.x, pt.y + ss.cy, ss.cx);
		}

	//Sleep(100);
}


//////////////////////////////////////////////////////////////////////////

//DEL void CPGedit::AttachEdit(CWnd *pWnd, UINT uiControlID)
//DEL 
//DEL {
//DEL 	//P2N("Attaching\r\n");
//DEL 
//DEL 	ASSERT(!IsWindow(GetSafeHwnd()));
//DEL 
//DEL 	ASSERT(pWnd);
//DEL 	ASSERT(::IsWindow(pWnd->GetSafeHwnd()));
//DEL 
//DEL 	CWnd* pEdit = pWnd->GetDlgItem(uiControlID);
//DEL 	ASSERT(pEdit);
//DEL 	ASSERT(::IsWindow(pEdit->GetSafeHwnd()));
//DEL 
//DEL 	SubclassWindow(pEdit->GetSafeHwnd());
//DEL }

//////////////////////////////////////////////////////////////////////////

void CPGedit::zDrawSquigly(CDC &dc, int iLeftX, int iY, int iWidth)

{
	//P2N("Drawing squiggly xx=%d yy=%d ww=%d\r\n", iLeftX, iY, iWidth);
	
	int	step = 2;
	int	vstep = 2;

	CRect ClientRect;
	GetClientRect(ClientRect);

	if(iLeftX   >= ClientRect.Width())
		return;

	if(iY + step >= ClientRect.Height())
		return;
	
	int iCurrentY = iY;
	int iCurrentX = iLeftX;

	//iCurrentX = max(iCurrentX, 0);
	//iWidth = min(ClientRect.Width() - 2 * step, iLeftX + iWidth);

	while (iCurrentX <= iLeftX + iWidth - 2 * step)
		{
		dc.MoveTo(iCurrentX, iY);

		dc.LineTo(iCurrentX + step , iY + vstep);
	
		dc.LineTo(iCurrentX + 2 * step, iY);
		
		iCurrentX += 2 * step;
		}
}

//////////////////////////////////////////////////////////////////////////

void CPGedit::GetLine(int idx, CString &str)

{
	int maxlen  = 5 * _MAX_PATH;

	char *mem = str.GetBuffer(maxlen);

	int len = CEdit::GetLine(idx, mem, maxlen);

	if(len < maxlen)
		mem[len] = 0;

	str.ReleaseBuffer();
}

BOOL CPGedit::PreTranslateMessage(MSG* pMsg) 

{
	// Create our timer when appropriate
	if(m_uiTimerID == 0)
		{
		if (::IsWindow(GetSafeHwnd()))
			{
			//P2N("initial timer ID\r\n");
			m_uiTimerID = SetTimer(1, 10, NULL);
			}
		}
	return CEdit::PreTranslateMessage(pMsg);
}

void CPGedit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	switch (nChar)
	{
	case  VK_UP:case  VK_DOWN:
		zCheckLineChange();
		break;
	}
	CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CPGedit::OnTimer(UINT nIDEvent) 
{

	//P2N("Timer for spell\r\n");

	KillTimer(nIDEvent);

	if(!firsttime)
		{
		firsttime = true;
		zInit_Spellcheck();
		}

	// if timer event is the spell checker event, check it now
	if (nIDEvent == m_uiTimerID)
	{
		DWORD dwCount = GetTickCount() - m_dwLastTick;

		if (dwCount > (DWORD)amsdelay && !m_Drawn)
			{
			//P2N("Timer for spell %d\r\n",m_uiTimerID);

			//RedrawSpellingErrors();
			RedrawWindow();
			//m_Drawn = TRUE;
			m_dwLastTick = GetTickCount();	
			}
	m_uiTimerID = SetTimer(m_uiTimerID, 100, NULL);
	}

	CEdit::OnTimer(nIDEvent);
}

void CPGedit::OnChange() 

{
	if(sarr.GetSize() > 100)
		sarr.RemoveAt(0);

	CString str;
	
	GetWindowText(str);
	
	//P2N("CPGedit::OnChange '%s'\r\n", str);

	sarr.Add(str);	

	SetModify(true);
	m_dwLastTick = GetTickCount();

	//m_Drawn = false;	
}

//DEL void CPGedit::OnVscroll() 
//DEL 
//DEL {
//DEL 	//P2N("Vscroll\r\n");	
//DEL }

void CPGedit::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 

{
	if(nSBCode == SB_ENDSCROLL)
		{
		//P2N("Hscrollbar msg %d\r\n", nSBCode);
		m_Drawn = false;
		RedrawWindow();
		}

	CEdit::OnHScroll(nSBCode, nPos, pScrollBar);
}

//DEL int CPGedit::indict(CString &str)
//DEL {
//DEL 	return (cnt++ % 2 == 0);
//DEL }

void CPGedit::OnLButtonDown(UINT nFlags, CPoint point) 

{
	CEdit::OnLButtonDown(nFlags, point);
	zCheckLineChange();
}

void CPGedit::zCheckLineChange()

{
	int start, cend;
	GetSel(start, cend);

	int currline = LineFromChar(start);
	if(currline != oldline	)
		{
		//P2N("line change\r\n");
		oldline = currline;
		m_Drawn = false;
		m_dwLastTick = GetTickCount();
		}
}

void CPGedit::OnRButtonDown(UINT nFlags, CPoint point) 

{
	// Nothing to do here
	if(!acheckon)
		{
		CEdit::OnRButtonDown(nFlags, point);
		return;
		}

	int ss,ee;
	GetSel(ss,ee);

	//P2N("Current Selection %d %d\r\n", ss, ee);

	CPoint cp(point);
	CMenu Menu;

	int pos = CharFromPos(point) & 0xffff;
	int line = CharFromPos(point) >> 16;

	//P2N("Right click pos=%d line=%d\r\n", pos, line);

	int idx = LineFromChar(pos);
	int linebeg = pos - LineIndex(idx);

	CString str;
	GetLine(idx, str);

	//P2N("Line on click pos=%d idx=%d '%s'\r\n", pos, idx, str);
	
	// Get Current word
	CString word, word2;
	int offs = 0, len = str.GetLength();

	for(int loop = 0; loop < len; loop++)
		{
		if(!isalpha(str[loop]) || str[loop] == '\'')
			{
			if(word != "")
				{
				if((linebeg >= offs) && (linebeg <=loop))
					{
					word2 = word;
					word = "";
					//P2N("Got Word: '%s'\r\n", word2);
					break;
					}
				}
			offs = loop + 1;
			word = "";
			}
		else
			{
			word += str[loop];
			}
		}
	// Output leftover 
	if(word != "")
		{
		if(linebeg >= offs && linebeg <=loop)
			{
			word2 = word;
			word = "";
			//P2N("Got Word: '%s'\r\n", word2);	
			}
		}

	if(ss == ee)
		{
		SetSel(LineIndex(idx) + offs, LineIndex(idx) + loop);
		}

	currword = word2;

	int res = SpellWord(word2);

	Menu.CreatePopupMenu();

	if(res)
		{
		Menu.AppendMenu(MF_GRAYED, 0, "Correct Spelling");
		}
	else
		{
		Menu.AppendMenu(MF_GRAYED, 0, "*** Spelling Mistake ***");
		}

	Menu.AppendMenu(MF_SEPARATOR, 0, "");
	Menu.AppendMenu(MF_STRING, ID_EDIT_COPY, "Copy");
	Menu.AppendMenu(MF_STRING, ID_EDIT_PASTE, "Paste");

	Menu.AppendMenu(MF_STRING, ID_EDIT_CUT, "Cut");
	Menu.AppendMenu(MF_STRING, ID_EDIT_SELECT_ALL, "Select All");
	
	if(!res)
		{
		Menu.AppendMenu(MF_SEPARATOR, 0, "");
		Menu.AppendMenu(MF_STRING, ID_SPELL_ADD, "Add to Dictionary");
		Menu.AppendMenu(MF_STRING, ID_SPELL_IGNORE, "Ignore Spelling");		
		}

	Menu.AppendMenu(MF_SEPARATOR, 0, "");

	// Suggested words:
	Suggest(Menu, word2);

	//Menu.AppendMenu(MF_SEPARATOR, 0, "");

	ClientToScreen(&cp);
	Menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON,
								cp.x, cp.y, this);
}

//////////////////////////////////////////////////////////////////////////

void CPGedit::zInit_Spellcheck()

{
	//P2N("CPGedit::zInit_Spellcheck %p\r\n", this);

	if(dic_good)
		return;

	dic_good = true;

	CWaitCursor wc;

    //////////////////////////////////////////////////////////////////
    // Assign file names

	dicmain = approot; dicmain +=  Cmaindic;    
	dicidx  = wdroot; dicidx +=  "spell.idx";

	dicmed	= approot; dicmed += Cmeddic;
	dicmedidx = wdroot; dicmedidx += "medspell.idx";

	dicuser = wdroot; dicuser +=  Cuserdic;

	//P2N("CPGedit::zInit_Spellcheck Dictionary: %s\r\n", dicmain);

	// message ("Started Spell Check");
    if(access(dicmain, 0))
        {
        CString num;
        num.Format("Cannot open Dictionary: %s", dicmain);
        MBOX(num);
        return;
        }
	
	//////////////////////////////////////////////////////////////////
	// Check for index, recreate if needed
	zIndexDic(dicmain, dicidx);

	// Also, do medical dictionary

	if(ameddic)
		{
		//P2N("CPGedit::zInit_Spellcheck Medical dictionary: %s\r\n", dicmed);

		if(access(dicmed, 0))
			{
			CString num;
			num.Format("Cannot open Medical Dictionary: %s", dicmed);
			MBOX(num);
			return;
			}
	
		// Check for index, recreate if needed
		zIndexDic(dicmed, dicmedidx);
		}
	
    if(_access(dicuser, 02) < 0)
		{
        FILE *fp3 = fopen(dicuser, "w+t");

        if(!fp3)
            {
			MBOX("Cannot open/create user dictionary file\n");
			return;
			}
		else
			{
			fclose(fp3);
			}
		}

	// Load indexes into memory  (only 26 entries)
	zLoadIndex(dicidx, widx);

	if(ameddic)
		{
		zLoadIndex(dicmedidx, wmedidx);
		}

	//////////////////////////////////////////////////////////////////////
	// Keep open file handles   (only 3 entries)

	fp1 = fopen(dicmain, "rt");
    if(!fp1)
        {
        MBOX("Internal: Cannot open dictionary data file\n");
        }

	//////////////////////////////////////////////////////////////////////
	fp2 = fopen(dicmed, "rt");
    if(!fp2)
        {
        MBOX("Internal: Cannot open med dictionary data file\n");
        }
    
	fp3 = fopen(dicuser, "r+t");
    if(!fp3)
        {
		MBOX("Internal: Cannot open/create user dictionary file\n");
		}
}

//////////////////////////////////////////////////////////////////////////

int		CPGedit::SpellWord(CString &str)

{
	int		offset = 0, ret = 0;
	int     cnt = 0, start = 0, end = 0;

    //in_spell = 0;

	if(str == "")
		return true;

    if(in_spell)
        return true;

    in_spell = TRUE;

	CString str2(str);	str2.MakeLower();

	//P2N("Spelling word dic_good=%d fp1=0x%x idxsize=%d '%s'\r\n", dic_good, fp1, widx.GetSize(), str2);
	//P2N("Spelling word '%s'\r\n", str2);

	// Check ignore list
	POSITION pos1 = ignore.GetHeadPosition();
	while(true)
		{
		if(!pos1)
			break;

		CString &xstr = ignore.GetNext(pos1);
		
		//P2N("Ignore scan '%s' ==> '%s'\r\n", xstr, str2);
		
		// Ignore this one as GOOD
		if(xstr == str2)
			{
			ret = true; goto endd;
			}
		}

	// Check user dictionary
	if(fp3)
		{
		fseek(fp3, 0, SEEK_SET);
		while(TRUE)
			{
			if(!fgets(buffer, MAX_READ, fp3) )
				break;

			buffer[strlen(buffer)-1] = '\0';

			//P2N("usedic check [%s] against [%s]\r\n'  ", str2, buffer);

			// Ignore this one as GOOD
			if(strcmp(buffer, str2) == 0)
				{
				ret = true; goto endd;
				}
			}
		}

	// Look up in dictionary
	if(fp1)
		{	
		// Figure out starting letter
		if(widx.GetSize() > (str2[0] - 'a'))
			offset =  widx.GetAt(str2[0] - 'a');

		//P2N("Main Dic: Starting at buffer offset %d\r\n", offset);
		
		fseek(fp1, offset, SEEK_SET);
		while(TRUE)
			{
			if(!fgets(buffer, MAX_READ, fp1) )
				break;

			buffer[strlen(buffer)-1] = '\0';

			// Top show searching effect
			//static int cnt = 0;
			//if(cnt++%1000==0)
			//	P2N("check [%s] against [%s]\r\n'  ", str2, buffer);

			if(!stricmp(str2, buffer) )
				{
				//P2N("Found: '%s' ", buffer);
				ret = true;
				break;
				}

			// Next letter, break

			// Is ascii?
			if(tolower(buffer[0]) >= 'a' && tolower(buffer[0]) <= 'z')
				{
				if(tolower(buffer[0]) !=  str2[0])
					break;
				}
			}
		}

	// Found, search no further
	if(ret)
		goto endd;

	// Look up in med dictionary
    if(fp2 && ameddic)
		{
		// Figure out starting letter

		if(wmedidx.GetSize() > (str2[0] - 'a'))
			offset =  wmedidx.GetAt(str2[0] - 'a');

		//P2N("Starting at med buffer offset %d\r\n", offset);

		fseek(fp2, offset, SEEK_SET);
		while(TRUE)
			{
			if(!fgets(buffer, MAX_READ, fp2) )
				break;

			buffer[strlen(buffer)-1] = '\0';

			//P2N("med check [%s] against [%s]\r\n'  ", str2, buffer);

			if(!stricmp(str2, buffer) )
				{
				P2N("Found in med: '%s'\r\n", buffer);
				ret = true;
				break;
				}

			// Next letter, break

			// Is ascii?
			if(tolower(buffer[0]) >= 'a' && tolower(buffer[0]) <= 'z')
				{
				if(tolower(buffer[0]) !=  str2[0])
					break;
				}
			}
		}
    
endd:

    in_spell = FALSE;

	//if(!ret)
	//	P2N("Misspelled word '%s'\r\n", str);

	return ret;
}

int comparestr(const void *elem1, const void *elem2 ) 

{
	CString *ptr1 = *((CString **)elem1);
	CString *ptr2 = *((CString **)elem2);

	//P2N("Compare %s to %s\r\n", *ptr1, *ptr2);

	return(ptr1->CompareNoCase(*ptr2));
}


void CPGedit::Suggest(CMenu &menu, CString str)

{
	int cnt = 0, offset = 0, command = ID_SPELL_WORD1;

	if(str == "")
		return;

	replace.RemoveAll();

	CString str2(str);
	str2.MakeLower();

	P2N("Suggesting %s\r\n", str);

	// Figure out starting letter
    if(widx.GetSize() > (str2[0] - 'a'))
        offset =  widx.GetAt(str2[0] - 'a');

	if(fp1) 
		{
		//P2N("Starting at buffer offset %d\r\n", offset);
		// Look up in dictionary
		fseek(fp1, offset, SEEK_SET);

		CStringList strlist;

		Distance dist;

		// Fill in selection
		while(TRUE)
			{
			if(!fgets(buffer, MAX_READ, fp1) )
				break;

			int len = strlen(buffer) - 1;
			buffer[len] = '\0';
			 
			for(int loop = 0; loop < len; loop++)
				{
				buffer[loop] = tolower(buffer[loop]);
				}

			//P2N("Distance [%s] against [%s]\r\n'  ", str2, buffer);
			//if(cnt++ > 1000)
			//	break;

			// Is ascii?
			if(tolower(buffer[0]) >= 'a' && tolower(buffer[0]) <= 'z')
				{
				if(tolower(buffer[0]) !=  str2[0])
					break;
				}

			// Levenstein diff
			int ddd = dist.LD(buffer, str2);		
			if(ddd < 3 ) // ||  eee < 3)
				{
				//P2N("Suggested %s\r\n", buffer);
				CString tmp; tmp.Format("%d %s", ddd, buffer);
				strlist.AddTail(tmp);
				}	
			}

		// Buld array
		CPtrArray  arr;

		POSITION pos1 = strlist.GetHeadPosition();
		while(true)
			{
			if(!pos1)
				break;
			CString &str = strlist.GetNext(pos1);
			arr.Add(&str);
			}

		// Verify array
		int size = arr.GetSize();

		//for(int loop = 0; loop < size; loop++)
		//	{
		//	CString *tmp = (CString *)arr.GetAt(loop);
		//	P2N("PTR %s\r\n", *tmp);
		//	}

		qsort(arr.GetData(), size, sizeof(void *), comparestr);

		int cnt2 = 0;
		for(int loop2 = 0; loop2 < size; loop2++)
			{
			CString *tmp = (CString *)arr.GetAt(loop2);
		
			if(cnt2 == 0)
				{
				CString tmp2; tmp2.Format("Suggestions for: '%s'", str);
				menu.AppendMenu(MF_GRAYED, 0, tmp2);
				menu.AppendMenu(MF_SEPARATOR, 0, "");
				}

			*tmp = tmp->Mid(2);
			replace.Add(*tmp);

			//P2N("Appending %d %s\r\n", loop2, *tmp);
			menu.AppendMenu(MF_STRING, command + loop2, *tmp);

			if(++cnt2 >= amaxsuggest) 
				break;
			}

		if(cnt2 == 0)
			{
			CString tmp2; tmp2.Format("No suggestions for: '%s'", str);
			menu.AppendMenu(MF_GRAYED, 0, tmp2);
			//menu.AppendMenu(MF_STRING, 0, str);
			}
	}

	//////////////////////////////////////////////////////////////////////
	// Med Suggestions:

	
	if(fp2) 
		{
		replacemed.RemoveAll();

		command = ID_SPELL_MED1;

		// Figure out starting letter
	    if(wmedidx.GetSize() > (str2[0] - 'a'))
		    offset =  wmedidx.GetAt(str2[0] - 'a');

		//P2N("Starting at buffer offset %d\r\n", offset);
		// Look up in dictionary
		fseek(fp2, offset, SEEK_SET);

		CStringList strlist;

		Distance dist;

		// Fill in selection
		while(TRUE)
			{
			if(!fgets(buffer, MAX_READ, fp2) )
				break;

			int len = strlen(buffer) - 1;
			buffer[len] = '\0';
			 
			for(int loop = 0; loop < len; loop++)
				{
				buffer[loop] = tolower(buffer[loop]);
				}

			//P2N("Distance [%s] against [%s]\r\n'  ", str2, buffer);
			//if(cnt++ > 1000)
			//	break;

			// Is ascii?
			if(tolower(buffer[0]) >= 'a' && tolower(buffer[0]) <= 'z')
				{
				if(tolower(buffer[0]) !=  str2[0])
					break;
				}

			// Levenstein diff
			int ddd = dist.LD(buffer, str2);		
			if(ddd < 3 ) // ||  eee < 3)
				{
				//P2N("Suggested %s\r\n", buffer);
				CString tmp; tmp.Format("%d %s", ddd, buffer);
				strlist.AddTail(tmp);
				}	
			}

		// Buld array
		CPtrArray  arr;

		POSITION pos1 = strlist.GetHeadPosition();
		while(true)
			{
			if(!pos1)
				break;
			CString &str = strlist.GetNext(pos1);
			arr.Add(&str);
			}

		// Verify array
		int size = arr.GetSize();

		//for(int loop = 0; loop < size; loop++)
		//	{
		//	CString *tmp = (CString *)arr.GetAt(loop);
		//	P2N("PTR %s\r\n", *tmp);
		//	}

		qsort(arr.GetData(), size, sizeof(void *), comparestr);

		int cnt2 = 0;
		for(int loop2 = 0; loop2 < size; loop2++)
			{
			CString *tmp = (CString *)arr.GetAt(loop2);
		
			if(cnt2 == 0)
				{
				menu.AppendMenu(MF_SEPARATOR, 0, "");
				CString tmp2; tmp2.Format("Med suggestions for: '%s'", str);
				menu.AppendMenu(MF_GRAYED, 0, tmp2);
				menu.AppendMenu(MF_SEPARATOR, 0, "");
				}

			*tmp = tmp->Mid(2);
			replacemed.Add(*tmp);

			//P2N("Appending %d %s\r\n", loop2, *tmp);
			menu.AppendMenu(MF_STRING, command + loop2, *tmp);

			if(++cnt2 >= amaxsuggest) 
				break;
			}

		if(cnt2 == 0)
			{
			CString tmp2; tmp2.Format("No Med suggestions for: '%s'", str);
			menu.AppendMenu(MF_SEPARATOR, 0, "");
			menu.AppendMenu(MF_GRAYED, 0, tmp2);
			//menu.AppendMenu(MF_STRING, 0, str);
			}
		}

	//menu.AppendMenu(MF_SEPARATOR, 0, "");
	//menu.AppendMenu(MF_STRING, ID_SPELL_MORE, "More Suggestions ...");
}

/////////////////////////////////////////////////////////////////////////////
// Return estimated number of patches needed between strings

int strdiff(const CString &str1, const CString &str2)

{
    char chh1, chh2;
    char chh11, chh22;
    char chh33, chh44;
    int idx1 = 0, idx2 = 0;
    int idx11 = 0, idx22 = 0;
    int idx33 = 0, idx44 = 0;
    int len1 = str1.GetLength();
    int len2 = str2.GetLength();
    int diff = 0;

	//if(abs(len1-len2) > 4)
	//	return 4;

    //P2N("strdiff %s - %s  ", str1, str2);
    
	if(!len1)
		return(len2);

	if(!len2)
		return(len1);

    while(TRUE)
    	{
        if(idx1 >= len1)
            break;

        if(idx2 >= len2)
            break;

        chh1 = str1.GetAt(idx1);
        chh2 = str2.GetAt(idx2);

        if(chh1 != chh2)
        {
        int diff2 = 0;
        int diff3 = 0;
        int diff4 = 0;

        idx11 = idx1;
        idx22 = idx2;
        idx33 = idx1;
        idx44 = idx2;

        // try str1, walk str2
        while(TRUE)
            {
            if(idx22 >= len2)
                break;

            chh22 = str2.GetAt(idx22);
            if(chh1 == chh22)
                break;

            diff2++;
            idx22++;
            }
        // try str2, walk str1
        while(TRUE)
            {
            if(idx11 >= len1)
                break;

            chh11 = str1.GetAt(idx11);
            if(chh11 == chh2)
                break;

            diff3++;
            idx11++;
            }

        // Walk both
        while(TRUE)
            {
            if(idx33 >= len1)
                break;

            if(idx44 >= len2)
                break;

            chh33 = str1.GetAt(idx33);
            chh44 = str2.GetAt(idx44);
            if(chh33 == chh44)
                break;

            diff4++;
            idx33++;
            idx44++;
            }

        //P2N("diff2=%d diff3=%d diff4=%d  ", diff2, diff3, diff4);

        // Assume closest match
        if(diff2 < diff3)
            {
            if(diff4 < diff2)
                {
                idx1 = idx33;
                idx2 = idx44;
                diff += diff4;
                }
            else
                {
                idx2 = idx22;
                diff += diff2;
                }
            }
        else
            {
            if(diff4 < diff3)
                {
                idx1 = idx33;
                idx2 = idx44;
                diff += diff4;
                }
            else
                {
                idx1 = idx11;
                diff += diff3;
                }
            }
        }
        idx1++;
        idx2++;
    }

    //P2N(" diff=%d\r\n", diff);

    return(diff);
}

int strdiff2(const CString &str1, const CString &str2)

{
    char chh1, chh2;
    char chh11, chh22;
    char chh33, chh44;
    int idx1 = 0, idx2 = 0;
    int idx11 = 0, idx22 = 0;
    int idx33 = 0, idx44 = 0;
    int len1 = str1.GetLength();
    int len2 = str2.GetLength();
    int diff = 0;

	//if(abs(len1-len2) > 4)
	//	return 4;

    //P2N("strdiff %s - %s  ", str1, str2);
    
	if(!len1)
		return(len2);

	if(!len2)
		return(len1);

    while(TRUE)
    	{
        if(idx1 >= len1)
            break;

        if(idx2 >= len2)
            break;

        chh1 = str1.GetAt(idx1);
        chh2 = str2.GetAt(idx2);

        if(chh1 != chh2)
        {
        int diff2 = 0;
        int diff3 = 0;
        int diff4 = 0;

        idx11 = idx1;
        idx22 = idx2;
        idx33 = idx1;
        idx44 = idx2;

        // try str1, walk str2
        while(TRUE)
            {
            if(idx22 >= len2)
                break;

            chh22 = str2.GetAt(idx22);
            if(chh1 == chh22)
                break;

            diff2++;
            idx22++;
            }
        // try str2, walk str1
        while(TRUE)
            {
            if(idx11 >= len1)
                break;

            chh11 = str1.GetAt(idx11);
            if(chh11 == chh2)
                break;

            diff3++;
            idx11++;
            }

        // Walk both
        while(TRUE)
            {
            if(idx33 >= len1)
                break;

            if(idx44 >= len2)
                break;

            chh33 = str1.GetAt(idx33);
            chh44 = str2.GetAt(idx44);
            if(chh33 == chh44)
                break;

            diff4++;
            idx33++;
            idx44++;
            }

       // P2N("diff2=%d diff3=%d diff4=%d  ", diff2, diff3, diff4);

        // Assume closest match
        if(diff2 > diff3)
            {
            if(diff4 > diff2)
                {
                idx1 = idx33;
                idx2 = idx44;
                diff += diff4;
                }
            else
                {
                idx2 = idx22;
                diff += diff2;
                }
            }
        else
            {
            if(diff4 > diff3)
                {
                idx1 = idx33;
                idx2 = idx44;
                diff += diff4;
                }
            else
                {
                idx1 = idx11;
                diff += diff3;
                }
            }
        }
        idx1++;
        idx2++;
    }

    //P2N(" diff=%d\r\n", diff);
    return(diff);
}

//
// Return the distance between 2 strings
//

int strdist(const CString &str1, const CString &str2)

{
    int len1 = str1.GetLength();
    int len2 = str2.GetLength();
    int diff = 0;

	return(diff);
}

void CPGedit::OnDestroy() 

{
	CEdit::OnDestroy();
	
	//P2N("Destroy PGedit %p\r\n", this);	
	//if(fp1) fclose(fp1);
	//if(fp3) fclose(fp3);
}

void CPGedit::OnSetFocus(CWnd* pOldWnd) 

{
	CEdit::OnSetFocus(pOldWnd);
	m_Drawn = false;

	if(anoautoselect)
		SetSel(-1, -1);
	//P2N("Setfocus\r\n");	
}

void CPGedit::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 

{
	if(nSBCode == SB_ENDSCROLL)
		{
		//P2N("Vscrollbar msg %d\r\n", nSBCode);
		m_Drawn = false;
		RedrawWindow();
		}
	CEdit::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CPGedit::SelAll()

{
	SetSel(0, -1);
	//Copy();
}

HBRUSH CPGedit::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	//P2N("CEditEx::CtlColor %d\r\n", nCtlColor); 

	if(disabled)
		{
		pDC->SetBkColor(grey);
		return NewBrush;
		}
	else
		return NULL;
}
	
void CPGedit::aDisable()

{
	oldcheckon = acheckon;
	acheckon = false;
	disabled = true;
	SetReadOnly(true);
}

void CPGedit::aEnable()

{
	acheckon = oldcheckon;
	disabled = false;
	SetReadOnly(false);
}

BOOL CPGedit::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult) 

{
	//P2N("CEditEx::OnChildNotify %d %s wParam=%d lParam=%d \r\n", message, mxpad.num2msg(message), wParam, lParam);	

	if(message == WM_COMMAND)
		{
		//P2N("Window handle=%d control=%d EN_CHANGE=%d message=%d\r\n", lParam,  LOWORD(wParam), EN_CHANGE, HIWORD(wParam));

		if(EN_CHANGE == HIWORD(wParam))
			{
			OnChange();
			}
		}
	return CEdit::OnChildNotify(message, wParam, lParam, pLResult);
}

//////////////////////////////////////////////////////////////////////////
// Index dictionary

int	CPGedit::zIndexDic(CString &dicf, CString &idxf)

{
	int crflag = false;
	
	CString fname(dicf); support.PathToFname(fname);
	CString idxname(idxf); support.PathToFname(idxname);

	if(access(idxf, 0) < 0)
		{
		crflag = true;
		}
	else
		{
		struct _stat dicbuf, idxbuf;

		// Just in case it blows up ...
		dicbuf.st_mtime =  idxbuf.st_mtime = 0;

		_stat(dicf, &dicbuf );
		_stat(idxf, &idxbuf );

 		//P2N("Stat modify dic: %d idx: %d\r\n",
		//						dicbuf.st_mtime, idxbuf.st_mtime);

		if(dicbuf.st_mtime > idxbuf.st_mtime)
			crflag = true;
		}

	// Create index
	if(crflag)
        {
		CString cont;

		GetWindowText(cont);

        CString num;
        num.Format("CPGedit::zInit_Spellcheck Creating dictionary index '%s'\r\n", idxf);
		P2N(num);
		//message(num);

        char    old = 0;
        int     found   = FALSE, count = 0, count2 = 0;

        FILE *fp1a = fopen(dicf, "rt");

        if(!fp1a)
            {
            MBOX("Cannot open dictionary data file\n");
            return 0;
            }

        FILE *fp2b = fopen(idxf, "w+t");

        if(!fp2b)
            {
  			CString idxn2  = wdroot; idxn2 +=  idxname;

			P2N("CPGedit::zInit_Spellcheck Cannot create dictionary index file, switching to user dir %s\n", idxn2);

			fp2b = fopen(idxn2, "w+t");
			
			if(!fp2b)
				{
				MBOX("Cannot create dictionary index file\n");
				return 0;
				}	
            }
	
        while(TRUE)
            {
            char    *readd;
            long    fpos;

            fpos  = ftell(fp1a);				/* remember prev pos */
            readd = fgets(buffer, MAX_READ, fp1a);

			count++;
			if(count % 10000 == 0)
				{
				CString str; str.Format("Generating dictionary index for %s (%d), Please wait ...", fname, count);
				SetWindowText(str);
				support.YieldToWinEx();
				}

			// Is ascii?
			if(tolower(buffer[0]) >= 'a' && tolower(buffer[0]) <= 'z')
				{	
				if(tolower(buffer[0]) != old)				/* new buffer */
					{
					fprintf(fp2b, "%d %c\n", fpos, tolower(buffer[0]));
					//P2N("boundary word: %s -- %d\r\n", buffer, fpos);
					old = tolower(buffer[0]);
					count2++;
					}
				}
			
			if(feof(fp1a))
				break;

            if(!readd)
                break;
            }

        fclose(fp1a); fclose(fp2b);
	
		SetWindowText(cont);

		//support.YieldToWinEx();

		P2N("Created index %s %d index entries %d unique\r\n", idxf, count, count2);
		}
	return 0;
}

//////////////////////////////////////////////////////////////////////////

void	CPGedit::zLoadIndex(CString &idxf, CDWordArray &warr)

{
	int cnt = 0;

	//P2N("Loading index %s\r\n", idxf);

	FILE *fp2z = fopen(idxf, "rt");

	if(!fp2z)
        {
        MBOX("Internal: Cannot open dictionary index file\n");
		return;
		}

	// Load index to memory
    while(TRUE)
        {
        char    *readd, letter;
        int     pos;

        readd = fgets(buffer, MAX_READ, fp2z);

		if(feof(fp2z))
			break;
			
        if(!readd)
            break;

        sscanf(buffer, "%d %c", &pos, &letter);

		//P2N("index=%d letter=%c\r\n", pos, letter);

		warr.SetAtGrow(cnt, pos);
        cnt++;
        }

    fclose(fp2z);

	//P2N("Loaded index for dic: '%s' Size=%d\r\n", idxf, warr.GetSize());
}

