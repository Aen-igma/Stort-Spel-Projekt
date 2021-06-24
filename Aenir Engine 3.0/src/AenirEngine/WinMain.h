#pragma once
#include"AenCore.h"
#include<Windows.h>

#include<io.h>
#include<fcntl.h>



#ifdef AEN_PLATFORM_WINDOWS


extern Aen::App* Aen::CreateApp();


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

	Aen::App* app = Aen::CreateApp();

	while(Aen::WindowHandle::HandleMsg()) {
		app->Run();
	}

	delete app;
	return 0;
}

#else

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPInstance, LPWSTR lpCmdLine, int cmdShow) {


	return 0;
}

#endif
#endif