# Microsoft Developer Studio Project File - Name="Peek" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Peek - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Peek.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Peek.mak" CFG="Peek - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Peek - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Peek - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Peek - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../Val/DataTypes" /I "../ValWin32/Application" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"msvcrtd.lib"

!ELSEIF  "$(CFG)" == "Peek - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../Val/DataTypes" /I "../ValWin32/Application" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcd.lib" /nodefaultlib:"nafxcwd.lib" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Peek - Win32 Release"
# Name "Peek - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Peek.cpp
# End Source File
# Begin Source File

SOURCE=.\Peek.rc
# End Source File
# Begin Source File

SOURCE=.\PeekAboutDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\PeekCreditsDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\PeekDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\PeekHelpDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Peek.h
# End Source File
# Begin Source File

SOURCE=.\PeekAboutDialog.h
# End Source File
# Begin Source File

SOURCE=.\PeekCreditsDialog.h
# End Source File
# Begin Source File

SOURCE=.\PeekDialog.h
# End Source File
# Begin Source File

SOURCE=.\PeekHelpDialog.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\copyd.bmp
# End Source File
# Begin Source File

SOURCE=.\res\copyf.bmp
# End Source File
# Begin Source File

SOURCE=.\res\copyu.bmp
# End Source File
# Begin Source File

SOURCE=.\res\logod.bmp
# End Source File
# Begin Source File

SOURCE=.\res\logou.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Peek.ico
# End Source File
# Begin Source File

SOURCE=.\res\Peek.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\license.txt

!IF  "$(CFG)" == "Peek - Win32 Release"

# Begin Custom Build - Copying $(InputPath)
OutDir=.\Release
InputPath=.\license.txt

"$(OutDir)/license.txt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy license.txt $(OutDir)

# End Custom Build

!ELSEIF  "$(CFG)" == "Peek - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\readme.txt

!IF  "$(CFG)" == "Peek - Win32 Release"

# Begin Custom Build - Copying $(InputPath)
OutDir=.\Release
InputPath=.\readme.txt

"$(OutDir)/readme.txt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy readme.txt $(OutDir)

# End Custom Build

!ELSEIF  "$(CFG)" == "Peek - Win32 Debug"

!ENDIF 

# End Source File
# End Target
# End Project
