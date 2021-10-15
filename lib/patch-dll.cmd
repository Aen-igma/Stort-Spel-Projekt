@echo off
copy debugdll\*.dll ..\bin\Debug-x64
copy releasedll\*.dll ..\bin\Release-x64
copy *.dll ..\bin\Release-x64
copy *.dll ..\bin\Debug-x64