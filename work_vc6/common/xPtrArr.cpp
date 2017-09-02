
/* =====[ xPtrArr.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the xPtrArr.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  12/20/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#include "stdafx.h"
#include "xPtrArr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CxPtrArr::CxPtrArr()

{
//	AP2N("CxPtrArr::CxPtrArr() %p\r\n", this);

	magic = PTRARR_MAGIC;
	m_pData = NULL;
	m_nSize = m_nMaxSize = m_nGrowBy = 0;

	dummy = &magic;
}

CxPtrArr::~CxPtrArr()

{
	ASSERT(magic == PTRARR_MAGIC);

	//P2N("CxPtrArr::&CxPtrArr() %p\r\n", this);

	delete[] (BYTE*)m_pData;
}

// -----------------------------------------------------------------------

/// \brief Remove pointer by value. 

/// Will not continue after the first item is removed. 
/// (aka: will not delete a duplicate item)\n
/// Optimized to start from the end, as it assumes most likely scenario of
/// last in - first out)
	
void CxPtrArr::RemoveValue(void *ptr)

{
	ASSERT(magic == PTRARR_MAGIC);

	int size = GetSize();
	for(int loop = size - 1; loop >= 0; loop--)
		{
		if(GetAt(loop) == ptr)
			{	
			//P2N("CxPtrArr::RemovePtr removed %p\r\n", ptr);
			RemoveAt(loop);
			}
		}
}

/// \brief Add a pointer only if it is unique

/// Adds new entry if it is unique. (aka not in the array already)
/// Searches from the end, assuming LIFO (last in first out)

int CxPtrArr::AddUniq(const void *ptr)

{
	ASSERT(magic == PTRARR_MAGIC);

	int ret = -1, was = 0, size = GetSize();

	for(int loop = size - 1; loop >= 0; loop--)
		{
		if(GetAt(loop) == ptr)
			{	
			was = true;
			break;
			}
		}
	if(!was)
		{
		//P2N("CxPtrArr::AddUniq added %p\r\n", ptr);
		ret = Add((void*)ptr);
		}	
	return ret;
}


/// \brief Find the first pointer that matches ptr

/// Find existing (stored) pointer, return index to it if found. Return -1 if not found

int CxPtrArr::Find(const void *ptr)

{
	ASSERT(magic == PTRARR_MAGIC);

	int ret = -1, size = GetSize();

	for(int loop = size - 1; loop >= 0; loop--)
		{
		if(GetAt(loop) == ptr)
			{	
			//P2N("CxPtrArr::Find %p\r\n", ptr);
			ret = loop;
			break;
			}
		}
	return ret;
}

// --------------------------------------------------------------
//
/// \brief   Set Size of array grow if needed 
/// 
/// \details Grows/Truncates the array as needed
/// \n  
/// Return type:      void 
/// \n  
/// \param nNewSize Arg Type: int
/// \param nGrowBy Arg Type: int optional argument

void CxPtrArr::SetSize(int nNewSize, int nGrowBy)

{
	ASSERT(magic == PTRARR_MAGIC);

	ASSERT(nNewSize >= 0);

	if (nGrowBy != -1)
		m_nGrowBy = nGrowBy;  // set new size

	if (nNewSize == 0)
	{
		// shrink to nothing
		delete[] (BYTE*)m_pData;
		m_pData = NULL;
		m_nSize = m_nMaxSize = 0;
	}
	else if (m_pData == NULL)
	{
		// create one with exact size
		m_pData = (void**) new BYTE[nNewSize * sizeof(void*)];

		memset(m_pData, 0, nNewSize * sizeof(void*));  // zero fill

		m_nSize = m_nMaxSize = nNewSize;
	}
	else if (nNewSize <= m_nMaxSize)
	{
		// it fits
		if (nNewSize > m_nSize)
			{
			// initialize the new elements
			memset(&m_pData[m_nSize], 0, (nNewSize-m_nSize) * sizeof(void*));
			}
		m_nSize = nNewSize;
	}
	else
	{
		// otherwise, grow array
		int nGrowBy = m_nGrowBy;
		if (nGrowBy == 0)
		{
			// heuristically determine growth when nGrowBy == 0
			//  (this avoids heap fragmentation in many situations)
			nGrowBy = min(1024, max(4, m_nSize / 8));
		}
		int nNewMax;
		if (nNewSize < m_nMaxSize + nGrowBy)
			nNewMax = m_nMaxSize + nGrowBy;  // granularity
		else
			nNewMax = nNewSize;  // no slush

		ASSERT(nNewMax >= m_nMaxSize);  // no wrap around
		void** pNewData = (void**) new BYTE[nNewMax * sizeof(void*)];

		// copy new data from old
		memcpy(pNewData, m_pData, m_nSize * sizeof(void*));

		// construct remaining elements
		ASSERT(nNewSize > m_nSize);

		memset(&pNewData[m_nSize], 0, (nNewSize-m_nSize) * sizeof(void*));


		// get rid of old stuff (note: no destructors called)
		delete[] (BYTE*)m_pData;
		m_pData = pNewData;
		m_nSize = nNewSize;
		m_nMaxSize = nNewMax;
	}
}

int CxPtrArr::Append(const CxPtrArr& src)

{
	ASSERT(magic == PTRARR_MAGIC);

	ASSERT(this != &src);   // cannot append to itself

	int nOldSize = m_nSize;
	SetSize(m_nSize + src.m_nSize);

	memcpy(m_pData + nOldSize, src.m_pData, src.m_nSize * sizeof(void*));

	return nOldSize;
}

void CxPtrArr::Copy(const CxPtrArr& src)
{
	
	ASSERT(magic == PTRARR_MAGIC);

	ASSERT(this != &src);   // cannot append to itself

	SetSize(src.m_nSize);

	memcpy(m_pData, src.m_pData, src.m_nSize * sizeof(void*));
}

/////////////////////////////////////////////////////////////////////////
/// Shrink to desired size
///
/// Frees any extra memory that was allocated while the array was grown. 
/// This function has no effect on the size or upper bound of the array.
		
void CxPtrArr::FreeExtra()

{
	if (m_nSize != m_nMaxSize)
		{	
		void** pNewData = NULL;

		if (m_nSize != 0)
			{
			pNewData = (void**) new BYTE[m_nSize * sizeof(void*)];
		
			// Replicate Data
			memcpy(pNewData, m_pData, m_nSize * sizeof(void*));
			}

		// get rid of old stuff (note: no destructors called)
		delete[] (BYTE*)m_pData;
		m_pData = pNewData;	m_nMaxSize = m_nSize;
		}
}

/////////////////////////////////////////////////////////////////////////////

void CxPtrArr::SetAtGrow(int nIndex, void* newElement)

{
	ASSERT(magic == PTRARR_MAGIC);

	ASSERT(nIndex >= 0);

	//P2N("CxPtrArr::SetAtGrow index=%d\r\n", nIndex);

	if (nIndex >= m_nSize)
		SetSize(nIndex+1);

	m_pData[nIndex] = newElement;
}


void CxPtrArr::InsertAt(int nIndex, void* newElement, int nCount)
{
	ASSERT(magic == PTRARR_MAGIC);
	
	ASSERT(nIndex >= 0);    // will expand to meet need
	ASSERT(nCount > 0);     // zero or negative size not allowed

	if (nIndex >= m_nSize)
	{
		// adding after the end of the array
		SetSize(nIndex + nCount);  // grow so nIndex is valid
	}
	else
	{
		// inserting in the middle of the array
		int nOldSize = m_nSize;
		SetSize(m_nSize + nCount);  // grow it to new size
		// shift old data up to fill gap
		memmove(&m_pData[nIndex+nCount], &m_pData[nIndex],
			(nOldSize-nIndex) * sizeof(void*));

		// re-init slots we copied from

		memset(&m_pData[nIndex], 0, nCount * sizeof(void*));
	}

	// insert new value in the gap
	ASSERT(nIndex + nCount <= m_nSize);

	// copy elements into the empty space
	while (nCount--)
		m_pData[nIndex++] = newElement;

}

//////////////////////////////////////////////////////////////////////////

void CxPtrArr::RemoveAt(int nIndex, int nCount)

{	
	ASSERT(nIndex >= 0);	ASSERT(nCount >= 0);
	
	//ASSERT(nIndex + nCount <= m_nSize);

	if(nIndex + nCount > m_nSize)
		{
		P2N("CxPtrArr::RemoveAt Removing from past the end nIndex=%d nCount=%d m_nSize=%d\r\n", 
			nIndex, nCount, m_nSize);
		return;
		}

	//P2N("CxPtrArr::RemoveAt index=%d\r\n", nIndex);

	// just remove a range
	int nMoveCount = m_nSize - (nIndex + nCount);

	if (nMoveCount)
		memmove(&m_pData[nIndex], &m_pData[nIndex + nCount],
			nMoveCount * sizeof(void*));
	m_nSize -= nCount;
}

void CxPtrArr::InsertAt(int nStartIndex, CxPtrArr* pNewArray)

{	
	ASSERT(pNewArray != NULL);
	ASSERT(nStartIndex >= 0);

	if (pNewArray->GetSize() > 0)
	{
		InsertAt(nStartIndex, pNewArray->GetAt(0), pNewArray->GetSize());

		for (int i = 0; i < pNewArray->GetSize(); i++)
			SetAt(nStartIndex + i, pNewArray->GetAt(i));
	}
}

void CxPtrArr::Dump()

{
	P2N(
	"CxPtrArr::Dump data=0x%p m_nSize=%d m_nMaxSize=%d m_nGrowBy=%d\r\n",
					m_pData, m_nSize, m_nMaxSize, m_nGrowBy);
}

//////////////////////////////////////////////////////////////////////////
// Soft index 

int CxPtrArr::IsValidIdx(int idx)
{

	if(idx >= 0 && idx < m_nSize)
		return true;
	else
		return false;
}
