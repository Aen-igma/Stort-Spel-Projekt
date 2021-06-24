#pragma once
#include"AenirEngine.h"



class Client : public Aen::App {
	public:
	Client() = delete;
	Client(const Aen::WindowDesc& desc, const std::wstring& windowName, const std::wstring& className);

	void Run() override;

	private:
};


Aen::App* Aen::CreateApp() {

	Aen::WindowDesc wDesc;
	wDesc.width = 720;
	wDesc.height = 480;
	wDesc.EXStyle = AEN_WS_EX_APPWINDOW;
	wDesc.style = AEN_WS_OVERLAPPEDWINDOW | AEN_WS_VISIBLE;
	wDesc.hMenu = 0;
	wDesc.WCStyle = 0;
	//wDesc.SetIcon(L"");
	//wDesc.SetSmallIcon(L"");
	//wDesc.SetCursor(L"");

	return new Client(wDesc, L"[ENTER CREATIVE NAME HERE]", L"Main_Window");
}