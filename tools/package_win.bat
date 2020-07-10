@echo off

set oldpath=%cd% 
cd "%~dp0"

if [%1] == [] (
	python package.py win || exit /b %errorlevel%
) else (
	python package.py win "%1" || exit /b %errorlevel%
)

cd "%oldpath%"
if not "%2" == "-s" pause