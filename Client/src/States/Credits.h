#pragma once
#include "States.h"

class Credits : public State
{
	Aen::Entity* m_UI;

	public:
	// Inherited via State
	virtual void Update(const float& deltaTime) override;
	virtual void Initialize() override;

	Credits(Aen::Window& window);
	~Credits();
};

