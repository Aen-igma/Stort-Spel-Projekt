@echo off
copy debugdll\*.dll ..\bin\Debug-x64
copy releasedll\*.dll ..\bin\Release-x64
copy *.dll ..\bin\Release-x64
copy *.dll ..\bin\Debug-x64
if not exist "..\Aenir Engine 3.0\src\AenirEngine\ThirdParty\PhysX\lib\debug\" mkdir "..\Aenir Engine 3.0\src\AenirEngine\ThirdParty\PhysX\lib\debug\"
if not exist "..\Aenir Engine 3.0\src\AenirEngine\ThirdParty\PhysX\lib\release\" mkdir "..\Aenir Engine 3.0\src\AenirEngine\ThirdParty\PhysX\lib\release\"
copy debugdll\*.lib "..\Aenir Engine 3.0\src\AenirEngine\ThirdParty\PhysX\lib\debug\"
copy releasedll\*.lib "..\Aenir Engine 3.0\src\AenirEngine\ThirdParty\PhysX\lib\release\"