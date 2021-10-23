#include "Gameplay.h"

Gameplay::Gameplay(Aen::Window& window)
	:State(window), m_speed(10.f), m_fSpeed(0.15f), m_mouseSense(5.f), m_toggleFullScreen(false)
{
}

Gameplay::~Gameplay() {}

void Gameplay::Initialize()
{
	State::SetLoad(false);
	char q = 219;
	// ----------------------------- Setup Camera ------------------------------- //

	m_camera = &Aen::EntityHandler::CreateEntity();
	m_camera->AddComponent<Aen::Camera>();
	m_camera->GetComponent<Aen::Camera>().SetCameraPerspective(70.f, m_Window.GetAspectRatio(), 0.01f, 100.f);
	m_camera->SetPos(0.f, 2.f, -2.f);

	Aen::GlobalSettings::SetMainCamera(*m_camera);

	// ------------------------ Setup Directional Light ------------------------- //

	m_dLight = &Aen::EntityHandler::CreateEntity();
	m_dLight->AddComponent<Aen::DirectionalLight>();
	m_dLight->GetComponent<Aen::DirectionalLight>().SetColor(Aen::Color::White);
	m_dLight->GetComponent<Aen::DirectionalLight>().SetStrength(1.f);
	m_dLight->SetRot(45.f, -135.f, 0.f);

	// ----------------------------- Load Meshes -------------------------------- //

	Aen::Mesh& plane = Aen::Resource::CreateMesh("Plane");
	plane.Load(AEN_RESOURCE_DIR("Plane.fbx"));
	Aen::Mesh& capsule = Aen::Resource::CreateMesh("Capsule");
	capsule.Load(AEN_RESOURCE_DIR("Capsule.fbx"));

	// -------------------------- Setup Material -------------------------------- //

	Aen::Material& planeMat = Aen::Resource::CreateMaterial("PlaneMaterial");
	Aen::Material& playerMat = Aen::Resource::CreateMaterial("PlayerMaterial");

	planeMat["InnerEdgeColor"] = Aen::Color::Red;
	planeMat["OuterEdgeColor"] = Aen::Color::Red;

	planeMat["BaseColor"] = Aen::Color::White;
	playerMat["BaseColor"] = Aen::Color::White;

	// -------------------------- Setup Entities -------------------------------- //

	m_plane = &Aen::EntityHandler::CreateEntity();
	m_plane->AddComponent<Aen::RigidBody>();
	m_plane->AddComponent<Aen::MeshInstance>();
	m_plane->GetComponent<Aen::MeshInstance>().SetMesh(plane);
	m_plane->GetComponent<Aen::MeshInstance>().SetMaterial(planeMat);
	m_plane->SetScale(0.5f, 0.5f, 0.5f);

	m_player = &Aen::EntityHandler::CreateEntity();
	m_player->AddComponent<Aen::CharacterController>();
	m_player->AddComponent<Aen::MeshInstance>();
	m_player->GetComponent<Aen::MeshInstance>().SetMesh(capsule);
	m_player->GetComponent<Aen::MeshInstance>().SetMaterial(playerMat);
	m_player->SetPos(0.f, 1.f, 0.f);

	// --------------------------- Setup Window --------------------------------- //

	m_Window.SetWindowSize(static_cast<UINT>(GetSystemMetrics(SM_CXSCREEN) * 0.4f), static_cast<UINT>(GetSystemMetrics(SM_CYSCREEN) * 0.4f));

	Aen::Input::ToggleRawMouse(false);
	cout << "Press Enter To Continue\n";
}

void Gameplay::Update(const float& deltaTime)
{
	// --------------------------- Raw Mouse and scroll Input --------------------------- //

	while (!Aen::Input::MouseBufferIsEmbty())
	{
		Aen::MouseEvent me = Aen::Input::ReadEvent();

		if (me.getInputType() == Aen::MouseEvent::RAW_MOVE)
		{
			m_camera->Rotate(
				-(float)me.GetPos().y * m_mouseSense * deltaTime,
				-(float)me.GetPos().x * m_mouseSense * deltaTime, 0.f);
		}
		if (me.getInputType() == Aen::MouseEvent::SCROLL_UP) {
			printf("scroll up\n");

		}
		else if (me.getInputType() == Aen::MouseEvent::SCROLL_DOWN) {
			printf("scroll down\n");

		}
	}

	// ------------------------------ Camera Controler ---------------------------------- //

	Aen::Vec3f axis;
	axis.x = (float)Aen::Input::KeyPress(Aen::Key::D) - (float)Aen::Input::KeyPress(Aen::Key::A);
	axis.y = (float)Aen::Input::KeyPress(Aen::Key::SPACE) - (float)Aen::Input::KeyPress(Aen::Key::LSHIFT);
	axis.z = (float)Aen::Input::KeyPress(Aen::Key::S) - (float)Aen::Input::KeyPress(Aen::Key::W);

	static Aen::Vec2i mouseAxis;

	if (Aen::Input::KeyPress(Aen::Key::RMOUSE)) {
		float focus = (Aen::Input::KeyPress(Aen::Key::LCONTROL)) ? m_fSpeed : 1.f;
		m_camera->MoveRelative(axis.x * deltaTime * m_speed * focus, 0.f, axis.z * deltaTime * m_speed * focus);
		m_camera->Move(0.f, axis.y * deltaTime * m_speed * focus, 0.f);

		if (m_toggleFullScreen)
			Aen::Input::SetMousePos((Aen::Vec2i)Aen::Vec2f(GetSystemMetrics(SM_CXSCREEN) * 0.5f, GetSystemMetrics(SM_CYSCREEN) * 0.5f));
		else
			Aen::Input::SetMousePos(m_Window.GetWindowPos() + (Aen::Vec2i)((Aen::Vec2f)m_Window.GetSize() * 0.5f));
	}

	if (Aen::Input::KeyDown(Aen::Key::RMOUSE)) {
		Aen::Input::SetMouseVisible(false);
		Aen::Input::ToggleRawMouse(true);
	}
	else if (Aen::Input::KeyUp(Aen::Key::RMOUSE)) {
		Aen::Input::SetMouseVisible(true);
		Aen::Input::ToggleRawMouse(false);
	}

	m_player->GetComponent<Aen::CharacterController>().Move(Aen::Vec3f(0.f, -1.f, 0.f), deltaTime);
	m_camera->GetComponent<Aen::Camera>().LookTowards(Aen::Transform(m_camera->GetComponent<Aen::Rotation>().GetTranform(), Aen::Vec3f(0.f, 0.f, -1.f)));

	// ------------------------------ Toggle Fullscreen --------------------------------- //

	if (Aen::Input::KeyDown(Aen::Key::F1)) {
		m_toggleFullScreen = !m_toggleFullScreen;
		Aen::WindowDesc wDesc;

		if (m_toggleFullScreen) {
			wDesc.width = GetSystemMetrics(SM_CXSCREEN) + 4u;
			wDesc.height = GetSystemMetrics(SM_CYSCREEN) + 4u;
			wDesc.EXStyle = AEN_WS_EX_APPWINDOW;
			wDesc.style = AEN_WS_POPUPWINDOW | AEN_WS_VISIBLE;
			m_Window.LoadSettings(wDesc);
		}
		else {
			wDesc.width = static_cast<UINT>(GetSystemMetrics(SM_CXSCREEN) * 0.4f);
			wDesc.height = static_cast<UINT>(GetSystemMetrics(SM_CYSCREEN) * 0.4f);
			wDesc.EXStyle = AEN_WS_EX_APPWINDOW;
			wDesc.style = AEN_WS_OVERLAPPEDWINDOW | AEN_WS_VISIBLE;
			m_Window.LoadSettings(wDesc);
		}
	}

	// ------------------------------ Quick Exist Button -------------------------------- //

	if (Aen::Input::KeyDown(Aen::Key::ESCAPE))
		m_Window.Exit();

	// ------------------------------------- States -------------------------------------- //
	if (Aen::Input::KeyDown(Aen::Key::ENTER))
	{
		State::SetState(States::Main_Menu);
	}

	cout << "Press Enter to Change to Main Menu\n";
}