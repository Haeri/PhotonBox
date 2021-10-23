@echo off

:: Store caller path
set old_path=%cd%
set err=0 
cd "%~dp0"
::-----------------------




cd ..
set root_path=%cd%

if not exist "%root_path%\external\vcpkg\scripts\buildsystems\vcpkg.cmake" (
	echo INFO: You forgot to download the submodules. I'll fix that for you.
	git submodule update --init
)

if not exist "build/" (
	echo INFO: First time setup will take longer as the dependencies need to be downloaded and compiled.
) else (
	rmdir /S /Q "build"
)

mkdir "build"
cd build

set build_type=
set triplet_value=x64-windows
if "%1" == "-static" (
	set build_type=-DBUILD_SHARED_LIBS=OFF
	set triplet_value=x64-windows-static
)

cmake .. ^
	-DVCPKG_TARGET_TRIPLET="%triplet_value%" ^
	-DCMAKE_TOOLCHAIN_FILE="%root_path%\external\vcpkg\scripts\buildsystems\vcpkg.cmake" %build_type%
set /a "err=%err%+%errorlevel%"




::-----------------------
:: Restore to caller path
cd "%old_path%"
if not "%1" == "-s" if not "%2" == "-s" pause
exit /b %err%