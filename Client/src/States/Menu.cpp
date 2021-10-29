#include "Menu.h"

MainMenu::MainMenu(Aen::Window& window)
	:State(window)
{

}

MainMenu::~MainMenu()
{
	Aen::EntityHandler::RemoveEntity(*testUI);
}

void MainMenu::Update(const float& deltaTime)
{
	if (Aen::Input::KeyDown(Aen::Key::ENTER))
	{
		State::SetState(States::Loadscreen);
	}
}

void MainMenu::Initialize()
{
	m_Window.SetWindowSize(static_cast<UINT>(GetSystemMetrics(SM_CXSCREEN) * 0.4f), static_cast<UINT>(GetSystemMetrics(SM_CYSCREEN) * 0.4f));
	cout << "Main Menu\n";
	cout << "Press Enter to Play\n";

	// ----------------------------- UI -------------------------------- //
	testUI = &Aen::EntityHandler::CreateEntity();
	testUI->AddComponent<Aen::UIComponent>();
	testUI->GetComponent<Aen::UIComponent>().AddButton(L"../Resource/Reimu.png", 0);
	testUI->GetComponent<Aen::UIComponent>().SetButtonPos(500.f, 500.f, 0);
	testUI->GetComponent<Aen::UIComponent>().SetButtonSize(300.f, 300.f, 0);

	testUI->GetComponent<Aen::UIComponent>().AddButton(L"../Resource/gunter2.png", 1);
	testUI->GetComponent<Aen::UIComponent>().SetButtonPos(900.f, 500.f, 1);
	testUI->GetComponent<Aen::UIComponent>().SetButtonSize(300.f, 300.f, 1);
	//Text
	testUI->GetComponent<Aen::UIComponent>().AddText(0);
	testUI->GetComponent<Aen::UIComponent>().SetTextPos(500.f,500.f,0);
	testUI->GetComponent<Aen::UIComponent>().SetTextSize(300.f, 300,0);

	testUI->GetComponent<Aen::UIComponent>().AddText(1);
	testUI->GetComponent<Aen::UIComponent>().SetTextPos(500.f, 500.f, 1);
	testUI->GetComponent<Aen::UIComponent>().SetTextSize(300.f, 300, 1);
	//m_cube = &Aen::EntityHandler::CreateEntity();
	//m_cube->AddComponent<Aen::MeshInstance>();
	//m_cube->GetComponent<Aen::MeshInstance>().SetMesh(cube);
	//m_cube->SetPos(0.f, 8.f, 10.f);
	//m_cube->SetScale(20.f, 20.f, 1.f);
}
