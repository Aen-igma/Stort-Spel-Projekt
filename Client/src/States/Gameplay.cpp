#include "Gameplay.h"

Gameplay::Gameplay(Aen::Window& window)
	:State(window), m_speed(10.f), m_fSpeed(0.15f), m_mouseSense(5.f), m_toggleFullScreen(true), m_targetDist(25.f), m_movementSpeed(4.f),
	m_finalDir(0.f, 0.f, -1.f), m_hp(2), IFRAMEMAX(2.f), m_iFrames(0.f) {}

Gameplay::~Gameplay() {
	Aen::GlobalSettings::RemoveMainCamera();
	Aen::EntityHandler::RemoveEntity(*m_camera);
	Aen::EntityHandler::RemoveEntity(*m_dLight);
	Aen::EntityHandler::RemoveEntity(*m_plane);
	Aen::EntityHandler::RemoveEntity(*m_player);
	Aen::EntityHandler::RemoveEntity(*m_reimube);
}

void Gameplay::Initialize()
{
	State::SetLoad(false);

	// ----------------------------- UI -------------------------------- //
	//UI  = &Aen::EntityHandler::CreateEntity();
	//UI->AddComponent<Aen::UIComponent>();
	//UI->GetComponent<Aen::UIComponent>().AddButton();

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
	Aen::Mesh& reimube = Aen::Resource::CreateMesh("Reimube");
	reimube.Load(AEN_RESOURCE_DIR("Cube.fbx"));

	// -------------------------- Setup Material -------------------------------- //

	Aen::Material& planeMat = Aen::Resource::CreateMaterial("PlaneMaterial");
	Aen::Material& playerMat = Aen::Resource::CreateMaterial("PlayerMaterial");
	Aen::Material& enemyMat = Aen::Resource::CreateMaterial("EnemyMaterial");
	Aen::Material& reimubeMat = Aen::Resource::CreateMaterial("ReimubeMat");

	enemyMat["InnerEdgeColor"] = Aen::Color::Red;
	enemyMat["OuterEdgeColor"] = Aen::Color::Red;
	enemyMat["BaseColor"] = Aen::Color::Red;

	reimubeMat.LoadeAndSetDiffuseMap(AEN_RESOURCE_DIR("greenMage.png"));
	reimubeMat["InnerEdgeColor"] = Aen::Color::Pink;
	reimubeMat["OuterEdgeColor"] = Aen::Color::Pink;

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
	m_player->AddComponent<Aen::AABoundBox>();
	m_player->GetComponent<Aen::AABoundBox>().CreateAABB();
	m_player->SetPos(0.f, 1.f, 0.f);

	m_reimube = &Aen::EntityHandler::CreateEntity();
	//m_reimube->AddComponent<Aen::RigidBody>();
	//m_reimube->GetComponent<Aen::RigidBody>().SetGeometry(Aen::GeometryType::CUBE, Aen::Vec3f(2.f, 2.f, 2.f));
	//m_reimube->GetComponent<Aen::RigidBody>().SetRigidType(Aen::RigidType::STATIC);
	m_reimube->AddComponent<Aen::MeshInstance>();
	m_reimube->GetComponent<Aen::MeshInstance>().SetMesh(reimube);
	m_reimube->GetComponent<Aen::MeshInstance>().SetMaterial(reimubeMat);
	m_reimube->AddComponent<Aen::AABoundBox>();
	m_reimube->GetComponent<Aen::AABoundBox>().CreateAABB();
	m_reimube->SetPos(0.f, 1.f, -3.f);

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

void Gameplay::Update(const float& deltaTime) {

	// Collision

	m_player->GetComponent<Aen::AABoundBox>().Update();
	m_reimube->GetComponent<Aen::AABoundBox>().Update();

	if (m_player->GetComponent<Aen::AABoundBox>().Intersects(m_reimube->GetComponent<Aen::AABoundBox>()))
	{
		m_hp--;
		m_player->GetComponent<Aen::AABoundBox>().ToggleActive(false);
		m_invincible = true;
	}

	cout << m_hp << endl;

	// Invincible frames
	if (m_invincible && m_iFrames <= IFRAMEMAX)
	{
		m_iFrames += deltaTime;
	}
	else 
	{
		m_player->GetComponent<Aen::AABoundBox>().ToggleActive(true);
		m_iFrames = 0.f;
		m_invincible = false;
	}

	if (m_hp <= 0)
	{
		State::SetState(States::Gameover);
	}


	static Aen::Vec3f axis;
	Aen::Vec3f targetDir(0.f, 0.f, -1.f);
	static bool lockedOn = false;
	auto enemies = Aen::EntityHandler::GetTagedEntities("Enemy");

	static Aen::Vec3f camDir;
	static Aen::Vec2f side;
	if(lockedOn)
		side.x = Aen::Lerp(side.x, axis.x, 0.05f);
	else
		side.x = Aen::Lerp(side.x, axis.x * 0.3f, 0.05f);
	side.y = Aen::Lerp(side.y, axis.z, 0.15f);

	// --------------------------- Raw Mouse and scroll Input --------------------------- //

	while (!Aen::Input::MouseBufferIsEmbty())
	{
		Aen::MouseEvent me = Aen::Input::ReadEvent();

		if (me.getInputType() == Aen::MouseEvent::RAW_MOVE)
		{
			if(!Aen::Input::GPGetActive(0u) && !lockedOn) {
				m_camera->Rotate(
					-(float)me.GetPos().y * m_mouseSense * deltaTime,
					-(float)me.GetPos().x * m_mouseSense * deltaTime, 0.f);
			}
		}
		if (me.getInputType() == Aen::MouseEvent::SCROLL_UP) {
			printf("scroll up\n");

		}
		else if (me.getInputType() == Aen::MouseEvent::SCROLL_DOWN) {
			printf("scroll down\n");

		}
	}

	// ------------------------------ Player Controler ---------------------------------- //

	

	if(Aen::Input::GPGetActive(0u)) {
		axis.x = -Aen::Input::GPGetAnalog(0u, Aen::Analog::LTHUMB).x;
		axis.z = Aen::Input::GPGetAnalog(0u, Aen::Analog::LTHUMB).y;

		m_camera->Rotate(
			Aen::Input::GPGetAnalog(0u, Aen::Analog::RTHUMB).y * m_mouseSense * 18.f * deltaTime,
			-Aen::Input::GPGetAnalog(0u, Aen::Analog::RTHUMB).x * m_mouseSense * 28.f * deltaTime, 0.f);

		// Dash/Dodge

		static bool toggle = false;
		static bool lTriggerPressed = false;

		if(Aen::Input::GPGetAnalog(0u, Aen::Analog::TRIGGER).x > 0.f && !toggle) {
			lTriggerPressed = true;
			toggle = true;
		} else if(Aen::Input::GPGetAnalog(0u, Aen::Analog::TRIGGER).x <= 0.f)
			toggle = false;

		if(lTriggerPressed) {
			lTriggerPressed = false;
			EventData data;
			data.accell = 14.f;
			data.duration = 0.3f;
			data.function = [&](float& accell) {
				m_player->GetComponent<Aen::CharacterController>().Move(m_finalDir * accell * deltaTime, deltaTime);
				accell -= 30.f * deltaTime;
			};

			m_eventQueue.emplace(data);
		}

		// Attack

		if(Aen::Input::GPKeyDown(0u, Aen::GP::A)) {
			EventData data;
			data.accell = 6.f;
			data.duration = 0.2f;
			data.function = [&](float& accell) {
				if(lockedOn) {
					Aen::Vec2f d2(Aen::Vec2f(camDir.x, camDir.z).Normalized());
					Aen::Vec3f d(d2.x, 0.f, d2.y);
					m_finalDir = Aen::Lerp(m_finalDir, d, 0.6f);
				}

				m_player->GetComponent<Aen::CharacterController>().Move(m_finalDir * accell * deltaTime, deltaTime);
				accell -= 12.f * deltaTime;
			};

			m_eventQueue.emplace(data);
		}

		// Lock On Target


		if(Aen::Input::GPKeyDown(0u, Aen::GP::LSHOULDER)) {
			lockedOn = !lockedOn;

			if(lockedOn) {
				m_targetDist = 25.f;
				for(auto i = enemies.first; i != enemies.second; i++) {
					Aen::Entity* enemy = i->second;
					Aen::Vec3f eDir = m_player->GetPos() - enemy->GetPos();
					float dist = eDir.Magnitude();
					if(dist < m_targetDist) {
						m_targetDist = dist;
						m_target = enemy;
					}
				}
			}
		}

	} else {
		axis.x = (float)Aen::Input::KeyPress(Aen::Key::A) - (float)Aen::Input::KeyPress(Aen::Key::D);
		axis.z = (float)Aen::Input::KeyPress(Aen::Key::W) - (float)Aen::Input::KeyPress(Aen::Key::S); 

		// Dash/Dodge

		if(Aen::Input::KeyDown(Aen::Key::SPACE)) {
			EventData data;
			data.accell = 14.f;
			data.duration = 0.4f;
			data.function = [&](float& accell) {
				m_player->GetComponent<Aen::CharacterController>().Move(m_finalDir * accell * deltaTime, deltaTime);
				accell -= 30.f * deltaTime;
			};

			m_eventQueue.emplace(data);
		}

		// Attack

		if(Aen::Input::KeyDown(Aen::Key::LMOUSE)) {
			EventData data;
			data.accell = 6.f;
			data.duration = 0.2f;
			data.function = [&](float& accell) {
				if(lockedOn) {
					Aen::Vec2f d2(Aen::Vec2f(camDir.x, camDir.z).Normalized());
					Aen::Vec3f d(d2.x, 0.f, d2.y);
					m_finalDir = Aen::Lerp(m_finalDir, d, 0.6f);
				}

				m_player->GetComponent<Aen::CharacterController>().Move(m_finalDir * accell * deltaTime, deltaTime);
				accell -= 12.f * deltaTime;
			};

			m_eventQueue.emplace(data);
		}

		// Lock On Target

		if(Aen::Input::KeyDown(Aen::Key::E)) {
			lockedOn = !lockedOn;

			if(lockedOn) {
				m_targetDist = 25.f;
				for(auto i = enemies.first; i != enemies.second; i++) {
					Aen::Entity* enemy = i->second;
					Aen::Vec3f eDir = m_player->GetPos() - enemy->GetPos();
					float dist = eDir.Magnitude();
					if(dist < m_targetDist) {
						m_targetDist = dist;
						m_target = enemy;
					}
				}
			}
		}
	}

	if(m_targetDist < 20.f && m_target && lockedOn) {
		Aen::Vec3f tDir = ((m_player->GetPos() + Aen::Vec3f(0.f, 1.f, 0.f)) - m_target->GetPos() + (camDir % Aen::Vec3f(0.f, 1.f, 0.f)).Normalized() * side.x).Normalized();
		float yaw = Aen::RadToDeg(std::atan2(tDir.x, tDir.z));
		float pitch = Aen::RadToDeg(std::acos(tDir * Aen::Vec3f(0.f, 1.f, 0.f))) - 90.f;

		m_camera->SetRot(pitch, yaw, 0.f);

		if(m_target) {
			Aen::Vec3f eDir = m_player->GetPos() - m_target->GetPos();
			if(eDir.Magnitude() > 20.f) lockedOn = false;
		}

	} else {
		lockedOn = false;
		m_targetDist = 25.f;
	}

	float r = Aen::Clamp(m_camera->GetRot().x, -45.f, 45.f);
	m_camera->SetRot(r, m_camera->GetRot().y, m_camera->GetRot().z);
	camDir = Aen::Lerp(camDir, Aen::Transform(m_camera->GetComponent<Aen::Rotation>().GetTranform(), targetDir).Normalized(), 0.6f).Normalized();
	
	m_ray.SetOrigin(m_player->GetPos() - camDir * 0.8f);
	m_ray.SetDirection(-camDir);
	m_ray.SetMaxDist(5.f);
	m_ray.Update();

	m_camera->SetPos(Aen::Lerp(m_camera->GetPos(), m_player->GetPos() + Aen::Vec3f(0.f, 0.8f, 0.f) + camDir * (-m_ray.GetDistance() - side.y) + (camDir % Aen::Vec3f(0.f, 1.f, 0.f)).Normalized() * 1.25f * side.x, 0.6f));
	m_camera->GetComponent<Aen::Camera>().LookTowards(camDir);
	m_player->GetComponent<Aen::CharacterController>().Move(Aen::Vec3f(0.f, -1.f, 0.f) * deltaTime, deltaTime);

	if (m_toggleFullScreen)
		Aen::Input::SetMousePos((Aen::Vec2i)Aen::Vec2f(GetSystemMetrics(SM_CXSCREEN) * 0.5f, GetSystemMetrics(SM_CYSCREEN) * 0.5f));
	else
		Aen::Input::SetMousePos(m_Window.GetWindowPos() + (Aen::Vec2i)((Aen::Vec2f)m_Window.GetSize() * 0.5f));
	
	Aen::Vec3f playerDir = m_camera->GetComponent<Aen::Camera>().GetForward() * axis.Normalized().z + m_camera->GetComponent<Aen::Camera>().GetRight() * axis.Normalized().x;
	Aen::Vec2f dir(playerDir.x, playerDir.z);

	if(!m_eventQueue.empty())
		if(m_eventQueue.front().duration > 0.f) {
			m_eventQueue.front().function(m_eventQueue.front().accell);
			m_eventQueue.front().duration -= deltaTime;
		} else {
			if(axis.Magnitude() > 0.f) m_finalDir = Aen::Vec3f(dir.Normalized().x, 0.f, dir.Normalized().y);
			m_eventQueue.pop();
		}

	if(m_eventQueue.empty()) {
		if(axis.Magnitude() > 0.f) {
			m_finalDir = Aen::Vec3f(dir.Normalized().x, 0.f, dir.Normalized().y);
			m_player->GetComponent<Aen::CharacterController>().Move(m_finalDir * m_movementSpeed * deltaTime, deltaTime);
		} else {
			Aen::Vec2f dir(camDir.x, camDir.z);
			m_finalDir = Aen::Vec3f(dir.Normalized().x, 0.f, dir.Normalized().y);
		}
	}

	// ---------------------------------- Enemies --------------------------------------- //

	for(auto i = enemies.first; i != enemies.second; i++) {
		Aen::Entity* enemy = i->second;
		Aen::Vec3f eDir = m_player->GetPos() - enemy->GetPos();
		float dist = eDir.Magnitude();
		if(dist  < 8.f)
			enemy->GetComponent<Aen::CharacterController>().Move(eDir.Normalized() * 3.f * deltaTime, deltaTime);
	}

	if(Aen::Input::KeyDown(Aen::Key::J)) {
		Aen::Entity* enemy = &Aen::EntityHandler::CreateEntity();
		m_enemyQueue.emplace(enemy);
		enemy->SetTag("Enemy");
		enemy->AddComponent<Aen::MeshInstance>();
		enemy->GetComponent<Aen::MeshInstance>().SetMesh("Capsule");
		enemy->GetComponent<Aen::MeshInstance>().SetMaterial("EnemyMaterial");
		enemy->AddComponent<Aen::CharacterController>();
		enemy->SetPos(0.f, 1.5f, 5.f);

		enemy = nullptr;
	}

	if (Aen::Input::KeyDown(Aen::Key::O) && !lockedOn) {
		Aen::EntityHandler::RemoveEntity(*m_enemyQueue.front());
		m_enemyQueue.pop();
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
	if (Aen::Input::KeyDown(Aen::Key::ENTER) && m_enemyQueue.size() == 0)
	{
		State::SetState(States::Gameover);
	}
}