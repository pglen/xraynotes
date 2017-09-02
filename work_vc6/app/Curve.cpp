// Curve.cpp : implementation file
//

#include "stdafx.h"
#include "xraynotes.h"
#include "Curve.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CCurve

CCurve::CCurve()
{
	arr = NULL;
	curdrag = false;
	inmm = false;
	curbusy = false;
	mousedist = 2;
	olddoc = NULL;
	low = 0, high = 255;
	lowlim = 0, highlim = 255;
}

CCurve::~CCurve()
{
	if( arr) free( arr);
	if(xarr) free(xarr);
	if(yarr) free(yarr);
}

BEGIN_MESSAGE_MAP(CCurve, CBaseWnd)
	//{{AFX_MSG_MAP(CCurve)
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCurve message handlers

void CCurve::OnMouseMove(UINT nFlags, CPoint point) 

{	
	CPoint npoint;
	
	RECT rc; GetClientRect(&rc);

	// Copy and adjust newpoint
	npoint.y = max(0, rc.bottom - point.y);  				// Fake mirrored y coordinates
	npoint.x = max(0, min(point.x, rc.right - 1));			// Contain mouse 

	//P2N("CCurve::OnMouseMove drag=%d pp=%d x=%d y=%d\r\n", curdrag, match, point.x, point.y);
	//P2N("Newpoint x=%d y=%d\r\n", npoint.x, npoint.y);

	if(!curdrag)
		{
		RECT mrc;

		int hit = 0;

		mrc.top		= npoint.y - mousedist;
		mrc.bottom	= npoint.y + mousedist;
		mrc.left	= npoint.x - mousedist;
		mrc.right	= npoint.x + mousedist;

		for(int loop = 0; loop < rc.right; loop++)	
			{
			if(::PtInRect(&mrc, arr[loop]))
				{
				//P2N("Match at %d\r\n", loop);
				hit = true;
				break;
				}
			}
		if(hit)
			SetCursor(grab);
		else
			SetCursor(LoadCursor(NULL, IDC_ARROW));
		
		CBaseWnd::OnMouseMove(nFlags, point);
		return;
		}

	if(inmm)
		{
		P2N("Reentering CBaseWnd::OnMouseMove\r\n");
		CBaseWnd::OnMouseMove(nFlags, point);
		return;
		}
	
	inmm = true;

	// Set distance as proportional diff
	int newpos = rc.bottom - point.y;
	double diff = newpos - arr[match].y;

	// Copy forth
	//for(int loop = 0; loop < rc.right; loop++)
	//	{
	//	xarr[loop].y = arr[loop].y;
	//	}

	// Draw line between curr point and extremes (or bounds)
	int high =  min(npoint.x + count, rc.right - 1);
	int low  =  min(max(npoint.x - count, 1), rc.right - 1);

	double rat = (double)(xarr[high].y - npoint.y) / (high - npoint.x);
	
	//P2N("Rat=%f arr[high].y =%d\r\n", rat, arr[high].y );

	for(int loop1 = npoint.x; loop1 < high; loop1++)
		{
		xarr[loop1].y  = (long)(npoint.y + double(loop1 - npoint.x) * rat);
		xarr[loop1].y  = min(max(xarr[loop1].y, 1), rc.bottom);
		}

	double rat2 = (double)(npoint.y - xarr[low].y) / (npoint.x - low);
	for(int loop1a = npoint.x; loop1a >= low; loop1a--)
		{
		xarr[loop1a].y  = long(npoint.y - double(npoint.x - loop1a) * rat2);
		xarr[loop1a].y  = min(max(xarr[loop1a].y, 1), rc.bottom);
		}
	
	// Bigger range for the smoothing (merge it better)
	high =  min(npoint.x + (count + count / 2) , rc.right - 1);
	low  =  max(npoint.x - (count + count / 2), 0);

	SmoothCurve(xarr, false, count / 2, low, high);

	// Lessen artifacts
	// SmoothCurve(xarr, false, 2);
	
	int cnt = 0;
	
	inmm = false;

	Invalidate();
	support.HeapCheck("MouseMove", false);

	CBaseWnd::OnMouseMove(nFlags, point);
}


void CCurve::OnPaint() 

{
	//P2N("CCurve::OnPaint\r\n");

	POINT	*pt;

	if(curdrag) pt = xarr;
	else	 pt = arr;

	//CClientDC dc(this);	
	CPaintDC	dc(this);

	RECT frc; GetClientRect(&frc);
		
	dc.FillSolidRect(&frc, RGB(128, 128, 128));
	dc.Draw3dRect(&frc, RGB(200,200,200), RGB(220,220,220) );

	RECT rc; GetClientRect(&rc);

	//CBrush gray(RGB(128,128, 128));
	//CBrush *old = (CBrush*)dc.SelectObject(gray);

	// Draw Histogram
	CPen pen(PS_SOLID , 1, RGB(100, 100, 100));
	CPen *oldpen = (CPen*)dc.SelectObject(pen);

	for(int loop = 0; loop < rc.right - 2; loop++)
		{
		int xxx = 255 * loop / rc.right;

		dc.MoveTo(loop,  rc.bottom - 2);
		dc.LineTo(loop, (rc.bottom - 2)- rc.bottom * hg[xxx] / 256);

		//P2N("xxx=%d hg[xxx]=%d\r\n", xxx, hg[xxx]);
		}
	
	CPen pen2(PS_SOLID , 1, RGB(0, 0, 0));
	(CPen*)dc.SelectObject(pen2);
	
	dc.MoveTo(0, rc.bottom);
	for(int loop2 = 0; loop2 < rc.right; loop2++)
		{
		//P2N(" %d  ", step * loop);
		//dc.SetPixel(arr[loop].x, rc.bottom - arr[loop].y, RGB(00, 00, 00));
		dc.LineTo(pt[loop2].x, rc.bottom - pt[loop2].y); //, RGB(00, 00, 00));
		}

	//dc.SelectObject(oldpen);

	CBaseWnd::OnPaint();
}

//////////////////////////////////////////////////////////////////////////

void CCurve::OnInitControl()

{
	RECT rc; GetClientRect(&rc);

	//P2N("CCurve::InitControl ww=%d hh=%d \r\n", rc.right, rc.bottom );

	arr = (POINT*) malloc (sizeof(POINT) * rc.right);
	ASSERT(arr);

	xarr = (POINT*) malloc (sizeof(POINT) * rc.right);
	ASSERT(xarr);

	yarr = (POINT*) malloc (sizeof(POINT) * rc.right);
	ASSERT(yarr);

	grab = AfxGetApp()->LoadCursor(IDC_CURSOR6);
	ASSERT(grab);

	int loop;
	double step = (double)rc.bottom / rc.right;

	for(loop = 0; loop < rc.right; loop++)
		{
		arr[loop].x = loop;  arr[loop].y = (int)(step * loop);
		xarr[loop].x = loop; xarr[loop].y = (int)(step * loop);
		}

	SetTimer(1, 100, NULL);
	SetTimer(2, 1000, NULL);

	CBaseWnd::OnInitControl();
}

void CCurve::OnLButtonDown(UINT nFlags, CPoint point) 

{
	RECT rc; GetClientRect(&rc);

	P2N("CCurve::OnLButtonDown x=%d y=%d\r\n", point.x, point.y);

	RECT mrc; 
	int num_matches = 0;
	int last_match  = 0;

	// Fake mirrored y coordinates
	point.y = rc.bottom - point.y;

	mrc.top		= point.y - mousedist;
	mrc.bottom	= point.y + mousedist;
	mrc.left	= point.x - mousedist;
	mrc.right	= point.x + mousedist;

	for(int loop = 0; loop < rc.right; loop++)	
		{
		if(::PtInRect(&mrc, arr[loop]))
			{
			//P2N("Match at %d\r\n", loop);
			num_matches++;
			last_match = loop;
			}
		}

	// Calculate the middle of matches
	if(num_matches)
		{
		match = last_match - num_matches/2;
		//P2N("Match mean %d\r\n", match);
	
		// Copy forth
		for(int loop1 = 0; loop1 < rc.right; loop1++)
			{
			xarr[loop1].y = arr[loop1].y;
			}

		if(((CButton*)GetParent()->GetDlgItem(IDC_RADIO1))->GetCheck())
			{
			count = DISTCNT;
			}
		else if(((CButton*)GetParent()->GetDlgItem(IDC_RADIO2))->GetCheck())
			{
			count = 4 * DISTCNT;
			}
		else if(((CButton*)GetParent()->GetDlgItem(IDC_RADIO3))->GetCheck())
			{
			count = 8 * DISTCNT;
			}

		SetCursor(grab);
		curdrag = true;
		SetCapture();
		}

	CBaseWnd::OnLButtonDown(nFlags, point);

}

void CCurve::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	//P2N("CCurve::OnKeyDown %d %d\r\n", nChar, nChar);
	CBaseWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CCurve::PreTranslateMessage(MSG* pMsg) 

{
	//P2N("CCurve::PreTranslateMessage %p %s\r\n", this, mxpad.num2msg(pMsg->message));
	return CBaseWnd::PreTranslateMessage(pMsg);
}

//////////////////////////////////////////////////////////////////////////

void CCurve::OnLButtonUp(UINT nFlags, CPoint point) 

{
	if(!curdrag)
		{
		CBaseWnd::OnLButtonUp(nFlags, point);
		return;
		}

	curdrag = false;
	ReleaseCapture();

	RECT rc; GetClientRect(&rc);

	// Smooth just a little
	//SmoothCurve(xarr, false, 10);

	// Copy it back
	for(int loop2 = 1; loop2 < rc.right - 1; loop2++)
		{
		arr[loop2].y = min(max(xarr[loop2].y, 0), rc.bottom);
		}
	
	// Apply histogam change	
	SetHist();
	// Apply to image

	Invalidate();

	CBaseWnd::OnLButtonUp(nFlags, point);
}

void CCurve::ResetCurve()

{
	//P2N("CCurve::ResetCurve\r\n");	

	RECT rc; GetClientRect(&rc);

	int loop;
	double step = (double)rc.bottom / rc.right;

	for(loop = 0; loop < rc.right; loop++)
		{
		arr[loop].x = loop; arr[loop].y = (int)(step * loop);
		}
	Invalidate();
}

void CCurve::GetTipText(CString &str)

{
	//P2N("CBaseWnd::GetTipText\r\n");
	
	RECT mrc, rc; GetClientRect(&rc);

	CPoint npoint;

	// Copy and adjust newpoint
	npoint.y = rc.bottom - lastmousepoint.y;  		// Fake mirrored y coordinates
	npoint.x = lastmousepoint.x;

	int hit = 0;

	mrc.top		= npoint.y - mousedist;
	mrc.bottom	= npoint.y + mousedist;
	mrc.left	= npoint.x - mousedist;
	mrc.right	= npoint.x + mousedist;

	for(int loop = 0; loop < rc.right; loop++)	
		{
		if(::PtInRect(&mrc, arr[loop]))
			{
			//P2N("Match at %d\r\n", loop);
			hit = true;
			break;
			}
		}

	if(hit)
		{
		str.Format("x=%d y=%d", 255 * npoint.x / rc.right, 255 * npoint.y / rc.bottom);
		}
	else
		{
		str = "Curve";
		}
}

//////////////////////////////////////////////////////////////////////////
// Maintain a running avarage (sideways) from loop point
//
// -> arr to yarr -> backto -> arr
//

void CCurve::SmoothCurve(POINT *zarr, int inval, int span, int low, int high)

{
	RECT rc; GetClientRect(&rc);
	
	//P2N("CCurve::SmoothCurve()\r\n");
	
	// EnCount for default arguments
	if(zarr == NULL)
		zarr = arr;

	if(span == 0) 
		span = AVGSPAN;

	if(low == -1)
		low = 0;
	
	if(high == -1)
		high = rc.right - 1;
	
	// Boundary checks -- do not mess with the stating points
	low  = max(low, 1);
	high = min(high, rc.right - 1);

	int cnt = 0;  // for testing 

	// Copy forth
	for(int loop = 0; loop < rc.right; loop++)
		{
		yarr[loop].y = zarr[loop].y;
		}
	
	// Avarage sideways with AVGSPAN limit sideways scope
	for(int loop2 = low; loop2 < high; loop2++)
		{
		double avgval = 0;

		// ----------------------------------
		//		  ^loop3    ^loop2    ^loop4
		//        | AVGSPAN | AVGSPAN |
		// ----------------------------------
		//                     

		// Estabilish scan bounds
		int loop3, loop3a, loop4;

		if(loop2 < span)	
			{
			loop3a = loop3 = 0;
			loop4 = 2 * loop2;
			}
		else if(loop2 > (rc.right - span))	
			{
			loop3a = loop3 = loop2 -  (rc.right - loop2);
			loop4 = rc.right;
			}
		else
			{
			loop3a = loop3 = loop2 - span;
			loop4 = loop2 + span;
			}

		if(loop3 == loop4)
			{
			// Propagate avarage
			avgval = zarr[loop2].y;
			}
		else
			{
			// Calc avarage
			for(; loop3 < loop4; loop3++)
				{
				avgval += zarr[loop3].y;
				}
			avgval /= (loop4 - loop3a);
			}

		//if(cnt++ > rc.right - 20 && cnt <= rc.right)
		//	P2N("loop3a=%d loop4=%d loop=%d avg=%f currval=%d \r\n", 
		//			loop3a, loop4, loop2, avgval,  zarr[loop2].y);

		// Save calulated value
		yarr[loop2].y = (long)ceil( avgval);
		}

	// Copy back
	for(int loop5 = 0; loop5 < rc.right; loop5++)
		{
		zarr[loop5].y = yarr[loop5].y;
		}
	
	if(inval)
		{
		Invalidate();
		}
}

BOOL CCurve::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 

{
	//P2N("CCurve::OnSetCursor curdrag=%d\r\n", curdrag);

	if(curdrag)
		{
		//P2N("CCurve::OnSetCursor grab\r\n");
		SetCursor(grab);
		}
	else if(curbusy)
		{
		//P2N("CCurve::OnSetCursor busy\r\n");
		SetCursor(LoadCursor(NULL, IDC_WAIT));
		}
	else 
		{
		//P2N("CCurve::OnSetCursor idle\r\n");
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		}
	return true;

	//return CBaseWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CCurve::GetHist()

{
	memset(hg, 0, sizeof(hg));

	// If there is an image, get histogram
	CShadesDlg *shadesdlg = (CShadesDlg* )AfxGetApp()->m_pMainWnd;
	ASSERT(shadesdlg->magic == SHADE_MAGIC);

	if(shadesdlg->current >= 0)
		{	
		CXrayDoc *doc = ((CXrayDoc*)shadesdlg->docarr[shadesdlg->current]);

		// Already running stuff ....
		if(doc)
			{
			olddoc = doc;
			maxhist = doc->img->Histogram(NULL, NULL, NULL, hg, 0);
			P2N("Got histogram\r\n");

			//D2N(hg, 255);
			for(int loop = 0; loop < 256; loop++)
				{
				//P2N("loop=%d hg[loop]=%d\r\n", loop, 255 * hg[loop] / maxhist);
				// Normalize it
				hg[loop] = 255 * hg[loop] / maxhist;
				}
			}
		}
}

void CCurve::OnShowWindow(BOOL bShow, UINT nStatus) 

{
	CBaseWnd::OnShowWindow(bShow, nStatus);
	
	P2N("CCurve::OnShowWindow %d\r\n", bShow);		
}

void CCurve::OnTimer(UINT nIDEvent) 

{
	if(nIDEvent == 1)
		{
		// Initial load
		KillTimer(nIDEvent);

		GetHist();
		Invalidate();
		}

	if(nIDEvent == 2)
		{
		// If there is doc change
		CShadesDlg *shadesdlg = (CShadesDlg* )AfxGetApp()->m_pMainWnd;
		ASSERT(shadesdlg->magic == SHADE_MAGIC);

		if(shadesdlg->current >= 0)
			{	
			CXrayDoc *doc = ((CXrayDoc*)shadesdlg->docarr[shadesdlg->current]);

			// Already running stuff ....
			if(doc)
				{
				if(doc != olddoc)
					{
					GetHist();
					Invalidate();
					}
				}
			}
		}

	CBaseWnd::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////

void CCurve::SetHist()

{
	RECT rc; GetClientRect(&rc);

	SetCursor(LoadCursor(NULL, IDC_WAIT));

	// Stretch the histogram to create the normalized image mapping.
	for(int ii = 0; ii <= 255; ii++)
		{
		if ( ii < (int) low )
			{
			hg2[ii] = 0;
			} 
		else 
			{
			if(ii > (int) high)
				hg2[ii] = 255;
			else
				hg2[ii] = ( 255 - 1) * ( ii - low) / ( high - low );
			}
		}

	// Normalize to width/height  (255/255)
	for(int loop3 = low; loop3 < rc.right; loop3++)
		{
		int idx = low + (high - low) * loop3 / rc.right;
		hg2[idx] = 255 * arr[loop3].y / rc.bottom;
		}

	// Limit it to output parameters
	{
	for(ii = 0; ii <= 255; ii++)
		{
		if ( ii < (int) lowlim )
			{
			hg2[ii] = lowlim;
			} 
		else 
			{
			if(ii > (int) highlim)
				hg2[ii] = highlim;
			}
		}
	}
	
	// If there is an image, set histogram
	CShadesDlg *shadesdlg = (CShadesDlg* )AfxGetApp()->m_pMainWnd;
	ASSERT(shadesdlg->magic == SHADE_MAGIC);

	if(shadesdlg->current >= 0)
		{	
		CXrayDoc *doc = ((CXrayDoc*)shadesdlg->docarr[shadesdlg->current]);

		// Already running stuff ....
		if(doc)
			{
			olddoc = doc;

			shadesdlg->BusyCursor();
			curbusy = true;

			doc->render->Copy(*doc->org);

			maxhist = doc->render->HistogramRemap(hg2);
			P2N("Remapped histogram\r\n");
			
			shadesdlg->BusyCursor(false);
			curbusy = false;

			shadesdlg->Invalidate();
			}
		}
	SetCursor(LoadCursor(NULL, IDC_ARROW));
}

void CCurve::ResetImg()

{
	CShadesDlg *shadesdlg = (CShadesDlg* )AfxGetApp()->m_pMainWnd;
	ASSERT(shadesdlg->magic == SHADE_MAGIC);

	if(shadesdlg->current < 0)
		{
		P2N("No document in CProcDlg::OnButton3\r\n");
		return;
		}

	CXrayDoc *doc = (CXrayDoc*)(shadesdlg->docarr[shadesdlg->current]); 

	if(doc)
		{
		ASSERT(doc->magic == DOC_MAGIC);

		//ShowStatus("Restoring image to previous state ...");	

		doc->render->Copy(*doc->org);
		shadesdlg->Invalidate();
		}
}

void CCurve::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	ResetCurve();
	SetHist();
	CBaseWnd::OnLButtonDblClk(nFlags, point);
}
