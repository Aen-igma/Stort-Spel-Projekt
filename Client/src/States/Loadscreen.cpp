#include "Loadscreen.h"

Loadscreen::Loadscreen(Aen::Window& window)
	:State(window)
{
}

Loadscreen::~Loadscreen()
{
}

void Loadscreen::Update(const float& deltaTime)
{
	if (Aen::Input::KeyDown(Aen::Key::NUMPAD0) && State::GetLoaded())
	{
		State::SetState(States::Gameplay);
	}
}

void Loadscreen::Initialize()
{
	m_Window.SetWindowSize(static_cast<UINT>(GetSystemMetrics(SM_CXSCREEN) * 0.4f), static_cast<UINT>(GetSystemMetrics(SM_CYSCREEN) * 0.4f));
	cout << "Press To Continue\n";
}
