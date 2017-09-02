#if !defined(AFX_DBDEF_H__20A1F573_0224_4579_A50E_629B53E2A16D__INCLUDED_)
#define AFX_DBDEF_H__20A1F573_0224_4579_A50E_629B53E2A16D__INCLUDED_

#include "editEx.h"

typedef struct _fields
		{
		CString name;
		CString value;
		CString value2;
		CString type;
		int		checked;
		CString	*var;
		CString	var2;
		CEditEx *ctrl;
		int		tipid;
		int		olddis;
		} fields;

typedef struct _flags
		{
		CString name;
		int		value;
		int		*var;
		CButton *ctrl;
		int		tipid;
		} flags;

#endif