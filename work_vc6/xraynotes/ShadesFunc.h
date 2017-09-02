
/* =====[ ShadesFunc.h ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the ShadesFunc.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  1/28/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#if !defined(AFX_SHADESFUNC_H__79B91B69_215A_41B3_ACE8_16C84F975788__INCLUDED_)
#define AFX_SHADESFUNC_H__79B91B69_215A_41B3_ACE8_16C84F975788__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FileDialogST.h"
#include "Flag.h"
#include "XrayM.h" 

//void	xxxCb(void *ptr);
//void	dumpmenu(CMenu *menu);
	
class CShadesFunc 
 
{
public:
	CShadesFunc(void *);
	virtual ~CShadesFunc();

	friend class CShadesDlg;

	CString oldopen;

	void	rCropCb(void *ptr);
	void	CropCb(void *ptr);
	void	IdleCb(void *ptr);
	void	MeasCb(void *ptr);
	void	MagCb(void *ptr);
	void	rMagCb(void *ptr);
	void	SVGCb(void *ptr);
	void	EllipseCb(void *ptr);
	void	TextCb(void *ptr);
	void	rTextCb(void *ptr);
	void	ImageCb(void *ptr);
	void	DrawCb(void *ptr);
	void	RectCb(void *ptr);
	void	LineCb(void *ptr);
	void	PatCb(void *ptr);
	void	NewCb(void *ptr);
	void	cNewCb(void *ptr);
	void	AutoCb(void *ptr);
	void	OptCb(void *ptr);
	void	DocPropCb(void *ptr);
	void	RedoCb(void *ptr);
	void	SelectCb(void *ptr);
	void	MarkCb(void *ptr);
	void	MagDocCb(void *ptr);
	void	CutCb(void *ptr);
	void	PasteCb(void *ptr);
	void	rPasteCb(void *ptr);
	void	CopyCb(void *ptr);
	void	rCopyCb(void *ptr);
	void	PrintCb(void *ptr);
	void	MonitorCb(void *ptr);
	void	UndoCb(void *ptr);
	void	NormCb(void *ptr);
	void	ColorCb(void *ptr);
	void	BrightCb(void *ptr);
	void	ExitCb(void *ptr);
	void	rExitCb(void *ptr);
	void	ListDocsCb(void *ptr);
	void	ConfCb(void *ptr);
	void	ToggleUICb(void *ptr);
	void	rOpenFileCb(void *ptr);
	int		OpenImageFile(const char *name);
	void	OpenFileCb(void *ptr);
	void	ScanDocCb(void *ptr);
	void	ProcCb(void *ptr);
	char 	*LoadPlugin(const char *name, int com);
	void	rProcCb(void *ptr);
	void	SaveFileCb(void *ptr);
	void	TestCb(void *ptr);
	void	CloseAll(void *ptr);
	void	CloseFileCb(void *ptr);
	void	rCloseFileCb(void *ptr);
	void	MBoxCb(void * ptr);
	void	SaveAsCb(void * ptr);
	int		OpenOneDicom(const char *fname, int uniq);
	void	OpenDicomFile(void *ptr, int ser = false, const char *name = NULL);
	void	OpenDicomSeries(void *ptr);

	CShadesDlg *dlg;

};

#endif // !defined(AFX_SHADESFUNC_H__79B91B69_215A_41B3_ACE8_16C84F975788__INCLUDED_)
