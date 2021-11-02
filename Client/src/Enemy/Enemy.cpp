#include"Enemy.h"

Enemy::Enemy()
	:m_enemy(&Aen::EntityHandler::CreateEntity()) {
	m_enemy->SetTag("Enemy");
	m_enemy->AddComponent<Aen::AABoundBox>();
	m_enemy->AddComponent<Aen::CharacterController>();
}

Enemy::~Enemy() {
}

Aen::Entity*& Enemy::GetEntity()
{
	return m_enemy;
	// TODO: insert return statement here
}

void Enemy::Update(const float& deltaTime, Player& player) {
	if (m_enemy->GetComponent<Aen::AABoundBox>().Intersects(player.GetHurtBox()->GetComponent<Aen::OBBox>()))
	{
		printf("am ded\n");
	}
}
