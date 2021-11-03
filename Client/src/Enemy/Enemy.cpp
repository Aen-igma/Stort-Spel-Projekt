#include"Enemy.h"

Enemy::Enemy()
	:m_enemy(&Aen::EntityHandler::CreateEntity()), m_targeted(false) {
	m_enemy->SetTag("Enemy");
	m_enemy->AddComponent<Aen::AABoundBox>();
	m_enemy->AddComponent<Aen::CharacterController>();
}

Enemy::~Enemy() {
}

Aen::Entity*& Enemy::GetEntity()
{
	return m_enemy;
}

void Enemy::Update(const float& deltaTime, Player& player) {
}

void Enemy::SetISTargeted(const bool& targeted) {
	m_targeted = targeted;
}
