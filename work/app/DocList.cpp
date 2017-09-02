
/* =====[ DocList.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the DocList.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  7/31/2007  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// DocList.cpp : implementation file
//

#include "stdafx.h"

#include <direct.h>

#include "xraynotes.h"
#include "DocList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDocList dialog


CDocList::CDocList(CWnd* pParent /*=NULL*/)
	: CDialog(CDocList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDocList)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDocList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDocList)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDocList, CDialog)
	//{{AFX_MSG_MAP(CDocList)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_CLOSE()
	ON_WM_SHOWWINDOW()
	ON_WM_ACTIVATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


#if 0
	// Attempt on drawing message handlers

	CxImage alpimg;
	CString alp(appdir);

	P2N("Get CWD=%s approot=%s\r\n", alp);

	alp += "\\alpimg.jpg";
	alpimg.Load(alp); 

	if(!alpimg.IsValid())
		{
		MBOX("Invalid alpha img");
		}

	alpimg.GrayScale();
	

	RECT rc;
	rc.left = disx; rc.top = disy;  
	rc.right = rc.left + 200; rc.bottom = rc.top + 200;

	alpimg.Resample(ddd->sel->GetWidth(), ddd->sel->GetHeight());


	//  10,10 ----------------- 200,10
	//		  |               |
	//        |               | 
	//        |               | 
	//        |               |
	// 200,10 ----------------- 200,200

	POINT points[3] = {0,0, 0,100, 150,20};
	ddd->sel->SelectionAddPolygon(points, 3);

	POINT points2[3] = {0,200, 200, 200, 150, 100};
	ddd->sel->SelectionAddPolygon(points2, 3);

	RECT rc2;
	rc2.left = 10; rc2.top = 10;  
	rc2.right = rc2.left + 20; rc2.bottom = rc2.top + 20;

#endif


/////////////////////////////////////////////////////////////////////////////
// CDocList message handlers

void CDocList::OnPaint() 

{
	CPaintDC dc(this); // device context for painting

	//P2N("Drawing to list dialog %s\r\n", ddd->filename);

	int loop, disx = dlg->docarr.GetSize() * 100, disy = 0;

	//CXrayDoc *doc = (CXrayDoc*)((CShadesDlg* )dlg)->docarr[((CShadesDlg* )dlg)->current]; 
	//ASSERT(doc->magic == DOC_MAGIC );

	//P2N("ListDocsCb\r\n");

	for(loop = 0; loop < dlg->docarr.GetSize(); loop++)
		{
		CXrayDoc *ddd =  ((CXrayDoc*)dlg->docarr[loop]);
		ASSERT(ddd->magic == DOC_MAGIC );

		// Make sure it is not a new one since dlg opened
		if(ddd->sel->IsValid())
			{
			ddd->sel->DrawX(dc.GetSafeHdc(),  disx, disy, -1, -1,								
						NULL, false, 0, 0);
			
			disx -= 100;
			disy += 20;
			}
		}

	dc.Draw3dRect(m_hitrec, RGB(255,255,255), RGB(128,128,128));

	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CDocList::OnEraseBkgnd(CDC* pDC) 

{
	//P2N("Got erase background on DocList\r\n");

#if 0
	// Get image from the Desktop
	CBitmap m_bitmap, m_bitmap2;

	HDC  hdc = ::GetDC(dlg->m_hWnd);
	CDC	*screen; 
	
	screen = CDC::FromHandle(hdc);

	CDC  dcMem;
	dcMem.CreateCompatibleDC(pDC);
	RECT rc; GetClientRect(&rc);

	m_bitmap.CreateCompatibleBitmap(screen, rc.right, rc.bottom);
	CBitmap *pOldBitmap  =  dcMem.SelectObject(&m_bitmap);
	
	RECT rcw;
	GetWindowRect(&rcw);
	dcMem.FillSolidRect(&rc, RGB(255, 66, 55));

	pDC->BitBlt(0, 0, rc.right, rc.bottom, screen, rcw.left, rcw.top,  SRCCOPY );
	//pDC->BitBlt(0,0, rc.right, rc.bottom, &dcMem, 0, 0, SRCCOPY );	
	//screen->BitBlt(0,0, rc.right, rc.bottom, &dcMem, 0, 0, SRCCOPY );	
	
	dcMem.SelectObject(pOldBitmap);

	return true;

#endif

	BOOL bRetValue = CDialog::OnEraseBkgnd(pDC);

	if (!m_hBitmap)
        return bRetValue;

	CRect   rect;
    GetClientRect(rect);
    
    CDC dc;  dc.CreateCompatibleDC(pDC);

    HBITMAP    pbmpOldBmp = NULL;

    pbmpOldBmp = (HBITMAP)::SelectObject(dc.m_hDC, m_hBitmap);
    
	int ixOrg, iyOrg;

	for (iyOrg = 0; iyOrg < rect.Height(); iyOrg += m_dwHeight)
        {
            for (ixOrg = 0; ixOrg < rect.Width(); ixOrg += m_dwWidth)
            {
                pDC->BitBlt (ixOrg, iyOrg, rect.Width(), rect.Height(), &dc, 0, 0, SRCCOPY);
            }
        }
	 
    ::SelectObject(dc.m_hDC, m_hBitmap);

    return bRetValue;
}


BOOL CDocList::OnInitDialog() 

{
	CDialog::OnInitDialog();
	
	BYTE	bAlpha = 255;

	//if (bAlpha < 255)
		{
        //  Set layered style for the dialog
        SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);

        //  Call it with 255 as alpha - opacity
		if(g_pSetLayeredWindowAttributes)
			{
			//g_pSetLayeredWindowAttributes(m_hWnd, 0, bAlpha, LWA_ALPHA);
			g_pSetLayeredWindowAttributes(m_hWnd, RGB(0, 255, 0), 0, LWA_COLORKEY);
			}
		}

	// Load bmp for transparent background
    HINSTANCE  hInstResource = NULL;
    
    // Find correct resource handle
    hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(IDB_BITMAP21), RT_BITMAP);
    
    // Load bitmap In
    m_hBitmap = (HBITMAP)::LoadImage(hInstResource, MAKEINTRESOURCE(IDB_BITMAP21),
                                   IMAGE_BITMAP, 0, 0, 0);        
    // Get bitmap size
	BITMAP  csBitmapSize;
  
    int nRetValue = ::GetObject(m_hBitmap, sizeof(csBitmapSize), &csBitmapSize);
    ASSERT(nRetValue);

	m_dwWidth = (DWORD)csBitmapSize.bmWidth;
	m_dwHeight = (DWORD)csBitmapSize.bmHeight;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDocList::OnClose() 

{
	P2N("CDocList::Close Window \r\n");	
	
	CDialog::OnClose();
}

//////////////////////////////////////////////////////////////////////////

void CDocList::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	//P2N("CDocList::Show Window %d\r\n", bShow);	

	dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);

	dlg->moire = true; //nState;
	dlg->Invalidate();
	support.YieldToWinEx();

	// Pre load all
	for(int loop = 0; loop < dlg->docarr.GetSize(); loop++)
		{
		CXrayDoc *ddd =  ((CXrayDoc*)dlg->docarr[loop]);
		ASSERT(ddd->magic == DOC_MAGIC );

		//P2N("Document %s\r\n", ddd->filename);

		if(!ddd->sel->IsValid())
			{
			//P2N("Scanning %s\r\n", ddd->filename);

			if(ddd->IsDicomFile())
				((CxImg*)ddd->imgarr[ddd->currframe])->Resample(180, 180,  0,  ddd->sel);
			else
				ddd->render->Resample(180, 180,  0,  ddd->sel);

			ddd->sel->CovertBlack();				// Alias pure black ----

			RGBQUAD fill = {0, 0, 0, 0};			// Black
			RGBQUAD fill2 = {80, 80, 80, 80};		// Mid Gray

			ddd->sel->Expand(7, 7, 7, 7, fill2 );
			ddd->sel->Expand(0, 50, 0, 50, fill );
			
			ddd->sel->Skew((float)0.0, (float)0.5, 100, 100);

			ddd->sel->SetTransTresh(255);
			ddd->sel->SetTransIndex (0);
			}	
		}
	
	RECT rc; GetClientRect(&rc);

	// Desktop
	HDC		hdc		=  ::GetDC(HWND_DESKTOP);
    int     xsize	=  ::GetDeviceCaps(hdc, HORZRES);
    int     ysize	=  ::GetDeviceCaps(hdc, VERTRES);

	int want_w = dlg->docarr.GetSize() * 100 + 180 + 80 + 7;
	int want_h = dlg->docarr.GetSize() * 20 + 180 + 100;

	// Resize and Center to desktop
	int want_x = xsize / 2 - want_w / 2;
	int	want_y = ysize / 2 - want_h / 2;

	SetWindowPos( NULL, want_x, want_y, want_w, want_h, SWP_NOZORDER );  	
}


void CDocList::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 

{
	CDialog::OnActivate(nState, pWndOther, bMinimized);
	
	//P2N("ACTIVATE Window nState=%d\r\n", nState);	

}

void CDocList::OnLButtonDown(UINT nFlags, CPoint point) 

{
	int selected = false;
	CRect hitrec;

	int loop;
	int want_w = dlg->docarr.GetSize() * 100 + 180 + 20;
	int want_h = 0; // = dlg->docarr.GetSize() * 20 + 180 + 100;

	// Calculate array position
	for(loop = 0; loop < dlg->docarr.GetSize(); loop++)
		{
		hitrec.SetRect(want_w - 100, want_h + 100, want_w, want_h + 300);

		//P2N("Hit test: top=%d left=%d right=%d bottom=%d \r\n", 
		//	hitrec.top, hitrec.left, hitrec.right, hitrec.bottom);

		if(hitrec.PtInRect(point))
			{
			CXrayDoc *ddd =  ((CXrayDoc*)dlg->docarr[loop]);
			ASSERT(ddd->magic == DOC_MAGIC );

			//P2N("Hit in Document %s\r\n", ddd->filename);

			//m_hitrec = hitrec;
			//Invalidate();

			// Select this document
			dlg->SelDoc(loop);	
			OnOK();
			selected = true;
			break;
			}
		want_w -= 100; 
		want_h += 20;
		}

	if(!selected)
		PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));	
	
	CDialog::OnLButtonDown(nFlags, point);
}

void CDocList::OnDestroy() 
{
	CDialog::OnDestroy();
	
	P2N("CDocList::Destroy\r\n");	
}

int CDocList::DoModal() 

{
	P2N("Domodal start\r\n");

	((CShadesApp*)AfxGetApp())->inmodal++;
	int ret  = CDialog::DoModal();
	((CShadesApp*)AfxGetApp())->inmodal--;

	P2N("Domodal end\r\n");

	return ret;
}

void CDocList::OnCancel()
{
	P2N("CDocList::OnCancel\r\n");

	Closing(true);
	CDialog::OnCancel();
}

void CDocList::OnOK()

{
	P2N("CDocList::OnOK\r\n");

	Closing(true);
	CDialog::OnOK();
}

void CDocList::Closing(int reason)

{
	dlg->moire = false;
	dlg->Invalidate();
	support.YieldToWinEx();
}

void CDocList::OnLButtonDblClk(UINT nFlags, CPoint point) 

{
	P2N("CDocList::Double click x=%d y=%d\r\n", point.x, point.y);

	CRect hitrec;

	int loop;
	int want_w = dlg->docarr.GetSize() * 100 + 180 + 20;
	int want_h = 0; // = dlg->docarr.GetSize() * 20 + 180 + 100;

	// Calculate array position
	for(loop = 0; loop < dlg->docarr.GetSize(); loop++)
		{
		hitrec.SetRect(want_w - 100, want_h, want_w, want_h + 300);

		//P2N("Hit test: top=%d left=%d right=%d bottom=%d \r\n", 
		//	hitrec.top, hitrec.left, hitrec.right, hitrec.bottom);

		if(hitrec.PtInRect(point))
			{
			CXrayDoc *ddd =  ((CXrayDoc*)dlg->docarr[loop]);
			ASSERT(ddd->magic == DOC_MAGIC );

			//P2N("Hit in Document %s\r\n", ddd->filename);

			//m_hitrec = hitrec;
			//Invalidate();

			// Select this document
			dlg->SelDoc(loop);	
			OnOK();
			break;
			}
		want_w -= 100; 
		want_h += 20;
		}

	CDialog::OnLButtonDblClk(nFlags, point);
}
