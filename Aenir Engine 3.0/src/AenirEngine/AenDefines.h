#pragma once

#ifdef _DEBUG
	#define AEN_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__)
#else
	#define AEN_NEW new
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

