#include"Player.h"
#include "Enemy\Boss\NecroBoss.h"

bool Player::m_healing{ false };

Player::Player()
	:m_player(&Aen::EntityHandler::CreateEntity()), m_camera(&Aen::EntityHandler::CreateEntity()),
	m_hurtbox(&Aen::EntityHandler::CreateEntity()), m_health(200.f), m_potion(80.f), m_potionCap(3), m_nrPotion(m_potionCap),m_timer(0),
	m_sword(&Aen::EntityHandler::CreateEntity()), m_playerMeshHolder(&Aen::EntityHandler::CreateEntity()),
	m_mouseSense(5.f), m_movementSpeed(8.f), m_finalDir(0.f, 0.f, -1.f),
	m_LIGHTATTACKTIME(.4f), m_HEAVYATTACKTIME(1.f), m_attackTimer(0.f),
	m_LIGHTCHARGETIME(0.f), m_HEAVYCHARGETIME(.5f),
	m_LIGHTATTACKSPEED(6.0f), m_HEAVYATTACKSPEED(2.54f)
{
	m_player->SetTag("Player");
	m_camera = &Aen::EntityHandler::CreateEntity();
	m_camera->AddComponent<Aen::Camera>();
	m_camera->GetComponent<Aen::Camera>().SetCameraPerspective(70.f, Aen::GlobalSettings::GetWindow()->GetAspectRatio(), 0.01f, 60.f);

	Aen::GlobalSettings::SetMainCamera(*m_camera);

	Aen::Mesh& sword = Aen::Resource::CreateMesh("Sword");
	sword.Load(AEN_MODEL_DIR("ShortSword.fbx"));

	Aen::Mesh* protag = &Aen::Resource::CreateMesh("Protag");
	protag->Load(AEN_MODEL_DIR("Protagonist.fbx"));

	Aen::Material& skin = Aen::Resource::CreateMaterial("Skin");
	skin["InnerEdgeColor"] = Aen::Color(0.3f, 0.1f, 0.08f, 1.f);
	skin["OuterEdgeColor"] = Aen::Color(0.13f, 0.1f, 0.08f, 1.f);
	skin["InnerEdgeThickness"] = 1;
	skin["OuterEdgeThickness"] = 2;
	skin["BaseColor"] = Aen::Color(0.8f, 0.67f, 0.6f, 1.f);
	skin["SpecularColor"] = Aen::Color::Red;
	skin["RimLightColor"] = Aen::Color(0.5f, 0.f, 0.f, 1.f);
	skin["RimLightIntensity"] = 0.8f;
	skin["RimLightSize"] = 0.4f;

	Aen::Material& shirt = Aen::Resource::CreateMaterial("Shirt");
	shirt["InnerEdgeColor"] = Aen::Color(0.1f, 0.08f, 0.05f, 1.f);
	shirt["OuterEdgeColor"] = Aen::Color(0.1f, 0.08f, 0.05f, 1.f);
	shirt["InnerEdgeThickness"] = 1;
	shirt["OuterEdgeThickness"] = 2;
	shirt["BaseColor"] = Aen::Color(0.78f, 0.77f, 0.69f, 1.f);
	shirt["RimLightColor"] = Aen::Color(0.5f, 0.f, 0.f, 1.f);
	shirt["RimLightIntensity"] = 0.8f;
	shirt["RimLightSize"] = 0.3f;

	Aen::Material& brown = Aen::Resource::CreateMaterial("Brown");
	brown["InnerEdgeColor"] = Aen::Color(0.13f, 0.014f, 0.012f, 1.f);
	brown["OuterEdgeColor"] = Aen::Color(0.032f, 0.017f, 0.012f, 1.f);
	brown["InnerEdgeThickness"] = 1;
	brown["OuterEdgeThickness"] = 2;
	brown["BaseColor"] = Aen::Color(0.23f, 0.17f, 0.16f, 1.f);
	brown["RimLightColor"] = Aen::Color(0.8f, 0.2f, 0.1f, 1.f);
	brown["RimLightIntensity"] = 1.f;
	brown["RimLightSize"] = 0.6f;

	Aen::Material& pants = Aen::Resource::CreateMaterial("Pants");
	pants["InnerEdgeColor"] = Aen::Color(0.06f, 0.07f, 0.07f, 1.f);
	pants["OuterEdgeColor"] = Aen::Color(0.06f, 0.07f, 0.07f, 1.f);
	pants["InnerEdgeThickness"] = 1;
	pants["OuterEdgeThickness"] = 2;
	pants["BaseColor"] = Aen::Color(0.44f, 0.41f, 0.34f, 1.f);

	Aen::Material& metal = Aen::Resource::CreateMaterial("Metal");
	metal["InnerEdgeColor"] = Aen::Color(0.04f, 0.04f, 0.07f, 1.f);
	metal["OuterEdgeColor"] = Aen::Color(0.04f, 0.04f, 0.07f, 1.f);
	metal["InnerEdgeThickness"] = 1;
	metal["OuterEdgeThickness"] = 2;
	metal["BaseColor"] = Aen::Color(0.32f, 0.36f, 0.4f, 1.f);
	metal["Roughness"] = 0.f;

	Aen::Material& shadow = Aen::Resource::CreateMaterial("Shadow");
	shadow["InnerEdgeColor"] = Aen::Color(0.1f, 0.03f, 0.01f, 1.f);
	shadow["OuterEdgeColor"] = Aen::Color(0.13f, 0.1f, 0.08f, 1.f);
	shadow["InnerEdgeThickness"] = 1;
	shadow["OuterEdgeThickness"] = 1;
	shadow["BaseColor"] = Aen::Color(0.8f, 0.67f, 0.6f, 1.f);
	shadow["ShadowColor"] = Aen::Color(0.3f, 0.2f, 0.2f, 1.f);
	shadow["ShadowOffset"] = 1.f;
	shadow["SpecularColor"] = Aen::Color::Red;

	Aen::Material& playerMat = Aen::Resource::CreateMaterial("PlayerMaterial");
	Aen::Material& swordMat = Aen::Resource::CreateMaterial("SwordMaterial");

	m_player->AddComponent<Aen::CharacterController>();
	mp_charCont = &m_player->GetComponent<Aen::CharacterController>();
	mp_charCont->Resize(2.3f);

	Aen::Animation& protagIdle = Aen::Resource::CreateAnimation("protagIdle");
	protagIdle.LoadAnimation(AEN_MODEL_DIR("Protagonist_Idle.fbx"));
	Aen::Animation& protagRun = Aen::Resource::CreateAnimation("protagRun");
	protagRun.LoadAnimation(AEN_MODEL_DIR("Protagonist_Run.fbx"));
	Aen::Animation& protagDash = Aen::Resource::CreateAnimation("protagDash");
	protagDash.LoadAnimation(AEN_MODEL_DIR("Protagonist_Dash.fbx"));
	Aen::Animation& protagAttack = Aen::Resource::CreateAnimation("protagAttack");
	protagAttack.LoadAnimation(AEN_MODEL_DIR("Protagonist_Attack.fbx"));

	m_playerMeshHolder->AddComponent<Aen::MeshInstance>();
	m_playerMeshHolder->GetComponent<Aen::MeshInstance>().SetMesh(*protag);
	m_playerMeshHolder->GetComponent<Aen::MeshInstance>().SetMaterial("Skin1", skin);
	m_playerMeshHolder->GetComponent<Aen::MeshInstance>().SetMaterial("Shirt1", shirt);
	m_playerMeshHolder->GetComponent<Aen::MeshInstance>().SetMaterial("Brown1", brown);
	m_playerMeshHolder->GetComponent<Aen::MeshInstance>().SetMaterial("Pants1", pants);
	m_playerMeshHolder->GetComponent<Aen::MeshInstance>().SetMaterial("Metal1", metal);
	m_playerMeshHolder->GetComponent<Aen::MeshInstance>().SetMaterial("Shadow1", shadow);
	m_playerMeshHolder->AddComponent<Aen::Animator>();
	m_playerMeshHolder->GetComponent<Aen::Animator>().AddAnimation(protagIdle, "Idle");
	m_playerMeshHolder->GetComponent<Aen::Animator>().AddAnimation(protagRun, "Run");
	m_playerMeshHolder->GetComponent<Aen::Animator>().AddAnimation(protagDash, "Dash");
	m_playerMeshHolder->GetComponent<Aen::Animator>().AddAnimation(protagAttack, "Attack");
	m_playerMeshHolder->GetComponent<Aen::Animator>().SetAnimation("Idle");
	m_playerMeshHolder->GetComponent<Aen::Animator>().SetAnimationScale(0.28f);
	m_playerMeshHolder->GetComponent<Aen::Animator>().SetFrameRate(24);
	m_playerMeshHolder->SetParent(*m_player);
	m_playerMeshHolder->SetPos(0.f, -3.1f, 0.f);

	m_player->AddComponent<Aen::AABoundBox>();
	mp_hitBox = &m_player->GetComponent<Aen::AABoundBox>();
	mp_hitBox->SetBoundingBox(0.45f,1.1f,0.45f);
	//m_player->SetPos(0.f, -1.f, 0.f);
	m_player->SetTag("Player");

	m_sword->AddComponent<Aen::MeshInstance>();
	m_sword->GetComponent<Aen::MeshInstance>().SetMesh(sword);
	m_sword->GetComponent<Aen::MeshInstance>().SetMaterial(swordMat);
	m_sword->SetPos(-0.65f, 1.45f, 0.f);

	m_hurtbox->AddComponent<Aen::OBBox>();
	mp_hurtBox = &m_hurtbox->GetComponent<Aen::OBBox>();
	mp_hurtBox->SetBoundingBox(1.f, 1.f, 1.0);
	mp_hurtBox->SetOffset(0.f, 0.f, 0.f);
	mp_hurtBox->ToggleActive(false);
	m_hurtbox->SetParent(*m_player);


	Aen::Mesh& eBar = Aen::Resource::CreateMesh("eBar");
	eBar.Load(AEN_MODEL_DIR("bar.fbx"));

	Aen::Material& barMat = Aen::Resource::CreateMaterial("barMat");
	Aen::Material& targetMat = Aen::Resource::CreateMaterial("targetMat");

	targetMat.LoadeAndSetDiffuseMap(AEN_TEXTURE_DIR("target1.png"));
	targetMat.LoadeAndSetOpacityMap(AEN_TEXTURE_DIR("target1.png"));
	targetMat.LoadeAndSetEmissionMap(AEN_TEXTURE_DIR("target1.png"));
	targetMat["InnerEdgeThickness"] = 0;
	targetMat["OuterEdgeColor"] = Aen::Color::Red;
	targetMat["GlowColor"] = Aen::Color::Red;

	barMat.LoadeAndSetDiffuseMap(AEN_TEXTURE_DIR("enemybar.png"));
	barMat.LoadeAndSetOpacityMap(AEN_TEXTURE_DIR("opacBar.png"));
	barMat.LoadeAndSetEmissionMap(AEN_TEXTURE_DIR("enemybar.png"));
	//barMat["InnerEdgeThickness"] = 0;
	barMat["GlowColor"] = Aen::Color::Red;
	barMat["InnerEdgeColor"] = Aen::Color::Red;
	barMat["OuterEdgeColor"] = Aen::Color::Yellow;

	m_targetUI = &Aen::EntityHandler::CreateEntity();
	m_targetUI->AddComponent<Aen::MeshInstance>();
	m_targetUI->GetComponent<Aen::MeshInstance>().SetMesh("eBar");
	m_targetUI->GetComponent<Aen::MeshInstance>().SetMaterial("targetMat");
	m_targetUI->SetScale(0, 0, 0);
	m_targetUI->SetRenderLayer(2);
}

Player::~Player() {
	Aen::GlobalSettings::RemoveMainCamera();
	m_playerMeshHolder->RemoveParent();
	Aen::EntityHandler::RemoveEntity(*m_playerMeshHolder);
	Aen::EntityHandler::RemoveEntity(*m_player);
	Aen::EntityHandler::RemoveEntity(*m_camera);
	m_sword->RemoveParent();
	Aen::EntityHandler::RemoveEntity(*m_sword);
	Aen::EntityHandler::RemoveEntity(*m_hurtbox);
	Aen::EntityHandler::RemoveEntity(*m_targetUI);
}

void Player::Update(std::deque<Enemy*>& e, const float& deltaTime) {

	static Aen::Vec3f axis;
	static Aen::Vec3f targetDir(0.f, 0.f, -1.f);
	static bool lockedOn = false;

	static Aen::Vec2f dir;
	static Aen::Vec3f camDir;
	static Aen::Vec2f side;
	if (lockedOn)
		side.x = Aen::Lerp(side.x, axis.x, 0.05f);
	else
		side.x = Aen::Lerp(side.x, axis.x * 0.3f, 0.05f);
	side.y = Aen::Lerp(side.y, axis.z, 0.15f);


#ifdef _DEBUG
	if (Aen::Input::KeyPress(Aen::Key::SHIFT)) m_movementSpeed = 24.f;
	else m_movementSpeed = 8.f;
#endif // _DEBUG


	m_sword->SetTransformation(m_playerMeshHolder->GetComponent<Aen::Animator>().GetBoneMat(19) * Aen::MatRotate(0.f, -10.f, 0.f) * m_playerMeshHolder->GetTransformation());


	// --------------------------- Raw Mouse and scroll Input --------------------------- //

	while (!Aen::Input::MouseBufferIsEmbty())
	{
		Aen::MouseEvent me = Aen::Input::ReadEvent();

		if (me.getInputType() == Aen::MouseEvent::MouseInput::RAW_MOVE)
		{
			if (!Aen::Input::GPGetActive(0u) && !lockedOn) {
				m_camera->Rotate(
					-(float)me.GetPos().y * m_mouseSense * deltaTime,
					(float)me.GetPos().x * m_mouseSense * deltaTime, 0.f);
			}
		}
		if (me.getInputType() == Aen::MouseEvent::MouseInput::SCROLL_UP) {
			printf("scroll up\n");

		}
		else if (me.getInputType() == Aen::MouseEvent::MouseInput::SCROLL_DOWN) {
			printf("scroll down\n");

		}
	}

	// ------------------------------ Player Controler ---------------------------------- //

	
	axis.x = (float)Aen::Input::KeyPress(Aen::Key::D) - (float)Aen::Input::KeyPress(Aen::Key::A);
	axis.z = (float)Aen::Input::KeyPress(Aen::Key::W) - (float)Aen::Input::KeyPress(Aen::Key::S);

	// Dash/Dodge

	if (Aen::Input::KeyDown(Aen::Key::SPACE)) {
		m_playerMeshHolder->GetComponent<Aen::Animator>().Reset();

		if(m_eventQueue.empty())
			if (axis.Magnitude() > 0.f) 
				m_finalDir = Aen::Vec3f(dir.Normalized().x, 0.f, dir.Normalized().y);
			else {
				Aen::Vec2f dir(camDir.x, camDir.z);
				m_finalDir = Aen::Vec3f(dir.Normalized().x, 0.f, dir.Normalized().y);
			}

		EventData data;
		data.accell = 20.f;
		data.duration = 0.4f;
		data.type = EventType::Dash;
		data.function = [&](float& accell, const float& attackDuration, const int& nrOf) {
			mp_charCont->Move(m_finalDir * accell * deltaTime, deltaTime);
			mp_hitBox->ToggleActive(false);
			accell -= 25.f * deltaTime;
		};

		AddEvent(data);
	}

	// Attack

	if (Aen::Input::KeyDown(Aen::Key::LMOUSE)) {
		m_playerMeshHolder->GetComponent<Aen::Animator>().Reset();

		if(m_eventQueue.empty()) {
			Aen::Vec2f dir(camDir.x, camDir.z);
			m_finalDir = Aen::Vec3f(dir.Normalized().x, 0.f, dir.Normalized().y);
		}

		EventData data;
		data.accell = m_LIGHTATTACKSPEED;
		data.duration = m_LIGHTATTACKTIME;
		data.type = EventType::Attack;
		data.damage = 20.f;
		data.function = [&](float& accell, const float& attackDuration, const int& nrOf) {
			mp_hurtBox->ToggleActive(true);
			SwordSwing(500.f, m_LIGHTATTACKTIME, deltaTime);
			if (lockedOn) {
				Aen::Vec2f d2(Aen::Vec2f(camDir.x, camDir.z).Normalized());
				Aen::Vec3f d(d2.x, 0.f, d2.y);
				m_finalDir = Aen::Lerp(m_finalDir, d, 0.6f);
			}
			mp_charCont->Move(m_finalDir * accell * deltaTime, deltaTime);
			accell -= 12.f * deltaTime;
		};

		AddEvent(data);
	}
	/*if (Aen::Input::KeyDown(Aen::Key::RMOUSE)) {
		EventData data;
		data.accell = m_HEAVYATTACKSPEED;
		data.duration = m_HEAVYATTACKTIME;
		data.type = EventType::Attack;
		data.damage = 40.f;
		data.function = [&](float& accell, const float& attackDuration, const int& nrOf)
		{

			if (lockedOn) {
				Aen::Vec2f d2(Aen::Vec2f(camDir.x, camDir.z).Normalized());
				Aen::Vec3f d(d2.x, 0.f, d2.y);
				m_finalDir = Aen::Lerp(m_finalDir, d, 0.6f);
			}

			mp_charCont->Move(m_finalDir * accell * deltaTime, deltaTime);
			accell -= deltaTime * 2;
			if (attackDuration < m_HEAVYCHARGETIME)
			{
				mp_hurtBox->ToggleActive(true);
				SwordSwing(250.f, m_HEAVYATTACKTIME, deltaTime);
			}
			else
				mp_hurtBox->ToggleActive(false);
		};

		AddEvent(data);
	}*/

	// Lock On Target

	if (Aen::Input::KeyDown(Aen::Key::TAB))
		if (m_targets.size() > 1u && lockedOn) {
			m_targets.front().target->SetISTargeted(false);
			TargetData temp = m_targets.front();
			m_targets.pop_front();
			m_targets.front().target->SetISTargeted(true);
			m_targets.emplace_back(temp);
		}
	
	if (Aen::Input::KeyDown(Aen::Key::E)) {
		lockedOn = !lockedOn;

		if (lockedOn) {
			for (auto i : e)
				i->SetISTargeted(false);

			m_targets.clear();
			for (auto i : e) {
				TargetData data;
				data.target = i;
				Aen::Vec3f eDir = m_player->GetPos() - data.target->GetEntity()->GetPos();
				data.distance = eDir.Magnitude();

				if (data.distance < 20.f)
					m_targets.emplace_back(data);
			}

			for (uint32_t i = 0u; i < m_targets.size(); i++) {

				uint32_t t(i);

				for (uint32_t k = i + 1u; k < m_targets.size(); k++)
					if (m_targets[t].distance > m_targets[k].distance)
						t = k;

				TargetData temp = m_targets[i];
				m_targets[i] = m_targets[t];
				m_targets[t] = temp;
			}



			if (!m_targets.empty())
				m_targets.front().target->SetISTargeted(true);
		}
	}
	

	if (!m_targets.empty() && lockedOn) {
		Aen::Vec3f tDir = ((m_player->GetPos() + Aen::Vec3f(0.f, 1.f, 0.f)) - m_targets.front().target->GetEntity()->GetPos() + (camDir % Aen::Vec3f(0.f, 1.f, 0.f)).Normalized() * side.x).Normalized();
		float yaw = Aen::RadToDeg(std::atan2(tDir.x, tDir.z));
		float pitch = Aen::RadToDeg(std::acos(tDir * Aen::Vec3f(0.f, 1.f, 0.f))) - 90.f;
		//Create UI
		m_targetUI->SetPos(m_targets.front().target->GetEntity()->GetPos());
		m_targetUI->SetScale(5.f, 1.f, 15.f);
		m_targetUI->SetRot(-GetCamera()->GetRot().x - 90.f, GetCamera()->GetRot().y + 180.f, 0);

		m_camera->SetRot(pitch, yaw, 0.f);
		Aen::Vec3f eDir = m_player->GetPos() - m_targets.front().target->GetEntity()->GetPos();
		if (eDir.Magnitude() > 20.f) lockedOn = false;
	}
	else {
		m_targetUI->SetScale(0, 0, 0);
		lockedOn = false;
		for (auto i : e)
			i->SetISTargeted(false);
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

	Aen::Vec3f playerDir = m_camera->GetComponent<Aen::Camera>().GetForward() * axis.Normalized().z + m_camera->GetComponent<Aen::Camera>().GetRight() * axis.Normalized().x;
	dir = Aen::Vec2f(playerDir.x, playerDir.z);

	Aen::Vec3f attackPos =/* m_player->GetPos() +*/ m_finalDir * 2.f;

	m_hurtbox->SetPos(attackPos);

	float yaw = std::atan2(m_finalDir.x, m_finalDir.z);
	float swordYaw = std::atan2(playerDir.x, playerDir.z);

	mp_hurtBox->SetOrientation(0.f, yaw, 0.f);
	m_player->SetRot(0.f, Aen::RadToDeg(yaw) + 180.f, 0.f);
	
	if(axis.Magnitude() > 0.f) {
		m_playerMeshHolder->GetComponent<Aen::Animator>().SetAnimationScale(0.28f);
		m_playerMeshHolder->GetComponent<Aen::Animator>().SetAnimation("Run");
	} else {
		m_playerMeshHolder->GetComponent<Aen::Animator>().SetAnimationScale(0.85f);
		m_playerMeshHolder->GetComponent<Aen::Animator>().SetAnimation("Idle");
	}

	if (!m_eventQueue.empty())
		if (m_eventQueue.front().duration > 0.f) {
			m_eventQueue.front().function(m_eventQueue.front().accell, m_eventQueue.front().duration, m_eventQueue.front().nrOfAttacks);
			m_eventQueue.front().duration -= deltaTime;

			if(m_eventQueue.front().type == EventType::Dash) {
				m_playerMeshHolder->GetComponent<Aen::Animator>().SetAnimationScale(0.35f);
				m_playerMeshHolder->GetComponent<Aen::Animator>().SetAnimation("Dash");
			} else if(m_eventQueue.front().type == EventType::Attack) {
				m_playerMeshHolder->GetComponent<Aen::Animator>().SetAnimationScale(0.35f);
				m_playerMeshHolder->GetComponent<Aen::Animator>().SetAnimation("Attack");
			}

		}
		else {
			if (axis.Magnitude() > 0.f)
				m_finalDir = Aen::Vec3f(dir.Normalized().x, 0.f, dir.Normalized().y);
			else if(m_eventQueue.size() > 1) {
				Aen::Vec2f dir(camDir.x, camDir.z);
				m_finalDir = Aen::Vec3f(dir.Normalized().x, 0.f, dir.Normalized().y);
			}

			m_eventQueue.pop_front();

			if(!m_eventQueue.empty()) {
				if(m_eventQueue.front().type == EventType::Attack) {
					Aen::Vec2f dir(camDir.x, camDir.z);
					m_finalDir = Aen::Vec3f(dir.Normalized().x, 0.f, dir.Normalized().y);
				}
			}
		}

	if (m_eventQueue.empty()) {
		if (axis.Magnitude() > 0.f) {
			m_finalDir = Aen::Vec3f(dir.Normalized().x, 0.f, dir.Normalized().y);
			mp_charCont->Move(m_finalDir * m_movementSpeed * deltaTime, deltaTime);
		}
	}

	if (!m_eventQueue.empty() && m_eventQueue.front().type == EventType::Dash)
		mp_hitBox->ToggleActive(false);
	else
		mp_hitBox->ToggleActive(true);

	m_v += Aen::Vec3f(-m_v.x * 1.8f, -30.f, -m_v.z * 1.8f) * deltaTime;
	m_v = Aen::Clamp(m_v, -Aen::Vec3f(20.f, 20.f, 20.f), Aen::Vec3f(20.f, 20.f, 20.f));
	mp_charCont->Move(m_v * deltaTime, deltaTime);
}

Aen::Entity*& Player::GetEntity() {
	return m_player;
}

Aen::Entity*& Player::GetHurtBox() {
	return m_hurtbox;
}

Aen::Entity*& Player::GetCamera()
{
	return m_camera;
}

void Player::UpdateAttack(std::deque<Enemy*>& e, const float& deltaTime) {
	// Attacking -------------------------------------------------------------------------------------

	if (!m_eventQueue.empty() && m_eventQueue.front().type == EventType::Attack) {
		m_attackTimer += deltaTime;
		Aen::Vec3f dir = Aen::Vec3f(0.f, 0.f, 0.f);

		for (int i = 0; i < e.size(); i++) {
			if (e[i]->GetEntity()->GetComponent<Aen::AABoundBox>().Intersects(*mp_hurtBox) && !e[i]->IsHurt()) {

				e[i]->Hurt(true);

				e[i]->SubtractHealth(m_eventQueue.front().damage);
				dir = Aen::Vec3f(0.f, 0.3f, 0.f) + (e[i]->GetEntity()->GetPos() - m_player->GetPos()).Normalized();
				e[i]->Move(dir.Normalized() * m_eventQueue.front().damage * e[i]->GetKnockback());
				
				if(e[i]->GetHealth() <= 0.f) {
					for(uint32_t k = 0u; k < m_targets.size(); k++)
						if(m_targets[k].target->GetEntity()->GetID() == e[i]->GetEntity()->GetID()) {
							m_targets.erase(m_targets.begin() + k);
							break;
						}

					switch (e[i]->GetEnemyType())
					{
					case EnemyType::BASE:
						delete e[i];
						break;
					case EnemyType::MINION:
						mp_boss->RemoveMinion(e[i]);
						break;
					case EnemyType::BOSS:
					{
						delete e[i];
						m_bossesAlive--;
						break;
					}

					default:
						break;
					}
						
					e[i] = nullptr;
					e.erase(e.begin() + i);
				
				}
			}
		}

	} else {
		for(auto& i : e) i->Hurt(false);
		m_hurtbox->GetComponent<Aen::OBBox>().ToggleActive(false);
		ResetSword();
	}
}

void Player::SubtractHealth(const float& damage) {
	m_health -= Aen::Abs(damage);
}

void Player::Move(const Aen::Vec3f& dir) {
	m_v = dir;
}

void Player::PotionUpdate()
{
	// ------------------------------		Health potion		---------------------------------- //
	if (Aen::Input::KeyDown(Aen::Key::Q) && m_nrPotion > 0 && m_health < 200.f && !m_healing) {

		m_healing = true;
		m_nrPotion -= 1;
	}

	if (m_healing) {
		m_timer += 1.f;
		if (m_timer <= m_potion) {
			m_health += 1.f;
		}
		else
			m_healing = false;
	}
	if(!m_healing) {
		m_timer = 0;
	}

	if (m_health > 200.f) // cap
		m_health = 200.f;
}

void Player::IncreaseHealthCap()
{
	m_potionCap += 1;
	m_nrPotion = m_potionCap;
}

const float& Player::GetHealth() {
	return m_health;
}

int Player::GetPotionNr()const
{
	return m_nrPotion;
}

void Player::SetHealing(const bool& b)
{
	m_healing = b;
}

bool& Player::IsHealing() const
{
	return m_healing;
}
void Player::Hurt(float dmg, float force, Aen::Vec3f dir)
{
	m_health -= std::abs(dmg);
	Aen::Vec3f launchDirection(dir + Aen::Vec3f(0.f,.3f, 0.f));
	Move(launchDirection * force);
}

const bool Player::IsAttacking() {
	if (!m_eventQueue.empty())
		return (m_eventQueue.front().type == EventType::Attack);
	return false;
}

Aen::AABoundBox* Player::GetHitBoxP() const
{
	return mp_hitBox;
}

void Player::SetBossP(Boss* boss)
{
	mp_boss = boss;
}

int Player::GetBossesAlive() const
{
	return m_bossesAlive;
}

void Player::AddBossesAlive(int n)
{
	m_bossesAlive += n;
	if (m_bossesAlive < 0) m_bossesAlive = 0;
}

void Player::SwordSwing(float speed, float time, const float& deltaTime)
{
	static float timer = 0.f;
	timer += deltaTime;
	if (timer > time)
	{
		m_sword->SetRot(0, 0, 0);
		timer = 0.f;
	}
	m_sword->Rotate(-speed * deltaTime, -speed * deltaTime, 0.f);
}

void Player::ResetSword()
{
	m_sword->SetRot(0.f, 0.f, 0.f);
}

void Player::AddEvent(EventData& event) {
	if (m_eventQueue.size() > 1u)
		m_eventQueue.pop_back();

	m_eventQueue.emplace_back(event);
}
