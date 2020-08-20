@echo off

set oldpath=%cd% 
cd "%~dp0"

python ./install.py

cd "%oldpath%"
if not "%1" == "-s" pause
exit /b %errorlevel%