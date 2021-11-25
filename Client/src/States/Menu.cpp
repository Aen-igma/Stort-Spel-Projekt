#include "Menu.h"

MainMenu::MainMenu(Aen::Window& window)
	:State(window), m_UI(nullptr)
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
		if (m_UI->GetComponent<Aen::UIComponent>().Intersects(0)) {
			State::SetState(States::Loadscreen);
		}
		if (m_UI->GetComponent<Aen::UIComponent>().Intersects(1)) {
			m_Window.Exit();
		}
		if (m_UI->GetComponent<Aen::UIComponent>().Intersects(2)) {
			State::SetState(States::Options);
		}
	}
	if (Aen::Input::KeyDown(Aen::Key::ESCAPE))
		m_Window.Exit();
}

void MainMenu::Initialize()
{
	//m_Window.SetWindowSize(static_cast<UINT>(GetSystemMetrics(SM_CXSCREEN) * 0.4f), static_cast<UINT>(GetSystemMetrics(SM_CYSCREEN) * 0.4f)); //Windowed

	//Fullscreen
	Aen::WindowDesc wDesc;
	wDesc.width = GetSystemMetrics(SM_CXSCREEN) + 4u;
	wDesc.height = GetSystemMetrics(SM_CYSCREEN) + 4u;
	wDesc.EXStyle = AEN_WS_EX_APPWINDOW;
	wDesc.style = AEN_WS_POPUPWINDOW | AEN_WS_VISIBLE;
	m_Window.LoadSettings(wDesc);

	// ----------------------------- UI -------------------------------- //
	m_UI = &Aen::EntityHandler::CreateEntity();
	m_UI->AddComponent<Aen::UIComponent>();
	m_UI->GetComponent<Aen::UIComponent>().AddPicture(AEN_RESOURCE_DIR_W(L"PathToTheTower.png"));
	m_UI->GetComponent<Aen::UIComponent>().SetPicPos(965.f, 520.f);
	m_UI->GetComponent<Aen::UIComponent>().SetPicSize(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));

	m_UI->GetComponent<Aen::UIComponent>().AddPicture(AEN_RESOURCE_DIR_W(L"Title.png"));
	m_UI->GetComponent<Aen::UIComponent>().SetPicPos(950.f, 200.f);
	m_UI->GetComponent<Aen::UIComponent>().SetPicSize(1200.f, 300.f);

	m_UI->GetComponent<Aen::UIComponent>().AddButton(AEN_RESOURCE_DIR_W(L"Play.png")); //0
	m_UI->GetComponent<Aen::UIComponent>().SetButtonPos(400.f, 900);
	m_UI->GetComponent<Aen::UIComponent>().SetButtonSize(400.f, 150.f);

	m_UI->GetComponent<Aen::UIComponent>().AddButton(AEN_RESOURCE_DIR_W(L"Quit.png")); //1
	m_UI->GetComponent<Aen::UIComponent>().SetButtonPos(1500.f, 900);
	m_UI->GetComponent<Aen::UIComponent>().SetButtonSize(400.f, 150.f);

	m_UI->GetComponent<Aen::UIComponent>().AddButton(AEN_RESOURCE_DIR_W(L"Options.png")); //2
	m_UI->GetComponent<Aen::UIComponent>().SetButtonPos(950.f, 900);
	m_UI->GetComponent<Aen::UIComponent>().SetButtonSize(400.f, 150.f);
	m_UI->GetComponent<Aen::UIComponent>().SaveButtonData();
	
	//Text
	//m_UI->GetComponent<Aen::UIComponent>().AddText(L"test", 72.f);
	//m_UI->GetComponent<Aen::UIComponent>().SetTextPos(300.f,100.f);
	//m_UI->GetComponent<Aen::UIComponent>().SetTextSize(900.f, 300);

	//m_cube = &Aen::EntityHandler::CreateEntity();
	//m_cube->AddComponent<Aen::MeshInstance>();
	//m_cube->GetComponent<Aen::MeshInstance>().SetMesh(cube);
	//m_cube->SetPos(0.f, 8.f, 10.f);
	//m_cube->SetScale(20.f, 20.f, 1.f);
}
