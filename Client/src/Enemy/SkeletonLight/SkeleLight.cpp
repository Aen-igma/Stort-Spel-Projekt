#include"SkeleLight.h"
#include "AenirEngine/LevelGeneration/LevelGenerator.h"

#define rand LehmerInt

SkeleLight::SkeleLight()
	:Enemy(), mp_skeleton(&Aen::EntityHandler::CreateEntity()), m_lDir(0.f, 0.f, -1.f)
{
	mp_skeleton->AddComponent<Aen::MeshInstance>();
	mp_skeleton->GetComponent<Aen::MeshInstance>().SetMesh("SkeletonLight");
	mp_skeleton->GetComponent<Aen::MeshInstance>().SetMaterial("SkeleLightMaterial");
	mp_skeleton->SetParent(*m_enemy);
	mp_skeleton->SetPos(0.f, 0.f, 0.f);

	m_enemy->GetComponent<Aen::AABoundBox>().SetBoundingBox(1.f, 2.f, 1.f);
	m_enemy->GetComponent<Aen::CharacterController>().Resize(0.8f);
	m_enemy->GetComponent<Aen::CharacterController>().SetRadius(1.f);
	m_enemy->SetRot(0, 90, 0);
	m_enemy->SetPos(0.f, 0.f, 0.f);
	m_knockbackScalar = 0.5f;

	// -----------------------------	Floating m_healthBar		------------------------------- //
	mp_healthBar = &Aen::EntityHandler::CreateEntity();
	mp_healthBar->AddComponent<Aen::MeshInstance>();
	mp_healthBar->GetComponent<Aen::MeshInstance>().SetMesh("eBar");
	mp_healthBar->GetComponent<Aen::MeshInstance>().SetMaterial("barMat");
	mp_healthBar->SetRot(180, 0, 0);
	mp_healthBar->SetPos(0, -100, 0);
	mp_healthBar->SetScale(5.f, 0.f, 5.f);
	mp_healthBar->SetRenderLayer(1);

	mp_hurtbox = &Aen::EntityHandler::CreateEntity();
	mp_hurtbox->AddComponent<Aen::OBBox>();
	mp_hurtbox->GetComponent<Aen::OBBox>().SetBoundingBox(1.f, 1.f, 1.f);
	mp_hurtbox->GetComponent<Aen::OBBox>().ToggleActive(false);
	mp_hurtbox->SetParent(*m_enemy);

	m_nDir = Aen::Vec2f(0.f, 0.f);
	m_health = 100.f;
	m_hurting, m_wait = false;
}

SkeleLight::SkeleLight(const Aen::Vec3f& pos)
	:Enemy(), mp_skeleton(&Aen::EntityHandler::CreateEntity())
{
	mp_skeleton->AddComponent<Aen::MeshInstance>();
	mp_skeleton->GetComponent<Aen::MeshInstance>().SetMesh("SkeletonLight");
	mp_skeleton->GetComponent<Aen::MeshInstance>().SetMaterial("SkeleLightMaterial");
	mp_skeleton->SetParent(*m_enemy);
	mp_skeleton->SetPos(0.f, 0.f, 0.f);

	m_enemy->GetComponent<Aen::AABoundBox>().SetBoundingBox(1.f, 2.f, 1.f);
	m_enemy->GetComponent<Aen::CharacterController>().Resize(0.8f);
	m_enemy->GetComponent<Aen::CharacterController>().SetRadius(1.f);
	m_enemy->SetRot(0, 90, 0);
	m_enemy->SetPos(pos);
	m_knockbackScalar = 0.5f;

	// -----------------------------	Floating m_healthBar		------------------------------- //
	mp_healthBar = &Aen::EntityHandler::CreateEntity();
	mp_healthBar->AddComponent<Aen::MeshInstance>();
	mp_healthBar->GetComponent<Aen::MeshInstance>().SetMesh("eBar");
	mp_healthBar->GetComponent<Aen::MeshInstance>().SetMaterial("barMat");
	mp_healthBar->SetRot(180, 0, 0);
	mp_healthBar->SetPos(0, -100, 0);
	mp_healthBar->SetScale(5.f, 0.f, 5.f);
	mp_healthBar->SetRenderLayer(1);

	mp_hurtbox = &Aen::EntityHandler::CreateEntity();
	mp_hurtbox->AddComponent<Aen::OBBox>();
	mp_hurtbox->GetComponent<Aen::OBBox>().SetBoundingBox(1.f, 1.f, 1.f);
	mp_hurtbox->GetComponent<Aen::OBBox>().ToggleActive(false);
	mp_hurtbox->SetParent(*m_enemy);

	m_nDir = Aen::Vec2f(0.f, 0.f);
	m_health = 200.f;
	m_hurting, m_wait = false;
}

SkeleLight::~SkeleLight()
{
	mp_skeleton->RemoveParent();
	mp_hurtbox->RemoveParent();
	Aen::EntityHandler::RemoveEntity(*mp_skeleton);
	Aen::EntityHandler::RemoveEntity(*m_enemy);
	Aen::EntityHandler::RemoveEntity(*mp_healthBar);
	Aen::EntityHandler::RemoveEntity(*mp_hurtbox);
}

Aen::Entity*& SkeleLight::GetEntity()
{
	return mp_skeleton;
}

void SkeleLight::Update(const float& deltaTime, Player& player)
{
	Aen::Vec3f eDir = player.GetEntity()->GetPos() - m_enemy->GetPos();
	float dist = eDir.Magnitude();

	mp_healthBar->SetScale(m_health / 20.f, 0.f, 5.f);

	if (!m_eventQueue.empty()) {
		if (m_eventQueue.front().duration > 0.f) {
			m_eventQueue.front().function(m_eventQueue.front().accell, m_eventQueue.front().duration, m_eventQueue.front().nrOfAttacks);
			m_eventQueue.front().duration -= deltaTime;
		}
		else {
			if(m_eventQueue.front().type == EventType::Attack)
				mp_hurtbox->GetComponent<Aen::OBBox>().ToggleActive(false);
			m_eventQueue.pop_front();
		}
	}
	else 
	{
		if (dist < 20.f)
		{
			CombatEvent(deltaTime, dist);
		}
		else 
		{
			mp_healthBar->SetRot(180, 0, 0);
			mp_healthBar->SetPos(0, -100, 0);
			m_rDir = Aen::Vec2f(float(rand() % 10) - 5, float(rand() % 10) - 5);
			RandomIdleEvent(deltaTime, m_rDir);
		}
	}

	if (dist < 20.f) {
		mp_healthBar->SetRot(-player.GetCamera()->GetRot().x - 90.f, player.GetCamera()->GetRot().y + 180.f, 0);
		mp_healthBar->SetPos(m_enemy->GetPos() + Aen::Vec3f(0, 7.f, 0));


		m_lDir = Aen::Lerp(m_lDir, eDir.Normalized(), 0.03f);
		float yaw = std::atan2(m_lDir.x, m_lDir.z);
		mp_hurtbox->GetComponent<Aen::OBBox>().SetOrientation(0.f, yaw, 0.f);
		yaw = Aen::RadToDeg(yaw);
		mp_skeleton->SetRot(0.f, yaw + 180, 0.f);
		mp_hurtbox->SetPos(m_lDir*2);

		m_Dir = Aen::Lerp(m_Dir, eDir.Normalized(), 0.1f);
		m_nDir = Aen::Vec2f(m_Dir.x, m_Dir.z);
		m_nDir = m_nDir.Normalized();

		mp_charCont->Move(Aen::Vec3f(m_nDir.x, 0.f, m_nDir.y) * 3.f * deltaTime, deltaTime);
		//m_enemy->GetComponent<Aen::CharacterController>().Move(Aen::Vec3f(m_nDir.x, 0.f, m_nDir.y) * 3.f * deltaTime, deltaTime);
		m_enemy->GetComponent<Aen::AABoundBox>().ToggleActive(true);
	}
	else
	{
		m_enemy->GetComponent<Aen::AABoundBox>().ToggleActive(false);
	}
	
	if (player.GetHitBoxP()->Intersects(mp_hurtbox->GetComponent<Aen::OBBox>())) {
		if (!m_hurting) {
			m_hurting = true;
			player.SubtractHealth(10.f);
			Aen::Vec3f dir = Aen::Vec3f(0.f, 0.3f, 0.f) + m_lDir.Normalized();
			player.Move(dir.Normalized() * 20.f);
		}
	}
	else
		m_hurting = false;

	if (IsHurt() && !m_wait)
	{
		m_wait = true;
		mp_skeleton->GetComponent<Aen::MeshInstance>().SetMaterial("EnemyMaterialHurt");
		WaitEvent();
	}

	if (!IsHurt() && m_wait)
	{
		m_wait = false;
		mp_skeleton->GetComponent<Aen::MeshInstance>().SetMaterial("SkeleLightMaterial");
	}

	m_v += Aen::Vec3f(-m_v.x * 1.8f, -30.f, -m_v.z * 1.8f) * deltaTime;
	m_v = Aen::Clamp(m_v, -Aen::Vec3f(20.f, 20.f, 20.f), Aen::Vec3f(20.f, 20.f, 20.f));
	mp_charCont->Move(m_v * deltaTime, deltaTime);
}

void SkeleLight::CombatEvent(const float& deltaTime, const float& distance)
{
	if (distance < 5.f)
	{
		// Attack
		EventData data;
		data.type = EventType::Attack;
		data.duration = 1;
		data.function = [&](float& accell, const float& attackDuration, const int& nrOfAttacks)
		{
			mp_hurtbox->GetComponent<Aen::OBBox>().ToggleActive(true);
		};
		m_eventQueue.emplace_back(data);

		// Wait after attacking
		data.type = EventType::Wait;
		data.duration = 2;
		data.function = [&](float& accell, const float& attackDuration, const int& nrOfAttacks)
		{
			mp_hurtbox->GetComponent<Aen::OBBox>().ToggleActive(false);
		};
		m_eventQueue.emplace_back(data);
	}

}

void SkeleLight::RandomIdleEvent(const float& deltaTime, const Aen::Vec2f& randDir)
{
	EventData data;

	switch (rand() % 2) {
	case 0:
		data.duration = rand() % 3 + 3;
		data.function = [&](float& accell, const float& attackDuration, const int& nrOfAttacks) {};
		break;
	case 1:
		data.duration = rand() % 3 + 1;
		data.function = [&](float& accell, const float& attackDuration, const int& nrOfAttacks) {
			mp_charCont->Move(Aen::Vec3f(randDir.x, 0.f, randDir.y).Normalized() * 3.f * deltaTime, deltaTime);

			m_lDir = Aen::Lerp(m_lDir, Aen::Vec3f(randDir.x, 0.f, randDir.y).Normalized(), 0.03f);
			float yaw = Aen::RadToDeg(std::atan2(m_lDir.x, m_lDir.z));
			mp_skeleton->SetRot(0.f, yaw + 180, 0.f);
		};
		break;
	}
	m_eventQueue.emplace_back(data);
}

void SkeleLight::WaitEvent()
{

	EventData data;

	data.type = EventType::Wait;
	data.duration = 2.f;
	data.function = [&](float& accell, const float& attackDuration, const int& nrOfAttacks) {};

	m_eventQueue.clear();
	m_eventQueue.emplace_back(data);
}
