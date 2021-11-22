#include "NecroBoss.h"

NecroBoss::NecroBoss():
	Enemy(), m_direction(0.f,0.f,1.f)
{
	m_health = 100.f;

	Aen::Mesh& skeleBoss = Aen::Resource::CreateMesh("SkeleBoss");
	skeleBoss.Load(AEN_RESOURCE_DIR("Models/Skeleton/skBoss_test.fbx"));
	Aen::Material& skeleBossMat = Aen::Resource::CreateMaterial("SkeleBossMat");
	skeleBossMat["GlowColor"] = Aen::Color::Cyan;


	skeleBossMat.LoadeAndSetDiffuseMap(AEN_RESOURCE_DIR("Textures/Skeleton/SkeletonBoss_diffuse.png"));
	skeleBossMat.LoadeAndSetEmissionMap(AEN_RESOURCE_DIR("Textures/Skeleton/SkeletonBoss_glow.png"));
	

	m_enemy->AddComponent<Aen::MeshInstance>();
	m_enemy->GetComponent<Aen::MeshInstance>().SetMesh(skeleBoss);
	m_enemy->GetComponent<Aen::MeshInstance>().SetMaterial("SkeleBossMat");

	m_enemy->AddComponent<Aen::OBBox>();
	m_enemy->GetComponent<Aen::OBBox>().SetBoundingBox(2, 2, 2);

	m_enemy->AddComponent<Aen::AABoundBox>();
	m_enemy->GetComponent<Aen::AABoundBox>().SetBoundsToMesh();
	m_enemy->GetComponent<Aen::AABoundBox>().SetBoundingBox(1,1,1);

	//m_enemy->setPos(1, 1, 1);
}

void NecroBoss::Update(const float& deltaTime, Player& player)
{
	m_deltatime = deltaTime;
	Aen::Vec3f eDir = player.GetEntity()->GetPos() - m_enemy->GetPos();
	float distance = eDir.Magnitude();
	m_v = Aen::Vec3f::zero;

	m_direction = Aen::Lerp(m_direction, eDir.Normalized(), 0.03f);
	float yaw = Aen::RadToDeg(std::atan2(m_direction.x, m_direction.z));
	

	if (!m_eventQueue.empty())
	{
		if (m_eventQueue.front().duration > 0.f)
		{
			m_eventQueue.front().function(m_eventQueue.front().accell, m_eventQueue.front().duration);
			m_eventQueue.front().duration -= deltaTime;
		}
		else
		{
			m_eventQueue.pop_front();
		}



	}

	m_enemy->SetRot(0.f, yaw + 180, 0.f);

	if (Aen::Input::KeyDown(Aen::Key::G))
		LightAttack();
	m_v += m_v += Aen::Vec3f(-m_v.x * 1.8f, -30.f, -m_v.z * 1.8f) * deltaTime;
	m_v = Aen::Clamp(m_v, -Aen::Vec3f(20.f, 20.f, 20.f), Aen::Vec3f(20.f, 20.f, 20.f));
	m_enemy->GetComponent<Aen::CharacterController>().Move(m_v * m_deltatime, m_deltatime);
}

void NecroBoss::LightAttack()
{
	EventData data;
	data.function = [&](float& accell, const float& attackDuration) {
		
		m_v = m_direction * 4.f;
	
	};
	data.duration = .1f;


	m_eventQueue.emplace_back(data);
}
