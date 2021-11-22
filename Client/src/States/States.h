#pragma once
#include "AenirEngine\AenCore.h"
#include <thread>

enum class States { Main_Menu, Loadscreen, Gameplay, Gameover, Victory, None};

class State
{
private:
	static States m_currentState;
	static bool m_load;
	static bool m_win;

protected:
	Aen::Window& m_Window;

public:
	State(Aen::Window& window);

	static void SetState(const States& state);
	static void SetLoad(const bool& load);
	static void SetWin(const bool& load);

	States& GetCurrentState()const;
	bool& GetLoaded()const;
	bool& GetWin()const;

	virtual ~State();
	virtual void Update(const float& deltaTime) = 0;
	virtual void Initialize() = 0;
};