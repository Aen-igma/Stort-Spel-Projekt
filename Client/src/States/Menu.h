#pragma once
#include "States.h"
#include <iostream>
using namespace std;

class MainMenu : public State
{
private:
	Aen::Entity* testUI;
	//Aen::Entity* m_cube;

public:
	MainMenu(Aen::Window& window);
	~MainMenu() override;

	// Inherited via State
	void Update(const float& deltaTime) override;
	void Initialize() override;
};