
/* =====[ GDIRotate.cpp ]========================================== 
                                                                             
   Description:     The GDIRotate project, implementation of the GDIRotate.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  3/24/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#include "stdafx.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <crtdbg.h>

#include "comdef.h"
#include "resource.h"
#include "mxpad.h"

#define MAX_STRLEN	100
#define MATHPI		3.1415926

//////////////////////////////////////////////////////////////////////////
// Some common defines, like MFC would

#ifndef ASSERT
#define ASSERT _ASSERT
#endif

#ifndef VERIFY
	#ifdef _DEBUG			
		#define VERIFY(aa) ASSERT(aa)			
	#else					
		#define VERIFY(aa) (aa)			
	#endif					
#endif

//define ASSERT(mm) if(!mm) MessageBox(NULL, "Assert failed", "ASSERT", MB_OK);

#define INITBLEND(aa)								\
		aa.BlendOp	= AC_SRC_OVER;					\
		aa.BlendFlags = 0;							\
		aa.SourceConstantAlpha = min(alpha, 255);	\
		aa.AlphaFormat = 0;							\

//////////////////////////////////////////////////////////////////////////
// Define PERF (uncomment below) for performance display

#define PERF

#include "MaskBlt.h"

//////////////////////////////////////////////////////////////////////////
//
//  Raster operations from MSDN
//

static	DWORD	ROP3FromIndex(DWORD dwIndex)

{
    static DWORD ROP3Table[256] = 
    {
        0x00000042, 0x00010289,
        0x00020C89, 0x000300AA,
        0x00040C88, 0x000500A9,
        0x00060865, 0x000702C5,
        0x00080F08, 0x00090245,
        0x000A0329, 0x000B0B2A,
        0x000C0324, 0x000D0B25,
        0x000E08A5, 0x000F0001,
        0x00100C85, 0x001100A6,
        0x00120868, 0x001302C8,
        0x00140869, 0x001502C9,
        0x00165CCA, 0x00171D54,
        0x00180D59, 0x00191CC8,
        0x001A06C5, 0x001B0768,
        0x001C06CA, 0x001D0766,
        0x001E01A5, 0x001F0385,
        0x00200F09, 0x00210248,
        0x00220326, 0x00230B24,
        0x00240D55, 0x00251CC5,
        0x002606C8, 0x00271868,
        0x00280369, 0x002916CA,
        0x002A0CC9, 0x002B1D58,
        0x002C0784, 0x002D060A,
        0x002E064A, 0x002F0E2A,
        0x0030032A, 0x00310B28,
        0x00320688, 0x00330008,
        0x003406C4, 0x00351864,
        0x003601A8, 0x00370388,
        0x0038078A, 0x00390604,
        0x003A0644, 0x003B0E24,
        0x003C004A, 0x003D18A4,
        0x003E1B24, 0x003F00EA,
        0x00400F0A, 0x00410249,
        0x00420D5D, 0x00431CC4,
        0x00440328, 0x00450B29,
        0x004606C6, 0x0047076A,
        0x00480368, 0x004916C5,
        0x004A0789, 0x004B0605,
        0x004C0CC8, 0x004D1954,
        0x004E0645, 0x004F0E25,
        0x00500325, 0x00510B26,
        0x005206C9, 0x00530764,
        0x005408A9, 0x00550009,
        0x005601A9, 0x00570389,
        0x00580785, 0x00590609,
        0x005A0049, 0x005B18A9,
        0x005C0649, 0x005D0E29,
        0x005E1B29, 0x005F00E9,
        0x00600365, 0x006116C6,
        0x00620786, 0x00630608,
        0x00640788, 0x00650606,
        0x00660046, 0x006718A8,
        0x006858A6, 0x00690145,
        0x006A01E9, 0x006B178A,
        0x006C01E8, 0x006D1785,
        0x006E1E28, 0x006F0C65,
        0x00700CC5, 0x00711D5C,
        0x00720648, 0x00730E28,
        0x00740646, 0x00750E26,
        0x00761B28, 0x007700E6,
        0x007801E5, 0x00791786,
        0x007A1E29, 0x007B0C68,
        0x007C1E24, 0x007D0C69,
        0x007E0955, 0x007F03C9,
        0x008003E9, 0x00810975,
        0x00820C49, 0x00831E04,
        0x00840C48, 0x00851E05,
        0x008617A6, 0x008701C5,
        0x008800C6, 0x00891B08,
        0x008A0E06, 0x008B0666,
        0x008C0E08, 0x008D0668,
        0x008E1D7C, 0x008F0CE5,
        0x00900C45, 0x00911E08,
        0x009217A9, 0x009301C4,
        0x009417AA, 0x009501C9,
        0x00960169, 0x0097588A,
        0x00981888, 0x00990066,
        0x009A0709, 0x009B07A8,
        0x009C0704, 0x009D07A6,
        0x009E16E6, 0x009F0345,
        0x00A000C9, 0x00A11B05,
        0x00A20E09, 0x00A30669,
        0x00A41885, 0x00A50065,
        0x00A60706, 0x00A707A5,
        0x00A803A9, 0x00A90189,
        0x00AA0029, 0x00AB0889,
        0x00AC0744, 0x00AD06E9,
        0x00AE0B06, 0x00AF0229,
        0x00B00E05, 0x00B10665,
        0x00B21974, 0x00B30CE8,
        0x00B4070A, 0x00B507A9,
        0x00B616E9, 0x00B70348,
        0x00B8074A, 0x00B906E6,
        0x00BA0B09, 0x00BB0226,
        0x00BC1CE4, 0x00BD0D7D,
        0x00BE0269, 0x00BF08C9,
        0x00C000CA, 0x00C11B04,
        0x00C21884, 0x00C3006A,
        0x00C40E04, 0x00C50664,
        0x00C60708, 0x00C707AA,
        0x00C803A8, 0x00C90184,
        0x00CA0749, 0x00CB06E4,
        0x00CC0020, 0x00CD0888,
        0x00CE0B08, 0x00CF0224,
        0x00D00E0A, 0x00D1066A,
        0x00D20705, 0x00D307A4,
        0x00D41D78, 0x00D50CE9,
        0x00D616EA, 0x00D70349,
        0x00D80745, 0x00D906E8,
        0x00DA1CE9, 0x00DB0D75,
        0x00DC0B04, 0x00DD0228,
        0x00DE0268, 0x00DF08C8,
        0x00E003A5, 0x00E10185,
        0x00E20746, 0x00E306EA,
        0x00E40748, 0x00E506E5,
        0x00E61CE8, 0x00E70D79,
        0x00E81D74, 0x00E95CE6,
        0x00EA02E9, 0x00EB0849,
        0x00EC02E8, 0x00ED0848,
        0x00EE0086, 0x00EF0A08,
        0x00F00021, 0x00F10885,
        0x00F20B05, 0x00F3022A,
        0x00F40B0A, 0x00F50225,
        0x00F60265, 0x00F708C5,
        0x00F802E5, 0x00F90845,
        0x00FA0089, 0x00FB0A09,
        0x00FC008A, 0x00FD0A0A,
        0x00FE02A9, 0x00FF0062,
    };

    return ROP3Table[dwIndex&0xFF];
}

//////////////////////////////////////////////////////////////////////////

static	BYTE SwapROP3_SrcDst(BYTE bRop3)

{
    // swap 1,2 bit and 5,6 bit
  

    DWORD dwRop3 = bRop3;
    BITS bits = *(BITS*)&dwRop3;

    unsigned t = bits.bit1;
    bits.bit1 = bits.bit2;
    bits.bit2 = t;

    t = bits.bit5;
    bits.bit5 = bits.bit6;
    bits.bit6 = t;

    return *(BYTE*)&bits;
}


//////////////////////////////////////////////////////////////////////////

void MakeMaskBmp(HBITMAP* pBmpSource, HBITMAP* hBmpMask,
							COLORREF clrpTransColor, // Pass 0xFFFFFFFF if unknown
								int iTransPixelX,  int iTransPixelY  )
{

	BITMAP bmp;	GetObject(*pBmpSource, sizeof(BITMAP), &bmp);
	
	int ww = bmp.bmWidth, hh = bmp.bmHeight;
	
#if 0
	char str[255]; _snprintf(str, sizeof(str), 
		"LONG   bmType=%d;  LONG   bmWidth=%d;  LONG   bmHeight=%d;  LONG   bmWidthBytes=%d; \r\n",
				bmp.bmType, bmp.bmWidth, bmp.bmHeight, bmp.bmWidthBytes);

	_RPT1( _CRT_WARN, "%s", str);
	
	 _snprintf(str, sizeof(str), 
	"WORD   bmPlanes=%d; WORD   bmBitsPixel=%d;  LPVOID bmBits=%p; \r\n", 
				bmp.bmPlanes, bmp.bmBitsPixel, bmp.bmBits); 

	_RPT1( _CRT_WARN, "%s", str);
#endif

	// We will need two DCs to work with. One to hold the Image
	// (the source), and one to hold the mask (destination).
	// When blitting onto a monochrome bitmap from a color, pixels
	// in the source color bitmap that are equal to the background
	// color are blitted as white. All the remaining pixels are
	// blitted as black.

	HDC hdcSrc = CreateCompatibleDC(NULL);	ASSERT(hdcSrc);
	HDC hdcDst = CreateCompatibleDC(NULL);	ASSERT(hdcDst);

	*hBmpMask = CreateBitmap(ww, hh, 1, 1, NULL); ASSERT(*hBmpMask);

	// Load the bitmaps into memory DC
	HBITMAP hbmold1 =  (HBITMAP) SelectObject(hdcSrc, *pBmpSource); ASSERT(hbmold1);
	HBITMAP hbmold2 =  (HBITMAP) SelectObject(hdcDst, *hBmpMask);	ASSERT(hbmold2);

	// Dynamically get the transparent color
	COLORREF clrTrans;
	if (clrpTransColor == -1)
		{
		// User did not specify trans color so get it from bmp
		clrTrans = GetPixel(hdcSrc, iTransPixelX, iTransPixelY);
		}
	else
		{
		clrTrans = clrpTransColor;
		}

	// Change the background to trans color
	COLORREF clrSaveBk  = SetBkColor(hdcSrc, clrTrans);

	COLORREF clrSaveDstText = SetTextColor(hdcSrc, RGB(255,255,255));
	SetBkColor(hdcSrc, RGB(0,0,0));

	// This call sets up the mask bitmap.
	BitBlt(hdcDst, 0,0, ww, hh, hdcSrc, 0, 0, NOTSRCCOPY);

	// Now, we need to paint onto the original image, making
	// sure that the "transparent" area is set to black. What
	// we do is AND the monochrome image onto the color Image
	// first. When blitting from mono to color, the monochrome
	// pixel is first transformed as follows:
	// if  1 (black) it is mapped to the color set by SetTextColor().
	// if  0 (white) is is mapped to the color set by SetBkColor().
	// Only then is the raster operation performed.


#if	0
	hdcSrc.BitBlt(0,0,bm.bmWidth, bm.bmHeight, &hdcDst, 0,0, SRCAND);
#endif

	// Clean up by deselecting any objects, and delete the
	// DC's.

	SetTextColor(hdcDst, clrSaveDstText);
	SetBkColor(hdcSrc, clrSaveBk);

	SelectObject(hdcSrc, hbmold1);
	SelectObject(hdcDst, hbmold2);

	DeleteDC(hdcSrc);	DeleteDC(hdcDst);

	//hBmpMask->GetObject(sizeof(BITMAP), &bm);
}

//////////////////////////////////////////////////////////////////////////

BOOL WINAPI MaskBlt(HDC hdcDest, int nXDest, int nYDest, 
				int nWidth, int nHeight,
					HDC hdcSrc, int nXSrc, int nYSrc,
						HBITMAP hbmMask, int xMask, int yMask,
							DWORD dwRop, int alpha)
{
	//P2N("XntMaskBlt %d %d\r\n", nWidth, nHeight);

	if ( hbmMask == NULL )
		{
		//P2N("XntMaskBlt NULL mask\r\n");

		return BitBlt(hdcDest, nXDest, nYDest, nWidth, nHeight, hdcSrc, 
					  nXSrc, nYSrc, FORE_ROP3(dwRop));
		}

	// 1. make mask bitmap's dc
	HDC hDCMask = ::CreateCompatibleDC(hdcDest);
	HBITMAP hOldMaskBitmap = (HBITMAP)::SelectObject(hDCMask, hbmMask);
	ASSERT ( hOldMaskBitmap );

	// 2. make masked Background bitmap

	// 2.1 make bitmap
	HDC hDC1 = ::CreateCompatibleDC(hdcDest); ASSERT ( hDC1 );
	HBITMAP hBitmap2 = ::CreateCompatibleBitmap(hdcDest, nWidth, nHeight);
	HBITMAP hOldBitmap2 = (HBITMAP)::SelectObject(hDC1, hBitmap2);
	ASSERT ( hOldBitmap2 );

	// 2.2 draw tmp bitmap and mask
	DWORD dwRop3 = BACK_ROP3(dwRop);
	::BitBlt(hDC1, 0, 0, nWidth, nHeight, hdcSrc, nXSrc, nYSrc, SRCCOPY);
	::BitBlt(hDC1, 0, 0, nWidth, nHeight, hdcDest, nXDest, nYDest, dwRop3);
	::BitBlt(hDC1, 0, 0, nWidth, nHeight, hDCMask, xMask, yMask, DSTERASE);

	// 3. make masked Foreground bitmap

	// 3.1 make bitmap
	HDC hDC2 = ::CreateCompatibleDC(hdcDest); ASSERT ( hDC2 );

	HBITMAP hBitmap3 = ::CreateCompatibleBitmap(hdcDest, nWidth, nHeight);
	HBITMAP hOldBitmap3 = (HBITMAP)::SelectObject(hDC2, hBitmap3);
	ASSERT ( hOldBitmap3 );

	// 3.2 draw src bitmap and mask
	dwRop3 = FORE_ROP3(dwRop);
	VERIFY(::BitBlt(hDC2, 0, 0, nWidth, nHeight, hdcDest, nXDest, nYDest, SRCCOPY));
	VERIFY(::BitBlt(hDC2, 0, 0, nWidth, nHeight, hdcSrc, nXSrc, nYSrc, dwRop3));
	VERIFY(::BitBlt(hDC2, 0, 0, nWidth, nHeight, hDCMask, xMask, yMask, SRCAND));

	// 4. combine two bitmaps and copy it to hdcDest
	VERIFY(::BitBlt(hDC1, 0, 0, nWidth, nHeight, hDC2, 0, 0, SRCPAINT));

	if(alpha == 255)
		{
		VERIFY(::BitBlt(hdcDest, nXDest, nYDest, nWidth, nHeight, hDC1, 0, 0, SRCCOPY));
		}
	else
		{
		BLENDFUNCTION m_bf;	INITBLEND(m_bf);

		AlphaBlend(hdcDest, nXDest,nYDest, nWidth, nHeight, hDC1, 0, 0, nWidth, nHeight, m_bf); 	
		}

	// 5. restore/release all objects
	::SelectObject(hDCMask, hOldMaskBitmap);

	::SelectObject(hDC1, hOldBitmap2);
	::SelectObject(hDC2, hOldBitmap3);

	// 6. delete all temp object
	DeleteObject(hBitmap2); 	DeleteObject(hBitmap3);

	DeleteDC(hDC1);	DeleteDC(hDC2);
	DeleteDC(hDCMask);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// Global Variables:

HINSTANCE hInst;						// current instance
TCHAR szTitle[MAX_STRLEN];			// The title bar text
TCHAR szWindowClass[MAX_STRLEN];	// The title bar text
double g_angle = 0; 						// The current rotation angle (rad)
int	  g_degree = 0; 					// The current rotation angle (deg)

//////////////////////////////////////////////////////////////////////////
// Foward declarations of functions included in this code module:

ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);


int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_STRLEN);
	LoadString(hInstance, IDC_GDIROTATE, szWindowClass, MAX_STRLEN);

	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
		{
		return FALSE;
		}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_GDIROTATE);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
		{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
			{
			TranslateMessage(&msg);	DispatchMessage(&msg);
			}
		}
	return msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//

ATOM MyRegisterClass(HINSTANCE hInstance)

{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_GDIROTATE);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_GDIROTATE;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)

{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
		{
		return FALSE;
		}

   // Initialize the Timer to 'delay' delay between screen updates
	int delay = 100;
	SetTimer(hWnd, 10, delay, 0);

	ShowWindow(hWnd, nCmdShow);	UpdateWindow(hWnd);

	return TRUE;
}

// Helper function for getting the minimum of 4 floats
float min4(float a, float b, float c, float d)
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
float max4(float a, float b, float c, float d)
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

// The Representation of a 32 bit color table entry
#pragma pack(push)
#pragma pack(1)
typedef struct ssBGR {
	unsigned char b;
	unsigned char g;
	unsigned char r;
	unsigned char pad;
} sBGR;

typedef sBGR *pBGR;
#pragma pack(pop)


// Returns the DI (Device Independent) bits of the Bitmap
// Here I use 32 bit since it's easy to adress in memory and no
// padding of the horizontal lines is required.

pBGR MyGetDibBits(HDC hdcSrc, HBITMAP hBmpSrc, int nx, int ny)

{
	BITMAPINFO bi;
	BOOL bRes;
	pBGR buf;

	bi.bmiHeader.biSize = sizeof(bi.bmiHeader);
	bi.bmiHeader.biWidth = nx;
	bi.bmiHeader.biHeight = ny;
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biBitCount = 32;
	bi.bmiHeader.biCompression = BI_RGB;
	bi.bmiHeader.biSizeImage = nx * 4 * ny;
	bi.bmiHeader.biClrUsed = 0;
	bi.bmiHeader.biClrImportant = 0;
	
	buf = (pBGR) malloc(nx * 4 * ny);
	bRes = GetDIBits(hdcSrc, hBmpSrc, 0, ny, buf, &bi, DIB_RGB_COLORS);
	if (!bRes) 
		{
		free(buf);
		buf = NULL;
		}
	return buf;
}

//////////////////////////////////////////////////////////////////////////
// Calculate rotated dimentions and normalize result to zero based rect
//

void	GetRotateRC(int SrcW, int SrcH, float angle, RECT *rc)

{
	float x1, x2, x3, x4, y1, y2, y3, y4, cA, sA;
	
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

	rc->left = ((int) floor(min4(x1, x2, x3, x4)));
	rc->top  = ((int) floor(min4(y1, y2, y3, y4)));

	rc->right = ((int) ceil(max4(x1, x2, x3, x4))) ; //- OfX;
	rc->bottom = ((int) ceil(max4(y1, y2, y3, y4))) ; //- OfY;

	//_RPT4( _CRT_WARN, "GetRotateDimR l=%d t=%d r=%d b=%d\r\n", 
	//				rc->left, rc->top, rc->right, rc->bottom);

	// Normalize it:
	if(rc->left != 0)
		{
		rc->right -= rc->left;
		rc->left = 0;
		}
	if(rc->top != 0)
		{
		rc->bottom -= rc->top;
		rc->top = 0;
		}
}


//////////////////////////////////////////////////////////////////////////
// Calculate rotated dimentions
//

void	GetRotateDim(int SrcW, int SrcH, float angle, int &dstW, int &dstH, int &OfX, int &OfY)

{
	float x1, x2, x3, x4, y1, y2, y3, y4, cA, sA;
	

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
	
	dstW = ((int) ceil(max4(x1, x2, x3, x4))) ; //- OfX;
	dstH = ((int) ceil(max4(y1, y2, y3, y4))) ; //- OfY;
}

//////////////////////////////////////////////////////////////////////////
// RotateMemoryDC rotates a memory DC and returns the rotated DC as well 
// as its dimensions

 void RotateMemoryDC(HDC hdcSrc, int SrcW, int SrcH, float angle, HDC hdcDst)

{
	float x1, x2, x3, x4, y1, y2, y3, y4;
	float cA, sA, CtX, CtY;
	float orgX, orgY; //, divisor;

	int dstW, dstH, OfX, OfY, stepX, stepY;
	
#ifdef PERF
	char str[40];
	LARGE_INTEGER lStart, lEnd, lFreq;
	QueryPerformanceFrequency(&lFreq);
	QueryPerformanceCounter(&lStart);
#endif
	
	HBITMAP hBmpDst = (HBITMAP) GetCurrentObject(hdcDst, OBJ_BITMAP);
	ASSERT(hBmpDst);

	HBITMAP hBmpSrc = (HBITMAP) GetCurrentObject(hdcSrc, OBJ_BITMAP);
	ASSERT(hBmpSrc);

	pBGR src, dst, dstLine;
	BITMAPINFO bi;

	// Rotate the bitmap around the center
	CtX = ((float) SrcW) / 2;	CtY = ((float) SrcH) / 2;

	// First, calculate the destination positions for the four courners to get dstW and dstH
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

	// Fill the new memory DC with the current Window color
	//RECT rt; rt.left = 0; rt.top = 0;
	//rt.right = dstW; rt.bottom = dstH;
	//FillRect(hdcDst, &rt, GetSysColorBrush(COLOR_WINDOW));



	//////////////////////////////////////////////////////////////////////////
	// Get the bitmap bits for the source and destination
	src = MyGetDibBits(hdcSrc, hBmpSrc, SrcW, SrcH);
	ASSERT(src);

	dst = MyGetDibBits(hdcDst, hBmpDst, dstW, dstH);
	ASSERT(dst);

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
			//orgX = cA * (curX + CtX * (cA - 1)); 
			//orgX += sA * (curY + CtY * (sA - 1)) / divisor;

			//orgY = CtY + (CtX - curX) * sA;
			//orgY += cA * (curY - CtY + (CtY - CtX) * sA);

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
				//dstLine[dstW - stepX - 1] = src[iorgX + iorgY * SrcW]; ????

				// Correct the black to non transparent
				if(col.b == 0 && col.r == 0 && col.g == 0)
					col.r = 1, col.g = 1, col.b = 1;

				dstLine[stepX] = col; //src[iorgX + iorgY * SrcW];
				} 
			else 
				{
				// Outside the source -> set the color to light grey
				// We preserve the original bits
				//dstLine[dstW - stepX - 1].b = 240;
				//dstLine[dstW - stepX - 1].g = 140;
				//dstLine[dstW - stepX - 1].r = 240;
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
	SetTextColor(hdcDst, RGB(10,10,10));
	TextOut(hdcDst, dstW/2 - 30, dstH/2, str, strlen(str));
#endif

	// Free the color arrays
	free(src), free(dst);
}

// A somewhat presentable semi-random pallete for testing

int rgbarr[17] = { 

	RGB(255,10,10),
	RGB(0,255,0),
	RGB(0,0,255),

	RGB(100,0,0),
	RGB(0,100,0),
	RGB(00,0,100),

	RGB(50,00,0),
	RGB(00,50,0),
	RGB(0,0,50),

	RGB(200,100,200),
	RGB(200,10,10),
	RGB(200,200,200),

	RGB(00,100,255),
	RGB(100,10,50),
	RGB(200,10,20),

	RGB(100,10,100),
	RGB(255,255,0)  // this last one is needed to comp for modulus
};

// --------------------------------------------------------------
//
/// \brief   Handler for drawing 
/// 
/// \details DescHere 
/// \n  
/// Return type:      void 
/// \n  
/// \param hWnd Arg Type: HWND 

void OnDraw(HWND hWnd)

{
	HDC hdscr, hdcDst, hdcNew;
	HBITMAP hBmpNew; HBRUSH hBr;
	int dstW, dstH, horsize = 255, versize = 150;

	RECT rt, rtmp; GetClientRect(hWnd, &rt);
	PAINTSTRUCT ps;	hdscr = BeginPaint(hWnd, &ps); 
	
	// Fill window for testing mask and transparency (solid)
	//HBRUSH bgBr = CreateSolidBrush(RGB(128, 128, 128));

	// Fill window for testing mask and transparency (hatched)
	HBRUSH bgBr = CreateHatchBrush(HS_BDIAGONAL,RGB(128, 128, 128));
	FillRect(hdscr, &rt, bgBr);	DeleteObject(bgBr);

	//////////////////////////////////////////////////////////////////////////
	// First draw the colored bars in the upper left part of the window 
	// that will serve as the starting bitmap to rotate. Also fill in 
	// some text, so rotation mirroring error shows up (if any) 

	int offsetx = 10, offsety = 10;
	int loopstep = horsize / 16;

	rtmp.left = offsetx; rtmp.right = offsetx + loopstep;
	rtmp.top = offsety; rtmp.bottom = versize + offsety;

	for (int loop = 0; loop < horsize; loop += loopstep ) 
		{
		// Fetch colors from array
		hBr = CreateSolidBrush(rgbarr[loop / (horsize / 16)]);

		FillRect(hdscr, &rtmp, hBr);	DeleteObject(hBr);
		rtmp.left += loopstep;	rtmp.right += loopstep;
		}

	char str[48]; 
	_snprintf(str, sizeof(str), "Rotation angle %d ", g_degree);
	TextOut(hdscr, horsize/4, versize/4, str, strlen(str));

	// BitBlt the starting Bitmap into a memory HDC
	hdcNew = CreateCompatibleDC(hdscr); 
	hBmpNew = CreateCompatibleBitmap(hdscr, horsize, versize);
	HBITMAP old2 = (HBITMAP)SelectObject(hdcNew, hBmpNew);

	// Copy original pattern to new HDC
	BitBlt(hdcNew, 0, 0, horsize, versize, hdscr, offsetx, offsety, SRCCOPY);
	
	g_angle =  ((float)(g_degree) / 180) * MATHPI;

	// Get new rotated dimensions
	//GetRotateDim(horsize, versize, g_angle, dstW, dstH, ofx, ofy);
	RECT rc; GetRotateRC(horsize, versize, (float)g_angle, &rc);

	dstW = rc.right; dstH = rc.bottom;

	//_RPT2( _CRT_WARN, "rect dim %d %d \r\n" ,  dstW, dstH);

	// Create the new memory DC
	hdcDst = CreateCompatibleDC(hdscr);
	HBITMAP hBmpDst = CreateCompatibleBitmap(hdscr, dstW, dstH);
	HBITMAP old1 = (HBITMAP)SelectObject(hdcDst, hBmpDst);
	
	// Get original screen for mask effect
	//BitBlt(hdcDst,  0, 0, dstW, dstH, 
	//		hdscr, (rt.right - rt.left)/2 - dstW/2, rt.bottom/2 - dstH/2, SRCCOPY);

	// For testing ...
	//FrameRect(hdcDst, &rt2, GetSysColorBrush(COLOR_WINDOWFRAME));

	// Rotate original to that memory HDC
	RotateMemoryDC(hdcNew, horsize, versize, (float)g_angle, hdcDst);

	//////////////////////////////////////////////////////////////////////////
	// Put it to screen, normal
	//BitBlt(hdscr, (rt.right - rt.left)/4 - dstW/2, rt.bottom/2 - dstH/2, 
	//				dstW, dstH, hdcDst, 0, 0, SRCCOPY);

	//////////////////////////////////////////////////////////////////////////
	// Put it to screen, transparent
	TransparentBlt(hdscr, (rt.right - rt.left)/3 - dstW/2, rt.bottom/2 - dstH/2, 
					dstW, dstH, hdcDst, 0, 0, dstW, dstH, 0);

	//////////////////////////////////////////////////////////////////////
	// Put it to screen, custom transparency

	// Unselect temporarily 
	SelectObject(hdcDst, old1);

	HBITMAP hBmpMask; 
	MakeMaskBmp(&hBmpDst, &hBmpMask, 0, 0, 0);

	//BOOL WINAPI XntMaskBlt(HDC hdcDest, int nXDest, int nYDest, 
	//			int nWidth, int nHeight,
	//				HDC hdcSrc, int nXSrc, int nYSrc,
	//					HBITMAP hbmMask, int xMask, int yMask,
	//						DWORD dwRop)

	// Put it back for masked transfer
	SelectObject(hdcDst, hBmpDst);

	MaskBlt(hdscr, 
			2 * (rt.right - rt.left)/3 - dstW/2, rt.bottom/2 - dstH/2, 
				dstW, dstH, 
					hdcDst, 0, 0, 
						hBmpMask, 0, 0,
							MAKEROP4(SRCCOPY, DSTCOPY), 100);
			
	//////////////////////////////////////////////////////////////////////////

	SelectObject(hdcDst, old1); SelectObject(hdcNew, old2);

	DeleteObject(hBmpDst); DeleteDC(hdcDst);
	DeleteObject(hBmpNew);	DeleteDC(hdcNew);
	
	EndPaint(hWnd, &ps);
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)

{
	int wmId, wmEvent;

	switch (message) 
		{
		case WM_TIMER:
			// Handle the redraw with the timer
			//g_angle = g_angle + .09f;
			//g_angle = g_angle % (2*MATHPI);

			g_degree += 5;
			g_degree %= 360;

			RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
			break;

		case WM_COMMAND:
			wmId    = LOWORD(wParam); wmEvent = HIWORD(wParam); 

			// Parse the menu selections:
			switch (wmId)
				{
				case IDM_ABOUT:
				   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				   break;

				case IDM_EXIT:
				   DestroyWindow(hWnd);
				   break;

				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
				}
			break;

		case WM_PAINT:
			OnDraw(hWnd);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
   return 0;
}

// Mesage handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)

{
	switch (message)
		{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
				{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
				}
			break;
		}

    return FALSE;
}



