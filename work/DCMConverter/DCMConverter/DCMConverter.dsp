# Microsoft Developer Studio Project File - Name="DCMConverter" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=DCMConverter - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DCMConverter.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DCMConverter.mak" CFG="DCMConverter - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DCMConverter - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "DCMConverter - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DCMConverter - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "D:\cximage599c_full\CxImage" /I "D:\dcmtk\dcmtk-3.5.3\config\include" /I "D:\dcmtk\dcmtk-3.5.3\dcmjpeg\include" /I "D:\dcmtk\dcmtk-3.5.3\ofstd\include" /I "D:\dcmtk\dcmtk-3.5.3\dcmdata\include" /I "D:\dcmtk\dcmtk-3.5.3\dcmimgle\include" /I "D:\dcmtk\dcmtk-3.5.3\dcmimage\include" /I "D:\dcmtk\dcmtk-3.5.3\dcmjpeg\libijg8" /I "D:\dcmtk\dcmtk-3.5.3\dcmjpeg\libijg12" /I "D:\dcmtk\dcmtk-3.5.3\dcmjpeg\libijg16" /I "D:\dcmtk\zlib-1.2.1" /I "D:\dcmtk\tiff-v3.6.1\tifflib" /I "D:\dcmtk\libpng-1.2.5" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /D "WITH_LIBPNG" /D "WITH_LIBTIFF" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 cximage.lib netapi32.lib wsock32.lib ofstd.lib zlib.lib libtiff.lib libpng.lib dcmdata.lib dcmimgle.lib dcmimage.lib dcmjpeg.lib ijg8.lib ijg12.lib ijg16.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"msvcprt.lib" /nodefaultlib:"msvcrt.lib" /libpath:"D:\cximage599c_full\CxImage\CxImageDLL\Release" /libpath:"D:\dcmtk\dcmtk-3.5.3\Debug" /libpath:"D:\dcmtk\config\include" /libpath:"D:\dcmtk\dcmjpeg\include" /libpath:"D:\dcmtk\ofstd\include" /libpath:"D:\dcmtk\dcmdata\include" /libpath:"D:\dcmtk\dcmimgle\include" /libpath:"D:\dcmtk\dcmimage\include" /libpath:"D:\dcmtk\dcmjpeg\libijg8" /libpath:"D:\dcmtk\dcmjpeg\libijg12" /libpath:"D:\dcmtk\dcmjpeg\libijg16" /libpath:"D:\dcmtk\zlib-1.2.1" /libpath:"D:\dcmtk\tiff-v3.6.1\libtiff" /libpath:"D:\dcmtk\libpng-1.2.5"

!ELSEIF  "$(CFG)" == "DCMConverter - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "D:\cximage599c_full\CxImage" /I "D:\dcmtk\dcmtk-3.5.3\config\include" /I "D:\dcmtk\dcmtk-3.5.3\dcmjpeg\include" /I "D:\dcmtk\dcmtk-3.5.3\ofstd\include" /I "D:\dcmtk\dcmtk-3.5.3\dcmdata\include" /I "D:\dcmtk\dcmtk-3.5.3\dcmimgle\include" /I "D:\dcmtk\dcmtk-3.5.3\dcmimage\include" /I "D:\dcmtk\dcmtk-3.5.3\dcmjpeg\libijg8" /I "D:\dcmtk\dcmtk-3.5.3\dcmjpeg\libijg12" /I "D:\dcmtk\dcmtk-3.5.3\dcmjpeg\libijg16" /I "D:\dcmtk\zlib-1.2.1" /I "D:\dcmtk\tiff-v3.6.1\tifflib" /I "D:\dcmtk\libpng-1.2.5" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "WITH_LIBPNG" /D "WITH_LIBTIFF" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 cximaged.lib netapi32.lib wsock32.lib ofstd.lib zlib.lib libtiff.lib libpng.lib dcmdata.lib dcmimgle.lib dcmimage.lib dcmjpeg.lib ijg8.lib ijg12.lib ijg16.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"msvcprtd.lib" /nodefaultlib:"msvcrtd.lib" /nodefaultlib:"msvcrt.lib" /pdbtype:sept /libpath:"D:\cximage599c_full\CxImage\CxImageDLL\Debug" /libpath:"D:\dcmtk\dcmtk-3.5.3\Debug" /libpath:"D:\dcmtk\config\include" /libpath:"D:\dcmtk\dcmjpeg\include" /libpath:"D:\dcmtk\ofstd\include" /libpath:"D:\dcmtk\dcmdata\include" /libpath:"D:\dcmtk\dcmimgle\include" /libpath:"D:\dcmtk\dcmimage\include" /libpath:"D:\dcmtk\dcmjpeg\libijg8" /libpath:"D:\dcmtk\dcmjpeg\libijg12" /libpath:"D:\dcmtk\dcmjpeg\libijg16" /libpath:"D:\dcmtk\zlib-1.2.1" /libpath:"D:\dcmtk\tiff-v3.6.1\libtiff" /libpath:"D:\dcmtk\libpng-1.2.5"

!ENDIF 

# Begin Target

# Name "DCMConverter - Win32 Release"
# Name "DCMConverter - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\DCMConverter.cpp
# End Source File
# Begin Source File

SOURCE=.\DCMConverter.rc
# End Source File
# Begin Source File

SOURCE=.\DCMConverterDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\xImageDCM.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\DCMConverter.h
# End Source File
# Begin Source File

SOURCE=.\DCMConverterDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\xImageDCM.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\DCMConverter.ico
# End Source File
# Begin Source File

SOURCE=.\res\DCMConverter.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
