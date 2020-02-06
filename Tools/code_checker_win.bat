@echo off

set oldpath=%cd% 
cd "%~dp0"

python code_checker.py

cd "%oldpath%"
if NOT "%1"=="-s" PAUSE