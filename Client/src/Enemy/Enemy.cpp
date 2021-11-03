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

void Enemy::SubtractHealth(const float& damage) {
	m_health -= Aen::Abs(damage);
}

void Enemy::Move(const Aen::Vec3f& dir) {
	m_v = dir;
}

const float& Enemy::GetHealth() {
	return m_health;
}

void Enemy::Hurt(const bool& hurt) {
	m_hurt = hurt;
}

const bool Enemy::IsHurt() {
	return m_hurt;
}
