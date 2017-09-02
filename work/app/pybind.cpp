
/* =====[ pybind.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the pybind.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  9/22/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#include "stdafx.h"
#include "xraynotes.h"
#include <direct.h>

#include "PyGlg.h"

extern CPyGlg *pbindclass;

PyObject *xxx = NULL;
PyObject *yyy = NULL;

/* Return the number of arguments of the application command line */
static PyObject* xnt_numdocs(PyObject *self, PyObject *args)
{
	CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);
	
	return pPy_BuildValue("i", dlg->docarr.GetSize());
}

/* Return the number of arguments of the application command line */

static PyObject* xnt_msgbox(PyObject *self, PyObject *args)

{
	//P2N("Running msgbox from python\r\n");

	const char *ss;

	if(!pPyArg_ParseTuple(args, "s", &ss) )
		return NULL;	
	
	CString str;

	str.Format("%s ", ss);
	xMsgBox(str);

	support.YieldToWinEx();

	return pPy_BuildValue("");
}

static PyObject* xnt_print(PyObject *self, PyObject *args)

{
	//P2N("Running print from python\r\n");

	const char *ss;

	CString str;

	if(!pPyArg_ParseTuple(args, "s", &ss) )
		return NULL;	
	
	str.Format("%s", ss);	
	pbindclass->Print(str);

	support.YieldToWinEx();

	return pPy_BuildValue("");
}

static PyObject* xnt_getver(PyObject *self, PyObject *args)

{
	//P2N("Running getver from python\r\n");

	PyObject* ss = pPyString_FromString(pPy_GetVersion());
	return ss;
}

static PyObject* xnt_clear(PyObject *self, PyObject *args)

{
	//P2N("Running clear from python\r\n");

	pbindclass->Clear();

	support.YieldToWinEx();

	return pPy_BuildValue("");
}

static PyObject* xnt_list(PyObject *self, PyObject *args)

{
	//P2N("Running xnt_list from python\r\n");
	
	CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);
	PyObject* sum = pPyTuple_New(dlg->docarr.GetSize());
	
	// Fill in document list to a tuple
	for(int loop = 0; loop < dlg->docarr.GetSize(); loop++)
        {
		PyObject *tmp;
	    //P2N("listing doc %d %s\r\n", loop, ((CXrayDoc*)dlg->docarr[loop])->filename);
		
		tmp = pPy_BuildValue("is", loop, ((CXrayDoc*)dlg->docarr[loop])->filename) ;
		
		// Add object to end:
		pPyTuple_SetItem(sum, loop, tmp);
		}

	return sum;
}

static PyObject* xnt_next(PyObject *self, PyObject *args)

{
	//P2N("Running next from python\r\n");

	CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);

	dlg->autoadvance = true;
	dlg->helper->NextDoc();
	support.YieldToWinEx();
	dlg->autoadvance = false;
	
	return pPy_BuildValue("");
}

static PyObject* xnt_message(PyObject *self, PyObject *args)

{
	//P2N("Running xnt_message from python\r\n");

	const char *ss;

	if(!pPyArg_ParseTuple(args, "s", &ss) )
		return NULL;	
	
	ShowStatus(ss);
	support.YieldToWinEx();

	return pPy_BuildValue("");
}


static PyObject* xnt_load(PyObject *self, PyObject *args)

{
	P2N("Running xnt_load from python\r\n");

	const char *name;

	if(!pPyArg_ParseTuple(args, "s", &name) )
		return NULL;	
	
	CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);

	dlg->functs->OpenImageFile(name);

	//ShowStatus(ss);
	support.YieldToWinEx();

	return pPy_BuildValue("");
}

static PyObject* xnt_prev(PyObject *self, PyObject *args)

{
	//P2N("Running nxnt_prevext from python\r\n");

	CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);

	dlg->autoadvance = true;
	dlg->helper->PrevDoc();
	support.YieldToWinEx();
	dlg->autoadvance = false;
	
	return pPy_BuildValue("");
}

static PyObject* xnt_mindlg(PyObject *self, PyObject *args)

{
	//P2N("Running xnt_mindlg from python\r\n");

	pbindclass->Minimize();
	
	//SetWindowPos(NULL, 0, 0, 0, 0, SWP_HIDEWINDOW | SWP_NOMOVE | SWP_NOSIZE); 	
	//CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);
	//dlg->SetFocus();

	support.YieldToWinEx();

	return pPy_BuildValue("");
}


static PyObject* xnt_maxdlg(PyObject *self, PyObject *args)

{
	//P2N("Running xnt_maxdlg from python\r\n");

	pbindclass->SetWindowPos(NULL, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE); 
	support.YieldToWinEx();
	return pPy_BuildValue("");
}

static PyObject* xnt_suppshow(PyObject *self, PyObject *args)

{
	int showflag;

	if(!pPyArg_ParseTuple(args, "i", &showflag) )
		return NULL;	

	//P2N("Running xnt_suppshow %d from python\r\n", showflag);

	CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);

	dlg->SuppWindows(showflag);

	support.YieldToWinEx();
	return pPy_BuildValue("");
}

static PyObject* xnt_setact(PyObject *self, PyObject *args)

{
	int docnum;

	if(!pPyArg_ParseTuple(args, "i", &docnum) )
		return NULL;	

	//P2N("Running xnt_setact %d from python\r\n", docnum);

	CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);

	if(docnum < dlg->docarr.GetSize() && docnum >= 0)
		{
		dlg->current = docnum;
		dlg->Invalidate();
		}
	else
		{
		//P2N("Raising exception\r\n");

		if(!xxx)
			xxx =  pPyErr_NewException( "xnt.IndexError", NULL, NULL);
		pPyErr_SetString(xxx, "Invalid document index");
		return NULL;
		}

	support.YieldToWinEx();
	return pPy_BuildValue("");
}

static PyObject* xnt_nice(PyObject *self, PyObject *args)

{
	//P2N("Running xnt_nice from python\r\n");

	support.YieldToWinEx();

	return pPy_BuildValue("");
}

static PyObject* xnt_buffer(PyObject *self, PyObject *args)

{
	P2N("Running xnt_buffer from python\r\n");

	CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);

	if(dlg->current < 0)
		return pPy_BuildValue("");

    CXrayDoc *doc = ((CXrayDoc*)dlg->docarr[dlg->current]);

	if(!doc)
		return pPy_BuildValue("");;

	PyObject *buff;

	void *ptr = (void*) doc->render->GetPixPtr();
	int size = doc->render->GetBuffSize();

	// Just for testing
	//memset(ptr, 0, 1000000);

	buff = pPyBuffer_FromReadWriteMemory(ptr, size);

	support.YieldToWinEx();
	
	return buff;
}

static PyObject* xnt_imginfo(PyObject *self, PyObject *args)

{
	P2N("Running xnt_buffer from python\r\n");

	CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);

	if(dlg->current < 0)
		return pPy_BuildValue("");

    CXrayDoc *doc = ((CXrayDoc*)dlg->docarr[dlg->current]);

	if(!doc)
		return pPy_BuildValue("");

	PyObject *inf;

	inf = pPy_BuildValue("iiii", doc->render->GetWidth(),
									doc->render->GetHeight(),
										doc->render->GetEffWidth(),
											doc->render->GetBitsPpix());
	
	support.YieldToWinEx();
	
	return inf;
}

static PyObject* xnt_inval(PyObject *self, PyObject *args)

{
	//P2N("Running xnt_inval from python\r\n");

	CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);

	dlg->Invalidate();
	support.YieldToWinEx();
	
	return pPy_BuildValue("");
}

static PyObject* xnt_exit(PyObject *self, PyObject *args)

{
	//P2N("Running xnt_inval from python\r\n");

	CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);


	dlg->Invalidate();
	support.YieldToWinEx();
	
	pbindclass->doexit = true;

	return pPy_BuildValue("");
}

//////////////////////////////////////////////////////////////////////////

PyMethodDef EmbMethods[] = 
	
{	
	{	"exit", xnt_exit, METH_VARARGS,	
		"Exit XrayNotes Application"
	},
	{	"imginfo", xnt_imginfo, METH_VARARGS,	
		"Get Image Info. Returns: img width, img height, img_effwidth, bytes_per_pix"
	},
	{	"inval", xnt_inval, METH_VARARGS,	
		"Invalidate (repaint) Display."
	},
	{	"buffer", xnt_buffer, METH_VARARGS,	
		"Return Display Buffer."
	},
	{	"load", xnt_load, METH_VARARGS,	
		"Show/Hide supplementary windows."
	},
	{	"nice", xnt_nice, METH_VARARGS,	
		"Show/Hide supplementary windows."
	},
	{	"suppshow", xnt_suppshow, METH_VARARGS,	
		"Show/Hide supplementary windows."
	},
	{	"setactive", xnt_setact, METH_VARARGS,	
		"Set Active document."
	},
	{	"message", xnt_message, METH_VARARGS,	
		"Show flash message."
	},
	{	"maxdlg", xnt_maxdlg, METH_VARARGS,	
		"Maximize python dialog."
	},
	{	"mindlg", xnt_mindlg, METH_VARARGS,	
		"Minimize python dialog."
	},
	{	"prevdoc", xnt_prev, METH_VARARGS,	
		"Switch to next documents."
	},
	{	"nextdoc", xnt_next, METH_VARARGS,	
		"Switch to next documents."
	},
	{	"list", xnt_list, METH_VARARGS,	
		"List loaded documents."
	},
	{	"clear", xnt_clear, METH_VARARGS,	
		"Clears GUI window."
	},
	{	"numdocs", xnt_numdocs, METH_VARARGS,	
		"Return the number of documents in XrayNotes."
	},
	{	"msgbox", xnt_msgbox, METH_VARARGS,	
		"Show message box from python."
	},
	{	"getver", xnt_getver, METH_VARARGS,	
		"Get Version of Python."
	},
	{	"printx", xnt_print, METH_VARARGS,	
		"Print to py dialog."
	},

	{NULL, NULL, 0, NULL}
};

//////////////////////////////////////////////////////////////////////////

char *moduledoc = 

"The XrayNotes application interface for Python. To acess the functionality within, use 'import xnt'\r\n"
"in your code. To see output from python's print, import the 'catcher' module at the beginning of your code.\r\n"
"\r\n"
"The following functions are available:\r\n"
"\r\n"
"getver -- returns a string representation of python version\r\n"
"msgbox -- envokes the XrayNotes Message box function. Useful for getting user attention. Args: str\r\n"
"message -- envokes the XrayNotes message flashing function. Useful for progress information. Args: str\r\n"
"printx -- prints to the GUI python dialog. Useful for verifying vars and progress reporting Args: str\r\n"
"clear -- clears the GUI python dialog.\r\n"
"nextdoc -- switches to the next XrayNotes Doc\r\n"
"prevdoc -- switches to the previous XrayNotes Doc\r\n"
"mindlg -- minimize python dialog\r\n"
"maxdlg -- maximize python dialog\r\n"
"setactive -- set active document Args: int\r\n"
"list -- list loaded documents\r\n"
"numdocs -- Return the number of documents in XrayNotes.\r\n"
"suppshow -- Show/Hide supplementary windows.\r\n"
"nice -- Let the XrayNotes application process messages.\r\n"
"load -- Load a document int XrayNotes. Args: fname_str\r\n"
"buff -- Return Current Image buffer\r\n"
"inval -- Repaint current display\r\n"
"imginfo -- Get Image Info. Returns: img_width, img_height eff_width bits_per_pix\r\n"
"exit -- Exit XrayNotes\r\n"
;

// EOF