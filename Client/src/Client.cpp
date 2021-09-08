#include"AenirEngine.h"
#include"Client.h"

Client::~Client() {

}

Client::Client(const Aen::WindowDesc& desc, const std::wstring& windowName, const std::wstring& className)
	:Aen::App(desc, windowName, className), m_speed(10.f), m_fSpeed(0.15f), m_mouseSense(50.f), m_toggleCamera(true), m_toggleFullScreen(false) {}

void Client::Start() {

	// ----------------------------- Setup Camera ------------------------------- //

	m_camera.AddComponent<Aen::Camera>();
	m_camera.GetComponent<Aen::Camera>().SetCameraPerspective(100.f, m_window.GetAspectRatio(), 0.01f, 100.f);
	m_camera.SetPos(0.f, 0.f, -2.f);

	Aen::GlobalSettings::SetMainCamera(m_camera);

	// ------------------------ Setup Directional Light ------------------------- //

	m_dLight.AddComponent<Aen::DirectionalLight>();
	m_dLight.GetComponent<Aen::DirectionalLight>().SetColor(Aen::Color::White);
	m_dLight.SetRot(45.f, 135.f, 0.f);

	// ----------------------------- Load Meshes -------------------------------- //

	Aen::Mesh& plane = Aen::Resource::CreateMesh("Plane");
	Aen::Mesh& cube = Aen::Resource::CreateMesh("Cube");
	plane.Load(AEN_RESOURCE_DIR("Plane.obj"));
	cube.Load(AEN_RESOURCE_DIR("Cube.obj"));

	// -------------------------- Setup Entities -------------------------------- //

	m_plane.AddComponent<Aen::MeshInstance>();
	m_plane.GetComponent<Aen::MeshInstance>().SetMesh(plane);
	m_plane.SetPos(0.f, -2.f, 0.f);
	m_plane.SetScale(20.f, 1.f, 20.f);

	m_cube.AddComponent<Aen::MeshInstance>();
	m_cube.GetComponent<Aen::MeshInstance>().SetMesh(cube);

	// --------------------------- Setup Window --------------------------------- //

	m_window.SetWindowSize(static_cast<UINT>(GetSystemMetrics(SM_CXSCREEN) * 0.4f), static_cast<UINT>(GetSystemMetrics(SM_CYSCREEN) * 0.4f));
	Aen::Input::SetMouseVisible(false);
}

void Client::Update(const float& deltaTime) {

	// ------------------------------ Camera Controler ---------------------------------- //

	Aen::Vec3f axis;
	axis.x = (float)Aen::Input::KeyPress(Aen::Key::D) - (float)Aen::Input::KeyPress(Aen::Key::A);
	axis.y = (float)Aen::Input::KeyPress(Aen::Key::SPACE) - (float)Aen::Input::KeyPress(Aen::Key::LSHIFT);
	axis.z = (float)Aen::Input::KeyPress(Aen::Key::W) - (float)Aen::Input::KeyPress(Aen::Key::S);

	
	
	static Aen::Vec2i mouseAxis;
	mouseAxis = Aen::Input::GetRawMouse();
	
	if(Aen::Input::KeyDown(Aen::Key::TAB)) {
		Aen::Input::SetMouseVisible(m_toggleCamera);
		m_toggleCamera = !m_toggleCamera;
	}

	if(m_toggleCamera) {
		float focus = (Aen::Input::KeyPress(Aen::Key::LCONTROL)) ? m_fSpeed : 1.f;
		Aen::Input::SetMousePos(m_window.GetWindowPos() + (Aen::Vec2i)((Aen::Vec2f)m_window.GetSize() * 0.5f));
		m_camera.MoveRelative(axis.x * deltaTime * m_speed * focus, 0.f, axis.z * deltaTime * m_speed * focus);
		m_camera.Move(0.f, axis.y * deltaTime * m_speed * focus, 0.f);
		m_camera.Rotate((float)mouseAxis.y * deltaTime * m_mouseSense, (float)mouseAxis.x * deltaTime * m_mouseSense, 0.f);
	}

	// ------------------------------ Toggle Fullscreen --------------------------------- //

	if(Aen::Input::KeyDown(Aen::Key::F1)) { 
		m_toggleFullScreen = !m_toggleFullScreen;
		Aen::WindowDesc wDesc;

		if(m_toggleFullScreen){
			wDesc.width = GetSystemMetrics(SM_CXSCREEN) + 4u;
			wDesc.height = GetSystemMetrics(SM_CYSCREEN) + 4u;
			wDesc.EXStyle = AEN_WS_EX_APPWINDOW;
			wDesc.style =  AEN_WS_POPUPWINDOW | AEN_WS_VISIBLE;
			m_window.LoadSettings(wDesc);
		} else {
			wDesc.width = static_cast<UINT>(GetSystemMetrics(SM_CXSCREEN) * 0.4f);
			wDesc.height = static_cast<UINT>(GetSystemMetrics(SM_CYSCREEN) * 0.4f);
			wDesc.EXStyle = AEN_WS_EX_APPWINDOW;
			wDesc.style = AEN_WS_OVERLAPPEDWINDOW | AEN_WS_VISIBLE;
			m_window.LoadSettings(wDesc);
		}
	}

	// ------------------------------ Quick Exist Button -------------------------------- //

	if(Aen::Input::KeyDown(Aen::Key::ESCAPE))
		m_window.Exit();
}
