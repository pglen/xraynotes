// DicomFile.h: interface for the DicomFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DICOMFILE_H__BDB3EB54_433F_42BA_8F32_0AC577B609F2__INCLUDED_)
#define AFX_DICOMFILE_H__BDB3EB54_433F_42BA_8F32_0AC577B609F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define		HEAD_OFFSET		128
#define		DATA_OFFSET		(HEAD_OFFSET + 4)

#define		MAX_FRAMES		1000

#include "..\\CxImage\\ximage.h"

class DicomFile  
{

typedef struct _element

	{
	WORD group;
	WORD element;
	char vr1, vr2;
	WORD len;

	} element;

typedef struct _obelement

{
	WORD group;
	WORD element;
	char vr1, vr2;
	WORD reserved;
	DWORD len;

} obelement;

typedef struct _imelement

{
	WORD group;
	WORD element;
	DWORD len;

} imelement;


public:
	void Dump();

	CFile	fp;
	CString	filename;

	void	*mem;

	CString	StorageUID;
	CString	TransferUID;
	CString	Manufacturer;
	CString	DevSerial;
	CString	SoftVer;
	CString	PhotoMetric;
	
	int		enabled;
	int		frames, buffers;
	int		rows, cols;
	int		bitalloc, bitstore, highbit;
	int		samperpix;
	int		wincenter, winwidth;
	int		start_base, start_width;
	int		reenter;
	int		newrows;
	int		changed;

	char	buffx[128];
	char	buff[1024];
	
	void	*framearr[MAX_FRAMES];
	int		lengtharr[MAX_FRAMES];

	DicomFile();
	virtual ~DicomFile();

	int		DicomFile::Open(const char *fname);
	int		DicomFile::ToImage(CxImage *img, int ll, int ul);
	//int		DicomFile::Histogram(int *wincenter, int *winwidth);
	int		DicomFile::Histogram(int *wincenter, int *winwidth, int showdlg = false);

protected:

	int		DicomFile::Read2Mem(int len, void **ptr);
	void	DicomFile::Interpret(void *ptr, void *buff2);
	
};

#endif // !defined(AFX_DICOMFILE_H__BDB3EB54_433F_42BA_8F32_0AC577B609F2__INCLUDED_)
