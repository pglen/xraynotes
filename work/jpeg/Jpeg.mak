# Microsoft Developer Studio Generated NMAKE File, Based on Jpeg.dsp
!IF "$(CFG)" == ""
CFG=Jpeg - Win32 Debug
!MESSAGE No configuration specified. Defaulting to Jpeg - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "jpeg - Win32 Release" && "$(CFG)" != "jpeg - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Jpeg.mak" CFG="Jpeg - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "jpeg - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "jpeg - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "jpeg - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\Jpeg.lib"


CLEAN :
	-@erase "$(INTDIR)\Jcapimin.obj"
	-@erase "$(INTDIR)\Jcapistd.obj"
	-@erase "$(INTDIR)\Jccoefct.obj"
	-@erase "$(INTDIR)\Jccolor.obj"
	-@erase "$(INTDIR)\Jcdctmgr.obj"
	-@erase "$(INTDIR)\Jchuff.obj"
	-@erase "$(INTDIR)\Jcinit.obj"
	-@erase "$(INTDIR)\Jcmainct.obj"
	-@erase "$(INTDIR)\Jcmarker.obj"
	-@erase "$(INTDIR)\Jcmaster.obj"
	-@erase "$(INTDIR)\Jcomapi.obj"
	-@erase "$(INTDIR)\Jcparam.obj"
	-@erase "$(INTDIR)\Jcphuff.obj"
	-@erase "$(INTDIR)\Jcprepct.obj"
	-@erase "$(INTDIR)\Jcsample.obj"
	-@erase "$(INTDIR)\Jctrans.obj"
	-@erase "$(INTDIR)\Jdapimin.obj"
	-@erase "$(INTDIR)\Jdapistd.obj"
	-@erase "$(INTDIR)\Jdatadst.obj"
	-@erase "$(INTDIR)\Jdatasrc.obj"
	-@erase "$(INTDIR)\Jdcoefct.obj"
	-@erase "$(INTDIR)\Jdcolor.obj"
	-@erase "$(INTDIR)\Jddctmgr.obj"
	-@erase "$(INTDIR)\Jdhuff.obj"
	-@erase "$(INTDIR)\Jdinput.obj"
	-@erase "$(INTDIR)\Jdmainct.obj"
	-@erase "$(INTDIR)\Jdmarker.obj"
	-@erase "$(INTDIR)\Jdmaster.obj"
	-@erase "$(INTDIR)\Jdmerge.obj"
	-@erase "$(INTDIR)\Jdphuff.obj"
	-@erase "$(INTDIR)\Jdpostct.obj"
	-@erase "$(INTDIR)\Jdsample.obj"
	-@erase "$(INTDIR)\Jdtrans.obj"
	-@erase "$(INTDIR)\Jerror.obj"
	-@erase "$(INTDIR)\Jfdctflt.obj"
	-@erase "$(INTDIR)\Jfdctfst.obj"
	-@erase "$(INTDIR)\Jfdctint.obj"
	-@erase "$(INTDIR)\Jidctflt.obj"
	-@erase "$(INTDIR)\Jidctfst.obj"
	-@erase "$(INTDIR)\Jidctint.obj"
	-@erase "$(INTDIR)\Jidctred.obj"
	-@erase "$(INTDIR)\Jmemmgr.obj"
	-@erase "$(INTDIR)\Jmemnobs.obj"
	-@erase "$(INTDIR)\Jquant1.obj"
	-@erase "$(INTDIR)\Jquant2.obj"
	-@erase "$(INTDIR)\Jutils.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\Jpeg.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Jpeg.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Jpeg.lib" 
LIB32_OBJS= \
	"$(INTDIR)\Jcapimin.obj" \
	"$(INTDIR)\Jcapistd.obj" \
	"$(INTDIR)\Jccoefct.obj" \
	"$(INTDIR)\Jccolor.obj" \
	"$(INTDIR)\Jcdctmgr.obj" \
	"$(INTDIR)\Jchuff.obj" \
	"$(INTDIR)\Jcinit.obj" \
	"$(INTDIR)\Jcmainct.obj" \
	"$(INTDIR)\Jcmarker.obj" \
	"$(INTDIR)\Jcmaster.obj" \
	"$(INTDIR)\Jcomapi.obj" \
	"$(INTDIR)\Jcparam.obj" \
	"$(INTDIR)\Jcphuff.obj" \
	"$(INTDIR)\Jcprepct.obj" \
	"$(INTDIR)\Jcsample.obj" \
	"$(INTDIR)\Jctrans.obj" \
	"$(INTDIR)\Jdapimin.obj" \
	"$(INTDIR)\Jdapistd.obj" \
	"$(INTDIR)\Jdatadst.obj" \
	"$(INTDIR)\Jdatasrc.obj" \
	"$(INTDIR)\Jdcoefct.obj" \
	"$(INTDIR)\Jdcolor.obj" \
	"$(INTDIR)\Jddctmgr.obj" \
	"$(INTDIR)\Jdhuff.obj" \
	"$(INTDIR)\Jdinput.obj" \
	"$(INTDIR)\Jdmainct.obj" \
	"$(INTDIR)\Jdmarker.obj" \
	"$(INTDIR)\Jdmaster.obj" \
	"$(INTDIR)\Jdmerge.obj" \
	"$(INTDIR)\Jdphuff.obj" \
	"$(INTDIR)\Jdpostct.obj" \
	"$(INTDIR)\Jdsample.obj" \
	"$(INTDIR)\Jdtrans.obj" \
	"$(INTDIR)\Jerror.obj" \
	"$(INTDIR)\Jfdctflt.obj" \
	"$(INTDIR)\Jfdctfst.obj" \
	"$(INTDIR)\Jfdctint.obj" \
	"$(INTDIR)\Jidctflt.obj" \
	"$(INTDIR)\Jidctfst.obj" \
	"$(INTDIR)\Jidctint.obj" \
	"$(INTDIR)\Jidctred.obj" \
	"$(INTDIR)\Jmemmgr.obj" \
	"$(INTDIR)\Jmemnobs.obj" \
	"$(INTDIR)\Jquant1.obj" \
	"$(INTDIR)\Jquant2.obj" \
	"$(INTDIR)\Jutils.obj"

"$(OUTDIR)\Jpeg.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "jpeg - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\Jpeg.lib"


CLEAN :
	-@erase "$(INTDIR)\Jcapimin.obj"
	-@erase "$(INTDIR)\Jcapistd.obj"
	-@erase "$(INTDIR)\Jccoefct.obj"
	-@erase "$(INTDIR)\Jccolor.obj"
	-@erase "$(INTDIR)\Jcdctmgr.obj"
	-@erase "$(INTDIR)\Jchuff.obj"
	-@erase "$(INTDIR)\Jcinit.obj"
	-@erase "$(INTDIR)\Jcmainct.obj"
	-@erase "$(INTDIR)\Jcmarker.obj"
	-@erase "$(INTDIR)\Jcmaster.obj"
	-@erase "$(INTDIR)\Jcomapi.obj"
	-@erase "$(INTDIR)\Jcparam.obj"
	-@erase "$(INTDIR)\Jcphuff.obj"
	-@erase "$(INTDIR)\Jcprepct.obj"
	-@erase "$(INTDIR)\Jcsample.obj"
	-@erase "$(INTDIR)\Jctrans.obj"
	-@erase "$(INTDIR)\Jdapimin.obj"
	-@erase "$(INTDIR)\Jdapistd.obj"
	-@erase "$(INTDIR)\Jdatadst.obj"
	-@erase "$(INTDIR)\Jdatasrc.obj"
	-@erase "$(INTDIR)\Jdcoefct.obj"
	-@erase "$(INTDIR)\Jdcolor.obj"
	-@erase "$(INTDIR)\Jddctmgr.obj"
	-@erase "$(INTDIR)\Jdhuff.obj"
	-@erase "$(INTDIR)\Jdinput.obj"
	-@erase "$(INTDIR)\Jdmainct.obj"
	-@erase "$(INTDIR)\Jdmarker.obj"
	-@erase "$(INTDIR)\Jdmaster.obj"
	-@erase "$(INTDIR)\Jdmerge.obj"
	-@erase "$(INTDIR)\Jdphuff.obj"
	-@erase "$(INTDIR)\Jdpostct.obj"
	-@erase "$(INTDIR)\Jdsample.obj"
	-@erase "$(INTDIR)\Jdtrans.obj"
	-@erase "$(INTDIR)\Jerror.obj"
	-@erase "$(INTDIR)\Jfdctflt.obj"
	-@erase "$(INTDIR)\Jfdctfst.obj"
	-@erase "$(INTDIR)\Jfdctint.obj"
	-@erase "$(INTDIR)\Jidctflt.obj"
	-@erase "$(INTDIR)\Jidctfst.obj"
	-@erase "$(INTDIR)\Jidctint.obj"
	-@erase "$(INTDIR)\Jidctred.obj"
	-@erase "$(INTDIR)\Jmemmgr.obj"
	-@erase "$(INTDIR)\Jmemnobs.obj"
	-@erase "$(INTDIR)\Jquant1.obj"
	-@erase "$(INTDIR)\Jquant2.obj"
	-@erase "$(INTDIR)\Jutils.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\Jpeg.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Jpeg.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Jpeg.lib" 
LIB32_OBJS= \
	"$(INTDIR)\Jcapimin.obj" \
	"$(INTDIR)\Jcapistd.obj" \
	"$(INTDIR)\Jccoefct.obj" \
	"$(INTDIR)\Jccolor.obj" \
	"$(INTDIR)\Jcdctmgr.obj" \
	"$(INTDIR)\Jchuff.obj" \
	"$(INTDIR)\Jcinit.obj" \
	"$(INTDIR)\Jcmainct.obj" \
	"$(INTDIR)\Jcmarker.obj" \
	"$(INTDIR)\Jcmaster.obj" \
	"$(INTDIR)\Jcomapi.obj" \
	"$(INTDIR)\Jcparam.obj" \
	"$(INTDIR)\Jcphuff.obj" \
	"$(INTDIR)\Jcprepct.obj" \
	"$(INTDIR)\Jcsample.obj" \
	"$(INTDIR)\Jctrans.obj" \
	"$(INTDIR)\Jdapimin.obj" \
	"$(INTDIR)\Jdapistd.obj" \
	"$(INTDIR)\Jdatadst.obj" \
	"$(INTDIR)\Jdatasrc.obj" \
	"$(INTDIR)\Jdcoefct.obj" \
	"$(INTDIR)\Jdcolor.obj" \
	"$(INTDIR)\Jddctmgr.obj" \
	"$(INTDIR)\Jdhuff.obj" \
	"$(INTDIR)\Jdinput.obj" \
	"$(INTDIR)\Jdmainct.obj" \
	"$(INTDIR)\Jdmarker.obj" \
	"$(INTDIR)\Jdmaster.obj" \
	"$(INTDIR)\Jdmerge.obj" \
	"$(INTDIR)\Jdphuff.obj" \
	"$(INTDIR)\Jdpostct.obj" \
	"$(INTDIR)\Jdsample.obj" \
	"$(INTDIR)\Jdtrans.obj" \
	"$(INTDIR)\Jerror.obj" \
	"$(INTDIR)\Jfdctflt.obj" \
	"$(INTDIR)\Jfdctfst.obj" \
	"$(INTDIR)\Jfdctint.obj" \
	"$(INTDIR)\Jidctflt.obj" \
	"$(INTDIR)\Jidctfst.obj" \
	"$(INTDIR)\Jidctint.obj" \
	"$(INTDIR)\Jidctred.obj" \
	"$(INTDIR)\Jmemmgr.obj" \
	"$(INTDIR)\Jmemnobs.obj" \
	"$(INTDIR)\Jquant1.obj" \
	"$(INTDIR)\Jquant2.obj" \
	"$(INTDIR)\Jutils.obj"

"$(OUTDIR)\Jpeg.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("Jpeg.dep")
!INCLUDE "Jpeg.dep"
!ELSE 
!MESSAGE Warning: cannot find "Jpeg.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "jpeg - Win32 Release" || "$(CFG)" == "jpeg - Win32 Debug"
SOURCE=.\Jcapimin.c

"$(INTDIR)\Jcapimin.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Jcapistd.c

"$(INTDIR)\Jcapistd.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Jccoefct.c

"$(INTDIR)\Jccoefct.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Jccolor.c

"$(INTDIR)\Jccolor.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Jcdctmgr.c

"$(INTDIR)\Jcdctmgr.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Jchuff.c

"$(INTDIR)\Jchuff.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Jcinit.c

"$(INTDIR)\Jcinit.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Jcmainct.c

"$(INTDIR)\Jcmainct.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Jcmarker.c

"$(INTDIR)\Jcmarker.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Jcmaster.c

"$(INTDIR)\Jcmaster.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Jcomapi.c

"$(INTDIR)\Jcomapi.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Jcparam.c

"$(INTDIR)\Jcparam.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Jcphuff.c

"$(INTDIR)\Jcphuff.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Jcprepct.c

"$(INTDIR)\Jcprepct.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Jcsample.c

"$(INTDIR)\Jcsample.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Jctrans.c

"$(INTDIR)\Jctrans.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Jdapimin.c

"$(INTDIR)\Jdapimin.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Jdapistd.c

"$(INTDIR)\Jdapistd.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Jdatadst.c

"$(INTDIR)\Jdatadst.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Jdatasrc.c

"$(INTDIR)\Jdatasrc.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Jdcoefct.c

"$(INTDIR)\Jdcoefct.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Jdcolor.c

"$(INTDIR)\Jdcolor.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Jddctmgr.c

"$(INTDIR)\Jddctmgr.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Jdhuff.c

"$(INTDIR)\Jdhuff.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Jdinput.c

"$(INTDIR)\Jdinput.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Jdmainct.c

"$(INTDIR)\Jdmainct.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Jdmarker.c

"$(INTDIR)\Jdmarker.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Jdmaster.c

"$(INTDIR)\Jdmaster.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Jdmerge.c

"$(INTDIR)\Jdmerge.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Jdphuff.c

"$(INTDIR)\Jdphuff.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Jdpostct.c

"$(INTDIR)\Jdpostct.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Jdsample.c

"$(INTDIR)\Jdsample.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Jdtrans.c

"$(INTDIR)\Jdtrans.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Jerror.c

"$(INTDIR)\Jerror.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Jfdctflt.c

"$(INTDIR)\Jfdctflt.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Jfdctfst.c

"$(INTDIR)\Jfdctfst.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Jfdctint.c

"$(INTDIR)\Jfdctint.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Jidctflt.c

"$(INTDIR)\Jidctflt.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Jidctfst.c

"$(INTDIR)\Jidctfst.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Jidctint.c

"$(INTDIR)\Jidctint.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Jidctred.c

"$(INTDIR)\Jidctred.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Jmemmgr.c

"$(INTDIR)\Jmemmgr.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Jmemnobs.c

"$(INTDIR)\Jmemnobs.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Jquant1.c

"$(INTDIR)\Jquant1.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Jquant2.c

"$(INTDIR)\Jquant2.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Jutils.c

"$(INTDIR)\Jutils.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

