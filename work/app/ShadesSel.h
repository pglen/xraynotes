// ShadesSel.h: interface for the CShadesSel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHADESSEL_H__DC3E1AB6_C2DC_4E66_997C_2A9B1B0AC170__INCLUDED_)
#define AFX_SHADESSEL_H__DC3E1AB6_C2DC_4E66_997C_2A9B1B0AC170__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define     UL  1
#define     UR  2
#define     LL  3
#define     LR  4

#define     UM  8
#define     RM  9
#define     BM  10
#define     LM  11

#define     HH  32

#define     WHOLE   16

class CShadesSel  
{
public:

	friend class CShadesDlg;

	CShadesSel(void *ptr);
	virtual ~CShadesSel();

	void CShadesSel::MarkToRender(int toimg);
	void CShadesSel::SelLdown(CPoint point);
	void CShadesSel::MarkLdown(CPoint point);
	void CShadesSel::MarkMove(CPoint point);
	void CShadesSel::RectMove(CPoint point);
	void CShadesSel::SelMove(CPoint point);
	void CShadesSel::SelToRender(int toimg);
	void CShadesSel::CalcSelRect(RECT *rc);


protected:

	CShadesDlg *dlg;
	int		layprog;
};

#endif // !defined(AFX_SHADESSEL_H__DC3E1AB6_C2DC_4E66_997C_2A9B1B0AC170__INCLUDED_)
