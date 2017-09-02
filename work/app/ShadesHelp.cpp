
/* =====[ ShadesHelp.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the ShadesHelp.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  11/11/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// ShadesHelp.cpp: implementation of the CShadesHelp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "xraynotes.h"
#include "shades.h"
#include "ShadesHelp.h"
#include "helperdlgs.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#define CONFCHEVRON(win, wname, wantx, wanty, ptrfo, txt)	\
	win.want_x = wantx; win.want_y = wanty;			\
	win.want_w = -1; win.want_h = -1;				\
	win.windowname = wname;							\
    win.fo = ptrfo;									\
    win.Show();										\
    win.m_prompt.SetText(txt);						\
    win.m_prompt.SetTrans();						\
    support.YieldToWinEx();							\


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShadesHelp::CShadesHelp()

{
	//P2N("CShadesHelp::CShadesHelp  Constructed helper\r\n");
	dlg = NULL;
	layprog = 0;
}

CShadesHelp::CShadesHelp(void *ptr)

{
	//P2N("CShadesHelp::CShadesHelp  Constructed helper\r\n");
	dlg = (CShadesDlg *)ptr;
	layprog = 0;
}

CShadesHelp::~CShadesHelp()

{

}

void CShadesHelp::DumpDocs()

{
    int ndoc = dlg->docarr.GetSize();

    for(int loop = 0; loop < ndoc; loop++)
        {
        CString str;

        ASSERT(((CXrayDoc*)dlg->docarr[loop])->magic == DOC_MAGIC);

        if(loop == dlg->current)
            str = "Current";
        else
            str = "";

        P2N("Doc %s -- %s %d\r\n", str, 
					((CXrayDoc*)dlg->docarr[loop])->filename, 
						((CXrayDoc*)dlg->docarr[loop])->magic == DOC_MAGIC);
        }

	if(ndoc == 0)
		P2N("No documents\r\n");
}


void CShadesHelp::PrevDoc()

{
    int oldcurr = dlg->current;

    dlg->current--;
    if(dlg->current < 0)
        {
        // Exception -> detects absence of document automatically
        dlg->current =  dlg->docarr.GetSize() - 1;
        }

    // Doc changed
    if(dlg->current != oldcurr)
        {
		dlg->SelDoc(dlg->current);

        if(!dlg->autoadvance)
            {
            ShowStatusP("Switched backward to: '%s'", 
					support.GetFileNameExt(((CXrayDoc*)dlg->docarr[dlg->current])->filename));
            }
        }
}

//////////////////////////////////////////////////////////////////////////

void CShadesHelp::NextDoc()

{
	int oldcurr = dlg->current;

    // Exception -> no document
    if(dlg->docarr.GetSize() == 0)
        {
        dlg->current = -1;
        return;
        }

    int curr = dlg->current + 1;

    if(curr >= dlg->docarr.GetSize())
        {
        curr = 0;
        }

    if(!dlg->docarr[curr])
        {
        P2N("Attempted to switch to closed doc\r\n");
        return;
        }

    if( ((CXrayDoc*)dlg->docarr[curr])->magic != DOC_MAGIC )
        {
        P2N("Bad Magic on NextDoc\r\n");
        return;
        }

    // Doc Changed
    if(dlg->current != curr)
        {
      	dlg->SelDoc(curr);
        
		HideStatus();

		//dlg->SelDoc(dlg->current);

        if(!dlg->autoadvance)
            {
            ShowStatusP("Switched forward to: '%s'", 
					support.GetFileNameExt(((CXrayDoc*)dlg->docarr[curr])->filename));
            }
        }
}

void CShadesHelp::OpenLastDicomFile()

{
	CString fname;
	fname = AfxGetApp()->GetProfileString(strConfig, "LastDicom");
	
	if(fname != "")
		{
		dlg->functs->OpenDicomFile(this, false, fname);
		}	
}

void CShadesHelp::OpenLastDicomSeries()

{
	CString fname;
	fname = AfxGetApp()->GetProfileString(strConfig, "LastSeries");
	
	if(fname != "")
		{
		dlg->functs->OpenDicomFile(this, true, fname);
		}	
}

void CShadesHelp::OpenLastSession()

{
	for(int loop4 = 0; loop4 < MAXDOCS; loop4++)
        {
		CString tmp; tmp.Format("LastFile%d", loop4);
		CString fname = AfxGetApp()->GetProfileString(strConfig, tmp, "");

        //P2N("Last session file '%s'\r\n", fname);

		if(fname == "")
			break;

		dlg->functs->OpenImageFile(fname);
        }
}

void CShadesHelp::OpenLastFile()

{
	CString lastfname = AfxGetApp()->GetProfileString(strConfig, "LastImageName", "");

	if(lastfname == "")
		{
		xMsgBox("No file has been opened yet.");
		return;
		}
    dlg->functs->OpenImageFile(lastfname);
}

void CShadesHelp::InitChevrons()

{
	nw.tiptext =	"The XrayNotes NorthWest Chevron.\r\n"
					"Hovering over this chevron opens the flyout window.\r\n"
					"One can also force flyout Open/Close by clicking on the chevron itself.\r\n"
					"Hotkey to open this (NW) flyout ALT-1 (Alt-Number-One)";

	sw.tiptext =	"The XrayNotes SouthWest Chevron.\r\n"
					"Hovering over this chevron opens the flyout window.\r\n"
					"One can also force flyout Open/Close by clicking on the chevron itself.\r\n"
					"Hotkey to open this (NW) flyout ALT-3 (Alt-Number-Three)";
	
	ne.tiptext =	"The XrayNotes NorthEast Chevron.\r\n"
					"Hovering over this chevron opens the flyout window.\r\n"
					"One can also force flyout Open/Close by clicking on the chevron itself.\r\n"
					"Hotkey to open this (NE) flyout ALT-2 (Alt-Number-Two)";
	
	se.tiptext =	"The XrayNotes SouthEast Chevron.\r\n"
					"Hovering over this chevron opens the flyout window.\r\n"
					"One can also force flyout Open/Close by clicking on the chevron itself.\r\n"
					"Hotkey to open this (SE) flyout ALT-4 (Alt-Number-Four)";


	// NW ================================================================

    /*nwfo.arr_l[0].func = OpenFileCb;
    nwfo.arr_l[1].func = ScanDocCb;
    nwfo.arr_l[2].func = CloseFileCb;
    nwfo.arr_l[3].func = SaveFileCb;
    nwfo.arr_l[4].func = SaveAsCb;
    nwfo.arr_l[5].func = PrintCb;
    nwfo.arr_l[6].func = CopyCb;
    nwfo.arr_l[7].func = CutCb;
    nwfo.arr_l[8].func = PasteCb;
    nwfo.arr_l[9].func = TestCb;*/

    nwfo.arr_l[0].str = "Open file via the standard 'Open File Dialog'.\r\n"
						"Use 'Open Patient' function on the top right toolbar [NW] for the data selection window.\r\n"
						"The default action is executed when pressing Space or Enter\r\n"
						"Right Click to see more menu options.\r\n"
						"The menu also opens up by pressing Ctrl-Enter of Shift-Enter\r\n"
						"Shortcut Key for File Open: 'Ctrl-O'";

    nwfo.arr_l[1].str = "Scan New Document into XrayNotes\r\n"
						"Document is scanned from the scanner interface, that can be selected\r\n"
						" from the right click menu.\r\n"
						"Shortcut Key for Scanning: 'Ctrl-??'";

    nwfo.arr_l[2].str = "Close Current Document.\r\n"
						"Right Click to see more close menu options.\r\n"
						"Hold Shift to close ALL Documents.\r\n"
						"Shortcut Key: Ctrl-F4, Ctrl-E";

    nwfo.arr_l[3].str = "Save file.\r\n"
						"Right Click to see more menu options.\r\n"
						"Shortcut key: Ctrl-S";

    nwfo.arr_l[4].str = "Save As - Save file under a new name or new location"
						"Right Click to see more close menu options.\r\n"
						"Shortcut key: Ctrl-??";

    nwfo.arr_l[5].str = "Print Current Document.\r\n"
						"Document is printed to the current (or default) printer\r\n"
						"Right Click to see more menu options.\r\n"		
						"Shortcut key: Ctrl-P";

    nwfo.arr_l[6].str = "Copy Current Selection to Clipboard.\r\n"
						"Right Click to see more menu options.\r\n"		
						"Shortcut key: 'Ctrl-C'";

    nwfo.arr_l[7].str = "Cut Current Selection to Clipboard. \r\n"
						"Right Click to see more menu options.\r\n"		
						"Shortcut key: 'Ctrl-X'";

    nwfo.arr_l[8].str = "Paste clipboard contents to current document.\r\n"
						"Right Click to see more menu options.\r\n"		
						"Shortcut key: 'Ctrl-V'";

    nwfo.arr_l[9].str = "Exit XrayNotes Application.\r\n"
						"Shortcut keys: 'Alt-F4' for standard close,\r\n"
                        "               'Alt-X'  for immidiate close with auto save option 'yes'. ";
	
/*	nwfo.arr_r[0].func = rOpenFileCb;
	nwfo.arr_r[2].func = rCloseFileCb;*/
   
    // SW ================================================================

  /*  swfo.arr_l[0].func = CropCb;
    swfo.arr_r[0].func = rCropCb;
    swfo.arr_l[1].func = NormCb;
    swfo.arr_l[2].func = BrightCb;
    swfo.arr_l[3].func = ColorCb;
    swfo.arr_l[4].func = MagCb;
    swfo.arr_l[5].func = ProcCb;
    swfo.arr_l[6].func = UndoCb;
    swfo.arr_l[7].func = RedoCb;
    swfo.arr_l[8].func = SelectCb;
    swfo.arr_l[9].func = MarkCb;

	swfo.arr_r[5].func = rProcCb;*/

	swfo.arr_l[0].str = "Start the cropping process.\r\n"
						"In this process, one marks a section if the image, and then press the 'Crop' button to\r\n"
						"Crop the image to the selection. The image will be converted to standard "
						"XRAYNOTES resolution (1280xNNNN)\r\n"
						"where the 'NNNN' will be calulated to keep the image's aspect ratio."
						"Shortcut Key: CTRL-  (Mnemonic: _X_)";

	swfo.arr_l[1].str = "Open the normalization dialog.\r\n"
						"In this dialog, one can adjust the image's histogram to follow a graphical representation of the curve\r\n"
						"Shortcut Key: CTRL-  (Mnemonic: _X_)";


	swfo.arr_l[2].str = "Open the Brightness / Contrast adjustment dialog.\r\n"
						"In this dialog, one can adjust the image's brightness and contrast components separately.\r\n"
						"Shortcut Key: CTRL-B  (Mnemonic: _B_right)";

	swfo.arr_l[3].str = "Open the color adjustment dialog.\r\n"
						"In this diaog, one can adjust the image's RGB (Red/Green/Blue) components separately.\r\n"
						"Shortcut Key: CTRL-R  (Mnemonic: colo_R_ize)";


	swfo.arr_l[4].str = "Open the MAG (magnification) dialog window.\r\n"
						"In this dialog, one can view the undelying document with magnification.\r\n"
						"When viewing a DICOM file, the MAG window can show 3D slice details.\r\n"
						"Shortcut Key: CTRL-ALT-M   (Mnemonic: Magnification)";

	swfo.arr_l[5].str = "Open the image processing dialog.\r\n "
						"In this dialog, one can adjust various image properties."
						"Adjust sharpness, (DSP) Gamma, (Histogram) Pinch / Punch (NonLinear)\r\n"
						"Shortcut Key: CTRL-D   (Mnemonic: _D_igital Signal Processing (DSP))";

	swfo.arr_l[6].str = "Undo the last operation.\r\n"
						"With this processing one can undo the latest operation. Only commited operations \r\n"
						"can be undone, as the uncommited operations are still in memory ans can be undone by \r\n"
						"cancelling the active processing dialog\r\n"
						"Shortcut Key: CTRL-U   (Mnemonic: _U_ndo";

	swfo.arr_l[7].str = "Redo last Undo operation.\r\n"
						"With this processing one can reverse the effect undos. Only commited operations can be undone,\r\n"
						"Shortcut Key: CTRL-R   (Mnemonic: _R_edo";

	swfo.arr_l[8].str = "Start Select operation.\r\n"
						"With this facility one can select a rectangular area of the image.\r\n"
						"Most opeartions operate on the selection, and if nothing is selected,\r\n"
						"opeations are effective on the whole image\r\n"
						"Shortcut Key: CTRL-?   (Mnemonic: __";

	swfo.arr_l[9].str = "Initiate Mark  operation.\r\n"
						"With this facility one can select a irregular (arbitrary) shaped area.\r\n"
						"Most opeartions are effective on the selection, and if nothing is selected,\r\n"
						"operations are effective on the whole image\r\n"
						"Shortcut Key: CTRL-?   (Mnemonic: __";


    // NE ================================================================

   /* nefo.arr_l[0].func = ListDocsCb;
    nefo.arr_l[1].func = DocPropCb;
    nefo.arr_l[2].func = ToggleUICb;
    nefo.arr_l[3].func = ConfCb;
    nefo.arr_l[4].func = OptCb;
    nefo.arr_l[5].func = AutoCb;
    nefo.arr_l[6].func = PatCb;
    nefo.arr_l[7].func = cNewCb;
    nefo.arr_l[8].func = NewCb;
    nefo.arr_l[9].func = ExitCb;*/

    nefo.arr_l[0].str = "Show a list of loaded documents. Shortcut key: Ctrl-Tab";
    nefo.arr_l[1].str = "Show Document Properties";
    nefo.arr_l[2].str = "Toggle visibility of standard tool windows.\r\n"
                        "Horizontal Scroll, Vertical Scroll, Magnify Window, Gesture Window\r\n"
                            "Shortcut key: Alt-Z";
    nefo.arr_l[3].str = "Show Configuration Window";
    nefo.arr_l[4].str = "Show Options Window";
    nefo.arr_l[5].str = "Auto Process current image and/or document. Document is cropped to current selection,\r\n"
                        "resampled to HDTV resolution, and placed on a standard (8.5 x 11) printable page.";

    nefo.arr_l[6].str = "Open Patient selection dialog. Shortcut key: Ctrl-T";
    nefo.arr_l[7].str = "Create new document or image from clipboard. Shortcut key: Ctrl-??";
    nefo.arr_l[8].str = "Create new document. Shortcut key: Ctrl-??";
    nefo.arr_l[9].str = "Exit XrayNotes Application. Shortcut keys: 'Alt-F4' for standard close,\r\n"
                        "'Alt-X' for immidiate close ";

    // SE ================================================================

    /*sefo.arr_l[0].func = LineCb;
    sefo.arr_l[1].func = DrawCb;
    sefo.arr_l[2].func = TextCb;
    sefo.arr_l[3].func = RectCb;
    sefo.arr_l[4].func = EllipCb;
    sefo.arr_l[5].func = ImageCb;
    sefo.arr_l[6].func = SVGCb;
    sefo.arr_l[7].func = MagDocCb;
    sefo.arr_l[8].func = MeasCb;
    sefo.arr_l[9].func = IdleCb;*/

    sefo.arr_l[0].str = "Start Line Drawing. Move mouse to beginning of the desired line, press and hold the left\r\n"
                        "mouse button, and start drawing. Line will be drawn until mouse button is released.\r\n"
						"Hold the Shift key for multiple line drawings.\r\n"
						"The line object is created at the current mouse position\r\n"
						"Shortcut key to create a new line: Ctrl-L";

    sefo.arr_l[1].str = "Start Freehand Drawing.";
    sefo.arr_l[2].str = "Add a Text Object to the Document.\r\n"
						"Text can be surrounded by a Rectangular Pad (like the Sticky Note)\r\n"
						"or a Circular Pad (to match the style of anatomical features)\r\n"
						"\r\nHold the Shift Key to add multiple TExt Objects. (Esc to stop)\r\n"
						"The text object is placed at the current mouse position\r\n"
						"Shortcut key to create an new Text Object Alt-T";

    sefo.arr_l[3].str = "Add a Rectangle to document.\r\n"
						"Hold the Shift Key to add multiple Rectangles. (Esc to stop)\r\n"
						"Shortcut key: ";

    sefo.arr_l[4].str = "Add Ellipse or Circle to document";


    sefo.arr_l[5].str = "Add Image to document";


    sefo.arr_l[6].str = "Add Scalable graphics to document";


    sefo.arr_l[7].str = "Add Magnifier Object to document";


    sefo.arr_l[8].str = "Add Measurement control to document";


    sefo.arr_l[9].str = "Set Drawing State to Idle.\r\n"
						"This will reset the drawing state to non drawing condition.\r\n"
                        "the 'Selection' state, with no tools or controls active. Shortcut key: Esc";

}

void CShadesHelp::TmpClosePanels()

{
    // Temporary close tool panels
    if(yscroll.IsWindowVisible())
        {
        yscroll.push = true;       yscroll.Hide();
        }
    if(xscroll.IsWindowVisible())
        {
        xscroll.push = true;        xscroll.Hide();
        }
    if(gesdlg.IsWindowVisible())
        {
        gesdlg.push = true;         gesdlg.Hide();
        }
    if(magdlg.IsWindowVisible())
        {
        magdlg.push = true;          magdlg.Hide();
        }
    if(ne.IsWindowVisible())
        {
        ne.push = true;              ne.Hide();
        }
    if(nw.IsWindowVisible())
        {
        nw.push = true;              nw.Hide();
        }
    if(se.IsWindowVisible())
        {
        se.push = true;              se.Hide();
        }
    if(sw.IsWindowVisible())
        {
        sw.push = true;              sw.Hide();
        }

    dlg->SetFocus();
}


void CShadesHelp::TmpRestorePanels()

{
    // Temporary unclose
    if(yscroll.push)
        {
        yscroll.push = false; yscroll.Show();
        }
    if(xscroll.push)
        {
        xscroll.push = false;  xscroll.Show();
        }
    if(gesdlg.push)
        {
        gesdlg.push = false;   gesdlg.Show();
        }
    if(magdlg.push)
        {
        magdlg.push = false;   magdlg.Show();
        }
    if(ne.push)
        {
        ne.push = false;       ne.Show();
        }
    if(nw.push)
        {
        nw.push = false;       nw.Show();
        }
    if(se.push)
        {
        se.push = false;       se.Show();
        }
    if(sw.push)
        {
        sw.push = false;       sw.Show();
        }

    dlg->SetFocus();
}


void CShadesHelp::OnDocSwitch()

{
	P2N("Document about to be switched to %d\r\n", dlg->current);	
}

void CShadesHelp::OnDocSetFocus()

{
	P2N("Document switched to %d\r\n", dlg->current);

	CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);

	// Send it to all helper dailogs
	OnDocChange();

	CXrayDoc* doc = dlg->GetCurrDoc();

	if(doc)
		{
		// Hide show DICOM panel
		if(doc->IsDicomFile())
			dicomwin.Show();
		else
			dicomwin.Hide();

		if(doc->backlink)
			xyzdlg.Show();
		else
			xyzdlg.Hide();

		}


}

void	CShadesHelp::MoveDicomZ(int flag)

{
	P2N("CShadesHelp::MoveDicomZ(int flag=%d)\r\n", flag);
}

//////////////////////////////////////////////////////////////////////////
// Broadcast it to all pads.

void CShadesHelp::OnDocChange()

{
	CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);

	// Send it to all client dailogs

	PostMessage(dlg->m_hWnd, WM_DOCCHANGE, 0, 0);
	
	PostMessage(magdlg.m_hWnd, WM_DOCCHANGE, 0, 0);
	PostMessage(gesdlg.m_hWnd, WM_DOCCHANGE, 0, 0);
	PostMessage(procdlg.m_hWnd, WM_DOCCHANGE, 0, 0);
	PostMessage(bcondlg.m_hWnd, WM_DOCCHANGE, 0, 0);
	PostMessage(coldlg.m_hWnd , WM_DOCCHANGE, 0, 0);
	PostMessage(dicomwin.m_hWnd, WM_DOCCHANGE, 0, 0);
	PostMessage(pydlg.m_hWnd    , WM_DOCCHANGE, 0, 0);
	
	//PostMessage(dbmain.m_hWnd   , WM_DOCCHANGE, 0, 0);

	PostMessage(magwin.m_hWnd   , WM_DOCCHANGE, 0, 0);
}

//////////////////////////////////////////////////////////////////////////

void	CShadesHelp::HandleMark(UINT nChar, UINT nRepCnt, UINT nFlags)

{
    int  changed = false;

	CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);

    RECT rc; dlg->GetClientRect(&rc);

    //P2N("Tool mark state\r\n");

    if(nChar == VK_LEFT)
        {
        if(GetKeyState(VK_CONTROL) & 0x80)
            {
            dlg->handlesel++;
            if(dlg->handlesel == dlg->xmarkarr.GetUpperBound())
                dlg->handlesel = 0;
            }
        else
            {
            dlg->xmarkarr[dlg->handlesel]--;
            }
        changed = true;
        }
    if(nChar == VK_RIGHT)
        {
        if(GetKeyState(VK_CONTROL) & 0x80)
            {
            dlg->handlesel--;
            if(dlg->handlesel < 0)
                dlg->handlesel = dlg->xmarkarr.GetUpperBound();
            }
        else
            {
            dlg->xmarkarr[dlg->handlesel]++;
            }

        changed = true;
        }

    if(nChar == VK_UP)
        {
        dlg->ymarkarr[dlg->handlesel]--;
        changed = true;
        }
    if(nChar == VK_DOWN)
        {
        dlg->ymarkarr[dlg->handlesel]++;
        changed = true;
        }
    if(nChar == VK_DELETE)
        {
        //P2N("XmarkArr del.\r\n");
        if(dlg->handlesel < dlg->xmarkarr.GetSize() && dlg->handlesel >= 0)
            {
            dlg->xmarkarr.RemoveAt(dlg->handlesel);
            dlg->ymarkarr.RemoveAt(dlg->handlesel);
            }
        changed = true;
        }

    if(nChar == VK_INSERT)
        {
        // Is valid index?
        if(dlg->handlesel < (dlg->xmarkarr.GetSize()) && dlg->handlesel >= 0)
            {
            int offset = 1;

            // Wrap around
            if(dlg->handlesel == dlg->xmarkarr.GetUpperBound())
                offset = -dlg->handlesel;

            DWORD xxx = dlg->xmarkarr[dlg->handlesel] + 
				((int)dlg->xmarkarr[dlg->handlesel + offset] - 
					(int)dlg->xmarkarr[dlg->handlesel]) / 2;

            DWORD yyy = dlg->ymarkarr[dlg->handlesel] + 
				((int)dlg->ymarkarr[dlg->handlesel + offset] - 
					(int)dlg->ymarkarr[dlg->handlesel]) / 2;

            dlg->xmarkarr.InsertAt(dlg->handlesel + offset, xxx);
            dlg->ymarkarr.InsertAt(dlg->handlesel + offset, yyy);

            //P2N("XmarkArr insert %d %d\r\n", xxx, yyy);
            changed = true;
            }
        }
    if(changed)
        dlg->InvalidateRect(&rc, false);
}

void	CShadesHelp::GetFirstLayer()

{
	layprog = 0;
}

void	CShadesHelp::GetLastLayer()

{

	CXrayDoc* doc = dlg->GetCurrDoc();
	if(doc)
		layprog = doc->m_layers.GetSize() - 1;
}


CLayer*	CShadesHelp::GetNextLayer()

{
	CLayer *ylay = NULL;
	CXrayDoc* doc = dlg->GetCurrDoc();
	
	if(doc)
		if(layprog < doc->m_layers.GetSize())
			ylay = (CLayer*)doc->m_layers[layprog];

	layprog++;

	return(ylay);
}

CLayer*	CShadesHelp::GetPrevLayer()

{
	CLayer *ylay = NULL;
	CXrayDoc* doc = dlg->GetCurrDoc();

	if(doc)
		if(layprog >= 0)
			ylay = (CLayer*)doc->m_layers[layprog];

	layprog--;

	return(ylay);
}

/// Get number of selected objects

int	CShadesHelp::GetNumSelObj()

{
	int ret = 0;

	CLayer *ylay = NULL;
	CXrayDoc* doc = dlg->GetCurrDoc();
	
	if(doc)
		for(int loop = 0; loop < doc->m_layers.GetSize(); loop++)
			{
			if( ((CLayer*)doc->m_layers[loop])->focus)
				ret++;
			}

	return ret;
}

void CShadesHelp::ClearAllObjFocus()
{
	CXrayDoc* doc = dlg->GetCurrDoc();

	// Clear all focuses
	int lsize = doc->m_layers.GetSize();
	for(int looplay2 = 0; looplay2 < lsize; looplay2++)
		((CLayer*)doc->m_layers[looplay2])->SetFocus(false);

	dlg->Invalidate(false);
}


//////////////////////////////////////////////////////////////////////////

char *CShadesHelp::state2str(int state)

{
	char *ret;

	switch(state)
		{
		case STATE_WIDTH : ret =  "STATE_WIDTH";	
		case STATE_HEIGHT: ret =  "STATE_HEIGHT";
		case STATE_FULL	 : ret =  "STATE_FULL";
		case STATE_VARI	 : ret =  "STATE_VARI";
		case STATE_FINE	 : ret =  "STATE_FINE";
		case STATE_MUL	 : ret =  "STATE_MUL";
		}
	return ret;
}

//////////////////////////////////////////////////////////////////////////

void CShadesHelp::InitChevPos(int xx, int yy) 

{
	RECT rc; dlg->GetClientRect(&rc);
   
	CONFCHEVRON(nw, "nw", 10 + xx, 10 + yy, &nwfo, ">>");
	CONFCHEVRON(sw, "sw", 10 + xx, rc.bottom - 100 + yy, &swfo, ">>");
	CONFCHEVRON(ne, "ne", rc.right - 40 + xx, 10 + yy, &nefo, "<<");
	CONFCHEVRON(se, "se", rc.right - 40 + xx, rc.bottom - 100 + yy, &sefo, "<<");

	nw.MoveToPos(); sw.MoveToPos(); ne.MoveToPos(); se.MoveToPos();
}
