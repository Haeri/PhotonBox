@echo off

set oldpath=%cd% 
cd "%~dp0"

python cache_cleaner.py

cd "%oldpath%"
if NOT "%1"=="-s" PAUSE