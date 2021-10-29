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
	if (Aen::Input::KeyDown(Aen::Key::ENTER))
	{
		State::SetState(States::Gameplay);
	}
}

void Loadscreen::Initialize()
{
	cout << "Loadstate\n";
	m_Window.SetWindowSize(static_cast<UINT>(GetSystemMetrics(SM_CXSCREEN) * 0.4f), static_cast<UINT>(GetSystemMetrics(SM_CYSCREEN) * 0.4f));
	
	if (!State::GetLoaded())
		State::SetLoad(true);
}
