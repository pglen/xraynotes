# Microsoft Developer Studio Generated NMAKE File, Based on jasper.dsp
!IF "$(CFG)" == ""
CFG=jasper - Win32 Debug
!MESSAGE No configuration specified. Defaulting to jasper - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "jasper - Win32 Release" && "$(CFG)" != "jasper - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "jasper.mak" CFG="jasper - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "jasper - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "jasper - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "jasper - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\jasper.lib"


CLEAN :
	-@erase "$(INTDIR)\bmp_cod.obj"
	-@erase "$(INTDIR)\bmp_dec.obj"
	-@erase "$(INTDIR)\bmp_enc.obj"
	-@erase "$(INTDIR)\jas_cm.obj"
	-@erase "$(INTDIR)\jas_debug.obj"
	-@erase "$(INTDIR)\jas_getopt.obj"
	-@erase "$(INTDIR)\jas_icc.obj"
	-@erase "$(INTDIR)\jas_iccdata.obj"
	-@erase "$(INTDIR)\jas_image.obj"
	-@erase "$(INTDIR)\jas_init.obj"
	-@erase "$(INTDIR)\jas_malloc.obj"
	-@erase "$(INTDIR)\jas_seq.obj"
	-@erase "$(INTDIR)\jas_stream.obj"
	-@erase "$(INTDIR)\jas_string.obj"
	-@erase "$(INTDIR)\jas_tvp.obj"
	-@erase "$(INTDIR)\jas_version.obj"
	-@erase "$(INTDIR)\jp2_cod.obj"
	-@erase "$(INTDIR)\jp2_dec.obj"
	-@erase "$(INTDIR)\jp2_enc.obj"
	-@erase "$(INTDIR)\jpc_bs.obj"
	-@erase "$(INTDIR)\jpc_cs.obj"
	-@erase "$(INTDIR)\jpc_dec.obj"
	-@erase "$(INTDIR)\jpc_enc.obj"
	-@erase "$(INTDIR)\jpc_math.obj"
	-@erase "$(INTDIR)\jpc_mct.obj"
	-@erase "$(INTDIR)\jpc_mqcod.obj"
	-@erase "$(INTDIR)\jpc_mqdec.obj"
	-@erase "$(INTDIR)\jpc_mqenc.obj"
	-@erase "$(INTDIR)\jpc_qmfb.obj"
	-@erase "$(INTDIR)\jpc_t1cod.obj"
	-@erase "$(INTDIR)\jpc_t1dec.obj"
	-@erase "$(INTDIR)\jpc_t1enc.obj"
	-@erase "$(INTDIR)\jpc_t2cod.obj"
	-@erase "$(INTDIR)\jpc_t2dec.obj"
	-@erase "$(INTDIR)\jpc_t2enc.obj"
	-@erase "$(INTDIR)\jpc_tagtree.obj"
	-@erase "$(INTDIR)\jpc_tsfb.obj"
	-@erase "$(INTDIR)\jpc_util.obj"
	-@erase "$(INTDIR)\jpg_dummy.obj"
	-@erase "$(INTDIR)\jpg_val.obj"
	-@erase "$(INTDIR)\mif_cod.obj"
	-@erase "$(INTDIR)\pgx_cod.obj"
	-@erase "$(INTDIR)\pgx_dec.obj"
	-@erase "$(INTDIR)\pgx_enc.obj"
	-@erase "$(INTDIR)\pnm_cod.obj"
	-@erase "$(INTDIR)\pnm_dec.obj"
	-@erase "$(INTDIR)\pnm_enc.obj"
	-@erase "$(INTDIR)\ras_cod.obj"
	-@erase "$(INTDIR)\ras_dec.obj"
	-@erase "$(INTDIR)\ras_enc.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\jasper.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W2 /GX /O2 /I ".\include" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "JAS_WIN_MSVC_BUILD" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\jasper.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\jasper.lib" 
LIB32_OBJS= \
	"$(INTDIR)\bmp_cod.obj" \
	"$(INTDIR)\bmp_dec.obj" \
	"$(INTDIR)\bmp_enc.obj" \
	"$(INTDIR)\jas_cm.obj" \
	"$(INTDIR)\jas_debug.obj" \
	"$(INTDIR)\jas_getopt.obj" \
	"$(INTDIR)\jas_icc.obj" \
	"$(INTDIR)\jas_iccdata.obj" \
	"$(INTDIR)\jas_image.obj" \
	"$(INTDIR)\jas_init.obj" \
	"$(INTDIR)\jas_malloc.obj" \
	"$(INTDIR)\jas_seq.obj" \
	"$(INTDIR)\jas_stream.obj" \
	"$(INTDIR)\jas_string.obj" \
	"$(INTDIR)\jas_tvp.obj" \
	"$(INTDIR)\jas_version.obj" \
	"$(INTDIR)\jp2_cod.obj" \
	"$(INTDIR)\jp2_dec.obj" \
	"$(INTDIR)\jp2_enc.obj" \
	"$(INTDIR)\jpc_bs.obj" \
	"$(INTDIR)\jpc_cs.obj" \
	"$(INTDIR)\jpc_dec.obj" \
	"$(INTDIR)\jpc_enc.obj" \
	"$(INTDIR)\jpc_math.obj" \
	"$(INTDIR)\jpc_mct.obj" \
	"$(INTDIR)\jpc_mqcod.obj" \
	"$(INTDIR)\jpc_mqdec.obj" \
	"$(INTDIR)\jpc_mqenc.obj" \
	"$(INTDIR)\jpc_qmfb.obj" \
	"$(INTDIR)\jpc_t1cod.obj" \
	"$(INTDIR)\jpc_t1dec.obj" \
	"$(INTDIR)\jpc_t1enc.obj" \
	"$(INTDIR)\jpc_t2cod.obj" \
	"$(INTDIR)\jpc_t2dec.obj" \
	"$(INTDIR)\jpc_t2enc.obj" \
	"$(INTDIR)\jpc_tagtree.obj" \
	"$(INTDIR)\jpc_tsfb.obj" \
	"$(INTDIR)\jpc_util.obj" \
	"$(INTDIR)\jpg_dummy.obj" \
	"$(INTDIR)\jpg_val.obj" \
	"$(INTDIR)\mif_cod.obj" \
	"$(INTDIR)\pgx_cod.obj" \
	"$(INTDIR)\pgx_dec.obj" \
	"$(INTDIR)\pgx_enc.obj" \
	"$(INTDIR)\pnm_cod.obj" \
	"$(INTDIR)\pnm_dec.obj" \
	"$(INTDIR)\pnm_enc.obj" \
	"$(INTDIR)\ras_cod.obj" \
	"$(INTDIR)\ras_dec.obj" \
	"$(INTDIR)\ras_enc.obj"

"$(OUTDIR)\jasper.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "jasper - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\jasper.lib"


CLEAN :
	-@erase "$(INTDIR)\bmp_cod.obj"
	-@erase "$(INTDIR)\bmp_dec.obj"
	-@erase "$(INTDIR)\bmp_enc.obj"
	-@erase "$(INTDIR)\jas_cm.obj"
	-@erase "$(INTDIR)\jas_debug.obj"
	-@erase "$(INTDIR)\jas_getopt.obj"
	-@erase "$(INTDIR)\jas_icc.obj"
	-@erase "$(INTDIR)\jas_iccdata.obj"
	-@erase "$(INTDIR)\jas_image.obj"
	-@erase "$(INTDIR)\jas_init.obj"
	-@erase "$(INTDIR)\jas_malloc.obj"
	-@erase "$(INTDIR)\jas_seq.obj"
	-@erase "$(INTDIR)\jas_stream.obj"
	-@erase "$(INTDIR)\jas_string.obj"
	-@erase "$(INTDIR)\jas_tvp.obj"
	-@erase "$(INTDIR)\jas_version.obj"
	-@erase "$(INTDIR)\jp2_cod.obj"
	-@erase "$(INTDIR)\jp2_dec.obj"
	-@erase "$(INTDIR)\jp2_enc.obj"
	-@erase "$(INTDIR)\jpc_bs.obj"
	-@erase "$(INTDIR)\jpc_cs.obj"
	-@erase "$(INTDIR)\jpc_dec.obj"
	-@erase "$(INTDIR)\jpc_enc.obj"
	-@erase "$(INTDIR)\jpc_math.obj"
	-@erase "$(INTDIR)\jpc_mct.obj"
	-@erase "$(INTDIR)\jpc_mqcod.obj"
	-@erase "$(INTDIR)\jpc_mqdec.obj"
	-@erase "$(INTDIR)\jpc_mqenc.obj"
	-@erase "$(INTDIR)\jpc_qmfb.obj"
	-@erase "$(INTDIR)\jpc_t1cod.obj"
	-@erase "$(INTDIR)\jpc_t1dec.obj"
	-@erase "$(INTDIR)\jpc_t1enc.obj"
	-@erase "$(INTDIR)\jpc_t2cod.obj"
	-@erase "$(INTDIR)\jpc_t2dec.obj"
	-@erase "$(INTDIR)\jpc_t2enc.obj"
	-@erase "$(INTDIR)\jpc_tagtree.obj"
	-@erase "$(INTDIR)\jpc_tsfb.obj"
	-@erase "$(INTDIR)\jpc_util.obj"
	-@erase "$(INTDIR)\jpg_dummy.obj"
	-@erase "$(INTDIR)\jpg_val.obj"
	-@erase "$(INTDIR)\mif_cod.obj"
	-@erase "$(INTDIR)\pgx_cod.obj"
	-@erase "$(INTDIR)\pgx_dec.obj"
	-@erase "$(INTDIR)\pgx_enc.obj"
	-@erase "$(INTDIR)\pnm_cod.obj"
	-@erase "$(INTDIR)\pnm_dec.obj"
	-@erase "$(INTDIR)\pnm_enc.obj"
	-@erase "$(INTDIR)\ras_cod.obj"
	-@erase "$(INTDIR)\ras_dec.obj"
	-@erase "$(INTDIR)\ras_enc.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\jasper.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W2 /Gm /GX /ZI /Od /I ".\include" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "JAS_WIN_MSVC_BUILD" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\jasper.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\jasper.lib" 
LIB32_OBJS= \
	"$(INTDIR)\bmp_cod.obj" \
	"$(INTDIR)\bmp_dec.obj" \
	"$(INTDIR)\bmp_enc.obj" \
	"$(INTDIR)\jas_cm.obj" \
	"$(INTDIR)\jas_debug.obj" \
	"$(INTDIR)\jas_getopt.obj" \
	"$(INTDIR)\jas_icc.obj" \
	"$(INTDIR)\jas_iccdata.obj" \
	"$(INTDIR)\jas_image.obj" \
	"$(INTDIR)\jas_init.obj" \
	"$(INTDIR)\jas_malloc.obj" \
	"$(INTDIR)\jas_seq.obj" \
	"$(INTDIR)\jas_stream.obj" \
	"$(INTDIR)\jas_string.obj" \
	"$(INTDIR)\jas_tvp.obj" \
	"$(INTDIR)\jas_version.obj" \
	"$(INTDIR)\jp2_cod.obj" \
	"$(INTDIR)\jp2_dec.obj" \
	"$(INTDIR)\jp2_enc.obj" \
	"$(INTDIR)\jpc_bs.obj" \
	"$(INTDIR)\jpc_cs.obj" \
	"$(INTDIR)\jpc_dec.obj" \
	"$(INTDIR)\jpc_enc.obj" \
	"$(INTDIR)\jpc_math.obj" \
	"$(INTDIR)\jpc_mct.obj" \
	"$(INTDIR)\jpc_mqcod.obj" \
	"$(INTDIR)\jpc_mqdec.obj" \
	"$(INTDIR)\jpc_mqenc.obj" \
	"$(INTDIR)\jpc_qmfb.obj" \
	"$(INTDIR)\jpc_t1cod.obj" \
	"$(INTDIR)\jpc_t1dec.obj" \
	"$(INTDIR)\jpc_t1enc.obj" \
	"$(INTDIR)\jpc_t2cod.obj" \
	"$(INTDIR)\jpc_t2dec.obj" \
	"$(INTDIR)\jpc_t2enc.obj" \
	"$(INTDIR)\jpc_tagtree.obj" \
	"$(INTDIR)\jpc_tsfb.obj" \
	"$(INTDIR)\jpc_util.obj" \
	"$(INTDIR)\jpg_dummy.obj" \
	"$(INTDIR)\jpg_val.obj" \
	"$(INTDIR)\mif_cod.obj" \
	"$(INTDIR)\pgx_cod.obj" \
	"$(INTDIR)\pgx_dec.obj" \
	"$(INTDIR)\pgx_enc.obj" \
	"$(INTDIR)\pnm_cod.obj" \
	"$(INTDIR)\pnm_dec.obj" \
	"$(INTDIR)\pnm_enc.obj" \
	"$(INTDIR)\ras_cod.obj" \
	"$(INTDIR)\ras_dec.obj" \
	"$(INTDIR)\ras_enc.obj"

"$(OUTDIR)\jasper.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("jasper.dep")
!INCLUDE "jasper.dep"
!ELSE 
!MESSAGE Warning: cannot find "jasper.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "jasper - Win32 Release" || "$(CFG)" == "jasper - Win32 Debug"
SOURCE=.\bmp\bmp_cod.c

"$(INTDIR)\bmp_cod.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\bmp\bmp_dec.c

"$(INTDIR)\bmp_dec.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\bmp\bmp_enc.c

"$(INTDIR)\bmp_enc.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\base\jas_cm.c

"$(INTDIR)\jas_cm.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\base\jas_debug.c

"$(INTDIR)\jas_debug.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\base\jas_getopt.c

"$(INTDIR)\jas_getopt.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\base\jas_icc.c

"$(INTDIR)\jas_icc.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\base\jas_iccdata.c

"$(INTDIR)\jas_iccdata.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\base\jas_image.c

"$(INTDIR)\jas_image.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\base\jas_init.c

"$(INTDIR)\jas_init.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\base\jas_malloc.c

"$(INTDIR)\jas_malloc.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\base\jas_seq.c

"$(INTDIR)\jas_seq.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\base\jas_stream.c

"$(INTDIR)\jas_stream.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\base\jas_string.c

"$(INTDIR)\jas_string.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\base\jas_tvp.c

"$(INTDIR)\jas_tvp.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\base\jas_version.c

"$(INTDIR)\jas_version.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\jp2\jp2_cod.c

"$(INTDIR)\jp2_cod.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\jp2\jp2_dec.c

"$(INTDIR)\jp2_dec.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\jp2\jp2_enc.c

"$(INTDIR)\jp2_enc.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\jpc\jpc_bs.c

"$(INTDIR)\jpc_bs.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\jpc\jpc_cs.c

"$(INTDIR)\jpc_cs.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\jpc\jpc_dec.c

"$(INTDIR)\jpc_dec.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\jpc\jpc_enc.c

"$(INTDIR)\jpc_enc.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\jpc\jpc_math.c

"$(INTDIR)\jpc_math.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\jpc\jpc_mct.c

"$(INTDIR)\jpc_mct.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\jpc\jpc_mqcod.c

"$(INTDIR)\jpc_mqcod.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\jpc\jpc_mqdec.c

"$(INTDIR)\jpc_mqdec.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\jpc\jpc_mqenc.c

"$(INTDIR)\jpc_mqenc.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\jpc\jpc_qmfb.c

"$(INTDIR)\jpc_qmfb.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\jpc\jpc_t1cod.c

"$(INTDIR)\jpc_t1cod.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\jpc\jpc_t1dec.c

"$(INTDIR)\jpc_t1dec.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\jpc\jpc_t1enc.c

"$(INTDIR)\jpc_t1enc.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\jpc\jpc_t2cod.c

"$(INTDIR)\jpc_t2cod.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\jpc\jpc_t2dec.c

"$(INTDIR)\jpc_t2dec.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\jpc\jpc_t2enc.c

"$(INTDIR)\jpc_t2enc.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\jpc\jpc_tagtree.c

"$(INTDIR)\jpc_tagtree.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\jpc\jpc_tsfb.c

"$(INTDIR)\jpc_tsfb.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\jpc\jpc_util.c

"$(INTDIR)\jpc_util.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\jpg\jpg_dummy.c

"$(INTDIR)\jpg_dummy.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\jpg\jpg_val.c

"$(INTDIR)\jpg_val.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\mif\mif_cod.c

"$(INTDIR)\mif_cod.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\pgx\pgx_cod.c

"$(INTDIR)\pgx_cod.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\pgx\pgx_dec.c

"$(INTDIR)\pgx_dec.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\pgx\pgx_enc.c

"$(INTDIR)\pgx_enc.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\pnm\pnm_cod.c

"$(INTDIR)\pnm_cod.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\pnm\pnm_dec.c

"$(INTDIR)\pnm_dec.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\pnm\pnm_enc.c

"$(INTDIR)\pnm_enc.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\ras\ras_cod.c

"$(INTDIR)\ras_cod.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\ras\ras_dec.c

"$(INTDIR)\ras_dec.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\ras\ras_enc.c

"$(INTDIR)\ras_enc.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

