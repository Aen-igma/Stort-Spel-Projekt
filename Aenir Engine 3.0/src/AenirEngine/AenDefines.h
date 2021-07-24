#pragma once

#ifdef _DEBUG
	#define AEN_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__)
	#define AEN_PRINT(msg)	std::cout << msg
	
	#ifdef _WIN64
	#define AEN_OUTPUT_DIR_STR(file) "../bin/Debug-x64/" + file
	#define AEN_OUTPUT_DIR_WSTR(file) L"../bin/Debug-x64/" + std::wstring(file)
	#else
	#define AEN_OUTPUT_DIR_STR(file) "../bin/Debug-x32/" + file
	#define AEN_OUTPUT_DIR_WSTR(file) L"../bin/Debug-x32/" + std::wstring(file)
	#endif

	
#else
	#define AEN_NEW new
	#define AEN_PRINT(msg)

	#ifdef _WIN64
	#define AEN_OUTPUT_DIR_STR(file) "../bin/Release-x64/" + file
	#define AEN_OUTPUT_DIR_WSTR(file) L"../bin/Release-x64/" + std::wstring(file)
	#else
	#define AEN_OUTPUT_DIR_STR(file) "../bin/Release-x32/" + file
	#define AEN_OUTPUT_DIR_WSTR(file) L"../bin/Release-x32/" + std::wstring(file)
	#endif

#endif


#ifdef AEN_PLATFORM_WINDOWS
	#ifdef AEN_BUILD_DLL
	#define AEN_DECLSPEC __declspec(dllexport)
	#else
	#define AEN_DECLSPEC __declspec(dllimport)
	#endif
#else
	#error Aenir Engine only Supports windows.
#endif