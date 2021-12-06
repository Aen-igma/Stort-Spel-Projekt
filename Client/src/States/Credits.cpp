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
	m_UI = &Aen::EntityHandler::CreateEntity();
	m_UI->AddComponent<Aen::UIComponent>();
	m_UI->GetComponent<Aen::UIComponent>().AddPicture(AEN_TEXTURE_DIR_W(L"MainMenu.png"));
	m_UI->GetComponent<Aen::UIComponent>().SetPicPos((961.f / 1920.f) * wDesc.width, (516.f / 1024.f) * wDesc.height);
	m_UI->GetComponent<Aen::UIComponent>().SetPicSize(wDesc.width, wDesc.height);

	// continue
	m_UI->GetComponent<Aen::UIComponent>().AddButton(AEN_TEXTURE_DIR_W(L"Back.png"));
	m_UI->GetComponent<Aen::UIComponent>().SetButtonPos((3.f / 6.f) * wDesc.width, (880.f / 1024.f) * wDesc.height);
	m_UI->GetComponent<Aen::UIComponent>().SetButtonSize((200.f / 1920.f) * wDesc.width, (75.f / 1024.f) * wDesc.height);
	m_UI->GetComponent<Aen::UIComponent>().SaveButtonData();

	const wchar_t credits[10][32] = {{L"Credits"}, {L"Adam Levin"}, {L"Andreas Johansson"}, {L"Christian Falk"}, {L"Filip Michael"}, {L"Julian Agonoy"}, {L"Linus Dorell"}, {L"Matthew Pettersson"}, {L"Oskar Nyman"},{L"\nIn collaboration\nwith BTH"} };

	for (int i = 0; i < 10; i++) {

		m_UI->GetComponent<Aen::UIComponent>().AddText(credits[i], 30.f); //1
		m_UI->GetComponent<Aen::UIComponent>().SetTextPos((1.f / 2.f) * wDesc.width, ((200.f + 50.f * i + ((i < 1) * -50.f))/ 1024.f) * wDesc.height);
		m_UI->GetComponent<Aen::UIComponent>().SetTextSize((900.f / 1920.f) * wDesc.width, (300 / 1024.f) * wDesc.height);
		m_UI->GetComponent<Aen::UIComponent>().SetColor(D2D1::ColorF::Black);
	};
}

Credits::Credits(Aen::Window& window)
	:State(window)
{
}

Credits::~Credits()
{
	Aen::EntityHandler::RemoveEntity(*m_UI);
}