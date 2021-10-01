#pragma once
#include "States.h"

class Gameplay : public State
{
private:

public:

	Gameplay();
	~Gameplay();

	// Inherited via State
	virtual void Update(float time) override;
	virtual void Render(float time) override;
};