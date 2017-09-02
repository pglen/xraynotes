
/* =====[ GdiRot.h ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the GdiRot.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  4/2/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#if !defined(AFX_GDIROT_H__A3B34995_A380_428F_B56E_4BEE389B64A8__INCLUDED_)
#define AFX_GDIROT_H__A3B34995_A380_428F_B56E_4BEE389B64A8__INCLUDED_


//////////////////////////////////////////////////////////////////////////
// The Representation of a 32 bit color table entry

#pragma pack(push)
#pragma pack(1)

typedef struct ssBGR 

	{
	unsigned char bb;
	unsigned char gg;
	unsigned char rr;
	unsigned char pad;
	} sBGR;

typedef sBGR *pBGR;
#pragma pack(pop) 

//////////////////////////////////////////////////////////////////////////
// Helper macro to define a 32 bit bitmap

#define DECL32BMPH(bi, nx, ny)						\
	bi.bmiHeader.biSize = sizeof(bi.bmiHeader);		\
	bi.bmiHeader.biWidth = nx;						\
	bi.bmiHeader.biHeight = ny;						\
	bi.bmiHeader.biPlanes = 1;						\
	bi.bmiHeader.biBitCount = 32;					\
	bi.bmiHeader.biCompression = BI_RGB;			\
	bi.bmiHeader.biSizeImage = nx * 4 * ny;			\
	bi.bmiHeader.biClrUsed = 0;						\
	bi.bmiHeader.biClrImportant = 0;				\


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGdiRot  
{


public:

	CGdiRot();
	virtual ~CGdiRot();
	
	void GetRotateDim(int SrcW, int SrcH, double angle, int &dstW, int &dstH);
	void GetRotateRC(int SrcW, int SrcH, float angle, RECT *rc);

	//void CGdiRot::RotateMemoryDC(HDC hdcSrc, int SrcW, int SrcH, double angle, HDC hdcDst);
	void RotateMemoryDC(HDC hdcSrc, int SrcW, int SrcH, float angle, HDC hdcDst);

protected:

	pBGR MyGetDibBits(HDC hdcSrc, HBITMAP hBmpSrc, int nx, int ny);

private:

	float min4(float a, float b, float c, float d);
	float max4(float a, float b, float c, float d);

	float max4v(float a, float b, float c, float d);
	float min4v(float a, float b, float c, float d);
};

#endif // !defined(AFX_GDIROT_H__A3B34995_A380_428F_B56E_4BEE389B64A8__INCLUDED_)
