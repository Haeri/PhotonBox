@echo off

:: Store caller path
set old_path=%cd%
set err=0 
cd "%~dp0"
::-----------------------



cd ..
rmdir /S /Q "build"
mkdir "build"
cd build

cmake .. -DVCPKG_TARGET_TRIPLET=x64-windows -DCMAKE_TOOLCHAIN_FILE="../extern/vcpkg/scripts/buildsystems/vcpkg.cmake"
set /a "err=%err%+%errorlevel%"




::-----------------------
:: Restore to caller path
cd "%old_path%"
if not "%1" == "-s" pause
exit /b %err%