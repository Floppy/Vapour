@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by SPINDOCTOR.HPJ. >"hlp\SpinDoctor.hm"
echo. >>"hlp\SpinDoctor.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\SpinDoctor.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\SpinDoctor.hm"
echo. >>"hlp\SpinDoctor.hm"
echo // Prompts (IDP_*) >>"hlp\SpinDoctor.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\SpinDoctor.hm"
echo. >>"hlp\SpinDoctor.hm"
echo // Resources (IDR_*) >>"hlp\SpinDoctor.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\SpinDoctor.hm"
echo. >>"hlp\SpinDoctor.hm"
echo // Dialogs (IDD_*) >>"hlp\SpinDoctor.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\SpinDoctor.hm"
echo. >>"hlp\SpinDoctor.hm"
echo // Frame Controls (IDW_*) >>"hlp\SpinDoctor.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\SpinDoctor.hm"
REM -- Make help for Project SPINDOCTOR


echo Building Win32 Help files
start /wait hcw /C /E /M "hlp\SpinDoctor.hpj"
if errorlevel 1 goto :Error
if not exist "hlp\SpinDoctor.hlp" goto :Error
if not exist "hlp\SpinDoctor.cnt" goto :Error
echo.
if exist Debug\nul copy "hlp\SpinDoctor.hlp" Debug
if exist Debug\nul copy "hlp\SpinDoctor.cnt" Debug
if exist Release\nul copy "hlp\SpinDoctor.hlp" Release
if exist Release\nul copy "hlp\SpinDoctor.cnt" Release
echo.
goto :done

:Error
echo hlp\SpinDoctor.hpj(1) : error: Problem encountered creating help file

:done
echo.
