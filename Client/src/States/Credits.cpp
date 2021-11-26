#include "Credits.h"

void Credits::Update(const float& deltaTime)
{
	m_UI->GetComponent<Aen::UIComponent>().Update();

	if (Aen::Input::KeyDown(Aen::Key::LMOUSE) && m_UI->GetComponent<Aen::UIComponent>().Intersects(0))
	{
		State::SetState(States::Main_Menu);
	}

	if (Aen::Input::KeyDown(Aen::Key::ESCAPE))
		m_Window.Exit();
}

void Credits::Initialize()
{
#ifdef _DEBUG
	cout << "Loadstate\n";
#endif // _DEBUG

	m_Window.SetWindowSize(static_cast<UINT>(GetSystemMetrics(SM_CXSCREEN) * 0.4f), static_cast<UINT>(GetSystemMetrics(SM_CYSCREEN) * 0.4f));
	Aen::WindowDesc wDesc;
	wDesc.width = GetSystemMetrics(SM_CXSCREEN) + 4u;
	wDesc.height = GetSystemMetrics(SM_CYSCREEN) + 4u;
	wDesc.EXStyle = AEN_WS_EX_APPWINDOW;
	wDesc.style = AEN_WS_POPUPWINDOW | AEN_WS_VISIBLE;
	m_Window.LoadSettings(wDesc);

	// ----------------------------- UI -------------------------------- //
	// keyboard
	m_UI = &Aen::EntityHandler::CreateEntity();
	m_UI->AddComponent<Aen::UIComponent>();
	m_UI->GetComponent<Aen::UIComponent>().AddPicture(AEN_RESOURCE_DIR_W(L"KeyboardLayout.png"));
	m_UI->GetComponent<Aen::UIComponent>().SetPicPos((965.f / 1920) * wDesc.width, (520.f / 1024) * wDesc.height); //Divide by standard size, mul with wDesc size
	m_UI->GetComponent<Aen::UIComponent>().SetPicSize(1700.f, 900.f);

	// continue
	m_UI->GetComponent<Aen::UIComponent>().AddButton(AEN_RESOURCE_DIR_W(L"Continue.png"));
	m_UI->GetComponent<Aen::UIComponent>().SetButtonPos((950.f / 1920) * wDesc.width, (900.f / 1024) * wDesc.height);
	m_UI->GetComponent<Aen::UIComponent>().SetButtonSize((300.f / 1920) * wDesc.width, (150.f / 1024) * wDesc.height);
	m_UI->GetComponent<Aen::UIComponent>().SaveButtonData();
}

Credits::Credits(Aen::Window& window)
	:State(window)
{
}

Credits::~Credits()
{
	Aen::EntityHandler::RemoveEntity(*m_UI);
}