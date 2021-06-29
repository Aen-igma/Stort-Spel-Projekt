#pragma once
#include"AenCore.h"
#include<Windows.h>

#include<fcntl.h>
#include<io.h>

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

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPInstance, _In_ LPWSTR lpCmdLine, _In_ int cmdShow) {
	RedirectIOToConsole();

	Aen::GameLoop gameLoop;
	gameLoop.m_app = Aen::CreateApp(); //    remove this
	gameLoop.Initialize();
	gameLoop.Run();
}

#else

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPInstance, _In_ LPWSTR lpCmdLine, _In_ int cmdShow) {
	Aen::GameLoop gameLoop;
	gameLoop.m_app = Aen::CreateApp(); //    remove this
	gameLoop.Initialize();
	gameLoop.Run();
	return 0;
}

#endif
#endif