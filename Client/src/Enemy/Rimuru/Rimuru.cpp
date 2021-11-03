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

Aen::Entity*& Rimuru::GetEntity() {
	return m_rimuru;
}

void Rimuru::Update(const float& deltaTime, Player& player) {
	Aen::Vec3f eDir = player.GetEntity()->GetPos() - m_enemy->GetPos();
	float dist = eDir.Magnitude();

	if(m_enemy->GetComponent<Aen::CharacterController>().IsGrounded())
		m_v.y = 0.f;

	if(dist < 20.f) {
		m_lDir = Aen::Lerp(m_lDir, eDir.Normalized(), 0.03f);
		float yaw = Aen::RadToDeg(std::atan2(m_lDir.x, m_lDir.z));
		m_rimuru->SetRot(0.f, yaw + 180, 0.f);
		
		m_Dir = Aen::Lerp(m_Dir, eDir.Normalized(), 0.1f);
		Aen::Vec2f nDir(m_Dir.x, m_Dir.z);
		nDir = nDir.Normalized();
		m_enemy->GetComponent<Aen::CharacterController>().Move(Aen::Vec3f(nDir.x, 0.f, nDir.y) * 3.f * deltaTime, deltaTime);

		if(m_targeted && player.IsAttacking() && m_toggleAttacked) {
			m_toggleAttacked = false;
		}

		if(m_targeted && player.IsAttacking() && rand() % 4 == 0) {
			m_enemy->GetComponent<Aen::AABoundBox>().ToggleActive(false);
			Aen::Vec3f right = eDir.Normalized() % Aen::Vec3f(0.f, 1.f, 0.f);
			m_v = Aen::Vec3f(0.f, 5.f, 0.f) + right * 10.f;
		} else
			m_enemy->GetComponent<Aen::AABoundBox>().ToggleActive(true);
	}


	m_v += Aen::Vec3f(-m_v.x * 1.5f, -30.f, -m_v.z * 1.5f) * deltaTime;
	m_v = Aen::Clamp(m_v, -Aen::Vec3f(20.f, 20.f, 20.f), Aen::Vec3f(20.f, 20.f, 20.f));


	m_enemy->GetComponent<Aen::CharacterController>().Move(m_v * deltaTime, deltaTime);
}
