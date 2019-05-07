@echo off

set oldpath=%cd% 
cd "%~dp0"

if [%1] == [] (
	python deployer.py win
) else (
	python deployer.py win "%1"
)

cd "%oldpath%"
if NOT "%2"=="-s" PAUSE