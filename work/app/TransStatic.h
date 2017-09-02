#pragma once

// CTransStatic

class CTransStatic : public CStatic
{
	DECLARE_DYNAMIC(CTransStatic)

public:
	CTransStatic();
	virtual ~CTransStatic();
		
// Dialog Data
	//{{AFX_DATA(CTransStatic)
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTransStatic)
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTransStatic)
	afx_msg void OnPaint();
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()

};

