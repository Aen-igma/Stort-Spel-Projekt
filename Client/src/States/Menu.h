#pragma once
#include "States.h"
#include <iostream>
using namespace std;

class MainMenu : public State
{
private:

public:
	MainMenu(Aen::Window& window);
	~MainMenu() override;

	// Inherited via State
	virtual void Update(const float& deltaTime) override;
	virtual void Initialize() override;
};