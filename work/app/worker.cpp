
/* =====[ worker.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the worker.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  5/14/2007  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// ProcDlg.cpp : implementation file
//

#include "stdafx.h"
#include "xraynotes.h"
#include "ProcDlg.h"

#include "worker.h"

#include <process.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


void	WorkerThread(void *lpParam)

{
    P2N("Started worker thread thread\r\n");

	return;

    CDemoDoc *pDoc = (CDemoDoc *)lpParam;

    if (pDoc==NULL) return;
    if (pDoc->image==NULL) return;

    POSITION pos;
    CView *pView;

    //CWaitCursor cursor;
    //Sleep(500);

    // Prepare for elaboration
    pDoc->image->SetProgress(0);
    pDoc->image->SetEscape(0);

    pDoc->SubmitUndo();

	// Auxilary thread for progress bar
	 if(statusdlg->showprog )
    	pDoc->hProgress = (HANDLE)_beginthread(RunProgressThread,0,pDoc);

    pDoc->Stopwatch(0);

    switch (pDoc->m_MenuCommand)
    {
    case PROC_FLIP:
        pDoc->image->Flip();
        break;
    case PROC_MIRROR:
        pDoc->image->Mirror();
        break;
    case PROC_NEGATIVE:
        pDoc->image->Negative();
        break;
    case PROC_GRAYSCALE:
        pDoc->image->GrayScale();
        break;
    case PROC_DITHER:
        pDoc->image->Dither((long)pDoc->m_fp[0]);
        break;
    case PROC_THRESHOLD:
        pDoc->image->Threshold((BYTE)pDoc->m_fp[0]);
        break;
    case PROC_COLORIZE:
        if ((BYTE)pDoc->m_fp[0]){
            pDoc->image->Colorize((BYTE)pDoc->m_fp[1],(BYTE)pDoc->m_fp[2],(BYTE)pDoc->m_fp[3]/100.0f);
        } else {
            pDoc->image->ShiftRGB((long)pDoc->m_fp[1],(long)pDoc->m_fp[2],(long)pDoc->m_fp[3]);
        }
        break;
    case PROC_LIGHTEN:
        pDoc->image->Light(20);
        break;
    case PROC_DARKEN:
        pDoc->image->Light(-20);
        break;

    case PROC_BRIGHTNESSCONTRASTCOLOR:
        switch((long)pDoc->m_fp[0])
        {
        case 1:
            pDoc->image->Light((long)pDoc->m_fp[1]);
            break;
        case 2:
            pDoc->image->Light((long)pDoc->m_fp[1], (long)pDoc->m_fp[2]);
            break;
        }
        break;

    case PROC_CONTRAST:
        pDoc->image->Light((long)pDoc->m_fp[0],25);
        break;
    case PROC_LESSCONTRAST:
        pDoc->image->Light((long)pDoc->m_fp[0],-25);
        break;
    case PROC_DILATE:
        pDoc->image->Dilate();
        break;
    case PROC_ERODE:
        pDoc->image->Erode();
        break;
    case PROC_CONTOUR:
        pDoc->image->Contour();
        break;
    case PROC_ADDNOISE:
        pDoc->image->Noise(50);
        break;
    case PROC_JITTER:
        pDoc->image->Jitter();
        break;
    case ID_FILTERS_NONLINEAR_EDGE:
        pDoc->image->Edge();
        break;
    case PROC_CIRCLETRANSFORM_CYLINDER:
        pDoc->image->CircleTransform(3,0,100);
        break;
    case PROC_CIRCLETRANSFORM_PINCH:
        pDoc->image->CircleTransform(1,0,100);
        break;
    case PROC_CIRCLETRANSFORM_PUNCH:
        pDoc->image->CircleTransform(0,0,100);
        break;
    case PROC_CIRCLETRANSFORM_SWIRL:
        pDoc->image->CircleTransform(2,0,100);
        break;
    case PROC_CIRCLETRANSFORM_BATHROOM:
        pDoc->image->CircleTransform(4);
        break;
    case PROC_EMBOSS:
        {
        long kernel[]={0,0,-1,0,0,0,1,0,0};
        pDoc->image->Filter(kernel,3,0,127);
        break;
        }
    case PROC_BLUR:
        {
        long kernel[]={1,1,1,1,1,1,1,1,1};
        pDoc->image->Filter(kernel,3,9,0);
        break;
        }
    case PROC_GAUSSIAN3X3:
        {
        long kernel[]={1,2,1,2,4,2,1,2,1};
        pDoc->image->Filter(kernel,3,16,0);
        break;
        }
    case PROC_GAUSSIAN5X5:
        {
        long kernel[]={0,1,2,1,0,1,3,4,3,1,2,4,8,4,2,1,3,4,3,1,0,1,2,1,0};
        pDoc->image->Filter(kernel,5,52,0);
        break;
        }
    case PROC_SOFTEN:
        {
        long kernel[]={1,1,1,1,8,1,1,1,1};
        pDoc->image->Filter(kernel,3,16,0);
        break;
        }
    case PROC_SHARPEN:
        {
        long kernel[]={-1,-1,-1,-1,15,-1,-1,-1,-1};
        pDoc->image->Filter(kernel,3,7,0);
        break;
        }
    case PROC_EDGE:
        {
        long kernel[]={-1,-1,-1,-1,8,-1,-1,-1,-1};
        pDoc->image->Filter(kernel,3,-1,255);
        break;
        }
    case PROC_MEDIAN:
        pDoc->image->Median(3);
        break;
    case PROC_GAMMA:
        pDoc->image->Gamma((long)pDoc->m_fp[0]/1000.0f);
        break;
    case PROC_HISTOGRAM_LOG:
        pDoc->image->HistogramLog();
        break;
    case PROC_HISTOGRAM_ROOT:
        pDoc->image->HistogramRoot();
        break;
    case PROC_HISTOGRAM_EQUALIZE:
        pDoc->image->HistogramEqualize();
        break;
    case PROC_HISTOGRAM_NORMALIZE:
        pDoc->image->HistogramNormalize();
        break;
    case PROC_HISTOGRAM_STRETCH:
        pDoc->image->HistogramStretch();
        break;
    case PROC_HISTOGRAM_STRETCH1:
        pDoc->image->HistogramStretch(1);
        break;
    case PROC_HISTOGRAM_STRETCH2:
        pDoc->image->HistogramStretch(2);
        break;
    case PROC_SKEW:
        pDoc->image->Skew((long)pDoc->m_fp[0]/1000.0f,(long)pDoc->m_fp[1]/1000.0f,
                            (long)pDoc->m_fp[2],(long)pDoc->m_fp[3],(long)pDoc->m_fp[4]!=0);
        break;
    case PROC_ROTATE:
        //pDoc->image->Rotate((float)(long)pDoc->m_fp[0]/1000);
        //***bd*** more rotation options
        CxImage::InterpolationMethod intm;
        CxImage::OverflowMethod overm;
        switch ((int)(pDoc->m_fp[1])) {
        case 0: intm=CxImage::IM_NEAREST_NEIGHBOUR; break;
        case 1: intm=CxImage::IM_BILINEAR; break;
        case 2: intm=CxImage::IM_BICUBIC; break;
        case 3: intm=CxImage::IM_BICUBIC2; break;
        case 4: intm=CxImage::IM_BSPLINE; break;
        case 5: intm=CxImage::IM_LANCZOS; break;
        case 6: intm=CxImage::IM_HERMITE; break;
        default: throw(0);
        }//switch
        switch ((int)(pDoc->m_fp[2])) {
        case 0: overm=CxImage::OM_BACKGROUND; break;
        case 1: overm=CxImage::OM_BACKGROUND; break;
        case 2: overm=CxImage::OM_BACKGROUND; break;
        case 3: overm=CxImage::OM_WRAP; break;
        case 4: overm=CxImage::OM_REPEAT; break;
        case 5: overm=CxImage::OM_MIRROR; break;
        case 6: overm=CxImage::OM_TRANSPARENT; break;
        }//switch
        switch ((int)(pDoc->m_fp[2])) {
        case 0: {
            RGBQUAD bkg = pDoc->image->GetPixelColor(0,0);
            pDoc->image->Rotate2((long)pDoc->m_fp[0]/1000.0f, 0, intm, overm, &bkg,true,pDoc->m_fp[3]!=0);
            break; }
        case 1: {
            RGBQUAD bkg = {0,0,0,0};
            pDoc->image->Rotate2((long)pDoc->m_fp[0]/1000.0f, 0, intm, overm, &bkg,true,pDoc->m_fp[3]!=0);
            break; }
        default:
            pDoc->image->Rotate2((long)pDoc->m_fp[0]/1000.0f, 0, intm, overm, 0,true,pDoc->m_fp[3]!=0);
        }
        break;
    case PROC_ROTATEL:
        pDoc->image->RotateLeft();
        break;
    case PROC_ROTATER:
        pDoc->image->RotateRight();
        break;
    case PROC_RESAMPLE:
        //***bd*** more resample options
        CxImage::InterpolationMethod rintm;
        switch ((long)(pDoc->m_fp[2]))
        {
        case 0: rintm=CxImage::IM_NEAREST_NEIGHBOUR; break;
        case 1: rintm=CxImage::IM_BILINEAR; break;
        case 2: rintm=CxImage::IM_BILINEAR; break;
        case 3: rintm=CxImage::IM_BICUBIC;  break;
        case 4: rintm=CxImage::IM_BICUBIC2; break;
        case 5: rintm=CxImage::IM_BSPLINE;  break;
        case 6: rintm=CxImage::IM_LANCZOS;  break;
        case 7: rintm=CxImage::IM_HERMITE;  break;
        default: throw(0);
        }//switch

        P2N("Interpoaltion method %d\r\n", pDoc->m_fp[2]);

        switch ((long)(pDoc->m_fp[2])) {
        case 0:
            pDoc->image->Resample((long)pDoc->m_fp[0],(long)pDoc->m_fp[1],1);
            break;
        case 1:
            pDoc->image->Resample((long)pDoc->m_fp[0],(long)pDoc->m_fp[1],0);
            break;

        default:
            pDoc->image->Resample2((long)pDoc->m_fp[0],(long)pDoc->m_fp[1],
                                    rintm, CxImage::OM_REPEAT);
        }
        pDoc->m_ZoomFactor = 1;
        break;

    case PROC_INCREASEBPP:
        pDoc->image->IncreaseBpp((long)pDoc->m_fp[0]);
        break;
    case PROC_DECREASEBPP:
        {
            long bit=(long)pDoc->m_fp[0];
            long method=(long)pDoc->m_fp[1];
            bool errordiffusion=((long)pDoc->m_fp[2])!=0;
            long colors=(long)pDoc->m_fp[3];

            //pDoc->image->IncreaseBpp(24);

            RGBQUAD* ppal = NULL;
            if (method==1){
                switch (bit){
                /*case 1:
                    {
                        CQuantizer q(2,8);
                        q.ProcessImage(pDoc->image->GetDIB());
                        ppal=(RGBQUAD*)calloc(2*sizeof(RGBQUAD),1);
                        q.SetColorTable(ppal);
                        break;
                    }*/
                case 4:
                    {
                        CQuantizer q(colors,8);
                        q.ProcessImage(pDoc->image->GetDIB());
                        ppal=(RGBQUAD*)calloc(16*sizeof(RGBQUAD),1);
                        q.SetColorTable(ppal);
                        break;
                    }
                case 8:
                    {
                        CQuantizer q(colors,(colors>16?7:8));
                        q.ProcessImage(pDoc->image->GetDIB());
                        ppal=(RGBQUAD*)calloc(256*sizeof(RGBQUAD),1);
                        q.SetColorTable(ppal);
                    }
                }
                pDoc->image->DecreaseBpp(bit,errordiffusion,ppal,colors);
            } else pDoc->image->DecreaseBpp(bit,errordiffusion,0);

            if (!pDoc->image->AlphaPaletteIsValid()) pDoc->image->AlphaPaletteEnable(0);

            if (ppal) free(ppal);
            break;
        }
    }

    pDoc->Stopwatch(1);
    pDoc->image->SetProgress(100);
    pDoc->hThread = 0;

    if(!pDoc->norefresh)
        {
        pos = pDoc->GetFirstViewPosition();
        pView = pDoc->GetNextView(pos);
        if (pView)
            SendMessage(pView->m_hWnd, WM_USER_NEWIMAGE,0,0);
        }
    pDoc->norefresh = 0;

    pDoc->SetModifiedFlag(true);
	pDoc->noundo = false;

    P2N("Done thread\r\n");
	
    _endthread();

    return ;


