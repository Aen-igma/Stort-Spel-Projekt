#include"Enemy.h"

Enemy::Enemy(EnemyType isMinion)
	:m_enemy(&Aen::EntityHandler::CreateEntity()), m_targeted(false), m_hurt(false), m_ENEMYTYPE(isMinion)
{
	m_enemy->SetTag("Enemy");
	m_enemy->AddComponent<Aen::AABoundBox>();
	m_enemy->AddComponent<Aen::CharacterController>();
	mp_hitbox = &m_enemy->GetComponent<Aen::AABoundBox>();
	mp_charCont = &m_enemy->GetComponent<Aen::CharacterController>();
	m_knockbackScalar = 1.f;
}

Enemy::~Enemy() {
	Aen::EntityHandler::RemoveEntity(*m_enemy);
}

Aen::Entity*& Enemy::GetEntity()
{
	return m_enemy;
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

const float& Enemy::GetKnockback()
{
	return m_knockbackScalar;
}

void Enemy::Hurt(const bool& hurt) {
	m_hurt = hurt;
}

const bool Enemy::IsHurt() const {
	return m_hurt;
}

const bool Enemy::IsAttacking() const
{
	if (!m_eventQueue.empty())
		return (m_eventQueue.front().type == EventType::Attack);
	return false;
}

void Enemy::SetStationary()
{
	m_stationary = !m_stationary;
}

void Enemy::SetStationary(bool b)
{
	m_stationary = b;
}

bool Enemy::GetStationary() const
{
	return m_stationary;
}

const EnemyType Enemy::GetEnemyType() const
{
	return m_ENEMYTYPE;
}
