#include "States.h"

States State::m_currentState{ States::Main_Menu };

State::State()
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
