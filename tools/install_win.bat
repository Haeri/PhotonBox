@echo off

:: Store caller path
set old_path = %cd%
set err=0 
cd "%~dp0"
::-----------------------




python ./install.py
set /a "err=%err%+%errorlevel%"



::-----------------------
:: Restore to caller path
cd "%old_path%"
if not "%1" == "-s" pause
exit /b %err%