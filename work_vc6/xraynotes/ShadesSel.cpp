// ShadesSel.cpp: implementation of the CShadesSel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "xraynotes.h"
#include "ShadesSel.h"
#include "helperdlgs.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShadesSel::CShadesSel(void *ptr)
{
	dlg = (CShadesDlg *)ptr;
}

CShadesSel::~CShadesSel()
{

}


//////////////////////////////////////////////////////////////////////////

void CShadesSel::SelLdown(CPoint point)

{
    // Execute hittest on rect selection
    CRect handul(dlg->selrect.left - 3, dlg->selrect.top - 3,
            dlg->selrect.left + 3, dlg->selrect.top + 3);

    if(handul.PtInRect(point))
        {
        P2N("Hittest on upper left\r\n");
        dlg->handlehit = UL;
        }

    CRect handur(dlg->selrect.right - 3, dlg->selrect.top - 3,
            dlg->selrect.right + 3, dlg->selrect.top + 3);

    if(handur.PtInRect(point))
        {
        //P2N("Hittest on upper right\r\n");
        dlg->handlehit = UR;
        }

    CRect handlr(dlg->selrect.right - 3, dlg->selrect.bottom - 3,
            dlg->selrect.right + 3, dlg->selrect.bottom + 3);

    if(handlr.PtInRect(point))
        {
        //P2N("Hittest on lower right\r\n");
        dlg->handlehit = LR;
        }

    CRect handll(dlg->selrect.left - 3, dlg->selrect.bottom - 3,
            dlg->selrect.left + 3, dlg->selrect.bottom + 3);

    if(handll.PtInRect(point))
        {
        //P2N("Hittest on lower left\r\n");
        dlg->handlehit = LL;
        }

    int hmid = dlg->selrect.left + (dlg->selrect.right - dlg->selrect.left) / 2 ;
    int vmid = dlg->selrect.top + (dlg->selrect.bottom - dlg->selrect.top) / 2 ;

    CRect handum(hmid - 3, dlg->selrect.top - 3,
            hmid + 3, dlg->selrect.top + 3);

    if(handum.PtInRect(point))
        {
        //P2N("Hittest on upper mid\r\n");
        dlg->handlehit = UM;
        }

    CRect handrm(dlg->selrect.right - 3, vmid - 3,
            dlg->selrect.right + 3, vmid + 3);

    if(handrm.PtInRect(point))
        {
        //P2N("Hittest on right mid\r\n");
        dlg->handlehit = RM;
        }

    CRect handbm(hmid - 3, dlg->selrect.bottom - 3,
    hmid + 3, dlg->selrect.bottom + 3);

    if(handbm.PtInRect(point))
        {
        //P2N("Hittest on bottom mid\r\n");
        dlg->handlehit = BM;
        }

    CRect handlm(dlg->selrect.left - 3, vmid - 3,
    dlg->selrect.left + 3, vmid + 3);

    if(handlm.PtInRect(point))
        {
        //P2N("Hittest on left mid\r\n");
        dlg->handlehit = LM;
        }

    // See if it is within the whole selection
    if(!dlg->handlehit)
        {
        CRect hand(dlg->selrect.left, dlg->selrect.top,
                        dlg->selrect.right, dlg->selrect.bottom);

        if(hand.PtInRect(point))
            {
            P2N("Hittest on whole\r\n");
            dlg->drag_x = point.x;
            dlg->drag_y = point.y;
            dlg->handlehit = WHOLE;
            }
        }

    // Start new selection
    if(!dlg->handlehit)
        {
        dlg->selrect.right = dlg->selrect.left = point.x;
        dlg->selrect.bottom = dlg->selrect.top  = point.y;
        dlg->Invalidate(false);
        }
}

//////////////////////////////////////////////////////////////////////////

void CShadesSel::MarkLdown(CPoint point)

{
    RECT    rc;    dlg->GetClientRect(&rc);

    // Hittest freehand selection

    int cnt = dlg->xmarkarr.GetSize() ;
    if(cnt > 0)
        {
        //dc.MoveTo(xmarkarr[0], ymarkarr[0]);

        int old_x = 0, old_y = 0, loop;
        for(loop = 0; loop < cnt; loop++)
            {
            // 12 away from prev drop?
            if(ABS(int(old_x - dlg->xmarkarr[loop]) ) > 12 || ABS(int(old_y - dlg->ymarkarr[loop]) ) > 12)
                {
                // Old hittest on handle
                //CRect htest2(xmarkarr[loop] - 4, ymarkarr[loop] - 4,
                //              xmarkarr[loop] + 4, ymarkarr[loop] + 4);
                //
                //htest2.NormalizeRect();

                // New hittest on bounding rect
                int loop_p = loop - 1, loop_n = loop + 1;
                if(loop == 0) {loop_p = cnt - 1;}
                if(loop >= cnt - 1) {loop_p = cnt-2; loop_n = 0;}

                CRect htest(dlg->xmarkarr[loop_p] , dlg->ymarkarr[loop_p],
                                dlg->xmarkarr[loop_n], dlg->ymarkarr[loop_n]);

                htest.NormalizeRect();
                htest.InflateRect(4,4);

                dlg->updaterect = htest;
                dlg->Invalidate(false);

                if(htest.PtInRect(point))
                    {
                    dlg->handlehit = HH;
                    dlg->handlesel = loop + 1;

                    if(dlg->handlesel == cnt-1)
                        dlg->handlesel = 0;

                    P2N("Point in freehand selection %d\r\n", loop);

                    if(GetKeyState(VK_SHIFT) & 0x80)
                        {
                        // Add new
                        if(dlg->handlesel < (dlg->xmarkarr.GetSize()) && dlg->handlesel >= 0)
                            {
                            int offset = 1;

                            // Wrap around
                            if(dlg->handlesel == dlg->xmarkarr.GetUpperBound())
                                offset = -dlg->handlesel;

                            DWORD xxx = dlg->xmarkarr[dlg->handlesel] + 
								((int)dlg->xmarkarr[dlg->handlesel + offset] - (int)dlg->xmarkarr[dlg->handlesel]) / 2;
                            DWORD yyy = dlg->ymarkarr[dlg->handlesel] +
								((int)dlg->ymarkarr[dlg->handlesel + offset] - (int)dlg->ymarkarr[dlg->handlesel]) / 2;

                            dlg->xmarkarr.InsertAt(dlg->handlesel + offset, xxx);
                            dlg->ymarkarr.InsertAt(dlg->handlesel + offset, yyy);
                            dlg->Invalidate(false);
                            }
                        }
                    else if(GetKeyState(VK_CONTROL) & 0x80)
                        {
                        // Delete
                        if(dlg->handlesel < dlg->xmarkarr.GetSize() && dlg->handlesel >= 0)
                            {
                            dlg->xmarkarr.RemoveAt(dlg->handlesel);
                            dlg->ymarkarr.RemoveAt(dlg->handlesel);
                            dlg->Invalidate(false);
                            }
                        }
                    else
                        {
                        // Just mark it
                        }
                    break;
                    }
                }
            }

        if(!dlg->handlehit)
            {
            // Backup for undo

            dlg->xmarkarr.RemoveAll();
            dlg->ymarkarr.RemoveAll();

            dlg->xmarkarr.Add(point.x);
            dlg->ymarkarr.Add(point.y);

            dlg->InvalidateRect(&rc, false);
            support.YieldToWinEx();
            }
        }
    dlg->helper->TmpClosePanels();
}

//////////////////////////////////////////////////////////////////////////

#define MARKSIZE    6   // Must be divisible by two

void CShadesSel::MarkMove(CPoint point)

{
    RECT rc;
    RECT rc2; dlg->GetClientRect(&rc2);

    if(dlg->handlehit == HH)
        {
        int cnt = dlg->xmarkarr.GetSize();
        if(cnt)
            {
            int xdiff = (point.x - dlg->drag_x);
            int ydiff = (point.y - dlg->drag_y);

            // Wrap loop
            // -------------1-2-3------------------7-8-9-----------------
            // ----1-2-3-4-5-6-----
            // -----------------------------------1-2-3-4-5-6-----

            int loop = dlg->handlesel;
            int larr[MARKSIZE + 1]; memset(larr, 0, sizeof(larr));

            for(int ii = 0 ; ii < MARKSIZE + 1 ; ii++)
                {
                if(dlg->handlesel + (ii - MARKSIZE/2) < 0 )
                    {
                    // Wrap right
                    //rr[ii] =  (cnt-1) - (ii-MARKSIZE/2) + handlesel;
                    }
                else if(dlg->handlesel + ( ii - MARKSIZE/2) > cnt - 1)
                    {
                    // Wrap left
                    //larr[ii] =  (ii-MARKSIZE/2) + (handlesel - (cnt-1));
                    }
                else
                    {
                    // No wrap
                    larr[ii] =  (ii - MARKSIZE/2) + dlg->handlesel;
                    }
                }


            //////////////////////////////////////////////////////////////////
            // Move neighbours also -> 1/1  3/4  1/2  1/4
#if 1
            dlg->xmarkarr[larr[0]] += xdiff/4;
            dlg->ymarkarr[larr[0]] += ydiff/4;

            dlg->xmarkarr[larr[1]] += xdiff/3;
            dlg->ymarkarr[larr[1]] += ydiff/3;

            dlg->xmarkarr[larr[2]] += xdiff/2;
            dlg->ymarkarr[larr[2]] += ydiff/2;

            dlg->xmarkarr[larr[3]] += xdiff;
            dlg->ymarkarr[larr[3]] += ydiff;

            dlg->xmarkarr[larr[4]] += xdiff/2;
            dlg->ymarkarr[larr[4]] += ydiff/2;

            dlg->xmarkarr[larr[5]] += xdiff/3;
            dlg->ymarkarr[larr[5]] += ydiff/3;

            dlg->xmarkarr[larr[6]] += xdiff/4;
            dlg->ymarkarr[larr[6]] += ydiff/4;
#endif

            dlg->Invalidate(false);

            #if 0
            // Calc update rect  -- first -- middle -- last
            if(dlg->handlesel == 0)
                {
                rc.left   =  dlg->xmarkarr[dlg->xmarkarr.GetSize() - 1];
                rc.top    =  dlg->ymarkarr[dlg->xmarkarr.GetSize() - 1];
                rc.right  =  dlg->xmarkarr[dlg->handlesel+1];
                rc.bottom =  dlg->ymarkarr[dlg->handlesel+1];

                norm_rect(&rc);

                rc.left = MIN(rc.left, point.x);
                rc.top = MIN(rc.top, point.y);
                rc.right = MAX(rc.right, point.x);
                rc.bottom = MAX(rc.bottom, point.y);
                }
            else if (handlesel < xmarkarr.GetSize() - 1)
                {
                rc.left = dlg->xmarkarr[dlg->handlesel - 3];
                rc.top = dlg->ymarkarr[dlg->handlesel - 3];
                rc.right = dlg->xmarkarr[dlg->handlesel + 3];
                rc.bottom = dlg->ymarkarr[dlg->handlesel + 3];

                norm_rect(&rc);

                rc.left = MIN(rc.left, point.x);
                rc.top = MIN(rc.top, point.y);
                rc.right = MAX(rc.right, point.x);
                rc.bottom = MAX(rc.bottom, point.y);

                //updaterect = rc;
                }
            else
                {
                rc.left   =  dlg->xmarkarr[dlg->handlesel - 1];
                rc.top    =  dlg->ymarkarr[dlg->handlesel - 1];
                rc.right  =  dlg->xmarkarr[0];
                rc.bottom =  dlg->ymarkarr[0];

                norm_rect(&rc);

                rc.left = MIN(rc.left, point.x);
                rc.top = MIN(rc.top, point.y);
                rc.right = MAX(rc.right, point.x);
                rc.bottom = MAX(rc.bottom, point.y);
                }

            // New operation relative to here


            pad_rect(&rc, 16);
            dlg->InvalidateRect(&rc, false);
            }
        #endif

        dlg->drag_x = point.x;
        dlg->drag_y = point.y;
        }
    }
else
    {
    if(ABS(int(dlg->old_x - point.x) ) > dlg->movetresh || ABS(int(dlg->old_y - point.y) ) > dlg->movetresh)
        {
        if(point.y < 20)
            {
            P2N("Potential scroll down\r\n");
            yscroll.m_scroll.SetScrollPos(yscroll.m_scroll.GetScrollPos() - 5);
            dlg->Invalidate(false);
            }

        if(point.y > rc2.bottom - 20)
            {
            P2N("Potential scroll up\r\n");
            yscroll.m_scroll.SetScrollPos(yscroll.m_scroll.GetScrollPos() + 5);
            dlg->Invalidate(false);
            }

        dlg->xmarkarr.Add(point.x);
        dlg->ymarkarr.Add(point.y);

        rc.left = MIN(dlg->old_x, point.x);
        rc.top = MIN(dlg->old_y, point.y);
        rc.right = MAX(dlg->old_x, point.x);
        rc.bottom = MAX(dlg->old_y, point.y);

        InflateRect(&rc, 16, 16);

        dlg->old_x = point.x;
        dlg->old_y = point.y;
        }
    dlg->InvalidateRect(&rc, false);
    }
}

void CShadesSel::RectMove(CPoint point)

{
	P2N("RectMove %d %d\r\n", point.x, point.y);
}

//////////////////////////////////////////////////////////////////////////

void CShadesSel::SelMove(CPoint point)

{
    //P2N("Selmove %d %d\r\n", point.x, point.y);

    RECT rc;

    // Check Handle hit
    if(dlg->handlehit)
        {
        rc.left     = dlg->selrect.left;
        rc.top      = dlg->selrect.top;
        rc.bottom   = dlg->selrect.bottom;
        rc.right    = dlg->selrect.right;

        switch(dlg->handlehit)
            {
            case UL:
                dlg->selrect.left = point.x;
                dlg->selrect.top = point.y;
                break;

            case UR:
                dlg->selrect.right = point.x;
                dlg->selrect.top = point.y;
                break;

            case LR:
                dlg->selrect.right = point.x;
                dlg->selrect.bottom = point.y;
                break;

            case LL:
                dlg->selrect.left = point.x;
                dlg->selrect.bottom = point.y;
                break;

            case UM:
                dlg->selrect.top = point.y;
                break;

            case RM:
                dlg->selrect.right = point.x;
                break;

            case BM:
                dlg->selrect.bottom = point.y;
                break;

            case LM:
                dlg->selrect.left = point.x;
                break;

            case WHOLE:
                dlg->selrect.left += point.x - dlg->drag_x;
                dlg->selrect.top += point.y - dlg->drag_y;
                dlg->selrect.right += point.x - dlg->drag_x;
                dlg->selrect.bottom += point.y - dlg->drag_y;

                dlg->drag_x = point.x;
                dlg->drag_y = point.y;
                break;
        }

        // Expand rect
        InflateRect(&rc, 8, 8);
        rc.left -= 4; rc.top -= 4; rc.right += 4; rc.bottom += 4;

        dlg->updaterect = rc;
        }
    else
        {
        rc.left     = MIN(dlg->selrect.left, point.x);
        rc.top      = MIN(dlg->selrect.top, point.y);
        rc.right    = MAX(dlg->selrect.right, point.x);
        rc.bottom   = MAX(dlg->selrect.bottom, point.y);

        // Expand rect to cover handles
        rc.left -= 8; rc.top -= 8; rc.right += 8; rc.bottom += 8;

        dlg->selrect.right = point.x;
        dlg->selrect.bottom = point.y;
        }

#if 0
    // Normalize rect
    if(dlg->selrect.right < dlg->selrect.left)
        {
        int tmp = dlg->selrect.left;
        dlg->selrect.left = dlg->selrect.right;
        dlg->selrect.right = tmp;

        rc.left     = MIN(dlg->selrect.left, point.x);
        rc.top      = MIN(dlg->selrect.top, point.y);
        rc.right    = MAX(dlg->selrect.right, point.x);
        rc.bottom   = MAX(dlg->selrect.bottom, point.y);
        }
    if(dlg->selrect.bottom < dlg->selrect.top)
        {
        int tmp = sdlg->elrect.top;
        dlg->selrect.top = dlg->selrect.bottom;
        dlg->selrect.bottom = tmp;

        rc.left     = MIN(dlg->selrect.left, point.x);
        rc.top      = MIN(dlg->selrect.top, point.y);
        rc.right    = MAX(dlg->selrect.right, point.x);
        rc.bottom   = MAX(dlg->selrect.bottom, point.y);
        }
#endif

    dlg->old_point = point;
    dlg->InvalidateRect(&rc, false);
}


void CShadesSel::SelToRender(int toimg)

{
    CXrayDoc *doc = dlg->GetCurrDoc();

	if(!doc)
		return;
	
    // Get update from scroll bars
    dlg->hscroll = xscroll.m_scroll.GetScrollPos();
    dlg->vscroll = yscroll.m_scroll.GetScrollPos();

    P2N("Marking rect scroll %d %d\r\n", dlg->hscroll, dlg->vscroll);

    // Needs to scroll this much:
    CxImage *img;

    if(toimg)
        img = doc->img;
    else
        img = doc->render;

    int reny = (int)img->GetHeight();
    int renx = (int)img->GetWidth();

    CRect rc(dlg->selrect);

//////////////////////////////////////////////////////////////////////////
//  -------------
//  |           |
//  -------------
//

    rc.top = reny - rc.top;
    rc.bottom = reny - rc.bottom;

    //rc.left =+  (hscroll * renx) / 100;
    //rc.right =+ (hscroll * renx) / 100;

    //CPoint point((hscroll * renx) / 100, (vscroll * reny) / 100);
    //rc += point;

    doc->render->SelectionClear();
    doc->render->SelectionAddRect(rc);

    doc->img->SelectionClear();
    doc->img->SelectionAddRect(rc);
}

//////////////////////////////////////////////////////////////////////////
// Calculate selection rect

void CShadesSel::CalcSelRect(RECT *rc)

{
    rc->top = 0xffff;
    rc->bottom = 0;
    rc->left = 0xffff;
    rc->right = 0;

    // Assemble polygon array for selection
    int size =  dlg->xmarkarr.GetSize();

    for(int loop = 0; loop < size; loop++)
        {
        //dlg->markers[loop].x
        //dlg->markers[loop].y

        rc->top = MIN((long)dlg->ymarkarr[loop], rc->top);
        rc->bottom = MAX((long)dlg->ymarkarr[loop], rc->bottom);

        rc->left = MIN((long)dlg->xmarkarr[loop], rc->left);
        rc->right = MAX((long)dlg->xmarkarr[loop], rc->right);
        }

    InflateRect(rc, 8, 8);

    //P2N("Calc max rect t=%d b=%d l=%d r=%d\r\n", rc->top, rc->bottom, rc->left, rc->right);
}


void CShadesSel::MarkToRender(int toimg)

{
    CXrayDoc *doc = dlg->GetCurrDoc();

	if(!doc)
		return;

    // Assemble polygon array for selection
    int size =  dlg->xmarkarr.GetSize();

    if(size == 0)
        return;
    
    POINT *markers = (POINT *)malloc(size * sizeof(POINT));
    ASSERT(markers);

    RECT rc; dlg->GetClientRect(&rc);

    // Get update from scroll bars
    dlg->hscroll = xscroll.m_scroll.GetScrollPos();
    dlg->vscroll = yscroll.m_scroll.GetScrollPos();

    P2N("Marking %d points\r\n", size);

    // Needs to scroll this much:
    CxImage *img;

    if(toimg)
        img = doc->img;
    else
        img = doc->render;

    img->SelectionClear();

    int reny = (int)( img->GetHeight() / doc->mag );
    int renx = (int)( img->GetWidth() / doc->mag );

    // Needs to scroll this much:
    int sdiff = renx - (rc.right - rc.left);
    int hdiff = reny - (rc.bottom - rc.top);

    sdiff = MAX(sdiff, 0);
    hdiff = MAX(hdiff, 0);

    //sdiff /= doc->mag;
    //hdiff /= doc->mag;

    int xpos = 0, ypos = 0;

    // Align to the middle if smaller
    if(renx < (rc.right - rc.left))
        {
        xpos = (rc.right - rc.left) / 2 - renx / 2;
        }
    if(reny < (rc.bottom - rc.top))
        {
        ypos = (rc.bottom - rc.top) / 2 - reny / 2;
        }

    for(int loop = 0; loop < size; loop++)
        {
        markers[loop].x = (int)dlg->xmarkarr[loop] + ((sdiff * dlg->hscroll) / 100);
        markers[loop].y = (int)(doc->render->GetHeight() - dlg->ymarkarr[loop]) - ((hdiff * dlg->vscroll) / 100);

        /// doc->mag /* + hdiff * vscroll / 100 */ ) );
        //markers[loop].x = (int)xmarkarr[loop] + ((hscroll * img->GetWidth()) / 100) ;
        //markers[loop].y = img->GetHeight() - (int)ymarkarr[loop];
        }

    img->SelectionAddPolygon(markers, size);

    free(markers);
}
