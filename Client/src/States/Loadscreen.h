#pragma once
#include "States.h"

class Loadscreen : public State
{
private:
	bool m_toggleFullScreen;
public:
	Loadscreen(Aen::Window& window);
	~Loadscreen()override;

	// Inherited via State
	virtual void Update(const float& deltaTime) override;
	virtual void Initialize() override;
};