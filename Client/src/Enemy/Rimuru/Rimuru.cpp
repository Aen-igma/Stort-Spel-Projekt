#include"Rimuru.h"

Rimuru::Rimuru()
	:Enemy() {
}

Rimuru::~Rimuru() {
}

void Rimuru::Update(const float& deltaTime, Aen::Entity& player) {
	Aen::Vec3f eDir = player.GetPos() - m_enemy->GetPos();
	float dist = eDir.Magnitude();
	if(dist  < 8.f)
	m_enemy->GetComponent<Aen::CharacterController>().Move(eDir.Normalized() * 3.f * deltaTime, deltaTime);
}
