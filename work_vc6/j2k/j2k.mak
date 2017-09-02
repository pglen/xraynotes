# Microsoft Developer Studio Generated NMAKE File, Based on j2k.dsp
!IF "$(CFG)" == ""
CFG=j2k - Win32 Debug
!MESSAGE No configuration specified. Defaulting to j2k - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "j2k - Win32 Release" && "$(CFG)" != "j2k - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "j2k.mak" CFG="j2k - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "j2k - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "j2k - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "j2k - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\j2k.lib"


CLEAN :
	-@erase "$(INTDIR)\bio.obj"
	-@erase "$(INTDIR)\cio.obj"
	-@erase "$(INTDIR)\dwt.obj"
	-@erase "$(INTDIR)\fix.obj"
	-@erase "$(INTDIR)\int.obj"
	-@erase "$(INTDIR)\j2k.obj"
	-@erase "$(INTDIR)\mct.obj"
	-@erase "$(INTDIR)\mqc.obj"
	-@erase "$(INTDIR)\pi.obj"
	-@erase "$(INTDIR)\t1.obj"
	-@erase "$(INTDIR)\t2.obj"
	-@erase "$(INTDIR)\tcd.obj"
	-@erase "$(INTDIR)\tgt.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\j2k.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "LIBJ2K_EXPORTS" /D "_AFXDLL" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\j2k.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\j2k.lib" 
LIB32_OBJS= \
	"$(INTDIR)\bio.obj" \
	"$(INTDIR)\cio.obj" \
	"$(INTDIR)\dwt.obj" \
	"$(INTDIR)\fix.obj" \
	"$(INTDIR)\int.obj" \
	"$(INTDIR)\j2k.obj" \
	"$(INTDIR)\mct.obj" \
	"$(INTDIR)\mqc.obj" \
	"$(INTDIR)\pi.obj" \
	"$(INTDIR)\t1.obj" \
	"$(INTDIR)\t2.obj" \
	"$(INTDIR)\tcd.obj" \
	"$(INTDIR)\tgt.obj"

"$(OUTDIR)\j2k.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "j2k - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\j2k.lib"


CLEAN :
	-@erase "$(INTDIR)\bio.obj"
	-@erase "$(INTDIR)\cio.obj"
	-@erase "$(INTDIR)\dwt.obj"
	-@erase "$(INTDIR)\fix.obj"
	-@erase "$(INTDIR)\int.obj"
	-@erase "$(INTDIR)\j2k.obj"
	-@erase "$(INTDIR)\mct.obj"
	-@erase "$(INTDIR)\mqc.obj"
	-@erase "$(INTDIR)\pi.obj"
	-@erase "$(INTDIR)\t1.obj"
	-@erase "$(INTDIR)\t2.obj"
	-@erase "$(INTDIR)\tcd.obj"
	-@erase "$(INTDIR)\tgt.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\j2k.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /D "LIBJ2K_EXPORTS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\j2k.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\j2k.lib" 
LIB32_OBJS= \
	"$(INTDIR)\bio.obj" \
	"$(INTDIR)\cio.obj" \
	"$(INTDIR)\dwt.obj" \
	"$(INTDIR)\fix.obj" \
	"$(INTDIR)\int.obj" \
	"$(INTDIR)\j2k.obj" \
	"$(INTDIR)\mct.obj" \
	"$(INTDIR)\mqc.obj" \
	"$(INTDIR)\pi.obj" \
	"$(INTDIR)\t1.obj" \
	"$(INTDIR)\t2.obj" \
	"$(INTDIR)\tcd.obj" \
	"$(INTDIR)\tgt.obj"

"$(OUTDIR)\j2k.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("j2k.dep")
!INCLUDE "j2k.dep"
!ELSE 
!MESSAGE Warning: cannot find "j2k.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "j2k - Win32 Release" || "$(CFG)" == "j2k - Win32 Debug"
SOURCE=.\bio.c

"$(INTDIR)\bio.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\cio.c

"$(INTDIR)\cio.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\dwt.c

"$(INTDIR)\dwt.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\fix.c

"$(INTDIR)\fix.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\int.c

"$(INTDIR)\int.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\j2k.c

"$(INTDIR)\j2k.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\mct.c

"$(INTDIR)\mct.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\mqc.c

"$(INTDIR)\mqc.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\pi.c

"$(INTDIR)\pi.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\t1.c

"$(INTDIR)\t1.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\t2.c

"$(INTDIR)\t2.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\tcd.c

"$(INTDIR)\tcd.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\tgt.c

"$(INTDIR)\tgt.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

