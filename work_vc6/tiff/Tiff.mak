# Microsoft Developer Studio Generated NMAKE File, Based on Tiff.dsp
!IF "$(CFG)" == ""
CFG=Tiff - Win32 Debug
!MESSAGE No configuration specified. Defaulting to Tiff - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "tiff - Win32 Release" && "$(CFG)" != "tiff - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Tiff.mak" CFG="Tiff - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "tiff - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "tiff - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "tiff - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\Tiff.lib"


CLEAN :
	-@erase "$(INTDIR)\fax3sm_winnt.obj"
	-@erase "$(INTDIR)\tif_aux.obj"
	-@erase "$(INTDIR)\tif_close.obj"
	-@erase "$(INTDIR)\tif_codec.obj"
	-@erase "$(INTDIR)\tif_compress.obj"
	-@erase "$(INTDIR)\tif_dir.obj"
	-@erase "$(INTDIR)\tif_dirinfo.obj"
	-@erase "$(INTDIR)\tif_dirread.obj"
	-@erase "$(INTDIR)\tif_dirwrite.obj"
	-@erase "$(INTDIR)\tif_dumpmode.obj"
	-@erase "$(INTDIR)\tif_error.obj"
	-@erase "$(INTDIR)\tif_fax3.obj"
	-@erase "$(INTDIR)\tif_flush.obj"
	-@erase "$(INTDIR)\tif_getimage.obj"
	-@erase "$(INTDIR)\tif_jpeg.obj"
	-@erase "$(INTDIR)\tif_luv.obj"
	-@erase "$(INTDIR)\tif_lzw.obj"
	-@erase "$(INTDIR)\tif_next.obj"
	-@erase "$(INTDIR)\tif_ojpeg.obj"
	-@erase "$(INTDIR)\tif_open.obj"
	-@erase "$(INTDIR)\tif_packbits.obj"
	-@erase "$(INTDIR)\tif_pixarlog.obj"
	-@erase "$(INTDIR)\tif_predict.obj"
	-@erase "$(INTDIR)\tif_print.obj"
	-@erase "$(INTDIR)\tif_read.obj"
	-@erase "$(INTDIR)\tif_strip.obj"
	-@erase "$(INTDIR)\tif_swab.obj"
	-@erase "$(INTDIR)\tif_thunder.obj"
	-@erase "$(INTDIR)\tif_tile.obj"
	-@erase "$(INTDIR)\tif_version.obj"
	-@erase "$(INTDIR)\tif_warning.obj"
	-@erase "$(INTDIR)\tif_write.obj"
	-@erase "$(INTDIR)\tif_zip.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\Tiff.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O1 /I "..\zlib" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_AFXDLL" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Tiff.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Tiff.lib" 
LIB32_OBJS= \
	"$(INTDIR)\fax3sm_winnt.obj" \
	"$(INTDIR)\tif_aux.obj" \
	"$(INTDIR)\tif_close.obj" \
	"$(INTDIR)\tif_codec.obj" \
	"$(INTDIR)\tif_compress.obj" \
	"$(INTDIR)\tif_dir.obj" \
	"$(INTDIR)\tif_dirinfo.obj" \
	"$(INTDIR)\tif_dirread.obj" \
	"$(INTDIR)\tif_dirwrite.obj" \
	"$(INTDIR)\tif_dumpmode.obj" \
	"$(INTDIR)\tif_error.obj" \
	"$(INTDIR)\tif_fax3.obj" \
	"$(INTDIR)\tif_flush.obj" \
	"$(INTDIR)\tif_getimage.obj" \
	"$(INTDIR)\tif_jpeg.obj" \
	"$(INTDIR)\tif_luv.obj" \
	"$(INTDIR)\tif_lzw.obj" \
	"$(INTDIR)\tif_next.obj" \
	"$(INTDIR)\tif_ojpeg.obj" \
	"$(INTDIR)\tif_open.obj" \
	"$(INTDIR)\tif_packbits.obj" \
	"$(INTDIR)\tif_pixarlog.obj" \
	"$(INTDIR)\tif_predict.obj" \
	"$(INTDIR)\tif_print.obj" \
	"$(INTDIR)\tif_read.obj" \
	"$(INTDIR)\tif_strip.obj" \
	"$(INTDIR)\tif_swab.obj" \
	"$(INTDIR)\tif_thunder.obj" \
	"$(INTDIR)\tif_tile.obj" \
	"$(INTDIR)\tif_version.obj" \
	"$(INTDIR)\tif_warning.obj" \
	"$(INTDIR)\tif_write.obj" \
	"$(INTDIR)\tif_zip.obj"

"$(OUTDIR)\Tiff.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "tiff - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\Tiff.lib"


CLEAN :
	-@erase "$(INTDIR)\fax3sm_winnt.obj"
	-@erase "$(INTDIR)\tif_aux.obj"
	-@erase "$(INTDIR)\tif_close.obj"
	-@erase "$(INTDIR)\tif_codec.obj"
	-@erase "$(INTDIR)\tif_compress.obj"
	-@erase "$(INTDIR)\tif_dir.obj"
	-@erase "$(INTDIR)\tif_dirinfo.obj"
	-@erase "$(INTDIR)\tif_dirread.obj"
	-@erase "$(INTDIR)\tif_dirwrite.obj"
	-@erase "$(INTDIR)\tif_dumpmode.obj"
	-@erase "$(INTDIR)\tif_error.obj"
	-@erase "$(INTDIR)\tif_fax3.obj"
	-@erase "$(INTDIR)\tif_flush.obj"
	-@erase "$(INTDIR)\tif_getimage.obj"
	-@erase "$(INTDIR)\tif_jpeg.obj"
	-@erase "$(INTDIR)\tif_luv.obj"
	-@erase "$(INTDIR)\tif_lzw.obj"
	-@erase "$(INTDIR)\tif_next.obj"
	-@erase "$(INTDIR)\tif_ojpeg.obj"
	-@erase "$(INTDIR)\tif_open.obj"
	-@erase "$(INTDIR)\tif_packbits.obj"
	-@erase "$(INTDIR)\tif_pixarlog.obj"
	-@erase "$(INTDIR)\tif_predict.obj"
	-@erase "$(INTDIR)\tif_print.obj"
	-@erase "$(INTDIR)\tif_read.obj"
	-@erase "$(INTDIR)\tif_strip.obj"
	-@erase "$(INTDIR)\tif_swab.obj"
	-@erase "$(INTDIR)\tif_thunder.obj"
	-@erase "$(INTDIR)\tif_tile.obj"
	-@erase "$(INTDIR)\tif_version.obj"
	-@erase "$(INTDIR)\tif_warning.obj"
	-@erase "$(INTDIR)\tif_write.obj"
	-@erase "$(INTDIR)\tif_zip.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\Tiff.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\zlib" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_AFXDLL" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Tiff.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Tiff.lib" 
LIB32_OBJS= \
	"$(INTDIR)\fax3sm_winnt.obj" \
	"$(INTDIR)\tif_aux.obj" \
	"$(INTDIR)\tif_close.obj" \
	"$(INTDIR)\tif_codec.obj" \
	"$(INTDIR)\tif_compress.obj" \
	"$(INTDIR)\tif_dir.obj" \
	"$(INTDIR)\tif_dirinfo.obj" \
	"$(INTDIR)\tif_dirread.obj" \
	"$(INTDIR)\tif_dirwrite.obj" \
	"$(INTDIR)\tif_dumpmode.obj" \
	"$(INTDIR)\tif_error.obj" \
	"$(INTDIR)\tif_fax3.obj" \
	"$(INTDIR)\tif_flush.obj" \
	"$(INTDIR)\tif_getimage.obj" \
	"$(INTDIR)\tif_jpeg.obj" \
	"$(INTDIR)\tif_luv.obj" \
	"$(INTDIR)\tif_lzw.obj" \
	"$(INTDIR)\tif_next.obj" \
	"$(INTDIR)\tif_ojpeg.obj" \
	"$(INTDIR)\tif_open.obj" \
	"$(INTDIR)\tif_packbits.obj" \
	"$(INTDIR)\tif_pixarlog.obj" \
	"$(INTDIR)\tif_predict.obj" \
	"$(INTDIR)\tif_print.obj" \
	"$(INTDIR)\tif_read.obj" \
	"$(INTDIR)\tif_strip.obj" \
	"$(INTDIR)\tif_swab.obj" \
	"$(INTDIR)\tif_thunder.obj" \
	"$(INTDIR)\tif_tile.obj" \
	"$(INTDIR)\tif_version.obj" \
	"$(INTDIR)\tif_warning.obj" \
	"$(INTDIR)\tif_write.obj" \
	"$(INTDIR)\tif_zip.obj"

"$(OUTDIR)\Tiff.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("Tiff.dep")
!INCLUDE "Tiff.dep"
!ELSE 
!MESSAGE Warning: cannot find "Tiff.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "tiff - Win32 Release" || "$(CFG)" == "tiff - Win32 Debug"
SOURCE=.\fax3sm_winnt.c

"$(INTDIR)\fax3sm_winnt.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\tif_aux.c

"$(INTDIR)\tif_aux.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\tif_close.c

"$(INTDIR)\tif_close.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\tif_codec.c

"$(INTDIR)\tif_codec.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\tif_compress.c

"$(INTDIR)\tif_compress.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\tif_dir.c

"$(INTDIR)\tif_dir.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\tif_dirinfo.c

"$(INTDIR)\tif_dirinfo.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\tif_dirread.c

"$(INTDIR)\tif_dirread.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\tif_dirwrite.c

"$(INTDIR)\tif_dirwrite.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\tif_dumpmode.c

"$(INTDIR)\tif_dumpmode.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\tif_error.c

"$(INTDIR)\tif_error.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\tif_fax3.c

"$(INTDIR)\tif_fax3.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\tif_flush.c

"$(INTDIR)\tif_flush.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\tif_getimage.c

"$(INTDIR)\tif_getimage.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\tif_jpeg.c

"$(INTDIR)\tif_jpeg.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\tif_luv.c

"$(INTDIR)\tif_luv.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\tif_lzw.c

"$(INTDIR)\tif_lzw.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\tif_next.c

"$(INTDIR)\tif_next.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\tif_ojpeg.c

"$(INTDIR)\tif_ojpeg.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\tif_open.c

"$(INTDIR)\tif_open.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\tif_packbits.c

"$(INTDIR)\tif_packbits.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\tif_pixarlog.c

"$(INTDIR)\tif_pixarlog.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\tif_predict.c

"$(INTDIR)\tif_predict.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\tif_print.c

"$(INTDIR)\tif_print.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\tif_read.c

"$(INTDIR)\tif_read.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\tif_strip.c

"$(INTDIR)\tif_strip.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\tif_swab.c

"$(INTDIR)\tif_swab.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\tif_thunder.c

"$(INTDIR)\tif_thunder.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\tif_tile.c

"$(INTDIR)\tif_tile.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\tif_version.c

"$(INTDIR)\tif_version.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\tif_warning.c

"$(INTDIR)\tif_warning.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\tif_write.c

"$(INTDIR)\tif_write.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\tif_zip.c

"$(INTDIR)\tif_zip.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

