@echo off
if not exist bin mkdir bin
cd bin
if not exist Debug-x64\ mkdir Debug-x64
if not exist Release-x64\ (
	del Release-x64
	mkdir Release-x64
 )
cd ..\lib
start lib-patcher.cmd