#pragma once
#include "AenirEngine\AenCore.h"

enum class States { Main_Menu, Loadscreen, Gameplay, Gameover, None};

class State {
private:
	static States m_currentState;

protected:
	Aen::Window& m_Window;

public:
	State(Aen::Window& window);

	static void SetState(const States& state);
	States& GetCurrentState()const;

	virtual ~State();
	virtual void Update(const float& deltaTime) = 0;
	virtual void Initialize() = 0;
};