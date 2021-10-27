#include "Gameplay.h"

Gameplay::Gameplay(Aen::Window& window)
	:State(window), m_speed(10.f), m_fSpeed(0.15f), m_mouseSense(5.f), m_toggleFullScreen(false), m_movementSpeed(4.f),
	m_finalDir(0.f, 0.f, -1.f) {}

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
	Aen::Mesh& reimube = Aen::Resource::CreateMesh("Reimube");
	reimube.Load(AEN_RESOURCE_DIR("Cube.fbx"));

	// -------------------------- Setup Material -------------------------------- //

	Aen::Material& planeMat = Aen::Resource::CreateMaterial("PlaneMaterial");
	Aen::Material& playerMat = Aen::Resource::CreateMaterial("PlayerMaterial");
	Aen::Material& reimubeMat = Aen::Resource::CreateMaterial("ReimubeMat");

	planeMat["InnerEdgeColor"] = Aen::Color::Red;
	planeMat["OuterEdgeColor"] = Aen::Color::Red;

	planeMat["BaseColor"] = Aen::Color::White;
	playerMat["BaseColor"] = Aen::Color::White;

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
	m_player->SetPos(0.f, 1.f, 0.f);

	m_reimube = &Aen::EntityHandler::CreateEntity();
	m_reimube->AddComponent<Aen::RigidBody>();
	m_reimube->GetComponent<Aen::RigidBody>().SetGeometry(Aen::GeometryType::CUBE, Aen::Vec3f(2.f, 2.f, 2.f));
	m_reimube->GetComponent<Aen::RigidBody>().SetRigidType(Aen::RigidType::STATIC);
	m_reimube->AddComponent<Aen::MeshInstance>();
	m_reimube->GetComponent<Aen::MeshInstance>().SetMesh(reimube);
	m_reimube->GetComponent<Aen::MeshInstance>().SetMaterial(reimubeMat);
	m_reimube->SetPos(0.f, 1.f, 0.f);

	// --------------------------- Setup Window --------------------------------- //

	m_Window.SetWindowSize(static_cast<UINT>(GetSystemMetrics(SM_CXSCREEN) * 0.4f), static_cast<UINT>(GetSystemMetrics(SM_CYSCREEN) * 0.4f));

	Aen::Input::ToggleRawMouse(true);
	Aen::Input::SetMouseVisible(false);
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
			if(!Aen::Input::GPGetActive(0u)) {
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

	Aen::Vec3f axis;

	if(Aen::Input::GPGetActive(0u)) {
		axis.x = Aen::Input::GPGetAnalog(0u, Aen::Analog::LTHUMB).x;
		axis.z = -Aen::Input::GPGetAnalog(0u, Aen::Analog::LTHUMB).y;

		m_camera->Rotate(
			-Aen::Input::GPGetAnalog(0u, Aen::Analog::RTHUMB).y * m_mouseSense * 18.f * deltaTime,
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
			data.duration = 0.1f;
			data.function = [&](float& accell) {
				m_player->GetComponent<Aen::CharacterController>().Move(m_finalDir * accell * deltaTime, deltaTime);
				accell -= 12.f * deltaTime;
			};

			m_eventQueue.emplace(data);
		}

	} else {
		axis.x = (float)Aen::Input::KeyPress(Aen::Key::D) - (float)Aen::Input::KeyPress(Aen::Key::A);
		axis.z = (float)Aen::Input::KeyPress(Aen::Key::S) - (float)Aen::Input::KeyPress(Aen::Key::W); 

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
			data.duration = 0.1f;
			data.function = [&](float& accell) {
				m_player->GetComponent<Aen::CharacterController>().Move(m_finalDir * accell * deltaTime, deltaTime);
				accell -= 12.f * deltaTime;
			};

			m_eventQueue.emplace(data);
		}
	}

	float r = Aen::Clamp(m_camera->GetRot().x, -45.f, 45.f);
	m_camera->SetRot(r, m_camera->GetRot().y, m_camera->GetRot().z);

	Aen::Vec3f camDir = Aen::Transform(m_camera->GetComponent<Aen::Rotation>().GetTranform(), Aen::Vec3f(0.f, 0.f, -1.f)).Normalized();
	camDir = Aen::Transform(m_camera->GetComponent<Aen::Rotation>().GetTranform(), Aen::Vec3f(0.f, 0.f, -1.f)).Normalized();
	m_camera->SetPos(Aen::Lerp(m_camera->GetPos(), m_player->GetPos() + Aen::Vec3f(0.f, 0.5f, 0.f) + camDir * -4.f, 0.3f));
	m_camera->GetComponent<Aen::Camera>().LookTowards(camDir);
	m_player->GetComponent<Aen::CharacterController>().Move(Aen::Vec3f(0.f, -1.f, 0.f) * deltaTime, deltaTime);

	if (m_toggleFullScreen)
		Aen::Input::SetMousePos((Aen::Vec2i)Aen::Vec2f(GetSystemMetrics(SM_CXSCREEN) * 0.5f, GetSystemMetrics(SM_CYSCREEN) * 0.5f));
	else
		Aen::Input::SetMousePos(m_Window.GetWindowPos() + (Aen::Vec2i)((Aen::Vec2f)m_Window.GetSize() * 0.5f));
	
	Aen::Vec3f playerDir = Aen::Transform(m_camera->GetComponent<Aen::Rotation>().GetTranform(), axis.Normalized());
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
}