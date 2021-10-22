#include "Menu.h"

MainMenu::MainMenu(Aen::Window& window)
	:State(window)
{

}

MainMenu::~MainMenu()
{
}

void MainMenu::Update(const float& deltaTime)
{
	if (Aen::Input::KeyDown(Aen::Key::ENTER))
	{
		State::SetState(States::Loadscreen);
	}
}

void MainMenu::Initialize()
{
	m_Window.SetWindowSize(static_cast<UINT>(GetSystemMetrics(SM_CXSCREEN) * 0.4f), static_cast<UINT>(GetSystemMetrics(SM_CYSCREEN) * 0.4f));
	cout << "Main Menu\n";
	cout << "Press Enter to Play\n";
}
