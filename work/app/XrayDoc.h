
/* =====[ XrayDoc.h ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the XrayDoc.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  11/26/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#if !defined(AFX_XRAYDOC_H__2DAEB549_B379_4FE8_9893_B25D5D2BEA7E__INCLUDED_)
#define AFX_XRAYDOC_H__2DAEB549_B379_4FE8_9893_B25D5D2BEA7E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// XrayDoc.h : header file
//

#define IMAGE_UNDO_SIZE		5
#define DOC_MAGIC	0x12345678

#include "Layer.h"
#include "dicomfile.h"
#include "xImg.h"

//define DRAWSTATE_NONE -1
//define DRAWSTATE_NOOP	0
//define DRAWSTATE_FREE	1
//define DRAWSTATE_RECT	2
//define DRAWSTATE_CIRC	3
//define DRAWSTATE_LINE	4
//define DRAWSTATE_IMG	5

#define ASSERT_VALID_DOC(dd) ASSERT(dd->magic == DOC_MAGIC);
#define CHECKDOC(dd) ASSERT(dd->magic == DOC_MAGIC);

#include "UniqID.h"

/////////////////////////////////////////////////////////////////////////////
// CXrayDoc command target

class CXrayDoc : public CCmdTarget

{
	DECLARE_DYNCREATE(CXrayDoc)

	static int gl_enum;

public:

	int			graytext;

	CUniqID		id;

	CString		filename;

	long		xkernel[9];
	int			num_to_show, old_num_to_show;
	int			magic, changed, state;
	int			showstate, oldshowstate;
	int			command;					// DSP command
	int			arg1, arg2, arg3, arg4;		// DSP parameters
	int			commit, uniq;
	int			drawstate, multidraw;
	int			thhandle, preview;
	int			currframe, oldcurrframe;	// DICOM frame
	int			currundo, lastundo;			// Undo related
	bool		finestate;
	double		mag;

	void		*self;
	
	CxImg		*img, *render;
	CxImg		*org, *sel;

	//CPoint		drawstart;

	CXrayDoc	*backlink;

	CPtrArray	rendarr;			// For dicom image set
	CPtrArray	imgarr;				// For dicom image set
	CPtrArray	dcarr;				// For dicom 
	CPtrArray	dcarr2;				// For dicom sorting function
	
	//DicomFile	*dicom;

	// Layer related
	CPtrArray	m_layers;			// List of layers
	CPtrArray	m_layers2;			// List of SHADOW layers for undo
	CPtrArray	m_layers3;			// List of SHADOW layers for redo
	CPtrArray	m_del_layers;		// List of deleted layers

	// Undo related
	CPtrList	undolist, redolist;
	CPtrList	UndoStack, RedoStack;

	// Supplementary strings
	CPtrList	StrStack;			// Undo Strings

	// Layer in focus
	CLayer		*m_currlay;

// Attributes
public:

	CXrayDoc();  

// Operations
public:
	int		IsDicomFile();
	int		IsDicomSeries();
	void	Redo();
	void	AddLayer(CLayer *lay);
	void	NewRender();
	void	AddUndo();
	void	Undo();

	double factor;
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXrayDoc)
	//}}AFX_VIRTUAL

	virtual ~CXrayDoc();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CXrayDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XRAYDOC_H__2DAEB549_B379_4FE8_9893_B25D5D2BEA7E__INCLUDED_)
