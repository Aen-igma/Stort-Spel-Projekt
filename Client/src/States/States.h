#pragma once
#include "PCH\PCH.h"

enum class States { Main_Menu, Loadscreen, Gameplay, Gameover, Ending};

class State {
private:
	States m_currentState;

public:

	void SetState(const States& state);

	virtual ~State();
	virtual void Update(float time) = 0;
	virtual void Render(float time) = 0;
};