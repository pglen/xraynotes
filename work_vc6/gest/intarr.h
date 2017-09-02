////////////////////////////////////////////////////////////////////////////

class CIntArray : public CObject
{

	DECLARE_DYNAMIC(CIntArray)
public:

// Construction
	CIntArray();

// Attributes
	int		GetSize();
	int		GetUpperBound();
	void	SetSize(int nNewSize, int nGrowBy = -1);

// Operations
	// Clean up
	void	FreeExtra();
	void	RemoveAll();

	// Accessing elements
	int		GetAt(int nIndex);
	void	SetAt(int nIndex, int newElement);

	// Potentially growing the array
	void	SetAtGrow(int nIndex, int newElement);

	int		Add(int newElement);

	int		Append(const CIntArray& src);
	void	Copy(const CIntArray& src);

	// overloaded operator helpers
	int		operator[](int nIndex) const;
	int&	operator[](int nIndex);

	// Operations that move elements around
	void	InsertAt(int nIndex, int newElement, int nCount = 1);
	void	RemoveAt(int nIndex, int nCount = 1);
	void	InsertAt(int nStartIndex, CIntArray* pNewArray);

// Implementation
protected:
	int	*m_pData;   // the actual array of data
	int m_nSize;     // # of elements (upperBound - 1)
	int m_nMaxSize;  // max allocated
	int m_nGrowBy;   // grow amount

public:
	~CIntArray();
#ifdef _DEBUG
	void Dump(CDumpContext&) const;
	void AssertValid() const;
#endif

protected:
	// local typedefs for class templates
	typedef int BASE_TYPE;
	typedef int BASE_ARG_TYPE;
};


