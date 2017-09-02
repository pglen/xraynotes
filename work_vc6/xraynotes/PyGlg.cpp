
/* =====[ PyGlg.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the PyGlg.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  9/24/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// PyGlg.cpp : implementation file
//

#include "stdafx.h"
#include "xraynotes.h"

#include <direct.h>
#include <process.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>

#include "PyGlg.h"

pfs			pPy_GetVersion;
pfo			pPyDict_New;
pfo_svar 	pPy_BuildValue;
pfo_osvar	pPyArg_ParseTuple;
pfo_s		pPyString_FromString;
pfo_oio		pPyTuple_SetItem;
pfo_i		pPyTuple_New;
pfv_os		pPyErr_SetString;
pfo_soo		pPyErr_NewException;
pfo_pi		pPyBuffer_FromReadWriteMemory;
pfv_o		pPyDict_Clear;
pfo_moo		pPyEval_EvalCode;
pfv			pPyErr_Clear;
pfv_ooo		pPyErr_Restore;
pfs_o		pPyString_AsString;
pfo_o		pPyObject_Str;
pfv_popopo	pPyErr_Fetch;
pfo_ssip	pPy_CompileStringFlags;
pfi_oso		pPyDict_SetItemString;
pfo			pPyEval_GetBuiltins;
pfv			pPy_Finalize;
pfo_smsoi	pPy_InitModule4;
pfv			pPy_Initialize;
pfv_s		pPy_SetProgramName;


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Imports from python xnt

extern PyMethodDef EmbMethods[];
extern char *moduledoc;

/////////////////////////////////////////////////////////////////////////////
// CPyGlg dialog

CPyGlg *pbindclass;

CPyGlg::CPyGlg(CWnd* pParent /*=NULL*/)
	: CGrayDlg(CPyGlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPyGlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	phand = NULL;
	pbindclass = this;
	running = doexit = pyerr = 0;

	windowname = "CPyGlg"; //support.GetFileNameExt(__FILE__);
}

CPyGlg::~CPyGlg()
{
	
}

void CPyGlg::DoDataExchange(CDataExchange* pDX)
{
	CGrayDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPyGlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPyGlg, CGrayDlg)
	//{{AFX_MSG_MAP(CPyGlg)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON8, OnButton8)
	ON_BN_CLICKED(IDC_BUTTON9, OnButton9)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPyGlg message handlers

void CPyGlg::OnButton1() 

{
	//P2N("Running Python statement\r\n");
	//Print("Executing command:\r\n");

	if(!phand)
		{
		xMsgBox("Python interpreter is not loaded.");
		return;
		}

	if(pyerr)
		{
		xMsgBox("Python interpreter functions are not loaded.");
		return;
		}

	if(running)
		{
		xMsgBox("Script is currently running. Please stop it before starting new.");
		return;
		}

	if(fstr == "")
		{
		xMsgBox("No script file specified.");
		return;
		}

	FILE *fp = fopen(fstr, "rt");
	if(!fp)
		{
		xMsgBox("Cannot open python file");
		return;
		}

	fseek(fp, 0, SEEK_END);
	int len = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	char	*buff;
	buff = (char *) malloc(len + 1);

	if(!buff)
		{
		xMsgBox("Cannot alloc memory for python file");
		fclose(fp);
		return;
		}

	int len2 = fread(buff, 1, len, fp);
	if(len2 < 0)
		{
		xMsgBox("Cannot read python file");
		fclose(fp);
		free(buff);
		return;
		}

	// Relese it here
	fclose(fp);

	len2 = min(len2, len);

	buff[len2] = 0;

	//P2N("'%s'\r\n", buff);

	PyObject *loc = pPyDict_New ();
	PyObject *glb = pPyDict_New ();

	pPyDict_SetItemString (glb, "__builtins__", pPyEval_GetBuiltins ());
	pPyDict_SetItemString (glb, "xnt", xnt_module);

	//PyDict_SetItemString (glb, "print", xnt_module);

	PyObject *rr, *p1 = NULL, *p2 = NULL, *p3 = NULL;
	CString tmp;

	try
		{			
		CString fname, tmp;
		fname = support.GetFileNameExt(fstr);

		rr =  pPy_CompileStringFlags(buff, fname, Py_file_input, 0);

		if(rr == NULL)
			{
			PyObject *v = NULL, *vv = NULL, *vvv = NULL;

			P2N("Compile is NULL\r\n");
			
			pPyErr_Fetch(&p1, &p2, &p3);
						
			v  = pPyObject_Str(p1), vv = pPyObject_Str(p2);
		
			if(v) 
				tmp.Format("%s\r\n", pPyString_AsString(v)), Print(tmp);

			if(vv) 
				tmp.Format("%s\r\n", pPyString_AsString(vv)), Print(tmp);

			pPyErr_Restore(p1, p2, p3);

			Py_XDECREF (v);	Py_XDECREF (vv);
			pPyErr_Clear();
			}
		else
			{
			running = true;

			PyObject *xx = pPyEval_EvalCode((PyCodeObject *)rr, glb, loc);
			
			if(xx == NULL)
				{
				PyObject *v = NULL, *vv = NULL, *vvv = NULL;

				P2N("Eval is NULL\r\n");
		
				pPyErr_Fetch(&p1, &p2, &p3);

				v	= pPyObject_Str(p1);
				vv	= pPyObject_Str(p2);
				vvv = pPyObject_Str(p3);
			
				if(v) 
					tmp.Format("%s\r\n", pPyString_AsString(v)), Print(tmp);

				if(vv) 
					tmp.Format("%s\r\n", pPyString_AsString(vv)), Print(tmp);
				
				if(vvv) 
					tmp.Format("%s\r\n", pPyString_AsString(vvv)), Print(tmp);

				//PyErr_Restore(p1, p2, p3);

				Py_XDECREF (v);
				Py_XDECREF (vv);
				Py_XDECREF (vvv);
				
				pPyErr_Clear();
				}
			running = false;

			Py_XDECREF (xx);
			}
		Py_XDECREF (rr);
		}

	catch (...)
		{
		P2N("C++ caught an exception while executing python\r\n");
		Print("C++ caught an exception while executing python code.\r\n");

		running = false;
		pPyErr_Clear();
		}

	Py_XDECREF (p1); Py_XDECREF (p2); Py_XDECREF (p3);

	pPyDict_Clear(glb); 
	pPyDict_Clear(loc); 

	Py_XDECREF (glb);
	Py_XDECREF (loc);

	pPyErr_Clear();

	running = false;

	P2N("Cleaning up buff\r\n");
	if(buff)
		free(buff);

	if(doexit)
		exit(0);
}

BOOL CPyGlg::OnInitDialog() 

{		
	CGrayDlg::OnInitDialog();
	
	m_title = "Python Script Control Window";

	// Reload script name:
	fstr = AfxGetApp()->GetProfileString(strConfig, "LastPyScript", "");

	if (LoadPython() < 0)
		fstr = "No Interpreter";
		//return 0;
	else
		InitPy();

	CEdit *ed = (CEdit *)GetDlgItem(IDC_EDIT1);
	ed->SetWindowText(fstr);
	
	return 0;
}

void CPyGlg::Print(const char *str)

{
	CString cstr, str2(str);	
	str2.Replace("\n", "\r\n");

	CEdit *ed = (CEdit *)GetDlgItem(IDC_EDIT2);

	ed->GetWindowText(cstr);
	cstr += str2;
	ed->SetWindowText(cstr);

	ed->SetSel(0, -1);

	support.YieldToWinEx();
}

void CPyGlg::OnButton2() 

{
	if(running)
		{
		xMsgBox("Script is currently running. Please stop it before starting new.");
		return;
		}

	CString fileName;

	BOOL bRet = 0;

	int	bOpenFileDialog = true;
	int	dwFlags = 0;
	int *pType = NULL;

	//CFileDialog
	CFileDialogST dlgFile(bOpenFileDialog);

	CString title;
	title = "Open Python Script File"; 
	
	dlgFile.m_ofn.Flags |= dwFlags | OFN_EXPLORER ; //OFN_ALLOWMULTISELECT | ;

	int nDocType = (pType != NULL) ? *pType : 0;
	int nIndex = 0;

	dlgFile.m_ofn.nFilterIndex = 0; //nIndex + 1;
	CString strDefExt = "py";
	dlgFile.m_ofn.lpstrDefExt = strDefExt;
	
	CString cwd = support.GetCurrentDir();

	CString sd(approot);	sd += "python";

	if(!support.IsDir(sd))
		{
		P2N("Switching to initial cwd\r\n");
		sd = intialcwd;	sd += "python";
		}

	dlgFile.m_ofn.lpstrInitialDir = sd;

	CString strFilter;
	
	strFilter += "Python Script File";
	strFilter += (TCHAR)NULL;
	strFilter += "*.py";
	strFilter += (TCHAR)NULL;
	
	strFilter += "*.*";
	strFilter += (TCHAR)NULL;
	strFilter += "*.*";
	strFilter += (TCHAR)NULL;
	strFilter += (TCHAR)NULL;

	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;
	dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(1000 * _MAX_PATH);

	//dlgFile.m_ofn.lpstrInitialDir = approot + "python";

	bRet = (dlgFile.DoModal() == IDOK) ? TRUE : FALSE;
	fileName.ReleaseBuffer();

	// Restore original path
	P2N("Restore original path %s\r\n", cwd);
	_chdir(cwd);

	//CString cwd2 = support.GetCurrentDir();
	//P2N("Restored original path2 %s\r\n", cwd2);

	if(!bRet)
		return ;

	fstr = fileName;

	CEdit *ed = (CEdit *)GetDlgItem(IDC_EDIT1);
	ed->SetWindowText(fstr);

	// Save last script
    AfxGetApp()->WriteProfileString(strConfig, "LastPyScript", fstr);

	//Print("Loaded %s\r\n", fstr);

	OnButton3();
}

void CPyGlg::OnButton3() 
{
	Clear();
}

void CPyGlg::OnDestroy() 

{
	CGrayDlg::OnDestroy();
	
	//P2N("Finalizing python\r\n");

	if(phand)
		pPy_Finalize();

	running = false;
}

void CPyGlg::Clear()

{
	CEdit *ed = (CEdit *)GetDlgItem(IDC_EDIT2);
	ed->SetWindowText("");
}

void CPyGlg::OnButton4() 

{
	P2N("Stop Pressed\r\n");
	if(running)
		{
		Print("Python Script terminated by request of user.\r\n");

		pPy_Finalize();
		InitPy();
		running = false;
		}
}

//////////////////////////////////////////////////////////////////////////
// Initialize python environment

void CPyGlg::InitPy()

{
	// Change to python script dir:

	if(!phand)
		return;

	CString cwd = support.GetCurrentDir();
	CString sd(intialcwd);	sd += "python";
	
	P2N("Init Python, script dir: %s\r\n", sd);
	
	_chdir(sd);

	pPy_SetProgramName("XrayNotes-Python-Binding");
	pPy_Initialize();
	
	xnt_module = pPy_InitModule4("xnt", EmbMethods, moduledoc, NULL, PYTHON_API_VERSION);

	//P2N("Loaded xnt %d\r\n", xnt_module);

	_chdir(cwd);
}

void CPyGlg::OnButton5() 

{
	Minimize();
}

void CPyGlg::OnOK() 

{		
	Print("Python Script terminated by request of user.\r\n");

	if(phand)
		pPy_Finalize();

	InitPy();
	
	CGrayDlg::OnOK();
}

void CPyGlg::OnButton8() 

{
	FILE *fp = fopen(fstr, "rt");
	if(!fp)
		{
		xMsgBox("Cannot open python script to show.");
		return;
		}

	fseek(fp, 0, SEEK_END);
	int len = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	char	*buff;
	buff = (char *) malloc(len + 1);

	if(!buff)
		{
		xMsgBox("Cannot alloc memory for python script");
		fclose(fp);
		return;
		}

	int len2 = fread(buff, 1, len, fp);
	if(len2 < 0)
		{
		xMsgBox("Cannot read python script");
		fclose(fp);
		free(buff);
		return;
		}

	len2 = min(len2, len);
	buff[len2] = 0;

	Clear();
	Print(buff);

	fclose(fp);
	free(buff);
}

void CPyGlg::OnButton9() 

{
	int ret = _spawnlp(_P_WAIT, "notepad", "notepad", fstr, NULL);
	P2N("spawn returned %d\r\n", ret);
	return;
}

int CPyGlg::TryExec(const char *str, const char *arg)

{
	return 0;
}

void CPyGlg::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 

{
	P2N("OnChar(UINT nChar=%d\r\n",  nChar);

	CGrayDlg::OnChar(nChar, nRepCnt, nFlags);
}

void CPyGlg::Minimize()
{
	SetWindowPos(NULL, 0, 0, 0, 0, SWP_HIDEWINDOW | SWP_NOMOVE | SWP_NOSIZE);
	CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);
	dlg->SetFocus();

	support.YieldToWinEx();
}

#define LOADPY(var, type, str)	\
	   var    	= (type)GetProcAddress(phand, str); \
	   if(!var) \
		{ \
		cstr.Format("Cannot load python function %s", str); \
		xMsgBox(cstr); \
		pyerr = true;\
		} \
	   
//////////////////////////////////////////////////////////////////////////

int CPyGlg::LoadPython()

{
	pyerr = false;

	CString str; support.GetEnvStr("PATH", str);
	
	//CString pylib(approot);	pylib += "python\\python25.dll";
	CString pylib("c:\\xraynotes\\python25.dll");
		
	P2N("LoadPython from '%s'\r\n", pylib);

	//phand = LoadLibrary("python25.dll" );
	phand = LoadLibrary(pylib);

	// Try Again from current dir ...
	if(!phand)
		{
		P2N("Loading from current dir\r\n");

		CString pylib2("c:\\xraynotes\\python25.dll");
		P2N("LoadPython from '%s'\r\n", pylib2);
		phand = LoadLibrary(pylib2);
		}

	if(!phand)
		{
		xMsgBox("Python functionality is not available. Please install Python 2.5 first");
		return -1;
		}

	// Load python entry points
	CString cstr; 

	LOADPY(pPy_GetVersion              	, pfs,		  "Py_GetVersion");
	LOADPY(pPyDict_New 					, pfo,		  "PyDict_New");
	LOADPY(pPy_BuildValue				, pfo_svar,   "Py_BuildValue");
	LOADPY(pPyArg_ParseTuple			, pfo_osvar,  "PyArg_ParseTuple");
	LOADPY(pPyString_FromString			, pfo_s,	  "PyString_FromString");
	LOADPY(pPyTuple_SetItem            	, pfo_oio,	  "PyTuple_SetItem");
	LOADPY(pPyTuple_New					, pfo_i,	  "PyTuple_New");
	LOADPY(pPyErr_SetString            	, pfv_os,	  "PyErr_SetString");
	LOADPY(pPyErr_NewException         	, pfo_soo,	  "PyErr_NewException");
	LOADPY(pPyBuffer_FromReadWriteMemory, pfo_pi,	  "PyBuffer_FromReadWriteMemory");
	LOADPY(pPyDict_Clear               	, pfv_o, 	  "PyDict_Clear");
	LOADPY(pPyEval_EvalCode            	, pfo_moo,	  "PyEval_EvalCode");
	LOADPY(pPyErr_Clear                 , pfv,		  "PyErr_Clear");
	LOADPY(pPyErr_Restore              	, pfv_ooo,	  "PyErr_Restore");
	LOADPY(pPyString_AsString          	, pfs_o,	  "PyString_AsString");
	LOADPY(pPyObject_Str               	, pfo_o,	  "PyObject_Str");
	LOADPY(pPyErr_Fetch					, pfv_popopo, "PyErr_Fetch");
	LOADPY(pPy_CompileStringFlags	    , pfo_ssip,	  "Py_CompileStringFlags");
	LOADPY(pPyDict_SetItemString       	, pfi_oso,	  "PyDict_SetItemString");
	LOADPY(pPyEval_GetBuiltins         	, pfo,		  "PyEval_GetBuiltins");
	LOADPY(pPy_Finalize                	, pfv,		  "Py_Finalize");
	LOADPY(pPy_InitModule4				, pfo_smsoi,  "Py_InitModule4");
	LOADPY(pPy_Initialize              	, pfv,		  "Py_Initialize");
	LOADPY(pPy_SetProgramName          	, pfv_s,	  "Py_SetProgramName");

	//P2N("LoadPython End\r\n");
	return 0;
}
