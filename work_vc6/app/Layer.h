
/* =====[ Layer.h ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the Layer.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  9/17/2007  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LAYER_H__8AEA0781_4873_4E0C_8809_E3FFAE93A848__INCLUDED_)
#define AFX_LAYER_H__8AEA0781_4873_4E0C_8809_E3FFAE93A848__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////
// What kind of layer is ...

#define		LAYER_NONE		0				// Placeholder
#define		LAYER_LINES		1				// Lines collection (drawing)
#define		LAYER_SLINE		2				// Single Line
#define		LAYER_RECT		3				// Rectangle
#define		LAYER_ELLIPSE	4				// Circle/Ellipse
#define		LAYER_TEXT		5				// 
#define		LAYER_IMAGE		6				// Image
#define		LAYER_MAG		7				// Magnifier Glass
#define		LAYER_EYE		8				// Red eye filter
#define		LAYER_MEASURE	9				// Layer measure
#define		LAYER_SVG		10				// Scalable vector draw
#define		LAYER_CONT		11				// Container for other objects

//define ZEROSTR(str) memset(str, 0, sizeof(str));
//define ZEROMEM(mem, size) memset(mem, 0, size);

//////////////////////////////////////////////////////////////////////////
// Mouse action constants

#define		MOUSE_LEFTDOWN		1
#define		MOUSE_LEFTUP		2
#define		MOUSE_LEFTDBL		3

#define		MOUSE_RIGHTDOWN		4
#define		MOUSE_RIGHTUP		5
#define		MOUSE_RIGHTDBL		6

#define		MOUSE_MOVE			7

//////////////////////////////////////////////////////////////////////////
// Keyboard action constants

#define		KEYDOWN			11
#define		KEYUP			12
#define		KEYCHAR			13
#define		SYSKEYDOWN		14
#define		SYSKEYUP		15
#define		SYSCHAR			16

//////////////////////////////////////////////////////////////////////////
// Sub Layer types

#define LAYER_MAGIC 0x1234

//////////////////////////////////////////////////////////////////////////
// Layer state definitions

#define	STATE_IDLE			0
#define	STATE_INIT			1
#define	STATE_MOVE			2
#define STATE_MOVE2			4
#define STATE_MOVE3			8
#define	STATE_RESIZE		16
#define	STATE_ROTATE		32

#define	STATE_INACTIVE		64

#include "UniqID.h"

#define APPLY_TRANS		1
#define APPLY_ROUND		2
#define APPLY_3D		3
#define APPLY_PAD		4
#define APPLY_BG		5
#define APPLY_FG		6
#define	APPLY_LWIDTH	7
#define	APPLY_START		8
#define	APPLY_END		9
#define	APPLY_LROUND	10
#define	APPLY_ARRLEN	11
#define	APPLY_ARRANG	12
#define	APPLY_ARRLEN2	13
#define	APPLY_ARRANG2	14
#define APPLY_LTYPE		15
#define APPLY_BRTYPE	16
#define APPLY_BRHATCH	17

#include "xTracker.h"

//////////////////////////////////////////////////////////////////////////
#include "memdc.h"

class CLayer  

{
	CUniqID	instance, type, proc, uext;

public:

	int		save;
	int		in_draw;
	// Methods callable externally
	void	Resize(int wto, int hto);
	void	Move(int xto, int yto);
	void	SetFocus(int foc = true);
	
	// Overridden methods
	int		Invalidate(int dummy1 = 0, int dummy2 = 0, int dummy3 = 0);
	int		HitTest(POINT pt, int dummy1 = 0, int dummy2 = 0);
	int		Prop(int dummy1, int dummy2, int dummy3);
	int		Init(POINT pt, int dummy1 = 0, int dummy2 = 0);
	int		KeyEvent(UINT event, UINT nFlags, UINT nChar);
	int		MouseEvent(UINT event,  UINT nFlags, CPoint point);
	int		Draw(CDC* pDC, float factor, int dummy1 = 0);

public:

	void	GetRect(RECT *rc);
	void	GetRect(CRect *rc);

	// Debug Aid
	void	QDump();
	void	Dump();
	char	*GetKindStr();

	// Data
	int		focus, state, magic;
	int		dragoffx, dragoffy;

	CRect	oldupdrect, updaterect;
	RECT	startrect, endrect;

	CLayer();
	CLayer(int kind);

	virtual ~CLayer();

	void DrawTrackerX(CDC *pDC, CRect *ur, int circ = false);
	void DrawTracker(CDC *pDC, CRect *rc, int twidth, int circ);
	void DrawFocus(CDC *pDC, RECT *rc, int solid = false);

protected:

	CString tiptext;
	void	GetMemDC(CDC *pDC, CDC *dcMem);
	void	GetMemDC(CDC *pDC, CDC *dcMem, int ww, int hh);

	void	PutMemDC(CDC *pDC, CDC *dcMem);
	void	PutMemDC(CDC *pDC, CDC *dcMem, CRect *rc2);

	void	FreeMemDC(CDC *pDC, CDC *dcMem);

	CRect	oldrc, newrc;
	float	gl_factor;

	CRect	dcarea;

	HBITMAP oldbm, bm;

	CxTracker tracker;

private:

	// Helpers
	char	*Ma2Str(int mact);
	int		in_mouse;
	

public:
	void Rotate(int deg);

//////////////////////////////////////////////////////////////////////////
// Global data

typedef struct _CgData

{
	int		kind; 
	int		lock, proccode, loccode;
	int		ver, magic;
	int		groupid;
	
	// Main coordinates
	int		xx, yy,	ww, hh;

	// Apply fg color to obj: 
	//		Text: text color, 
	//		Shapes: pen for outline

	COLORREF	bg_color, fg_color;
	COLORREF	hatch_color;

	// Misc. flags
	int		visible, zorder;
	int		inline_menu;
	int		protect;

	int		drag, drag_x, drag_y;
	int		resize, resize_x, resize_y;
	int		rotate, rotate_x, rotate_y;
	int		rotate_angle, rotate_org;
	int		locked, alpha;
	int		line_width, line_type;
	int		brush_type, brush_hatch; 
	int		group;
	//int		degree;

} CgData;

//////////////////////////////////////////////////////////////////////////
// Private data

typedef struct _CpPata

{
	// Transitional sizes
	int		new_ww, new_hh;
	int		old_xx, old_yy, old_ww, old_hh, old_rot;

	// Iconify subsystem
	int		iconify;
	int		old_ico_xx, old_ico_yy;

	// Procedural codes subsystem
	//CString procstr, procdescr, reserved;	

} CpData;

	CgData	gdata, old_gdata;
	CpData	pdata;
	
protected:

	void InitVars();

};

// Included here for convenience

#include "Layrect.h"
#include "LayCirc.h"
#include "Laytext.h"
#include "LayLines.h"
#include "LaySline.h"
#include "LayLines.h"
#include "LayImg.h"

#endif // !defined(AFX_LAYER_H__8AEA0781_4873_4E0C_8809_E3FFAE93A848__INCLUDED_)
