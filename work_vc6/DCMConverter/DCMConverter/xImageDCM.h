// xImageDCM.h: interface for the CxImageDCM class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XIMAGEDCM_H__4A7A1D0F_2FB5_46D6_AD86_75B1E8528608__INCLUDED_)
#define AFX_XIMAGEDCM_H__4A7A1D0F_2FB5_46D6_AD86_75B1E8528608__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ximage.h"

class CxImageDCM : public CxImage  
{
public:
	bool SaveAsBMP(const TCHAR* fileName);
	bool SaveAsJPG(const TCHAR* fileName);
	CxImageDCM();
	virtual ~CxImageDCM();

	bool LoadDCM(const TCHAR* filename);
	bool SaveAsDCM(const TCHAR* filename);	
	//

};

#endif // !defined(AFX_XIMAGEDCM_H__4A7A1D0F_2FB5_46D6_AD86_75B1E8528608__INCLUDED_)
