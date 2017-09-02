
/* =====[ LayImg.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the LayImg.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  11/28/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// LayImg.cpp: implementation of the CLayImg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "xraynotes.h"

#include "Layer.h"
#include "XntMask.h"
#include "gdirot.h"
#include "XntMask.h"
#include "BmpDC.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLayImg::CLayImg()

{
	//P2N("CLayImg::CLayImg()\r\n");
	InitVars();
}

CLayImg::~CLayImg()

{
	if(image)
		delete image;

	if(org_image)
		delete org_image;
}

//////////////////////////////////////////////////////////////////////////

int CLayImg::Draw(CDC *pDC, float factor, int dummy1)

{
	ASSERT(gdata.kind == LAYER_IMAGE);

	//P2N("CLayImg::Draw xx=%d yy=%d ww=%d hh=%d focus=%d alpha=%d\r\n",
	//		gdata.xx, gdata.yy, gdata.ww, gdata.hh, focus, gdata.alpha);

	RECT oldbound;
	
	// Alpha blend
    BLENDFUNCTION m_bf;

    m_bf.BlendOp = AC_SRC_OVER;
    m_bf.BlendFlags = 0;
    m_bf.SourceConstantAlpha = gdata.alpha;
    m_bf.AlphaFormat = 0;

	CDC  dcMem;	dcMem.CreateCompatibleDC(pDC);

	// CreateCompatibleBitmap does not work on color/memory device!

	HBITMAP bm =::CreateCompatibleBitmap(pDC->GetSafeHdc(), abs(gdata.ww), abs(gdata.hh));
	HBITMAP oldbm = (HBITMAP)::SelectObject(dcMem, bm);

	// Fill it in
	CRect rc2; GetRect(&rc2);	rc2.NormalizeRect();

	CRect rc(0, 0, abs(gdata.ww), abs(gdata.hh));
	dcMem.FillSolidRect(rc, RGB(255,255,255));

	CRect	rect(0,0,0,0);
	CSize	ss(0,0);

	// Set new clipping 
	pDC->GetClipBox(&oldbound);

	if(image && image->IsValid())
		{

#if 0
		if(gdata.rotate_angle != 0)
			{
			// Cache rotation angle
			if(old_rotate != gdata.rotate_angle)
				{
				//float angle = (float)((MATHPI * gdata.rotate_angle) / 180.0);
				
				P2N("CLayImg::Draw - rotated %d\r\n", gdata.rotate_angle);

				org_image->Rotate((float)gdata.rotate_angle, image);
				//org_image->Rotate2((float)gdata.rotate_angle, image, CxImage::IM_NEAREST_NEIGHBOUR,
				//	CxImage::OM_BACKGROUND, 0, true, true);


				//image->Rotate(angle);

				old_rotate = gdata.rotate_angle;
				}
			}
#endif

		//rect.SetRect(0, 0,
		//		(int)(image->GetWidth() * factor),
		//			(int)(image->GetHeight() * factor));

		rect.SetRect(0, 0, gdata.ww, gdata.hh);

		//GetRect(&rect);

		//if(img_elliptify)
		//	{
		//	pDC->BeginPath();
		//	pDC->Ellipse(rect);
		//	pDC->EndPath();
		//	pDC->SelectClipPath(RGN_COPY);
		//	}

		image->Draw(dcMem.GetSafeHdc(), rect, 0, 0);
		}

	CRect rcorg; GetRect(&rcorg); 	
	int dstW, dstH;
	XntMask xmask; CGdiRot rotdc; 
	float angle = -(float)((MATHPI * gdata.rotate_angle) / 180.0);
	
	RECT rc5; 
	rotdc.GetRotateRC(rcorg.Width(), rcorg.Height(), angle, &rc5);
	dstW = rc5.right; dstH = rc5.bottom;
	
	// Create the new memory DCs
	CBmpDC hdcDst(dcMem, dstW, dstH);

	CRect rc4;
	rc4.left = rcorg.left + (rcorg.Width()/2 - dstW/2);
	rc4.top  = rcorg.top + (rcorg.Height()/2 - dstH/2);
	rc4.right  = rc4.left + dstW;
	rc4.bottom = rc4.top + dstH;

	//////////////////////////////////////////////////////////////////////////
	// Rotate that memory HDC
	rotdc.RotateMemoryDC(dcMem, rcorg.Width(), rcorg.Height(), angle, hdcDst);

	HBITMAP hBmpMask = xmask.MakeMaskBmpDC(hdcDst, dstW, dstH, 0);

	// Put it out
	xmask.MaskBlt(pDC->GetSafeHdc(), rc4.left, rc4.top,	dstW, dstH, 
				hdcDst, 0, 0, hBmpMask, 0, 0,
						MAKEROP4(SRCCOPY, DSTCOPY), gdata.alpha);

	// Allow clipping region to draw all:
	//pDC->SelectClipRgn(NULL);
	
#if 0
	if(gdata.alpha != 255)
		{
		AlphaBlend(pDC->GetSafeHdc(),
			(int)(rc2.left * factor), 
				(int)(rc2.top * factor),
					(int)(rc2.Width() * factor), (int)(rc2.Height() * factor),
						dcMem, 0,0, rc2.Width(), rc2.Height(), m_bf);
		}
	else
		{

		::BitBlt(pDC->GetSafeHdc(),
			(int)(rc2.left * factor), 
				(int)(rc2.top * factor),
					(int)(rc2.Width() * factor), 
						(int)(rc2.Height() * factor),
					dcMem, 0,0, SRCCOPY);
		}
#endif

	::SelectObject(dcMem, oldbm);  ::DeleteObject(bm);
	//dcMem.DeleteDC();

	//pDC->SelectObject(oldbrush);

	//CRgn rgn; rgn.CreateRectRgn(oldbound.left, oldbound.top,
	//				oldbound.right, oldbound.bottom);		
	//pDC->SelectClipRgn(&rgn);	
	
	if(focus)
		{
		CRect rect(gdata.xx, gdata.yy, 
			gdata.xx+gdata.ww, gdata.yy+gdata.hh);

		rect.InflateRect(2,2);
		DrawFocus(pDC, &rect);
		DrawTrackerX(pDC, &rect);
		}

	return 0;
}

//////////////////////////////////////////////////////////////////////////

void CLayImg::InitVars()


{
	gdata.kind = LAYER_IMAGE;	
	image = org_image = NULL;
	old_rotate = 0;

	//handlesize = 10;
}

int CLayImg::Init(POINT pt, int dummy1, int dummy2)

{
	P2N("CLayImg::Init\r\n");

	state = STATE_INIT;

	gdata.xx = pt.x;
	gdata.yy = pt.y;

	gdata.ww = 8;
	gdata.hh = 8;

	return 0;
}

//////////////////////////////////////////////////////////////////////////

int CLayImg::MouseEvent(UINT event, UINT nFlags, CPoint point)

{
	int ret = -1;

	//P2N("CLayImg::MouseEvent %d\r\n", event);
	
	switch(event)
		{
		case    MOUSE_LEFTUP      : 
			if(state == STATE_INIT)
				{
				OpenImage();

				if(image)
					{
					Move(gdata.xx - image->GetWidth()/2,
							gdata.yy - image->GetHeight()/2);
					}
				state = STATE_IDLE;
				ret = 0;
				}
			break;

		case MOUSE_RIGHTDOWN   :
			{
			CShadesDlg *dlg; GMD(dlg);  CXrayDoc *doc; GCD(doc, dlg);
			if(doc && doc->m_currlay)
				{			
				// pop the RClick menu
				CXrayM *omenu = new CXrayM; ASSERT(omenu);
				omenu->LoadMenu(IDR_MENU15, 1);

				omenu->SetItemText(0, "Rectangle Object");
				// Disable multi items
				if(dlg->helper->GetNumSelObj() < 2)
					{	
					omenu->SetItemStatus(7, MF_GRAYED);
					omenu->SetItemStatus(8, MF_GRAYED);
					}
				else
					{
					omenu->SetItemText(0, "Multiple Objects");
					}
				omenu->Show(point.x, point.y);
				ret = 0;
				}
			}
		break;
		}

	return ret;
}

static CString oldopen;

void CLayImg::OpenImage()

{
	CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);

	if(oldopen == "")
		oldopen = dataroot;

	CString fileName;

	int	bOpenFileDialog = true;
	int	dwFlags = 0;
	int *pType = NULL;

	if(((CShadesApp*)AfxGetApp())->inmodal)		
		{
		ShowStatus("An instance of a Modal Dialog is open already.");
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
	
	P2N("Openinig image for object %s", fileName);
	POSITION pos = dlgFile.GetStartPosition( );

	int cnt = 0;
	while(true)
		{
		if(!pos) 
			break;

		CString str = dlgFile.GetNextPathName(pos);

		P2N("Opening new image: '%s'\r\n",  str); 
		
		//OpenImageFile(str);

		image = new CxImg;
		image->Load(str);

		if(!image->IsValid())
			{
			// Reset for error display
			delete image ;	image = NULL;
			
			xMsgBox("Cannot Open Image '%s'", str);
			}
		else
			{
			
			gdata.ww = image->GetWidth();
			gdata.hh = image->GetHeight();

			org_image = new CxImg;
			org_image->Copy(*image);
			}
		cnt++;
		
		support.YieldToWinEx();
		}
	
	dlg->Invalidate();
	support.YieldToWinEx();

	oldopen = fileName;
	support.GetDirName(oldopen);

	return;
}

