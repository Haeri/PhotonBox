@echo off

echo ============ DEPLOYER ============
echo.
echo DEBUG, RELEASE or PACKED (d/r/p)?

:ask
set INPUT=
set /P INPUT=Deploy mode: %=%

If /I "%INPUT%"=="r" goto release
If /I "%INPUT%"=="d" goto debug
If /I "%INPUT%"=="p" goto packed
goto ask

:debug
echo .obj > ".\filter.txt"
rmdir /s /q "..\Deploy_Debug"
mkdir "..\Deploy_Debug"
xcopy /s /i /exclude:filter.txt "..\PhotonBox\engine-res" "..\Deploy_Debug\PhotonBox\engine-res"
IF %ERRORLEVEL% NEQ 0 (
	goto err
)
xcopy /s /i /exclude:filter.txt "..\Game\res" "..\Deploy_Debug\Game\res"
IF %ERRORLEVEL% NEQ 0 (
	goto err
)
xcopy /s /i "..\Redist\Debug" "..\Deploy_Debug\Game"
IF %ERRORLEVEL% NEQ 0 (
	goto err
)
xcopy /s /i "..\Game\bin\x64\Debug\Game.exe" "..\Deploy_Debug\Game"
IF %ERRORLEVEL% NEQ 0 (
	goto err
)
goto succ


:release
echo .obj > ".\filter.txt"
rmdir /s /q "..\Deploy_Release"
mkdir "..\Deploy_Release"
xcopy /s /i /exclude:filter.txt "..\PhotonBox\engine-res" "..\Deploy_Release\PhotonBox\engine-res"
IF %ERRORLEVEL% NEQ 0 (
	goto err
)
xcopy /s /i /exclude:filter.txt "..\Game\res" "..\Deploy_Release\Game\res"
IF %ERRORLEVEL% NEQ 0 (
	goto err
)
xcopy /s /i "..\Redist\Release" "..\Deploy_Release\Game"
IF %ERRORLEVEL% NEQ 0 (
	goto err
)
xcopy /s /i "..\Game\bin\x64\Release\Game.exe" "..\Deploy_Release\Game"
IF %ERRORLEVEL% NEQ 0 (
	goto err
)
goto succ

:packed
echo .obj > ".\filter.txt"
rmdir /s /q "..\Deploy_Release"
mkdir "..\Deploy_Release"
xcopy /s /i /exclude:filter.txt "..\PhotonBox\engine-res" "..\Deploy_Release\Game\engine-res"
IF %ERRORLEVEL% NEQ 0 (
	goto err
)
xcopy /s /i /exclude:filter.txt "..\Game\res" "..\Deploy_Release\Game\res"
IF %ERRORLEVEL% NEQ 0 (
	goto err
)
xcopy /s /i "..\Redist\Release" "..\Deploy_Release\Game"
IF %ERRORLEVEL% NEQ 0 (
	goto err
)
xcopy /s /i "..\Game\bin\x64\Release\Game.exe" "..\Deploy_Release\Game"
IF %ERRORLEVEL% NEQ 0 (
	goto err
)
goto succ


:err
echo.
echo ==============================================
echo An error occured!
echo ==============================================
goto end

:succ
echo.
echo ==============================================
echo Deploying succesful!
echo ==============================================
goto end

:end
del ".\filter.txt"
echo.
pause
exit