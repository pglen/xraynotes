
/* =====[ ShadesHelp.h ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the ShadesHelp.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  10/27/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// ShadesHelp.h: interface for the CShadesHelp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHADESHELP_H__85236EAB_E09F_463E_813F_DC3FE06CF20C__INCLUDED_)
#define AFX_SHADESHELP_H__85236EAB_E09F_463E_813F_DC3FE06CF20C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CShadesHelp 

{
public:
	void	ClearAllObjFocus();

	void	OnDocChange();
	int		GetNumSelObj();
	void	HandleMark(UINT nChar, UINT nRepCnt, UINT nFlags);
	void	MoveDicomZ(int flag);
	void	OnDocSetFocus();
	void	OnDocSwitch();

	void	OpenLastDicomSeries();
	void	OpenLastDicomFile();
	void	OpenLastSession();
	void	OpenLastFile();
	void	InitChevrons();

	void	TmpRestorePanels();
	void	TmpClosePanels();

	void	InitChevPos(int xx = 0, int yy = 0);

	friend class CShadesDlg;

	char	*state2str(int state);

	CShadesHelp();
	CShadesHelp(void *ptr);

	virtual ~CShadesHelp();

	void	DumpDocs();
	void	PrevDoc();
	void	NextDoc();

	void	GetFirstLayer();
	void	GetLastLayer();
	CLayer*	GetNextLayer();
	CLayer*	GetPrevLayer();

protected:

	CShadesDlg *dlg;
	int		layprog;

	//void	CloseAllDialogs();

};

#endif // !defined(AFX_SHADESHELP_H__85236EAB_E09F_463E_813F_DC3FE06CF20C__INCLUDED_)
