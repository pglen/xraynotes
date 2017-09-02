# Microsoft Developer Studio Generated NMAKE File, Based on cximagemfcdll.dsp
!IF "$(CFG)" == ""
CFG=CxImageMfcDll - Win32 Debug
!MESSAGE No configuration specified. Defaulting to CxImageMfcDll - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "CxImageMfcDll - Win32 Release" && "$(CFG)" != "CxImageMfcDll - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "cximagemfcdll.mak" CFG="CxImageMfcDll - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CxImageMfcDll - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CxImageMfcDll - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "CxImageMfcDll - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "..\..\bin\cximage.dll" "$(OUTDIR)\cximagemfcdll.pch"

!ELSE 

ALL : "jasper - Win32 Release" "zlib - Win32 Release" "tiff - Win32 Release" "png - Win32 Release" "jpeg - Win32 Release" "jbig - Win32 Release" "j2k - Win32 Release" "..\..\bin\cximage.dll" "$(OUTDIR)\cximagemfcdll.pch"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"j2k - Win32 ReleaseCLEAN" "jbig - Win32 ReleaseCLEAN" "jpeg - Win32 ReleaseCLEAN" "png - Win32 ReleaseCLEAN" "tiff - Win32 ReleaseCLEAN" "zlib - Win32 ReleaseCLEAN" "jasper - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\cximagemfcdll.obj"
	-@erase "$(INTDIR)\cximagemfcdll.pch"
	-@erase "$(INTDIR)\cximagemfcdll.res"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\tif_xfile.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\ximabmp.obj"
	-@erase "$(INTDIR)\ximadsp.obj"
	-@erase "$(INTDIR)\ximaenc.obj"
	-@erase "$(INTDIR)\ximaexif.obj"
	-@erase "$(INTDIR)\ximage.obj"
	-@erase "$(INTDIR)\ximagif.obj"
	-@erase "$(INTDIR)\ximahist.obj"
	-@erase "$(INTDIR)\ximaico.obj"
	-@erase "$(INTDIR)\ximainfo.obj"
	-@erase "$(INTDIR)\ximaint.obj"
	-@erase "$(INTDIR)\ximaj2k.obj"
	-@erase "$(INTDIR)\ximajas.obj"
	-@erase "$(INTDIR)\ximajbg.obj"
	-@erase "$(INTDIR)\ximajpg.obj"
	-@erase "$(INTDIR)\ximalpha.obj"
	-@erase "$(INTDIR)\ximalyr.obj"
	-@erase "$(INTDIR)\ximamng.obj"
	-@erase "$(INTDIR)\ximapal.obj"
	-@erase "$(INTDIR)\ximapcx.obj"
	-@erase "$(INTDIR)\ximapng.obj"
	-@erase "$(INTDIR)\ximasel.obj"
	-@erase "$(INTDIR)\ximatga.obj"
	-@erase "$(INTDIR)\ximath.obj"
	-@erase "$(INTDIR)\ximatif.obj"
	-@erase "$(INTDIR)\ximatran.obj"
	-@erase "$(INTDIR)\ximawbmp.obj"
	-@erase "$(INTDIR)\ximawmf.obj"
	-@erase "$(INTDIR)\ximawnd.obj"
	-@erase "$(INTDIR)\xmemfile.obj"
	-@erase "$(OUTDIR)\cximage.exp"
	-@erase "$(OUTDIR)\cximage.lib"
	-@erase "..\..\bin\cximage.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "../../zlib" /I "../../jasper/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "JAS_WIN_MSVC_BUILD" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x410 /fo"$(INTDIR)\cximagemfcdll.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\cximagemfcdll.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\cximage.pdb" /machine:I386 /out:"../../bin/cximage.dll" /implib:"$(OUTDIR)\cximage.lib" 
LINK32_OBJS= \
	"$(INTDIR)\cximagemfcdll.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\tif_xfile.obj" \
	"$(INTDIR)\ximabmp.obj" \
	"$(INTDIR)\ximadsp.obj" \
	"$(INTDIR)\ximaenc.obj" \
	"$(INTDIR)\ximaexif.obj" \
	"$(INTDIR)\ximage.obj" \
	"$(INTDIR)\ximagif.obj" \
	"$(INTDIR)\ximahist.obj" \
	"$(INTDIR)\ximaico.obj" \
	"$(INTDIR)\ximainfo.obj" \
	"$(INTDIR)\ximaint.obj" \
	"$(INTDIR)\ximaj2k.obj" \
	"$(INTDIR)\ximajas.obj" \
	"$(INTDIR)\ximajbg.obj" \
	"$(INTDIR)\ximajpg.obj" \
	"$(INTDIR)\ximalpha.obj" \
	"$(INTDIR)\ximalyr.obj" \
	"$(INTDIR)\ximamng.obj" \
	"$(INTDIR)\ximapal.obj" \
	"$(INTDIR)\ximapcx.obj" \
	"$(INTDIR)\ximapng.obj" \
	"$(INTDIR)\ximasel.obj" \
	"$(INTDIR)\ximatga.obj" \
	"$(INTDIR)\ximath.obj" \
	"$(INTDIR)\ximatif.obj" \
	"$(INTDIR)\ximatran.obj" \
	"$(INTDIR)\ximawbmp.obj" \
	"$(INTDIR)\ximawmf.obj" \
	"$(INTDIR)\ximawnd.obj" \
	"$(INTDIR)\xmemfile.obj" \
	"$(INTDIR)\cximagemfcdll.res" \
	"..\..\j2k\Release\j2k.lib" \
	"..\..\jbig\Release\jbig.lib" \
	"..\..\jpeg\Release\Jpeg.lib" \
	"..\..\png\Release\png.lib" \
	"..\..\tiff\Release\Tiff.lib" \
	"..\..\zlib\Release\zlib.lib" \
	"..\..\jasper\Release\jasper.lib"

"..\..\bin\cximage.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "CxImageMfcDll - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "..\..\bin\cximaged.dll" "$(OUTDIR)\cximagemfcdll.pch"

!ELSE 

ALL : "jasper - Win32 Debug" "zlib - Win32 Debug" "tiff - Win32 Debug" "png - Win32 Debug" "jpeg - Win32 Debug" "jbig - Win32 Debug" "j2k - Win32 Debug" "..\..\bin\cximaged.dll" "$(OUTDIR)\cximagemfcdll.pch"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"j2k - Win32 DebugCLEAN" "jbig - Win32 DebugCLEAN" "jpeg - Win32 DebugCLEAN" "png - Win32 DebugCLEAN" "tiff - Win32 DebugCLEAN" "zlib - Win32 DebugCLEAN" "jasper - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\cximagemfcdll.obj"
	-@erase "$(INTDIR)\cximagemfcdll.pch"
	-@erase "$(INTDIR)\cximagemfcdll.res"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\tif_xfile.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\ximabmp.obj"
	-@erase "$(INTDIR)\ximadsp.obj"
	-@erase "$(INTDIR)\ximaenc.obj"
	-@erase "$(INTDIR)\ximaexif.obj"
	-@erase "$(INTDIR)\ximage.obj"
	-@erase "$(INTDIR)\ximagif.obj"
	-@erase "$(INTDIR)\ximahist.obj"
	-@erase "$(INTDIR)\ximaico.obj"
	-@erase "$(INTDIR)\ximainfo.obj"
	-@erase "$(INTDIR)\ximaint.obj"
	-@erase "$(INTDIR)\ximaj2k.obj"
	-@erase "$(INTDIR)\ximajas.obj"
	-@erase "$(INTDIR)\ximajbg.obj"
	-@erase "$(INTDIR)\ximajpg.obj"
	-@erase "$(INTDIR)\ximalpha.obj"
	-@erase "$(INTDIR)\ximalyr.obj"
	-@erase "$(INTDIR)\ximamng.obj"
	-@erase "$(INTDIR)\ximapal.obj"
	-@erase "$(INTDIR)\ximapcx.obj"
	-@erase "$(INTDIR)\ximapng.obj"
	-@erase "$(INTDIR)\ximasel.obj"
	-@erase "$(INTDIR)\ximatga.obj"
	-@erase "$(INTDIR)\ximath.obj"
	-@erase "$(INTDIR)\ximatif.obj"
	-@erase "$(INTDIR)\ximatran.obj"
	-@erase "$(INTDIR)\ximawbmp.obj"
	-@erase "$(INTDIR)\ximawmf.obj"
	-@erase "$(INTDIR)\ximawnd.obj"
	-@erase "$(INTDIR)\xmemfile.obj"
	-@erase "$(OUTDIR)\cximaged.exp"
	-@erase "$(OUTDIR)\cximaged.lib"
	-@erase "$(OUTDIR)\cximaged.pdb"
	-@erase "..\..\bin\cximaged.dll"
	-@erase "..\..\bin\cximaged.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../zlib" /I "../../jasper/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "JAS_WIN_MSVC_BUILD" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x410 /fo"$(INTDIR)\cximagemfcdll.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\cximagemfcdll.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\cximaged.pdb" /debug /machine:I386 /out:"../../bin/cximaged.dll" /implib:"$(OUTDIR)\cximaged.lib" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\cximagemfcdll.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\tif_xfile.obj" \
	"$(INTDIR)\ximabmp.obj" \
	"$(INTDIR)\ximadsp.obj" \
	"$(INTDIR)\ximaenc.obj" \
	"$(INTDIR)\ximaexif.obj" \
	"$(INTDIR)\ximage.obj" \
	"$(INTDIR)\ximagif.obj" \
	"$(INTDIR)\ximahist.obj" \
	"$(INTDIR)\ximaico.obj" \
	"$(INTDIR)\ximainfo.obj" \
	"$(INTDIR)\ximaint.obj" \
	"$(INTDIR)\ximaj2k.obj" \
	"$(INTDIR)\ximajas.obj" \
	"$(INTDIR)\ximajbg.obj" \
	"$(INTDIR)\ximajpg.obj" \
	"$(INTDIR)\ximalpha.obj" \
	"$(INTDIR)\ximalyr.obj" \
	"$(INTDIR)\ximamng.obj" \
	"$(INTDIR)\ximapal.obj" \
	"$(INTDIR)\ximapcx.obj" \
	"$(INTDIR)\ximapng.obj" \
	"$(INTDIR)\ximasel.obj" \
	"$(INTDIR)\ximatga.obj" \
	"$(INTDIR)\ximath.obj" \
	"$(INTDIR)\ximatif.obj" \
	"$(INTDIR)\ximatran.obj" \
	"$(INTDIR)\ximawbmp.obj" \
	"$(INTDIR)\ximawmf.obj" \
	"$(INTDIR)\ximawnd.obj" \
	"$(INTDIR)\xmemfile.obj" \
	"$(INTDIR)\cximagemfcdll.res" \
	"..\..\j2k\Debug\j2k.lib" \
	"..\..\jbig\Debug\jbig.lib" \
	"..\..\jpeg\Debug\Jpeg.lib" \
	"..\..\png\Debug\png.lib" \
	"..\..\tiff\Debug\Tiff.lib" \
	"..\..\zlib\Debug\zlib.lib" \
	"..\..\jasper\Debug\jasper.lib"

"..\..\bin\cximaged.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("cximagemfcdll.dep")
!INCLUDE "cximagemfcdll.dep"
!ELSE 
!MESSAGE Warning: cannot find "cximagemfcdll.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "CxImageMfcDll - Win32 Release" || "$(CFG)" == "CxImageMfcDll - Win32 Debug"
SOURCE=.\cximagemfcdll.cpp

"$(INTDIR)\cximagemfcdll.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\cximagemfcdll.rc

"$(INTDIR)\cximagemfcdll.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "CxImageMfcDll - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "../../zlib" /I "../../jasper/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "JAS_WIN_MSVC_BUILD" /Fp"$(INTDIR)\cximagemfcdll.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\cximagemfcdll.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "CxImageMfcDll - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../zlib" /I "../../jasper/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "JAS_WIN_MSVC_BUILD" /Fp"$(INTDIR)\cximagemfcdll.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\cximagemfcdll.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\tif_xfile.cpp

"$(INTDIR)\tif_xfile.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ximabmp.cpp

"$(INTDIR)\ximabmp.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ximadsp.cpp

"$(INTDIR)\ximadsp.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ximaenc.cpp

"$(INTDIR)\ximaenc.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ximaexif.cpp

"$(INTDIR)\ximaexif.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ximage.cpp

"$(INTDIR)\ximage.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ximagif.cpp

"$(INTDIR)\ximagif.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ximahist.cpp

"$(INTDIR)\ximahist.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ximaico.cpp

"$(INTDIR)\ximaico.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ximainfo.cpp

"$(INTDIR)\ximainfo.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ximaint.cpp

"$(INTDIR)\ximaint.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ximaj2k.cpp

"$(INTDIR)\ximaj2k.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ximajas.cpp

"$(INTDIR)\ximajas.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ximajbg.cpp

"$(INTDIR)\ximajbg.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ximajpg.cpp

"$(INTDIR)\ximajpg.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ximalpha.cpp

"$(INTDIR)\ximalpha.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ximalyr.cpp

"$(INTDIR)\ximalyr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ximamng.cpp

"$(INTDIR)\ximamng.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ximapal.cpp

"$(INTDIR)\ximapal.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ximapcx.cpp

"$(INTDIR)\ximapcx.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ximapng.cpp

"$(INTDIR)\ximapng.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ximasel.cpp

"$(INTDIR)\ximasel.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ximatga.cpp

"$(INTDIR)\ximatga.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ximath.cpp

"$(INTDIR)\ximath.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ximatif.cpp

"$(INTDIR)\ximatif.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ximatran.cpp

"$(INTDIR)\ximatran.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ximawbmp.cpp

"$(INTDIR)\ximawbmp.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ximawmf.cpp

"$(INTDIR)\ximawmf.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ximawnd.cpp

"$(INTDIR)\ximawnd.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\xmemfile.cpp

"$(INTDIR)\xmemfile.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!IF  "$(CFG)" == "CxImageMfcDll - Win32 Release"

"j2k - Win32 Release" : 
   cd "\develop\mfc\cximage\j2k"
   $(MAKE) /$(MAKEFLAGS) /F .\j2k.mak CFG="j2k - Win32 Release" 
   cd "..\CxImage\CxImageDLL"

"j2k - Win32 ReleaseCLEAN" : 
   cd "\develop\mfc\cximage\j2k"
   $(MAKE) /$(MAKEFLAGS) /F .\j2k.mak CFG="j2k - Win32 Release" RECURSE=1 CLEAN 
   cd "..\CxImage\CxImageDLL"

!ELSEIF  "$(CFG)" == "CxImageMfcDll - Win32 Debug"

"j2k - Win32 Debug" : 
   cd "\develop\mfc\cximage\j2k"
   $(MAKE) /$(MAKEFLAGS) /F .\j2k.mak CFG="j2k - Win32 Debug" 
   cd "..\CxImage\CxImageDLL"

"j2k - Win32 DebugCLEAN" : 
   cd "\develop\mfc\cximage\j2k"
   $(MAKE) /$(MAKEFLAGS) /F .\j2k.mak CFG="j2k - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\CxImage\CxImageDLL"

!ENDIF 

!IF  "$(CFG)" == "CxImageMfcDll - Win32 Release"

"jbig - Win32 Release" : 
   cd "\develop\mfc\cximage\jbig"
   $(MAKE) /$(MAKEFLAGS) /F .\jbig.mak CFG="jbig - Win32 Release" 
   cd "..\CxImage\CxImageDLL"

"jbig - Win32 ReleaseCLEAN" : 
   cd "\develop\mfc\cximage\jbig"
   $(MAKE) /$(MAKEFLAGS) /F .\jbig.mak CFG="jbig - Win32 Release" RECURSE=1 CLEAN 
   cd "..\CxImage\CxImageDLL"

!ELSEIF  "$(CFG)" == "CxImageMfcDll - Win32 Debug"

"jbig - Win32 Debug" : 
   cd "\develop\mfc\cximage\jbig"
   $(MAKE) /$(MAKEFLAGS) /F .\jbig.mak CFG="jbig - Win32 Debug" 
   cd "..\CxImage\CxImageDLL"

"jbig - Win32 DebugCLEAN" : 
   cd "\develop\mfc\cximage\jbig"
   $(MAKE) /$(MAKEFLAGS) /F .\jbig.mak CFG="jbig - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\CxImage\CxImageDLL"

!ENDIF 

!IF  "$(CFG)" == "CxImageMfcDll - Win32 Release"

"jpeg - Win32 Release" : 
   cd "\develop\mfc\cximage\jpeg"
   $(MAKE) /$(MAKEFLAGS) /F .\Jpeg.mak CFG="jpeg - Win32 Release" 
   cd "..\CxImage\CxImageDLL"

"jpeg - Win32 ReleaseCLEAN" : 
   cd "\develop\mfc\cximage\jpeg"
   $(MAKE) /$(MAKEFLAGS) /F .\Jpeg.mak CFG="jpeg - Win32 Release" RECURSE=1 CLEAN 
   cd "..\CxImage\CxImageDLL"

!ELSEIF  "$(CFG)" == "CxImageMfcDll - Win32 Debug"

"jpeg - Win32 Debug" : 
   cd "\develop\mfc\cximage\jpeg"
   $(MAKE) /$(MAKEFLAGS) /F .\Jpeg.mak CFG="jpeg - Win32 Debug" 
   cd "..\CxImage\CxImageDLL"

"jpeg - Win32 DebugCLEAN" : 
   cd "\develop\mfc\cximage\jpeg"
   $(MAKE) /$(MAKEFLAGS) /F .\Jpeg.mak CFG="jpeg - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\CxImage\CxImageDLL"

!ENDIF 

!IF  "$(CFG)" == "CxImageMfcDll - Win32 Release"

"png - Win32 Release" : 
   cd "\develop\mfc\cximage\png"
   $(MAKE) /$(MAKEFLAGS) /F .\png.mak CFG="png - Win32 Release" 
   cd "..\CxImage\CxImageDLL"

"png - Win32 ReleaseCLEAN" : 
   cd "\develop\mfc\cximage\png"
   $(MAKE) /$(MAKEFLAGS) /F .\png.mak CFG="png - Win32 Release" RECURSE=1 CLEAN 
   cd "..\CxImage\CxImageDLL"

!ELSEIF  "$(CFG)" == "CxImageMfcDll - Win32 Debug"

"png - Win32 Debug" : 
   cd "\develop\mfc\cximage\png"
   $(MAKE) /$(MAKEFLAGS) /F .\png.mak CFG="png - Win32 Debug" 
   cd "..\CxImage\CxImageDLL"

"png - Win32 DebugCLEAN" : 
   cd "\develop\mfc\cximage\png"
   $(MAKE) /$(MAKEFLAGS) /F .\png.mak CFG="png - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\CxImage\CxImageDLL"

!ENDIF 

!IF  "$(CFG)" == "CxImageMfcDll - Win32 Release"

"tiff - Win32 Release" : 
   cd "\develop\mfc\cximage\tiff"
   $(MAKE) /$(MAKEFLAGS) /F .\Tiff.mak CFG="tiff - Win32 Release" 
   cd "..\CxImage\CxImageDLL"

"tiff - Win32 ReleaseCLEAN" : 
   cd "\develop\mfc\cximage\tiff"
   $(MAKE) /$(MAKEFLAGS) /F .\Tiff.mak CFG="tiff - Win32 Release" RECURSE=1 CLEAN 
   cd "..\CxImage\CxImageDLL"

!ELSEIF  "$(CFG)" == "CxImageMfcDll - Win32 Debug"

"tiff - Win32 Debug" : 
   cd "\develop\mfc\cximage\tiff"
   $(MAKE) /$(MAKEFLAGS) /F .\Tiff.mak CFG="tiff - Win32 Debug" 
   cd "..\CxImage\CxImageDLL"

"tiff - Win32 DebugCLEAN" : 
   cd "\develop\mfc\cximage\tiff"
   $(MAKE) /$(MAKEFLAGS) /F .\Tiff.mak CFG="tiff - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\CxImage\CxImageDLL"

!ENDIF 

!IF  "$(CFG)" == "CxImageMfcDll - Win32 Release"

"zlib - Win32 Release" : 
   cd "\develop\mfc\cximage\zlib"
   $(MAKE) /$(MAKEFLAGS) /F .\zlib.mak CFG="zlib - Win32 Release" 
   cd "..\CxImage\CxImageDLL"

"zlib - Win32 ReleaseCLEAN" : 
   cd "\develop\mfc\cximage\zlib"
   $(MAKE) /$(MAKEFLAGS) /F .\zlib.mak CFG="zlib - Win32 Release" RECURSE=1 CLEAN 
   cd "..\CxImage\CxImageDLL"

!ELSEIF  "$(CFG)" == "CxImageMfcDll - Win32 Debug"

"zlib - Win32 Debug" : 
   cd "\develop\mfc\cximage\zlib"
   $(MAKE) /$(MAKEFLAGS) /F .\zlib.mak CFG="zlib - Win32 Debug" 
   cd "..\CxImage\CxImageDLL"

"zlib - Win32 DebugCLEAN" : 
   cd "\develop\mfc\cximage\zlib"
   $(MAKE) /$(MAKEFLAGS) /F .\zlib.mak CFG="zlib - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\CxImage\CxImageDLL"

!ENDIF 

!IF  "$(CFG)" == "CxImageMfcDll - Win32 Release"

"jasper - Win32 Release" : 
   cd "\develop\mfc\cximage\jasper"
   $(MAKE) /$(MAKEFLAGS) /F .\jasper.mak CFG="jasper - Win32 Release" 
   cd "..\CxImage\CxImageDLL"

"jasper - Win32 ReleaseCLEAN" : 
   cd "\develop\mfc\cximage\jasper"
   $(MAKE) /$(MAKEFLAGS) /F .\jasper.mak CFG="jasper - Win32 Release" RECURSE=1 CLEAN 
   cd "..\CxImage\CxImageDLL"

!ELSEIF  "$(CFG)" == "CxImageMfcDll - Win32 Debug"

"jasper - Win32 Debug" : 
   cd "\develop\mfc\cximage\jasper"
   $(MAKE) /$(MAKEFLAGS) /F .\jasper.mak CFG="jasper - Win32 Debug" 
   cd "..\CxImage\CxImageDLL"

"jasper - Win32 DebugCLEAN" : 
   cd "\develop\mfc\cximage\jasper"
   $(MAKE) /$(MAKEFLAGS) /F .\jasper.mak CFG="jasper - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\CxImage\CxImageDLL"

!ENDIF 


!ENDIF 

