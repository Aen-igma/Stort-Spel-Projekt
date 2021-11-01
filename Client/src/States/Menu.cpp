#include "Menu.h"

MainMenu::MainMenu(Aen::Window& window)
	:State(window)
{

}

MainMenu::~MainMenu()
{
	Aen::EntityHandler::RemoveEntity(*m_UI);
}

void MainMenu::Update(const float& deltaTime)
{
	m_UI->GetComponent<Aen::UIComponent>().Update();

	if (Aen::Input::KeyDown(Aen::Key::LMOUSE))
	{
		if (m_UI->GetComponent<Aen::UIComponent>().Intersects(1)) {
			State::SetState(States::Loadscreen);
		}
		if (m_UI->GetComponent<Aen::UIComponent>().Intersects(0)) {
			m_Window.Exit();
		}
	}
}

void MainMenu::Initialize()
{
	m_Window.SetWindowSize(static_cast<UINT>(GetSystemMetrics(SM_CXSCREEN) * 0.4f), static_cast<UINT>(GetSystemMetrics(SM_CYSCREEN) * 0.4f));

	cout << "Main Menu\n";

	// ----------------------------- UI -------------------------------- //
	m_UI = &Aen::EntityHandler::CreateEntity();
	m_UI->AddComponent<Aen::UIComponent>();
	m_UI->GetComponent<Aen::UIComponent>().AddButton(L"../Resource/PathToTheTower.png", 0);
	m_UI->GetComponent<Aen::UIComponent>().SetButtonPos(965.f, 520.f, 0);
	m_UI->GetComponent<Aen::UIComponent>().SetButtonSize(1700.f, 900.f, 0);

	m_UI->GetComponent<Aen::UIComponent>().AddButton(L"../Resource/Play.png", 1);
	m_UI->GetComponent<Aen::UIComponent>().SetButtonPos(950.f, 950, 1);
	m_UI->GetComponent<Aen::UIComponent>().SetButtonSize(300.f, 200.f, 1);
	m_UI->GetComponent<Aen::UIComponent>().SaveButtonData();

	
	//Text
	m_UI->GetComponent<Aen::UIComponent>().AddText();
	m_UI->GetComponent<Aen::UIComponent>().SetTextPos(-100.f,-100.f);
	m_UI->GetComponent<Aen::UIComponent>().SetTextSize(300.f, 300);

	//m_cube = &Aen::EntityHandler::CreateEntity();
	//m_cube->AddComponent<Aen::MeshInstance>();
	//m_cube->GetComponent<Aen::MeshInstance>().SetMesh(cube);
	//m_cube->SetPos(0.f, 8.f, 10.f);
	//m_cube->SetScale(20.f, 20.f, 1.f);
}
