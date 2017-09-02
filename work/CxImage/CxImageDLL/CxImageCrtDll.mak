# Microsoft Developer Studio Generated NMAKE File, Based on CxImageCrtDll.dsp
!IF "$(CFG)" == ""
CFG=CxImageCrtDll - Win32 Debug
!MESSAGE No configuration specified. Defaulting to CxImageCrtDll - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "CxImageCrtDll - Win32 Release" && "$(CFG)" != "CxImageCrtDll - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CxImageCrtDll.mak" CFG="CxImageCrtDll - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CxImageCrtDll - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CxImageCrtDll - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "CxImageCrtDll - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

!IF "$(RECURSE)" == "0" 

ALL : "..\..\bin\cximagecrt.dll"

!ELSE 

ALL : "jasper - Win32 Release" "zlib - Win32 Release" "tiff - Win32 Release" "png - Win32 Release" "jpeg - Win32 Release" "jbig - Win32 Release" "j2k - Win32 Release" "..\..\bin\cximagecrt.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"j2k - Win32 ReleaseCLEAN" "jbig - Win32 ReleaseCLEAN" "jpeg - Win32 ReleaseCLEAN" "png - Win32 ReleaseCLEAN" "tiff - Win32 ReleaseCLEAN" "zlib - Win32 ReleaseCLEAN" "jasper - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\CxImageCrtDll.obj"
	-@erase "$(INTDIR)\CxImageCrtDll.res"
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
	-@erase "$(OUTDIR)\cximagecrt.exp"
	-@erase "$(OUTDIR)\cximagecrt.lib"
	-@erase "..\..\bin\cximagecrt.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "../../zlib" /I "../../jasper/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "JAS_WIN_MSVC_BUILD" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\CxImageCrtDll.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\CxImageCrtDll.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\cximagecrt.pdb" /machine:I386 /out:"../../bin/cximagecrt.dll" /implib:"$(OUTDIR)\cximagecrt.lib" 
LINK32_OBJS= \
	"$(INTDIR)\CxImageCrtDll.obj" \
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
	"$(INTDIR)\CxImageCrtDll.res" \
	"..\..\j2k\Release\j2k.lib" \
	"..\..\jbig\Release\jbig.lib" \
	"..\..\jpeg\Release\Jpeg.lib" \
	"..\..\png\Release\png.lib" \
	"..\..\tiff\Release\Tiff.lib" \
	"..\..\zlib\Release\zlib.lib" \
	"..\..\jasper\Release\jasper.lib"

"..\..\bin\cximagecrt.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "CxImageCrtDll - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

!IF "$(RECURSE)" == "0" 

ALL : "..\..\bin\cximagecrtd.dll"

!ELSE 

ALL : "jasper - Win32 Debug" "zlib - Win32 Debug" "tiff - Win32 Debug" "png - Win32 Debug" "jpeg - Win32 Debug" "jbig - Win32 Debug" "j2k - Win32 Debug" "..\..\bin\cximagecrtd.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"j2k - Win32 DebugCLEAN" "jbig - Win32 DebugCLEAN" "jpeg - Win32 DebugCLEAN" "png - Win32 DebugCLEAN" "tiff - Win32 DebugCLEAN" "zlib - Win32 DebugCLEAN" "jasper - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\CxImageCrtDll.obj"
	-@erase "$(INTDIR)\CxImageCrtDll.res"
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
	-@erase "$(OUTDIR)\cximagecrtd.exp"
	-@erase "$(OUTDIR)\cximagecrtd.lib"
	-@erase "$(OUTDIR)\cximagecrtd.pdb"
	-@erase "..\..\bin\cximagecrtd.dll"
	-@erase "..\..\bin\cximagecrtd.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../zlib" /I "../../jasper/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "JAS_WIN_MSVC_BUILD" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\CxImageCrtDll.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\CxImageCrtDll.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\cximagecrtd.pdb" /debug /machine:I386 /out:"../../bin/cximagecrtd.dll" /implib:"$(OUTDIR)\cximagecrtd.lib" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\CxImageCrtDll.obj" \
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
	"$(INTDIR)\CxImageCrtDll.res" \
	"..\..\j2k\Debug\j2k.lib" \
	"..\..\jbig\Debug\jbig.lib" \
	"..\..\jpeg\Debug\Jpeg.lib" \
	"..\..\png\Debug\png.lib" \
	"..\..\tiff\Debug\Tiff.lib" \
	"..\..\zlib\Debug\zlib.lib" \
	"..\..\jasper\Debug\jasper.lib"

"..\..\bin\cximagecrtd.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("CxImageCrtDll.dep")
!INCLUDE "CxImageCrtDll.dep"
!ELSE 
!MESSAGE Warning: cannot find "CxImageCrtDll.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "CxImageCrtDll - Win32 Release" || "$(CFG)" == "CxImageCrtDll - Win32 Debug"
SOURCE=.\CxImageCrtDll.cpp

"$(INTDIR)\CxImageCrtDll.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\CxImageCrtDll.rc

"$(INTDIR)\CxImageCrtDll.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=..\tif_xfile.cpp

"$(INTDIR)\tif_xfile.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ximabmp.cpp

!IF  "$(CFG)" == "CxImageCrtDll - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "../../zlib" /I "../../jasper/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "JAS_WIN_MSVC_BUILD" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\ximabmp.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "CxImageCrtDll - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../zlib" /I "../../jasper/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "JAS_WIN_MSVC_BUILD" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\ximabmp.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\ximadsp.cpp

!IF  "$(CFG)" == "CxImageCrtDll - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "../../zlib" /I "../../jasper/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "JAS_WIN_MSVC_BUILD" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\ximadsp.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "CxImageCrtDll - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../zlib" /I "../../jasper/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "JAS_WIN_MSVC_BUILD" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\ximadsp.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\ximaenc.cpp

!IF  "$(CFG)" == "CxImageCrtDll - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "../../zlib" /I "../../jasper/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "JAS_WIN_MSVC_BUILD" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\ximaenc.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "CxImageCrtDll - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../zlib" /I "../../jasper/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "JAS_WIN_MSVC_BUILD" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\ximaenc.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\ximaexif.cpp

!IF  "$(CFG)" == "CxImageCrtDll - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "../../zlib" /I "../../jasper/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "JAS_WIN_MSVC_BUILD" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\ximaexif.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "CxImageCrtDll - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../zlib" /I "../../jasper/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "JAS_WIN_MSVC_BUILD" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\ximaexif.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\ximage.cpp

!IF  "$(CFG)" == "CxImageCrtDll - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "../../zlib" /I "../../jasper/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "JAS_WIN_MSVC_BUILD" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\ximage.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "CxImageCrtDll - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../zlib" /I "../../jasper/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "JAS_WIN_MSVC_BUILD" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\ximage.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\ximagif.cpp

!IF  "$(CFG)" == "CxImageCrtDll - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "../../zlib" /I "../../jasper/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "JAS_WIN_MSVC_BUILD" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\ximagif.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "CxImageCrtDll - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../zlib" /I "../../jasper/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "JAS_WIN_MSVC_BUILD" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\ximagif.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\ximahist.cpp

"$(INTDIR)\ximahist.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ximaico.cpp

!IF  "$(CFG)" == "CxImageCrtDll - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "../../zlib" /I "../../jasper/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "JAS_WIN_MSVC_BUILD" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\ximaico.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "CxImageCrtDll - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../zlib" /I "../../jasper/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "JAS_WIN_MSVC_BUILD" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\ximaico.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\ximainfo.cpp

"$(INTDIR)\ximainfo.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ximaint.cpp

"$(INTDIR)\ximaint.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ximaj2k.cpp

!IF  "$(CFG)" == "CxImageCrtDll - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "../../zlib" /I "../../jasper/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "JAS_WIN_MSVC_BUILD" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\ximaj2k.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "CxImageCrtDll - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../zlib" /I "../../jasper/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "JAS_WIN_MSVC_BUILD" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\ximaj2k.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\ximajas.cpp

"$(INTDIR)\ximajas.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ximajbg.cpp

!IF  "$(CFG)" == "CxImageCrtDll - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "../../zlib" /I "../../jasper/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "JAS_WIN_MSVC_BUILD" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\ximajbg.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "CxImageCrtDll - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../zlib" /I "../../jasper/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "JAS_WIN_MSVC_BUILD" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\ximajbg.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\ximajpg.cpp

!IF  "$(CFG)" == "CxImageCrtDll - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "../../zlib" /I "../../jasper/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "JAS_WIN_MSVC_BUILD" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\ximajpg.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "CxImageCrtDll - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../zlib" /I "../../jasper/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "JAS_WIN_MSVC_BUILD" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\ximajpg.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\ximalpha.cpp

!IF  "$(CFG)" == "CxImageCrtDll - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "../../zlib" /I "../../jasper/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "JAS_WIN_MSVC_BUILD" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\ximalpha.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "CxImageCrtDll - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../zlib" /I "../../jasper/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "JAS_WIN_MSVC_BUILD" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\ximalpha.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\ximalyr.cpp

"$(INTDIR)\ximalyr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ximamng.cpp

!IF  "$(CFG)" == "CxImageCrtDll - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "../../zlib" /I "../../jasper/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "JAS_WIN_MSVC_BUILD" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\ximamng.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "CxImageCrtDll - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../zlib" /I "../../jasper/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "JAS_WIN_MSVC_BUILD" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\ximamng.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\ximapal.cpp

!IF  "$(CFG)" == "CxImageCrtDll - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "../../zlib" /I "../../jasper/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "JAS_WIN_MSVC_BUILD" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\ximapal.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "CxImageCrtDll - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../zlib" /I "../../jasper/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "JAS_WIN_MSVC_BUILD" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\ximapal.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\ximapcx.cpp

!IF  "$(CFG)" == "CxImageCrtDll - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "../../zlib" /I "../../jasper/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "JAS_WIN_MSVC_BUILD" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\ximapcx.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "CxImageCrtDll - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../zlib" /I "../../jasper/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "JAS_WIN_MSVC_BUILD" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\ximapcx.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\ximapng.cpp

!IF  "$(CFG)" == "CxImageCrtDll - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "../../zlib" /I "../../jasper/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "JAS_WIN_MSVC_BUILD" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\ximapng.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "CxImageCrtDll - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../zlib" /I "../../jasper/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "JAS_WIN_MSVC_BUILD" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\ximapng.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\ximasel.cpp

!IF  "$(CFG)" == "CxImageCrtDll - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "../../zlib" /I "../../jasper/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "JAS_WIN_MSVC_BUILD" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\ximasel.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "CxImageCrtDll - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../zlib" /I "../../jasper/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "JAS_WIN_MSVC_BUILD" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\ximasel.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\ximatga.cpp

!IF  "$(CFG)" == "CxImageCrtDll - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "../../zlib" /I "../../jasper/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "JAS_WIN_MSVC_BUILD" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\ximatga.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "CxImageCrtDll - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../zlib" /I "../../jasper/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "JAS_WIN_MSVC_BUILD" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\ximatga.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\ximath.cpp

"$(INTDIR)\ximath.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ximatif.cpp

!IF  "$(CFG)" == "CxImageCrtDll - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "../../zlib" /I "../../jasper/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "JAS_WIN_MSVC_BUILD" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\ximatif.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "CxImageCrtDll - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../zlib" /I "../../jasper/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "JAS_WIN_MSVC_BUILD" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\ximatif.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\ximatran.cpp

!IF  "$(CFG)" == "CxImageCrtDll - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "../../zlib" /I "../../jasper/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "JAS_WIN_MSVC_BUILD" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\ximatran.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "CxImageCrtDll - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../zlib" /I "../../jasper/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "JAS_WIN_MSVC_BUILD" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\ximatran.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\ximawbmp.cpp

!IF  "$(CFG)" == "CxImageCrtDll - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "../../zlib" /I "../../jasper/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "JAS_WIN_MSVC_BUILD" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\ximawbmp.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "CxImageCrtDll - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../zlib" /I "../../jasper/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "JAS_WIN_MSVC_BUILD" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\ximawbmp.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\ximawmf.cpp

!IF  "$(CFG)" == "CxImageCrtDll - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "../../zlib" /I "../../jasper/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "JAS_WIN_MSVC_BUILD" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\ximawmf.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "CxImageCrtDll - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../zlib" /I "../../jasper/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "JAS_WIN_MSVC_BUILD" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\ximawmf.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\ximawnd.cpp

!IF  "$(CFG)" == "CxImageCrtDll - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "../../zlib" /I "../../jasper/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "JAS_WIN_MSVC_BUILD" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\ximawnd.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "CxImageCrtDll - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../zlib" /I "../../jasper/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "JAS_WIN_MSVC_BUILD" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\ximawnd.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\xmemfile.cpp

!IF  "$(CFG)" == "CxImageCrtDll - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "../../zlib" /I "../../jasper/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "JAS_WIN_MSVC_BUILD" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\xmemfile.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "CxImageCrtDll - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../zlib" /I "../../jasper/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "JAS_WIN_MSVC_BUILD" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\xmemfile.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

!IF  "$(CFG)" == "CxImageCrtDll - Win32 Release"

"j2k - Win32 Release" : 
   cd "\develop\mfc\cximage\j2k"
   $(MAKE) /$(MAKEFLAGS) /F .\j2k.mak CFG="j2k - Win32 Release" 
   cd "..\CxImage\CxImageDLL"

"j2k - Win32 ReleaseCLEAN" : 
   cd "\develop\mfc\cximage\j2k"
   $(MAKE) /$(MAKEFLAGS) /F .\j2k.mak CFG="j2k - Win32 Release" RECURSE=1 CLEAN 
   cd "..\CxImage\CxImageDLL"

!ELSEIF  "$(CFG)" == "CxImageCrtDll - Win32 Debug"

"j2k - Win32 Debug" : 
   cd "\develop\mfc\cximage\j2k"
   $(MAKE) /$(MAKEFLAGS) /F .\j2k.mak CFG="j2k - Win32 Debug" 
   cd "..\CxImage\CxImageDLL"

"j2k - Win32 DebugCLEAN" : 
   cd "\develop\mfc\cximage\j2k"
   $(MAKE) /$(MAKEFLAGS) /F .\j2k.mak CFG="j2k - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\CxImage\CxImageDLL"

!ENDIF 

!IF  "$(CFG)" == "CxImageCrtDll - Win32 Release"

"jbig - Win32 Release" : 
   cd "\develop\mfc\cximage\jbig"
   $(MAKE) /$(MAKEFLAGS) /F .\jbig.mak CFG="jbig - Win32 Release" 
   cd "..\CxImage\CxImageDLL"

"jbig - Win32 ReleaseCLEAN" : 
   cd "\develop\mfc\cximage\jbig"
   $(MAKE) /$(MAKEFLAGS) /F .\jbig.mak CFG="jbig - Win32 Release" RECURSE=1 CLEAN 
   cd "..\CxImage\CxImageDLL"

!ELSEIF  "$(CFG)" == "CxImageCrtDll - Win32 Debug"

"jbig - Win32 Debug" : 
   cd "\develop\mfc\cximage\jbig"
   $(MAKE) /$(MAKEFLAGS) /F .\jbig.mak CFG="jbig - Win32 Debug" 
   cd "..\CxImage\CxImageDLL"

"jbig - Win32 DebugCLEAN" : 
   cd "\develop\mfc\cximage\jbig"
   $(MAKE) /$(MAKEFLAGS) /F .\jbig.mak CFG="jbig - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\CxImage\CxImageDLL"

!ENDIF 

!IF  "$(CFG)" == "CxImageCrtDll - Win32 Release"

"jpeg - Win32 Release" : 
   cd "\develop\mfc\cximage\jpeg"
   $(MAKE) /$(MAKEFLAGS) /F .\Jpeg.mak CFG="jpeg - Win32 Release" 
   cd "..\CxImage\CxImageDLL"

"jpeg - Win32 ReleaseCLEAN" : 
   cd "\develop\mfc\cximage\jpeg"
   $(MAKE) /$(MAKEFLAGS) /F .\Jpeg.mak CFG="jpeg - Win32 Release" RECURSE=1 CLEAN 
   cd "..\CxImage\CxImageDLL"

!ELSEIF  "$(CFG)" == "CxImageCrtDll - Win32 Debug"

"jpeg - Win32 Debug" : 
   cd "\develop\mfc\cximage\jpeg"
   $(MAKE) /$(MAKEFLAGS) /F .\Jpeg.mak CFG="jpeg - Win32 Debug" 
   cd "..\CxImage\CxImageDLL"

"jpeg - Win32 DebugCLEAN" : 
   cd "\develop\mfc\cximage\jpeg"
   $(MAKE) /$(MAKEFLAGS) /F .\Jpeg.mak CFG="jpeg - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\CxImage\CxImageDLL"

!ENDIF 

!IF  "$(CFG)" == "CxImageCrtDll - Win32 Release"

"png - Win32 Release" : 
   cd "\develop\mfc\cximage\png"
   $(MAKE) /$(MAKEFLAGS) /F .\png.mak CFG="png - Win32 Release" 
   cd "..\CxImage\CxImageDLL"

"png - Win32 ReleaseCLEAN" : 
   cd "\develop\mfc\cximage\png"
   $(MAKE) /$(MAKEFLAGS) /F .\png.mak CFG="png - Win32 Release" RECURSE=1 CLEAN 
   cd "..\CxImage\CxImageDLL"

!ELSEIF  "$(CFG)" == "CxImageCrtDll - Win32 Debug"

"png - Win32 Debug" : 
   cd "\develop\mfc\cximage\png"
   $(MAKE) /$(MAKEFLAGS) /F .\png.mak CFG="png - Win32 Debug" 
   cd "..\CxImage\CxImageDLL"

"png - Win32 DebugCLEAN" : 
   cd "\develop\mfc\cximage\png"
   $(MAKE) /$(MAKEFLAGS) /F .\png.mak CFG="png - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\CxImage\CxImageDLL"

!ENDIF 

!IF  "$(CFG)" == "CxImageCrtDll - Win32 Release"

"tiff - Win32 Release" : 
   cd "\develop\mfc\cximage\tiff"
   $(MAKE) /$(MAKEFLAGS) /F .\Tiff.mak CFG="tiff - Win32 Release" 
   cd "..\CxImage\CxImageDLL"

"tiff - Win32 ReleaseCLEAN" : 
   cd "\develop\mfc\cximage\tiff"
   $(MAKE) /$(MAKEFLAGS) /F .\Tiff.mak CFG="tiff - Win32 Release" RECURSE=1 CLEAN 
   cd "..\CxImage\CxImageDLL"

!ELSEIF  "$(CFG)" == "CxImageCrtDll - Win32 Debug"

"tiff - Win32 Debug" : 
   cd "\develop\mfc\cximage\tiff"
   $(MAKE) /$(MAKEFLAGS) /F .\Tiff.mak CFG="tiff - Win32 Debug" 
   cd "..\CxImage\CxImageDLL"

"tiff - Win32 DebugCLEAN" : 
   cd "\develop\mfc\cximage\tiff"
   $(MAKE) /$(MAKEFLAGS) /F .\Tiff.mak CFG="tiff - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\CxImage\CxImageDLL"

!ENDIF 

!IF  "$(CFG)" == "CxImageCrtDll - Win32 Release"

"zlib - Win32 Release" : 
   cd "\develop\mfc\cximage\zlib"
   $(MAKE) /$(MAKEFLAGS) /F .\zlib.mak CFG="zlib - Win32 Release" 
   cd "..\CxImage\CxImageDLL"

"zlib - Win32 ReleaseCLEAN" : 
   cd "\develop\mfc\cximage\zlib"
   $(MAKE) /$(MAKEFLAGS) /F .\zlib.mak CFG="zlib - Win32 Release" RECURSE=1 CLEAN 
   cd "..\CxImage\CxImageDLL"

!ELSEIF  "$(CFG)" == "CxImageCrtDll - Win32 Debug"

"zlib - Win32 Debug" : 
   cd "\develop\mfc\cximage\zlib"
   $(MAKE) /$(MAKEFLAGS) /F .\zlib.mak CFG="zlib - Win32 Debug" 
   cd "..\CxImage\CxImageDLL"

"zlib - Win32 DebugCLEAN" : 
   cd "\develop\mfc\cximage\zlib"
   $(MAKE) /$(MAKEFLAGS) /F .\zlib.mak CFG="zlib - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\CxImage\CxImageDLL"

!ENDIF 

!IF  "$(CFG)" == "CxImageCrtDll - Win32 Release"

"jasper - Win32 Release" : 
   cd "\develop\mfc\cximage\jasper"
   $(MAKE) /$(MAKEFLAGS) /F .\jasper.mak CFG="jasper - Win32 Release" 
   cd "..\CxImage\CxImageDLL"

"jasper - Win32 ReleaseCLEAN" : 
   cd "\develop\mfc\cximage\jasper"
   $(MAKE) /$(MAKEFLAGS) /F .\jasper.mak CFG="jasper - Win32 Release" RECURSE=1 CLEAN 
   cd "..\CxImage\CxImageDLL"

!ELSEIF  "$(CFG)" == "CxImageCrtDll - Win32 Debug"

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

