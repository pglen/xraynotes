
/* =====[ maskblt.h ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the maskblt.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  3/2/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#if !defined(AFX_XNTMASK_H__AF6E6E3E_1A1D_4D98_8A70_467E93D2BFE3__INCLUDED_)
#define AFX_XNTMASK_H__AF6E6E3E_1A1D_4D98_8A70_467E93D2BFE3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define		DSTCOPY		0x00AA0029
#define		DSTERASE	0x00220326 // dest = dest & (~src) : DSna

typedef struct _bits
	{
	unsigned bit0 : 1;
	unsigned bit1 : 1;
	unsigned bit2 : 1;
	unsigned bit3 : 1;

	unsigned bit4 : 1;
	unsigned bit5 : 1;
	unsigned bit6 : 1;
	unsigned bit7 : 1;
	} BITS;

#define		FORE_ROP3(ROP4)        (0x00FFFFFF&(ROP4))
#define		BACK_ROP3(ROP4)        (ROP3FromIndex \
				(SwapROP3_SrcDst(BYTE((ROP4)>>24))))


void MakeMaskBitmap(
	CBitmap* pBmpSource, CBitmap* pBmpMask,
	COLORREF clrpTransColor, // Pass 0xFFFFFFFF if unknown
	int iTransPixelX = 0,
	int iTransPixelY = 0 );


BOOL WINAPI XntMaskBlt(HDC hdcDest, int nXDest, int nYDest, 
				int nWidth, int nHeight,
					HDC hdcSrc, int nXSrc, int nYSrc,
						HBITMAP hbmMask, int xMask, int yMask,
							DWORD dwRop );


#endif