
/* =====[ XntMask.h ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the XntMask.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  4/2/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#if !defined(AFX_XNTMASK_H__97924157_E860_47C4_BB27_3F6060012721__INCLUDED_)
#define AFX_XNTMASK_H__97924157_E860_47C4_BB27_3F6060012721__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define	DSTCOPY		0x00AA0029
#define	DSTERASE	0x00220326 // dest = dest & (~src) : DSna
#define	DEFAULTROP	MAKEROP4(SRCCOPY, DSTCOPY)

#ifndef INITBLEND

#define INITBLEND(aa, bb)							\
		aa.BlendOp	= AC_SRC_OVER;					\
		aa.BlendFlags = 0;							\
		aa.SourceConstantAlpha = min(bb, 255);		\
		aa.AlphaFormat = 0;							\

#endif

#define		FORE_ROP3(ROP4)        (0x00FFFFFF&(ROP4))

#define		BACK_ROP3(ROP4)        (ROP3FromIndex	\
				(SwapROP3_SrcDst(BYTE((ROP4)>>24))))

//////////////////////////////////////////////////////////////////////////

class XntMask  

{	
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

public:

	XntMask();
	virtual ~XntMask();

	HBITMAP	MakeMaskBmpDC(HDC hdcSrc, int ww, int hh, COLORREF clrpTransColor);

	HBITMAP	MakeMaskBmp(HBITMAP* pBmpSource, COLORREF TransCol = 0, 
								int iTransPixX = 0,  int iTransPixY = 0);

	void	MakeMaskBmp(HBITMAP* pBmpSource, HBITMAP* hBmpMask,
							COLORREF clrpTransColor = 0, 
								int iTransPixelX = 0,  int iTransPixelY = 0);

	BOOL	MaskBlt(HDC hdcDest, int nXDest, int nYDest, 
				int nWidth, int nHeight,
					HDC hdcSrc, int nXSrc, int nYSrc,
						HBITMAP hbmMask, int xMask, int yMask,
							 DWORD dwRop = DEFAULTROP, int alpha = 255);

private:

	DWORD	ROP3FromIndex(DWORD dwIndex);
	BYTE	SwapROP3_SrcDst(BYTE bRop3);

};

#endif // !defined(AFX_XNTMASK_H__97924157_E860_47C4_BB27_3F6060012721__INCLUDED_)
