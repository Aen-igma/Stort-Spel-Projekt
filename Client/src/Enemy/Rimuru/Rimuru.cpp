#include"Rimuru.h"

Rimuru::Rimuru()
	:Enemy(), m_rimuru(&Aen::EntityHandler::CreateEntity()) {
	m_rimuru->AddComponent<Aen::MeshInstance>();
	m_rimuru->GetComponent<Aen::MeshInstance>().SetMesh("Rimuru");
	m_rimuru->GetComponent<Aen::MeshInstance>().SetMaterial("EnemyMaterial");
	m_rimuru->SetPos(0.f, -0.2f, 0.f);
	m_rimuru->SetScale(0.8f, 0.8f, 0.8f);
	m_rimuru->SetParent(*m_enemy);

	m_enemy->GetComponent<Aen::AABoundBox>().SetBoundingBox(1.2f, 0.8f, 1.2f);
	m_enemy->GetComponent<Aen::AABoundBox>().SetOffset(0.f, -0.2f, 0.f);
	m_enemy->GetComponent<Aen::CharacterController>().SetHeight(0.2f);
	m_enemy->SetPos(0.f, 1.5f, 5.f);
}

Rimuru::~Rimuru() {
	Aen::EntityHandler::RemoveEntity(*m_rimuru);
}

void Rimuru::Update(const float& deltaTime, Aen::Entity& player) {
	Aen::Vec3f eDir = player.GetPos() - m_enemy->GetPos();
	float dist = eDir.Magnitude();

	m_lDir = Aen::Lerp(m_lDir, eDir.Normalized(), 0.03f);
	float yaw = Aen::RadToDeg(std::atan2(m_lDir.x, m_lDir.z));

	if(dist < 8.f) {
		m_enemy->GetComponent<Aen::CharacterController>().Move(eDir.Normalized() * 3.f * deltaTime, deltaTime);
		m_rimuru->SetRot(0.f, yaw + 180, 0.f);
	}

	m_enemy->GetComponent<Aen::CharacterController>().Move(Aen::Vec3f(0.f, -1.f, 0.f) * deltaTime, deltaTime);
}
