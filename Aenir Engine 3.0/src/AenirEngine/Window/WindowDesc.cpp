#include"WindowDesc.h"

namespace Aen {

	WindowDesc::WindowDesc()
		:width(0), height(0), EXStyle(0), style(0), hMenu(NULL), WCStyle(0), icon(LoadIcon(NULL, AEN_IDI_APPLICATION)), iconSmall(LoadIcon(NULL, AEN_IDI_APPLICATION)),
		cursor(LoadCursor(NULL, IDC_ARROW)), background(CreateSolidBrush(RGB(0, 0, 0))) {
	}

	void WindowDesc::LoadBigIcon(const std::wstring& filePath) {
		icon = (HICON)LoadImageW(NULL, filePath.c_str(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);
	}

	void WindowDesc::LoadSmallIcon(const std::wstring& filePath) {
		iconSmall = (HICON)LoadImageW(NULL, filePath.c_str(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);
	}

	void WindowDesc::LoadMouseCursor(const std::wstring& filePath) {
		cursor = LoadCursorFromFileW(filePath.c_str());
	}

	void WindowDesc::SetIcon(const LPCWSTR& flag) {
		icon = LoadIconW(NULL, flag);
	}

	void WindowDesc::SetSmallIcon(const LPCWSTR& flag) {
		iconSmall = LoadIconW(NULL, flag);
	}

	void WindowDesc::SetCursor(const LPCWSTR& flag) {
		cursor = LoadCursorW(NULL, flag);
	}
}