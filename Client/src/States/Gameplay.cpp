#include "Gameplay.h"

Gameplay::Gameplay(Aen::Window& window)
	:State(window), m_speed(10.f), m_fSpeed(0.15f), m_toggleFullScreen(true), m_hp(200.f),
	IFRAMEMAX(1.5f), m_iFrames(0.f) {}

Gameplay::~Gameplay() {
	//Aen::EntityHandler::RemoveEntity(*m_dLight);
	Aen::EntityHandler::RemoveEntity(*m_plane);
	Aen::EntityHandler::RemoveEntity(*m_reimube1);
	Aen::EntityHandler::RemoveEntity(*m_reimube2);
	Aen::EntityHandler::RemoveEntity(*m_reimube3);
	Aen::EntityHandler::RemoveEntity(*m_reimube4);
	Aen::EntityHandler::RemoveEntity(*m_UI);
	
	for (auto& b : *m_levelImporter.GetEntityList()) {
		Aen::EntityHandler::RemoveEntity(*b);
	}

	for (auto& d : m_enemyQueue) {
		delete d;
	}

	Aen::Resource::RemoveAllMaterials();
	Aen::Resource::RemoveAllMeshes();
	Aen::Resource::RemoveAllTextures();
}

void Gameplay::Initialize()
{
	srand((UINT)time(NULL));
	State::SetLoad(false);
	// -----------------------------	UI	------------------------------- //
	m_UI = &Aen::EntityHandler::CreateEntity();
	m_UI->AddComponent<Aen::UIComponent>();
	m_UI->GetComponent<Aen::UIComponent>().AddPicture(AEN_RESOURCE_DIR_W(L"healthbar.png"), 0);
	m_UI->GetComponent<Aen::UIComponent>().SetPicPos(220.f, 60.f, 0);
	m_UI->GetComponent<Aen::UIComponent>().SetPicSize(m_hp * 2.f, 150.f, 0);

	m_UI->GetComponent<Aen::UIComponent>().AddPicture(AEN_RESOURCE_DIR_W(L"GoalText.png"), 1);
	m_UI->GetComponent<Aen::UIComponent>().SetPicPos(965.f, 100.f, 1);
	m_UI->GetComponent<Aen::UIComponent>().SetPicSize(600.f, 100.f, 1);


	// -------------------------- Particle System ------------------------------- //
	m_PS = &Aen::EntityHandler::CreateEntity();
	//m_PS->AddComponent<Aen::PSSystemcomponent>();
	//m_PS->GetComponent<Aen::PSSystemcomponent>().Initialize(m_device,L"healthbar.png");
	
	// ----------------------------- Setup Camera ------------------------------- //

	// ------------------------ Setup Directional Light ------------------------- //

	/*m_dLight = &Aen::EntityHandler::CreateEntity();
	m_dLight->AddComponent<Aen::DirectionalLight>();
	m_dLight->GetComponent<Aen::DirectionalLight>().SetColor(Aen::Color::White);
	m_dLight->GetComponent<Aen::DirectionalLight>().SetStrength(1.f);
	m_dLight->SetRot(45.f, -135.f, 0.f);*/

	// ----------------------------- Load Meshes -------------------------------- //

	//Aen::Mesh& plane = Aen::Resource::CreateMesh("Plane");
	//plane.Load(AEN_RESOURCE_DIR("Floor_Final.fbx"));
	Aen::Mesh& rimuru = Aen::Resource::CreateMesh("Rimuru");
	rimuru.Load(AEN_RESOURCE_DIR("Slime.fbx"));
	Aen::Mesh& reimube = Aen::Resource::CreateMesh("Reimube");
	reimube.Load(AEN_RESOURCE_DIR("Cube.fbx"));
	//Aen::Mesh& wall = Aen::Resource::CreateMesh("Wall");
	//wall.Load(AEN_RESOURCE_DIR("Wall_Final.fbx"));
	//Aen::Mesh& wallDoor = Aen::Resource::CreateMesh("WallDoor");
	//wallDoor.Load(AEN_RESOURCE_DIR("Wall_Door_Final.fbx"));

	// -------------------------- Setup Material -------------------------------- //

	Aen::Material& planeMat = Aen::Resource::CreateMaterial("PlaneMaterial");
	Aen::Material& enemyMat = Aen::Resource::CreateMaterial("EnemyMaterial");
	Aen::Material& reimubeMat = Aen::Resource::CreateMaterial("ReimubeMat");
	Aen::Material& wallMat = Aen::Resource::CreateMaterial("WallMat");

	enemyMat.LoadeAndSetDiffuseMap(AEN_RESOURCE_DIR("SlimeRimuruFace.png"));
	enemyMat["InnerEdgeColor"] = Aen::Color::Cyan;
	enemyMat["OuterEdgeColor"] = Aen::Color::Cyan;

	wallMat.LoadeAndSetDiffuseMap(AEN_RESOURCE_DIR("Brick_Diffuse.png"));
	wallMat["InnerEdgeColor"] = Aen::Color(0.2f, 0.26f, 0.37f, 1.f);
	wallMat["OuterEdgeColor"] = Aen::Color(0.2f, 0.26f, 0.37f, 1.f);

	reimubeMat.LoadeAndSetDiffuseMap(AEN_RESOURCE_DIR("greenMage.png"));
	reimubeMat["InnerEdgeColor"] = Aen::Color::Pink;
	reimubeMat["OuterEdgeColor"] = Aen::Color::Pink;

	planeMat.LoadeAndSetDiffuseMap(AEN_RESOURCE_DIR("Floor_Diffuse.png"));
	planeMat["InnerEdgeColor"] = Aen::Color(0.2f, 0.26f, 0.37f, 1.f);
	planeMat["OuterEdgeColor"] = Aen::Color(0.2f, 0.26f, 0.37f, 1.f);

	// -------------------------- Setup Entities -------------------------------- //

	/*m_wall = &Aen::EntityHandler::CreateEntity();
	m_wall->AddComponent<Aen::MeshInstance>();
	m_wall->GetComponent<Aen::MeshInstance>().SetMesh(wallDoor);
	m_wall->GetComponent<Aen::MeshInstance>().SetMaterial(wallMat);
	m_wall->SetPos(22.f, 0.f, 0.f);*/

	/*Aen::Entity* wallE = &Aen::EntityHandler::CreateEntity();
	wallE->AddComponent<Aen::MeshInstance>();
	wallE->GetComponent<Aen::MeshInstance>().SetMesh(wall);
	wallE->GetComponent<Aen::MeshInstance>().SetMaterial(wallMat);
	wallE->SetPos(-22.f, 0.f, 0.f);

	wallE = nullptr;

	wallE = &Aen::EntityHandler::CreateEntity();
	wallE->AddComponent<Aen::MeshInstance>();
	wallE->GetComponent<Aen::MeshInstance>().SetMesh(wall);
	wallE->GetComponent<Aen::MeshInstance>().SetMaterial(wallMat);
	wallE->SetPos(0.f, 0.f, 22.f);
	wallE->SetRot(0.f, 90.f, 0.f);

	wallE = nullptr;

	wallE = &Aen::EntityHandler::CreateEntity();
	wallE->AddComponent<Aen::MeshInstance>();
	wallE->GetComponent<Aen::MeshInstance>().SetMesh(wall);
	wallE->GetComponent<Aen::MeshInstance>().SetMaterial(wallMat);
	wallE->SetPos(0.f, 0.f, -22.f);
	wallE->SetRot(0.f, 90.f, 0.f);

	wallE = nullptr;

	wallE = &Aen::EntityHandler::CreateEntity();
	wallE->AddComponent<Aen::MeshInstance>();
	wallE->GetComponent<Aen::MeshInstance>().SetMesh(plane);
	wallE->GetComponent<Aen::MeshInstance>().SetMaterial(planeMat);
	wallE->SetPos(0.f, 22.f, 0.f);

	wallE = nullptr;*/

	m_plane = &Aen::EntityHandler::CreateEntity();
	m_plane->AddComponent<Aen::RigidBody>();
	m_plane->GetComponent<Aen::RigidBody>().SetOffset(0.f, -0.5f, 0.f);
	m_plane->GetComponent<Aen::RigidBody>().SetGeometry(Aen::GeometryType::PLANE);
	//m_plane->GetComponent<Aen::RigidBody>().SetGeometry(Aen::GeometryType::CUBE, Aen::Vec3f(1.f, 44.f, 44.f));
	//m_plane->AddComponent<Aen::MeshInstance>();
	//m_plane->GetComponent<Aen::MeshInstance>().SetMesh(plane);
	//m_plane->GetComponent<Aen::MeshInstance>().SetMaterial(planeMat);

	m_reimube1 = &Aen::EntityHandler::CreateEntity();
	m_reimube1->AddComponent<Aen::RigidBody>();
	m_reimube1->GetComponent<Aen::RigidBody>().SetGeometry(Aen::GeometryType::CUBE, Aen::Vec3f(50.f, 10.f, 2.f));
	/*m_reimube->GetComponent<Aen::RigidBody>().SetRigidType(Aen::RigidType::STATIC);*/
	m_reimube1->AddComponent<Aen::MeshInstance>();
	//m_reimube1->GetComponent<Aen::MeshInstance>().SetMesh(reimube);
	//m_reimube1->GetComponent<Aen::MeshInstance>().SetMaterial(reimubeMat);
	m_reimube1->AddComponent<Aen::AABoundBox>();
	m_reimube1->GetComponent<Aen::AABoundBox>().SetBoundsToMesh();
	m_reimube1->SetPos(0.f, 5.f, 22.f);

	m_reimube2 = &Aen::EntityHandler::CreateEntity();
	m_reimube2->AddComponent<Aen::RigidBody>();
	m_reimube2->GetComponent<Aen::RigidBody>().SetGeometry(Aen::GeometryType::CUBE, Aen::Vec3f(50.f, 10.f, 2.f));
	/*m_reimube->GetComponent<Aen::RigidBody>().SetRigidType(Aen::RigidType::STATIC);*/
	m_reimube2->AddComponent<Aen::MeshInstance>();
	//m_reimube2->GetComponent<Aen::MeshInstance>().SetMesh(reimube);
	//m_reimube2->GetComponent<Aen::MeshInstance>().SetMaterial(reimubeMat);
	m_reimube2->AddComponent<Aen::AABoundBox>();
	m_reimube2->GetComponent<Aen::AABoundBox>().SetBoundsToMesh();
	m_reimube2->SetPos(0.f, 5.f, -154.f);


	m_reimube3 = &Aen::EntityHandler::CreateEntity();
	m_reimube3->AddComponent<Aen::RigidBody>();
	m_reimube3->GetComponent<Aen::RigidBody>().SetGeometry(Aen::GeometryType::CUBE, Aen::Vec3f(2.f, 10.f, 176.f));
	/*m_reimu3e->GetComponent<Aen::RigidBody>().SetRigidType(Aen::RigidType::STATIC);*/
	m_reimube3->AddComponent<Aen::MeshInstance>();
	//m_reimube3->GetComponent<Aen::MeshInstance>().SetMesh(reimube);
	//m_reimube3->GetComponent<Aen::MeshInstance>().SetMaterial(reimubeMat);
	m_reimube3->AddComponent<Aen::AABoundBox>();
	m_reimube3->GetComponent<Aen::AABoundBox>().SetBoundsToMesh();
	m_reimube3->SetPos(22.f, 5.f, -65.f);

	m_reimube4 = &Aen::EntityHandler::CreateEntity();
	m_reimube4->AddComponent<Aen::RigidBody>();
	m_reimube4->GetComponent<Aen::RigidBody>().SetGeometry(Aen::GeometryType::CUBE, Aen::Vec3f(2.f, 10.f, 176.f));
	/*m_reimu4e->GetComponent<Aen::RigidBody>().SetRigidType(Aen::RigidType::STATIC);*/
	m_reimube4->AddComponent<Aen::MeshInstance>();
	//m_reimube4->GetComponent<Aen::MeshInstance>().SetMesh(reimube);
	//m_reimube4->GetComponent<Aen::MeshInstance>().SetMaterial(reimubeMat);
	m_reimube4->AddComponent<Aen::AABoundBox>();
	m_reimube4->GetComponent<Aen::AABoundBox>().SetBoundsToMesh();
	m_reimube4->SetPos(-22.f, 5.f, -65.f);


	// ------ Level Importer ------ //
	std::string path = AEN_LEVEL_DIR("NewTestLevel.Level");
	m_levelImporter.import(path);


	//---------ENEMIES----------//
	int numEnemies = 20;
	int offset = -10;
	Aen::Vec3f enemyPos{0.f, 1.f, -15.f};
	for (int u = 0; u < numEnemies; u++) {
		m_enemyQueue.emplace_back(AEN_NEW Rimuru(enemyPos + Aen::Vec3f((rand() % 38) - 19.f, 0.f, offset)));
		offset -= 5;
	}

	//m_attack->SetParent(*m_player);

	//printf("");

	// --------------------------- Setup Window --------------------------------- //

	m_Window.SetWindowSize(static_cast<UINT>(GetSystemMetrics(SM_CXSCREEN) * 0.4f), static_cast<UINT>(GetSystemMetrics(SM_CYSCREEN) * 0.4f));
	Aen::WindowDesc wDesc;
	wDesc.width = GetSystemMetrics(SM_CXSCREEN) + 4u;
	wDesc.height = GetSystemMetrics(SM_CYSCREEN) + 4u;
	wDesc.EXStyle = AEN_WS_EX_APPWINDOW;
	wDesc.style = AEN_WS_POPUPWINDOW | AEN_WS_VISIBLE;
	m_Window.LoadSettings(wDesc);

	Aen::Input::ToggleRawMouse(true);
	Aen::Input::SetMouseVisible(false);
	cout << "Press Enter To Continue\n";
}

// ---------------------------------------------------------		Update		--------------------------------------------------------------- //

void Gameplay::Update(const float& deltaTime) {

	if (m_hp != m_player.GetHealth()) { //ers�tt collision med enemy i if satsen
		float hp = (m_hp - m_player.GetHealth());

		m_UI->GetComponent<Aen::UIComponent>().LessenPic(hp * 2.f, 0);
		m_hp = m_player.GetHealth();
	}

	if (m_toggleFullScreen)
		Aen::Input::SetMousePos((Aen::Vec2i)Aen::Vec2f(GetSystemMetrics(SM_CXSCREEN) * 0.5f, GetSystemMetrics(SM_CYSCREEN) * 0.5f));
	else
		Aen::Input::SetMousePos(m_Window.GetWindowPos() + (Aen::Vec2i)((Aen::Vec2f)m_Window.GetSize() * 0.5f));

	// ---------------------------------- Enemies --------------------------------------- //

	m_player.Update(m_enemyQueue, deltaTime);

	for(auto& i : m_enemyQueue)
		i->Update(deltaTime, m_player);

	m_player.UpdateAttack(m_enemyQueue, deltaTime);

	if(m_player.GetHealth() <= 0.f)
		State::SetState(States::Gameover);

	if(m_enemyQueue.empty())
		State::SetState(States::Victory);

	#ifdef _DEBUG
		if(Aen::Input::KeyDown(Aen::Key::J))
			m_enemyQueue.emplace_back(AEN_NEW Rimuru());
	#endif


	//if (Aen::Input::KeyDown(Aen::Key::O)) {
	//	delete m_enemyQueue.front();
	//	m_enemyQueue.pop_front();
	//}

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
	/*if (m_hp <= 0 && m_enemyQueue.size() == 0)
	{
		State::SetState(States::Gameover);
	}*/
}