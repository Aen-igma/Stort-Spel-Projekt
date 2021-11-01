#include"Client.h"

Client::~Client() {

	delete mp_state;
}

Client::Client(const Aen::WindowDesc& desc, const std::wstring& windowName, const std::wstring& className)
	:Aen::App(desc, windowName, className), mp_state(nullptr), m_typeState(States::None), mp_gameplay(nullptr), mp_gameEnd(nullptr)
{}

void Client::Start()
{
	State::SetState(States::Main_Menu);
}

void Client::Update(const float& deltaTime) 
{
	if (m_typeState != mp_state->GetCurrentState()){

		ChangeState(mp_state->GetCurrentState());
	}

	if (mp_state)
		mp_state->Update(deltaTime);

	//if (mp_gameplay->GetLoaded())
	//{
	//	std::thread work(&Gameplay::Initialize, mp_gameplay); //Initialize Gameplay
	//	work.join();
	//}
}

void Client::ChangeState(const States& states)
{
	delete mp_state;
	mp_state = nullptr;
	
	switch (states) 
	{
		case States::Gameplay:
			mp_state = AEN_NEW Gameplay(m_window);
			///mp_gameplay = nullptr;
			break;
		case States::Main_Menu:
			mp_state = AEN_NEW MainMenu(m_window);
			break;
		case States::Loadscreen:
			mp_state = AEN_NEW Loadscreen(m_window);
			mp_gameplay = AEN_NEW Gameplay(m_window);
			break;
		case States::Gameover:
			mp_state = AEN_NEW GameEnd(m_window);
			break;
	}

	//if (mp_state && mp_state->GetCurrentState() != States::Gameplay)
		mp_state->Initialize();

	m_typeState = states;
}
