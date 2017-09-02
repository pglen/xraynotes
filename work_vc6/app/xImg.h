// xImg.h: interface for the CxImg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XIMG_H__5130A71B_1E80_4BFD_92E4_EDD09C1DE487__INCLUDED_)
#define AFX_XIMG_H__5130A71B_1E80_4BFD_92E4_EDD09C1DE487__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\\CxImage\\ximage.h"

#define IMG_MAGIC	0x34567

#define	 CHECKIMG(dd) ASSERT(dd->magic == IMG_MAGIC);


class CxImg : public CxImage  
{
	
public:

	RGBQUAD inline CxImg::InlinePixColor(long x,long y, bool bGetAlpha);

	bool	Load(const TCHAR* filename, DWORD imagetype=0);
	
	int		GetEffWidth();
	int		GetBuffSize();
	int		GetBitsPpix();
	unsigned char * GetPixPtr();

	int		Perspective(int pwidth, int pfarhh, CxImage* iDst = NULL);
	void	Template();
	int		IsValid();
	bool	Edge2(long Ksize = 2);

	bool	HistogramClip(int clip = 1);
	bool	DrawCircle(int xx, int yy, int rad, COLORREF cr, BYTE alpha = 255);
	void	Circle(int CenterX, int CenterY, int RADIUS, COLORREF cr, BYTE alpha = 255);
	void	Circle8(int xCenter, int yCenter, int radius, COLORREF cr,  BYTE alpha = 255);
	bool	DrawhLine(int xx, int yy, int ww, COLORREF cr, BYTE alpha = 255);
	void	CircleFill(int CenterX, int CenterY, int RADIUS, COLORREF cr, BYTE alpha);
	bool	HistogramRemap(long *newmap);
	
	int		magic, loaded;

	CxImg();
	virtual ~CxImg();

};

#endif // !defined(AFX_XIMG_H__5130A71B_1E80_4BFD_92E4_EDD09C1DE487__INCLUDED_)
