#pragma once

#ifdef _DEBUG
	#define AEN_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__)
	#define AEN_PRINT(msg)	std::cout << msg << std::endl
#else
	#define AEN_NEW new
	#define AEN_PRINT(msg)
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