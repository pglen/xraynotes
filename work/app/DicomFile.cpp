
/* =====[ DicomFile.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the DicomFile.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  10/31/2007  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// DicomFile.cpp: implementation of the DicomFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "xraynotes.h"
#include "DicomFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DicomFile::DicomFile()

{
	frames = buffers = 0;
	rows = cols = 0;
	bitalloc = bitstore = highbit = 0;
	wincenter = winwidth = 0;
	samperpix = 0;
	newrows = 0;
	changed = 0;
	enabled = true;

	memset(framearr, 0, sizeof(framearr));
	memset(lengtharr, 0, sizeof(lengtharr));
	
	reenter = false;
	mem = NULL;
}

DicomFile::~DicomFile()

{
	for(int loop = 0; loop < buffers; loop++)
		{
		//P2N("Freeing lenght=%d, ptr=%p\r\n", lengtharr[loop], framearr[loop]);
		if(framearr[loop])
			free(framearr[loop]);
		}
	free(mem);
}

//////////////////////////////////////////////////////////////////////////
//

void	DicomFile::Interpret(void *ptr, void *buff2)

{
	element *ptr3 = (element *)ptr;
	const char *buff = (const char *)buff2;

	if(ptr3->group == 0x2 && ptr3->element == 0x2)
		{
		//P2N("Storage class UID: %s\r\n", buff);
		StorageUID = buff;
		}

	if(ptr3->group == 0x2 && ptr3->element == 0x10)
		{
		//P2N("Transfer UID: %s\r\n", buff);
		TransferUID = buff;
		}

	if(ptr3->group == 0x8 && ptr3->element == 0x70)
		{
		//P2N("Manufacturer: %s\r\n", buff);
		Manufacturer = buff;
		}
	
	if(ptr3->group == 0x18 && ptr3->element == 0x1000)
		{
		//P2N("Device serial: %s\r\n", buff);
		DevSerial = buff;
		}
	if(ptr3->group == 0x18 && ptr3->element == 0x1020)
		{
		//P2N("Software version: %s\r\n", buff);
		SoftVer = buff;
		}

	if(ptr3->group == 0x28 && ptr3->element == 0x2)
		{
		//P2N("Samples per pix: %d\r\n", *((WORD *)buff));
		samperpix = (int) *((WORD *)buff);
		}

	if(ptr3->group == 0x28 && ptr3->element == 0x4)
		{
		//P2N("Photometric: %s\r\n", buff);
		PhotoMetric = buff;
		}

	if(ptr3->group == 0x28 && ptr3->element == 0x8)
		{
		//P2N("Number of frames: %s\r\n", buff);
		frames = atoi(buff);
		}

	if(ptr3->group == 0x28 && ptr3->element == 0x10)
		{
		//P2N("Rows: %d\r\n", *((WORD *)buff));
		rows = (int) *((WORD *)buff);
		}

	if(ptr3->group == 0x28 && ptr3->element == 0x11)
		{
		//P2N("Cols: %d\r\n", *((WORD *)buff));
		cols = (int) *((WORD *)buff);
		}

	if(ptr3->group == 0x28 && ptr3->element == 0x100)
		{
		//P2N("Bits alloc: %d\r\n", *((WORD *)buff));
		bitalloc = (int) *((WORD *)buff);
		}

	if(ptr3->group == 0x28 && ptr3->element == 0x101)
		{
		//P2N("Bits stored: %d\r\n", *((WORD *)buff));
		bitstore = (int) *((WORD *)buff);
		}

	if(ptr3->group == 0x28 && ptr3->element == 0x102)
		{
		//P2N("High bit: %d\r\n", *((WORD *)buff));
		highbit = (int) *((WORD *)buff);
		}

	if(ptr3->group == 0x28 && ptr3->element == 0x103)
		{
		//P2N("Representation: %d\r\n", *((WORD *)buff));
		}

	if(ptr3->group == 0x28 && ptr3->element == 0x1050)
		{
		start_base = wincenter = (int)atof(buff);
		//P2N("Window Center: %d\r\n", wincenter);
		}

	if(ptr3->group == 0x28 && ptr3->element == 0x1051)
		{
		start_width = winwidth = (int)atof(buff);
		//P2N("Window idth %d\r\n", winwidth);
		}
		
	return;
}

//////////////////////////////////////////////////////////////////////////
// Return true for OK

int		DicomFile::Open(const char *fname)

{
	int inpix = false, gotx;

	//P2N("DicomFile::Open: '%s'\r\n", fname);
 
	if(!fp.Open(fname,  CFile::modeRead, NULL))
		{
		P2N("DicomFile::Open - Cannot open file '%s' %s\r\n", fname, support.Err2Str());
		return 0;
		}

	filename = fname;
	ULONGLONG flen = fp.GetLength();

	//P2N("File len %d\r\n", flen);
	//gotx = fp.Read(buff, sizeof(buff));
	//D2N(buff, gotx);
	//fp.Seek(0, CFile::begin);

	gotx = fp.Read(buff, DATA_OFFSET);

	//P2N("buffer check %c %c %c %c\r\n", 
	//	buff[HEAD_OFFSET],  buff[HEAD_OFFSET+1],  buff[HEAD_OFFSET+2],  buff[HEAD_OFFSET+3]);

	if(strncmp(&buff[HEAD_OFFSET], "DICM", 4))
		{
		//MBOX("Not a dicom file, trying zero offset");
		//P2N("Direct dicom file, trying zero offset.\r\n");
		fp.Seek(0, CFile::begin);
		}
	else
		{
		//P2N("DICOM file\r\n");
		}

	////////////////////////////////////////////////////////
	int cnt = 1000, cnt2 = 0;

	while(true)
		{
		element *ptr = (element *)buffx;

		ULONGLONG old = fp.Seek(0, CFile::current);

		memset(buffx, 0, sizeof(buffx));
		memset(buff, 0, sizeof(buff));

		if(!fp.Read(buffx, sizeof(element)))
			{
			//P2N("At EOF\r\n");
			break;
			}

		if(ptr->group == 0xfffe)
			{
			imelement *ptr4 = (imelement *)buffx;

			fp.Seek(old, CFile::begin);
			fp.Read(buffx, sizeof(imelement));

			P2N("listgroup: (%04x,%04x) %d length: %d %d\r\n", 
						ptr4->group, ptr4->element, cnt2++, ptr4->len, old);

			//Read2Mem(ptr4->len, &mem);

			lengtharr[buffers] = ptr4->len;
			Read2Mem(lengtharr[buffers], &framearr[buffers]);
			buffers++;
			}
		else if(ptr->group == 0x7fe0 && ptr->element == 0x0)
			{
			inpix = true;
			obelement *ptr2 = (obelement *)buffx;

			fp.Seek(old, CFile::begin);
			fp.Read(buffx, sizeof(obelement));

			//D2N(buffx, sizeof(obelement));

			//P2N("pixcount: %x element: %x  length: %d \r\n", 
			//			ptr2->group, ptr2->element, ptr2->len);
			}
		else if(ptr->group == 0x7fe0 ) //&& ptr->element == 0x10)
			{
			obelement *ptr2 = (obelement *)buffx;
			
			fp.Seek(old, CFile::begin);
			fp.Read(buffx, sizeof(obelement));

			//D2N(buffx, sizeof(obelement));

			//P2N("pixgroup: %x element: %x  length: %d \r\n", 
			//			ptr2->group, ptr2->element, ptr2->len);

			if(ptr2->len == 0xffffffff)
				{
				//P2N("Reading list\r\n");
				}
			else
				{
				ULONGLONG curr = fp.Seek(0, CFile::current);

				//P2N("Reading %d bytes pixel data flen=%d\r\n", flen - curr, flen);

				// Read the rest of the file, it is pixels
				ULONGLONG len = flen - curr;

				lengtharr[(int)buffers] = len;
				Read2Mem(lengtharr[buffers], &framearr[buffers]);
				buffers++;

				break;
				}
			}
		else if( (ptr->vr1 == 'O' && ptr->vr2 == 'B') ||
				 (ptr->vr1 == 'O' && ptr->vr2 == 'W') ||
				 (ptr->vr1 == 'S' && ptr->vr2 == 'Q') ||
				 (ptr->vr1 == 'U' && ptr->vr2 == 'N') 
				)
			{
			obelement *ptr2 = (obelement *)buffx;

			fp.Seek(old, CFile::begin);
			fp.Read(buffx, sizeof(obelement));

			//P2N("obGroup: (%04x,%04x) vr: %c%c length: %d \r\n", ptr2->group, ptr2->element, ptr2->vr1, ptr2->vr2, ptr2->len);
			
			Read2Mem(ptr2->len, &mem);

			//fp.Read(buff, ptr2->len);

			//if(len < 1000)
			//	D2N(buff, len);
			}
		else if( 
				 (ptr->vr1 == 'A' && ptr->vr2 == 'S') ||
				 (ptr->vr1 == 'A' && ptr->vr2 == 'E') ||
				 (ptr->vr1 == 'A' && ptr->vr2 == 'T') ||
				 (ptr->vr1 == 'C' && ptr->vr2 == 'S') ||
				 (ptr->vr1 == 'D' && ptr->vr2 == 'A') ||
				 (ptr->vr1 == 'D' && ptr->vr2 == 'S') ||
				 (ptr->vr1 == 'D' && ptr->vr2 == 'T') ||
				 (ptr->vr1 == 'F' && ptr->vr2 == 'L') ||
				 (ptr->vr1 == 'F' && ptr->vr2 == 'D') ||
				 (ptr->vr1 == 'I' && ptr->vr2 == 'S') ||
				 (ptr->vr1 == 'L' && ptr->vr2 == 'O') ||
				 (ptr->vr1 == 'L' && ptr->vr2 == 'T') ||
				 (ptr->vr1 == 'P' && ptr->vr2 == 'N') ||
				 (ptr->vr1 == 'S' && ptr->vr2 == 'H') ||
				 (ptr->vr1 == 'S' && ptr->vr2 == 'L') ||
				 (ptr->vr1 == 'S' && ptr->vr2 == 'S') ||
				 (ptr->vr1 == 'S' && ptr->vr2 == 'T') ||
				 (ptr->vr1 == 'T' && ptr->vr2 == 'M') ||
				 (ptr->vr1 == 'D' && ptr->vr2 == 'A') ||
				 (ptr->vr1 == 'U' && ptr->vr2 == 'I') ||
				 (ptr->vr1 == 'U' && ptr->vr2 == 'N') ||
				 (ptr->vr1 == 'U' && ptr->vr2 == 'L') ||
				 (ptr->vr1 == 'U' && ptr->vr2 == 'S') ||
				 (ptr->vr1 == 'U' && ptr->vr2 == 'T') 
				)
			{
			//P2N("nnGroup: (%04x,%04x)  vr: %c%c length: %d \r\n", ptr->group, ptr->element, ptr->vr1, ptr->vr2, ptr->len);
		
			//fp.Read(buff, ptr->len);
			int got = Read2Mem(ptr->len, &mem);
			((char *)mem)[got] = 0;

			Interpret(ptr, mem);

			//if(ptr->len < 1000)
			//	D2N(buff, ptr->len);
			}		
		else	// implicit buffer
			{
			imelement *ptr3 = (imelement *)buffx;
			// Re-read correct size
			fp.Seek(old, CFile::begin);
			fp.Read(buffx, sizeof(imelement));

			//D2N(buffx, sizeof(imelement));
			//P2N("ImGroup: (%04x,%04x)  length: %d \r\n", ptr3->group, ptr3->element, ptr3->len);
		
			int got = Read2Mem(ptr3->len, &mem);
			((char *)mem)[got] = 0;

			Interpret(ptr3, mem);
		
			//if(ptr3->len < 100)
			//	D2N(buff, ptr3->len);
			}
	
		// Safety net
		if(cnt-- == 0)
			break;
		}

	// Make sure it is a DICOM file
	if(!rows || !cols)
		{
		//MBOX("Not a Dicom File");
		//P2N("zero cols or rows\r\n");
		return 0;
		}

	// Correct to the end of buffer
	newrows = lengtharr[0]/ (cols * (bitalloc/8));
	rows = min(rows, newrows);

	//P2N("Corrected rows: %d newrows: %d\r\n", rows, newrows);

	//Dump();
			
	return 1;
}

//////////////////////////////////////////////////////////////////////////
// Put stuff into memory

int		DicomFile::Read2Mem(int len, void **ptr)

{
	//P2N("Read len=%d mem=%p \r\n", len, *ptr);

	ULONGLONG flen = fp.GetLength();
	ULONGLONG old = fp.Seek(0, CFile::current);

	if(old + len > flen)
		{
		P2N("Reading past the end of file\r\n");
		len = (int)(flen - old);
		if(len < 0)
			{
			P2N("Negative number of bytes\r\n");
			return 0;
			}
		}

	// Free old contents
	if(*ptr)
		free(*ptr);

	// Allocate new buffer for content, add 1 dor eos
	*ptr = malloc(len + 1);

	//ASSERT(buff);
	if(*ptr == NULL)
		{
		P2N("Cannot allocate memory for buffer.\r\n");
		return 0;
		}

	int gotx = fp.Read(*ptr, len);
	
	//D2N(buff, gotx);

	return gotx;
}

void DicomFile::Dump()

{
	// Dump parameters

	P2N("Transfer UID='%s'\r\n", TransferUID);
	P2N("frames=%d, buffers=%d\r\n", frames, buffers);
	P2N("rows=%d, cols=%d\r\n", rows, cols);
	P2N("bitalloc=%d, bitstore=%d, highbit=%d\r\n", bitalloc, bitstore, highbit);
	P2N("wincenter=%d, winwidth=%d\r\n", wincenter, winwidth);

	P2N("Expected size %d\r\n", cols * rows * (bitalloc/8));

	// Dump buffers
	for(int loop = 0; loop < buffers; loop++)
		{
		P2N("lenght=%d, ptr=%p\r\n", lengtharr[loop], framearr[loop]);
		}
}

//////////////////////////////////////////////////////////////////////////
//

int		DicomFile::ToImage(CxImage *img, int center, int wwidth)

{
	if(reenter)
		return 0;

	reenter = true;

	//P2N("DicomFile::ToImage() center=%d wwidth=%d\r\n", center, wwidth);

	// Special case
	if(bitalloc == 16)
		{
		int newlen = lengtharr[0] / 2;

		//P2N("Convering to 8 bit array legtharray=%d newlen=%d\r\n", lengtharr[0], newlen);

		BYTE	*mem  = (BYTE *)framearr[0];
		BYTE    *mem2 = (BYTE *)malloc(newlen + 2);
		BYTE	*mem3 = (BYTE*)mem2;

		ASSERT(mem2);
 
		short inx; 
		for(int loop = 0; loop < newlen; loop++)
			{
			inx   =  *((short*)mem);  
		
			//////////////////////////////////////////////////////////////
			// Compress it to window
			//        0        ll        ul   65535
			// -------|---------|--------|-----|--------------
			//                   \      /
			//                    |    |
			// -------------------|----|---------------------
			//                    0   255
			//
			
			//int ul = wincenter  +  winwidth/2;
			//int ll = wincenter  -  winwidth/2;
			
			int ul = center  +  wwidth/2;
			int ll = center  -  wwidth/2;

			double rat = double (ul - ll) / 256;

			if(rat == 0)
				rat = 1;

			if(inx < ll)
				inx = 0;
			else if(inx > ul)
				inx = 255;
			else
				inx = short(double(inx - ll) / rat);

			//////////////////////////////////////////////////////////////
			inx = max(0, min(255, inx));

			*mem2 = (BYTE)inx;

			mem2+= 1; mem += 2;	
			}

		// Convert and show newly acquired image
		if(!img->CreateFromArray(
			mem3,						//BYTE* 	pArray,
			cols,						//DWORD  	didwWidth,
			newrows,					//DWORD  	dwHeight,
			8,							//DWORD  	dwBitsperpixel,
			cols,						//DWORD  	dwBytesperline,
			true ))						//bool  	bFlipImage
			{
			MBOXP("Cannot create Image from dicom file. Error: '%s'", img->GetLastError());
			//goto endd;
			img->Create(500, 500, 24);
			}

		if(mem3)
			free(mem3);
		}
	else
		{
		// Convert and show newly acquired image
		if(!img->CreateFromArray(
			(BYTE*)framearr[0],					//BYTE *   	 pArray,
			cols,								//DWORD  	didwWidth,
			newrows,							//DWORD  	dwHeight,
			bitstore,							//DWORD  	dwBitsperpixel,
			cols * bitstore / 8,		//DWORD  	dwBytesperline,
			true								//bool  	bFlipImage
			) ) 
			{
			MBOXP("Cannot create Image from dicom file. Error: '%s'", img->GetLastError());
			//goto endd;
			img->Create(500, 500, 24);
			}
		}

	reenter = false;
	return 0;
}

//////////////////////////////////////////////////////////////////////////
// Calculate probable window for initial display

#include "PlotDlg.h"

CPlotDlg plot;

#define WORD_NUM 0x10000

int		DicomFile::Histogram(int *wincenter, int *winwidth, int showdlg)

{
	int		*cntarr = new int [WORD_NUM];

	ASSERT(cntarr);

	WORD *mem = (WORD*)framearr[0];
	int bufflen = lengtharr[0] / sizeof(WORD);

	memset(cntarr, 0, WORD_NUM);

	if(showdlg)
		{
		plot.Show();
		plot.CenterWindow();
		}

	// Special case
	if(bitalloc == 16)
		{
		WORD inx; 

		for(int loop = 0; loop < bufflen; loop++)
			{
			inx   =  *((WORD*)mem++);  
			cntarr[inx]++;
			}

		int maxval = 0, maxidx = 0;

		// Ignore low frequency regio
		for(int loop2 = 100; loop2 < WORD_NUM; loop2++)
			{
			if(showdlg)
				{
				plot.m_plot.AddLine(cntarr[loop2]);
				}

			if(maxval < cntarr[loop2])
				{
				maxval = cntarr[loop2];
				maxidx = loop2;
				}
			}

		// Find 10 % mark either way
		int loop3;
		for(loop3 = 0; loop3 < maxidx; loop3++)
			{
			if(cntarr[maxidx + loop3] < maxval / 10)
				break;

			if(cntarr[maxidx - loop3] < maxval / 10)
				break;
			}

		*wincenter = maxidx;
		*winwidth = 2 * loop3;

		// Testing, testing 
		//P2N("Maxval %d maxidx %d  breakwin: %d\r\n", maxval, maxidx, loop3);

		if(showdlg)
			{
			plot.m_plot.Invalidate();
			support.YieldToWinEx();
			plot.Invalidate();
			support.YieldToWinEx();
			Sleep(1000);
			plot.SetActiveWindow();
			//plot.DoModal();
			}
		}

	delete cntarr;

	return 0;
}

