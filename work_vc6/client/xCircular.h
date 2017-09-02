// xCircular.h: interface for the CxCircular class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XCIRCULAR_H__3BA24507_4AAB_4B3C_94D1_E498A0165A19__INCLUDED_)
#define AFX_XCIRCULAR_H__3BA24507_4AAB_4B3C_94D1_E498A0165A19__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define CIRC_DEF_SIZE	4096

class CxCircular  

{
public:

	int Add(char chh);
	int Get(char *chh);

	CxCircular();
	CxCircular(int bufsize);

	virtual ~CxCircular();

protected:

	int head, tail, size;
	char	*buff;
	InitVars();

};

#endif // !defined(AFX_XCIRCULAR_H__3BA24507_4AAB_4B3C_94D1_E498A0165A19__INCLUDED_)
