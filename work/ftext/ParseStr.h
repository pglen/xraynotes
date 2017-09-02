// ParseStr.h: interface for the CParseStr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARSESTR_H__170445B6_8961_47D5_98D3_31A49543F711__INCLUDED_)
#define AFX_PARSESTR_H__170445B6_8961_47D5_98D3_31A49543F711__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CParseStr  
{
public:
	CParseStr();
	CParseStr::CParseStr(const char *text);

	int		CParseStr::GetCount();

	virtual ~CParseStr();

	void	ParseStr(CString &text);
	void	FreeParseStr();

	CString *GetAt(int idx);

private:

	CPtrArray  arr;
};

#endif // !defined(AFX_PARSESTR_H__170445B6_8961_47D5_98D3_31A49543F711__INCLUDED_)
