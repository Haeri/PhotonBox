@echo off

:: Store caller path
set old_path=%cd%
set err=0 
cd "%~dp0"
::-----------------------



cd ..

xcopy ".\res" ".\pb_res" /I/E
xcopy ".\build\Release\PhotonBox.lib" ".\lib\PhotonBox.lib*"

set /a "err=%err%+%errorlevel%"




::-----------------------
:: Restore to caller path
cd "%old_path%"
if not "%1" == "-s" if not "%2" == "-s" pause
exit /b %err%