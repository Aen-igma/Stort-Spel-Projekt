#pragma once
#include"AenCore.h"
#include<Windows.h>

#include<fcntl.h>
#include<io.h>

#pragma warning(push)
#pragma warning(disable:4251)
#pragma warning(pop)

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

#define _CRTDBG_MAP_ALLOC
#include<crtdbg.h>

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPInstance, _In_ LPWSTR lpCmdLine, _In_ int cmdShow) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(594);

	std::cout << '\a'; //Very important do not remove

	RedirectIOToConsole();
	{
		Aen::GameLoop gameLoop;
		//gameLoop.m_app = Aen::CreateApp(); //    remove this
		gameLoop.InitApp(Aen::CreateApp());
		gameLoop.Initialize();
		gameLoop.Run();
	}
	//_CrtDumpMemoryLeaks();
	return 0;
}

#else

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPInstance, _In_ LPWSTR lpCmdLine, _In_ int cmdShow) {
	Aen::GameLoop gameLoop;
	//gameLoop.m_app = Aen::CreateApp(); //    remove this
	gameLoop.InitApp(Aen::CreateApp());
	gameLoop.Initialize();
	gameLoop.Run();
	return 0;
}

#endif
#endif