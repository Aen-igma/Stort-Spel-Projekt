#pragma once
#include "AenirEngine\AenCore.h"
#include <thread>

enum class States { Main_Menu, Loadscreen, Gameplay, Gameover, None};

class State : public std::thread
{
private:
	static States m_currentState;
	static bool m_load;

protected:
	Aen::Window& m_Window;

public:
	State(Aen::Window& window);

	static void SetState(const States& state);
	static void SetLoad(const bool& load);

	States& GetCurrentState()const;
	bool& GetLoaded()const;

	virtual ~State();
	virtual void Update(const float& deltaTime) = 0;
	virtual void Initialize() = 0;
};