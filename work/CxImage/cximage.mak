# Microsoft Developer Studio Generated NMAKE File, Based on cximage.dsp
!IF "$(CFG)" == ""
CFG=CxImage - Win32 Debug
!MESSAGE No configuration specified. Defaulting to CxImage - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "CxImage - Win32 Release" && "$(CFG)" != "CxImage - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "cximage.mak" CFG="CxImage - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CxImage - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "CxImage - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "CxImage - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\cximage.lib"


CLEAN :
	-@erase "$(INTDIR)\tif_xfile.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\ximabmp.obj"
	-@erase "$(INTDIR)\ximadsp.obj"
	-@erase "$(INTDIR)\ximaenc.obj"
	-@erase "$(INTDIR)\ximaexif.obj"
	-@erase "$(INTDIR)\xImage.obj"
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
	-@erase "$(OUTDIR)\cximage.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "..\jpeg" /I "..\png" /I "..\zlib" /I "..\mng" /I "..\tiff" /I "..\j2k" /I "..\jasper\include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_AFXDLL" /D "JAS_WIN_MSVC_BUILD" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\cximage.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\cximage.lib" 
LIB32_OBJS= \
	"$(INTDIR)\tif_xfile.obj" \
	"$(INTDIR)\ximabmp.obj" \
	"$(INTDIR)\ximadsp.obj" \
	"$(INTDIR)\ximaenc.obj" \
	"$(INTDIR)\ximaexif.obj" \
	"$(INTDIR)\xImage.obj" \
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
	"$(INTDIR)\xmemfile.obj"

"$(OUTDIR)\cximage.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "CxImage - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\cximage.lib"


CLEAN :
	-@erase "$(INTDIR)\tif_xfile.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\ximabmp.obj"
	-@erase "$(INTDIR)\ximadsp.obj"
	-@erase "$(INTDIR)\ximaenc.obj"
	-@erase "$(INTDIR)\ximaexif.obj"
	-@erase "$(INTDIR)\xImage.obj"
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
	-@erase "$(OUTDIR)\cximage.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\jpeg" /I "..\png" /I "..\zlib" /I "..\mng" /I "..\tiff" /I "..\j2k" /I "..\jasper\include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_AFXDLL" /D "JAS_WIN_MSVC_BUILD" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\cximage.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\cximage.lib" 
LIB32_OBJS= \
	"$(INTDIR)\tif_xfile.obj" \
	"$(INTDIR)\ximabmp.obj" \
	"$(INTDIR)\ximadsp.obj" \
	"$(INTDIR)\ximaenc.obj" \
	"$(INTDIR)\ximaexif.obj" \
	"$(INTDIR)\xImage.obj" \
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
	"$(INTDIR)\xmemfile.obj"

"$(OUTDIR)\cximage.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("cximage.dep")
!INCLUDE "cximage.dep"
!ELSE 
!MESSAGE Warning: cannot find "cximage.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "CxImage - Win32 Release" || "$(CFG)" == "CxImage - Win32 Debug"
SOURCE=.\tif_xfile.cpp

"$(INTDIR)\tif_xfile.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ximabmp.cpp

"$(INTDIR)\ximabmp.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ximadsp.cpp

"$(INTDIR)\ximadsp.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ximaenc.cpp

"$(INTDIR)\ximaenc.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ximaexif.cpp

"$(INTDIR)\ximaexif.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\xImage.cpp

"$(INTDIR)\xImage.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ximagif.cpp

"$(INTDIR)\ximagif.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ximahist.cpp

"$(INTDIR)\ximahist.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ximaico.cpp

"$(INTDIR)\ximaico.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ximainfo.cpp

"$(INTDIR)\ximainfo.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ximaint.cpp

"$(INTDIR)\ximaint.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ximaj2k.cpp

"$(INTDIR)\ximaj2k.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ximajas.cpp

"$(INTDIR)\ximajas.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ximajbg.cpp

"$(INTDIR)\ximajbg.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ximajpg.cpp

"$(INTDIR)\ximajpg.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ximalpha.cpp

"$(INTDIR)\ximalpha.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ximalyr.cpp

"$(INTDIR)\ximalyr.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ximamng.cpp

"$(INTDIR)\ximamng.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ximapal.cpp

"$(INTDIR)\ximapal.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ximapcx.cpp

"$(INTDIR)\ximapcx.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ximapng.cpp

"$(INTDIR)\ximapng.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ximasel.cpp

"$(INTDIR)\ximasel.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ximatga.cpp

"$(INTDIR)\ximatga.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ximath.cpp

"$(INTDIR)\ximath.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ximatif.cpp

"$(INTDIR)\ximatif.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ximatran.cpp

"$(INTDIR)\ximatran.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ximawbmp.cpp

"$(INTDIR)\ximawbmp.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ximawmf.cpp

"$(INTDIR)\ximawmf.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ximawnd.cpp

"$(INTDIR)\ximawnd.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\xmemfile.cpp

"$(INTDIR)\xmemfile.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

