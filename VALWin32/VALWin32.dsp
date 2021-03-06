# Microsoft Developer Studio Project File - Name="VALWin32" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=VALWin32 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "VALWin32.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "VALWin32.mak" CFG="VALWin32 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "VALWin32 - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "VALWin32 - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "VALWin32"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "VALWin32 - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "Application" /I "Render" /I "..\VAL\Application" /I "..\VAL\Avatar" /I "..\VAL\AvatarFiles" /I "..\VAL\DataTypes" /I "..\VAL\Image" /I "..\VAL\Wrapper" /I "..\VAL\ImageFiles" /I "..\VAL\Render" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_LIB" /D "_MBCS" /D "VAL_BUILD" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x809 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x809 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "VALWin32 - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "Application" /I "Render" /I "..\VAL\Application" /I "..\VAL\Avatar" /I "..\VAL\AvatarFiles" /I "..\VAL\DataTypes" /I "..\VAL\Image" /I "..\VAL\Wrapper" /I "..\VAL\ImageFiles" /I "..\VAL\Render" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_LIB" /D "_AFXDLL" /D "_MBCS" /D "VAL_BUILD" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x809 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x809 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "VALWin32 - Win32 Release"
# Name "VALWin32 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Application\FolderDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\NumericEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\Render\RCOpenGLBufferWin32.cpp
# End Source File
# Begin Source File

SOURCE=.\Render\RCOpenGLWin32.cpp

!IF  "$(CFG)" == "VALWin32 - Win32 Release"

!ELSEIF  "$(CFG)" == "VALWin32 - Win32 Debug"

# SUBTRACT CPP /FA<none>

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Application\Registry.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\StaticLink.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Application\VALWin32.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Application\FolderDialog.h
# End Source File
# Begin Source File

SOURCE=.\Application\NumericEdit.h
# End Source File
# Begin Source File

SOURCE=.\Render\RCOpenGLBufferWin32.h
# End Source File
# Begin Source File

SOURCE=.\Render\RCOpenGLWin32.h
# End Source File
# Begin Source File

SOURCE=.\Application\Registry.h
# End Source File
# Begin Source File

SOURCE=.\Application\StaticLink.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Application\VALWin32.h
# End Source File
# End Group
# End Target
# End Project
