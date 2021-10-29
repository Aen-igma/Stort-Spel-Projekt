#include "GameEnd.h"
GameEnd::GameEnd(Aen::Window& window)
	:State(window), m_speed(10.f), m_fSpeed(0.15f), m_mouseSense(5.f), m_toggleFullScreen(true) {}

GameEnd::~GameEnd()
{
	Aen::EntityHandler::RemoveEntity(*m_UI);
}

void GameEnd::Update(const float& deltaTime)
{
	m_UI->GetComponent<Aen::UIComponent>().Update(m_Window);

	if (Aen::Input::KeyDown(Aen::Key::LMOUSE) && m_UI->GetComponent<Aen::UIComponent>().getBool())
	{
		State::SetState(States::Main_Menu);
	}

	/*if (m_toggleFullScreen)
		Aen::Input::SetMousePos((Aen::Vec2i)Aen::Vec2f(GetSystemMetrics(SM_CXSCREEN) * 0.5f, GetSystemMetrics(SM_CYSCREEN) * 0.5f));
	else
		Aen::Input::SetMousePos(m_Window.GetWindowPos() + (Aen::Vec2i)((Aen::Vec2f)m_Window.GetSize() * 0.5f));*/

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
	m_UI->GetComponent<Aen::UIComponent>().SetButtonPos(950.f, 800.f, 0);
	m_UI->GetComponent<Aen::UIComponent>().SetButtonSize(300.f, 300.f, 0);
	m_UI->GetComponent<Aen::UIComponent>().SaveButtonData();

	//Text
	m_UI->GetComponent<Aen::UIComponent>().AddText();
	m_UI->GetComponent<Aen::UIComponent>().SetTextPos(950.f, 200.f);
	m_UI->GetComponent<Aen::UIComponent>().SetTextSize(300.f, 300);


	m_Window.SetWindowSize(static_cast<UINT>(GetSystemMetrics(SM_CXSCREEN) * 0.4f), static_cast<UINT>(GetSystemMetrics(SM_CYSCREEN) * 0.4f));
	cout << "Game End\n";
	cout << "Press Enter to return to Main Menu\n";

	Aen::Input::ToggleRawMouse(true);
	Aen::Input::SetMouseVisible(true);
}
