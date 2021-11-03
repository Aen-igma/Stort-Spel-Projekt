#include "GameEnd.h"
GameEnd::GameEnd(Aen::Window& window)
	:State(window), m_speed(10.f), m_fSpeed(0.15f), m_mouseSense(5.f), m_toggleFullScreen(true) {}

GameEnd::~GameEnd()
{
	Aen::EntityHandler::RemoveEntity(*m_UI);
}

void GameEnd::Update(const float& deltaTime)
{
	m_UI->GetComponent<Aen::UIComponent>().Update();

	if (Aen::Input::KeyDown(Aen::Key::LMOUSE))
	{
		if (m_UI->GetComponent<Aen::UIComponent>().Intersects(0)) {
			State::SetState(States::Main_Menu);
		}
		if (m_UI->GetComponent<Aen::UIComponent>().Intersects(1)) {
			m_Window.Exit();
		}
	}

	if (Aen::Input::KeyDown(Aen::Key::F1)) {
		m_toggleFullScreen = !m_toggleFullScreen;
		Aen::WindowDesc wDesc;

		if (m_toggleFullScreen) {
			wDesc.width = GetSystemMetrics(SM_CXSCREEN) + 4u;
			wDesc.height = GetSystemMetrics(SM_CYSCREEN) + 4u;
			wDesc.EXStyle = AEN_WS_EX_APPWINDOW;
			wDesc.style = AEN_WS_POPUPWINDOW | AEN_WS_VISIBLE;
			m_Window.LoadSettings(wDesc);
		}
		else {
			wDesc.width = static_cast<UINT>(GetSystemMetrics(SM_CXSCREEN) * 0.4f);
			wDesc.height = static_cast<UINT>(GetSystemMetrics(SM_CYSCREEN) * 0.4f);
			wDesc.EXStyle = AEN_WS_EX_APPWINDOW;
			wDesc.style = AEN_WS_OVERLAPPEDWINDOW | AEN_WS_VISIBLE;
			m_Window.LoadSettings(wDesc);
		}
	}
}

void GameEnd::Initialize()
{
	// ----------------------------- UI -------------------------------- //
	m_UI = &Aen::EntityHandler::CreateEntity();
	m_UI->AddComponent<Aen::UIComponent>();
	m_UI->GetComponent<Aen::UIComponent>().AddButton(L"../Resource/Menu.png", 0);
	m_UI->GetComponent<Aen::UIComponent>().SetButtonPos(700.f, 800.f, 0);
	m_UI->GetComponent<Aen::UIComponent>().SetButtonSize(300.f, 200.f, 0);
	m_UI->GetComponent<Aen::UIComponent>().SaveButtonData();

	m_UI->GetComponent<Aen::UIComponent>().AddButton(L"../Resource/Quit.png", 1);
	m_UI->GetComponent<Aen::UIComponent>().SetButtonPos(1200.f, 800, 1);
	m_UI->GetComponent<Aen::UIComponent>().SetButtonSize(300.f, 200.f, 1);
	m_UI->GetComponent<Aen::UIComponent>().SaveButtonData();

	m_UI->GetComponent<Aen::UIComponent>().AddPicture(L"../Resource/GameOver.png", 0);
	m_UI->GetComponent<Aen::UIComponent>().SetPicPos(1000.f, 200.f, 0);
	m_UI->GetComponent<Aen::UIComponent>().SetPicSize(1200.f, 300.f, 0);

	//Text
	//m_UI->GetComponent<Aen::UIComponent>().AddText();
	//m_UI->GetComponent<Aen::UIComponent>().SetTextPos(950.f, 200.f);
	//m_UI->GetComponent<Aen::UIComponent>().SetTextSize(300.f, 300.f);


	m_Window.SetWindowSize(static_cast<UINT>(GetSystemMetrics(SM_CXSCREEN) * 0.4f), static_cast<UINT>(GetSystemMetrics(SM_CYSCREEN) * 0.4f));
	Aen::WindowDesc wDesc;
	wDesc.width = GetSystemMetrics(SM_CXSCREEN) + 4u;
	wDesc.height = GetSystemMetrics(SM_CYSCREEN) + 4u;
	wDesc.EXStyle = AEN_WS_EX_APPWINDOW;
	wDesc.style = AEN_WS_POPUPWINDOW | AEN_WS_VISIBLE;
	m_Window.LoadSettings(wDesc);

	cout << "Game End\n";
	cout << "Press Enter to return to Main Menu\n";

	Aen::Input::ToggleRawMouse(true);
	Aen::Input::SetMouseVisible(true);
}
