#include"Rimuru.h"

Rimuru::Rimuru()
	:Enemy(), m_rimuru(&Aen::EntityHandler::CreateEntity()), m_lDir(0.f, 0.f, -1.f) {
	m_rimuru->AddComponent<Aen::MeshInstance>();
	m_rimuru->GetComponent<Aen::MeshInstance>().SetMesh("Rimuru");
	m_rimuru->GetComponent<Aen::MeshInstance>().SetMaterial("EnemyMaterial");
	m_rimuru->SetScale(0.8f, 0.8f, 0.8f);
	m_rimuru->SetParent(*m_enemy);

	m_enemy->GetComponent<Aen::AABoundBox>().SetBoundingBox(1.2f, 0.8f, 1.2f);
	m_enemy->GetComponent<Aen::CharacterController>().Resize(0.2f);
	m_enemy->GetComponent<Aen::CharacterController>().SetRadius(1.f);
	
	m_enemy->SetPos(-11.f, 1.5f, 0.f);

	m_health = 100.f;
	m_wait = false;
	m_dodge = false;
	m_hurting = false;
	m_toggleAttacked = false;
}

Rimuru::Rimuru(const Aen::Vec3f& pos)
	:Enemy(), m_rimuru(&Aen::EntityHandler::CreateEntity()) {
	m_rimuru->AddComponent<Aen::MeshInstance>();
	m_rimuru->GetComponent<Aen::MeshInstance>().SetMesh("Rimuru");
	m_rimuru->GetComponent<Aen::MeshInstance>().SetMaterial("EnemyMaterial");
	m_rimuru->SetScale(0.8f, 0.8f, 0.8f);
	m_rimuru->SetParent(*m_enemy);

	m_enemy->GetComponent<Aen::AABoundBox>().SetBoundingBox(1.2f, 0.8f, 1.2f);
	m_enemy->GetComponent<Aen::CharacterController>().Resize(0.2f);
	m_enemy->GetComponent<Aen::CharacterController>().SetRadius(1.f);

	m_enemy->SetPos(pos);
	m_health = 100.f;
	m_wait = false;
	m_dodge = false;
	m_hurting = false;
	m_toggleAttacked = false;
}

Rimuru::~Rimuru() {
	m_rimuru->RemoveParent();
	Aen::EntityHandler::RemoveEntity(*m_rimuru);
	Aen::EntityHandler::RemoveEntity(*m_enemy);
}

Aen::Entity*& Rimuru::GetEntity() {
	return m_rimuru;
}

void Rimuru::Update(const float& deltaTime, Player& player) {
	Aen::Vec3f eDir = player.GetEntity()->GetPos() - m_enemy->GetPos();
	float dist = eDir.Magnitude();

	/*if(m_enemy->GetComponent<Aen::CharacterController>().IsGrounded())
		m_v.y = 0.f;*/

	if(!m_eventQueue.empty()) {
		if(m_eventQueue.front().duration > 0.f) {
			m_eventQueue.front().function(m_eventQueue.front().accell, m_eventQueue.front().duration);
			m_eventQueue.front().duration -= deltaTime;
		} else {
			m_eventQueue.pop_front();
		}
	} else {
		if(dist < 20.f)
			RandomCombatEvent(deltaTime);
		else {
			m_rDir = Aen::Vec2f(float(rand() % 10) - 5, float(rand() % 10) - 5);
			RandomIdleEvent(deltaTime, m_rDir);
		}
	}

	if(dist < 20.f) {
		m_lDir = Aen::Lerp(m_lDir, eDir.Normalized(), 0.03f);
		float yaw = Aen::RadToDeg(std::atan2(m_lDir.x, m_lDir.z));
		m_rimuru->SetRot(0.f, yaw + 180, 0.f);
		
		m_Dir = Aen::Lerp(m_Dir, eDir.Normalized(), 0.1f);
		Aen::Vec2f nDir(m_Dir.x, m_Dir.z);
		nDir = nDir.Normalized();

		m_enemy->GetComponent<Aen::CharacterController>().Move(Aen::Vec3f(nDir.x, 0.f, nDir.y) * 3.f * deltaTime, deltaTime);

		static float d = 0.f;
		if(m_targeted && player.IsAttacking() && !m_toggleAttacked) {
			m_toggleAttacked = true;
			d = float(rand() % 3 - 1);
			m_dodge = (rand() % 2 == 0);
		} else if(!player.IsAttacking()) {
			m_toggleAttacked = false;
			m_dodge = false;
		}

		if(m_dodge) {
			m_enemy->GetComponent<Aen::AABoundBox>().ToggleActive(false);
			Aen::Vec3f right = eDir.Normalized() % Aen::Vec3f(0.f, 1.f, 0.f);
			m_v = Aen::Vec3f(0.f, 10.f, 0.f) - Aen::Vec3f(nDir.x, 0.f, nDir.y).Normalized() * 18.f + right.Normalized() * d * 20.f;
			m_dodge = false;
		} else
			m_enemy->GetComponent<Aen::AABoundBox>().ToggleActive(true);
	}
	else
	{
		m_enemy->GetComponent<Aen::AABoundBox>().ToggleActive(false);
	}


	if(player.GetEntity()->GetComponent<Aen::AABoundBox>().Intersects(m_enemy->GetComponent<Aen::AABoundBox>())) {
		if(!m_hurting) {
			m_hurting = true;
			player.SubtractHealth(10.f);
			Aen::Vec3f dir = Aen::Vec3f(0.f, 0.3f, 0.f) + m_lDir.Normalized();
			player.Move(dir.Normalized() * 20.f);
		}
	} else
		m_hurting = false;

	if (IsHurt() && !m_wait)
	{
		m_wait = true;
		m_rimuru->GetComponent<Aen::MeshInstance>().SetMaterial("EnemyMaterialHurt");
		WaitEvent();
	}
	
	if(!IsHurt() && m_wait)
	{
		m_wait = false;
		m_rimuru->GetComponent<Aen::MeshInstance>().SetMaterial("EnemyMaterial");
	}

	m_v += Aen::Vec3f(-m_v.x * 1.8f, -30.f, -m_v.z * 1.8f) * deltaTime;
	m_v = Aen::Clamp(m_v, -Aen::Vec3f(20.f, 20.f, 20.f), Aen::Vec3f(20.f, 20.f, 20.f));
	m_enemy->GetComponent<Aen::CharacterController>().Move(m_v * deltaTime, deltaTime);
}

void Rimuru::RandomCombatEvent(const float& deltaTime) {
	EventData data;
	switch(rand() % 2) {
		case 0:
		data.duration = rand() % 2 + 1;
		data.function = [&](float& accell, const float& attackDuration) {};
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

void Rimuru::RandomIdleEvent(const float& deltaTime, const Aen::Vec2f& randDir) {
	EventData data;

	switch(rand() % 2) {
		case 0:
		data.duration = rand() % 3 + 3;
		data.function = [&](float& accell, const float& attackDuration) {};
		break;
		case 1:
		data.duration = rand() % 3 + 1;
		data.function = [&](float& accell, const float& attackDuration) {
			m_enemy->GetComponent<Aen::CharacterController>().Move(Aen::Vec3f(randDir.x, 0.f, randDir.y).Normalized() * 3.f * deltaTime, deltaTime);

			m_lDir = Aen::Lerp(m_lDir, Aen::Vec3f(randDir.x, 0.f, randDir.y).Normalized(), 0.03f);
			float yaw = Aen::RadToDeg(std::atan2(m_lDir.x, m_lDir.z));
			m_rimuru->SetRot(0.f, yaw + 180, 0.f);
		};
		break;
	}

	m_eventQueue.emplace_back(data);
}

void Rimuru::WaitEvent()
{
	EventData data;

	data.type = EventType::Wait;
	data.duration = 2.f;
	data.function = [&](float& accell, const float& attackDuration) {};

	m_eventQueue.clear();
	m_eventQueue.emplace_back(data);
}