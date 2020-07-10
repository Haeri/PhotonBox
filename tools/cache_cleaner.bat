@echo off

set oldpath=%cd% 
cd "%~dp0"

python cache_cleaner.py || exit /b %errorlevel%

cd "%oldpath%"
if not "%1" == "-s" pause