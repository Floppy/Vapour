# Microsoft Developer Studio Project File - Name="WJESFX" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=WJESFX - Win32 Sims Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "WJESFX.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "WJESFX.mak" CFG="WJESFX - Win32 Sims Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "WJESFX - Win32 Sims Release" (based on "Win32 (x86) Application")
!MESSAGE "WJESFX - Win32 Half Life Release" (based on "Win32 (x86) Application")
!MESSAGE "WJESFX - Win32 Unreal Tournament Release" (based on "Win32 (x86) Application")
!MESSAGE "WJESFX - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "WJESFX - Win32 Sims Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WJESFX___Win32_Sims_Release"
# PROP BASE Intermediate_Dir "WJESFX___Win32_Sims_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Sims"
# PROP Intermediate_Dir "Sims"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "..\VAL\Application" /I "..\VAL\Avatar" /I "..\VAL\AvatarFiles" /I "..\VAL\DataTypes" /I "..\VAL\Image" /I "..\VAL\ImageFiles" /I "..\VALWin32\Application" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\VAL\Application" /I "..\VAL\Avatar" /I "..\VAL\AvatarFiles" /I "..\VAL\DataTypes" /I "..\VAL\Image" /I "..\VAL\ImageFiles" /I "..\VALWin32\Application" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "GAME_SIMS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 zlib.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"msvcrt.lib" /nodefaultlib:"libcd.lib" /nodefaultlib:"libcmtd.lib" /nodefaultlib:"msvcrtd.lib"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 zlib.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"msvcrt.lib" /nodefaultlib:"libcd.lib" /nodefaultlib:"libcmtd.lib" /nodefaultlib:"msvcrtd.lib" /out:"Sims/simssfx.exe" /libpath:"..\VAL\Release" /libpath:"..\VALWin32\Release"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "WJESFX - Win32 Half Life Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WJESFX___Win32_Half_Life_Release"
# PROP BASE Intermediate_Dir "WJESFX___Win32_Half_Life_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Halflife"
# PROP Intermediate_Dir "Halflife"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "..\VAL\Application" /I "..\VAL\Avatar" /I "..\VAL\AvatarFiles" /I "..\VAL\DataTypes" /I "..\VAL\Image" /I "..\VAL\ImageFiles" /I "..\VALWin32\Application" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\VAL\Application" /I "..\VAL\Avatar" /I "..\VAL\AvatarFiles" /I "..\VAL\DataTypes" /I "..\VAL\Image" /I "..\VAL\ImageFiles" /I "..\VALWin32\Application" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "GAME_HALFLIFE" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 zlib.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"msvcrt.lib" /nodefaultlib:"libcd.lib" /nodefaultlib:"libcmtd.lib" /nodefaultlib:"msvcrtd.lib"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 zlib.lib val.lib valwin32.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"msvcrt.lib" /nodefaultlib:"libcd.lib" /nodefaultlib:"libcmtd.lib" /nodefaultlib:"msvcrtd.lib" /out:"Halflife/hlsfx.exe" /libpath:"..\VAL\Release" /libpath:"..\VALWin32\Release"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "WJESFX - Win32 Unreal Tournament Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WJESFX___Win32_Unreal_Tournament_Release"
# PROP BASE Intermediate_Dir "WJESFX___Win32_Unreal_Tournament_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "UnrealTournament"
# PROP Intermediate_Dir "UnrealTournament"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "..\VAL\Application" /I "..\VAL\Avatar" /I "..\VAL\AvatarFiles" /I "..\VAL\DataTypes" /I "..\VAL\Image" /I "..\VAL\ImageFiles" /I "..\VALWin32\Application" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\VAL\Application" /I "..\VAL\Avatar" /I "..\VAL\AvatarFiles" /I "..\VAL\DataTypes" /I "..\VAL\Image" /I "..\VAL\ImageFiles" /I "..\VALWin32\Application" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "GAME_UNREALTOURNAMENT" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 zlib.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"msvcrt.lib" /nodefaultlib:"libcd.lib" /nodefaultlib:"libcmtd.lib" /nodefaultlib:"msvcrtd.lib"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 zlib.lib val.lib valwin32.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"msvcrt.lib" /nodefaultlib:"libcd.lib" /nodefaultlib:"libcmtd.lib" /nodefaultlib:"msvcrtd.lib" /out:"UnrealTournament/utsfx.exe" /libpath:"..\VAL\Release" /libpath:"..\VALWin32\Release"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "WJESFX - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WJESFX___Win32_Debug"
# PROP BASE Intermediate_Dir "WJESFX___Win32_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "..\VAL\Application" /I "..\VAL\Avatar" /I "..\VAL\AvatarFiles" /I "..\VAL\DataTypes" /I "..\VAL\Image" /I "..\VAL\ImageFiles" /I "..\VALWin32\Application" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "GAME_HALFLIFE" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\VAL\Application" /I "..\VAL\Avatar" /I "..\VAL\AvatarFiles" /I "..\VAL\DataTypes" /I "..\VAL\Image" /I "..\VAL\ImageFiles" /I "..\VALWin32\Application" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "GAME_HALFLIFE" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 zlib.lib val.lib valwin32.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"msvcrt.lib" /nodefaultlib:"libcd.lib" /nodefaultlib:"libcmtd.lib" /nodefaultlib:"msvcrtd.lib" /out:"Halflife/hlsfx.exe" /libpath:"..\VAL\Release" /libpath:"..\VALWin32\Release"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 zlibd.lib /nologo /subsystem:windows /incremental:yes /debug /machine:I386 /nodefaultlib:"libcd.lib" /nodefaultlib:"nafxcwd.lib" /pdbtype:sept /libpath:"..\VAL\Debug" /libpath:"..\VALWin32\Debug"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "WJESFX - Win32 Sims Release"
# Name "WJESFX - Win32 Half Life Release"
# Name "WJESFX - Win32 Unreal Tournament Release"
# Name "WJESFX - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AboutDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CreditsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgressDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD BASE CPP /Yc"stdafx.h"
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\WJESFX.cpp
# End Source File
# Begin Source File

SOURCE=.\WJESFX.rc
# End Source File
# Begin Source File

SOURCE=.\WJESFXDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AboutDlg.h
# End Source File
# Begin Source File

SOURCE=.\CreditsDlg.h
# End Source File
# Begin Source File

SOURCE=.\ProgressDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\WJESFX.h
# End Source File
# Begin Source File

SOURCE=.\WJESFXDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\logod.bmp
# End Source File
# Begin Source File

SOURCE=.\res\logou.bmp
# End Source File
# Begin Source File

SOURCE=.\res\WJESFX.ico
# End Source File
# Begin Source File

SOURCE=.\res\WJESFX.rc2
# End Source File
# End Group
# End Target
# End Project
