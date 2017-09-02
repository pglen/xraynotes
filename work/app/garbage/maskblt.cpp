
/* =====[ maskblt.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the maskblt.cpp                
                                                                             
                    Transparent blitting.
					
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  3/2/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#include "Stdafx.h"
#include "xraynotes.h"
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

BOOL WINAPI XntMaskBlt(HDC hdcDest, int nXDest, int nYDest, 
				int nWidth, int nHeight,
					HDC hdcSrc, int nXSrc, int nYSrc,
						HBITMAP hbmMask, int xMask, int yMask,
							DWORD dwRop)

{
	P2N("XntMaskBlt %d %d\r\n", nWidth, nHeight);

	if ( hbmMask == NULL )
		{
		//P2N("XntMaskBlt NULL mask\r\n");

		return BitBlt(hdcDest, nXDest, nYDest, nWidth, nHeight, hdcSrc, 
					  nXSrc, nYSrc, FORE_ROP3(dwRop));
		}

	// 1. make mask bitmap's dc
	HDC hDCMask = ::CreateCompatibleDC(hdcDest);
	HBITMAP hOldMaskBitmap = (HBITMAP)::SelectObject(hDCMask, hbmMask);
	if(!hOldMaskBitmap)
		P2N("NULL hOldMaskBitmap %d %s\r\n", errno, strerror(errno));

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
	::BitBlt(hDC2, 0, 0, nWidth, nHeight, hdcDest, nXDest, nYDest, SRCCOPY);
	::BitBlt(hDC2, 0, 0, nWidth, nHeight, hdcSrc, nXSrc, nYSrc, dwRop3);
	::BitBlt(hDC2, 0, 0, nWidth, nHeight, hDCMask, xMask, yMask, SRCAND);


	// 4. combine two bitmaps and copy it to hdcDest
	::BitBlt(hDC1, 0, 0, nWidth, nHeight, hDC2, 0, 0, SRCPAINT);
	::BitBlt(hdcDest, nXDest, nYDest, nWidth, nHeight, hDC1, 0, 0, SRCCOPY);

	// 5. restore all objects
	::SelectObject(hDCMask, hOldMaskBitmap);

	::SelectObject(hDC1, hOldBitmap2);
	::SelectObject(hDC2, hOldBitmap3);

	// 6. delete all temp object
	DeleteObject(hBitmap2);
	DeleteObject(hBitmap3);

	DeleteDC(hDC1);	DeleteDC(hDC2);
	DeleteDC(hDCMask);

	return TRUE;
}


void MakeMaskBitmap(
	CBitmap* pBmpSource, CBitmap* pBmpMask,
	COLORREF clrpTransColor, // Pass 0xFFFFFFFF if unknown
	int iTransPixelX,      // = 0
	int iTransPixelY       // = 0
)
{
	BITMAP bm;

	// Get the dimensions of the source bitmap
	pBmpSource->GetObject(sizeof(BITMAP), &bm);

	// We will need two DCs to work with. One to hold the Image
	// (the source), and one to hold the mask (destination).
	// When blitting onto a monochrome bitmap from a color, pixels
	// in the source color bitmap that are equal to the background
	// color are blitted as white. All the remaining pixels are
	// blitted as black.

	CDC hdcSrc, hdcDst;

	hdcSrc.CreateCompatibleDC(NULL);
	hdcDst.CreateCompatibleDC(NULL);

	//pBmpMask->CreateBitmap( bm.bmWidth, bm.bmHeight, 1, 1, NULL);
	// Create the mask bitmap
	//pBmpMask->DeleteObject();

	pBmpMask->CreateBitmap( bm.bmWidth, bm.bmHeight, 1, 1, NULL);

	// Load the bitmaps into memory DC
	CBitmap* hbmSrcT = (CBitmap*) hdcSrc.SelectObject(pBmpSource);
	CBitmap* hbmDstT = (CBitmap*) hdcDst.SelectObject(pBmpMask);

	// Dynamically get the transparent color
	COLORREF clrTrans;
	if (clrpTransColor == 0xFFFFFFFF)
	{
		// User did not specify trans color so get it from bmp
		clrTrans = hdcSrc.GetPixel(iTransPixelX, iTransPixelY);
	}
	else
	{
		clrTrans = clrpTransColor;
	}

	// Change the background to trans color
	COLORREF clrSaveBk  = hdcSrc.SetBkColor(clrTrans);

	// This call sets up the mask bitmap.
	hdcDst.BitBlt(0,0,bm.bmWidth, bm.bmHeight, &hdcSrc, 0, 0, NOTSRCCOPY);

	// Now, we need to paint onto the original image, making
	// sure that the "transparent" area is set to black. What
	// we do is AND the monochrome image onto the color Image
	// first. When blitting from mono to color, the monochrome
	// pixel is first transformed as follows:
	// if  1 (black) it is mapped to the color set by SetTextColor().
	// if  0 (white) is is mapped to the color set by SetBkColor().
	// Only then is the raster operation performed.

	COLORREF clrSaveDstText = hdcSrc.SetTextColor(RGB(255,255,255));
	hdcSrc.SetBkColor(RGB(0,0,0));

#if	0
	hdcSrc.BitBlt(0,0,bm.bmWidth, bm.bmHeight, &hdcDst,0,0,SRCAND);
#endif

	// Clean up by deselecting any objects, and delete the
	// DC's.
	hdcDst.SetTextColor(clrSaveDstText);

	hdcSrc.SetBkColor(clrSaveBk);
	hdcSrc.SelectObject(hbmSrcT);
	hdcDst.SelectObject(hbmDstT);

	hdcSrc.DeleteDC();
	hdcDst.DeleteDC();

	pBmpMask->GetObject(sizeof(BITMAP), &bm);
}
