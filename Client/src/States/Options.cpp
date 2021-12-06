#include "Options.h"

Options::Options(Aen::Window& window)
	:State(window)
{
	m_UI = nullptr;
}

Options::~Options()
{
	Aen::EntityHandler::RemoveEntity(*m_UI);
}

#define VSYNCHEIGHT 700.f

void Options::Update(const float& deltaTime)
{
	m_UI->GetComponent<Aen::UIComponent>().Update();

	if (Aen::Input::KeyDown(Aen::Key::ESCAPE))
		m_Window.Exit();

	if (Aen::Input::KeyDown(Aen::Key::LMOUSE))
	{
		if (m_UI->GetComponent<Aen::UIComponent>().Intersects(0)) {
			State::SetState(States::Main_Menu);
		}

		if (m_UI->GetComponent<Aen::UIComponent>().Check(1)) {
			Aen::GlobalSettings::SetVSync(true);
			m_UI->GetComponent<Aen::UIComponent>().SetPicPos((3.f / 7.f) * GetSystemMetrics(SM_CXSCREEN), (VSYNCHEIGHT / 1024.f) * GetSystemMetrics(SM_CYSCREEN));
			m_UI->GetComponent<Aen::UIComponent>().SetPicSize((100.f / 1920.f) * GetSystemMetrics(SM_CXSCREEN), (100.f / 1024.f) * GetSystemMetrics(SM_CYSCREEN));
		}

		if (m_UI->GetComponent<Aen::UIComponent>().Check(2)) {
			Aen::GlobalSettings::SetVSync(false);
			m_UI->GetComponent<Aen::UIComponent>().SetPicPos((4.f / 7.f) * GetSystemMetrics(SM_CXSCREEN), (VSYNCHEIGHT / 1024.f) * GetSystemMetrics(SM_CYSCREEN));
			m_UI->GetComponent<Aen::UIComponent>().SetPicSize((100.f / 1920.f) * GetSystemMetrics(SM_CXSCREEN), (100.f / 1024.f) * GetSystemMetrics(SM_CYSCREEN));
		}
	}
}

void Options::Initialize()
{
	//m_Window.SetWindowSize(static_cast<UINT>(GetSystemMetrics(SM_CXSCREEN) * 0.4f), static_cast<UINT>(GetSystemMetrics(SM_CYSCREEN) * 0.4f));
	Aen::WindowDesc wDesc;
	wDesc.width = GetSystemMetrics(SM_CXSCREEN) + 4u;
	wDesc.height = GetSystemMetrics(SM_CYSCREEN) + 4u;
	wDesc.EXStyle = AEN_WS_EX_APPWINDOW;
	wDesc.style = AEN_WS_POPUPWINDOW | AEN_WS_VISIBLE;
	m_Window.LoadSettings(wDesc);

	m_UI = &Aen::EntityHandler::CreateEntity();
	m_UI->AddComponent<Aen::UIComponent>();
	m_UI->GetComponent<Aen::UIComponent>().AddPicture(AEN_TEXTURE_DIR_W(L"MainMenu.png"));
	m_UI->GetComponent<Aen::UIComponent>().SetPicPos((961.f / 1920.f) * wDesc.width, (516.f / 1024.f) * wDesc.height);
	m_UI->GetComponent<Aen::UIComponent>().SetPicSize(wDesc.width, wDesc.height);

	m_UI->GetComponent<Aen::UIComponent>().AddPicture(AEN_TEXTURE_DIR_W(L"checkbox.png")); //1
	m_UI->GetComponent<Aen::UIComponent>().SetPicPos((3.f / 7.f)* wDesc.width, (VSYNCHEIGHT / 1024.f) * wDesc.height);
	m_UI->GetComponent<Aen::UIComponent>().SetPicSize((50.f / 1920.f)* wDesc.width, (50.f / 1024.f)* wDesc.height);

	m_UI->GetComponent<Aen::UIComponent>().AddPicture(AEN_TEXTURE_DIR_W(L"checkbox.png")); //2
	m_UI->GetComponent<Aen::UIComponent>().SetPicPos((4.f / 7.f)* wDesc.width, (VSYNCHEIGHT / 1024.f)* wDesc.height);
	m_UI->GetComponent<Aen::UIComponent>().SetPicSize((50.f / 1920.f)* wDesc.width, (50.f / 1024.f)* wDesc.height);

	m_UI->GetComponent<Aen::UIComponent>().AddPicture(AEN_TEXTURE_DIR_W(L"check.png")); //3
	if (Aen::GlobalSettings::GetVSync())
		m_UI->GetComponent<Aen::UIComponent>().SetPicPos((3.f / 7.f)* wDesc.width, (VSYNCHEIGHT / 1024.f)* wDesc.height);
	else 
		m_UI->GetComponent<Aen::UIComponent>().SetPicPos((2.f / 7.f)* wDesc.width, (VSYNCHEIGHT / 1024.f)* wDesc.height);

	m_UI->GetComponent<Aen::UIComponent>().SetPicSize((100.f / 1920.f)* wDesc.width, (100.f / 1024.f)* wDesc.height);

	m_UI->GetComponent<Aen::UIComponent>().AddButton(AEN_TEXTURE_DIR_W(L"Back.png")); //0
	m_UI->GetComponent<Aen::UIComponent>().SetButtonPos((400.f / 1920.f)* wDesc.width, (900 / 1024.f)* wDesc.height);
	m_UI->GetComponent<Aen::UIComponent>().SetButtonSize((200.f / 1920.f)* wDesc.width, (75.f / 1024.f)* wDesc.height);
	m_UI->GetComponent<Aen::UIComponent>().SaveButtonData();

	m_UI->GetComponent<Aen::UIComponent>().AddText(L"Vsync", 64.f); //0
	m_UI->GetComponent<Aen::UIComponent>().SetTextPos((2.5f / 7.f)* wDesc.width, (VSYNCHEIGHT / 1024.f)* wDesc.height);
	m_UI->GetComponent<Aen::UIComponent>().SetTextSize((900.f / 1920.f)* wDesc.width, (300 / 1024.f)* wDesc.height);
	m_UI->GetComponent<Aen::UIComponent>().SetColor(D2D1::ColorF::Black);

	m_UI->GetComponent<Aen::UIComponent>().AddText(L"On", 39.f); //1
	m_UI->GetComponent<Aen::UIComponent>().SetTextPos((3.2f / 7.f)* wDesc.width, (VSYNCHEIGHT / 1024.f)* wDesc.height);
	m_UI->GetComponent<Aen::UIComponent>().SetTextSize((900.f / 1920.f)* wDesc.width, (300 / 1024.f)* wDesc.height);
	m_UI->GetComponent<Aen::UIComponent>().SetColor(D2D1::ColorF::Black);

	m_UI->GetComponent<Aen::UIComponent>().AddText(L"Off", 39.f); //2
	m_UI->GetComponent<Aen::UIComponent>().SetTextPos((4.2f / 7.f)* wDesc.width, (VSYNCHEIGHT / 1024.f) * wDesc.height);
	m_UI->GetComponent<Aen::UIComponent>().SetTextSize((900.f / 1920.f)* wDesc.width, (300 / 1024.f) * wDesc.height);
	m_UI->GetComponent<Aen::UIComponent>().SetColor(D2D1::ColorF::Black);
}
