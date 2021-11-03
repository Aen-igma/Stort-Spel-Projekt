#pragma once
#include <wincodec.h>

#ifdef _DEBUG
	#define AEN_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__)
	#define AEN_PRINT(msg)	std::cout << msg
	#define AEN_ENDL std::cout << std::endl
	
	#define AEN_RESOURCE_DIR(file) "../Resource/" + std::string(file)
	#define AEN_RESOURCE_DIR_W(file) L"../Resource/" + std::wstring(file)

	#define AEN_LEVEL_DIR(file) "../LevelFolder/" + std::string(file)

	#ifdef _WIN64
	#define AEN_OUTPUT_DIR_STR(file) "../bin/Debug-x64/" + std::string(file)
	#define AEN_OUTPUT_DIR_WSTR(file) L"../bin/Debug-x64/" + std::wstring(file)
	#else
	#define AEN_OUTPUT_DIR_STR(file) "../bin/Debug-x32/" + std::string(file)
	#define AEN_OUTPUT_DIR_WSTR(file) L"../bin/Debug-x32/" + std::wstring(file)
	#endif

	
#else
	#define AEN_NEW new
	#define AEN_PRINT(msg)
	#define AEN_ENDL
	
	#define AEN_RESOURCE_DIR(file) ((IsDebuggerPresent()) ? "../Resource/" + std::string(file) : "../../Resource/" + std::string(file))
	#define AEN_RESOURCE_DIR_W(file) ((IsDebuggerPresent()) ? L"../Resource/" + std::wstring(file) : L"../../Resource/" + std::wstring(file))

	#define AEN_LEVEL_DIR(file) ((IsDebuggerPresent()) ? "../LevelFolder/" + std::string(file) : "../../LevelFolder/" + std::string(file))

	#ifdef _WIN64
	#define AEN_OUTPUT_DIR_STR(file) "../bin/Release-x64/" + std::string(file)
	#define AEN_OUTPUT_DIR_WSTR(file) L"../bin/Release-x64/" + std::wstring(file)
	#else
	#define AEN_OUTPUT_DIR_STR(file) "../bin/Release-x32/" + std::string(file)
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