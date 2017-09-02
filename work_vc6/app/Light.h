// Light.h: interface for the CLight class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LIGHT_H__A723F715_F4D8_4E99_BBD7_135BF5C62109__INCLUDED_)
#define AFX_LIGHT_H__A723F715_F4D8_4E99_BBD7_135BF5C62109__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GrayDlg.h"

#include "Triangle.h"

class CLight : public CGrayDlg  

{

public:
	CLight();
	virtual ~CLight();

	// Dialog Data
	//{{AFX_DATA(CMagDlg)
	enum { IDD = IDD_DIALOG22 };
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLight)
	//}}AFX_VIRTUAL

	// Implementation

protected:

	// Generated message map functions
	//{{AFX_MSG(CLight)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_LIGHT_H__A723F715_F4D8_4E99_BBD7_135BF5C62109__INCLUDED_)
