#pragma once
#include"AenCore.h"
#include<Windows.h>

#include<io.h>
#include<fcntl.h>



#ifdef AEN_PLATFORM_WINDOWS
#ifdef _DEBUG

void RedirectIOToConsole() {
	AllocConsole();
	HANDLE stdHandle;
	int hConsole;
	FILE* fp;
	stdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	hConsole = _open_osfhandle((intptr_t)stdHandle, _O_TEXT);
	fp = _fdopen(hConsole, "w");

	freopen_s(&fp, "CONOUT$", "w", stdout);
}

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPInstance, LPWSTR lpCmdLine, int cmdShow) {
	RedirectIOToConsole();
	Aen::GameLoop gameLoop;
	gameLoop.m_app = Aen::CreateApp(); //    remove this
	gameLoop.Initialize();
	gameLoop.Run();
	return 0;
}

#else

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPInstance, LPWSTR lpCmdLine, int cmdShow) {
	Aen::GameLoop gameLoop;
	gameLoop.Initialize();
	gameLoop.Run();
	return 0;
}

#endif
#endif