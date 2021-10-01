#pragma once
#include "PCH\PCH.h"

enum class States { Main_Menu, Loadscreen, Gameplay, Gameover, Ending};

class State {
private:
	static States m_currentState;

public:

	State();
	static void SetState(const States& state);
	States& GetCurrentState()const;

	virtual ~State();
	virtual void Update(float time) = 0;
	virtual void Initialize() = 0;
};