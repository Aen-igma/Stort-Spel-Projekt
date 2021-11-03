#include"Player.h"

Player::Player()
	:m_player(&Aen::EntityHandler::CreateEntity()), m_camera(&Aen::EntityHandler::CreateEntity()),
	m_mouseSense(5.f), m_targetDist(25.f), m_movementSpeed(6.f), m_finalDir(0.f, 0.f, -1.f){
	
	m_camera = &Aen::EntityHandler::CreateEntity();
	m_camera->AddComponent<Aen::Camera>();
	m_camera->GetComponent<Aen::Camera>().SetCameraPerspective(70.f, Aen::GlobalSettings::GetWindow()->GetAspectRatio(), 0.01f, 200.f);
	m_camera->SetPos(0.f, 2.f, -2.f);

	Aen::GlobalSettings::SetMainCamera(*m_camera);

	Aen::Mesh& capsule = Aen::Resource::CreateMesh("Capsule");
	capsule.Load(AEN_RESOURCE_DIR("Capsule.fbx"));

	Aen::Material& playerMat = Aen::Resource::CreateMaterial("PlayerMaterial");

	m_player->AddComponent<Aen::CharacterController>();
	m_player->AddComponent<Aen::MeshInstance>();
	m_player->GetComponent<Aen::MeshInstance>().SetMesh(capsule);
	m_player->GetComponent<Aen::MeshInstance>().SetMaterial(playerMat);
	m_player->AddComponent<Aen::AABoundBox>();
	m_player->GetComponent<Aen::AABoundBox>().SetBoundsToMesh();
	m_player->SetPos(0.f, 1.f, 0.f);
}

Player::~Player() {
	Aen::EntityHandler::RemoveEntity(*m_player);
	Aen::EntityHandler::RemoveEntity(*m_camera);
}

void Player::Update(const float& deltaTime) {

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
					(float)me.GetPos().x * m_mouseSense * deltaTime, 0.f);
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
		axis.x = Aen::Input::GPGetAnalog(0u, Aen::Analog::LTHUMB).x;
		axis.z = Aen::Input::GPGetAnalog(0u, Aen::Analog::LTHUMB).y;

		m_camera->Rotate(
			Aen::Input::GPGetAnalog(0u, Aen::Analog::RTHUMB).y * m_mouseSense * 18.f * deltaTime,
			Aen::Input::GPGetAnalog(0u, Aen::Analog::RTHUMB).x * m_mouseSense * 28.f * deltaTime, 0.f);

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
			data.accell = 16.f;
			data.duration = 0.3f;
			data.function = [&](float& accell) {
				m_player->GetComponent<Aen::CharacterController>().Move(m_finalDir * accell * deltaTime, deltaTime);
				accell -= 25.f * deltaTime;
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
		axis.x = (float)Aen::Input::KeyPress(Aen::Key::D) - (float)Aen::Input::KeyPress(Aen::Key::A);
		axis.z = (float)Aen::Input::KeyPress(Aen::Key::W) - (float)Aen::Input::KeyPress(Aen::Key::S); 

		// Dash/Dodge

		if(Aen::Input::KeyDown(Aen::Key::SPACE)) {
			EventData data;
			data.accell = 16.f;
			data.duration = 0.4f;
			data.function = [&](float& accell) {
				m_player->GetComponent<Aen::CharacterController>().Move(m_finalDir * accell * deltaTime, deltaTime);
				accell -= 25.f * deltaTime;
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
	m_player->GetComponent<Aen::CharacterController>().Move(Aen::Vec3f(0.f, -20.f, 0.f) * deltaTime, deltaTime);

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
}

Aen::Entity*& Player::GetEntity() {
	return m_player;
}
