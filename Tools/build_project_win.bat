@echo off

set oldpath=%cd% 
cd "%~dp0"

if [%1] == [] (
	call MSBuild.exe ../PhotonBox.sln /p:Configuration=Release
	goto end
) 
if %1 == "Release" (
	call MSBuild.exe ../PhotonBox.sln /p:Configuration=Release
	goto end
)
if %1 == "Debug" (
	call MSBuild.exe ../PhotonBox.sln
	goto end
)

:end 
cd "%oldpath%"
IF NOT "%2"=="-s" PAUSE