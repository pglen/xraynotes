#if !defined(AFX_PYGLG_H__0A1D309C_4B01_4FDD_A516_8990387D10BD__INCLUDED_)
#define AFX_PYGLG_H__0A1D309C_4B01_4FDD_A516_8990387D10BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PyGlg.h : header file
//

#include "Python.h"
#include "GrayDlg.h"

//////////////////////////////////////////////////////////////////////////
// Pointer to functions

// Mnemonics: p=pointer f=function v=void s=strptr o=objptr var=variable args

// Read: pfv		=> pointer to function returning void
// Read: pfo_svar	=> ptr to func ret object, arguments: str and variable args

typedef PyAPI_FUNC(void)		(*pfv)();
typedef PyAPI_FUNC(PyObject *)  (*pfo)();
typedef PyAPI_FUNC(char *)		(*pfs)();
typedef PyAPI_FUNC(void)		(*pfv_s)(const char *);
typedef PyAPI_FUNC(PyObject *)	(*pfo_osvar)(PyObject *, char *, ...);
typedef PyAPI_FUNC(PyObject *)	(*pfo_oio)(PyObject *, Py_ssize_t, PyObject *);
typedef PyAPI_FUNC(PyObject *)	(*pfo_svar)(char *, ...);
typedef PyAPI_FUNC(PyObject *)	(*pfo_s)(char *);
typedef PyAPI_FUNC(PyObject *)	(*pfo_i)(int);
typedef PyAPI_FUNC(void)		(*pfv_os)(PyObject *, char *);
typedef PyAPI_FUNC(PyObject *)	(*pfo_soo)(char *, PyObject *, PyObject *);
typedef PyAPI_FUNC(PyObject *)	(*pfo_pi)(void *, Py_ssize_t);
typedef PyAPI_FUNC(void)		(*pfv_o)(PyObject *);
typedef PyAPI_FUNC(PyObject *)	(*pfo_moo)(PyCodeObject *, PyObject *, PyObject *);
typedef PyAPI_FUNC(PyObject *)	(*pfo_ooo)(PyObject *, PyObject *, PyObject *);
typedef PyAPI_FUNC(void)		(*pfv_ooo)(PyObject *, PyObject *, PyObject *);
typedef PyAPI_FUNC(char *)		(*pfs_o)(PyObject *);
typedef PyAPI_FUNC(PyObject *)	(*pfo_o)(PyObject *);
typedef PyAPI_FUNC(void)		(*pfv_popopo)(PyObject **, PyObject **, PyObject **);
typedef PyAPI_FUNC(PyObject *)	(*pfo_ssip)(const char *, const char *, int, PyCompilerFlags *);
typedef PyAPI_FUNC(int)			(*pfi_oso)(PyObject *, const char *, PyObject *);
typedef PyAPI_FUNC(PyObject *)	(*pfo_smsoi)(const char *, PyMethodDef *, const char *, PyObject*, int);


extern pfs			pPy_GetVersion                  ;
extern pfo			pPyDict_New 					;
extern pfo_svar 	pPy_BuildValue				;
extern pfo_osvar	pPyArg_ParseTuple       ;
extern pfo_s		pPyString_FromString		;
extern pfo_oio		pPyTuple_SetItem            ;
extern pfo_i		pPyTuple_New				;
extern pfv_os		pPyErr_SetString            ;
extern pfo_soo		pPyErr_NewException         ;
extern pfo_pi		pPyBuffer_FromReadWriteMemory;
extern pfv_o		pPyDict_Clear               ;
extern pfo_moo		pPyEval_EvalCode            ;
extern pfv			pPyErr_Clear                    ;
extern pfv_ooo		pPyErr_Restore              ;
extern pfs_o		pPyString_AsString          ;
extern pfo_o		pPyObject_Str               ;
extern pfv_popopo	pPyErr_Fetch            ;
extern pfo_ssip	pPy_CompileStringFlags  ;
extern pfi_oso		pPyDict_SetItemString       ;
extern pfo			pPyEval_GetBuiltins             ;
extern pfv			pPy_Finalize                    ;
extern pfo_smsoi	pPy_InitModule4         ;
extern pfv			pPy_Initialize                  ;
extern pfv_s		pPy_SetProgramName          ;

/////////////////////////////////////////////////////////////////////////////
// CPyGlg dialog

class CPyGlg : public CGrayDlg
{
// Construction
public:
	void LoadPython();

	HINSTANCE phand;
	int		pyerr;
	void	Minimize();
	int		CPyGlg::TryExec(const char *str, const char *arg);
	void	InitPy();
	void	Clear();
	void	Print(const char * str);
	int		doexit;

	CPyGlg(CWnd* pParent = NULL);   // standard constructor

	CPyGlg::~CPyGlg();

	int		running;
	//int		want_x, want_y;

	void	*parent;
	CString fstr;
	PyObject *xnt_module;

// Dialog Data
	//{{AFX_DATA(CPyGlg)
	enum { IDD = IDD_PYTHON };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPyGlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPyGlg)
	afx_msg void OnButton1();
	virtual BOOL OnInitDialog();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnDestroy();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	virtual void OnOK();
	afx_msg void OnButton8();
	afx_msg void OnButton9();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PYGLG_H__0A1D309C_4B01_4FDD_A516_8990387D10BD__INCLUDED_)
