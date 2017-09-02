// xImageDCM.cpp: implementation of the CxImageDCM class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "DCMConverter.h"
#include "xImageDCM.h"


#include "osconfig.h"    /* make sure OS specific configuration is included first */


#ifdef HAVE_GUSI_H
#include <GUSI.h>
#endif

#include "dctk.h"          /* for various dcmdata headers */
#include "dcdebug.h"       /* for SetDebugLevel */
#include "cmdlnarg.h"      /* for prepareCmdLineArgs */
#include "dcuid.h"         /* for dcmtk version name */
#include "dcrledrg.h"      /* for DcmRLEDecoderRegistration */

#include "dcmimage.h"      /* for DicomImage */
#include "digsdfn.h"       /* for DiGSDFunction */
#include "diciefn.h"       /* for DiCIELABFunction */

#include "ofconapp.h"      /* for OFConsoleApplication */
#include "ofcmdln.h"       /* for OFCommandLine */

#include "diregist.h"      /* include to support color images */
#include "ofstd.h"         /* for OFStandard */

#include "djdecode.h"     /* for dcmjpeg decoders */
#include "dipijpeg.h"     /* for dcmimage JPEG plugin */
#include "dipitiff.h"     /* for dcmimage TIFF plugin */
#include "dipipng.h"      /* for dcmimage PNG plugin */
#include "zlib.h"         /* for zlibVersion() */

#include "ofstream.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CxImageDCM::CxImageDCM()
{
	//init pointers
	pDib = pSelection = pAlpha = NULL;
	pLayers = NULL;
	//init structures
	memset(&head,0,sizeof(BITMAPINFOHEADER));
	memset(&info,0,sizeof(CXIMAGEINFO));
	//init default attributes
    info.dwType = 0;
	info.nQuality = 90;
	info.nAlphaMax = 255;
	info.nBkgndIndex = -1;
	info.bEnabled = true;
	SetXDPI(96);
	SetYDPI(96);
}

CxImageDCM::~CxImageDCM()
{ 

}

bool CxImageDCM::SaveAsBMP(const TCHAR *fileName)
{//please refer to the implementation of dcmj2pnm
	return false;
}

bool CxImageDCM::SaveAsJPG(const TCHAR* fileName)
{//you may also use DCMTK's JPG encoding plug-in
	return CxImage::Save(fileName,CXIMAGE_FORMAT_JPG);
}

bool CxImageDCM::LoadDCM(const TCHAR* filename)
{  
	DcmFileFormat *dfile = new DcmFileFormat();
    OFCondition cond = dfile->loadFile(filename, EXS_Unknown,
		EGL_withoutGL,DCM_MaxReadLength,OFFalse);
	
	if (cond.bad()) {
		AfxMessageBox(cond.text());
    }
	
	E_TransferSyntax xfer = dfile->getDataset()->getOriginalXfer();
    DicomImage *di = new DicomImage(dfile, xfer, 
		CIF_AcrNemaCompatibility ,
		0, 1);
	
	if (di->getStatus() != EIS_Normal)
        AfxMessageBox(DicomImage::getString(di->getStatus()));
	
	di->writeBMP("c:\\from_dicom.bmp",24);
	
	return CxImage::Load("c:\\from_dicom.bmp",CXIMAGE_FORMAT_BMP);
	
}

bool CxImageDCM::SaveAsDCM(const TCHAR* filename)
{
	CxImageDCM::IncreaseBpp(24);
	char uid[100]; 
	DcmFileFormat fileformat; 
	DcmDataset *dataset = fileformat.getDataset(); 
	dataset->putAndInsertString(DCM_SOPClassUID, 
		UID_SecondaryCaptureImageStorage); 
	dataset->putAndInsertString(DCM_SOPInstanceUID, 
		dcmGenerateUniqueIdentifier(uid, SITE_INSTANCE_UID_ROOT)); 
	dataset->putAndInsertString(DCM_PatientsName, "Doe^John");
		
//	dataset->putAndInsertUint32(DCM_MetaElementGroupLength,128);
	dataset->putAndInsertUint16(DCM_FileMetaInformationVersion,
		0x0001);
	dataset->putAndInsertString(DCM_MediaStorageSOPClassUID,
		UID_MultiframeTrueColorSecondaryCaptureImageStorage);
	dataset->putAndInsertString(DCM_MediaStorageSOPInstanceUID,
		dcmGenerateUniqueIdentifier(uid,SITE_INSTANCE_UID_ROOT));
	dataset->putAndInsertString(DCM_TransferSyntaxUID,
		UID_LittleEndianExplicitTransferSyntax);
	dataset->putAndInsertString(DCM_ImplementationClassUID,
		dcmGenerateUniqueIdentifier(uid,SITE_INSTANCE_UID_ROOT));
	dataset->putAndInsertString(DCM_ImplementationVersionName,
		"test this version name");

	dataset->putAndInsertString(DCM_UID,
		UID_MultiframeTrueColorSecondaryCaptureImageStorage);
	dataset->putAndInsertString(DCM_PhotometricInterpretation,
		"RGB"); 
	dataset->putAndInsertUint16(DCM_SamplesPerPixel,3);
	dataset->putAndInsertUint16(DCM_BitsAllocated,8);
	dataset->putAndInsertUint16(DCM_BitsStored,8); 
	dataset->putAndInsertUint16(DCM_HighBit,7);
	dataset->putAndInsertUint16(DCM_PixelRepresentation,0);
	dataset->putAndInsertUint16(DCM_PlanarConfiguration,0);
	dataset->putAndInsertUint16(DCM_Rows,GetHeight());
	dataset->putAndInsertUint16(DCM_Columns,GetWidth());
	//add more tags here
	/* ... */ 
	BYTE* pData=new BYTE[GetHeight()*info.dwEffWidth];
	BYTE* pSrc=GetBits(head.biHeight-1);
	BYTE* pDst=pData;
	for(long y=0; y < head.biHeight; y++){
		memcpy(pDst,pSrc,info.dwEffWidth);
		pSrc-=info.dwEffWidth;
		pDst+=info.dwEffWidth;
	}
	dataset->putAndInsertUint8Array(DCM_PixelData, 
		pData, GetHeight()*info.dwEffWidth); 
	delete[] pData;

	OFCondition status = fileformat.saveFile(filename, 
		EXS_LittleEndianImplicit,EET_UndefinedLength,EGL_withoutGL); 
	if (status.bad()) 
		AfxMessageBox("Error: cannot write DICOM file ");

	return true;     
}