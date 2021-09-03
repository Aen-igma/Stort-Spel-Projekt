#include"PCH.h"
#include"WindowHandle.h"
#include"Window.h"
#include"Input.h"


namespace Aen {

	int Window::idIt(0);

	Window::~Window() {
		Exit();
		WindowHandle::GetWindows().erase(id);
	}

	Window::Window(const std::wstring& windowName, const std::wstring& className)
		:m_cName(className), m_wName(windowName), id(idIt) {
		hInstance = GetModuleHandle(NULL);
		lpfnWndProc = MsgRouter;
		lpszClassName = className.c_str();

		cbSize = sizeof(WNDCLASSEXW);
		lpszMenuName = 0;
		cbClsExtra = 0;
		cbWndExtra = 0;
		style = 0;
		hIcon = LoadIcon(NULL, AEN_IDI_APPLICATION);
		hIconSm = LoadIcon(NULL, AEN_IDI_APPLICATION);
		hCursor = LoadCursor(NULL, AEN_IDC_ARROW);
		hbrBackground = CreateSolidBrush(RGB(0, 0, 0));

		if(!Register()) {
			MessageBoxW(m_hwnd, L"AEN_ERROR: Failed to register window.", L"ERROR", MB_ICONERROR);
			throw;
		}

		m_hInstance = GetModuleHandle(NULL);
		m_style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
		m_width = 720;
		m_height = 480;
		m_EXstyle = 0;
		m_hwndParent = 0;
		m_hMenu = 0;

		if(!Initialize()) {
			MessageBoxW(m_hwnd, L"AEN_ERROR: Failed to register window.", L"ERROR", MB_ICONERROR);
			throw;
		}
		
		WindowHandle::GetWindows().insert(std::pair<int, Window*>(id, this));
		idIt++;
	}

	Window::Window(const WindowDesc& desc, const std::wstring& windowName, const std::wstring& className) 
		:m_cName(className), m_wName(windowName), id(idIt) {
		hInstance = GetModuleHandle(NULL);
		lpfnWndProc = MsgRouter;
		lpszClassName = className.c_str();

		cbSize = sizeof(WNDCLASSEXW);
		lpszMenuName = 0;
		cbClsExtra = 0;
		cbWndExtra = 0;
		style = desc.WCStyle;
		hIcon = desc.icon;
		hIconSm = desc.iconSmall;
		hCursor = desc.cursor;
		hbrBackground = desc.background;

		if(!Register()) {
			MessageBoxW(m_hwnd, L"AEN_ERROR: Failed to register window.", L"ERROR", MB_ICONERROR);
			throw;
		}

		m_hInstance = GetModuleHandle(NULL);
		m_style = desc.style;
		m_width = desc.width;
		m_height = desc.height;
		m_EXstyle = desc.EXStyle;
		m_hwndParent = 0;
		m_hMenu = desc.hMenu;

		if(!Initialize()) {
			MessageBoxW(m_hwnd, L"AEN_ERROR: Failed to register window.", L"ERROR", MB_ICONERROR);
			throw;
		}

		WindowHandle::GetWindows().insert(std::pair<int, Window*>(id, this));
		idIt++;
	}

	void Window::Exit() {
		if(m_hwnd != NULL) {
			UnregisterClassW(m_cName.c_str(), m_hInstance);
			DestroyWindow(m_hwnd);
		}
	}

	void Window::LoadSettings(const WindowDesc& desc) {

		m_style = desc.style;
		m_width = desc.width;
		m_height = desc.height;
		m_EXstyle = desc.EXStyle;
		m_hMenu = desc.hMenu;

		style = desc.WCStyle;
		hIcon = desc.icon;
		hIconSm = desc.iconSmall;
		hCursor = desc.cursor;

		RECT rect = RECT();
		rect.left = static_cast<LONG>(GetSystemMetrics(SM_CXSCREEN) * 0.5f - m_width * 0.5f);
		rect.top = static_cast<LONG>(GetSystemMetrics(SM_CYSCREEN) * 0.5f - m_height * 0.5f);
		rect.right = rect.left + m_width;
		rect.bottom = rect.top + m_height;
		AdjustWindowRect(&rect, m_style, FALSE);

		SetWindowLongPtrW(m_hwnd, GWL_STYLE, m_style);
		SetWindowLongPtrW(m_hwnd, GWL_EXSTYLE, m_EXstyle);

		SetClassLongPtrW(m_hwnd, GCL_STYLE, (LONG_PTR)hIconSm);
		SetClassLongPtrW(m_hwnd, GCLP_HICON, (LONG_PTR)hIcon);
		SetClassLongPtrW(m_hwnd, GCLP_HICONSM, (LONG_PTR)hIconSm);
		SetClassLongPtrW(m_hwnd, GCLP_HCURSOR, (LONG_PTR)hCursor);

		SetWindowPos(
			m_hwnd,
			0,
			rect.left,
			rect.top,
			rect.right - rect.left,
			rect.bottom - rect.top,
			SWP_SHOWWINDOW);
	}

	void Window::SetWindowSize(const UINT& width, const UINT& height) {

		m_width = width;
		m_height = height;

		RECT rect = RECT();
		rect.left = (LONG)(GetSystemMetrics(SM_CXSCREEN) * 0.5f - m_width * 0.5f);
		rect.top = (LONG)(GetSystemMetrics(SM_CYSCREEN) * 0.5f - m_height * 0.5f);
		rect.right = rect.left + m_width;
		rect.bottom = rect.top + m_height;
		AdjustWindowRect(&rect, m_style, FALSE);

		SetWindowPos(
			m_hwnd,
			0,
			rect.left,
			rect.top,
			rect.right - rect.left,
			rect.bottom - rect.top,
			SWP_SHOWWINDOW);
	}

	const Vec2i Window::GetSize() const {
		Vec2i out(m_width, m_height);
		return out;
	}

	const Vec2i Window::GetWindowPos() const {
		return wPos;
	}

	const float Window::GetAspectRatio() const {
		return static_cast<float>(m_width) / static_cast<float>(m_height);
	}

	const bool Window::IsActive() const {
		return (m_hwnd == GetActiveWindow());
	}

	HWND& Window::GetWHND() {
		return m_hwnd;
	}

	LRESULT Window::WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

		switch(uMsg) {
			case WM_DESTROY:
				PostQuitMessage(0);
				return 0;
				break;
			case WM_SIZE:
				m_width = LOWORD(lParam);
				m_height = HIWORD(lParam);
				break;
		}

		return DefWindowProcW(hwnd, uMsg, wParam, lParam);
	}

	extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT Window::MsgRouter(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		Window* pWnd = nullptr;

		if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam)) return true;

		if(uMsg == WM_NCCREATE) {
			pWnd = (Window*)reinterpret_cast<LPCREATESTRUCT>(lParam)->lpCreateParams;
			SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		} else
			pWnd = reinterpret_cast<Window*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));


		switch(uMsg) {
			case WM_INPUT: {
				UINT dataSize = 0;
				GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, NULL, &dataSize, sizeof(RAWINPUTHEADER));

				if(dataSize > 0) {
					std::unique_ptr<BYTE[]> rawData = std::make_unique<BYTE[]>(dataSize);
					if(GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, rawData.get(), &dataSize, sizeof(RAWINPUTHEADER)) == dataSize) {
						RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(rawData.get());
						if(raw->header.dwType == RIM_TYPEMOUSE && (raw->data.mouse.lLastX != 0 || raw->data.mouse.lLastY != 0)) {
							Input::OnRawMouse((int)raw->data.mouse.lLastX, (int)raw->data.mouse.lLastY);
						}
					}
				}

				return DefWindowProcW(hwnd, uMsg, wParam, lParam);
			}
		}

		if(pWnd)
			return pWnd->WinProc(hwnd, uMsg, wParam, lParam);
		return DefWindowProcW(hwnd, uMsg, wParam, lParam);
	}

	bool Window::Register() {
		if(RegisterClassExW(this))
			return true;
		else
			return false;
	}

	bool Window::Initialize() {
		RECT rect = RECT();
		rect.left = static_cast<LONG>(GetSystemMetrics(SM_CXSCREEN) * 0.5f - m_width * 0.5f);
		rect.top = static_cast<LONG>(GetSystemMetrics(SM_CYSCREEN) * 0.5f - m_height * 0.5f);
		rect.right = rect.left + m_width;
		rect.bottom = rect.top + m_height;
		AdjustWindowRect(&rect, m_style, FALSE);

		wPos[0] = rect.left;
		wPos[1] = rect.top;

		m_hwnd = CreateWindowExW(
			m_EXstyle,
			m_cName.c_str(),
			m_wName.c_str(),
			m_style,
			rect.left,
			rect.top,
			rect.right - rect.left,
			rect.bottom - rect.top,
			m_hwndParent,
			m_hMenu,
			m_hInstance,
			this);

		return (m_hwnd != NULL);
	}

	bool Window::HandleMsg() {
		MSG msg;

		ZeroMemory(&msg, sizeof(msg));
		while(PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}

		if(msg.message == WM_NULL)
			if(!IsWindow(m_hwnd)) {
				m_hwnd = NULL;
				UnregisterClassW(m_cName.c_str(), m_hInstance);
				return false;
			}

		RECT rect;
		if(GetWindowRect(m_hwnd, &rect)) {
			wPos[0] = rect.left;
			wPos[1] = rect.top;
		}

		return true;
	}
}