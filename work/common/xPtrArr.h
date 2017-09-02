
/* =====[ xPtrArr.h ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the xPtrArr.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  12/20/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// xPtrArr.h: interface for the CxPtrArr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XPTRARR_H__BBEF6A88_28E3_49C4_BA73_54244624BD14__INCLUDED_)
#define AFX_XPTRARR_H__BBEF6A88_28E3_49C4_BA73_54244624BD14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define PTRARR_MAGIC	0x232323

#include "../common/mxpad.h"


//////////////////////////////////////////////////////////////////////////

/// \brief %CxPtrArr array of void pointers

/// Plugin replacement for the MFC %CPtrArray.\n
/// \n
/// Additional Functions implemented:
/// \li Find, \li AddUnique, \li RemoveValue, \li ...\n
///
/// Extensive use of the macro ASSERT(magic == PTRARR_MAGIC); for
///  sanity checks.\n
/// The DEBUG version ASSERTS on some out of bound conditions, the Release 
/// version checks for overflow, and in that case it does a NOOP

class CxPtrArr 

{

public:

//////////////////////////////////////////////////////////////////////////
	
class CxPtrArrExc
{
	CString str;

public:

    CxPtrArrExc(){};
    CxPtrArrExc(const char *cstr){str = cstr;};
    ~CxPtrArrExc(){};
    
	const char *ShowReason() const { return str; }

};

public:

	void	*dummy;	
	int		Find(const void *ptr);
	int		AddUniq(const void *ptr);
	void	RemoveValue(void *ptr);

	CxPtrArr();
	virtual ~CxPtrArr();

	int		IsValidIdx(int idx);


public:

	// Attributes

	/// Set New Size, Expand if neccessary
	void SetSize(int nNewSize, int nGrowBy = -1);

	// Operations

	/// Free extra allocated space
	void FreeExtra();

	// Potentially growing the array
	// Growing functions:

	/// Set array value, grow it if needed
	void	SetAtGrow(int nIndex, void* newElement);

	int		Append(const CxPtrArr& src);
	void	Copy(const CxPtrArr& src);

	// Operations that move elements around
	void InsertAt(int nIndex, void* newElement, int nCount = 1);
	void RemoveAt(int nIndex, int nCount = 1);
	void InsertAt(int nStartIndex, CxPtrArr* pNewArray);

	// Implementation

protected:

	void** m_pData;		// the actual array of data
	int m_nSize;		// # of elements (upperBound - 1)
	int m_nMaxSize;		// max allocated
	int m_nGrowBy;		// grow amount
	
public:

	int magic;

	////////////////////////////////////////////////////////////////////////////

public:
	void Dump();

	/// \brief Returns the size of the array. 
	///
	/// Since indexes are zero-based,  the size is 1 greater than the largest 
	/// index

	inline int GetSize() const
		{ return m_nSize; }

	/// \brief Return the index of the upper bound (zero-based).
	///
	/// Returns the current upper bound of this array. Because array indexes are zero-based, 
	/// this function returns a value 1 less than GetSize. \n
	/// The condition %GetUpperBound( ) = -1 indicates that the array contains no elements.

	inline int GetUpperBound() const
		{ return m_nSize - 1; }

	/// \brief Removes all data from the array.

	///	Removes all the pointers from this array but does not actually delete what is behind the pointers. 
	///	If the array is already empty, the function still works. 
	///	The RemoveAll function frees all memory used for pointer storage.

	inline void RemoveAll()
		{ SetSize(0); }

	//////////////////////////////////////////////////////////////////////////
	// Accessing elements

	/// Get member helper
	inline void* GetAt(int nIndex) const
		{ ASSERT(nIndex >= 0 && nIndex < m_nSize);
			return m_pData[nIndex]; }

	/// Get member helper, exception raised
	inline void* GetAtR(int nIndex) const
		{ 
		if(nIndex < 0)
			{	
			throw(CxPtrArrExc("CxPtrArr: Out of bounds (index < 0)"));
			return dummy;
			}
		if(nIndex >= m_nSize)
			{
			throw(CxPtrArrExc("CxPtrArr: Out of bounds (index > m_nSize)"));
			return dummy;			
			}
		return m_pData[nIndex]; 
		}

	/// Set member helper
	inline void SetAt(int nIndex, void* newElement)
		{ ASSERT(nIndex >= 0 && nIndex < m_nSize);
			m_pData[nIndex] = newElement; }

	/// Get member helper
	inline void*& ElementAt(int nIndex)
		{ ASSERT(nIndex >= 0 && nIndex < m_nSize);
			return m_pData[nIndex]; }

	/// Get member helper
	inline void*& ElementAtR(int nIndex)
		{ 
		//ASSERT(nIndex >= 0 && nIndex < m_nSize);
		if(nIndex < 0)
			{	
			throw(CxPtrArrExc("CxPtrArr: Out of bounds (index < 0)"));
			return dummy;
			}
		if(nIndex >= m_nSize)
			{
			throw(CxPtrArrExc("CxPtrArr: Out of bounds (index > m_nSize)"));
			return dummy;			
			}

		return m_pData[nIndex]; }

	/// Get pointer to actual array. Use it with care
	inline const void** CxPtrArr::GetData() const
		{ return (const void**)m_pData; }

	/// Get pointer to actual array. Use it with care
	inline void** CxPtrArr::GetData()
		{ return (void**)m_pData; }

	// Add new element at the end
	inline int Add(void* newElement)
		{ int nIndex = m_nSize;	SetAtGrow(nIndex, newElement);
			return nIndex; }

	//////////////////////////////////////////////////////////////////////////
	// Overloaded operator helpers

	/// Casting to integer will yield the size of the array
	inline 	operator int () 
		{ return m_nSize; }

	/// Casting to const integer will yield the size of the array
	inline 	operator int() const
		{ return m_nSize; }

	// Direct Access to the element data (may return NULL)

	/// Array access to members of the class data
	inline void* operator[](int nIndex) const
		{ return GetAtR(nIndex); }

	/// Array access to members of the class data
	inline void*& operator[](int nIndex)
		{ return ElementAtR(nIndex); }

	// Append item
	inline CxPtrArr& 	operator += (void *item) 
		{
		Add(item);	return *this;
		}
};

#endif // !defined(AFX_XPTRARR_H__BBEF6A88_28E3_49C4_BA73_54244624BD14__INCLUDED_)
