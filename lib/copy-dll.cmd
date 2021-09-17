@echo off
copy debug\*.txt ..\bin\Debug-x64
copy release\*.txt ..\bin\Release-x64
copy *.txt ..\bin\Release-x64
copy *.txt ..\bin\Debug-x64