
/* =====[ GdiRot.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the GdiRot.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  3/27/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#include "stdafx.h"
#include "xraynotes.h"
#include "GdiRot.h"
#include "float.h"

//////////////////////////////////////////////////////////////////////////
// Define this to numeric one (1) for showing perf. statistics on the
// drawn object

#define MEASURE_PERF	0

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGdiRot::CGdiRot()
{

}

CGdiRot::~CGdiRot()
{

}

//////////////////////////////////////////////////////////////////////////
// Helper function for getting the maximum of 4 floats

// Helper function for getting the minimum of 4 floats
float CGdiRot::min4(float a, float b, float c, float d)

{
	if (a < b) {
		if (c < a) {
			if (d < c)
				return d;
			else
				return c;
		} else {
			if (d < a)
				return d;
			else
				return a;
		}
	} else {
		if (c < b) {
			if (d < c)
				return d;
			else
				return c;
		} else {
			if (d < b)
				return d;
			else
				return b;
		}
	}
}

// Helper function for getting the maximum of 4 floats

float CGdiRot::max4(float a, float b, float c, float d)
{
	if (a > b) {
		if (c > a) {
			if (d > c)
				return d;
			else
				return c;
		} else {
			if (d > a)
				return d;
			else
				return a;
		}
	} else {
		if (c > b) {
			if (d > c)
				return d;
			else
				return c;
		} else {
			if (d > b)
				return d;
			else
				return b;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// An alternate algorythm to verfy results of the max/min  routines
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Verify max4
//

float CGdiRot::max4v(float a, float b, float c, float d)

{
	float res = -FLT_MAX, arr[4];
	
	arr[0] = a;	arr[1] = b;	arr[2] = c;	arr[3] = d;

	for(int loop = 0; loop < 4; loop++)
		if(arr[loop] > res)
			res = arr[loop];

	return res;
}

//////////////////////////////////////////////////////////////////////////
// Verify min4
//

float CGdiRot::min4v(float a, float b, float c, float d)

{
	float res = FLT_MAX, arr[4];
	
	arr[0] = a;	arr[1] = b;	arr[2] = c;	arr[3] = d;

	for(int loop = 0; loop < 4; loop++)
		if(arr[loop] < res)
			res = arr[loop];

	return res;
}

//////////////////////////////////////////////////////////////////////////
// Returns the DI (Device Independent) bits of the Bitmap
// We use 32 bit bitmaps since it's easy to adress it in memory and no
// padding of the horizontal line(s) are required.

// --------------------------------------------------------------
//
/// \brief   Get Device Independent Bits
/// 
/// \details Returns the DI (Device Independent) bits of the Bitmap 
/// \n  
/// Return type:      
/// \n  
/// \param hdcSrc Arg Type: HDC Device context to interrogate
/// \param hBmpSrc Arg Type: HBITMAP bitmap to interrogate 
/// \param nx Arg Type: int Size of x extent
/// \param ny Arg Type: int Size of y extent

pBGR	CGdiRot::MyGetDibBits(HDC hdcSrc, HBITMAP hBmpSrc, int nx, int ny)

{
	BOOL bRes;
	
	// Correct empty rectangle to 1x1
	if(nx == 0)	nx = 1;
	if(ny == 0)	ny = 1;
	
	BITMAPINFO bi;	DECL32BMPH(bi, nx, ny);	
	pBGR buf = (pBGR) malloc(nx * 4 * ny);	
	
	if(buf)
		{		
		bRes = GetDIBits(hdcSrc, hBmpSrc, 0, ny, buf, &bi, DIB_RGB_COLORS);

		// Could not get it, erase 
		if (!bRes) 
			{
			free(buf); buf = NULL;
			}
		}
	return buf;
}

//////////////////////////////////////////////////////////////////////////
// Calculate rotated dimentions
//

void	CGdiRot::GetRotateDim(int SrcW, int SrcH, double angle, int &dstW, int &dstH)

{
	float x1, x2, x3, x4, y1, y2, y3, y4, cA, sA;
	int OfX, OfY;

	// Short circuit for zero angles
	if(angle == 0)
		{
		dstW = SrcW,  dstH = SrcH;	return;
		}

	// Rotate the bitmap around the center
	float CtX = ((float) SrcW) / 2;	
	float CtY = ((float) SrcH) / 2;

	// Calculate the destination positions for the four courners
	cA = (float) cos(angle); sA = (float) sin(angle);

	x1 = CtX + (-CtX) * cA - (-CtY) * sA;
	x2 = CtX + (SrcW - CtX) * cA - (-CtY) * sA;
	x3 = CtX + (SrcW - CtX) * cA - (SrcH - CtY) * sA;
	x4 = CtX + (-CtX) * cA - (SrcH - CtY) * sA;

	y1 = CtY + (-CtY) * cA + (-CtX) * sA;
	y2 = CtY + (SrcH - CtY) * cA + (-CtX) * sA;
	y3 = CtY + (SrcH - CtY) * cA + (SrcW - CtX) * sA;
	y4 = CtY + (-CtY) * cA + (SrcW - CtX) * sA;

	OfX = ((int) floor(min4(x1, x2, x3, x4)));
	OfY = ((int) floor(min4(y1, y2, y3, y4)));
	
	dstW = ((int) ceil(max4(x1, x2, x3, x4))) - OfX;
	dstH = ((int) ceil(max4(y1, y2, y3, y4))) - OfY;
}

//////////////////////////////////////////////////////////////////////////
// RotateMemoryDC rotates a memory DC and returns the rotated DC as well 
// as its dimensions

// --------------------------------------------------------------
//
/// \brief   RotateMemoryDC rotates a memory DC 
/// 
/// \details RotateMemoryDC rotates a memory DC and returns the rotated DC as well 
/// as its dimensions 
/// \n  
/// Return type:      void 
/// \n  
/// \param hdcSrc Arg Type: HDC - Source Device context
/// \param SrcW Arg Type: int	
/// \param SrcH Arg Type: int 
/// \param angle Arg Type: float - Angle to rotate with (RAD)
/// \param hdcDst Arg Type: HDC   - Destination Device context

void CGdiRot::RotateMemoryDC(HDC hdcSrc, int SrcW, int SrcH, float angle, HDC hdcDst)

{
	float x1, x2, x3, x4, y1, y2, y3, y4;
	float cA, sA, CtX, CtY;
	float orgX, orgY; // divisor;

	int dstW, dstH, OfX, OfY, stepX, stepY;

	HBITMAP hBmpDst = (HBITMAP) GetCurrentObject(hdcDst, OBJ_BITMAP);
	ASSERT(hBmpDst);

	HBITMAP hBmpSrc = (HBITMAP) GetCurrentObject(hdcSrc, OBJ_BITMAP);
	ASSERT(hBmpSrc);
	
#ifdef PERF
	char str[40];
	LARGE_INTEGER lStart, lEnd, lFreq;
#endif

	pBGR src, dst, dstLine;
	BITMAPINFO bi;

	// Rotate the bitmap around the center
	CtX = ((float) SrcW) / 2;	CtY = ((float) SrcH) / 2;

	// First, calculate the destination positions for the four courners to get dstW and dstH
	cA = (float) cos(-angle); sA = (float) sin(-angle);

	x1 = CtX + (-CtX) * cA - (-CtY) * sA;
	x2 = CtX + (SrcW - CtX) * cA - (-CtY) * sA;
	x3 = CtX + (SrcW - CtX) * cA - (SrcH - CtY) * sA;
	x4 = CtX + (-CtX) * cA - (SrcH - CtY) * sA;

	y1 = CtY + (-CtY) * cA + (-CtX) * sA;
	y2 = CtY + (SrcH - CtY) * cA + (-CtX) * sA;
	y3 = CtY + (SrcH - CtY) * cA + (SrcW - CtX) * sA;
	y4 = CtY + (-CtY) * cA + (SrcW - CtX) * sA;

	OfX = ((int) floor(min4(x1, x2, x3, x4)));
	OfY = ((int) floor(min4(y1, y2, y3, y4)));
	
	dstW = ((int) ceil(max4(x1, x2, x3, x4))) - OfX;
	dstH = ((int) ceil(max4(y1, y2, y3, y4))) - OfY;

	// Start timing
#ifdef PERF
	QueryPerformanceFrequency(&lFreq);
	QueryPerformanceCounter(&lStart);
#endif

	//////////////////////////////////////////////////////////////////////////
	// Get the bitmap bits for the source and destination
	src = MyGetDibBits(hdcSrc, hBmpSrc, SrcW, SrcH); ASSERT(src);
	dst = MyGetDibBits(hdcDst, hBmpDst, dstW, dstH); ASSERT(dst);

	dstLine = dst;
	//divisor = cA*cA + sA*sA;

		// Step through the destination bitmap
	for (stepY = 0; stepY < dstH; stepY++) 
		{
		for (stepX = 0; stepX < dstW; stepX++) 
			{
			float curX = (float) stepX + OfX  - CtX;
			float curY = (float) stepY + OfY  - CtY;

			// Calculate the source coordinate
			orgX = CtX + (curX) * cA - (curY) * sA;
			orgY = CtY + (curY) * cA + (curX) * sA;

			int iorgX = (int)orgX, iorgY = (int)orgY;
				
			// Round it ... did not make any difference
			//if(orgX - floor(orgX) > 0.5) iorgX = ceil(orgX);			
			//if(orgY - floor(orgY) > 0.5) iorgY = ceil(orgY);
			
			if ((iorgX >= 0) && (iorgY >= 0) && (iorgX < SrcW) && (iorgY < SrcH)) 
				{
				sBGR col = src[iorgX + iorgY * SrcW];

				// Inside the source bitmap -> copy the bits
				
				// Correct the black to non transparent

				//if(col.bb == 0 && col.rr == 0 && col.gg == 0)
				//	col.rr = 1, col.gg = 1, col.bb = 1;

				dstLine[stepX] = col; //src[iorgX + iorgY * SrcW];
				} 
			else 
				{
				// Outside the source -> set the color to light grey
				// We preserve the original bits
				//dstLine[stepX].bb = 240;
				//dstLine[stepX].gg = 240;
				//dstLine[stepX].rr = 240;
				}
			}
		dstLine = dstLine + dstW;
		}

	// Test: copy bits unrotated
	//for (stepY = 0; stepY < SrcH; stepY++) 
	//	{
	//	for (stepX = 0; stepX < SrcW; stepX++) 
	//		{
	//		dst[stepY * dstW + stepX] = 
	//			src[stepY * SrcW + stepX];
	//		}
	//	}
		
	// Set the new Bitmap
	bi.bmiHeader.biSize = sizeof(bi.bmiHeader);
	bi.bmiHeader.biWidth = dstW;
	bi.bmiHeader.biHeight = dstH;
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biBitCount = sizeof(DWORD) * 8;
	bi.bmiHeader.biCompression = BI_RGB;
	bi.bmiHeader.biSizeImage = dstW * sizeof(DWORD) * dstH;
	bi.bmiHeader.biClrUsed = 0;
	bi.bmiHeader.biClrImportant = 0;

	SetDIBits(hdcDst, hBmpDst, 0, dstH, dst, &bi, DIB_RGB_COLORS);
	
#ifdef PERF
	// End timing and show Time taken in milliseconds
	QueryPerformanceCounter(&lEnd);
	sprintf(str, "time: %d ms", ((lEnd.QuadPart - lStart.QuadPart) * 1000) / lFreq.QuadPart);
	// Output into the middle
	TextOut(hdcDst, dstW/2 - 30, dstH/2, str, strlen(str));
#endif

	// Free the color arrays
	free(src), free(dst);
}

//////////////////////////////////////////////////////////////////////////
// Calculate rotated dimentions and normalize result to zero based rect
//

void	CGdiRot::GetRotateRC(int SrcW, int SrcH, float angle, RECT *rc)

{
	float x1, x2, x3, x4, y1, y2, y3, y4, cA, sA;
	
	// Rotate the bitmap around the center
	float CtX = ((float) SrcW) / 2, CtY = ((float) SrcH) / 2;

	// Calculate the destination positions for the four courners
	cA = (float) cos(angle); sA = (float) sin(angle);

	x1 = CtX + (-CtX) * cA - (-CtY) * sA;
	x2 = CtX + (SrcW - CtX) * cA - (-CtY) * sA;
	x3 = CtX + (SrcW - CtX) * cA - (SrcH - CtY) * sA;
	x4 = CtX + (-CtX) * cA - (SrcH - CtY) * sA;

	y1 = CtY + (-CtY) * cA + (-CtX) * sA;
	y2 = CtY + (SrcH - CtY) * cA + (-CtX) * sA;
	y3 = CtY + (SrcH - CtY) * cA + (SrcW - CtX) * sA;
	y4 = CtY + (-CtY) * cA + (SrcW - CtX) * sA;

	rc->left = ((int) floor(min4(x1, x2, x3, x4)));
	rc->top  = ((int) floor(min4(y1, y2, y3, y4)));

	rc->right = ((int) ceil(max4(x1, x2, x3, x4))); 
	rc->bottom = ((int) ceil(max4(y1, y2, y3, y4)));

	//P2N("GetRotateDimR l=%d t=%d r=%d b=%d\r\n", 
	//				rc->left, rc->top, rc->right, rc->bottom);

	// Normalize it:
	if(rc->left != 0)
		{
		rc->right -= rc->left, rc->left = 0;
		}

	if(rc->top != 0)
		{
		rc->bottom -= rc->top, rc->top = 0;
		}
}

