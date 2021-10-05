#include "States.h"

States State::m_currentState{ States::Gameplay };

State::State(Aen::Window& window)
	:m_Window(window)
{
}

void State::SetState(const States& state)
{
	m_currentState = state;
}

States& State::GetCurrentState() const
{
	return this->m_currentState;
}

State::~State()
{
}