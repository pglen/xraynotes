
class	CFontItem
{
public:

	int magic;
	
	LOGFONT	lf;
	CFont font;
	TEXTMETRIC tm;
	int	color;

	CFontItem() { magic = FOMAGIC;}
	CFontItem(LOGFONT *plf) 
		{ 
		magic = FOMAGIC; lf = *plf; color = RGB(0,0,0);
		font.CreateFontIndirect(&lf);
		
		CPaintDC dc(GetDesktopWindow());
		int sss = dc.SaveDC();
		CFont *old = (CFont*)dc.SelectObject(font);
		dc.GetTextMetrics(&tm);
		dc.RestoreDC(sss);

		//P2N("tm.tmDescent=%d tmAscent=%d\r\n", tm.tmDescent, tm.tmAscent ); 
		}
};

//////////////////////////////////////////////////////////////////////////

class	CCharItem
{
public:

	int		magic;				// Sanity check value

	TCHAR 	chh;				// Actual character
	DWORD	fg;					// fg color
	DWORD	bg;					// bg color
	DWORD	ww;					// width
	DWORD	ff;					// enum fonts numbers
	
	CCharItem()	 {magic = CHMAGIC; fg=ww=ff=0, bg=RGB(255,255,255);}
	~CCharItem() {ASSERT(magic == CHMAGIC);}
};

//////////////////////////////////////////////////////////////////////////

class	CLineItem
{
public:

	int			magic;			// Sanity check value
	int			num;			// Line number
	int			height;			// Calculated height of largest
	int			base;			// Baseline of largest 
	int			align;			// Alignment of the line
	int			start;			// Alignment start value
	
	CString		line;			// Actual line string 
	CxPtrArr	charr;			// Array of char descriptors

	CLineItem() {magic = LIMAGIC; num = 0; height = 0; 
					align = 0; base = 0; start = 0; }
	~CLineItem() 
		{
		ASSERT(magic == LIMAGIC);
		for(int loop = 0; loop < charr.GetSize(); loop++)
			{
			delete (CCharItem*)charr[loop];
			}
		}
};
