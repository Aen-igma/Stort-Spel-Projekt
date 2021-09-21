@echo off
copy debug\*.dll ..\bin\Debug-x64
copy release\*.dll ..\bin\Release-x64
copy *.dll ..\bin\Release-x64
copy *.dll ..\bin\Debug-x64