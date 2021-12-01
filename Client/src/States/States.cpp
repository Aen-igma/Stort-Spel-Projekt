#include "States.h"

States State::m_currentState{ States::None };
bool State::m_load{ false };
bool State::m_win{ false };

State::State(Aen::Window& window)
	:m_Window(window)
{
}

void State::SetState(const States& state)
{
	m_currentState = state;
}

void State::SetLoad(const bool& load)
{
	m_load = load;
}

void State::SetWin(const bool& load)
{
	m_win = load;
}

States& State::GetCurrentState() const
{
	return this->m_currentState;
}

bool& State::GetLoaded() const
{
	return this->m_load;
}

bool& State::GetWin() const
{
	return this->m_win;
}

State::~State()
{
}