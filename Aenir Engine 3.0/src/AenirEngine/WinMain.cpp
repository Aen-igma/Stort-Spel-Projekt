#pragma once
#include"AenCore.h"
#include<Windows.h>

#include<fcntl.h>
#include<io.h>

#pragma warning(push)
#pragma warning(disable:4251)
#pragma warning(pop)


//Hopkok av Stefan Petersson (spr@bth.se), copyright 2003-2019 etc etc bla bla, glhf :-)

#include <d3d11_4.h>
#include <dxgi1_6.h>

#pragma comment(lib, "dxgi.lib")

#include <psapi.h>

#include <string>

void vramUsage()
{
	IDXGIFactory* dxgifactory = nullptr;
	HRESULT ret_code = ::CreateDXGIFactory(
		__uuidof(IDXGIFactory),
		reinterpret_cast<void**>(&dxgifactory));

	if (SUCCEEDED(ret_code))
	{
		IDXGIAdapter* dxgiAdapter = nullptr;

		if (SUCCEEDED(dxgifactory->EnumAdapters(0, &dxgiAdapter)))
		{
			IDXGIAdapter4* dxgiAdapter4 = NULL;
			if (SUCCEEDED(dxgiAdapter->QueryInterface(__uuidof(IDXGIAdapter4), (void**)&dxgiAdapter4)))
			{
				DXGI_QUERY_VIDEO_MEMORY_INFO info;

				if (SUCCEEDED(dxgiAdapter4->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &info)))
				{
					float memoryUsage = float(info.CurrentUsage / 1024.0 / 1024.0); //MiB

					char msg[100];
					sprintf_s(msg, "%.2f MiB used", memoryUsage);
					MessageBoxA(0, msg, "VRAM", 0);
				};

				dxgiAdapter4->Release();
			}
			dxgiAdapter->Release();
		}
		dxgifactory->Release();
	}
}

void ramUsage()
{
	//src: https://docs.microsoft.com/en-us/windows/desktop/api/psapi/ns-psapi-_process_memory_counters

	DWORD currentProcessID = GetCurrentProcessId();

	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, currentProcessID);

	if (NULL == hProcess)
		return;

	PROCESS_MEMORY_COUNTERS pmc{};
	if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)))
	{
		//PagefileUsage is the:
			//The Commit Charge value in bytes for this process.
			//Commit Charge is the total amount of memory that the memory manager has committed for a running process.

		float memoryUsage = float(pmc.PagefileUsage / 1024.0 / 1024.0); //MiB

		char msg[100];
		sprintf_s(msg, "%.2f MiB committed", memoryUsage);
		MessageBoxA(0, msg, "RAM", 0);
	}

	CloseHandle(hProcess);
}





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

	std::cout << '\a'; //Very important do not remove

	RedirectIOToConsole();
	{
		Aen::GameLoop gameLoop;
		gameLoop.InitApp(Aen::CreateApp());
		gameLoop.Initialize();
		gameLoop.Run();

		vramUsage();
		ramUsage();
	}
	return 0;
}

#else

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPInstance, _In_ LPWSTR lpCmdLine, _In_ int cmdShow) {
	Aen::GameLoop gameLoop;
	gameLoop.InitApp(Aen::CreateApp());
	gameLoop.Initialize();
	gameLoop.Run();

	vramUsage();
	ramUsage();
	return 0;
}

#endif
#endif