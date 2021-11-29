#include "Gameplay.h"

Gameplay::Gameplay(Aen::Window& window)
	:State(window), m_speed(10.f), m_fSpeed(0.15f), m_toggleFullScreen(true), m_hp(200.f), m_timer(0),m_deathTimer(0),
	IFRAMEMAX(1.5f), m_iFrames(0.f) {}

Gameplay::~Gameplay() {
	//Aen::EntityHandler::RemoveEntity(*m_dLight);
	Aen::EntityHandler::RemoveEntity(*m_plane);
	Aen::EntityHandler::RemoveEntity(*m_reimube1);
	Aen::EntityHandler::RemoveEntity(*m_UI);
	//Aen::EntityHandler::RemoveEntity(*m_wall);
	
	/*for (auto& b : *m_levelImporter.GetEntityList()) {
		Aen::EntityHandler::RemoveEntity(*b);
	}*/

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
	rimuru.Load(AEN_MODEL_DIR("Slime.fbx"));
	Aen::Mesh& reimube = Aen::Resource::CreateMesh("Reimube");
	reimube.Load(AEN_MODEL_DIR("Cube.fbx"));
	//Aen::Mesh& wall = Aen::Resource::CreateMesh("Wall");
	//wall.Load(AEN_RESOURCE_DIR("Wall_Final.fbx"));
	//Aen::Mesh& wallDoor = Aen::Resource::CreateMesh("WallDoor");
	//wallDoor.Load(AEN_RESOURCE_DIR("Wall_Door_Final.fbx"));

	// -------------------------- Setup Material -------------------------------- //

	Aen::Material& planeMat = Aen::Resource::CreateMaterial("PlaneMaterial");
	Aen::Material& enemyMat = Aen::Resource::CreateMaterial("EnemyMaterial");
	Aen::Material& enemyMatHurt = Aen::Resource::CreateMaterial("EnemyMaterialHurt");
	Aen::Material& reimubeMat = Aen::Resource::CreateMaterial("ReimubeMat");

	enemyMat.LoadeAndSetDiffuseMap(AEN_TEXTURE_DIR("SlimeRimuruFace.png"));
	enemyMat["InnerEdgeColor"] = Aen::Color::Cyan;
	enemyMat["OuterEdgeColor"] = Aen::Color::Cyan;
	enemyMat["BaseColor"] = Aen::Color::Cyan;
	// Material to switch to when enemy is hurt
	enemyMatHurt["BaseColor"] = Aen::Color::Red;

	reimubeMat.LoadeAndSetDiffuseMap(AEN_TEXTURE_DIR("greenMage.png"));
	reimubeMat["InnerEdgeColor"] = Aen::Color::Pink;
	reimubeMat["OuterEdgeColor"] = Aen::Color::Pink;

	planeMat.LoadeAndSetDiffuseMap(AEN_TEXTURE_DIR("Floor_Diffuse.png"));
	planeMat["InnerEdgeColor"] = Aen::Color(0.2f, 0.26f, 0.37f, 1.f);
	planeMat["OuterEdgeColor"] = Aen::Color(0.2f, 0.26f, 0.37f, 1.f);

	//targetMat["InnerEdgeColor"] = Aen::Color::Red;
	//targetMat["OuterEdgeColor"] = Aen::Color::Green;

	// -------------------------- Setup Entities -------------------------------- //

	m_plane = &Aen::EntityHandler::CreateEntity();
	m_plane->AddComponent<Aen::StaticBody>();
	m_plane->GetComponent<Aen::StaticBody>().SetGeometry(Aen::StaticGeometryType::PLANE);


	//m_plane->GetComponent<Aen::StaticBody>().SetGeometry(Aen::StaticGeometryType::CUBE, Aen::Vec3f(1.f, 44.f, 44.f));
	//m_plane->AddComponent<Aen::MeshInstance>();
	//m_plane->GetComponent<Aen::MeshInstance>().SetMesh(plane);
	//m_plane->GetComponent<Aen::MeshInstance>().SetMaterial(planeMat);

	m_reimube1 = &Aen::EntityHandler::CreateEntity();
	m_reimube1->AddComponent<Aen::MeshInstance>();
	m_reimube1->GetComponent<Aen::MeshInstance>().SetMesh(reimube);
	m_reimube1->GetComponent<Aen::MeshInstance>().SetMaterial(enemyMat);
	//m_reimube1->AddComponent<Aen::StaticBody>();
	//m_reimube1->GetComponent<Aen::StaticBody>().SetBoundsToMesh(true);
	m_reimube1->SetPos(0.f, 1.f, 11.f);
	m_reimube1->SetRot(10, 1, 1);
	//m_reimube1->SetRenderLayer(1);

	// ------ Level Importer ------ //
	//std::string path = AEN_LEVEL_DIR("NewTestLevel.Level");
	//m_levelImporter.import(path);

	// ------------------- Procedural generation testing staging grounds ------- //
	//std::vector<string> levelPaths;

	m_levelGenerator.LoadMutipleRoomFiles();


	m_levelGenerator.AddLoadedToGeneration();

	m_levelGenerator.SetMapTheme(Aen::RoomTheme::GENERIC);

	//Match this value to the size of the rooms we are using
	m_levelGenerator.SetRoomDimension(43.f);
	mptr_map = m_levelGenerator.GenerateLevel();
	m_levelGenerator.GenerationTestingFunction();
	m_levelGenerator.CleanMap();

	//Use this value to set the start of the player / origin of the map
	Aen::Vec3f playerStartPos;
	Aen::Vec3f ChestPos;
	Aen::Vec3f DoorPos;
	Aen::Vec3f EnemyPos;
	int roomNormal = 0;

	for (UINT y = 0; y < Aen::mapSize; y++) {
		for (UINT x = 0; x < Aen::mapSize; x++) {
			m_levelGenerator.SpawnRoom(Aen::Vec2i(x, y));

			if (mptr_map[y * Aen::mapSize + x].m_roomSpecial == Aen::SpecialRoom::ENTRANCE) {
				m_levelGenerator.GetRoomPos(x, y, &playerStartPos.x, &playerStartPos.z);
			}
			mptr_map[x + y * Aen::mapSize].mptr_parent;

			if (mptr_map[y * Aen::mapSize + x].m_roomSpecial == Aen::SpecialRoom::ITEM) {
				m_levelGenerator.GetRoomPos(x, y, &ChestPos.x, &ChestPos.z);
				m_levelGenerator.GetRoomPos(x, y, &DoorPos.x, &DoorPos.z);
				roomNormal = mptr_map[y * Aen::mapSize + x].connectionDirections;
			}

			if (mptr_map[y * Aen::mapSize + x].m_roomSpecial == Aen::SpecialRoom::NONE && mptr_map[y * Aen::mapSize + x].m_present) {
				m_levelGenerator.GetRoomPos(x, y, &EnemyPos.x, &EnemyPos.z);
				m_enemyQueue.emplace_back(AEN_NEW Rimuru(EnemyPos));
			}

			if (mptr_map[y * Aen::mapSize + x].m_roomSpecial == Aen::SpecialRoom::BOSS) {

				int index = m_enemyQueue.size();
				m_levelGenerator.GetRoomPos(x, y, &EnemyPos.x, &EnemyPos.z);
				m_enemyQueue.emplace_back(AEN_NEW Rimuru(EnemyPos));
			}
		}
	}
	m_chest.GetEntity()->SetPos(ChestPos);
	m_player.GetEntity()->SetPos(ChestPos.x + 10.f, ChestPos.y + 5.f, ChestPos.z);
	m_chest.SetType(Type::Open);
	m_door.SetType(Type::Open);

	if (roomNormal == 1) { //north
		m_door.GetEntity()->SetRot(0, 180, 0);
	}
	else if (roomNormal == 10) {//east
		m_door.GetEntity()->SetRot(0, 90, 0);
	}
	else if (roomNormal == 100) {//south
		m_door.GetEntity()->SetRot(0, 0, 0);
	}
	else if (roomNormal == 1000) {//west
		m_door.GetEntity()->SetRot(0, -90, 0);
	}
	m_door.GetEntity()->SetPos(DoorPos.x, 3.2f, DoorPos.z);
	m_door.GetEntity()->MoveRelative(0.f, 0, 21.5f);

	//m_attack->SetParent(*m_player);

	//printf("");

	//std::vector<Aen::Vec3f> tempEnemies = m_levelGenerator.GetHandlerPtr()->GetEnemyPos();
	//for (size_t i = 0; i < m_levelGenerator.GetHandlerPtr()->GetEnemyPos().size(); i++)
	//{
	//	m_enemyQueue.emplace_back(AEN_NEW Rimuru(tempEnemies[i]));
	//}
	//m_enemyQueue.emplace_back(AEN_NEW Rimuru(Aen::Vec3f(0,0,0)));


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
	screenSize.x = wDesc.width;
	screenSize.y = wDesc.height;

	// -----------------------------	UI	------------------------------- //
	m_UI = &Aen::EntityHandler::CreateEntity();
	m_UI->AddComponent<Aen::UIComponent>();
	m_UI->GetComponent<Aen::UIComponent>().AddPicture(AEN_RESOURCE_DIR_W(L"healthbar.png")); //0
	m_UI->GetComponent<Aen::UIComponent>().SetPicPos((350.f / 1920)* wDesc.width, (100.f / 1024) * wDesc.height);
	m_UI->GetComponent<Aen::UIComponent>().SetPicSize(((m_hp * 2.f) / 1920) * wDesc.width, (150.f / 1024) * wDesc.height);

	m_UI->GetComponent<Aen::UIComponent>().AddPicture(AEN_RESOURCE_DIR_W(L"bar.png")); //1
	m_UI->GetComponent<Aen::UIComponent>().SetPicPos((350.f / 1920) * wDesc.width, (100.f / 1024) * wDesc.height);
	m_UI->GetComponent<Aen::UIComponent>().SetPicSize(((m_hp * 2.f) / 1920) * wDesc.width, (150.f / 1024) * wDesc.height);

	m_UI->GetComponent<Aen::UIComponent>().AddPicture(AEN_RESOURCE_DIR_W(L"potion.png")); //2
	m_UI->GetComponent<Aen::UIComponent>().SetPicPos((125.f / 1920) * wDesc.width, (100.f / 1024) * wDesc.height);
	m_UI->GetComponent<Aen::UIComponent>().SetPicSize((150.f / 1920) * wDesc.width, (150.f / 1024) * wDesc.height);

	//m_UI->GetComponent<Aen::UIComponent>().AddPicture(AEN_RESOURCE_DIR_W(L"GoalText.png"), 1);
	//m_UI->GetComponent<Aen::UIComponent>().SetPicPos(965.f, 100.f, 1);
	//m_UI->GetComponent<Aen::UIComponent>().SetPicSize(600.f, 100.f, 1);

	m_UI->GetComponent<Aen::UIComponent>().AddText(L"Kill All Enemies", 72.f); //0
	m_UI->GetComponent<Aen::UIComponent>().SetTextPos((965.f / 1920) * wDesc.width, (100.f / 1024) * wDesc.height);
	m_UI->GetComponent<Aen::UIComponent>().SetTextSize((900.f / 1920) * wDesc.width, (300 / 1024) * wDesc.height);

	m_UI->GetComponent<Aen::UIComponent>().AddText(L"", 50.f); //1 - Amount of potion
	m_UI->GetComponent<Aen::UIComponent>().SetTextPos((120.f / 1920) * wDesc.width, (110.f / 1024) * wDesc.height);
	m_UI->GetComponent<Aen::UIComponent>().SetTextSize((150.f / 1920) * wDesc.width, (150.f / 1024) * wDesc.height);
	m_UI->GetComponent<Aen::UIComponent>().SetColor(D2D1::ColorF::Black);

	m_UI->GetComponent<Aen::UIComponent>().AddText(L"Interact (F)", 60.f); //2
	m_UI->GetComponent<Aen::UIComponent>().SetTextPos(965.f, 800.f);
	m_UI->GetComponent<Aen::UIComponent>().SetTextSize(900.f, 300);
	m_UI->GetComponent<Aen::UIComponent>().SetColor(D2D1::ColorF::Aqua);

	Aen::Input::ToggleRawMouse(true);
	Aen::Input::SetMouseVisible(false);
}

// ---------------------------------------------------------		Update		--------------------------------------------------------------- //

void Gameplay::Update(const float& deltaTime) {
	
	wstringstream potionNr;
	potionNr << m_player.GetPotionNr();
	m_player.PotionUpdate();
	if (m_hp != m_player.GetHealth()) {
		float hp = (m_hp - m_player.GetHealth());

		m_UI->GetComponent<Aen::UIComponent>().UpdatePicture((hp * 2.f) * (1.f/1920.f) * screenSize.x, 0);
		m_hp = m_player.GetHealth();
	}
	m_UI->GetComponent<Aen::UIComponent>().ChangeText(1, potionNr.str().c_str());

	if (m_toggleFullScreen)
		Aen::Input::SetMousePos((Aen::Vec2i)Aen::Vec2f(GetSystemMetrics(SM_CXSCREEN) * 0.5f, GetSystemMetrics(SM_CYSCREEN) * 0.5f));
	else
		Aen::Input::SetMousePos(m_Window.GetWindowPos() + (Aen::Vec2i)((Aen::Vec2f)m_Window.GetSize() * 0.5f));

	// ---------------------------------- Enemies --------------------------------------- //

	m_player.Update(m_enemyQueue, deltaTime);

	m_chest.Update(deltaTime, m_player.GetEntity());
	m_door.Update(deltaTime, m_player.GetEntity());
	if (m_chest.GetNear() || m_door.GetNear()) {
		m_UI->GetComponent<Aen::UIComponent>().SetTextPos((965.f / 1920.f) * screenSize.x, (800.f / 1024.f) * screenSize.y, 2);
		m_UI->GetComponent<Aen::UIComponent>().SetTextSize((900.f / 1920.f) * screenSize.x, (300.f / 1024.f) * screenSize.y, 2);

		if (Aen::Input::KeyDown(Aen::Key::F) && m_chest.GetType() == Type::Open && m_chest.GetNear()) {
			m_player.IncreaseHealthCap();
			m_chest.SetType(Type::Locked);
		}
	}
	else {
		m_UI->GetComponent<Aen::UIComponent>().SetTextPos(-100.f, -100.f, 2);
	}

	if (Aen::Input::KeyDown(Aen::Key::F)) {

		m_door.GetEntity()->SetRot(0, 90.f, 0);
	}

	for (auto& i : m_enemyQueue) {
		i->Update(deltaTime, m_player);
	}

	m_player.UpdateAttack(m_enemyQueue, deltaTime);


	if (m_hp <= 0.f) {
		SetWin(false);
		m_UI->GetComponent<Aen::UIComponent>().SetPicPos(0.f, 0.f, 0);
		m_deathTimer += deltaTime;

		if (m_deathTimer > 0.2f) {
			State::SetState(States::Gameover);
		}
	}

	//if (m_enemyQueue.empty()) {
	//	SetWin(true);
	//	State::SetState(States::Gameover);
	//}

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
			screenSize.x = wDesc.width;
			screenSize.y = wDesc.height;
			wDesc.height = GetSystemMetrics(SM_CYSCREEN) + 4u;
			wDesc.EXStyle = AEN_WS_EX_APPWINDOW;
			wDesc.style = AEN_WS_POPUPWINDOW | AEN_WS_VISIBLE;
			m_Window.LoadSettings(wDesc);
		}
		else {
			wDesc.width = static_cast<UINT>(GetSystemMetrics(SM_CXSCREEN) * 0.4f);
			screenSize = wDesc.width;
			screenSize.y = wDesc.height;
			wDesc.height = static_cast<UINT>(GetSystemMetrics(SM_CYSCREEN) * 0.4f);
			wDesc.EXStyle = AEN_WS_EX_APPWINDOW;
			wDesc.style = AEN_WS_OVERLAPPEDWINDOW | AEN_WS_VISIBLE;
			m_Window.LoadSettings(wDesc);
		}
	}

	// ------------------------------ Quick Exit Button -------------------------------- //

	if (Aen::Input::KeyDown(Aen::Key::ESCAPE)) {
		//State::SetState(States::Gameover);
		m_Window.Exit();
	}
	// ------------------------------------- States -------------------------------------- //
	//if (m_hp <= 0 && m_enemyQueue.size() == 0)
	//{
	//	State::SetState(States::Gameover);
	//}
}