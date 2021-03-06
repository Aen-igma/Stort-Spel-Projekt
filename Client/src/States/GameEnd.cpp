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
	m_Window.SetWindowSize(static_cast<UINT>(GetSystemMetrics(SM_CXSCREEN) * 0.4f), static_cast<UINT>(GetSystemMetrics(SM_CYSCREEN) * 0.4f));
	Aen::WindowDesc wDesc;
	wDesc.width = GetSystemMetrics(SM_CXSCREEN) + 4u;
	wDesc.height = GetSystemMetrics(SM_CYSCREEN) + 4u;
	wDesc.EXStyle = AEN_WS_EX_APPWINDOW;
	wDesc.style = AEN_WS_POPUPWINDOW | AEN_WS_VISIBLE;
	m_Window.LoadSettings(wDesc);

	// ----------------------------- UI -------------------------------- //
	m_UI = &Aen::EntityHandler::CreateEntity();
	m_UI->AddComponent<Aen::UIComponent>();
	m_UI->GetComponent<Aen::UIComponent>().AddButton(AEN_TEXTURE_DIR_W(L"Menu.png"));
	m_UI->GetComponent<Aen::UIComponent>().SetButtonPos(700.f, 800.f);
	m_UI->GetComponent<Aen::UIComponent>().SetButtonSize(350.f, 150.f);

	m_UI->GetComponent<Aen::UIComponent>().AddButton(AEN_TEXTURE_DIR_W(L"Quit.png"));
	m_UI->GetComponent<Aen::UIComponent>().SetButtonPos(1200.f, 800);
	m_UI->GetComponent<Aen::UIComponent>().SetButtonSize(350.f, 150.f);
	m_UI->GetComponent<Aen::UIComponent>().SaveButtonData();

	if (!GetWin()) {
		m_UI->GetComponent<Aen::UIComponent>().AddPicture(AEN_TEXTURE_DIR_W(L"GameOver.png"));
	}
	else {
		m_UI->GetComponent<Aen::UIComponent>().AddPicture(AEN_TEXTURE_DIR_W(L"White.png"));
		m_UI->GetComponent<Aen::UIComponent>().SetPicPos((961.f / 1920.f) * wDesc.width, (516.f / 1024.f) * wDesc.height);
		m_UI->GetComponent<Aen::UIComponent>().SetPicSize(wDesc.width, wDesc.height);

		m_UI->GetComponent<Aen::UIComponent>().AddPicture(AEN_TEXTURE_DIR_W(L"Victory.png"));
	}

	m_UI->GetComponent<Aen::UIComponent>().SetPicPos(1000.f, 200.f);
	m_UI->GetComponent<Aen::UIComponent>().SetPicSize(1400.f, 300.f);

	cout << "Game End\n";
	cout << "Press Enter to return to Main Menu\n";

	Aen::Input::ToggleRawMouse(true);
	Aen::Input::SetMouseVisible(true);
}
