@echo off
setlocal EnableDelayedExpansion
setlocal enableextensions 

set script_dir=%CD%

set PathSDK="%script_dir%\..\dependencies\openvibe-sdk-release"
set VerboseOuptut=OFF

set BuildType=Release

goto parameter_parse

:print_help
	echo Usage: windows-launch-visual-studio.cmd --sdk ^<path to openvibe SDK^>
	exit /b

:parameter_parse
for %%A in (%*) DO (
	if /i "%%A"=="--sdk" (
		set next=SDK
	) else if "!next!"=="SDK" (
		set PathSDK=%%A
		set next=
	) else if /i "%%A"=="--debug" (
		set BuildType=Debug
		set PathSDK="%script_dir%\..\dependencies\openvibe-sdk-debug"
		set next=
	)
)

call "windows-initialize-environment.cmd" --sdk "%PathSDK%"

SET "OV_PATH_ROOT=%CD%\..\..\openvibe-designer-build\dist\%BuildType%-%PlatformTarget%"
SET "PATH=%OV_PATH_ROOT%\bin;%PATH%"

REM for visual studio express...
if not defined USE_EXPRESS (
	SET USE_EXPRESS=1
)

set SolutionPath=%CD%\..\..\openvibe-designer-build\vs-project\Designer.sln

if %USE_EXPRESS% == 1 (
	echo Use %VSCMake% Express Edition
	
	if "%VSCMake%"=="Visual Studio 12" (
		start /b "%VSINSTALLDIR%\Common7\IDE\WDExpress.exe" %SolutionPath%
	) else (
		"%VSINSTALLDIR%\Common7\IDE\VCExpress.exe" %SolutionPath%
	)
) else (
	echo Use %VSCMake% Community Edition
	"%VSINSTALLDIR%\Common7\IDE\devenv.exe" %SolutionPath%
)
