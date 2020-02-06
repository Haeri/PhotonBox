@echo off

set oldpath=%cd% 
cd "%~dp0"

if [%1] == [] (
	python package.py win
) else (
	python package.py win "%1"
)

cd "%oldpath%"
if NOT "%2"=="-s" PAUSE