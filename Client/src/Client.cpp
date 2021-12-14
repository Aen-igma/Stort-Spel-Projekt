#include"AenirEngine.h"
#include"Client.h"

Client::~Client() {
	
}

Client::Client(const Aen::WindowDesc& desc, const std::wstring& windowName, const std::wstring& className)
	:Aen::App(desc, windowName, className), m_speed(10.f), m_fSpeed(0.15f), m_mouseSense(5.f), m_toggleFullScreen(false) {}

void Client::Start() {

	// ----------------------------- Setup Camera ------------------------------- //

	m_camera = &Aen::EntityHandler::CreateEntity();
	m_camera->AddComponent<Aen::Camera>();
	m_camera->GetComponent<Aen::Camera>().SetCameraPerspective(70.f, m_window.GetAspectRatio(), 0.01f, 100.f);
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
	plane.Load(AEN_MODEL_DIR("Plane.obj"));

	// ----------------------------- Load Reimushes -------------------------------- //

	m_plane = &Aen::EntityHandler::CreateEntity();
	m_plane->AddComponent<Aen::MeshInstance>();
	m_plane->GetComponent<Aen::MeshInstance>().SetMesh(plane);
	m_plane->AddComponent<Aen::RigidBody>();
	m_plane->GetComponent<Aen::RigidBody>().CreateMaterial();
	m_plane->GetComponent<Aen::RigidBody>().CreatePlane();
	m_plane->SetScale(40.f, 1.f, 40.f);

	Aen::GlobalSettings::GetImGuiHandler()->StartUp();

	// --------------------------- Setup Window --------------------------------- //

	m_window.SetWindowSize(static_cast<UINT>(GetSystemMetrics(SM_CXSCREEN) * 0.4f), static_cast<UINT>(GetSystemMetrics(SM_CYSCREEN) * 0.4f));

	// ------------------- Procedural generation testing staging grounds ------- //
	

	Aen::Input::ToggleRawMouse(false);

}

void Client::Update(const float& deltaTime) {

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

		if(m_toggleFullScreen)
			Aen::Input::SetMousePos((Aen::Vec2i)Aen::Vec2f(GetSystemMetrics(SM_CXSCREEN) * 0.5f, GetSystemMetrics(SM_CYSCREEN) * 0.5f));
		else
			Aen::Input::SetMousePos(m_window.GetWindowPos() + (Aen::Vec2i)((Aen::Vec2f)m_window.GetSize() * 0.5f));
	} 

	if(Aen::Input::KeyDown(Aen::Key::RMOUSE)) {
		Aen::Input::SetMouseVisible(false);
		Aen::Input::ToggleRawMouse(true);
	} else if(Aen::Input::KeyUp(Aen::Key::RMOUSE)) {
		Aen::Input::SetMouseVisible(true);
		Aen::Input::ToggleRawMouse(false);
	}

	// ------------------------------ Toggle Fullscreen --------------------------------- //

	if (Aen::Input::KeyDown(Aen::Key::F1)) {
		m_toggleFullScreen = !m_toggleFullScreen;
		Aen::WindowDesc wDesc;

		if (m_toggleFullScreen) {
			wDesc.width = GetSystemMetrics(SM_CXSCREEN) + 4u;
			wDesc.height = GetSystemMetrics(SM_CYSCREEN) + 4u;
			wDesc.EXStyle = AEN_WS_EX_APPWINDOW;
			wDesc.style = AEN_WS_POPUPWINDOW | AEN_WS_VISIBLE;
			m_window.LoadSettings(wDesc);
		}
		else {
			wDesc.width = static_cast<UINT>(GetSystemMetrics(SM_CXSCREEN) * 0.4f);
			wDesc.height = static_cast<UINT>(GetSystemMetrics(SM_CYSCREEN) * 0.4f);
			wDesc.EXStyle = AEN_WS_EX_APPWINDOW;
			wDesc.style = AEN_WS_OVERLAPPEDWINDOW | AEN_WS_VISIBLE;
			m_window.LoadSettings(wDesc);
		}
	}

	// ------------------------------ Quick Exist Button -------------------------------- //

	if (Aen::Input::KeyDown(Aen::Key::ESCAPE))
		m_window.Exit();

	// ---------------------------------------------------------------------------------- //


	

	
}