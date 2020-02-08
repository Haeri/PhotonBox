@echo off

set oldpath=%cd% 
cd "%~dp0"

python code_checker.py || exit /b %errorlevel%

cd "%oldpath%"
if not "%1" == "-s" pause