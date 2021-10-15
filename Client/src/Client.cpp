#include"AenirEngine.h"
#include"Client.h"

Client::~Client() {
	for (UINT i = 0; i < Aen::mapSize * Aen::mapSize; i++) {
		if (rooms[i] != nullptr) {
			delete rooms[i];
			rooms[i] = nullptr;
		}
	}
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

	//m_spotLight.AddComponent<Aen::SpotLight>();
	//m_spotLight.GetComponent<Aen::SpotLight>().SetColor(Aen::Color::Red);
	//m_spotLight.GetComponent<Aen::SpotLight>().SetStrength(1.f);
	//m_spotLight.GetComponent<Aen::SpotLight>().SetConeSize(40.f);
	//m_spotLight.GetComponent<Aen::SpotLight>().SetStrength(500.f);
	//m_spotLight.GetComponent<Aen::SpotLight>().SetLightDist(1.f, 0.f, 0.f, 10.f);
	//m_spotLight.SetPos(0.f, 2.f, -5.f);
	//m_spotLight.SetRot(45.f, 0.f, 0.f);

	// ----------------------------- Load Meshes -------------------------------- //

	Aen::Mesh& plane = Aen::Resource::CreateMesh("Plane");
	Aen::Mesh& cube = Aen::Resource::CreateMesh("Cube");
	//Aen::Mesh& sphere = Aen::Resource::CreateMesh("Sphere");
	plane.Load(AEN_RESOURCE_DIR("Plane.obj"));
	cube.Load(AEN_RESOURCE_DIR("Cube.obj"));
	//sphere.Load(AEN_RESOURCE_DIR("Sphere.obj"));

	// ----------------------------- Load Reimushes -------------------------------- //

	m_ReimuTex = &Aen::Resource::CreateTexture("ReimuTex");
	m_ReimuMat = &Aen::Resource::CreateMaterial("ReimuMat");
	m_reimubeMesh = &Aen::Resource::CreateMesh("Cube");
	m_reimubeMesh->Load(AEN_RESOURCE_DIR("Cube.obj"));
	m_ReimuTex->LoadTexture(AEN_RESOURCE_DIR("Reimu.png"));
	m_ReimuMat->SetDiffuseMap(*m_ReimuTex);

	(*m_ReimuMat)["OuterEdgeColor"] = Aen::Color(0.9f, 0.33f, 0.5f, 1.f);
	(*m_ReimuMat)["InnerEdgeColor"] = Aen::Color(0.9f, 0.33f, 0.5f, 1.f);
	(*m_ReimuMat)["OuterEdgeThickness"] = 0.003f;
	(*m_ReimuMat)["InnerEdgeThickness"] = 0.003f;

	m_meshcube = &cube;
	// -------------------------- Setup Entities -------------------------------- //

	/*m_sphere.AddComponent<Aen::MeshInstance>();
	m_sphere.GetComponent<Aen::MeshInstance>().SetMesh(sphere);*/

	m_plane.AddComponent<Aen::MeshInstance>();
	m_plane.GetComponent<Aen::MeshInstance>().SetMesh(plane);
	m_plane.AddComponent<Aen::RigidBody>();
	m_plane.GetComponent<Aen::RigidBody>().CreateMaterial();
	m_plane.GetComponent<Aen::RigidBody>().CreatePlane();

	m_plane.SetPos(0.f, -30.f, 0.f);
	m_plane.SetScale(40.f, 1.f, 40.f);
	m_plane.SetRot(45, 0, 0);

	Aen::GlobalSettings::GetImGuiHandler()->ReadAllFilesFromResourceFolder();
	Aen::GlobalSettings::GetImGuiHandler()->LoadLevel(0);

	// --------------------------- Setup Window --------------------------------- //

	m_window.SetWindowSize(static_cast<UINT>(GetSystemMetrics(SM_CXSCREEN) * 0.4f), static_cast<UINT>(GetSystemMetrics(SM_CYSCREEN) * 0.4f));

	// ------------------- Procedural generation testing staging grounds ------- //
	
	//LevelGenerator::GenerationTestingFunction();
	/*srand((unsigned int)time(NULL));
	SetLehmerConstSeed(100);
	LehmerInt();
	Aen::Room* map = Aen::LevelGenerator::GenerationTestingFunction();

	for (UINT y = 0; y < Aen::mapSize; y++) {
		for (UINT x = 0; x < Aen::mapSize; x++) {
			rooms[x + y * Aen::mapSize] = nullptr;
		}
	}
	for (UINT y = 0; y < Aen::mapSize; y++) {
		for (UINT x = 0; x < Aen::mapSize; x++) {
			if (map[x + y * Aen::mapSize].m_present) {
				m_buildLevel.CreateRooms(rooms, x, y);
				//rooms[x + y * Aen::mapSize] = new Aen::Entity();
				//rooms[x + y * Aen::mapSize]->AddComponent<Aen::MeshInstance>();
				//rooms[x + y * Aen::mapSize]->GetComponent<Aen::MeshInstance>().SetMesh(*m_meshcube);
				//rooms[x + y * Aen::mapSize]->SetPos(x * 2, 0.f, y * 2);
			}
			else {
				if (rooms[x + y * Aen::mapSize] != nullptr)
					delete rooms[x + y * Aen::mapSize];
				rooms[x + y * Aen::mapSize] = nullptr;
			}
		}
	}*/

	Aen::Input::ToggleRawMouse(false);

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

	if (Aen::Input::KeyDown(Aen::Key::L)) {
		
		SetLehmerConstSeed(LehmerInt());
		Aen::Room* map = Aen::LevelGenerator::GenerationTestingFunction();
		 
		for (UINT y = 0; y < Aen::mapSize; y++) {
			for (UINT x = 0; x < Aen::mapSize; x++) {
				if (map[x + y * Aen::mapSize].m_present) {
					if (rooms[x + y * Aen::mapSize] == nullptr) {
						rooms[x + y * Aen::mapSize] = new Aen::Entity();
						rooms[x + y * Aen::mapSize]->AddComponent<Aen::MeshInstance>();
						rooms[x + y * Aen::mapSize]->GetComponent<Aen::MeshInstance>().SetMesh(*m_meshcube);
						rooms[x + y * Aen::mapSize]->SetPos(x * 2, 0.f, y * 2);
					}
				}
				else {
					if (rooms[x + y * Aen::mapSize] != nullptr){
						delete rooms[x + y * Aen::mapSize];
						rooms[x + y * Aen::mapSize] = nullptr;
					}
				}
			}
		}
	}
	if (Aen::Input::KeyPress(Aen::Key::RMOUSE)) {
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

	// ------------------------------------- Reimubes -------------------------------------- //

	if (Aen::Input::KeyPress(Aen::Key::J)) {
		Aen::Entity* e = AEN_NEW Aen::Entity();
		e->AddComponent<Aen::RigidBody>();
		e->AddComponent<Aen::MeshInstance>();

		e->GetComponent<Aen::MeshInstance>().SetMesh(*m_reimubeMesh);
		e->GetComponent<Aen::MeshInstance>().SetMaterial(*m_ReimuMat);
		e->GetComponent<Aen::RigidBody>().CreateMaterial();
		e->GetComponent<Aen::RigidBody>().CreateCube();
		e->SetPos(0.f, 10.f, 0.f);

		m_reimubes.emplace(m_reimubeCount++, e);
		e = nullptr;
	}

	if (Aen::Input::KeyPress(Aen::Key::K)) {
		for (auto& i : m_reimubes) {
			delete i.second;
			i.second = nullptr;
			m_reimubes.erase(i.first);
			break;
		}
	}
}


void inline levelBuilder::initLevelBuilder()
{
}

bool levelBuilder::CreateRooms(Aen::Entity** storage, uint8_t x, uint8_t y)
{
	static const Aen::Room* map_ptr = Aen::LevelGenerator::GetMapPointer();

	if (storage[x + y * Aen::mapSize] == nullptr) {
		storage[x + y * Aen::mapSize] = new Aen::Entity();
		storage[x + y * Aen::mapSize]->AddComponent<Aen::MeshInstance>();
		storage[x + y * Aen::mapSize]->GetComponent<Aen::MeshInstance>().SetMesh((Aen::Mesh&)map_ptr[x + y * Aen::mapSize].mptr_mesh);
	}
	else {
		storage[x + y * Aen::mapSize]->GetComponent<Aen::MeshInstance>().SetMesh(*((Aen::Mesh*)map_ptr[x + y * Aen::mapSize].mptr_mesh));
	}
	storage[x + y * Aen::mapSize]->SetPos(x * Aen::roomDimension, 0.f, y * Aen::roomDimension);


	return true;
}

levelBuilder::levelBuilder()
{
	Aen::m_mapTheme = Aen::RoomTheme::PLACEHOLDER;

	//Straight corridors
	Aen::Room a;
	a.mptr_mesh = &Aen::Resource::CreateMesh("corridorPlaceholder");
	((Aen::Mesh*)a.mptr_mesh)->Load("corridorPlaceholder.obj");
	a.m_baseChance = 0xf;
	a.connectionDirections = 101;
	a.m_present = true;
	a.m_roomTheme = Aen::m_mapTheme;
	AddRoomToGeneration(&a);


	//90 degree corners
	Aen::Room b;
	b.mptr_mesh = &Aen::Resource::CreateMesh("bendPlaceholder");
	((Aen::Mesh*)b.mptr_mesh)->Load("bendPlaceholder.obj");
	b.m_baseChance = 0xf;
	b.m_present = true;
	b.connectionDirections = 11;
	b.m_roomTheme = Aen::m_mapTheme;
	AddRoomToGeneration(&b);


	//T junction
	Aen::Room c;

	c.mptr_mesh = &Aen::Resource::CreateMesh("twayPlaceholder");
	((Aen::Mesh*)c.mptr_mesh)->Load("twayPlaceholder.obj");
	c.m_baseChance = 0xf;
	c.connectionDirections = 1011;
	c.m_present = true;
	c.m_roomTheme = Aen::m_mapTheme;
	AddRoomToGeneration(&c);


	//4-way junction
	Aen::Room d;
	d.mptr_mesh = &Aen::Resource::CreateMesh("fourwayPlaceholder");
	((Aen::Mesh*)d.mptr_mesh)->Load("fourwayPlaceholder.obj");
	d.m_baseChance = 0xf;
	d.connectionDirections = 1111;
	d.m_present = true;
	d.m_roomTheme = Aen::m_mapTheme;
	AddRoomToGeneration(&d);


	//entrance
	Aen::Room e;
	e.mptr_mesh = &Aen::Resource::CreateMesh("entrancePlaceholder");
	((Aen::Mesh*)e.mptr_mesh)->Load("entrancePlaceholder.obj");
	e.m_baseChance = 0xf;
	e.m_present = true;
	e.m_roomSpecial = Aen::SpecialRoom::ENTRANCE;
	e.m_roomTheme = Aen::m_mapTheme;
	e.connectionDirections = 1;
	AddRoomToGeneration(&e);

	this->initLevelBuilder();
}
