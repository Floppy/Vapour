# Microsoft Developer Studio Project File - Name="Saver" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Saver - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Saver.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Saver.mak" CFG="Saver - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Saver - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Saver - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Saver - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\VAL\Application" /I "..\VAL\Avatar" /I "..\VAL\AvatarFiles" /I "..\VAL\DataTypes" /I "..\VAL\Image" /I "..\VAL\ImageFiles" /I "..\VAL\Render" /I "..\VAL\Wrapper" /I "..\VAL\Model" /I "..\VAL\Scene" /I "..\VALWin32\Application" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 AME_SDK_DLL.lib opengl32.lib glu32.lib zlib.lib nafxcw.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"msvcrt.lib" /include:"?g_oAvatarProxyGeneric@@3V?$CAvatarFileProxy@VCAvatarFileGeneric@@@@A" /include:"?g_oAvatarProxyAME@@3V?$CAvatarFileProxy@VCAvatarFileAME@@@@A" /include:"?g_oRCProxyOpenGLWin32@@3V?$CRenderContextProxy@VCRCOpenGLWin32@@@@A" /out:"Release/Saver.scr"
# SUBTRACT LINK32 /nodefaultlib
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Setting SFX Markers
PostBuild_Cmds=sfxset Release\saver.scr sadata.wje
# End Special Build Tool

!ELSEIF  "$(CFG)" == "Saver - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\VAL\Application" /I "..\VAL\Avatar" /I "..\VAL\AvatarFiles" /I "..\VAL\DataTypes" /I "..\VAL\Image" /I "..\VAL\ImageFiles" /I "..\VAL\Render" /I "..\VAL\Wrapper" /I "..\VAL\Model" /I "..\VAL\Scene" /I "..\VALWin32\Application" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 AME_SDK_DLL.lib opengl32.lib glu32.lib zlibd.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcd.lib" /nodefaultlib:"nafxcwd.lib" /include:"?g_oAvatarProxyGeneric@@3V?$CAvatarFileProxy@VCAvatarFileGeneric@@@@A" /include:"?g_oAvatarProxyAME@@3V?$CAvatarFileProxy@VCAvatarFileAME@@@@A" /include:"?g_oRCProxyOpenGLWin32@@3V?$CRenderContextProxy@VCRCOpenGLWin32@@@@A" /out:"Debug/Saver.scr" /pdbtype:sept
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Setting SFX Markers
PostBuild_Cmds=sfxset debug\saver.scr
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "Saver - Win32 Release"
# Name "Saver - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Saver.cpp
# End Source File
# Begin Source File

SOURCE=.\Saver.rc
# End Source File
# Begin Source File

SOURCE=.\SaverSettings.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Saver.h
# End Source File
# Begin Source File

SOURCE=.\SaverSettings.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\Scricon3.ico
# End Source File
# Begin Source File

SOURCE=.\res\Vapour.ico
# End Source File
# End Group
# End Target
# End Project
