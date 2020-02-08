@echo off

set oldpath=%cd% 
cd "%~dp0"

call "premake/win/premake5.exe" --file="../premake5.lua" vs2017 || exit /b %errorlevel%

cd "%oldpath%"
if not "%1" == "-s" pause