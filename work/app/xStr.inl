
/* =====[ xStr.inl ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the xStr.inl                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  12/24/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// Functions included here are candidates for inline 

// Debug aids:
	
inline void CxStr::_PostConstruct()

{
#ifdef _DEBUG
	CxStr_UseArr.Add(this);
#endif
}

inline void CxStr::_PostDestruct()

{
#ifdef _DEBUG

	// Remove from global array
	int len = CxStr_UseArr.GetSize();

	// Scan backwards, most likely push/pull
	for (int loop = len - 1; loop >= 0; loop--)
		{
		CxStr *str = (CxStr *)CxStr_UseArr[loop];	
		ASSERT(str->magic == STR_MAGIC);

		//P2N("CxStr::~CxStr Scanning: %p\r\n", str);

		if(str == this)
			{
			CxStr_UseArr.RemoveAt(loop);
			break;
			}
		}

	#endif
}
