#include"Client.h"

Client::~Client() {

	//delete mp_state;
}

Client::Client(const Aen::WindowDesc& desc, const std::wstring& windowName, const std::wstring& className)
	:Aen::App(desc, windowName, className)/*, mp_state(nullptr), m_typeState(States::None), mp_gameplay(nullptr), mp_gameEnd(nullptr), mp_victory(nullptr)*/
{}

void Client::Start()
{
	//State::SetState(States::Main_Menu);
	m_cam = &Aen::EntityHandler::CreateEntity();
	m_cam->AddComponent<Aen::Camera>();
	m_cam->GetComponent<Aen::Camera>().SetCameraPerspective(70.f, Aen::GlobalSettings::GetWindow()->GetAspectRatio(), 0.01f, 200.f);

	Aen::GlobalSettings::SetMainCamera(*m_cam);

	m_cam->SetPos(0.f, 0.f, 2.f);

	m_cube = &Aen::EntityHandler::CreateEntity();
	Aen::Mesh& cube = Aen::Resource::CreateMesh("CubeMesh");
	cube.Load(AEN_RESOURCE_DIR("Cube.fbx"));
	Aen::Material& material = Aen::Resource::CreateMaterial("CubeMaterial");
	material.LoadeAndSetDiffuseMap(AEN_RESOURCE_DIR("gunter2.png"));
	m_cube->AddComponent<Aen::MeshInstance>();
	m_cube->GetComponent<Aen::MeshInstance>().SetMesh(cube);
	m_cube->GetComponent<Aen::MeshInstance>().SetMaterial(material);
	Aen::Input::ToggleRawMouse(true);
}

void Client::Update(const float& deltaTime) 
{
	static sm::Vector3 axis;
	axis.x = (float)Aen::Input::KeyPress(Aen::Key::D) - (float)Aen::Input::KeyPress(Aen::Key::A);
	axis.z = (float)Aen::Input::KeyPress(Aen::Key::W) - (float)Aen::Input::KeyPress(Aen::Key::S);

	m_cam->Move(-axis.x * deltaTime, 0.f, -axis.z * deltaTime);

	if (Aen::Input::KeyPress(Aen::Key::H))
	{
		//Aen::MouseEvent me = Aen::Input::ReadEvent();

		//if (me.getInputType() == Aen::MouseEvent::RAW_MOVE)
		//{
			m_cam->Rotate(
				30.f,
				30.f, 0);
			/*m_camera->Rotate(
				-(float)me.GetPos().y * m_mouseSense * deltaTime,
				(float)me.GetPos().x * m_mouseSense * deltaTime, 0.f);
				*/
		//}
	}
	//if (m_typeState != mp_state->GetCurrentState()){

	//	ChangeState(mp_state->GetCurrentState());
	//}

	//if (mp_state)
		//mp_state->Update(deltaTime);

	//if (mp_gameplay->GetLoaded())
	//{
	//	std::thread work(&Gameplay::Initialize, mp_gameplay); //Initialize Gameplay
	//	work.join();
	//}
	if (Aen::Input::KeyDown(Aen::Key::ESCAPE))
		m_window.Exit();
}

//void Client::ChangeState(const States& states)
//{
//	delete mp_state;
//	mp_state = nullptr;
//	
//	switch (states) 
//	{
//		case States::Gameplay:
//			mp_state = AEN_NEW Gameplay(m_window);
//			///mp_gameplay = nullptr;
//			break;
//		case States::Main_Menu:
//			mp_state = AEN_NEW MainMenu(m_window);
//			break;
//		case States::Loadscreen:
//			mp_state = AEN_NEW Loadscreen(m_window);
//			//mp_gameplay = AEN_NEW Gameplay(m_window);
//			break;
//		case States::Gameover:
//			mp_state = AEN_NEW GameEnd(m_window);
//			break;
//		case States::Victory:
//			mp_state = AEN_NEW Victory(m_window);
//			break;
//	}
//
//	//if (mp_state && mp_state->GetCurrentState() != States::Gameplay)
//		mp_state->Initialize();
//
//	m_typeState = states;
//}
