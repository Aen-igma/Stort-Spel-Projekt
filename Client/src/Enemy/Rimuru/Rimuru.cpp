#include"Rimuru.h"

Rimuru::Rimuru()
	:Enemy(), m_rimuru(&Aen::EntityHandler::CreateEntity()), m_lDir(0.f, 0.f, -1.f) {
	m_rimuru->AddComponent<Aen::MeshInstance>();
	m_rimuru->GetComponent<Aen::MeshInstance>().SetMesh("Rimuru");
	m_rimuru->GetComponent<Aen::MeshInstance>().SetMaterial("EnemyMaterial");
	m_rimuru->SetScale(0.8f, 0.8f, 0.8f);
	m_rimuru->SetParent(*m_enemy);

	m_enemy->GetComponent<Aen::AABoundBox>().SetBoundingBox(1.2f, 0.8f, 1.2f);
	m_enemy->GetComponent<Aen::CharacterController>().SetHeight(0.2f);
	m_enemy->SetPos(-11.f, 1.5f, 0.f);
}

Rimuru::~Rimuru() {
	m_rimuru->RemoveParent();
	Aen::EntityHandler::RemoveEntity(*m_rimuru);
	Aen::EntityHandler::RemoveEntity(*m_enemy);
}

void Rimuru::Update(const float& deltaTime, Aen::Entity& player) {
	Aen::Vec3f eDir = player.GetPos() - m_enemy->GetPos();
	float dist = eDir.Magnitude();

	if(dist < 12.f) {
		m_lDir = Aen::Lerp(m_lDir, eDir.Normalized(), 0.03f);
		float yaw = Aen::RadToDeg(std::atan2(m_lDir.x, m_lDir.z));
		m_rimuru->SetRot(0.f, yaw + 180, 0.f);
		
		m_Dir = Aen::Lerp(m_Dir, eDir.Normalized(), 0.1f);
		Aen::Vec2f nDir(m_Dir.x, m_Dir.z);
		nDir = nDir.Normalized();
		m_enemy->GetComponent<Aen::CharacterController>().Move(Aen::Vec3f(nDir.x, 0.f, nDir.y) * 3.f * deltaTime, deltaTime);
	}

	m_v += Aen::Vec3f(-m_v.x * 1.5f, -30.f, -m_v.z * 1.5f) * deltaTime;
	m_v = Aen::Clamp(m_v, -Aen::Vec3f(20.f, 20.f, 20.f), Aen::Vec3f(20.f, 20.f, 20.f));

	if(m_enemy->GetComponent<Aen::CharacterController>().IsGrounded())
		m_v.y = 0.f;

	m_enemy->GetComponent<Aen::CharacterController>().Move(m_v * deltaTime, deltaTime);
}
