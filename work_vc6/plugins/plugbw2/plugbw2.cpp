
/* =====[ plugbw.cpp ]========================================== 
                                                                             
   Description:     The XrayNotes project, implementation of the plugbw.cpp  
   
					The DLL, when placed in the XrayNotes "plugin" directory
					will appear in the XrayNotes 'plugin' menu.

					The user can call this by selecting the plugin menu 
					and clicking on the plugin item.
                                                                             
                    Defines the behavior for the DLL
                                                                             
   Compiled:        MS-VC 6.00                                               
					Any tool can be used that generates a Windows DLL

                                                                             
   Notes:           The user can call this with selecting the plugin menu 
					and clicking on the plugin item.                                            

					
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------   -----------  ----------------------------   
      0.00  10/6/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#include "stdafx.h"

#define PLUGDLL_EXPORTS

#include "plugbw2.h"

// This is used to output debug strings to the 'MX Debug Pad'.
#include "../common/mxpad.h"

//////////////////////////////////////////////////////////////////////////
// For your convenience (synonym for major.minor -> 1.0)

#define DLLVER	10	

//////////////////////////////////////////////////////////////////////////
// XrayNotes will not check the DLLs that are less than the API version it 
// needs.
// Current version is 10  (synonym for major.minor -> 1.0)

#define APIVER	10

//////////////////////////////////////////////////////////////////////////
// Globals for this DLL

static int		gl_escape = 0;
static char		*gl_message = "Empty Message";

static void YieldToXrayNotes();

//////////////////////////////////////////////////////////////////////////
// Short description of the DLL (will show in menu)

	char * descr = 
"Convert To GrayScale AVG version";

//////////////////////////////////////////////////////////////////////////
// Long description of the DLL (the DOC request)

	char * ldescr = 

"Sample DLL: Convert To GrayScale\r\n"
"This particular example converts the image to GrayScale";

/////////////////////////////////////////////////////////////////////////
// Function entry points to the DLL 

	char *allfunc =  
 		"DllMain, Process, ";
 
/////////////////////////////////////////////////////////////////////////
// Windows needs this: 

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
		{
		case DLL_PROCESS_ATTACH:
			//P2N("DLL_PROCESS_ATTACH ....\r\n");
			// Call your init func
			break;

		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
			break;

		case DLL_PROCESS_DETACH:
			//P2N("DLL_PROCESS_DETACH ....\r\n");
			// Call your de-init func
			break;
		}
    return TRUE;
}

extern "C" PLUGDLL_API char *Message()

{
	return gl_message;
}


extern "C" PLUGDLL_API int DLLVersion(void)

{
	return DLLVER;
}

PLUGDLL_API int APIVersion(void)

{
	return APIVER;
}

extern "C" PLUGDLL_API const char * Description(void)

{
	return descr;
}


extern "C" PLUGDLL_API const char * LongDescr(void)

{
	return ldescr;
}

extern "C" PLUGDLL_API void Stop(void)

{
	gl_escape = true;
	return;
}

// This is avaraging the 3 colors
#define xRGB2GRAY(r,g,b) ( (((int)(r)) + ((int)(g)) + ((int)(b)) ) / 3   )

//////////////////////////////////////////////////////////////////////////
// Process the image. Main entry point
//
// Input parameters:
// 
//		void *ptr,  pointer to image
//		int effw,	effective width
//		int height,	length of total image memory 
//		int bpp		bytes per pixel (typically 3)
//		int			color depth		(265 pow 3) or ...
// 
// Return values:
//		
//		0 (Zero) as a noop
//		BIT_0 set if you want to signify a silent error.
//		BIT_1 set if you want the image to refresh.
//		BIT_2 set if you want a 'Failed DLL' error message to appear
//		BIT_3 set if the processing was stopped, message is to appear
//	
// If the client DLL can not process an image, return BIT_2 set, and
// fill in popup message into gl_message.
//

extern "C" PLUGDLL_API int Process(void *ptr, int effw, int height, int bpp)

{
	//P2N("Processing .... %p %d %d %d \r\n", ptr, effw, height, bpp);

	int ret = 0, xx, yy;

	if(!ptr)
		{	
		gl_message = "NULL pointer passed";
		return BIT_2;
		}

	BYTE *bptr = (BYTE*)ptr;
	
	gl_escape = 0;

	// Only process true color messages
	if(bpp != 3)
		{
		gl_message = "This plugin only processes RGB (3bpp) images.";
		ret |= BIT_2;
		return ret;
		}
	
	int yyy = 0;

	// The processing of the image goes here:
	for(yy = 0; yy < height; yy++ )
		{
		YieldToXrayNotes();
	
		//Sleep(1);

		// See if commanded to stop from the outside  
		if (gl_escape) 
			{
			gl_message = "Plugin stopped by the user, "
							"image may be partially processed.";
			ret |= BIT_3 | BIT_1;
			return ret;
			}

		for( xx = 0; xx < effw; xx += bpp)
			{
			// Pixel is at yy * effwidth + xx * bpp;
			
			BYTE blue 	= bptr[yyy + xx];
			BYTE green	= bptr[yyy + xx + 1];  
			BYTE red	= bptr[yyy + xx + 2];
			
			BYTE gg = xRGB2GRAY(red, green, blue);

			//////////////////////////////////////////////////////////
			// For this example, we convert to grayscale 
			// and write it back
			
			bptr[yyy + xx] = gg; 
			bptr[yyy + xx + 1] = gg; 
			bptr[yyy + xx + 2] = gg; 
			}

		yyy += effw;		// this way we have incremental buffer access,

		}  // end for

	ret |= BIT_1;
	 
	return ret;
}

//////////////////////////////////////////////////////////////////////////
// This allows the long process to yield control to the application
// You may want to do this to a.) allow your app to respond while 
// in the plugin and/or 2.) allow your app to process messages 
// to cancel (stop) the plugin that appears hung.

static void YieldToXrayNotes()

{
	int ret_val = 0;
	MSG msg;
    
	while(TRUE)
        {
        if(!PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
           break;
        TranslateMessage(&msg); DispatchMessage(&msg);
		Sleep(0);
        }
}