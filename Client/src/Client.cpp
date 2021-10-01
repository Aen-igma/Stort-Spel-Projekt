#include"AenirEngine.h"
#include"Client.h"
#include"Level/LevelGenerator.h"

Client::~Client() {

}

Client::Client(const Aen::WindowDesc& desc, const std::wstring& windowName, const std::wstring& className)
	:Aen::App(desc, windowName, className), m_speed(10.f), m_fSpeed(0.15f), m_mouseSense(5.f), m_toggleFullScreen(false) {}

void Client::Start() {

	// ----------------------------- Setup Camera ------------------------------- //

	m_camera.AddComponent<Aen::Camera>();
	m_camera.GetComponent<Aen::Camera>().SetCameraPerspective(70.f, m_window.GetAspectRatio(), 0.01f, 100.f);
	m_camera.SetPos(0.f, 0.f, -2.f);

	Aen::GlobalSettings::SetMainCamera(m_camera);

	// ------------------------ Setup Directional Light ------------------------- //

	m_dLight.AddComponent<Aen::DirectionalLight>();
	m_dLight.GetComponent<Aen::DirectionalLight>().SetColor(Aen::Color::White);
	m_dLight.GetComponent<Aen::DirectionalLight>().SetStrength(1.f);
	m_dLight.SetRot(45.f, -135.f, 0.f);

	// --------------------------- Setup Spot Light ----------------------------- //

	m_spotLight.AddComponent<Aen::SpotLight>();
	m_spotLight.GetComponent<Aen::SpotLight>().SetColor(Aen::Color::Red);
	m_spotLight.GetComponent<Aen::SpotLight>().SetStrength(1.f);
	m_spotLight.GetComponent<Aen::SpotLight>().SetConeSize(40.f);
	m_spotLight.GetComponent<Aen::SpotLight>().SetStrength(500.f);
	m_spotLight.GetComponent<Aen::SpotLight>().SetLightDist(1.f, 0.f, 0.f, 10.f);
	m_spotLight.SetPos(0.f, 2.f, -5.f);
	m_spotLight.SetRot(45.f, 0.f, 0.f);

	// ----------------------------- Load Meshes -------------------------------- //

	Aen::Mesh& plane = Aen::Resource::CreateMesh("Plane");
	Aen::Mesh& cube = Aen::Resource::CreateMesh("Cube");
	Aen::Mesh& sphere = Aen::Resource::CreateMesh("Sphere");
	plane.Load(AEN_RESOURCE_DIR("Plane.obj"));
	cube.Load(AEN_RESOURCE_DIR("Cube.obj"));
	sphere.Load(AEN_RESOURCE_DIR("Sphere.obj"));

	// -------------------------- Setup Entities -------------------------------- //

	m_sphere.AddComponent<Aen::MeshInstance>();
	m_sphere.GetComponent<Aen::MeshInstance>().SetMesh(sphere);

	m_plane.AddComponent<Aen::MeshInstance>();
	m_plane.GetComponent<Aen::MeshInstance>().SetMesh(plane);
	m_plane.SetPos(0.f, -2.f, 0.f);
	m_plane.SetScale(40.f, 1.f, 40.f);

	/*m_plane1.AddComponent<Aen::MeshInstance>();
	m_plane1.GetComponent<Aen::MeshInstance>().SetMesh(plane);
	m_plane1.SetPos(0.f, 8.f, -10.f);
	m_plane1.SetScale(20.f, 1.f, 20.f);
	m_plane1.SetRot(90.f, 0.f, 0.f);*/

	m_cube.AddComponent<Aen::MeshInstance>();
	m_cube.GetComponent<Aen::MeshInstance>().SetMesh(cube);
	m_cube.SetPos(0.f, 8.f, 10.f);
	m_cube.SetScale(20.f, 20.f, 1.f);

	Aen::GlobalSettings::GetImGuiHandler()->StartUp();
	// --------------------------- Setup Window --------------------------------- //

	m_window.SetWindowSize(static_cast<UINT>(GetSystemMetrics(SM_CXSCREEN) * 0.4f), static_cast<UINT>(GetSystemMetrics(SM_CYSCREEN) * 0.4f));

	// ------------------- Procedural generation testing staging grounds ------- //

	LevelGenerator::GenerationTestingFunction();
	Aen::Input::ToggleRawMouse(false);

	srand((unsigned int)time(NULL));
}

void Client::Update(const float& deltaTime) {

	// --------------------------- Raw Mouse and scroll Input --------------------------- //

	while (!Aen::Input::MouseBufferIsEmbty())
	{
		Aen::MouseEvent me = Aen::Input::ReadEvent();

		if (me.getInputType() == Aen::MouseEvent::RAW_MOVE)
		{
			m_camera.Rotate(
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

	if(Aen::Input::KeyPress(Aen::Key::RMOUSE)) {
		float focus = (Aen::Input::KeyPress(Aen::Key::LCONTROL)) ? m_fSpeed : 1.f;
		m_camera.MoveRelative(axis.x * deltaTime * m_speed * focus, 0.f, axis.z * deltaTime * m_speed * focus);
		m_camera.Move(0.f, axis.y * deltaTime * m_speed * focus, 0.f);

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

	if(Aen::Input::KeyDown(Aen::Key::G)) {
		Aen::Entity* e = AEN_NEW Aen::Entity;

		int r = rand() % 6;
		Aen::Color clr;
		switch(r) {
			case 0: clr = Aen::Color::Red;
			break;
			case 1: clr = Aen::Color::Green;
			break;
			case 2: clr = Aen::Color::Blue;
			break;
			case 3: clr = Aen::Color::Magenta;
			break;
			case 4: clr = Aen::Color::Yellow;
			break;
			case 5: clr = Aen::Color::Cyan;
			break;
		}

		e->AddComponent<Aen::PointLight>();
		e->GetComponent<Aen::PointLight>().SetColor(clr);
		e->GetComponent<Aen::PointLight>().SetLightDist(1.f, 1.f, 1.f, 5.f);
		e->GetComponent<Aen::PointLight>().SetStrength(100.f);
		e->SetPos(rand() % 20 - 10, 1.f / (rand() % 100) + 0.5f, rand() % 20 - 20);
		m_pLights.emplace(ind++, e);
		e = nullptr;
	}

	if(Aen::Input::KeyPress(Aen::Key::H)) {
		for(auto i : m_pLights) {
			int index = i.first;
			delete i.second;
			i.second = nullptr;
			m_pLights.erase(index);
			break;
		}
	}

	//for(auto i : m_pLights) {
	//	Aen::Vec4f l = i.second->GetComponent<Aen::PointLight>().GetDist();
	//	Aen::Vec3f p = i.second->GetPos();
	//	//p = Aen::Transform(m_camera.GetComponent<Aen::Camera>().GetView(), p);
	//	m_sphere.SetPos(p);
	//	m_sphere.SetScale(l.w, l.w, l.w);
	//	break;
	//}
}
