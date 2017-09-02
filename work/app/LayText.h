
/* =====[ LayText.h ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the LayText.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  11/28/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// LayText.h: interface for the CLayText class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LAYTEXT_H__04B8C211_378C_4AFE_AC6A_21ECD73052B3__INCLUDED_)
#define AFX_LAYTEXT_H__04B8C211_378C_4AFE_AC6A_21ECD73052B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Layer.h"

class CLayText  : public CLayer

{

public:

	friend class CLayer;
	friend class CTextProp;

typedef struct _TXData
	{
	int		text_paddingx, text_paddingy;
	int		text_align, text_3d, text_ontop;
	int		text_round;
	} TXData;

public:

	//int		Init(POINT pt, int dummy1 = 0, int dummy2 = 0);
	void	SetNewPadding(int padx, int pady);
	void	SetNewText(const char *str);
	void	SetNewFont();
	
	CLayText();
	virtual ~CLayText();

	TXData		txdata, old_txdata;
	CString		text;	


	int		Prop(int dummy1 = 0, int dummy2 = 0, int dummy3 = 0);
	//int		Invalidate(int dummy1 = 0, int dummy2 = 0, int dummy3 = 0);
	//int		HitTest(POINT pt, int hit, int dummy2 = 0);
	int		KeyEvent(UINT event, UINT nFlags, UINT nChar);
	int		MouseEvent(UINT event, UINT nFlags, CPoint point);
	int		Init(POINT pt, int dummy1 = 0, int dummy2 = 0);
	int		Draw(CDC *pDC, float factor, int dummy1 = 0);

protected:

	// Text parameters

private:

	int		text_cursor, text_lines;
	LOGFONT	text_logfont;
	CFont	text_font;

private:

	void Render_Text(float factor);
};

#endif // !defined(AFX_LAYTEXT_H__04B8C211_378C_4AFE_AC6A_21ECD73052B3__INCLUDED_)
