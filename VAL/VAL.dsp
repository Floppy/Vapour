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
# PROP Scc_ProjName "VAL"
# PROP Scc_LocalPath "."
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
# ADD CPP /nologo /MT /W3 /GX /O2 /I "Application" /I "Avatar" /I "AvatarFiles" /I "DataTypes" /I "Image" /I "ImageFiles" /I "Wrapper" /I "Render" /I "Model" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "VAL_BUILD" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "Application" /I "Avatar" /I "AvatarFiles" /I "DataTypes" /I "Image" /I "ImageFiles" /I "Wrapper" /I "Render" /I "Model" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "VAL_BUILD" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Debug\VALd.lib"

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

SOURCE=.\AvatarFiles\AvatarFile3DS.cpp
# End Source File
# Begin Source File

SOURCE=.\AvatarFiles\AvatarFileAME.cpp
# End Source File
# Begin Source File

SOURCE=.\AvatarFiles\AvatarFileDXF.cpp
# End Source File
# Begin Source File

SOURCE=.\AvatarFiles\avatarfilegeneric.cpp

!IF  "$(CFG)" == "VAL - Win32 Release"

!ELSEIF  "$(CFG)" == "VAL - Win32 Debug"

# SUBTRACT CPP /FA<none>

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\AvatarFiles\AvatarFileHalflife.cpp
# End Source File
# Begin Source File

SOURCE=.\AvatarFiles\AvatarFileHANIM.cpp
# End Source File
# Begin Source File

SOURCE=.\AvatarFiles\AvatarFileOBJ.cpp
# End Source File
# Begin Source File

SOURCE=.\AvatarFiles\AvatarFileSims.cpp
# End Source File
# Begin Source File

SOURCE=.\Avatar\AvatarFileStore.cpp
# End Source File
# Begin Source File

SOURCE=.\AvatarFiles\AvatarFileUnreal.cpp
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

SOURCE=.\Render\DisplayContext.cpp
# End Source File
# Begin Source File

SOURCE=.\DataTypes\EulerRotation.cpp
# End Source File
# Begin Source File

SOURCE=.\Model\HeightField.cpp
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

SOURCE=.\ImageFiles\ImageFileRaw.cpp
# End Source File
# Begin Source File

SOURCE=.\Image\ImageFileStore.cpp
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

SOURCE=.\Render\RenderAvatar.cpp
# End Source File
# Begin Source File

SOURCE=.\Render\RenderContext.cpp
# End Source File
# Begin Source File

SOURCE=.\Render\RenderContextStore.cpp
# End Source File
# Begin Source File

SOURCE=.\Render\RenderLight.cpp
# End Source File
# Begin Source File

SOURCE=.\Render\RenderObject.cpp
# End Source File
# Begin Source File

SOURCE=.\DataTypes\Scalar.cpp
# End Source File
# Begin Source File

SOURCE=.\Render\Scene.cpp
# End Source File
# Begin Source File

SOURCE=.\Render\SceneAvatarPose.cpp
# End Source File
# Begin Source File

SOURCE=.\Render\SceneHLThumbnail.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\SFX.cpp
# End Source File
# Begin Source File

SOURCE=.\Wrapper\SgatoHalfLife.cpp
# End Source File
# Begin Source File

SOURCE=.\Wrapper\SgatoSims.cpp
# End Source File
# Begin Source File

SOURCE=.\Wrapper\SgatoUnrealTournament.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Application\Stores.cpp
# End Source File
# Begin Source File

SOURCE=.\Model\SurfaceMaterial.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\TimeLimit.cpp
# End Source File
# Begin Source File

SOURCE=.\Model\TriMesh.cpp
# End Source File
# Begin Source File

SOURCE=.\AvatarFiles\Unreal.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\VAL.cpp
# End Source File
# Begin Source File

SOURCE=.\DataTypes\Vector3D.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Wedgie.cpp
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

SOURCE=.\AvatarFiles\AvatarFile3DS.h
# End Source File
# Begin Source File

SOURCE=.\AvatarFiles\AvatarFileAME.h
# End Source File
# Begin Source File

SOURCE=.\AvatarFiles\AvatarFileDXF.h
# End Source File
# Begin Source File

SOURCE=.\AvatarFiles\avatarfilegeneric.h
# End Source File
# Begin Source File

SOURCE=.\AvatarFiles\AvatarFileHalflife.h
# End Source File
# Begin Source File

SOURCE=.\AvatarFiles\AvatarFileHANIM.h
# End Source File
# Begin Source File

SOURCE=.\AvatarFiles\AvatarFileOBJ.h
# End Source File
# Begin Source File

SOURCE=.\Avatar\AvatarFileProxy.h
# End Source File
# Begin Source File

SOURCE=.\AvatarFiles\AvatarFileSims.h
# End Source File
# Begin Source File

SOURCE=.\AvatarFiles\AvatarFileSimsOptions.h
# End Source File
# Begin Source File

SOURCE=.\Avatar\AvatarFileStore.h
# End Source File
# Begin Source File

SOURCE=.\AvatarFiles\AvatarFileUnreal.h
# End Source File
# Begin Source File

SOURCE=.\AvatarFiles\AvatarFileUnrealOptions.h
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

SOURCE=.\Render\DisplayContext.h
# End Source File
# Begin Source File

SOURCE=.\DataTypes\DList.h
# End Source File
# Begin Source File

SOURCE=.\DataTypes\EulerRotation.h
# End Source File
# Begin Source File

SOURCE=.\AvatarFiles\HalflifeMDL.h
# End Source File
# Begin Source File

SOURCE=.\Model\HeightField.h
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

SOURCE=.\ImageFiles\ImageFileRaw.h
# End Source File
# Begin Source File

SOURCE=.\Image\ImageFileStore.h
# End Source File
# Begin Source File

SOURCE=.\DataTypes\MathConstants.h
# End Source File
# Begin Source File

SOURCE=.\Model\Model.h
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

SOURCE=.\Render\RenderAvatar.h
# End Source File
# Begin Source File

SOURCE=.\Render\RenderContext.h
# End Source File
# Begin Source File

SOURCE=.\Render\RenderContextProxy.h
# End Source File
# Begin Source File

SOURCE=.\Render\RenderContextStore.h
# End Source File
# Begin Source File

SOURCE=.\Render\RenderLight.h
# End Source File
# Begin Source File

SOURCE=.\Render\RenderObject.h
# End Source File
# Begin Source File

SOURCE=.\DataTypes\Scalar.h
# End Source File
# Begin Source File

SOURCE=.\Render\Scene.h
# End Source File
# Begin Source File

SOURCE=.\Render\SceneAvatarPose.h
# End Source File
# Begin Source File

SOURCE=.\Render\SceneHLThumbnail.h
# End Source File
# Begin Source File

SOURCE=.\Application\SFX.h
# End Source File
# Begin Source File

SOURCE=.\Wrapper\SgatoHalfLife.h
# End Source File
# Begin Source File

SOURCE=.\Wrapper\SgatoSims.h
# End Source File
# Begin Source File

SOURCE=.\Wrapper\SgatoUnrealTournament.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Model\SurfaceMaterial.h
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

SOURCE=.\Model\TriMesh.h
# End Source File
# Begin Source File

SOURCE=.\AvatarFiles\Unreal.h
# End Source File
# Begin Source File

SOURCE=.\Application\VAL.h
# End Source File
# Begin Source File

SOURCE=.\DataTypes\Vector3D.h
# End Source File
# Begin Source File

SOURCE=.\Application\Wedgie.h
# End Source File
# Begin Source File

SOURCE=.\Wrapper\Wrapper.h
# End Source File
# End Group
# End Target
# End Project
