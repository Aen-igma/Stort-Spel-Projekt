#include"Enemy.h"

Enemy::Enemy()
	:m_enemy(&Aen::EntityHandler::CreateEntity()) {
	m_enemy->SetTag("Enemy");
	m_enemy->AddComponent<Aen::MeshInstance>();
	m_enemy->GetComponent<Aen::MeshInstance>().SetMesh("Capsule");
	m_enemy->GetComponent<Aen::MeshInstance>().SetMaterial("EnemyMaterial");
	m_enemy->AddComponent<Aen::CharacterController>();
	m_enemy->SetPos(0.f, 1.5f, 5.f);
	m_enemy->AddComponent<Aen::AABoundBox>();
	m_enemy->GetComponent<Aen::AABoundBox>().SetBoundsToMesh();
}

Enemy::~Enemy() {
	Aen::EntityHandler::RemoveEntity(*m_enemy);
}

void Enemy::Update(const float& deltaTime, Aen::Entity& player) {

	/*if (this->m_enemy->GetComponent<Aen::AABoundBox>().Intersects(player.GetComponent<Aen::AABoundBox>())) {

	}*/
}
