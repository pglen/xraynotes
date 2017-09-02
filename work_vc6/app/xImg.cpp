
/* =====[ xImg.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the xImg.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  1/20/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// xImg.cpp: implementation of the CxImg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "xraynotes.h"
#include "xImg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CxImg::CxImg()

{
	//P2N("Constructing CxImg %p\r\n", this);
	magic = IMG_MAGIC;
	loaded = false;

	// Disable dynamic transparency 
	SetTransTresh(255);
}

//////////////////////////////////////////////////////////////////////////

CxImg::~CxImg()

{
}

//////////////////////////////////////////////////////////////////////////
// Used to assert, now runtime check

int CxImg::IsValid()

{
	if(magic != IMG_MAGIC)
		return false;
	else
		return CxImage::IsValid();
}

//////////////////////////////////////////////////////////////////////////

void CxImg::Template()

{
	int tresh = 10;
	int xx, yy;
	RGBQUAD color;
 
	SHORT	rr, gg, bb;
	SHORT	rrr = 0, ggg = 0, bbb = 0;

	P2N("Executing template\r\n");

	for(yy=0; yy < head.biHeight; yy++)
		{
		info.nProgress = (long)(50 * yy / head.biHeight);
		
		if (info.nEscape) break;
	
		for(xx=0; xx < head.biWidth; xx++)
			{
			color = GetPixelColor( xx, yy );
			
			rr = color.rgbRed;
			gg = color.rgbGreen;
			bb = color.rgbBlue;

			if(abs(rrr - rr) < tresh)
				{
				rr = rrr;
				}
			else
				rrr = rr;

			if(abs(ggg - gg) < tresh)
				{
				gg = ggg;
				}
			else
				ggg = gg;

			if(abs(bbb - bb) < tresh)
				{
				bb = bbb;
				}
			else
				bbb = bb;

			//rr = min(rr + 10, 255); 
			//gg = min(gg + 10, 255); 
			//bb = min(bb + 10, 255);

			SetPixelColor(xx, yy, RGBtoRGBQUAD(RGB(rr, gg, bb)));
			}
		}
}

///////////////////////////////////////////////////////////////
// Clip in % of histogram

bool CxImg::HistogramClip(int clip)

{
	if (!pDib) 
		return false;

	int histogram[256];
	int threshold_intensity, intense;
	int x, y, i;
	unsigned int normalize_map[256];
	unsigned int high, low, YVal;

	RGBQUAD color;
	RGBQUAD	yuvClr;

	memset( &histogram, 0, sizeof( int ) * 256 );
	memset( &normalize_map, 0, sizeof( unsigned int ) * 256 );
 
     // form histogram
	for(y=0; y < head.biHeight; y++){
		info.nProgress = (long)(50*y/head.biHeight);
		if (info.nEscape) break;
		for(x=0; x < head.biWidth; x++){
			color = GetPixelColor( x, y );
			YVal = (unsigned int)RGB2GRAY(color.rgbRed, color.rgbGreen, color.rgbBlue);
			histogram[YVal]++;
		}
	}

	// find histogram boundaries by locating the 1 percent levels
	threshold_intensity = clip * ( head.biWidth * head.biHeight) / 100;

	intense = 0;
	for( low = 0; low < 255; low++ ){
		intense += histogram[low];
		if( intense > threshold_intensity )	break;
	}

	intense = 0;
	for( high = 255; high != 0; high--){
		intense += histogram[ high ];
		if( intense > threshold_intensity ) break;
	}

	if ( low == high ){
		// Unreasonable contrast;  use zero threshold to determine boundaries.
		threshold_intensity = 0;
		intense = 0;
		for( low = 0; low < 255; low++){
			intense += histogram[low];
			if( intense > threshold_intensity )	break;
		}
		intense = 0;
		for( high = 255; high != 0; high-- ){
			intense += histogram [high ];
			if( intense > threshold_intensity )	break;
		}
	}
	if( low == high ) return false;  // zero span bound

	// Stretch the histogram to create the normalized image mapping.
	for(i = 0; i <= 255; i++){
		if ( i < (int) low ){
			normalize_map[i] = 0;
		} else {
			if(i > (int) high)
				normalize_map[i] = 255;
			else
				normalize_map[i] = ( 255 - 1) * ( i - low) / ( high - low );
		}
	}

	// Normalize
	if( head.biClrUsed == 0 )
		{
		P2N("RGB image\r\n");
		for( y = 0; y < head.biHeight; y++ ){
			info.nProgress = (long)(50+50*y/head.biHeight);
			if (info.nEscape) break;
			for( x = 0; x < head.biWidth; x++ ){

				color = GetPixelColor( x, y );
				yuvClr = RGBtoYUV( color );

                yuvClr.rgbRed = (BYTE)normalize_map[yuvClr.rgbRed];

				color = YUVtoRGB( yuvClr );
				SetPixelColor( x, y, color );
			}
		}
	} else {
		for(i = 0; i < (int)head.biClrUsed; i++){

			color = GetPaletteColor( (BYTE)i );
			yuvClr = RGBtoYUV( color );

            yuvClr.rgbRed = (BYTE)normalize_map[yuvClr.rgbRed];

			color = YUVtoRGB( yuvClr );
 			SetPaletteColor( (BYTE)i, color );
		}
	}
	return true;
}

//

bool CxImg::Edge2(long Ksize)

{
	if (!pDib) return false;

	long k2 = Ksize/2;
	long kmax= Ksize-k2;
	BYTE r,g,b,rr,gg,bb;
	RGBQUAD c;

	CxImage tmp(*this,pSelection!=0,true,true);
	if (!tmp.IsValid()) return false;

	long xmin,xmax,ymin,ymax;
	if (pSelection){
		xmin = info.rSelectionBox.left; xmax = info.rSelectionBox.right;
		ymin = info.rSelectionBox.bottom; ymax = info.rSelectionBox.top;
	} else {
		xmin = ymin = 0;
		xmax = head.biWidth; ymax=head.biHeight;
	}

	for(long y=ymin; y<ymax; y++){
		info.nProgress = (long)(100*y/head.biHeight);
		if (info.nEscape) break;
		for(long x=xmin; x<xmax; x++){
#if CXIMAGE_SUPPORT_SELECTION
			if (SelectionIsInside(x,y))
#endif //CXIMAGE_SUPPORT_SELECTION
			{
				r=b=g=0;
				rr=bb=gg=255;
				for(long j=-k2;j<kmax;j++){
					for(long k=-k2;k<kmax;k++){
						c=GetPixelColor(x+j,y+k);
						if (c.rgbRed > r) r=c.rgbRed;
						if (c.rgbGreen > g) g=c.rgbGreen;
						if (c.rgbBlue > b) b=c.rgbBlue;

						if (c.rgbRed < rr) rr=c.rgbRed;
						if (c.rgbGreen < gg) gg=c.rgbGreen;
						if (c.rgbBlue < bb) bb=c.rgbBlue;
					}
				}
				c.rgbRed   = 255-abs(r-rr);
				c.rgbGreen = 255-abs(g-gg);
				c.rgbBlue  = 255-abs(b-bb);
				tmp.SetPixelColor(x,y,c);
			}
		}
	}
	Transfer(tmp);
	return true;
}

/*
 * Draw Circle 
 *	 int xx				coordinate
 *	 int yy				coordinate
 *	 int rad			radius of the circle
 *	 COLORREF cr		color of the circle
 *	 BYTE alpha			blending 0=none 255 = full
 *
 */

bool CxImg::DrawCircle(int xx, int yy, int rad, COLORREF cr, BYTE alpha)

{
	P2N("CxImg::DrawCirce(int xx=%d, int yy=%d, int rad=%d, COLORREF cr=%d, BYTE alpha=%d)",
									xx, yy, rad, cr, alpha);

	if (rad == 0) return false;

	COLORREF cr2 = RGB(0,0,0);
	RGBQUAD  qcr = RGBtoRGBQUAD(cr);

	long xcenter = xx;
	long ycenter = yy;
	
	long xmin = max(0L, min(head.biWidth,  xcenter - rad));
	long xmax = max(0L, min(head.biWidth,  xcenter + rad));
	long ymin = max(0L, min(head.biHeight, ycenter - rad));
	long ymax = max(0L, min(head.biHeight, ycenter + rad));

	long y, yo;

	// Lower half
	for (y=ymin; y < ycenter; y++)
		{
		for (long x=xmin; x < xmax; x++) 
			{
			yo = (long)(ycenter - rad * sqrt(1 - pow( (float)(x - xcenter) /(float)rad, 2)));
			if (yo < y) 
			//if (yo == y) 
				{
				if(alpha == 255)
					{
					SetPixelColor(x, y, cr);
					}
				else
					{
					BlendPixelColor(x, y, qcr, (float)alpha/255);
					}
				}			
			}
		}

	// Upper half
	for (y=ycenter;  y < ymax; y++)
		{
		for (long x=xmin; x<xmax; x++)
			{
			yo = (long)(ycenter + rad * sqrt(1-pow((float)(x - xcenter)/(float)rad,2)));
			if (yo>y) 
			//if (yo == y) 
				{
				if(alpha == 255)
					{
					SetPixelColor(x, y, cr);
					}
				else
					{
					BlendPixelColor(x, y, qcr, (float)alpha/255);
					}
				}
			}
		}
	return true;
}

///////////////////////////////////////////////////////////////////////////

void CxImg::Circle(int CenterX, int CenterY, int RADIUS, COLORREF cr, BYTE alpha)

 {
  int X = 0, Y = RADIUS;
  int d = 3 - (2 * RADIUS);

  while(X<=Y)
	{
    SetPixelColor(CenterX + X, CenterY + Y, cr);
    SetPixelColor(CenterX + X, CenterY - Y, cr);
    SetPixelColor(CenterX - X, CenterY + Y, cr);
    SetPixelColor(CenterX - X, CenterY - Y, cr);
    SetPixelColor(CenterX + Y, CenterY + X, cr);
    SetPixelColor(CenterX + Y, CenterY - X, cr);
    SetPixelColor(CenterX - Y, CenterY + X, cr);
    SetPixelColor(CenterX - Y, CenterY - X, cr);
    
	if(d < 0)
        d = d + (4 * X) + 6;
    else
		{
		d = d + 4 * (X - Y) + 10;
		Y = Y - 1;
		}
    X++;
	}
} 

void CxImg::CircleFill(int CenterX, int CenterY, int RADIUS, COLORREF cr, BYTE alpha)

 {
  int X = 0, Y = RADIUS;
  int d = 3 - (2 * RADIUS);

  while(X<=Y)
	{
    SetPixelColor(CenterX + X, CenterY + Y, cr);
    SetPixelColor(CenterX + X, CenterY - Y, cr);
    SetPixelColor(CenterX - X, CenterY + Y, cr);
    SetPixelColor(CenterX - X, CenterY - Y, cr);
    SetPixelColor(CenterX + Y, CenterY + X, cr);
    SetPixelColor(CenterX + Y, CenterY - X, cr);
    SetPixelColor(CenterX - Y, CenterY + X, cr);
    SetPixelColor(CenterX - Y, CenterY - X, cr);
    
	if(d < 0)
        d = d + (4 * X) + 6;
    else
		{
		d = d + 4 * (X - Y) + 10;
		Y = Y - 1;
		}
    X++;
	}
} 



void	CxImg::Circle8(int xCenter, int yCenter, int radius, COLORREF cr,  BYTE alpha)
    
 {
    int x, y, r2;

    r2 = radius * radius;
    
	SetPixelColor(xCenter, yCenter + radius, cr);
    SetPixelColor(xCenter, yCenter - radius, cr);
    SetPixelColor(xCenter + radius, yCenter, cr);
    SetPixelColor(xCenter - radius, yCenter, cr);

    y = radius;
    x = 1;
    y = (int) (sqrt(r2 - 1) + 0.5);
    while (x < y) 
		{
        SetPixelColor(xCenter + x, yCenter + y, cr);
        SetPixelColor(xCenter + x, yCenter - y, cr);
        SetPixelColor(xCenter - x, yCenter + y, cr);
        SetPixelColor(xCenter - x, yCenter - y, cr);
        SetPixelColor(xCenter + y, yCenter + x, cr);
        SetPixelColor(xCenter + y, yCenter - x, cr);
        SetPixelColor(xCenter - y, yCenter + x, cr);
        SetPixelColor(xCenter - y, yCenter - x, cr);
        x += 1;
        y = (int) (sqrt(r2 - x*x) + 0.5);
		}
    if (x == y) 
		{
        SetPixelColor(xCenter + x, yCenter + y, cr);
        SetPixelColor(xCenter + x, yCenter - y, cr);
        SetPixelColor(xCenter - x, yCenter + y, cr);
        SetPixelColor(xCenter - x, yCenter - y, cr);
		}
}

//////////////////////////////////////////////////////////////////////////

bool CxImg::DrawhLine(int xx, int yy, int ww, COLORREF cr, BYTE alpha)

{
	for(int loop = 0; loop < ww; loop++)
		{
		SetPixelColor(xx + loop, yy, cr);
		}

	return 0;
}

///////////////////////////////////////////////////////////////
// Apply new histogram

bool CxImg::HistogramRemap(long *newmap)

{
	if (!pDib) 
		return false;

	//int threshold_intensity, intense;
	int xx, yy, ii;
	//unsigned int normalize_map[256];
	//unsigned int high, low, YVal;

	RGBQUAD color, yuvClr;
	
	// Normalize
	if( head.biClrUsed == 0 )
		{
		//P2N("RGB image\r\n");

		for( yy = 0; yy < head.biHeight; yy++ )
			{
			info.nProgress = (long)(100 * yy / head.biHeight);
			if (info.nEscape) break;

			for( xx = 0; xx < head.biWidth; xx++)
				{
				color = GetPixelColor( xx, yy);
				yuvClr = RGBtoYUV(color);

                //yuvClr.rgbRed = (BYTE) min(255, newmap[yuvClr.rgbRed]);
				yuvClr.rgbRed = (BYTE)newmap[yuvClr.rgbRed];

				color = YUVtoRGB( yuvClr );
				SetPixelColor( xx, yy, color );
				}
			}
		} 
	else 
		{
		for(ii = 0; ii < (int)head.biClrUsed; ii++)
			{
			color = GetPaletteColor( (BYTE)ii );
			yuvClr = RGBtoYUV( color );

            yuvClr.rgbRed = (BYTE)newmap[yuvClr.rgbRed];

			color = YUVtoRGB( yuvClr );
 			SetPaletteColor( (BYTE)ii, color );
			}
		}
	return true;
}

//////////////////////////////////////////////////////////////////////////
// Scale to perspective
//
//	Return TRUE if ok, false otherwise
//

int CxImg::Perspective(int pwidth, int pfarhh, CxImage* iDst)

{
	P2N("Perspective on image  ww=%d hh=%d  - pwidth=%d, pfarhh=%d\r\n", 
			 head.biWidth, head.biHeight, pwidth, pfarhh);

	CxImage tmp(pwidth, head.biHeight, head.biBitCount, info.dwType);
	if (!tmp.IsValid()) return false;
	tmp.SetPalette(GetPalette(), head.biClrUsed);

	float xScale, yScale1, yScale2, scalediff, fX, fY, fY2;
	xScale = (float)head.biWidth  / (float)pwidth;
	yScale2 = (float)head.biHeight / (float)pfarhh;
	yScale1 = 1;

	int hhh = tmp.GetHeight(), www = tmp.GetWidth(), xx, yy, yy2 = hhh/2;
	scalediff = (yScale2 - yScale1) / www;

	P2N("xScale=%f Scale2 %f scaledif %f\r\n", xScale, yScale2, scalediff);

	//if( head.biClrUsed == 0 )
		{
		//P2N("RGB image\r\n");

		//
		//		| 
		//		| |
		//	---	| |	pfarhh
		//		| |
		//		| 
		//        pwidth

		for( yy = 0; yy < hhh; yy++ )
			{
			//info.nProgress = (long)(100 * yy / head.biHeight);

			if (info.nEscape) 
				break;
	
			yScale1 = 0;

			for( xx = 0; xx < www; xx++)
				{
				fX = xx * xScale;
				fY = yy * yScale1;
				fY2 = yy2 * yScale1;

				yScale1 += scalediff;

				RGBQUAD color  = GetPixelColor((long)fX, (long)fY);

				///RGBQUAD color2 = GetPixelColor((long)fX, (long)fY2);
				//RGBQUAD color = GetPixelColor((long)fX, (long)yy);

				tmp.SetPixelColor( xx, yy,  color );
				//tmp.SetPixelColor( xx, yy2, color2 );
				}

			yScale1 += scalediff;
			}  // end for

		} 

	if (iDst) 
		iDst->Transfer(tmp);
    else 
		Transfer(tmp);

	return 0;
}

unsigned char * CxImg::GetPixPtr()

{
	//P2N("img ptr=%p\r\n", info.pImage);

	return info.pImage;
}

int CxImg::GetBuffSize()

{
	//P2N("info.dwEffWidth=%d  head.biHeight=%d head.biWidth=%d\r\n", 
	//			info.dwEffWidth, head.biHeight, head.biWidth);

	//P2N("img size=%d\r\n", info.dwEffWidth * head.biHeight);

	return info.dwEffWidth * head.biHeight;
}

int CxImg::GetEffWidth()

{
	return info.dwEffWidth;
}

int CxImg::GetBitsPpix()

{
	return head.biBitCount;
}

////////////////////////////////////////////////////////////////////////////////

RGBQUAD inline CxImg::InlinePixColor(long x,long y, bool bGetAlpha)

{
//	RGBQUAD rgb={0,0,0,0};

	RGBQUAD rgb=info.nBkgndColor; //<mpwolski>

	if ((pDib==NULL)||(x<0)||(y<0)||
		(x>=head.biWidth)||(y>=head.biHeight)){
		if (info.nBkgndIndex != -1){
			if (head.biBitCount<24) return GetPaletteColor((BYTE)info.nBkgndIndex);
			else return info.nBkgndColor;
		} else if (pDib) return GetPixelColor(0,0);
		return rgb;
	}

	if (head.biClrUsed)
		{
		rgb = GetPaletteColor(GetPixelIndex(x,y));
		} 
	else 
		{
		BYTE* iDst  = info.pImage + y*info.dwEffWidth + x*3;
		rgb.rgbBlue = *iDst++;
		rgb.rgbGreen= *iDst++;
		rgb.rgbRed  = *iDst;
		}

#if CXIMAGE_SUPPORT_ALPHA
	if (pAlpha && bGetAlpha) rgb.rgbReserved = AlphaGet(x,y);
#else
	rgb.rgbReserved = 0;
#endif //CXIMAGE_SUPPORT_ALPHA
	return rgb;
}

bool	CxImg::Load(const TCHAR* filename, DWORD imagetype)

{
	P2N("Loading %s \r\n", filename);

	bool ret = CxImage::Load(filename, imagetype);

	SetTransTresh(255);
	return ret;
}
	