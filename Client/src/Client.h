#pragma once
#include "States\Gameplay.h"
#include "States\Menu.h"
#include "States\Loadscreen.h"
#include "States\GameEnd.h"
#include "States\Victory.h"
#include "States\Credits.h"
#include "States\Options.h"
#include "AenirEngine.h"

#include<stack>



class Client : public Aen::App {
	public:
	~Client() override;
	Client() = delete;
	Client(const Aen::WindowDesc& desc, const std::wstring& windowName, const std::wstring& className);

	void Start() override;
	void Update(const float& deltaTime) override;
	void ChangeState(const States& states);

	private:
	State* mp_state;
	States m_typeState;
	Gameplay* mp_gameplay;
};

Aen::App* Aen::CreateApp() {

	Aen::WindowDesc wDesc;
	wDesc.width = GetSystemMetrics(SM_CXSCREEN) + 4u;
	wDesc.height = GetSystemMetrics(SM_CYSCREEN) + 4u;
	wDesc.EXStyle = AEN_WS_EX_APPWINDOW;
	wDesc.style = AEN_WS_OVERLAPPEDWINDOW | AEN_WS_VISIBLE;
	wDesc.hMenu = 0;
	wDesc.WCStyle = 0;

	return AEN_NEW Client(wDesc, L"Aenir Engine", L"Main_Window");
}

