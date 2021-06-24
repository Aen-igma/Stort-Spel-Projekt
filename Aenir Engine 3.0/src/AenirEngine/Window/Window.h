#pragma once
#include"..\AenMath.h"
#include"WindowDesc.h"

namespace Aen {

	class Window : protected WNDCLASSEXW {
		public:
		Window() = delete;
		~Window();
		Window(const std::wstring& windowName, const std::wstring& className);
		Window(const WindowDesc& desc, const std::wstring& windowName, const std::wstring& className);

		void Exit();
		void LoadSettings(const WindowDesc& desc);
		void SetWindowSize(const UINT& width, const UINT& height);

		const Vec2i GetSize();
		const Vec2i GetWindowPos();
		const float GetAspectRatio();

		LRESULT CALLBACK WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		private:

		static LRESULT CALLBACK MsgRouter(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual bool Register();
		bool Initialize();
		bool HandleMsg();

		HWND m_hwnd;
		UINT m_width;
		UINT m_height;
		std::wstring m_cName;
		std::wstring m_wName;
		HINSTANCE m_hInstance;
		DWORD m_EXstyle;
		DWORD m_style;
		HWND m_hwndParent;
		HMENU m_hMenu;
		static int idIt;
		int id;

		Vec2i wPos;

		friend struct WindowHandle;
	};
}