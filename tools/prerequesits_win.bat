@echo off

set oldpath=%cd% 
cd "%~dp0"

python ./prerequesits.py

cd "%oldpath%"
if not "%1" == "-s" pause
exit /b %errorlevel%