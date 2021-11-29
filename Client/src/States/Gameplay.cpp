#include "Gameplay.h"

Gameplay::Gameplay(Aen::Window& window)
	:State(window), m_speed(10.f), m_fSpeed(0.15f), m_toggleFullScreen(true), m_hp(200.f),
	IFRAMEMAX(1.5f), m_iFrames(0.f) {
	
}

Gameplay::~Gameplay() {
	//Aen::EntityHandler::RemoveEntity(*m_dLight);
	Aen::EntityHandler::RemoveEntity(*m_plane);
	/*Aen::EntityHandler::RemoveEntity(*m_reimube1);*/
	Aen::EntityHandler::RemoveEntity(*m_UI);
	
	/*for (auto& b : *m_levelImporter.GetEntityList()) {
		Aen::EntityHandler::RemoveEntity(*b);
	}*/

	/*for (auto& d : m_enemyQueue) {
		delete d;
	}*/

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
	m_UI->GetComponent<Aen::UIComponent>().AddPicture(AEN_TEXTURE_DIR_W(L"healthbar.png")); //0
	m_UI->GetComponent<Aen::UIComponent>().SetPicPos(350.f, 100.f);
	m_UI->GetComponent<Aen::UIComponent>().SetPicSize(m_hp * 2.f, 150.f);

	m_UI->GetComponent<Aen::UIComponent>().AddPicture(AEN_TEXTURE_DIR_W(L"bar.png")); //1
	m_UI->GetComponent<Aen::UIComponent>().SetPicPos(350.f, 100.f);
	m_UI->GetComponent<Aen::UIComponent>().SetPicSize(m_hp * 2.f, 150.f);

	m_UI->GetComponent<Aen::UIComponent>().AddPicture(AEN_TEXTURE_DIR_W(L"potion.png")); //2
	m_UI->GetComponent<Aen::UIComponent>().SetPicPos(125.f, 100.f);
	m_UI->GetComponent<Aen::UIComponent>().SetPicSize(150.f, 150.f);

	//m_UI->GetComponent<Aen::UIComponent>().AddPicture(AEN_RESOURCE_DIR_W(L"GoalText.png"), 1);
	//m_UI->GetComponent<Aen::UIComponent>().SetPicPos(965.f, 100.f, 1);
	//m_UI->GetComponent<Aen::UIComponent>().SetPicSize(600.f, 100.f, 1);

	m_UI->GetComponent<Aen::UIComponent>().AddText(L"Kill All Enemies", 72.f); //0
	m_UI->GetComponent<Aen::UIComponent>().SetTextPos(965.f, 100.f);
	m_UI->GetComponent<Aen::UIComponent>().SetTextSize(900.f, 300);

	m_UI->GetComponent<Aen::UIComponent>().AddText(L"5", 50.f); //1 - Amount of potion
	m_UI->GetComponent<Aen::UIComponent>().SetTextPos(120.f, 110.f);
	m_UI->GetComponent<Aen::UIComponent>().SetTextSize(150.f, 150.f);
	m_UI->GetComponent<Aen::UIComponent>().SetColor(D2D1::ColorF::Black);

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
	Aen::Animation& testAnim2 = Aen::Resource::CreateAnimation("TimWaveHead");
	testAnim2.LoadAnimation("AnimWaveHead.fbx");
	Aen::Animation& testAnim = Aen::Resource::CreateAnimation("TimWave");
	testAnim.LoadAnimation("AnimTri3.fbx");
	

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
	reimubeMat["OuterEdgeColor"] = Aen::Color::Pink;*/

	planeMat.LoadeAndSetDiffuseMap(AEN_TEXTURE_DIR("Floor_Diffuse.png"));
	planeMat["InnerEdgeColor"] = Aen::Color(0.2f, 0.26f, 0.37f, 1.f);
	planeMat["OuterEdgeColor"] = Aen::Color(0.2f, 0.26f, 0.37f, 1.f);

	/*potMat.LoadeAndSetDiffuseMap(AEN_RESOURCE_DIR("Pot_UV_DONE_2.png"));
	potMat["InnerEdgeColor"] = Aen::Color(0.2f, 0.26f, 0.37f, 1.f);
	potMat["OuterEdgeColor"] = Aen::Color(0.2f, 0.26f, 0.37f, 1.f);

	torchMat.LoadeAndSetDiffuseMap(AEN_RESOURCE_DIR("Torch_UV_DONE.png"));
	torchMat["InnerEdgeColor"] = Aen::Color(0.2f, 0.26f, 0.37f, 1.f);
	torchMat["OuterEdgeColor"] = Aen::Color(0.2f, 0.26f, 0.37f, 1.f);

	healingPotMat.LoadeAndSetDiffuseMap(AEN_RESOURCE_DIR("Healing_Pot_UV_DONE.png"));
	healingPotMat["InnerEdgeColor"] = Aen::Color(0.2f, 0.26f, 0.37f, 1.f);
	healingPotMat["OuterEdgeColor"] = Aen::Color(0.2f, 0.26f, 0.37f, 1.f);*/

	//skelLightMat.LoadeAndSetDiffuseMap(AEN_RESOURCE_DIR("Healing_Pot_UV_DONE.png"));
	//skelLightMat["InnerEdgeColor"] = Aen::Color(0.2f, 0.26f, 0.37f, 1.f);
	//skelLightMat["OuterEdgeColor"] = Aen::Color(0.2f, 0.26f, 0.37f, 1.f);
	//skelLightMat["BaseColor"] = Aen::Color(0.8f, 0.8f, 0.5f, 1.f);

	//doorMat["InnerEdgeColor"] = Aen::Color(0.2f, 0.26f, 0.37f, 1.f);
	//doorMat["OuterEdgeColor"] = Aen::Color(0.2f, 0.26f, 0.37f, 1.f);
	//doorMat["BaseColor"] = Aen::Color(0.1f, 0.1f, 0.05f, 1.f);

	//chestMat["InnerEdgeColor"] = Aen::Color(0.2f, 0.26f, 0.37f, 1.f);
	//chestMat["OuterEdgeColor"] = Aen::Color(0.2f, 0.26f, 0.37f, 1.f);
	////chestMat["BaseColor"] = Aen::Color(0.1f, 0.1f, 0.05f, 1.f);

	//webMat.LoadeAndSetOpacityMap(AEN_RESOURCE_DIR("Web_UV.png"));
	/*webMat["InnerEdgeColor"] = Aen::Color(0.2f, 0.26f, 0.37f, 1.f);
	webMat["OuterEdgeColor"] = Aen::Color(0.2f, 0.26f, 0.37f, 1.f);*/

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

	wallE = nullptr;*/

	// -------------------------- Setup Entities -------------------------------- //

	/*m_pot = &Aen::EntityHandler::CreateEntity();
	m_pot->AddComponent<Aen::MeshInstance>();
	m_pot->GetComponent<Aen::MeshInstance>().SetMesh(pot);
	m_pot->GetComponent<Aen::MeshInstance>().SetMaterial(potMat);
	m_pot->SetPos(-12.f, 2.f, 2.f);

	m_torch = &Aen::EntityHandler::CreateEntity();
	m_torch->AddComponent<Aen::MeshInstance>();
	m_torch->GetComponent<Aen::MeshInstance>().SetMesh(torch);
	m_torch->GetComponent<Aen::MeshInstance>().SetMaterial(torchMat);
	m_torch->SetPos(-12.f, 2.f, -2.f);

	m_healingPot = &Aen::EntityHandler::CreateEntity();
	m_healingPot->AddComponent<Aen::MeshInstance>();
	m_healingPot->GetComponent<Aen::MeshInstance>().SetMesh(healingPot);
	m_healingPot->GetComponent<Aen::MeshInstance>().SetMaterial(healingPotMat);
	m_healingPot->SetScale(0.3f, 0.3f, 0.3f);
	m_healingPot->SetPos(-12.f, 2.f, -4.f);*/

	/*m_skelLight = &Aen::EntityHandler::CreateEntity();
	m_skelLight->AddComponent<Aen::MeshInstance>();
	m_skelLight->GetComponent<Aen::MeshInstance>().SetMesh(skelLight);
	m_skelLight->GetComponent<Aen::MeshInstance>().SetMaterial(skelLightMat);
	m_skelLight->SetPos(-12.f, 1.f, 6.f);

	m_door = &Aen::EntityHandler::CreateEntity();
	m_door->AddComponent<Aen::MeshInstance>();
	m_door->GetComponent<Aen::MeshInstance>().SetMesh(door);
	m_door->GetComponent<Aen::MeshInstance>().SetMaterial(doorMat);
	m_door->SetScale(0.5f, 0.5f, 0.5f);
	m_door->SetPos(-12.f, 4.f, 12.f);
	m_door->SetRot(0.f, 90.f, 0.f);

	m_chest = &Aen::EntityHandler::CreateEntity();
	m_chest->AddComponent<Aen::MeshInstance>();
	m_chest->GetComponent<Aen::MeshInstance>().SetMesh(chest);
	m_chest->GetComponent<Aen::MeshInstance>().SetMaterial(chestMat);
	m_chest->SetScale(0.5f, 0.5f, 0.5f);
	m_chest->SetPos(-12.f, 2.f, 16.f);
	m_chest->SetRot(0.f, -90.f, 0.f);*/

	
	//m_tim->SetPos(2.f, 2.f, 2.f);

	/*m_web = &Aen::EntityHandler::CreateEntity();
	m_web->AddComponent<Aen::MeshInstance>();
	m_web->GetComponent<Aen::MeshInstance>().SetMesh(web);
	m_web->GetComponent<Aen::MeshInstance>().SetMaterial(webMat);
	m_web->SetPos(4.f, 2.f, 4.f);
	m_web->SetScale(3.f, 1.f, 2.f);*/

	m_plane = &Aen::EntityHandler::CreateEntity();
	m_plane->AddComponent<Aen::StaticBody>();
	m_plane->GetComponent<Aen::StaticBody>().SetGeometry(Aen::StaticGeometryType::PLANE);


	//m_plane->GetComponent<Aen::StaticBody>().SetGeometry(Aen::StaticGeometryType::CUBE, Aen::Vec3f(1.f, 44.f, 44.f));
	//m_plane->AddComponent<Aen::MeshInstance>();
	//m_plane->GetComponent<Aen::MeshInstance>().SetMesh(plane);
	//m_plane->GetComponent<Aen::MeshInstance>().SetMaterial(planeMat);


	//m_reimube1 = &Aen::EntityHandler::CreateEntity();
	//m_reimube1->AddComponent<Aen::MeshInstance>();
	//m_reimube1->GetComponent<Aen::MeshInstance>().SetMesh(reimube);
	//m_reimube1->GetComponent<Aen::MeshInstance>().SetMaterial(enemyMat);
	////m_reimube1->AddComponent<Aen::StaticBody>();
	////m_reimube1->GetComponent<Aen::StaticBody>().SetBoundsToMesh(true);
	//m_reimube1->SetPos(0.f, 1.f, 11.f);
	//m_reimube1->SetRot(10, 1, 1);
	//m_reimube1->SetRenderLayer(1);

	
	
	


	// ------ Level Importer ------ //
	//std::string path = AEN_LEVEL_DIR("NewTestLevel.Level");
	//m_levelImporter.import(path);

	// ------------------- Procedural generation testing staging grounds ------- //
	//std::vector<string> levelPaths;

	//m_levelGenerator.LoadMutipleRoomFiles();


	//m_levelGenerator.AddLoadedToGeneration();

	//m_levelGenerator.SetMapTheme(Aen::RoomTheme::GENERIC);

	////Match this value to the size of the rooms we are using
	//m_levelGenerator.SetRoomDimension(43.f);
	//mptr_map = m_levelGenerator.GenerationTestingFunction();

	////Use this value to set the start of the player / origin of the map
	//Aen::Vec3f playerStartPos(0.f, 0.f, 0.f);


	/*for (UINT y = 0; y < Aen::mapSize; y++) {
		for (UINT x = 0; x < Aen::mapSize; x++) {
			m_levelGenerator.SpawnRoom(Aen::Vec2i(x, y));

			if (mptr_map[y * Aen::mapSize + x].m_roomSpecial == Aen::SpecialRoom::ENTRANCE) {
				m_levelGenerator.GetRoomPos(x, y, &playerStartPos.x, &playerStartPos.z);
			}
		}
	}

	m_player.GetEntity()->SetPos(playerStartPos + Aen::Vec3f(0.f, 0.8f, 0.f));


	//---------ENEMIES----------//
	/*int numEnemies = 10;
	int offset = -10;
	Aen::Vec3f enemyPos{0.f, 1.f, -15.f};
	for (int u = 0; u < numEnemies; u++) {
		m_enemyQueue.emplace_back(AEN_NEW Rimuru(enemyPos + Aen::Vec3f((rand() % 38) - 19.f, 0.f, offset)));
		offset -= 5;
	}*/

	std::vector<Aen::Vec3f> tempEnemies = m_levelGenerator.GetHandlerPtr()->GetEnemyPos();
	for (size_t i = 0; i < m_levelGenerator.GetHandlerPtr()->GetEnemyPos().size(); i++)
	{
		m_enemyQueue.emplace_back(AEN_NEW Rimuru(tempEnemies[i]));
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

	if (m_hp != m_player.GetHealth()) { //ersätt collision med enemy i if satsen
		wstringstream potionNr;
		float hp = (m_hp - m_player.GetHealth());
		potionNr << m_player.GetPotionNr();

		m_UI->GetComponent<Aen::UIComponent>().UpdatePicture(hp * 2.f, 0);
		m_UI->GetComponent<Aen::UIComponent>().TextNr(1, potionNr.str().c_str());
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

	//if(m_enemyQueue.empty())
	//	State::SetState(States::Victory);

	#ifdef _DEBUG
		if(Aen::Input::KeyDown(Aen::Key::J))
			m_enemyQueue.emplace_back(AEN_NEW Rimuru());
	#endif


	/*if (Aen::Input::KeyDown(Aen::Key::O)) {
		m_tim->GetComponent<Aen::Animator>().Pause();
	}
	if (Aen::Input::KeyDown(Aen::Key::P)) {
		m_tim->GetComponent<Aen::Animator>().Run();
	}*/

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

	// ------------------------------ Quick Exit Button -------------------------------- //

	if (Aen::Input::KeyDown(Aen::Key::ESCAPE))
	{
		State::SetState(States::Gameover);
		//m_Window.Exit();
	}

	// ------------------------------------- States -------------------------------------- //
	/*if (m_hp <= 0 && m_enemyQueue.size() == 0)
	{
		State::SetState(States::Gameover);
	}*/
}