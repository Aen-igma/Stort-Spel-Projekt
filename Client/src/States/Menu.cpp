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
		else if (m_UI->GetComponent<Aen::UIComponent>().Intersects(1)) {
			m_Window.Exit();
		}
		else if (m_UI->GetComponent<Aen::UIComponent>().Intersects(3)) {
			State::SetState(States::Credits);
			}
		else if (m_UI->GetComponent<Aen::UIComponent>().Intersects(2)) {
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

#ifdef _DEBUG
	cout << "Main Menu\n";
	cout << "Press Enter to Play\n";
#endif // _DEBUG


	// ----------------------------- UI -------------------------------- //
	m_UI = &Aen::EntityHandler::CreateEntity();
	m_UI->AddComponent<Aen::UIComponent>();
	m_UI->GetComponent<Aen::UIComponent>().AddPicture(AEN_RESOURCE_DIR_W(L"PathToTheTower.png"));
	m_UI->GetComponent<Aen::UIComponent>().SetPicPos((961.f / 1920.f) * wDesc.width, (516.f / 1024.f) * wDesc.height);
	m_UI->GetComponent<Aen::UIComponent>().SetPicSize(wDesc.width, wDesc.height);

	m_UI->GetComponent<Aen::UIComponent>().AddPicture(AEN_RESOURCE_DIR_W(L"Title.png"));
	m_UI->GetComponent<Aen::UIComponent>().SetPicPos((1.f / 2.f) * wDesc.width,(400.f / 1024.f) * wDesc.height);
	m_UI->GetComponent<Aen::UIComponent>().SetPicSize((2.f / 3.f) * (1200.f / 1920.f) * wDesc.width, (2.f / 3.f) * (300.f / 1024.f) * wDesc.height);

	m_UI->GetComponent<Aen::UIComponent>().AddButton(AEN_RESOURCE_DIR_W(L"Play.png")); //0
	m_UI->GetComponent<Aen::UIComponent>().SetButtonPos((3.f / 6.f) * wDesc.width, (680 / 1024.f) * wDesc.height);
	m_UI->GetComponent<Aen::UIComponent>().SetButtonSize((200.f / 1920.f) * wDesc.width, (75.f / 1024.f) * wDesc.height);

	m_UI->GetComponent<Aen::UIComponent>().AddButton(AEN_RESOURCE_DIR_W(L"Quit.png")); //1
	m_UI->GetComponent<Aen::UIComponent>().SetButtonPos((3.f / 6.f) * wDesc.width, (880.f / 1024.f) * wDesc.height);
	m_UI->GetComponent<Aen::UIComponent>().SetButtonSize((200.f / 1920.f) * wDesc.width, (75.f / 1024.f) * wDesc.height);

	m_UI->GetComponent<Aen::UIComponent>().AddButton(AEN_RESOURCE_DIR_W(L"Options.png")); //2
	m_UI->GetComponent<Aen::UIComponent>().SetButtonPos((4.f / 9.f) * wDesc.width, (780.f / 1024.f) * wDesc.height);
	m_UI->GetComponent<Aen::UIComponent>().SetButtonSize((200.f / 1920.f)* wDesc.width, (75.f / 1024.f)* wDesc.height);

	m_UI->GetComponent<Aen::UIComponent>().AddButton(AEN_RESOURCE_DIR_W(L"Credits.png")); //3
	m_UI->GetComponent<Aen::UIComponent>().SetButtonPos((5.f / 9.f) * wDesc.width, (780.f / 1024.f) * wDesc.height);
	m_UI->GetComponent<Aen::UIComponent>().SetButtonSize((200.f / 1920.f) * wDesc.width, (75.f / 1024.f) * wDesc.height);
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
