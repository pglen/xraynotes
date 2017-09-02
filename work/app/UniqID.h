// UniqID.h: interface for the CUniqID class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UNIQID_H__205B19D5_6BC9_4D62_8C32_AC944D006D38__INCLUDED_)
#define AFX_UNIQID_H__205B19D5_6BC9_4D62_8C32_AC944D006D38__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CUniqID  
{
public:
	void	unPack(void *ptr);
	void	Pack(void **ptr);
	void	*Pack();
	void	PackFree(void *ptr);

	int		Compare(CUniqID *other);
	void	Copy(CUniqID *id);
	void	Create(const char *str);
	void	Create();
	CString GetStr();

	inline	CUniqID& operator = (CUniqID *id) 
			{ Copy(id); return *this; };
	
	inline  CUniqID& operator = (const char *str) 
			{ Create(str); return *this; };

	inline	operator CString() 
			{ return GetStr(); };
	
	inline	int operator == (CUniqID other) 
			{ return Compare(&other); };
	
	CUniqID();
	virtual ~CUniqID();

	unsigned long unique[8];

#if 0
	unsigned char part_1[4];
	unsigned char part_2[4];
	unsigned char part_3[4];
	unsigned char part_4[4];
	unsigned char part_5[4];
	unsigned char part_6[4];
	unsigned char part_7[4];
	unsigned char part_8[4];
#endif


};

#endif // !defined(AFX_UNIQID_H__205B19D5_6BC9_4D62_8C32_AC944D006D38__INCLUDED_)
