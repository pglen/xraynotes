// LevenStein.h: interface for the LevenStein class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LEVENSTEIN_H__39CAA9C6_55BD_4F72_9A21_95DE640EC1F2__INCLUDED_)
#define AFX_LEVENSTEIN_H__39CAA9C6_55BD_4F72_9A21_95DE640EC1F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class LevenStein  
{
public:
	LevenStein();
	virtual ~LevenStein();

};

class Distance
{
  public:
    int LD (char const *s, char const *t);
  private:
    int Minimum (int a, int b, int c);
    int *GetCellPointer (int *pOrigin, int col, int row, int nCols);
    int GetAt (int *pOrigin, int col, int row, int nCols);
    void PutAt (int *pOrigin, int col, int row, int nCols, int x);
}; 

#endif // !defined(AFX_LEVENSTEIN_H__39CAA9C6_55BD_4F72_9A21_95DE640EC1F2__INCLUDED_)
