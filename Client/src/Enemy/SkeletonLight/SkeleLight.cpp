#include"SkeleLight.h"
#include "AenirEngine/LevelGeneration/LevelGenerator.h"

#define rand LehmerInt

SkeleLight::SkeleLight()
	:Enemy(), m_skeleton(&Aen::EntityHandler::CreateEntity()), m_lDir(0.f, 0.f, -1.f)
{
	m_skeleton->AddComponent<Aen::MeshInstance>();
	m_skeleton->GetComponent<Aen::MeshInstance>().SetMesh("SkeletonLight");
	m_skeleton->GetComponent<Aen::MeshInstance>().SetMaterial("SkeleLightMaterial");
	m_skeleton->SetParent(*m_enemy);
	
	m_enemy->GetComponent<Aen::AABoundBox>().SetBoundingBox(1.f, 1.4f, 1.f);
	m_enemy->GetComponent<Aen::CharacterController>().Resize(0.7f);
	m_enemy->GetComponent<Aen::CharacterController>().SetRadius(1.f);
	m_enemy->SetPos(0.f, 0.f, 0.f);

	// -----------------------------	Floating m_healthBar		------------------------------- //
	m_healthBar = &Aen::EntityHandler::CreateEntity();
	m_healthBar->AddComponent<Aen::MeshInstance>();
	m_healthBar->GetComponent<Aen::MeshInstance>().SetMesh("eBar");
	m_healthBar->GetComponent<Aen::MeshInstance>().SetMaterial("barMat");
	m_healthBar->SetRot(180, 0, 0);
	m_healthBar->SetPos(0, -100, 0);
	m_healthBar->SetScale(5.f, 0.f, 5.f);
	m_healthBar->SetRenderLayer(1);

	m_health = 100.f;
	m_toggleAttacked, m_hurting, m_wait = false;
}

SkeleLight::SkeleLight(const Aen::Vec3f& pos)
	:Enemy(), m_skeleton(&Aen::EntityHandler::CreateEntity())
{
	m_skeleton->AddComponent<Aen::MeshInstance>();
	m_skeleton->GetComponent<Aen::MeshInstance>().SetMesh("SkeletonLight");
	m_skeleton->GetComponent<Aen::MeshInstance>().SetMaterial("SkeleLightMaterial");
	m_skeleton->SetParent(*m_enemy);

	m_enemy->GetComponent<Aen::AABoundBox>().SetBoundingBox(1.f, 1.4f, 1.f);
	m_enemy->GetComponent<Aen::CharacterController>().Resize(0.7f);
	m_enemy->GetComponent<Aen::CharacterController>().SetRadius(1.f);
	m_enemy->SetPos(pos);

	// -----------------------------	Floating m_healthBar		------------------------------- //
	m_healthBar = &Aen::EntityHandler::CreateEntity();
	m_healthBar->AddComponent<Aen::MeshInstance>();
	m_healthBar->GetComponent<Aen::MeshInstance>().SetMesh("eBar");
	m_healthBar->GetComponent<Aen::MeshInstance>().SetMaterial("barMat");
	m_healthBar->SetRot(180, 0, 0);
	m_healthBar->SetPos(0, -100, 0);
	m_healthBar->SetScale(5.f, 0.f, 5.f);
	m_healthBar->SetRenderLayer(1);

	m_health = 100.f;
	m_toggleAttacked, m_hurting, m_wait = false;
}

SkeleLight::~SkeleLight()
{
	m_skeleton->RemoveParent();
	Aen::EntityHandler::RemoveEntity(*m_skeleton);
	Aen::EntityHandler::RemoveEntity(*m_enemy);
	Aen::EntityHandler::RemoveEntity(*m_healthBar);
}

Aen::Entity*& SkeleLight::GetEntity()
{
	return m_skeleton;
}

void SkeleLight::Update(const float& deltaTime, Player& player)
{
	Aen::Vec3f eDir = player.GetEntity()->GetPos() - m_enemy->GetPos();
	float dist = eDir.Magnitude();

	m_healthBar->SetScale(m_health / 20.f, 0.f, 5.f);

	if (!m_eventQueue.empty()) {
		if (m_eventQueue.front().duration > 0.f) {
			m_eventQueue.front().function(m_eventQueue.front().accell, m_eventQueue.front().duration);
			m_eventQueue.front().duration -= deltaTime;
		}
		else {
			m_eventQueue.pop_front();
		}
	}
	else {
		if (dist < 20.f)
		{
			RandomCombatEvent(deltaTime);
		}
		else 
		{
			RandomIdleEvent(deltaTime, Aen::Vec2f(float(rand() % 10) - 5, float(rand() % 10) - 5));
		}
	}

	if (dist < 20.f) {
		m_healthBar->SetRot(-player.GetCamera()->GetRot().x - 90.f, player.GetCamera()->GetRot().y + 180.f, 0);
		m_healthBar->SetPos(m_enemy->GetPos() + Aen::Vec3f(0, 4.f, 0));
		m_enemy->GetComponent<Aen::AABoundBox>().ToggleActive(true);
	}
	else
	{
		m_enemy->GetComponent<Aen::AABoundBox>().ToggleActive(false);
		m_healthBar->SetRot(180, 0, 0);
		m_healthBar->SetPos(0, -100, 0);
	}


	if (player.GetEntity()->GetComponent<Aen::AABoundBox>().Intersects(m_enemy->GetComponent<Aen::AABoundBox>())) {
		if (!m_hurting) {
			m_hurting = true;
			player.SubtractHealth(10.f);
			Aen::Vec3f dir = Aen::Vec3f(0.f, 0.3f, 0.f);
			player.Move(dir.Normalized() * 20.f);
		}
	}
	else
		m_hurting = false;

	if (IsHurt() && !m_wait)
	{
		m_wait = true;
		m_skeleton->GetComponent<Aen::MeshInstance>().SetMaterial("EnemyMaterialHurt");
		WaitEvent(rand() % 2 + 1);
	}

	if (!IsHurt() && m_wait)
	{
		m_wait = false;
		m_skeleton->GetComponent<Aen::MeshInstance>().SetMaterial("SkeleLightMaterial");
	}

	m_v += Aen::Vec3f(-m_v.x * 1.8f, -30.f, -m_v.z * 1.8f) * deltaTime;
	m_v = Aen::Clamp(m_v, -Aen::Vec3f(20.f, 20.f, 20.f), Aen::Vec3f(20.f, 20.f, 20.f));
	m_enemy->GetComponent<Aen::CharacterController>().Move(m_v * deltaTime, deltaTime);
}

void SkeleLight::RandomCombatEvent(const float& deltaTime)
{
	EventData data;
	switch (rand() % 2) {
	case 0:
		WaitEvent(rand() % 2 + 1);
		break;
	case 1:
		data.duration = 0.01f;
		data.function = [&](float& accell, const float& attackDuration) {
			m_v = Aen::Vec3f(0.f, 8.f, 0.f) + m_lDir * 14.f;
		};
		break;
	}

	m_eventQueue.emplace_back(data);
}

void SkeleLight::RandomIdleEvent(const float& deltaTime, const Aen::Vec2f& randDir)
{
	EventData data;

	switch (rand() % 2) {
	case 0:
		WaitEvent(rand() % 3 + 3);
		break;
	case 1:
		data.duration = rand() % 3 + 1;
		data.function = [&](float& accell, const float& attackDuration) {
			m_enemy->GetComponent<Aen::CharacterController>().Move(Aen::Vec3f(randDir.x, 0.f, randDir.y).Normalized() * 3.f * deltaTime, deltaTime);

			m_lDir = Aen::Lerp(m_lDir, Aen::Vec3f(randDir.x, 0.f, randDir.y).Normalized(), 0.03f);
			float yaw = Aen::RadToDeg(std::atan2(m_lDir.x, m_lDir.z));
			m_skeleton->SetRot(0.f, yaw + 180, 0.f);
		};
		break;
	}

	m_eventQueue.emplace_back(data);
}

void SkeleLight::WaitEvent(uint64_t randomTime)
{
	EventData data;

	data.type = EventType::Wait;
	data.duration = randomTime;
	data.function = [&](float& accell, const float& attackDuration) {};

	m_eventQueue.clear();
	m_eventQueue.emplace_back(data);
}
