@echo off

set oldpath=%cd% 
cd "%~dp0"

if [%1] == [] (
	call MSBuild.exe ../PhotonBox.sln /p:Configuration=Release || exit /b %errorlevel%
	goto end
) 
if "%1" == "Release" (
	call MSBuild.exe ../PhotonBox.sln /p:Configuration=Release || exit /b %errorlevel%
	goto end
)
if "%1" == "Debug" (
	call MSBuild.exe ../PhotonBox.sln || exit /b %errorlevel%
	goto end
)

:end 
cd "%oldpath%"
if not "%2" == "-s" pause