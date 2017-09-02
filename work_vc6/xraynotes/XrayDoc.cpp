
/* =====[ XrayDoc.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the XrayDoc.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  4/11/2007  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

/////////////////////////////////////////////////////////////////////////////
// XrayDoc.cpp : implementation file
//

#include "stdafx.h"
#include "xraynotes.h"
#include "XrayDoc.h"

int		CXrayDoc::gl_enum = 0;	

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ITERATE_ARR(loopx, arrx)	\
	for(int loopx = 0; loopx < arrx.GetSize(); loopx++)

static	long	xarr[] = { 1, 1, 1, 1, 8, 1, 1, 1, 1};

/////////////////////////////////////////////////////////////////////////////
// CXrayDoc

IMPLEMENT_DYNCREATE(CXrayDoc, CCmdTarget)

CXrayDoc::CXrayDoc()

{
	//P2N("Constructed document %p\r\n", this);

	magic = DOC_MAGIC;

	img		= new CxImg;	ASSERT(img);
	render	= new CxImg;	ASSERT(render);
	org		= new CxImg;	ASSERT(org);
	sel		= new CxImg; 	ASSERT(sel);

	memcpy(xkernel, xarr, sizeof(xarr));

	self = this;		
	preview = false;

	//dicom = NULL;
	m_currlay = NULL;
	
	multidraw = 0;
    drawstate = TOOL_IDLE;
    
	state = changed = 0;
	mag = factor = 1;
	
	arg1 = arg2 = arg3 = arg4 = 0;
	command = thhandle = commit = 0;

	backlink = NULL;

	finestate = false;
	uniq = -1;
	oldshowstate = -1;	
	oldcurrframe = currframe = -1;
	
	old_num_to_show = num_to_show = 1;
	
	showstate = STATE_FULL;

	P2N("New Doc this=%p id=%s\r\n", this, id.GetStr());
	//P2N("New Doc this=%p id=%s\r\n", this);

	graytext = 0;
}


CXrayDoc::~CXrayDoc()

{	
	//P2N("Destructing document %p\r\n", this);
	
	XDELETE (img);	XDELETE (render);	
	XDELETE (org);	XDELETE (sel);
	
	for(int loop = 0; loop < dcarr.GetSize(); loop++)
		{
		delete ( DicomFile*)dcarr[loop];
		}

	for(int loop2 = 0; loop2 < imgarr.GetSize(); loop2++)
		{
		delete ( CxImg*)imgarr[loop2];
		}

	for(int loop3 = 0; loop3 < rendarr.GetSize(); loop3++)
		{
		delete ( CxImg*)rendarr[loop3];
		}

	for(int loop4 = 0; loop4 < m_layers.GetSize(); loop4++)
		{
		delete ( CLayer*) m_layers[loop4];
		}
}

BEGIN_MESSAGE_MAP(CXrayDoc, CCmdTarget)
	//{{AFX_MSG_MAP(CXrayDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXrayDoc message handlers

void CXrayDoc::AddUndo()

{
	// Make sure stack is not overgrown
	if(undolist.GetCount() >= IMAGE_UNDO_SIZE)
		{
		//P2N("Removing from undo end\r\n");

		CxImage *tmp2 = (CxImage *)undolist.GetTail();
		delete tmp2;

		undolist.RemoveTail();
		}
		
	CxImage *tmp = new(CxImage);
	ASSERT(tmp);

	tmp->Copy(*img);
	undolist.AddHead(tmp);
}

void CXrayDoc::Undo()

{
	if(undolist.GetCount() > 0)
		{
		P2N("Undo at %d\r\n", undolist.GetCount() );

		CxImage *tmp2 = (CxImage *)undolist.RemoveHead();
		img->Copy(*tmp2);
		delete tmp2;
		}
	else
		{
		ShowStatus("Nothing to undo ...");
		//support.YieldToWinEx();
		}
}

void CXrayDoc::NewRender()
{
	oldshowstate = -1;
}

void CXrayDoc::AddLayer(CLayer *lay)

{
	//P2N("Adding layer %s %p\r\n", lay->GetKindStr(), lay);

	m_layers.Add(lay);
	m_currlay = lay;
	changed++;

	ASSERT(lay->magic == LAYER_MAGIC);
}

void CXrayDoc::Redo()

{
	ShowStatus("Nothing to redo ...");
}

// Is it dicom file?
	
int CXrayDoc::IsDicomFile()

{
	// Is it dicom file?
	if(dcarr.GetSize() == 0)
		return false;
	else
		return true;
}

// Is it dicom series?
	
int CXrayDoc::IsDicomSeries()

{
	// Is it dicom file?
	if(dcarr.GetSize() > 1)
		return true;
	else
		return false;
}

