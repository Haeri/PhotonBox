@echo off
cd ..
rmdir /s /q ".vs"
del /s /q "PhotonBox.sln"

cd PhotonBox
rmdir /s /q "bin"
del /s /q "PhotonBox.vcxproj"
del /s /q "PhotonBox.vcxproj.filters"
del /s /q "PhotonBox.vcxproj.user"

cd ..
cd Game
rmdir /s /q "bin"
del /s /q "Game.vcxproj"
del /s /q "Game.vcxproj.filters"
del /s /q "Game.vcxproj.user"

pause