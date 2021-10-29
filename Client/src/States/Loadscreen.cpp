#include "Loadscreen.h"

Loadscreen::Loadscreen(Aen::Window& window)
	:State(window), m_toggleFullScreen(true)
{
}

Loadscreen::~Loadscreen()
{
}

void Loadscreen::Update(const float& deltaTime)
{
	if (Aen::Input::KeyDown(Aen::Key::ENTER))
	{
		State::SetState(States::Gameplay);
	}

	//cout << "Loadstate\n";
}

void Loadscreen::Initialize()
{
	cout << "Loadstate\n";
	m_Window.SetWindowSize(static_cast<UINT>(GetSystemMetrics(SM_CXSCREEN) * 0.4f), static_cast<UINT>(GetSystemMetrics(SM_CYSCREEN) * 0.4f));
	/*Aen::WindowDesc wDesc;
	wDesc.width = GetSystemMetrics(SM_CXSCREEN) + 4u;
	wDesc.height = GetSystemMetrics(SM_CYSCREEN) + 4u;
	wDesc.EXStyle = AEN_WS_EX_APPWINDOW;
	wDesc.style = AEN_WS_POPUPWINDOW | AEN_WS_VISIBLE;
	m_Window.LoadSettings(wDesc);*/

	if (!State::GetLoaded())
		State::SetLoad(true);
}
