#include"Enemy.h"

Enemy::Enemy()
	:m_enemy(&Aen::EntityHandler::CreateEntity()) {
	m_enemy->SetTag("Enemy");
	m_enemy->AddComponent<Aen::AABoundBox>();
	m_enemy->AddComponent<Aen::CharacterController>();
}

Enemy::~Enemy() {
	Aen::EntityHandler::RemoveEntity(*m_enemy);
}

void Enemy::Update(const float& deltaTime, Aen::Entity& player) {
}
