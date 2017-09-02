
/* =====[ ShadesFunc.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the ShadesFunc.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  1/28/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// ShadesFunc.cpp: implementation of the CShadesFunc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include <stdio.h>
#include <io.h>
#include <direct.h>

#include "xraynotes.h"
#include "multimon.h"
#include "ShadesFunc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "stdafx.h"
#include "xraynotes.h"
#include "shades.h"

#include "ShadesHelp.h"
#include "helperdlgs.h"

static	int		oldmon = 0;
static	int		curr = 0;
static	int		switched = 0;

static	RECT	firstrect;

typedef BOOL (CALLBACK* MONITORENUMPROC)(HMONITOR, HDC, LPRECT, LPARAM);

typedef BOOL (WINAPI *lpfnEnumDisplayMonitors )
				(HDC             hdc,
				LPCRECT         lprcClip,
				MONITORENUMPROC lpfnEnum,
				LPARAM          dwData);

BOOL CALLBACK MonitorEnumProc(
  HMONITOR hMonitor,  // handle to display monitor
  HDC hdcMonitor,     // handle to monitor-appropriate device context
  LPRECT lprcMonitor, // pointer to monitor intersection rectangle
  LPARAM dwData       // data passed from EnumDisplayMonitors
)

{
	CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);
	ASSERT(dlg->magic == SHADE_MAGIC);

	P2N("Monitor enum hMonitor=%d top=%d left=%d bottom=%d right=%d\r\n", hMonitor, lprcMonitor->top, 
		lprcMonitor->left, lprcMonitor->bottom, lprcMonitor->right)	;

	// Save first monitor params for wrap-around
	if(curr == 0)
		{
		memcpy(&firstrect, lprcMonitor, sizeof(RECT));
		}

	if (oldmon >= curr)
		{
		// Do nothing	
		}
	else
		{
		switched++;
		oldmon = curr;
		dlg->MoveWindow(lprcMonitor);
		dlg->Invalidate();
		//statusdlg->CenterStatus(dlg);
		return false;
		}
	curr++;
	return true;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShadesFunc::CShadesFunc(void *ptr)
{
	//P2N("CShadesFunc::CShadesFunc  Constructed helper\r\n");
	dlg = (CShadesDlg *)ptr;
	ASSERT(dlg->magic == SHADE_MAGIC);
}

CShadesFunc::~CShadesFunc()
{

}

	
// Template for new function

#if 0

void	CShadesFunc::xxxCb(void *ptr)

{
	MBOXP("Placeholder for XXX");
}

#endif

//////////////////////////////////////////////////////////////////////////
// List all menu items:

#if 0

void	CShadesFunc::dumpmenu(CMenu *menu)

{
	//CMenu *menu = (CMenu *)menu2;

	int idx = 0;
	char buf[_MAX_PATH] = "";

	while(true)
		{	
		MENUITEMINFO MenuItemInfo;

		memset(&MenuItemInfo, 0, sizeof(MenuItemInfo));

		//ZeroMemory(&MenuItemInfo, sizeof(MenuItemInfo));
		
		MenuItemInfo.cbSize = sizeof(MenuItemInfo);
		
		MenuItemInfo.fMask = 
			MIIM_CHECKMARKS | MIIM_DATA | MIIM_ID | MIIM_STATE | MIIM_SUBMENU | MIIM_TYPE ;

		MenuItemInfo.dwTypeData = buf;
		MenuItemInfo.cch = sizeof(buf);

		int ret = menu->GetMenuItemInfo(idx, &MenuItemInfo, true);

		if(!ret)
			break;

		P2N("    UINT    idx;          %d\r\n",  	 idx); 
		//P2N("    UINT    cbSize;       %d\r\n",  	 MenuItemInfo.cbSize); 
		P2N("    UINT    fMask;        %d\r\n",      MenuItemInfo.fMask); 
		P2N("    UINT    fType;        %d\r\n",      MenuItemInfo.fType); 
		P2N("    UINT    fState;       %d\r\n",      MenuItemInfo.fState); 
		//P2N("    UINT    wID;          %d\r\n",      MenuItemInfo.wID); 
		P2N("    HMENU   hSubMenu;     %d\r\n",      MenuItemInfo.hSubMenu); 
		P2N("    HBITMAP hbmpChecked;  %d\r\n",      MenuItemInfo.hbmpChecked); 
		P2N("    HBITMAP hbmpUnchecked;%d\r\n",      MenuItemInfo.hbmpUnchecked); 
		P2N("    DWORD   dwItemData;   %d\r\n",      MenuItemInfo.dwItemData); 
		P2N("    LPTSTR  dwTypeData;   %s\r\n",      MenuItemInfo.dwTypeData); 
		P2N("    UINT    cch;          %d\r\n",      MenuItemInfo.cch); 
		
		idx++;
		}

#if 1
	for(DWORD loop = 0; loop < menu->GetMenuItemCount(); loop++)
		{
		CString str, ostr;
		int id =  menu->GetMenuItemID(loop);

		menu->GetMenuString(loop, ostr, MF_BYPOSITION); 
		P2N("Menu old: %d %d '%s'\r\n", loop, id, ostr);

		menu->GetMenuText(loop, str, MF_BYPOSITION);
		P2N("Menu: %d %d '%s'\r\n", loop, id, str);
		}
#endif
	
}

#endif


void	CShadesFunc::rCropCb(void *ptr)

{
	CPoint point = dlg->currmouse; dlg->ClientToScreen(&point);
	CXrayM *rmenu = new CXrayM; rmenu->LoadMenu(IDR_MENU7, 1);
	rmenu->Show(point.x, point.y);
}

void	CShadesFunc::CropCb(void *ptr)

{
	CXrayDoc *doc; GETCURRDOC(doc, dlg);

	RET_IF_NODOC_MSG(dlg, "Crop")
	
	ShowStatus("Cropping document ...");
}

void	CShadesFunc::IdleCb(void *ptr)

{

	dlg->Idle();

	//MBOXP("Placeholder for idle");
}

void	CShadesFunc::MeasCb(void *ptr)

{
	MBOXP("Placeholder for meas");
}

void	CShadesFunc::MagCb(void *ptr)

{
	magwin.Show(true);
}

void	CShadesFunc::rMagCb(void *ptr)

{
	CPoint point = dlg->currmouse; dlg->ClientToScreen(&point);

	CXrayM *rmenu = new CXrayM; 
	rmenu->LoadMenu(IDR_MENU9, 1);
	rmenu->Show(point.x, point.y);
}


void	CShadesFunc::SVGCb(void *ptr)

{

	MBOXP("Placeholder for SVG");
}

void	CShadesFunc::EllipseCb(void *ptr)

{
	CXrayDoc *doc; GETCURRDOC(doc, dlg);

	if(!doc)
		{
		ShowStatus("Cannot Draw Rectangle without a document or image loaded.");
		return;
		}

	if(GetKeyState( VK_SHIFT)  & 0x80)
		doc->multidraw = true;
	
	doc->drawstate = TOOL_CIRCLE;
}

void	CShadesFunc::TextCb(void *ptr)

{
	//MBOXP("Placeholder for TextCb");

	CXrayDoc *doc; GCD(doc, dlg);

	if(!doc)
		{
		ShowStatus("Cannot place Text ot Note without a document or image loaded.");
		return;
		}

	if(GetKeyState( VK_SHIFT)  & 0x80)
		doc->multidraw = true;
	
	doc->drawstate = TOOL_TEXT;
}


void	CShadesFunc::rTextCb(void *ptr)

{
	//MBOXP("Placeholder for rTextCb");

	CPoint point = dlg->currmouse;	dlg->ClientToScreen(&point);
	CXrayM *omenu = new CXrayM; ASSERT(omenu);
	omenu->LoadAndShow(IDR_MENU16, 1, point.x, point.y);
}


void	CShadesFunc::ImageCb(void *ptr)

{

	CXrayDoc *doc; GETCURRDOC(doc, dlg);

	if(!doc)
		{
		ShowStatus("Cannot place an image without a document or image loaded.");
		return;
		}

	if(GetKeyState( VK_SHIFT)  & 0x80)
		doc->multidraw = true;
	
	doc->drawstate = TOOL_IMAGE;
}

void	CShadesFunc::DrawCb(void *ptr)

{
	CXrayDoc *doc; GETCURRDOC(doc, dlg);

	if(!doc)
		{
		ShowStatus("Cannot Draw without a document or image loaded.");
		return;
		}

	if(GetKeyState( VK_SHIFT)  & 0x80)
		doc->multidraw = true;
	
	doc->drawstate = TOOL_DRAW;
}

void	CShadesFunc::RectCb(void *ptr)

{
	CXrayDoc *doc; GETCURRDOC(doc, dlg);

	if(!doc)
		{
		ShowStatus("Cannot Draw Rectangle without a document or image loaded.");
		return;
		}

	if(GetKeyState( VK_SHIFT)  & 0x80)
		doc->multidraw = true;
	
	doc->drawstate = TOOL_RECT;
}

void	CShadesFunc::LineCb(void *ptr)

{
	CXrayDoc *doc; GETCURRDOC(doc, dlg);

	if(!doc)
		{
		ShowStatus("Cannot Draw line without a document or image loaded.");
		return;
		}

	if(GetKeyState( VK_SHIFT)  & 0x80)
		doc->multidraw = true;
	
	doc->drawstate = TOOL_SLINE;

}

void	CShadesFunc::PatCb(void *ptr)

{

	//MBOXP("Placeholder for Pat");
	// CDialog CWnd
	
	CDBMain dbmain;

	dbmain.DoModal();

	//dbm.Show(true);
	//dbm.SetActiveWindow();
	
	//P2N("Started  Modal\r\n");
	//while(true)
	//	{
	//	if(dbm.RunModalLoop(MLF_NOIDLEMSG))
	//		break;
	//
	//		//if(!dbmain.ContinueModal())
	//		//	break;
	//		}
	//	P2N("Done Modal\r\n");
}

void	CShadesFunc::NewCb(void *ptr)

{

}


void	CShadesFunc::cNewCb(void *ptr)

{
	//MBOXP("Placeholder for stock");

	//CAddrEntry ae;
	//ae.DoModal();

}

void	CShadesFunc::AutoCb(void *ptr)

{
	MBOXP("Placeholder for Auto");
}


void	CShadesFunc::OptCb(void *ptr)

{
	ASSERT(dlg->magic == SHADE_MAGIC);

	HINSTANCE user32 = LoadLibrary("user32.dll");
	//P2N("User32=%d\r\n", user32);
	ASSERT(user32);

	lpfnEnumDisplayMonitors pp = (lpfnEnumDisplayMonitors)
			GetProcAddress(user32, "EnumDisplayMonitors");

	//P2N("procptr=%d\r\n", pp);
	ASSERT(pp);

	curr = 0; switched = 0;

	BOOL ret = pp(
		  NULL,				//HDC hdc,                   // handle to a display device context 
		  NULL,				//LPCRECT lprcClip,          // specifies a clipping rectangle 
		  MonitorEnumProc,	//MONITORENUMPROC lpfnEnum,  // pointer to a callback function
		  NULL				//LPARAM dwData              // data passed to the callback function 
			);

	//P2N("%d Monitors displayed\r\n", curr);

	if(!switched)
		{
		P2N("Monitors wrapped around\r\n", curr);
		oldmon = 0;
		dlg->MoveWindow(&firstrect);
		dlg->Invalidate();
		statusdlg->CenterStatus(dlg);
		}

	//MBOXP("Placeholder for Opt");
}


void	CShadesFunc::DocPropCb(void *ptr)

{
	MBOXP("Placeholder for doc prop");
}


void	CShadesFunc::RedoCb(void *ptr)

{
	if(dlg->current < 0)
		{
		ShowStatus("At least one document must be loaded before redo can operate ...");
		return ;
		}

	MBOXP("Placeholder for Redo");
}


void	CShadesFunc::SelectCb(void *ptr)

{
	if(dlg->current < 0)
		{
		ShowStatus("Cannot Select without a document loaded.");
		return;
		}
	
	//dlg->drawstate = TOOL_SELECT;
}


void	CShadesFunc::MarkCb(void *ptr)

{
	if(dlg->current < 0)
		{
		ShowStatus("Cannot Mark without a document loaded.");
		return;
		}

	//dlg->drawstate =  TOOL_MARK;
}

void	CShadesFunc::MagDocCb(void *ptr)

{
	MBOXP("Placeholder for Mag");
}


void	CShadesFunc::CutCb(void *ptr)

{
	MBOXP("Placeholder for cut");
}

void	CShadesFunc::PasteCb(void *ptr)

{
	//MBOXP("Placeholder for paste");
}

void	CShadesFunc::rPasteCb(void *ptr)

{
	// Right click for menu
	CPoint point = dlg->currmouse;	dlg->ClientToScreen(&point);

	// Get the menu body
	CXrayM *omenu = new CXrayM; ASSERT(omenu);
	omenu->LoadMenu(IDR_MENU11, 1); 
	omenu->Show(point.x, point.y);
}

void	CShadesFunc::CopyCb(void *ptr)

{
	MBOXP("Placeholder for Copy");
}

void	CShadesFunc::rCopyCb(void *ptr)

{
	//MBOXP("Placeholder for Copy");

	// Right click for menu
	CPoint point = dlg->currmouse;	dlg->ClientToScreen(&point);

	// Get the menu body
	CXrayM *omenu = new CXrayM; ASSERT(omenu);
	omenu->LoadMenu(IDR_MENU10, 1); 
	omenu->Show(point.x, point.y);
}

void	CShadesFunc::PrintCb(void *ptr)

{
	MBOXP("Placeholder for print");
}


void	CShadesFunc::MonitorCb(void *ptr)

{
	ShowStatus("Monitor Change");

	P2N("Monitor count: %d\r\n", GetSystemMetrics(SM_CMONITORS));
	P2N("virtual w: %d\r\n", GetSystemMetrics(SM_CXVIRTUALSCREEN));
	P2N("virtual h: %d\r\n", GetSystemMetrics(SM_CYVIRTUALSCREEN));
	P2N("virtual x: %d\r\n", GetSystemMetrics(SM_XVIRTUALSCREEN ));
	P2N("virtual y: %d\r\n", GetSystemMetrics(SM_YVIRTUALSCREEN ));

	dlg->windowmove = true;
}

//////////////////////////////////////////////////////////////////////////
// Operations from Menu

void	CShadesFunc::UndoCb(void *ptr)

{
	if(dlg->current < 0)
		{
		ShowStatus("At least one document must be loaded before undo can operate ...");
		return ;
		}

	CXrayDoc *doc = dlg->GetCurrDoc();
	
	if(!doc)
		return;

	ASSERT(doc->magic == DOC_MAGIC );

	ShowStatus("Undoing previous image op ...");

	if(doc->undolist.GetCount() > 0)
		{
		doc->Undo();
		doc->NewRender();
		HideStatus();
		((CShadesDlg* )dlg)->Invalidate();
		}
	else
		{
		ShowStatus("Nothing to undo ...");
		}
}


void	CShadesFunc::NormCb(void *ptr)

{
	//cdlg.parent = AfxGetApp()->m_pMainWnd;
	
	cdlg.Show();

#if 0
	if(((CShadesDlg* )dlg)->current < 0)
		{
		ShowStatus("At least one document must be loaded before ...");
		return ;
		}

	CXrayDoc *doc = (CXrayDoc*)((CShadesDlg* )dlg)->docarr[((CShadesDlg* )dlg)->current]; 

	ShowStatus("Normalizing Histogram ...");

	HCURSOR oldc = SetCursor(LoadCursor(NULL, IDC_WAIT));

	doc->AddUndo();

	doc->render->Copy(*doc->img);
	doc->render->HistogramEqualize();
	
	SetCursor(oldc);

	HideStatus();
	((CShadesDlg* )dlg)->Invalidate();
#endif

}

void	CShadesFunc::ColorCb(void *ptr)

{
	//if(((CShadesDlg* )dlg)->current < 0)
	//	{
	//	ShowStatus("At least one document must be loaded before adjusting RGB ...");
	//	return ;
	//	}

	coldlg.parent = AfxGetApp()->m_pMainWnd;

	//P2N("Bricon\r\n");
	//bdlg.DoModal();

	coldlg.Show();

	return;
}

void	CShadesFunc::BrightCb(void *ptr)

{
	//if(((CShadesDlg* )dlg)->current < 0)
	//	{
	//	ShowStatus("At least one document must be loaded before adjusting brightness/contrast ...");
	//	return ;
	//	}

	bcondlg.parent = AfxGetApp()->m_pMainWnd;

	//P2N("Bricon\r\n");

	bcondlg.Show();

	return;
}

void	CShadesFunc::ExitCb(void *ptr)

{
	dlg->EndDialog(IDOK, false);
	return;
}

void	CShadesFunc::rExitCb(void *ptr)

{
	// Right click for menu
	CPoint point = dlg->currmouse;	dlg->ClientToScreen(&point);

	// Get the menu body
	CXrayM *omenu = new CXrayM; ASSERT(omenu);
	omenu->LoadMenu(IDR_MENU12, 1); 

	// Del/Undel The secret menu
	if(!(misc.IsCtrlDown() && misc.IsAltDown()))
		{
		int idx = omenu->FindMenuItem("Dump");
		if(idx)
			{
			for(int loop = idx; loop < omenu->GetNumItems(); loop++)
				omenu->DelMenuItem(idx);
			}
		}
	omenu->Show(point.x, point.y);
	return;
}

CDocList	dlist;

void	CShadesFunc::ListDocsCb(void *ptr)

{
	static int first_run = true;

	if(((CShadesDlg* )dlg)->current < 0)
		{
		ShowStatus("At least one document must be loaded before document selection can operate ...");
		return ;
		}

	if(first_run)
		{
		first_run = false;
		dlist.Create(CDocList::IDD);
		}

	dlist.ShowWindow(SW_SHOW);
	//dlist.Invalidate();

	//::SendMessage(AfxGetApp()->m_pMainWnd->m_hWnd, WM_APP_OPEN, 0, 0);
}

#include "xConfig.h"

void	CShadesFunc::ConfCb(void *ptr)

{
	static int first_run = true;

	//xMsgBox("Conf dialog\r\n");

	CxConfig conf;

	conf.DoModal();
}

void	CShadesFunc::ToggleUICb(void *ptr)

{
	CToolShow ts; ts.SetCenterFlag(true);
	ts.DoModal();
	
#if 0
	if(magdlg.IsWindowVisible())
		{
		magdlg.Hide();
		xscroll.Hide();
		yscroll.Hide();
		gesdlg.Hide();
		}
	else
		{
		magdlg.Show();
		xscroll.Show();
		yscroll.Show();
		gesdlg.Show();
		}
#endif

}

void	CShadesFunc::rOpenFileCb(void *ptr)

{
	//P2N("OpenFileCb Right Click\r\n");

	// Right click for file menu
	CPoint point = dlg->currmouse;	dlg->ClientToScreen(&point);

	// Get the menu body
	CXrayM *omenu = new CXrayM; ASSERT(omenu);
	omenu->LoadMenu(IDR_MENU3, 1); 

	//////////////////////////////////////////////////////////////////////
	// Also Load last resent file list
	CXrayM *rr = omenu->FindSubmenu(0); ASSERT(rr);

	rr->Clear();
	//P2N("Recent file menu %p\r\n", rr);

	for(int loop6 = 0; loop6 < MAXRECENT; loop6++)
		{
		CString tmp; tmp.Format("RecentImage%d", loop6);
		CString fname = AfxGetApp()->GetProfileString(strConfig, tmp, "");

		if(fname == "")
			break;

		//P2N("Recent file %s\r\n", fname);
		rr->AppendMenuItem(fname, WM_OPEN_RECENT + loop6);
		}

	rr->AppendMenuItem("");
	rr->AppendMenuItem("Clear Recent File List", WM_CLEAR_RECENT);

	//////////////////////////////////////////////////////////////////////
	// Also Load last resent DICOM list

	CXrayM *dd = omenu->FindSubmenu(1);	ASSERT(dd);

	CString txt; dd->GetMenuItemText(0, txt);
	
	//P2N("Dicom submenu: '%s'\r\n", txt);

	dd->Clear();

	for(int loop7 = 0; loop7 < MAXRECENT; loop7++)
		{
		CString tmp; tmp.Format("RecentDICOM%d", loop7);
		CString fname = AfxGetApp()->GetProfileString(strConfig, tmp, "");

		if(fname == "")
			break;

		//P2N("Recent DICOM file %s\r\n", fname);
		dd->AppendMenuItem(fname, WM_OPEN_RECENT_DICOM + loop7);
		}

	dd->AppendMenuItem("");
	dd->AppendMenuItem("Clear Recent DICOM List", WM_CLEAR_RECENT_DICOM);

	//////////////////////////////////////////////////////////////////////
	// Also Load last resent DICOM series list
	
	CXrayM *ds = omenu->FindSubmenu(2);	ASSERT(ds);

	CString txt2; ds->GetMenuItemText(0, txt2);
	
	//P2N("Dicom series submenu: '%s'\r\n", txt2);

	ds->Clear();

	for(int loop8 = 0; loop8 < MAXRECENT; loop8++)
		{
		CString tmp; tmp.Format("RecentDICOM%d", loop8);
		CString fname = AfxGetApp()->GetProfileString(strConfig, tmp, "");

		if(fname == "")
			break;

		//P2N("Recent DICOM file %s\r\n", fname);
		ds->AppendMenuItem(fname, WM_OPEN_RECENT_DICOM_S + loop8);
		}

	ds->AppendMenuItem("");
	ds->AppendMenuItem("Clear Recent DICOM Series List", WM_CLEAR_RECENT_DICOM_S);

	omenu->Show(point.x, point.y);
}

//////////////////////////////////////////////////////////////////////////

int		CShadesFunc::OpenImageFile(const char *name)

{
    CString str, sname = support.GetFileNameExt(name);
	CXrayDoc *doc = NULL;
	
	RECT rc; dlg->GetClientRect(&rc);
	int  newcurr, renx = (rc.right - rc.left);
    double mag;
	
    P2N("In Open file %s\r\n", name);

    // Is a document by this name already open?

    int ndoc = dlg->docarr.GetSize();
    
	for(int loop = 0; loop < ndoc; loop++)
        {
        if( ((CXrayDoc*)dlg->docarr[loop])->filename == name)
            {
            xMsgBox("Document by this name is already open.", "Duplicate Open");
            goto endd;
            }
        }
	if(_access(name, 0) < 0)
		{
	    MBOXP("File '%s' does not exist", sname);
		goto endd;
		}
	if(_access(name, 04) < 0)
		{
	    MBOXP("Cannot open file '%s' for reading", sname);
		goto endd;
		}
	if(_access(name, 02) < 0)
		{
	    MBOXP("File: '%s' is read only. XrayNotes will not ne able to save changes to it.", sname);
		}

    ShowStatusP("Loading file '%s'", name);
    
    doc = new CXrayDoc;   ASSERT(doc);
    doc->filename = name;
   
	// Load and image into newly acquired doc
    if(doc->img->Load(name) == 0)
        {
        MBOXP("Cannot Load Image '%s'", name);
		delete doc;
        goto endd;
        }

	doc->img->loaded = true;

	newcurr =  dlg->docarr.Add(doc);

	AfxGetApp()->WriteProfileString(strConfig, "LastImageName", doc->filename);

    //dlg->lastfilename = name;

    // Make an intelligent assumption on initial image state
    
    if((int)doc->img->GetWidth() < rc.right)
        {
        P2N("Showstate set to full\r\n");
        doc->showstate = STATE_FULL;
        }
    if((int)doc->img->GetHeight() < rc.bottom)
        {
        doc->showstate = STATE_FULL;
        }

    dlg->current = newcurr;

    // Update mag dlg mag factor
    if(doc->showstate == STATE_FULL)
        mag = 1;
	else
		mag = (double)renx / doc->img->GetHeight();

    str.Format("%.5f", mag);   magdlg.m_mag.SetText(str);
   
    dlg->Invalidate();
    support.YieldToWinEx();

endd: 
    ;
    //dlg->DumpDocs();

	return 0;
}


void	CShadesFunc::OpenFileCb(void *ptr)

{
	//::SendMessage(AfxGetApp()->m_pMainWnd->m_hWnd, WM_APP_OPEN, 0, 0);

	//P2N("OpenFileCb App Open\r\n");

	if(oldopen == "")
		oldopen = dataroot;

	CString fileName;
	int	bOpenFileDialog = true;
	int	dwFlags = 0;
	int *pType = NULL;

	if(((CShadesApp*)AfxGetApp())->inmodal)		
		{
		ShowStatus("An instance of a Modal dialog is open already.");
		return;
		}

	//CFileDialog dlgFile(bOpenFileDialog);
	CFileDialogST dlgFile(bOpenFileDialog);

	CString title;	
	dlgFile.m_ofn.lpstrInitialDir = oldopen;
	
	if (bOpenFileDialog) 
		title="Open File"; 
	else 
		title="Save File";

	dlgFile.m_ofn.Flags |= dwFlags | OFN_ALLOWMULTISELECT | OFN_EXPLORER;

	int nDocType = (pType != NULL) ? *pType : 0;
	int nIndex = 0;

	dlgFile.m_ofn.nFilterIndex = 0; //nIndex + 1;
	CString strDefExt = "jpg";
	dlgFile.m_ofn.lpstrDefExt = strDefExt;
		
	CString strFilter;
	
	strFilter += "Common image formats (jpg, gif, png)";
	strFilter += (TCHAR)NULL;
	strFilter += "*.jpg;*.gif;*.png";
	strFilter += (TCHAR)NULL;

	strFilter += "JPEG Images (*.jpg)";	strFilter += (TCHAR)NULL;
	strFilter += "*.jpg";
	strFilter += (TCHAR)NULL;
	
	strFilter += "GIF Images (*.gif)"; 	strFilter += (TCHAR)NULL;
	strFilter += "*.gif";
	strFilter += (TCHAR)NULL;
	
	strFilter += "PNG Images (*.png)";	strFilter += (TCHAR)NULL;
	strFilter += "*.png";
	strFilter += (TCHAR)NULL;
	
	strFilter += "*.*";	strFilter += (TCHAR)NULL;
	strFilter += "*.*";
	strFilter += (TCHAR)NULL;
	strFilter += (TCHAR)NULL;

	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;
	dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(1000 *_MAX_PATH);

	BOOL bRet = (dlgFile.DoModal() == IDOK) ? TRUE : FALSE;
	
	//dlgFile.Create();
	//dlgFile.ShowWindow();
	
	fileName.ReleaseBuffer();

	if(!bRet)
		return;
	
	//P2N("Openinig %s", fileName);
	POSITION pos = dlgFile.GetStartPosition( );

	int cnt = 0;
	while(true)
		{
		if(!pos) 
			break;

		CString str = dlgFile.GetNextPathName(pos);

		P2N("Opening new file: '%s'\r\n",  str); 
		
		OpenImageFile(str);

		cnt++;
		
		support.YieldToWinEx();
		}
	
	dlg->Invalidate();
	support.YieldToWinEx();

	if(cnt > 1)
		{
		ShowStatus("Loaded All requested files.");
		}

	//if (bRet)
	//	{
	//	if (pType != NULL)
	//		{
	//		int nIndex = (int)dlgFile.m_ofn.nFilterIndex - 1;
	//		ASSERT(nIndex >= 0);
	//		}
	//	//P2N("Opening %s", fileName);
	//	//OpenFile(fileName);
	//	}

	oldopen = fileName;
	support.GetDirName(oldopen);

	return;
}

//////////////////////////////////////////////////////////////////////////

void	CShadesFunc::ScanDocCb(void *ptr)

{
	xMsgBox("Scanning new document .... ", "");
}

void	CShadesFunc::ProcCb(void *ptr)

{
	procdlg.parent = dlg;
	procdlg.Show();
}

char 	*CShadesFunc::LoadPlugin(const char *name, int com)

{
	static char buff[_MAX_PATH];


	//char *fname = "";

	P2N("Loading plugin: '%s'\r\n", name);

	// Scan the plugin table
	for(int loop = 0; loop < dlg->plugarr.GetSize(); loop++)
		{
		Plugin *pp = (Plugin *)dlg->plugarr[loop];

		ASSERT(pp->magic == PLUG_MAGIC);

		if(strcmp(name, pp->Name) == 0)
			{
			P2N("%s already loaded\r\n", name);
			return NULL;
			}		
		}

	Plugin *pplug = new Plugin;

	ASSERT(pplug);
	memset(pplug, 0, sizeof(Plugin));

	pplug->magic = PLUG_MAGIC;

	// Add it if not already loaded
	pplug->hm = LoadLibrary(name);
	
	if(!pplug->hm)
		{
		_snprintf(buff, sizeof(buff), "Cannot load plugin '%s'", name);
		return buff;
		}

	pplug->comid = com;
	strncpy(pplug->Name, name, _MAX_PATH);

	//P2N("module=%d %x\r\n", hh, hh);

	pplug->Description = (pf_rs)GetProcAddress(pplug->hm, "Description");
	
	if(!pplug->Description)
		{
		_snprintf(buff, sizeof(buff), "Cannot recognize plugin: '%s'", name);
		return buff;
		}

	int ver = 0;

	pplug->DLLVersion = (pf_ri)GetProcAddress(pplug->hm, "DLLVersion");	
	if(pplug->DLLVersion)
		{
		ver = pplug->DLLVersion();
		//P2N("Dll version %d\r\n", ver);
		}
	
	_snprintf(buff, sizeof(buff), "%s (%d.%d)", 
				pplug->Description(), ver/10, ver % 10);
	
	pplug->Process	  = (pf_ri_piii)GetProcAddress(pplug->hm, "Process");
	pplug->Message	  = (pf_rs)GetProcAddress(pplug->hm, "Message");
	pplug->Stop		  = (pf_rv)GetProcAddress(pplug->hm, "Stop");
	pplug->APIVersion = (pf_ri)GetProcAddress(pplug->hm, "APIVersion");

	dlg->plugarr.Add(pplug);

	// Dump plug array:
	//for(int loop2 = 0; loop2 < dlg->plugarr.GetSize(); loop2++)
	//	{
	//	Plugin *pp = (Plugin *)dlg->plugarr[loop2];
	//	ASSERT(pp->magic == PLUG_MAGIC);
	//	P2N("Loaded %d %s\r\n", pp->comid, pp->Name);
	//	}

	return buff;
}

void	CShadesFunc::rProcCb(void *ptr)

{
	P2N("rProcCb\r\n");
	

	CPoint point = dlg->currmouse;
	dlg->ClientToScreen(&point);
	
	int command = ID_PROCESS_DUMMY;
	command++;

	if(dlg->plugin_running)
		{
		// Cmenu
		//dlg.pmenu->ModifyODMenu("Stop Currently Running Plugin", command);
		//dlg.pmenu->EnableMenuItem(command, MF_ENABLED);

		}
	else
		{
		//dlg->ppmenu->ModifyODMenu("List of Installed Plugins:", command);
		//dlg->ppmenu->EnableMenuItem(command, MF_GRAYED);
		}

	command++;

	long hFile;	struct _finddata_t c_file;
	CString fname;

	if((hFile = _findfirst("plugins\\*.dll", &c_file )) == -1L )
		{
		P2N("No such file\r\n");
		//dlg->ppmenu->AppendMenu(MF_STRING , command, "No plugins found");
		}
	else
		{
		//P2N( "\r\nRDO HID SYS ARC  FILE         DATE %25c SIZE\r\n", ' ' );
		//P2N( "--- --- --- ---  ----         ---- %25c ----\r\n", ' ' );
			
		while (true)
			{		
			if(c_file.name[0] != '.')
				{
				command++;
				fname = "plugins\\"; fname += c_file.name;
				const char *desc = LoadPlugin(fname, command);

				if(desc)
					{
					//BCMenu *popup = dlg->ppmenu->AppendODPopupMenu(desc);

					//dlg->ppmenu->AppendMenu(MF_STRING | MF_GRAYED, ++command, desc);
					//dlg->ppmenu->AppendMenu(MF_POPUP | MF_OWNERDRAW, (int)popup->m_hMenu, desc);
					//dlg->ppmenu->DeleteMenu((char*)desc);

					//popup->AppendMenu(MF_STRING, ++command, "Run Plugin");
					//popup->AppendMenu(MF_STRING, ++command, "Show Documentation");
					
					//BCMenu *popup2 = popup->AppendODPopupMenu(desc);
					//popup2->AppendMenu(MF_STRING, ++command, "Run Pluginx");
					}
				}

			if(_findnext( hFile, &c_file ) != 0 )
				break;
			}
		_findclose( hFile );
		}

	//dumpmenu(dlg->ppmenu);

	//dlg->ppmenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, dlg, NULL);	
}

//////////////////////////////////////////////////////////////////////////

void	CShadesFunc::SaveFileCb(void *ptr)

{

	// Anything to save?
	if(dlg->current < 0 )
		{
		ShowStatus("Nothing to Save ...");
		return;
		}

	CXrayDoc *doc = (CXrayDoc*)((CShadesDlg* )dlg)->docarr[((CShadesDlg* )dlg)->current]; 
	ASSERT(doc->magic == DOC_MAGIC );

	if(!doc->changed )
		{
		ShowStatus("Document has not changed, skipping save.");
		return;
		}

	CString str;
		
	int	type = FindImageType(doc->filename);
	if(type == 	CXIMAGE_FORMAT_UNKNOWN)
		{
		str.Format("Unknown file type for Image '%s', cannot save.", doc->filename);
		xMsgBox(str);
		return;
		}

	dlg->BusyCursor();
	str.Format("Saving File '%s'", doc->filename);
	ShowStatus(str, 1);

	// Save image
	if(doc->img->Save(doc->filename, type) == 0)
		{
		str.Format("Cannot Save Image '%s'", doc->filename);
		xMsgBox(str);
		return;
		}

	str.Format("Savied File '%s' OK", doc->filename);
	ShowStatus(str, 0, 1000);
	doc->changed = false;

	dlg->BusyCursor(false);
}

//////////////////////////////////////////////////////////////////////////

void	CShadesFunc::TestCb(void *ptr)

{
	CTestDlg  td;
	td.DoModal();
}


void	CShadesFunc::CloseAll(void *ptr)

{

	// Anything to close?
	if(dlg->current < 0 )
		{
		ShowStatus("Nothing to Close ...");
		return;
		}

	// Stop autoadvance
	dlg->autoadvance = false; 
	dlg->KillTimer(2);

	if(dlg->current >= dlg->docarr.GetSize() )
		{
		ShowStatus("Internal Error: Out of bounds on close");
		return;
		}

	if( ((CXrayDoc*)dlg->docarr[dlg->current])->magic != DOC_MAGIC )
		{
		P2N("Document with bad magic\r\n");
		ShowStatus("Tried to access an unloaded document (bad magic)");
		return;
		}

	P2N("Closing ALL\r\n");

	//dlg->DumpDocs();

	int		old = dlg->current;
	void	*old_ptr = dlg->docarr[dlg->current];

	for(int loop = dlg->docarr.GetSize() - 1; loop >= 0; loop--)
		{
		delete ((CXrayDoc*)dlg->docarr[loop]);

		// Unlink from chain
		dlg->docarr.RemoveAt(loop);
		}
	
	// Make sure it reflects the 'no document' state
	dlg->current = -1;

	// Commit to screen as well
	dlg->Invalidate();
	dlg->helper->DumpDocs();
}

//////////////////////////////////////////////////////////////////////////

void	CShadesFunc::CloseFileCb(void *ptr)

{

	//dlg->DumpDocs();
	//support.YieldToWinEx();

	// Anything to close?
	if(dlg->current < 0 )
		{
		ShowStatus("Nothing to Close ...");
		return;
		}

	// Stop autoadvance
	dlg->autoadvance = false; 
	dlg->KillTimer(2);

	if(dlg->current >= dlg->docarr.GetSize() )
		{
		ShowStatus("Internal Error: Out of bounds on close");
		return;
		}

	if( ((CXrayDoc*)dlg->docarr[dlg->current])->magic != DOC_MAGIC )
		{
		P2N("Document with bad magic\r\n");
		ShowStatus("Tried to access an unloaded document (bad magic)");
		return;
		}
	
	// Close current document 
	P2N("Closing %s\r\n", ((CXrayDoc*)dlg->docarr[dlg->current])->filename);

	int		old = dlg->current;
	void	*old_ptr = dlg->docarr[dlg->current];

	// Unlink from chain
	dlg->docarr.RemoveAt(old);

	// Switch to another document 
	dlg->current++; 
	if(dlg->current >= dlg->docarr.GetSize())
		{
		dlg->current = 0;
		}

	// Not in chain, switched to other doc, destroy
	delete ((CXrayDoc*)old_ptr);

	// Make sure it reflects the 'no document' state
	if(dlg->docarr.GetSize() == 0)
		dlg->current = -1;

	// Commit to screen as well
	dlg->helper->OnDocSetFocus();
	dlg->Invalidate();

	//dlg->DumpDocs();
}

//////////////////////////////////////////////////////////////////////////
// Right clock close menu

void	CShadesFunc::rCloseFileCb(void *ptr)

{

	CPoint point = dlg->currmouse; dlg->ClientToScreen(&point);
	CXrayM *rmenu = new CXrayM; rmenu->LoadMenu(IDR_MENU5, 1);
	rmenu->Show(point.x, point.y);
}

//////////////////////////////////////////////////////////////////////////

void	CShadesFunc::MBoxCb(void * ptr)

{
	CMsgBox glmb;

	//CMsgBox mb;

	glmb.m_header = "Message Box Primer";
	glmb.m_text = "Hello World.\r\nThis is a new message box. Are you sure you want to temper with it?\r\nssssss\r\n oooo\r\nreoilkjfds\r\ndsl;kjfsdlkfds\r\nqqqqqqqq\r\n";

	//mb.DoModal();

	xMsgBox(glmb.m_text, NULL);
	xMsgBox("Hello is not an option", NULL);
	xMsgBox("This is a long message to test the vertical expansion of the message box.", NULL);
	xMsgBox("message", "This is a long message to test the vertical expansion of the message box.");
}

//////////////////////////////////////////////////////////////////////////

void	CShadesFunc::SaveAsCb(void * ptr)

{

	//MsgBox("Hello Save");

	// Anything to save?
	if(dlg->current < 0 )
		{
		ShowStatus("Nothing to Save ...");
		return;
		}

	CXrayDoc *doc = (CXrayDoc*)((CShadesDlg* )dlg)->docarr[((CShadesDlg* )dlg)->current]; 
	ASSERT(doc->magic == DOC_MAGIC);

	//CFileDialog
	CFileDialogST dlgFile(false);
	dlgFile.m_ofn.lpstrTitle = "Save As";

	int ret = dlg->PromptFileName(doc->filename, dlgFile);

	if(!ret)
		{
		ShowStatus("Cancelled selection ....");
		return;
		}

	CString newname = dlgFile.GetFileName();

	P2N("Save as: %s\r\n", newname);

	dlg->BusyCursor();

	// Save image
	CString str;
	str.Format("Saving Image file '%s'", newname);
	ShowStatus(str);
	
	int	type = FindImageType(doc->filename);
	if(type == 	CXIMAGE_FORMAT_UNKNOWN)
		{
		str.Format("Unknown file type for new image %s", newname);
		xMsgBox(str);
		return;
		}

	if(doc->img->Save(newname, type) == 0)
		{
		CString str;
		str.Format("Cannot Save Image file %s", newname);
		xMsgBox(str);
		return;
		}

	doc->filename = newname;
	
	str.Format("Saved Image file '%s' OK", doc->filename);
	ShowStatus(str);
	
	dlg->BusyCursor(false);
	doc->changed = 0;
}

//////////////////////////////////////////////////////////////////////////

int		CShadesFunc::OpenOneDicom(const char *fname, int uniq)

{

	P2N("OpenOneDicom %d -- name='%s'\r\n", uniq, fname);

	CXrayDoc *doc = NULL;
	int found = false;

	// Find if this is a duplicate
	for(int loop2 = 0; loop2 < dlg->docarr.GetSize(); loop2++)
		{
		if( ((CXrayDoc*)dlg->docarr[loop2])->filename == fname)
			{
			P2N("This file is already loaded '%s'\r\n", fname);
			return -1;
			}
		}
	
	// Find if this is a series
	for(int loop = 0; loop < dlg->docarr.GetSize(); loop++)
		{
		if( ((CXrayDoc*)dlg->docarr[loop])->uniq == uniq)
			{
			// Add to this document 
			found = true;
			doc = (CXrayDoc*)dlg->docarr[loop];
			break;
			}
		}

	DicomFile *dicommf = NULL;
	dicommf	= new DicomFile;

	if(!dicommf->Open(fname))
		{
		P2N("Cannot open DICOM file '%s' %s", fname, support.Err2Str());
		delete dicommf;
		return -2;
		}

	if(!dicommf->buffers)
		{
		P2N("Not a DICOM compatible file or Unsupported DICOM Extension. %s", fname);
		delete dicommf;
		return -3;
		}

	// Not part of an existing doc/frame, create new
	if(!found)
		{
		doc = new CXrayDoc;
		doc->uniq = uniq;
		doc->filename = fname;
		dlg->current =  dlg->docarr.Add(doc);	// Commit doc
		}

	//dlg->lastfilename = fname;

	//int center = doc->dicom->wincenter  + doc->dicom->winwidth / 2;
	//if(center < 1000)
	//	center += 1000;
	//int width = doc->dicom->winwidth;
	//dicomwin.start_base = doc->dicom->wincenter; //center;
	//dicomwin.start_width = doc->dicom->winwidth;
 
	CxImg *frame = new CxImg; ASSERT(frame); 
	CxImg *rframe = new CxImg; ASSERT(rframe); 

	// Commit this frame to doc
	int curr = doc->dcarr.Add(dicommf);
	ASSERT(curr ==  doc->dcarr.GetSize() - 1);  // Assume last added
	
	// Add to shadow/offset array
	doc->dcarr2.Add(dicommf);
	
	//P2N("Got dicom file as index %d\r\n", doc->currframe);
	
	((DicomFile*) doc->dcarr[curr])->ToImage(frame, 
						((DicomFile*) doc->dcarr[curr])->wincenter, 
							((DicomFile*) doc->dcarr[curr])->winwidth);

	doc->imgarr.Add(frame);
	doc->rendarr.Add(rframe);

	// Only Copy / Refresh if it is newly created
	if(!found)
		{
		doc->currframe = 0;
		doc->img->Copy(*frame);
		dicomwin.Show();
		dlg->Invalidate();
		}
	return 1;
}


void	CShadesFunc::OpenDicomFile(void *ptr, int ser, const char *name)

{

	//P2N("OpenDicomFile '%s'\r\n", name);

	P2N("OpenDicomFile flag=%d name=%s\r\n", ser, name);

	int	bOpenFileDialog = true, dialog = false, dwFlags = 0;
	int *pType = NULL;

	CString fname;

	//CFileDialog dlgFile(bOpenFileDialog);
	CFileDialogST dlgFile(bOpenFileDialog);

	CString title;
	
	if (bOpenFileDialog) title="Open Dicom File"; 
	else title="Save Dicom File";

	dlgFile.m_ofn.Flags |= dwFlags | OFN_ALLOWMULTISELECT | OFN_EXPLORER;

	int nDocType = (pType != NULL) ? *pType : 0;
	int nIndex = 0;

	if(name == NULL || name[0] == '\0')
		{
		dialog = true;

		dlgFile.m_ofn.nFilterIndex = 0; //nIndex + 1;
		CString strDefExt = "";
		dlgFile.m_ofn.lpstrDefExt = strDefExt;
			
		CString strFilter;
		
		strFilter += "All Files (*.*)";	strFilter += (TCHAR)NULL;	strFilter += "*.*"; 
		strFilter += (TCHAR)NULL;
		
		strFilter += "DICOM (*.dcm)";	strFilter += (TCHAR)NULL;	strFilter += "*.dcm";
		strFilter += (TCHAR)NULL;
		
		strFilter += (TCHAR)NULL;

		dlgFile.m_ofn.lpstrFilter	= strFilter;
		dlgFile.m_ofn.lpstrTitle	= title;
		dlgFile.m_ofn.lpstrFile		= fname.GetBuffer(1000 *_MAX_PATH);

		//dlg->inm
		BOOL bRet = (dlgFile.DoModal() == IDOK) ? TRUE : FALSE;

		fname.ReleaseBuffer();

		if(!bRet)	
			return;		
		}
	else
		{
		fname = name;
		}

	int cnt = 0;

	// Load it under this grouping ID
	int uniq = CXrayDoc::gl_enum++;

	// Load series
	if(ser)
		{
		// Get extension
		CString drive, dir, filename, ext;
		support.SplitPath(fname, drive, dir, filename, ext);

		// Windows do not care
		//ext.MakeLower();

		// Occupy series dir as current:
		CString tmp; tmp.Format("%s\\%s", drive, dir);
		_chdir(tmp);

		// Padd it to wild card
		CString nnn; nnn = drive + dir + "*" + ext;

		//P2N("Attempted name: '%s'\r\n", nnn);

		struct _finddata_t c_file;
		long hFile;	
		int err = 0;
		
		if((hFile = _findfirst( nnn, &c_file )) == -1L )
			{
			P2N("No such file\r\n");
			MBOX("No file(s) found for this series.");
			return;
			}
		else
			{
			int ret;
			//P2N( "\r\nRDO HID SYS ARC  FILE         DATE %25c SIZE\r\n", ' ' );
			//P2N( "--- --- --- ---  ----         ---- %25c ----\r\n", ' ' );
				
			while (true)
				{		
				if(c_file.name[0] != '.')
					{
					//P2N( " %-12s %.24s  %9ld\r\n",	   c_file.name, ctime( &( c_file.time_write ) ), c_file.size );

					if((ret = OpenOneDicom(c_file.name, uniq)) < 0)
						err++;
					else
						cnt++;
					}

				if(_findnext( hFile, &c_file ) != 0 )
					break;

				}
			_findclose( hFile );

			// Relect loaded state
			dicomwin.SetCurrFrame(0);

			if(cnt > 1)
				{
				CString tmp; tmp.Format("Loaded %d DICOM files.", cnt);
				ShowStatus(tmp);
				}
			if(err > 0)
				{
				CString tmp, tmp2;
				
				if(ret == -1)
					tmp.Format("This DICOM file  is already loaded.");
				else if(ret == -2)
					tmp.Format("Cannot open DICOM file  %s", support.Err2Str());
				else if(ret == -3)
					tmp.Format("Not a DICOM compatible file or Unsupported DICOM Extension.");

				tmp2.Format("Error loading DICOM files, %d files could not be loaded.\r\n"
						"Last error was:  %s", err, tmp);

				MBOX(tmp2);
				}
			}
		} 
	else
		{
		//P2N("Openinig %s", fileName);

		if(dialog)
			{
			POSITION pos = dlgFile.GetStartPosition( );

			while(true)
				{
				if(!pos) 
					break;
				CString str = dlgFile.GetNextPathName(pos);
				
				if(OpenOneDicom(str, uniq) < 0)
					cnt++;
				}

			if(cnt > 1)
				{
				ShowStatusP("Loaded %d DICOM files.", cnt);
				}
			}
		else
			{
			int retd = OpenOneDicom(fname, uniq);

			// Report on error
			if(retd < 0)
				{
				CString name = support.GetFileNameExt(fname);

				if(retd == -1)
					ShowStatusP("This DICOM file '%s' is already loaded.", name);
				else if(retd == -2)
					ShowStatusP("Cannot open DICOM file '%s' - %s", name, support.Err2Str());
				else if(retd == -3)
					ShowStatusP("Not a DICOM compatible file or Unsupported DICOM Extension '%s'.", name);
				else
					ShowStatusP("Cannot load DICOM file '%s'", name);
				}
			}
		}

	dicomwin.m_frame.SetRange(0, cnt - 1);

	// Hack to display the new tickmarks
	dicomwin.m_ok.SetFocus();

    if(fname != "")
		{
		if(ser)
			AfxGetApp()->WriteProfileString(strConfig, "LastSeries", fname);
		else
			AfxGetApp()->WriteProfileString(strConfig, "LastDicom", fname);
		}

	dlg->Invalidate();

	return;
}

void	CShadesFunc::OpenDicomSeries(void *ptr)

{
	//MBOXP("Placeholder for OpenDicomSeries");

	OpenDicomFile(ptr, true);
}




