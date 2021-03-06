#include"Rimuru.h"
#include "AenirEngine/LevelGeneration/LevelGenerator.h"

#define rand LehmerInt

Rimuru::Rimuru(const Aen::Vec3f& pos, EnemyType isMinion)
	:Enemy(isMinion), m_rimuru(&Aen::EntityHandler::CreateEntity()) {
	m_rimuru->AddComponent<Aen::MeshInstance>();
	m_rimuru->GetComponent<Aen::MeshInstance>().SetMesh("Rimuru");
	m_rimuru->GetComponent<Aen::MeshInstance>().SetMaterial("SlimeMaterial");
	m_rimuru->SetScale(0.8f, 0.8f, 0.8f);
	m_rimuru->SetPos(0.f, -0.4f, 0.f);
	m_rimuru->SetParent(*m_enemy);

	m_enemy->GetComponent<Aen::AABoundBox>().SetBoundingBox(1.2f, 0.8f, 1.2f);
	m_enemy->GetComponent<Aen::CharacterController>().Resize(0.2f);
	m_enemy->GetComponent<Aen::CharacterController>().SetRadius(1.f);
	m_enemy->SetPos(pos);

	m_healthBar = &Aen::EntityHandler::CreateEntity();
	m_healthBar->AddComponent<Aen::MeshInstance>();
	m_healthBar->GetComponent<Aen::MeshInstance>().SetMesh("eBar");
	m_healthBar->GetComponent<Aen::MeshInstance>().SetMaterial("barMat");
	m_healthBar->SetRot(180, 0, 0);
	m_healthBar->SetPos(0, -100, 0);
	m_healthBar->SetScale(5.f, 0.f, 5.f);
	m_healthBar->SetRenderLayer(1);

	m_health = 50.f;
	m_wait, m_dodge, m_hurting, m_toggleAttacked = false;
}

Rimuru::~Rimuru() {
	m_rimuru->RemoveParent();
	Aen::EntityHandler::RemoveEntity(*m_rimuru);
	Aen::EntityHandler::RemoveEntity(*m_enemy);
	Aen::EntityHandler::RemoveEntity(*m_healthBar);
}

Aen::Entity*& Rimuru::GetEntity() {
	return m_rimuru;
}

void Rimuru::Update(const float& deltaTime, Player& player) {
	Aen::Vec3f eDir = player.GetEntity()->GetPos() - m_enemy->GetPos();
	float dist = eDir.Magnitude();

	m_healthBar->SetScale(m_health / 20.f, 0.f, 5.f);

	/*if(m_enemy->GetComponent<Aen::CharacterController>().IsGrounded())
		m_v.y = 0.f;*/

	if(!m_eventQueue.empty()) {
		if(m_eventQueue.front().duration > 0.f) {
			m_eventQueue.front().function(m_eventQueue.front().accell, m_eventQueue.front().duration, m_eventQueue.front().nrOfAttacks);
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
		m_healthBar->SetRot(-player.GetCamera()->GetRot().x - 90.f, player.GetCamera()->GetRot().y + 180.f, 0);
		m_healthBar->SetPos(m_enemy->GetPos() + Aen::Vec3f(0, 2.f, 0));

		m_lDir = Aen::Lerp(m_lDir, eDir.Normalized(), 0.03f);
		float yaw = Aen::RadToDeg(std::atan2(m_lDir.x, m_lDir.z));
		m_rimuru->SetRot(0.f, yaw + 180, 0.f);
		
		m_Dir = Aen::Lerp(m_Dir, eDir.Normalized(), 0.1f);
		Aen::Vec2f nDir(m_Dir.x, m_Dir.z);
		nDir = nDir.Normalized();

		mp_charCont->Move(Aen::Vec3f(nDir.x, 0.f, nDir.y) * 3.f * deltaTime, deltaTime);

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
			mp_hitbox->ToggleActive(false);
			Aen::Vec3f right = eDir.Normalized() % Aen::Vec3f(0.f, 1.f, 0.f);
			m_v = Aen::Vec3f(0.f, 10.f, 0.f) - Aen::Vec3f(nDir.x, 0.f, nDir.y).Normalized() * 18.f + right.Normalized() * d * 20.f;
			m_dodge = false;
		} else
			mp_hitbox->ToggleActive(true);
	}
	else
	{
		mp_hitbox->ToggleActive(false);
		m_healthBar->SetRot(180, 0, 0);
		m_healthBar->SetPos(0, -100, 0);
	}

	if(player.GetHitBoxP()->Intersects(m_enemy->GetComponent<Aen::AABoundBox>())) {
		if(!m_hurting) {
			m_hurting = true;

			if (player.IsHealing()) {
				player.SetHealing(false);
				player.SubtractHealth(m_damage);
			}
			else {
				player.SubtractHealth(m_damage);
			}
			Aen::Vec3f dir = Aen::Vec3f(0.f, 0.3f, 0.f) + m_lDir.Normalized();
			player.Move(dir.Normalized() * 20.f);
		}
	}
	else {
		m_hurting = false;
	}

	if (IsHurt() && !m_wait)
	{
		m_wait = true;
		m_rimuru->GetComponent<Aen::MeshInstance>().SetMaterial("EnemyMaterialHurt");
		WaitEvent();
	}
	
	if(!IsHurt() && m_wait)
	{
		m_wait = false;
		m_rimuru->GetComponent<Aen::MeshInstance>().SetMaterial("SlimeMaterial");
	}

	m_v += Aen::Vec3f(-m_v.x * 1.8f, -30.f, -m_v.z * 1.8f) * deltaTime;
	m_v = Aen::Clamp(m_v, -Aen::Vec3f(20.f, 20.f, 20.f), Aen::Vec3f(20.f, 20.f, 20.f));
	mp_charCont->Move(m_v * deltaTime, deltaTime);
}

void Rimuru::RandomCombatEvent(const float& deltaTime) {
	EventData data;
	switch(rand() % 2) {
		case 0:
		data.duration = rand() % 2 + 1;
		data.function = [&](float& accell, const float& attackDuration, const int& nrOfAttacks) {};
		break;
		case 1:
		data.duration = 0.01f;
		data.function = [&](float& accell, const float& attackDuration, const int& nrOfAttacks) {
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
		data.function = [&](float& accell, const float& attackDuration, const int& nrOfAttacks) {};
		break;
		case 1:
		data.duration = rand() % 3 + 1;
		data.function = [&](float& accell, const float& attackDuration, const int& nrOfAttacks) {
			mp_charCont->Move(Aen::Vec3f(randDir.x, 0.f, randDir.y).Normalized() * 3.f * deltaTime, deltaTime);

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
	data.function = [&](float& accell, const float& attackDuration, const int& nrOfAttacks) {};

	m_eventQueue.clear();
	m_eventQueue.emplace_back(data);
}