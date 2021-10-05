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
	if (Aen::Input::KeyDown(Aen::Key::NUMPAD0))
	{
		State::SetState(States::Gameplay);
	}

	cout << "Menu\n";
}

void MainMenu::Initialize()
{
}
