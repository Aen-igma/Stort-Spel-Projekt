#pragma once
#include "States\Gameplay.h"
#include "States\Menu.h"
#include "States\Loadscreen.h"
#include "AenirEngine.h"

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
	thread work;

	Gameplay* gameplay;
};

Aen::App* Aen::CreateApp() {

	Aen::WindowDesc wDesc;
	wDesc.width = GetSystemMetrics(SM_CXSCREEN) + 4u;
	wDesc.height = GetSystemMetrics(SM_CYSCREEN) + 4u;
	wDesc.EXStyle = AEN_WS_EX_APPWINDOW;
	wDesc.style = AEN_WS_OVERLAPPEDWINDOW | AEN_WS_VISIBLE;
	wDesc.hMenu = 0;
	wDesc.WCStyle = 0;

	return new Client(wDesc, L"Aenir Engine", L"Main_Window");
}


class levelBuilder : public LevelGenerator {

	static bool CreateRooms(std::vector<Aen::Entity>* storage, uint8_t x, uint8_t y);

	public:
};