#pragma once
#include"WindowDef.h"


namespace Aen {

	struct AEN_DECLSPEC WindowDesc {
		UINT width;
		UINT height;
		DWORD EXStyle;
		DWORD style;
		HMENU hMenu;
		
		UINT WCStyle;
		HICON icon;
		HICON iconSmall;
		HCURSOR cursor;
		HBRUSH background;

		WindowDesc();

		void LoadBigIcon(const std::wstring& filePath);
		void LoadSmallIcon(const std::wstring& filePath);
		void LoadMouseCursor(const std::wstring& filePath);

		void SetIcon(const LPCWSTR& flag);
		void SetSmallIcon(const LPCWSTR& flag);
		void SetCursor(const LPCWSTR& flag);
	};
}