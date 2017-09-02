
/* =====[ Flag.h ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the Flag.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  11/20/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// Flag.h: interface for the CFlag class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FLAG_H__2A489A6C_DEC9_42B8_994E_8215E2C70C9C__INCLUDED_)
#define AFX_FLAG_H__2A489A6C_DEC9_42B8_994E_8215E2C70C9C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFlag  

//pragma auto_inline(on)

{
public:
	
	inline int		IsFirstTime() { 
							if(value == 0)
								{value = 1; return true; }
							else 
								{ return false; }
							}

	inline int		Reset() { int lod = value;  value = 0; return lod; }
	inline int		Set() {	int lod = value; value = 1;	return lod; }

	inline int operator ==(int nval) {	return value == nval;}
	inline int operator =(int nval)  { value = nval; return value; }
	inline operator unsigned int () { return value; }

	int operator ~();
	int operator !();
	operator int ();
	
	bool	operator ||(int nval);

	CFlag();
	virtual ~CFlag();

//protected:

	int value;	
};

//pragma auto_inline(on)


#endif // !defined(AFX_FLAG_H__2A489A6C_DEC9_42B8_994E_8215E2C70C9C__INCLUDED_)
