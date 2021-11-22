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
			m_UI->GetComponent<Aen::UIComponent>().SetPicPos(1100.f, 100.f, 3);
			m_UI->GetComponent<Aen::UIComponent>().SetPicSize(150.f, 150.f);
		}

		if (m_UI->GetComponent<Aen::UIComponent>().Check(2)) {
			Aen::GlobalSettings::SetVSync(false);
			m_UI->GetComponent<Aen::UIComponent>().SetPicPos(1300.f, 100.f, 3);
			m_UI->GetComponent<Aen::UIComponent>().SetPicSize(150.f, 150.f);
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
	m_UI->GetComponent<Aen::UIComponent>().AddPicture(AEN_RESOURCE_DIR_W(L"PathToTheTower.png")); //0
	m_UI->GetComponent<Aen::UIComponent>().SetPicPos(965.f, 520.f);
	m_UI->GetComponent<Aen::UIComponent>().SetPicSize(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));

	m_UI->GetComponent<Aen::UIComponent>().AddPicture(AEN_RESOURCE_DIR_W(L"checkbox.png")); //1
	m_UI->GetComponent<Aen::UIComponent>().SetPicPos(1100.f, 100.f);
	m_UI->GetComponent<Aen::UIComponent>().SetPicSize(50.f, 50.f);

	m_UI->GetComponent<Aen::UIComponent>().AddPicture(AEN_RESOURCE_DIR_W(L"checkbox.png")); //2
	m_UI->GetComponent<Aen::UIComponent>().SetPicPos(1300.f, 100.f);
	m_UI->GetComponent<Aen::UIComponent>().SetPicSize(50.f, 50.f);

	m_UI->GetComponent<Aen::UIComponent>().AddPicture(AEN_RESOURCE_DIR_W(L"check.png")); //3
	if (Aen::GlobalSettings::GetVSync())
		m_UI->GetComponent<Aen::UIComponent>().SetPicPos(1100.f, 100.f);
	else 
		m_UI->GetComponent<Aen::UIComponent>().SetPicPos(1300.f, 100.f);

	m_UI->GetComponent<Aen::UIComponent>().SetPicSize(150.f, 150.f);

	m_UI->GetComponent<Aen::UIComponent>().AddButton(AEN_RESOURCE_DIR_W(L"Back.png")); //0
	m_UI->GetComponent<Aen::UIComponent>().SetButtonPos(400.f, 900);
	m_UI->GetComponent<Aen::UIComponent>().SetButtonSize(400.f, 200.f);
	m_UI->GetComponent<Aen::UIComponent>().SaveButtonData();

	m_UI->GetComponent<Aen::UIComponent>().AddText(L"Vsync", 128.f); //0
	m_UI->GetComponent<Aen::UIComponent>().SetTextPos(800.f, 100.f);
	m_UI->GetComponent<Aen::UIComponent>().SetTextSize(900.f, 300);
	m_UI->GetComponent<Aen::UIComponent>().SetColor(D2D1::ColorF::Black);

	m_UI->GetComponent<Aen::UIComponent>().AddText(L"On", 78.f); //1
	m_UI->GetComponent<Aen::UIComponent>().SetTextPos(1200.f, 100.f);
	m_UI->GetComponent<Aen::UIComponent>().SetTextSize(900.f, 300);
	m_UI->GetComponent<Aen::UIComponent>().SetColor(D2D1::ColorF::Black);

	m_UI->GetComponent<Aen::UIComponent>().AddText(L"Off", 78.f); //2
	m_UI->GetComponent<Aen::UIComponent>().SetTextPos(1400.f, 100.f);
	m_UI->GetComponent<Aen::UIComponent>().SetTextSize(900.f, 300);
	m_UI->GetComponent<Aen::UIComponent>().SetColor(D2D1::ColorF::Black);
}
