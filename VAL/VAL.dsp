# Microsoft Developer Studio Project File - Name="VAL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=VAL - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "VAL.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "VAL.mak" CFG="VAL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "VAL - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "VAL - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "VAL - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "Application" /I "Avatar" /I "AvatarFiles" /I "DataTypes" /I "Image" /I "ImageFiles" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Copy out release VAL library
PostBuild_Cmds=copy Release\VAL.lib ..\..\Lib	copy Application\SGAToSims.h ..\..\Include	copy Application\CommandLine.h ..\..\Include	copy Application\ProgressControl.h ..\..\Include
# End Special Build Tool

!ELSEIF  "$(CFG)" == "VAL - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "Application" /I "Avatar" /I "AvatarFiles" /I "DataTypes" /I "Image" /I "ImageFiles" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Debug\VALd.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Copy out debug VAL library
PostBuild_Cmds=copy Debug\VALd.lib ..\..\Lib	copy Application\SGAToSims.h ..\..\Include	copy Application\CommandLine.h ..\..\Include	copy Application\ProgressControl.h ..\..\Include
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "VAL - Win32 Release"
# Name "VAL - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Avatar\Avatar.cpp
# End Source File
# Begin Source File

SOURCE=.\Avatar\AvatarFile.cpp
# End Source File
# Begin Source File

SOURCE=.\AvatarFiles\AvatarFileAME.cpp
# End Source File
# Begin Source File

SOURCE=.\AvatarFiles\AvatarFileSims.cpp
# End Source File
# Begin Source File

SOURCE=.\Avatar\AvatarFileStore.cpp
# End Source File
# Begin Source File

SOURCE=.\Avatar\AvatarPose.cpp
# End Source File
# Begin Source File

SOURCE=.\DataTypes\AxisRotation.cpp
# End Source File
# Begin Source File

SOURCE=.\Avatar\BodyPart.cpp
# End Source File
# Begin Source File

SOURCE=.\Image\ColourOctantPos.cpp
# End Source File
# Begin Source File

SOURCE=.\Image\ColourOctree.cpp
# End Source File
# Begin Source File

SOURCE=.\Image\ColourQuant.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\CommandLine.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\CompressDeflate.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\CRC32.cpp
# End Source File
# Begin Source File

SOURCE=.\DataTypes\EulerRotation.cpp
# End Source File
# Begin Source File

SOURCE=.\DataTypes\HomTransform.cpp
# End Source File
# Begin Source File

SOURCE=.\Image\Image.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageFiles\ImageFileBMP.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageFiles\ImageFileJPEG.cpp
# End Source File
# Begin Source File

SOURCE=.\Image\ImageFileStore.cpp
# End Source File
# Begin Source File

SOURCE=.\Image\JointList.cpp
# End Source File
# Begin Source File

SOURCE=.\Image\Palette.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\ProgressControl.cpp
# End Source File
# Begin Source File

SOURCE=.\DataTypes\Quaternion.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\SgatoSims.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Application\Stores.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\TimeLimit.cpp
# End Source File
# Begin Source File

SOURCE=.\DataTypes\Vector3D.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Avatar\Avatar.h
# End Source File
# Begin Source File

SOURCE=.\Avatar\AvatarFile.h
# End Source File
# Begin Source File

SOURCE=.\AvatarFiles\AvatarFileAME.h
# End Source File
# Begin Source File

SOURCE=.\Avatar\AvatarFileProxy.h
# End Source File
# Begin Source File

SOURCE=.\AvatarFiles\AvatarFileSims.h
# End Source File
# Begin Source File

SOURCE=.\Avatar\AvatarFileStore.h
# End Source File
# Begin Source File

SOURCE=.\Avatar\AvatarPose.h
# End Source File
# Begin Source File

SOURCE=.\DataTypes\AxisRotation.h
# End Source File
# Begin Source File

SOURCE=.\Image\basefilter.h
# End Source File
# Begin Source File

SOURCE=.\Image\bilinearfilter.h
# End Source File
# Begin Source File

SOURCE=.\Avatar\BodyPart.h
# End Source File
# Begin Source File

SOURCE=.\Image\boxfilter.h
# End Source File
# Begin Source File

SOURCE=.\Image\ColourOctantPos.h
# End Source File
# Begin Source File

SOURCE=.\Image\ColourOctree.h
# End Source File
# Begin Source File

SOURCE=.\Image\ColourQuant.h
# End Source File
# Begin Source File

SOURCE=.\Application\CommandLine.h
# End Source File
# Begin Source File

SOURCE=.\Application\Compress.h
# End Source File
# Begin Source File

SOURCE=.\Application\CompressDeflate.h
# End Source File
# Begin Source File

SOURCE=.\Application\CRC32.h
# End Source File
# Begin Source File

SOURCE=.\Application\Defs.h
# End Source File
# Begin Source File

SOURCE=.\DataTypes\DList.h
# End Source File
# Begin Source File

SOURCE=.\DataTypes\EulerRotation.h
# End Source File
# Begin Source File

SOURCE=.\Avatar\HalflifeMDL.h
# End Source File
# Begin Source File

SOURCE=.\DataTypes\HomTransform.h
# End Source File
# Begin Source File

SOURCE=.\Image\Image.h
# End Source File
# Begin Source File

SOURCE=.\Image\ImageFile.h
# End Source File
# Begin Source File

SOURCE=.\ImageFiles\ImageFileBMP.h
# End Source File
# Begin Source File

SOURCE=.\ImageFiles\ImageFileJPEG.h
# End Source File
# Begin Source File

SOURCE=.\Image\ImageFileProxy.h
# End Source File
# Begin Source File

SOURCE=.\Image\ImageFileStore.h
# End Source File
# Begin Source File

SOURCE=.\Image\JointList.h
# End Source File
# Begin Source File

SOURCE=.\DataTypes\MathConstants.h
# End Source File
# Begin Source File

SOURCE=.\Image\Palette.h
# End Source File
# Begin Source File

SOURCE=.\DataTypes\Point3D.h
# End Source File
# Begin Source File

SOURCE=.\Application\ProgressControl.h
# End Source File
# Begin Source File

SOURCE=.\DataTypes\Quaternion.h
# End Source File
# Begin Source File

SOURCE=.\Application\SgatoSims.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\DataTypes\TexCoord.h
# End Source File
# Begin Source File

SOURCE=.\Application\TimeLimit.h
# End Source File
# Begin Source File

SOURCE=.\DataTypes\TriFace.h
# End Source File
# Begin Source File

SOURCE=.\DataTypes\Vector3D.h
# End Source File
# End Group
# End Target
# End Project
