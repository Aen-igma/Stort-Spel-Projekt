#include "Gameplay.h"

Gameplay::Gameplay(Aen::Window& window)
	:State(window), m_speed(10.f), m_fSpeed(0.15f), m_toggleFullScreen(true), m_hp(200.f), m_timer(0),m_deathTimer(0),
	IFRAMEMAX(1.5f), m_iFrames(0.f), m_Mat(Aen::Resource::CreateMaterial("Bill")), m_TransTimer(0.001f) {}

Gameplay::~Gameplay() {
	Aen::EntityHandler::RemoveEntity(*m_dLight);
	Aen::EntityHandler::RemoveEntity(*m_plane);
	mp_uiComp = nullptr;
	Aen::EntityHandler::RemoveEntity(*m_UI);
	Aen::EntityHandler::RemoveEntity(*m_PS);
	Aen::EntityHandler::RemoveEntity(*m_bill);
	Aen::EntityHandler::RemoveEntity(*m_throne);
	
	for (auto& d : m_enemyQueue) {
		delete d;
	}
	m_pSkeleBoss, m_plane, m_UI = nullptr;

	Aen::GlobalSettings::StopQuadtree();

	Aen::Resource::RemoveAllMaterials();
	Aen::Resource::RemoveAllMeshes();
	Aen::Resource::RemoveAllTextures();
	Aen::Resource::RemoveAllAnimations();

}

void Gameplay::Initialize()
{
	srand((UINT)time(NULL));
	State::SetLoad(false);
	// ----------------------------- Setup Camera ------------------------------- //

	m_debugCam = &Aen::EntityHandler::CreateEntity();
	m_debugCam->AddComponent<Aen::Camera>();
	m_debugCam->GetComponent<Aen::Camera>().SetCameraPerspective(90.f, Aen::GlobalSettings::GetWindow()->GetAspectRatio(), 0.01f, 300.f);

	// ------------------------ Setup Directional Light ------------------------- //

	m_dLight = &Aen::EntityHandler::CreateEntity();
	m_dLight->AddComponent<Aen::DirectionalLight>();
	m_dLight->GetComponent<Aen::DirectionalLight>().SetColor(Aen::Color(0.3f, 0.3f, 0.5f, 1.f));
	m_dLight->GetComponent<Aen::DirectionalLight>().SetStrength(0.3f);
	m_dLight->SetRot(3.f, 45.f, 0.f);

	// ----------------------------- Animations -------------------------------- //
	
	// Skel_Light
	Aen::Animation& skelIdle = Aen::Resource::CreateAnimation("Skel_Idle");
	skelIdle.LoadAnimation(AEN_ANIMATION_DIR("Skel_Light_NewIdle.fbx"));
	Aen::Animation& skelWalk = Aen::Resource::CreateAnimation("Skel_Walk");
	skelWalk.LoadAnimation(AEN_ANIMATION_DIR("Skel_Light_Walking_2.fbx"));
	Aen::Animation& skelAttack = Aen::Resource::CreateAnimation("Skel_Attack");
	skelAttack.LoadAnimation(AEN_ANIMATION_DIR("Skel_Light_NewAttack.fbx"));

	// Boss
	Aen::Animation& bossThrone = Aen::Resource::CreateAnimation("Boss_Throne");
	bossThrone.LoadAnimation(AEN_ANIMATION_DIR("Boss_Skeletor_Throne_Sit.fbx"));
	Aen::Animation& bossWalk = Aen::Resource::CreateAnimation("Boss_Walk");
	bossWalk.LoadAnimation(AEN_ANIMATION_DIR("Boss_Skeletor_Hover.fbx"));
	Aen::Animation& bossAttack = Aen::Resource::CreateAnimation("Boss_Attack");
	bossAttack.LoadAnimation(AEN_ANIMATION_DIR("Boss_Skeletor_Attack.fbx"));
	Aen::Animation& bossSummon = Aen::Resource::CreateAnimation("Boss_Summon");
	bossSummon.LoadAnimation(AEN_ANIMATION_DIR("Boss_Skeletor_Summon.fbx"));

	

	// ----------------------------- Load Meshes -------------------------------- //

	Aen::Mesh& rimuru = Aen::Resource::CreateMesh("Rimuru");
	rimuru.Load(AEN_MODEL_DIR("Slime.fbx"));
	Aen::Mesh& skeleLight = Aen::Resource::CreateMesh("SkeletonLight");
	skeleLight.Load(AEN_MODEL_DIR("Skel_Light_Walking_2.fbx"));
	Aen::Mesh& throne = Aen::Resource::CreateMesh("Throne");
	throne.Load(AEN_MODEL_DIR("Throne.fbx"));

	// -------------------------- Setup Material -------------------------------- //

	Aen::Material& planeMat = Aen::Resource::CreateMaterial("PlaneMaterial");
	Aen::Material& slimeMat = Aen::Resource::CreateMaterial("SlimeMaterial");
	Aen::Material& skeleLightMat = Aen::Resource::CreateMaterial("SkeleLightMaterial");
	Aen::Material& enemyMatHurt = Aen::Resource::CreateMaterial("EnemyMaterialHurt");
	Aen::Material& throneMat = Aen::Resource::CreateMaterial("ThroneMaterial");

	Aen::Material& psMat = Aen::Resource::CreateMaterial("PSMaterial");
	psMat.LoadeAndSetDiffuseMap(AEN_TEXTURE_DIR("Flames2.png"));
	psMat.LoadeAndSetOpacityMap(AEN_TEXTURE_DIR("FlamesOppacity.png"));

	slimeMat.LoadeAndSetDiffuseMap(AEN_TEXTURE_DIR("SlimeRimuruFace.png"));
	slimeMat["InnerEdgeColor"] = Aen::Color::Cyan;
	slimeMat["OuterEdgeColor"] = Aen::Color::Cyan;
	slimeMat["BaseColor"] = Aen::Color::Cyan;

	skeleLightMat.LoadeAndSetDiffuseMap(AEN_TEXTURE_DIR("Skel_Light_UV3_DONE.png"));
	skeleLightMat["InnerEdgeColor"] = Aen::Color::Black;
	skeleLightMat["OuterEdgeColor"] = Aen::Color::Black;
	skeleLightMat["BaseColor"] = Aen::Color::White;

	// Material to switch to when enemy is hurt
	enemyMatHurt.LoadeAndSetEmissionMap(AEN_TEXTURE_DIR("White.png"));
	enemyMatHurt["BaseColor"] = Aen::Color::Red;
	enemyMatHurt["GlowColor"] = Aen::Color::Red;

	planeMat.LoadeAndSetDiffuseMap(AEN_TEXTURE_DIR("Floor_Diffuse.png"));
	planeMat["InnerEdgeColor"] = Aen::Color(0.2f, 0.26f, 0.37f, 1.f);
	planeMat["OuterEdgeColor"] = Aen::Color(0.2f, 0.26f, 0.37f, 1.f);

	throneMat.LoadeAndSetDiffuseMap(AEN_TEXTURE_DIR("Throne.png"));
	throneMat["InnerEdgeColor"] = Aen::Color::Black;
	throneMat["OuterEdgeColor"] = Aen::Color::Black;

	// -------------------------- Setup Entities -------------------------------- //
	// 
	// -------------------------- Particle System -------------------------------- //

	//Comment if you want the engine to work, big problem here
	m_PS = &Aen::EntityHandler::CreateEntity();
	m_PS->AddComponent<Aen::PSSystemcomponent>();
	m_PS->GetComponent<Aen::PSSystemcomponent>().Initialize();
	/*m_PS->GetComponent<Aen::PSSystemcomponent>().SetRespawnHeight(10);*/
	m_PS->GetComponent<Aen::PSSystemcomponent>().SetEmitPos(0,0,0);
	m_PS->GetComponent<Aen::PSSystemcomponent>().SetNrOfPS(5);
	m_PS->GetComponent<Aen::PSSystemcomponent>().SetMaterial(psMat);


	m_plane = &Aen::EntityHandler::CreateEntity();
	m_plane->AddComponent<Aen::StaticBody>();
	m_plane->GetComponent<Aen::StaticBody>().SetGeometry(Aen::StaticGeometryType::PLANE);

	m_throne = &Aen::EntityHandler::CreateEntity();
	m_throne->AddComponent<Aen::MeshInstance>();
	m_throne->GetComponent<Aen::MeshInstance>().SetMesh(throne);
	m_throne->GetComponent<Aen::MeshInstance>().SetMaterial(throneMat);
	//m_throne->AddComponent<Aen::StaticBody>();
	//m_throne->GetComponent<Aen::StaticBody>().SetBoundsToMesh(true);
	

	// ------------------- Procedural generation testing staging grounds ------- //
	m_levelGenerator.LoadMutipleRoomFiles();

	m_levelGenerator.AddLoadedToGeneration();

	m_levelGenerator.SetMapTheme(Aen::RoomTheme::GENERIC);

	//Match this value to the size of the rooms we are using
	//m_levelGenerator.SetRoomDimension(80.f); //Deprecated, using the default value instead of setting it in run time
	//mptr_map = m_levelGenerator.GenerateLevel();
	mptr_map = m_levelGenerator.GenerationTestingFunction();
	m_levelGenerator.CleanMap();

	//Use this value to set the start of the player / origin of the map
	Aen::Vec3f playerStartPos(0.f, 0.f, 0.f);
	Aen::Vec3f ChestPos;
	Aen::Vec3f doorPos;
	//Aen::Vec3f QuadMin(1000.f, 10.f, 1000.f);
	//Aen::Vec3f QuadMax(0.f, 10.f, 0.f);
	int roomNormal = 0;
	int itemNormal = 0;

	for (UINT y = 0; y < Aen::mapSize; y++) {
		for (UINT x = 0; x < Aen::mapSize; x++) {
			m_levelGenerator.SpawnRoom(Aen::Vec2i(x, y));

			if (mptr_map[y * Aen::mapSize + x].m_present)
			{
				/*QuadMin.x = min(QuadMin.x, y * 80);
				QuadMin.z = min(QuadMin.z, x * 80);
				QuadMax.x = max(QuadMax.x, y * 80);
				QuadMax.z = max(QuadMax.z, x * 80);*/

				if (mptr_map[y * Aen::mapSize + x].m_roomSpecial == Aen::SpecialRoom::ENTRANCE) {
					m_levelGenerator.GetRoomPos(x, y, &playerStartPos.x, &playerStartPos.z);

				}
				mptr_map[x + y * Aen::mapSize].mptr_parent;

				if (mptr_map[y * Aen::mapSize + x].m_roomSpecial == Aen::SpecialRoom::BOSS) 
				{
					m_levelGenerator.GetRoomPos(x, y, &m_bossPos.x, &m_bossPos.z);
					m_levelGenerator.GetRoomPos(x, y, &doorPos.x, &doorPos.z);
					roomNormal = mptr_map[y * Aen::mapSize + x].connectionDirections;
		
				}
				mptr_map[x + y * Aen::mapSize].mptr_parent;

				if (mptr_map[y * Aen::mapSize + x].m_roomSpecial == Aen::SpecialRoom::ITEM) {
					itemNormal = mptr_map[y * Aen::mapSize + x].connectionDirections;
					m_levelGenerator.GetRoomPos(x, y, &ChestPos.x, &ChestPos.z);
				}
			}
		}
	}
	/*QuadMax.x += 40.f;
	QuadMax.z += 40.f;
	QuadMin.x -= 40.f;
	QuadMin.z -= 40.f;*/

	m_chest.GetEntity()->SetPos(ChestPos);
	//m_PS->SetPos(ChestPos.x + 10.f, ChestPos.y + 5.f, ChestPos.z);
	//m_PS->GetComponent<Aen::PSSystemcomponent>().SetRespawnHeight(ChestPos.y + 10.f);
	//m_PS->GetComponent<Aen::PSSystemcomponent>().SetEmitPos(ChestPos.x + 10.f, ChestPos.y + 5.f, ChestPos.z);
	m_chest.SetType(Type::Open);

	//m_player.GetEntity()->SetPos(m_bossPos.x, m_bossPos.y + 5.f, m_bossPos.z);
	m_player.GetEntity()->SetPos(playerStartPos.x, playerStartPos.y + 5.f, playerStartPos.z);
	//m_player.GetEntity()->SetPos(ChestPos.x + 10.f, ChestPos.y + 5.f, ChestPos.z);
	m_chest.SetType(Type::Open);
	m_door.SetType(Type::Closed);
	m_debugCam->SetPos(playerStartPos.x, playerStartPos.y, playerStartPos.z);


	if (itemNormal == 1) { //north
		m_chest.GetEntity()->SetRot(0, 0, 0);
	}
	else if (itemNormal == 10) {//east
		m_chest.GetEntity()->SetRot(0, -90, 0);
	}
	else if (itemNormal == 100) {//south
		m_chest.GetEntity()->SetRot(0, 180, 0);
	}
	else if (itemNormal == 1000) {//west
		m_chest.GetEntity()->SetRot(0, 90, 0);
	}

	//m_attack->SetParent(*m_player);
	//printf("");
	//---------ENEMIES----------//
	// ALWAYS SPAWN BOSS BEFORE OTHER ENEMIES!!!!!

	m_enemyQueue.emplace_back(AEN_NEW Boss(m_bossPos, 200.f));
	m_pSkeleBoss = dynamic_cast<Boss*>(m_enemyQueue[m_enemyQueue.size() - 1]);
	m_player.AddBossesAlive(1);
	m_pSkeleBoss->GetEntity()->SetPos(m_bossPos);
	m_pSkeleBoss->GetBossEngage(false);
	m_player.SetBossP(m_pSkeleBoss);

	/*std::vector<Aen::Vec3f> tempSlimes = m_levelGenerator.GetHandlerPtr()->GetEnemyPos();
	std::vector<Aen::Vec3f> tempLskels = m_levelGenerator.GetHandlerPtr()->GetLskelPos();
	for (size_t i = 0; i < m_levelGenerator.GetHandlerPtr()->GetEnemyPos().size(); i++)
	{
		m_enemyQueue.emplace_back(AEN_NEW Rimuru(tempSlimes[i]));
	}
	for (size_t i = 0; i < m_levelGenerator.GetHandlerPtr()->GetLskelPos().size(); i++)
	{
		m_enemyQueue.emplace_back(AEN_NEW SkeleLight(tempLskels[i]));
	}*/

	cout << "BOSS ROOM: " << roomNormal << endl;
	m_throne->SetScale(2.f, 2.f, 2.f);

	// -- Door, Throne, Boss Rotations -- //
	if (roomNormal == 1) { //north
		m_throne->SetPos(m_bossPos.x, m_bossPos.y + 6.11f, m_bossPos.z + 33.35f);
		m_door.GetEntity()->SetRot(0, 180, 0);
		m_throne->SetRot(0.f, 180.f, 0.f);
		m_pSkeleBoss->SetThronePosition(m_throne->GetPos().x - 1.f, m_throne->GetPos().y, m_throne->GetPos().z);
		m_pSkeleBoss->GetEntity()->SetRot(0.f, 0.f, 0.f);
		m_grave.GetEntity()->SetRot(0, 180, 0);
		m_exit.GetEntity()->SetRot(0, 180, 0);
	}
	else if (roomNormal == 10) {//east
		m_throne->SetPos(m_bossPos.x - 33.35f, m_bossPos.y + 6.11f, m_bossPos.z);
		m_door.GetEntity()->SetRot(0, 90, 0);
		m_throne->SetRot(0.f, 90.f, 0.f);
		m_pSkeleBoss->SetThronePosition(m_throne->GetPos().x, m_throne->GetPos().y, m_throne->GetPos().z - 1.f);
		m_pSkeleBoss->GetEntity()->SetRot(0.f, -90.f, 0.f);
		m_grave.GetEntity()->SetRot(0, 90, 0);
		m_exit.GetEntity()->SetRot(0, 90, 0);
	}
	else if (roomNormal == 100) {//south
		m_throne->SetPos(m_bossPos.x, m_bossPos.y + 6.11f, m_bossPos.z - 33.35f);
		m_door.GetEntity()->SetRot(0, 0, 0);
		m_throne->SetRot(0.f, 0.f, 0.f);
		m_pSkeleBoss->SetThronePosition(m_throne->GetPos().x +1.f, m_throne->GetPos().y, m_throne->GetPos().z);
		m_pSkeleBoss->GetEntity()->SetRot(0.f, 180.f, 0.f);
		m_grave.GetEntity()->SetRot(0, 0, 0);
		m_exit.GetEntity()->SetRot(0, 0, 0);
	}
	else if (roomNormal == 1000) {//west
		m_throne->SetPos(m_bossPos.x + 33.35f, m_bossPos.y + 6.11f, m_bossPos.z);
		m_door.GetEntity()->SetRot(0, -90, 0);
		m_throne->SetRot(0.f, -90.f, 0.f);
		m_pSkeleBoss->SetThronePosition(m_throne->GetPos().x, m_throne->GetPos().y, m_throne->GetPos().z +1.f);
		m_pSkeleBoss->GetEntity()->SetRot(0.f, 90.f, 0.f);
		m_grave.GetEntity()->SetRot(0, -90, 0);
		m_exit.GetEntity()->SetRot(0, -90, 0);
	}
	//m_pSkeleBoss->SetThronePosition(m_throne->GetPos());
	m_door.GetEntity()->SetPos(doorPos.x, 0.f, doorPos.z);
	m_door.GetEntity()->MoveRelative(0.f, 0, 39.5f);
	m_grave.GetEntity()->SetPos(0, -5, 0);

	m_exit.GetEntity()->SetPos(doorPos.x, 10.f, doorPos.z);
	m_exit.GetEntity()->MoveRelative(0, 0, -21.5f);
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
	mp_uiComp = &m_UI->GetComponent<Aen::UIComponent>();
	mp_uiComp->AddPicture(AEN_TEXTURE_DIR_W(L"healthbar.png")); //0
	mp_uiComp->SetPicPos((350.f / 1920)* wDesc.width, (100.f / 1024) * wDesc.height);
	mp_uiComp->SetPicSize(((m_hp * 2.f) / 1920) * wDesc.width, (150.f / 1024) * wDesc.height);

	mp_uiComp->AddPicture(AEN_TEXTURE_DIR_W(L"bar.png")); //1
	mp_uiComp->SetPicPos((350.f / 1920) * wDesc.width, (100.f / 1024) * wDesc.height);
	mp_uiComp->SetPicSize(((m_hp * 2.f) / 1920) * wDesc.width, (150.f / 1024) * wDesc.height);

	mp_uiComp->AddPicture(AEN_TEXTURE_DIR_W(L"potion.png")); //2
	mp_uiComp->SetPicPos((125.f / 1920) * wDesc.width, (100.f / 1024) * wDesc.height);
	mp_uiComp->SetPicSize((150.f / 1920) * wDesc.width, (150.f / 1024) * wDesc.height);

	m_UI->GetComponent<Aen::UIComponent>().AddPicture(AEN_TEXTURE_DIR_W(L"healthbar.png"));//3 - bosshealthbar

	mp_uiComp->AddText(L"- Find the boss", 30.f); //0
	mp_uiComp->SetTextPos((175.f / 1920) * wDesc.width, (300.f / 1024) * wDesc.height);
	mp_uiComp->SetTextSize((900.f / 1920) * wDesc.width, (300 / 1024) * wDesc.height);
	mp_uiComp->SetColor(D2D1::ColorF::Gold);

	mp_uiComp->AddText(L"3", 50.f); //1 - Amount of potion
	mp_uiComp->SetTextPos((120.f / 1920) * wDesc.width, (110.f / 1024) * wDesc.height);
	mp_uiComp->SetTextSize((150.f / 1920) * wDesc.width, (150.f / 1024) * wDesc.height);
	mp_uiComp->SetColor(D2D1::ColorF::Black);

	mp_uiComp->AddText(L"Interact (F)", 60.f); //2
	mp_uiComp->SetTextPos((965.f / 1920.f) * wDesc.width, (800.f / 1024.f) * wDesc.height);
	mp_uiComp->SetTextSize((900.f / 1920.f) * wDesc.width, (300.f / 1024.f) * wDesc.height);
	mp_uiComp->SetColor(D2D1::ColorF::Aqua);

	mp_uiComp->AddText(L"- Find Item Room (Optional)", 30.f); //3
	mp_uiComp->SetTextPos((200.f / 1920) * wDesc.width, (350.f / 1024) * wDesc.height);
	mp_uiComp->SetTextSize((900.f / 1920) * wDesc.width, (300 / 1024) * wDesc.height);
	mp_uiComp->SetColor(D2D1::ColorF::Gold);

	//Pause menu UI
	mp_uiComp->AddPicture(AEN_TEXTURE_DIR_W(L"PauseOverlay.png")); //4
	mp_uiComp->SetPicPos((1.f / 2.f)* wDesc.width, (1.f / 2.f)* wDesc.height);
	mp_uiComp->SetPicSize(wDesc.width, wDesc.height);

	mp_uiComp->AddButton(AEN_TEXTURE_DIR_W(L"Continue.png")); //0
	mp_uiComp->SetButtonPos((1.f / 6.f)* wDesc.width, (800.f / 1024.f)* wDesc.height);
	mp_uiComp->SetButtonSize((200.f / 1920.f) * wDesc.width, (75.f / 1024.f) * wDesc.height);

	mp_uiComp->AddButton(AEN_TEXTURE_DIR_W(L"Quit.png")); //1
	mp_uiComp->SetButtonPos((1.f / 6.f)* wDesc.width, (900 / 1024.f)* wDesc.height);
	mp_uiComp->SetButtonSize((200.f / 1920.f)* wDesc.width, (75.f / 1024.f)* wDesc.height);

	mp_uiComp->SaveButtonData();

	mp_uiComp->SetPicSize(0, 0, 4);
	mp_uiComp->SetButtonSize(0, 0, 0);
	mp_uiComp->SetButtonSize(0, 0, 1);

	m_Mat.LoadeAndSetDiffuseMap(AEN_TEXTURE_DIR("White.png"));
	m_Mat.LoadeAndSetEmissionMap(AEN_TEXTURE_DIR("White.png"));
	m_Mat["InnerEdgeColor"] = Aen::Color::White;
	m_Mat["OuterEdgeColor"] = Aen::Color::White;
	m_Mat["GlowColor"] = Aen::Color::White;
	m_Mat["OpacityStr"] = 1.f;

	m_bill = &Aen::EntityHandler::CreateEntity();
	m_bill->AddComponent<Aen::MeshInstance>();
	m_bill->GetComponent<Aen::MeshInstance>().SetMesh("PLANE");
	m_bill->GetComponent<Aen::MeshInstance>().SetMaterial("Bill");
	m_bill->SetScale(0, 0, 0);
	m_bill->SetRenderLayer(2);
	//------QUADTREE------//
	Aen::GlobalSettings::StartQuadtree(0, 4, 10);
	/*Aen::GlobalSettings::StartQuadtree(QuadMin, QuadMax, 0, 4, 10);*/

	Aen::Input::ToggleRawMouse(true);
	Aen::Input::SetMouseVisible(false);
	SetWin(false);
	m_bossHP = m_pSkeleBoss->GetHealth();

	
}

// ---------------------------------------------------------		Update		--------------------------------------------------------------- //

void Gameplay::Update(const float& deltaTime) {

	



	if (Aen::Input::KeyDown(Aen::Key::ESCAPE)) {
		m_paused = !m_paused;

		if (m_paused) {
			Aen::Input::SetMouseVisible(true);
			mp_uiComp->SetPicSize(screenSize.x, screenSize.y, 4);
			mp_uiComp->SetTextPos(-100, 0, 0);
			mp_uiComp->SetTextPos(-100, 0, 1);
			mp_uiComp->SetTextPos(-100, 0, 3);
		}
		else{
			Aen::Input::SetMouseVisible(false);
			mp_uiComp->SetPicSize(0, 0, 4);
			mp_uiComp->SetButtonSize(0, 0, 0);
			mp_uiComp->SetButtonSize(0, 0, 1);
			mp_uiComp->SetTextPos((175.f / 1920) * screenSize.x, (300.f / 1024) * screenSize.y, 0);
			mp_uiComp->SetTextSize((900.f / 1920) * screenSize.x, (300 / 1024) * screenSize.y, 0);
			mp_uiComp->SetTextPos((120.f / 1920) * screenSize.x, (110.f / 1024) * screenSize.y, 1);
			mp_uiComp->SetTextSize((150.f / 1920) * screenSize.x, (150.f / 1024) * screenSize.y, 1);
			mp_uiComp->SetTextPos((200.f / 1920) * screenSize.x, (350.f / 1024) * screenSize.y, 3);
			mp_uiComp->SetTextSize((900.f / 1920) * screenSize.x, (300 / 1024) * screenSize.y, 3);

		}
		//State::SetState(States::Loadscreen);
		//m_Window.m_exit();
	}

	if (m_paused) {
		mp_uiComp->Update();
		if (Aen::Input::KeyDown(Aen::Key::LMOUSE) && mp_uiComp->Intersects(0))
		{
			m_paused = false;
			Aen::Input::SetMouseVisible(false);
			mp_uiComp->SetPicSize(0, 0, 4);
			mp_uiComp->SetButtonSize(0, 0, 0);
			mp_uiComp->SetButtonSize(0, 0, 1);
			mp_uiComp->SetTextPos((175.f / 1920) * screenSize.x, (300.f / 1024) * screenSize.y, 0);
			mp_uiComp->SetTextSize((900.f / 1920) * screenSize.x, (300 / 1024) * screenSize.y, 0);
			mp_uiComp->SetTextPos((120.f / 1920) * screenSize.x, (110.f / 1024) * screenSize.y, 1);
			mp_uiComp->SetTextSize((150.f / 1920) * screenSize.x, (150.f / 1024) * screenSize.y, 1);
			mp_uiComp->SetTextPos((200.f / 1920) * screenSize.x, (350.f / 1024) * screenSize.y, 3);
			mp_uiComp->SetTextSize((900.f / 1920) * screenSize.x, (300 / 1024) * screenSize.y, 3);
		}
		if (Aen::Input::KeyDown(Aen::Key::LMOUSE) && mp_uiComp->Intersects(1))
		{
			State::SetState(States::Gameover);
		}
		return;
	}

	if (m_hp != m_player.GetHealth()) { //ers�tt collision med enemy i if satsen
		mp_uiComp->UpdatePicture(((m_hp - m_player.GetHealth()) * 2.f) * (1.f/1920.f) * screenSize.x, 0);
		m_hp = m_player.GetHealth();
	}
	static int pots;
	if (pots != m_player.GetPotionNr()) {
		mp_uiComp->ChangeText(1, std::to_wstring(m_player.GetPotionNr()).c_str());
		pots = m_player.GetPotionNr();
	}

	m_player.PotionUpdate();

	if (m_exit.GetType() == Type::Closed) {

		mp_uiComp->SetDraw(false);
		m_TransTimer += deltaTime * 0.5f;
		m_Mat["OpacityStr"] = m_TransTimer;
		m_bill->SetScale((10 / 1920.f) * screenSize.x, (5 / 1024.f) * screenSize.y, 0);

		if (m_TransTimer > 1.5f) {
			SetWin(true);
			m_exit.SetType(Type::Locked);
			State::SetState(States::Gameover);
		}
		return;
	}

	if (m_toggleFullScreen)
		Aen::Input::SetMousePos((Aen::Vec2i)Aen::Vec2f(GetSystemMetrics(SM_CXSCREEN) * 0.5f, GetSystemMetrics(SM_CYSCREEN) * 0.5f));
	else
		Aen::Input::SetMousePos(m_Window.GetWindowPos() + (Aen::Vec2i)((Aen::Vec2f)m_Window.GetSize() * 0.5f));

	// ---------------------------------- Debug Camera --------------------------------------- //

	if (Aen::Input::KeyDown(Aen::Key::I)) {
		m_debug = true;
		Aen::GlobalSettings::SetMainCamera(*m_debugCam);
	}
	if (Aen::Input::KeyDown(Aen::Key::O)) {
		m_debug = false;
		Aen::GlobalSettings::SetMainCamera(*m_player.GetCamera());
	}

	if (m_debug) {
		static Aen::Vec2f mouse;
		while (!Aen::Input::MouseBufferIsEmbty()) {
			Aen::MouseEvent me = Aen::Input::ReadEvent();

			if (me.getInputType() == Aen::MouseEvent::MouseInput::RAW_MOVE) {

				mouse.x += (float)me.GetPos().y * 5.f * deltaTime;
				mouse.y += (float)me.GetPos().x * 5.f * deltaTime;
				m_debugCam->SetRot(mouse.x, mouse.y, 0.f);

			}
		}

		Aen::Vec3f axis;
		axis.x = Aen::Input::KeyPress(Aen::Key::A) - Aen::Input::KeyPress(Aen::Key::D);
		axis.y = Aen::Input::KeyPress(Aen::Key::S) - Aen::Input::KeyPress(Aen::Key::W);
		axis.z = Aen::Input::KeyPress(Aen::Key::SPACE) - Aen::Input::KeyPress(Aen::Key::SHIFT);
		if (Aen::Input::KeyPress(Aen::Key::CONTROL))
		{
			m_debugCam->MoveRelative(Aen::Vec3f(axis.x, 0.f, axis.y) * deltaTime * 40.f);
			m_debugCam->Move(Aen::Vec3f(0.f, axis.z, 0.f) * deltaTime * 40.f);
		}
		else
		{
			m_debugCam->MoveRelative(Aen::Vec3f(axis.x, 0.f, axis.y) * deltaTime * 10.f);
			m_debugCam->Move(Aen::Vec3f(0.f, axis.z, 0.f) * deltaTime * 10.f);
		}

	}
	else {
		m_player.Update(m_enemyQueue, deltaTime);
	}
	// ---------------------------------- Enemies --------------------------------------- //

	m_bill->SetPos(m_player.GetCamera()->GetPos());
	m_bill->SetRot(m_player.GetCamera()->GetRot().x, m_player.GetCamera()->GetRot().y, 0);
	m_bill->MoveRelative(0, 0, -2);

	m_chest.Update(deltaTime, m_player.GetEntity());
	m_door.Update(deltaTime, m_player.GetEntity());
	m_grave.Update(deltaTime, m_player.GetEntity());
	m_exit.Update(deltaTime, m_player.GetCamera());
	if (m_chest.GetNear() || m_door.GetNear() || m_grave.GetNear()) {
		mp_uiComp->SetTextPos((965.f / 1920.f) * screenSize.x, (800.f / 1024.f) * screenSize.y, 2);
		mp_uiComp->SetTextSize((900.f / 1920.f) * screenSize.x, (300.f / 1024.f) * screenSize.y, 2);

		if (m_door.GetNear())
			mp_uiComp->ChangeText(2, L"Interact(F)");
	}
	else {
		mp_uiComp->SetTextPos(-100.f, -100.f, 2);
	}

	if (Aen::Input::KeyDown(Aen::Key::F)) {

		if (m_chest.GetType() == Type::Open && m_chest.GetNear()) {
			m_player.IncreaseHealthCap();
			m_chest.SetType(Type::Locked);
			mp_uiComp->ChangeText(2, L"Health Potions Restored");
		}
		else if (m_chest.GetType() == Type::Locked && m_chest.GetNear()) {
			mp_uiComp->ChangeText(2, L"Can't Get More health potions");
		}

		else if(m_door.GetType() == Type::Closed && m_door.GetNear()) {
			m_door.SetType(Type::Opening);
			m_pSkeleBoss->GetBossEngage(true);
		}
		else if (m_grave.GetNear() && m_grave.GetType() == Type::Closed) {
			m_exit.SetType(Type::Opening);
		}
	}

	if (m_grave.GetType() == Type::Closing) {

		m_grave.SetType(Type::Closed);
		m_grave.GetEntity()->SetPos(m_door.GetEntity()->GetPos());
		m_grave.GetEntity()->MoveRelative(0.f, 0, -55.f);
	}

	for (auto& i : m_enemyQueue) {
		i->Update(deltaTime, m_player);
	}

	m_player.UpdateAttack(m_enemyQueue, deltaTime);

	int enemiesToSummon = 0;
	if (m_player.GetBossesAlive() > 0)
	{
		if (m_pSkeleBoss->GetBS() != BossState::STATIONARY && m_door.GetType() == Type::Open) {
			mp_uiComp->ChangeText(0, L"- Kill the Boss");
			mp_uiComp->SetPicPos((1000.f / 1920) * screenSize.x, (700.f / 1024) * screenSize.y, 3);
			mp_uiComp->SetPicSize((1200.f / 1920) * screenSize.x, (150.f / 1024) * screenSize.y, 3);
			m_door.SetType(Type::Locking);
		}

		if (m_bossHP != m_pSkeleBoss->GetHealth()) {

			mp_uiComp->UpdatePicture((m_bossHP - m_pSkeleBoss->GetHealth()) * 6.f * (1.f / 1920.f)* screenSize.x, 3);
			m_bossHP = m_pSkeleBoss->GetHealth();
		}

		Aen::Vec3f minionOffset(-8.f,0,8.f);
		enemiesToSummon = m_pSkeleBoss->GetEnemiesToSummon();
		for (int i = 0; i < enemiesToSummon; i++)
		{
			minionOffset.z *= -1;
			Rimuru* bossMinion = AEN_NEW Rimuru(m_bossPos + minionOffset, EnemyType::MINION);
			minionOffset += Aen::Vec3f(3.f, 0, 0);
			m_pSkeleBoss->EmplaceMinion(bossMinion);
			m_enemyQueue.emplace_back(bossMinion);
		}

	}
	else enemiesToSummon = 0;


	if (m_hp <= 0.f) {
		SetWin(false);
		mp_uiComp->SetPicPos(0.f, 0.f, 0);
		m_deathTimer += deltaTime;

		if (m_deathTimer > 0.2f) {
			State::SetState(States::Gameover);
		}
	}
	if (m_player.GetBossesAlive() <= 0.f && m_grave.GetType() == Type::Open)
	{
		m_grave.SetType(Type::Closing);
		mp_uiComp->SetPicPos(0, 0, 3);
	}
	// ------------------------------ Toggle Fullscreen --------------------------------- //

	if (Aen::Input::KeyDown(Aen::Key::F1)) {
		m_toggleFullScreen = !m_toggleFullScreen;
		Aen::WindowDesc wDesc;

		if (m_toggleFullScreen) {
			wDesc.width = GetSystemMetrics(SM_CXSCREEN) + 4u;
			wDesc.height = GetSystemMetrics(SM_CYSCREEN) + 4u;
			screenSize.x = wDesc.width;
			screenSize.y = wDesc.height;
			wDesc.EXStyle = AEN_WS_EX_APPWINDOW;
			wDesc.style = AEN_WS_POPUPWINDOW | AEN_WS_VISIBLE;
			m_Window.LoadSettings(wDesc);
		}
		else {
			wDesc.width = static_cast<UINT>(GetSystemMetrics(SM_CXSCREEN) * 0.4f);
			wDesc.height = static_cast<UINT>(GetSystemMetrics(SM_CYSCREEN) * 0.4f);
			screenSize.x = wDesc.width;
			screenSize.y = wDesc.height;
			wDesc.EXStyle = AEN_WS_EX_APPWINDOW;
			wDesc.style = AEN_WS_OVERLAPPEDWINDOW | AEN_WS_VISIBLE;
			m_Window.LoadSettings(wDesc);
		}
	}

	//Aen::GlobalSettings::SetPlayerPos(m_player.GetEntity()->GetPos());
	
	// ------------------------------ Quick Exit Button -------------------------------- //

	// ------------------------------------- States -------------------------------------- //
	//if (m_hp <= 0 && m_enemyQueue.size() == 0)
	//{
	//	State::SetState(States::Gameover);
	//}
}