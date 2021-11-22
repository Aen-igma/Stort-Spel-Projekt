#include "NecroBoss.h"

NecroBoss::NecroBoss():
	Enemy()
{
	m_enemy->AddComponent<Aen::MeshInstance>();
	m_enemy->GetComponent<Aen::MeshInstance>().SetMesh("SkeleBoss");
	m_enemy->AddComponent<Aen::AABoundBox>();
}

void NecroBoss::Update(const float& deltaTime, Player& player)
{
}
