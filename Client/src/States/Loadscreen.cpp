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
	if (Aen::Input::KeyDown(Aen::Key::NUMPAD0))
	{
		State::SetState(States::Gameplay);
	}

	//cout << "Loadstate\n";
}

void Loadscreen::Initialize()
{
	cout << "Loadstate\n";
	m_Window.SetWindowSize(static_cast<UINT>(GetSystemMetrics(SM_CXSCREEN) * 0.4f), static_cast<UINT>(GetSystemMetrics(SM_CYSCREEN) * 0.4f));
	
	if (!State::GetLoaded())
		State::SetLoad(true);
}
