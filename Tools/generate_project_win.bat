@echo off

set oldpath=%cd% 
cd "%~dp0"
call "premake/win/premake5.exe" --file="../premake5.lua" vs2017
cd "%oldpath%"
IF NOT "%1"=="-s" PAUSE