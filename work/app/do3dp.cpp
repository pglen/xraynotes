
/* =====[ do3dp.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the do3dp.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  11/27/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "xraynotes.h"

#include "do3dp.h"

//////////////////////////////////////////////////////////////////////////

static	RGBQUAD	Do3Dp2(int xx, int yy, int rev, CxImg *img);

static	CPtrArray imgs;
static	double factor;
static	int diff;
static	RGBQUAD bg;

static	int gl_midx = 0, gl_midy = 0;

//////////////////////////////////////////////////////////////////////////
//
// Function name:    Do3Dp2
// Description:      process one pixel of image
// Return type:      
// Argument:         int xx
// Argument:         int yy
// Argument:         int rev
// Argument:         CxImg *img
//
//////////////////////////////////////////////////////////////////////////

RGBQUAD	Do3Dp2(int xx, int yy, int rev, CxImg *img)

{
	//int putx = 0, puty = 0, putz = 0;

	int imgww = img->GetWidth();
	int imghh = img->GetHeight();

	double step = 0.05 / imgs.GetSize() ;
	
	int arrsize = imgs.GetSize();
	//int istep = 2;
	//int sumdiff;

	RGBQUAD maxpix; RGBSET(maxpix, 0, 0, 0);
	//RGBQUAD maxpix; RGBSET(maxpix, 255, 255, 255);

	int zz = -1;

	if(rev)
		zz = arrsize;

	while(true)
		{
		// Up/down logic
		if(rev)
			{
			zz--;
			if(zz < 0)
				break;
			}
		else
			{
			zz++;
			if(zz >= arrsize)
				break;
			}

		int xxx, yyy;

		// Factor from the mid point
		//int xxx = int((xx - gl_midx) / (factor + (zz * diff)) + gl_midx);
		//int yyy = int((yy - gl_midy) / (factor + (zz * diff)) + gl_midy);

		xxx = int((xx - gl_midx) * (1 + step * zz) + gl_midx);
		yyy = int((yy - gl_midy) * (1 + step * zz) + gl_midy);

		//   |---------|
		//   |         |
		//   |    x    |
		//   |         |
		//   |---------|

		//int xdist = xx - xmid;
		//int ydist = yy - ymid;

		//int xxx = xx + (zz * diff); // + imgww/2;
		//int yyy = yy + (zz * diff); // + imghh/2;

		RGBQUAD pix = ((CxImg*)imgs[zz])->GetPixelColorWithOverflow(xxx, yyy, CxImage::OM_COLOR, &bg);

		//if( sumdiff < (RGBSUM(maxpix) - RGBSUM(pix) ))
		//if(RGBSUM(maxpix) > RGBSUM(pix))

		//if(RGBSUM(maxpix) < RGBSUM(pix))

		if(maxpix.rgbBlue < pix.rgbBlue)
			{
			maxpix = pix;
			//putx = xx, puty = yy; putz = zz;
			}

		//sumdiff = RGBSUM(maxpix) - RGBSUM(pix);
		}

	//int dim = maxpix.rgbBlue;
	//maxpix.rgbBlue = maxpix.rgbGreen = maxpix.rgbRed = dim;

	return maxpix;
}

//////////////////////////////////////////////////////////////////////////
//
// Function name:    Do3Dp
// Description:      <empty description>
// Return type:      
// Argument:         int rev
//
//////////////////////////////////////////////////////////////////////////

void	Do3Dp(int rev, CXrayDoc *sdoc, CXrayDoc *tdoc, int midx, int midy)

{
	//P2N("Do3Dp %d %d\r\n", midx, midy);

	CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);

	if(!sdoc) return;
	if(!sdoc->IsDicomFile())
		{
		xMsgBox("Cannot generate 3D: Not a dicom file / series");
		return;
		}
	if(sdoc->dcarr.GetSize() < 2) 
		{
		xMsgBox("Cannot generate 3D: Not a dicom series");
		return;
		}

	// Create if not there
	CXrayDoc *ndoc = tdoc;
	if(!ndoc)
		{
		ndoc = new CXrayDoc; ASSERT(ndoc);
		
		// Create uniq 3D file (for this directory)
		while(true)
			{
			int uniq2 = CXrayDoc::gl_enum++;
			ndoc->filename.Format("%sUntitled_3d_%d.png", tmproot, uniq2);
			if(support.IsFile(ndoc->filename) == 0)
				break;
			}

		// New Document is Born
		dlg->current =  dlg->docarr.Add(ndoc);
		}

	int curr = -1, imgcnt = 0;
	
	// Count frames here
	imgcnt = sdoc->imgarr.GetSize();
	if(!imgcnt)
		{
		P2N("No images to feed onto 3D\r\n");
		return;
		}

	ndoc->backlink = sdoc;

	RECT rc; dlg->GetClientRect(&rc);

	//P2N("Client rect ww=%d\r\n",  rc.right - rc.left);
	int scrww = rc.right - rc.left;

	// Fill in current image
	int dist, alpha = 255, cnt = 0;

	int imgww = sdoc->img->GetWidth();
	int imghh = sdoc->img->GetHeight();

	dist = scrww / imgcnt;

	factor = 0.90;

	//ShowStatus("Processing Frames for 3D render ...", true);

	// Create new image to receive 3D rendering
	CxImg *img = ndoc->img;
	//img->Create(int(imgww / factor), int(imghh / factor), 24);

	if(tdoc == NULL)
		{
		img->Create(int(imgww), int(imghh), 24);
		img->AlphaCreate();	img->AlphaSet(255);	img->SetTransTresh(255);
		img->Clear(0);
		//mask.Create(imgww, imghh, 24);	mask.Clear(0);
		}
	else
		{
		//tdoc->oldshowstate = -1;
		img = ndoc->render;
		}

	curr = imgcnt;
	imgs.RemoveAll();

	// Generate array of images
	while(true)
		{
		if(--curr < 0)
			break;

		CxImg *rend = (CxImg *)sdoc->imgarr[curr]; CHECKIMG(rend)
		
		if(((DicomFile*)sdoc->dcarr[curr])->enabled )
			{
			//P2N("Building img array=%d rend=%p\r\n", cnt++, rend);
			imgs.Add(rend);	
			}
		}

	//P2N("Image count %d\r\n", imgs.GetSize());

	// Assign globals
	diff = (int)((1-factor) / imgs.GetSize());
	//diff = 1;

	gl_midx = midx * imgww / 100;
	gl_midy = midy * imghh / 100;

	//P2N("gl_midx=%d gl_midy=%d\r\n", gl_midx, gl_midy);

	//double diff = (1 - factor) / 3 ;//imgs.GetSize();
	//P2N("Step %f\r\n", diff);

	RGBSET(bg, 0,0,0);

	//if(rev)
	//	RGBSET(bg,255,255,255);

	dlg->BusyCursor();

	//////////////////////////////////////////////////////////////////////
	//Process image pix by pix

	for (int xx = 0; xx < imgww; xx += 1)
		{
		//support.YieldToWinEx();

		if((xx % 10) == 0)
			{
			img->SetProgress(100 * xx/imgww);	
			}

		for (int yy =0; yy < imghh; yy += 1)
			{
			RGBQUAD pix = Do3Dp2(xx, yy, rev, img);
			img->SetPixelColor(xx, yy, pix);
			}
		}

	dlg->BusyCursor(false);

	//dlg->helper->OnDocSetFocus();
	//HideStatus();

	dlg->InvalidateSurface();

	dlg->helper->OnDocSetFocus();
}
