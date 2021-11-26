#include "NecroBoss.h"

Boss::Boss(float hp) :
	//m_hurtbox(&Aen::EntityHandler::CreateEntity()),
	mE_hurtBox(&Aen::EntityHandler::CreateEntity()),
	Enemy(), m_direction(0.f, 0.f, 1.f),

	m_isEngaged(false), m_hp(hp), m_isHurting(false), m_areMinionsSummoned(false),
	LIGHTDMG(20.f), HEAVYDMG(50.f), LIGHTFORCE(20.f), HEAVYFORCE(100.f),
	m_speed(2.f)
{
	m_health = 100.f;


	Aen::Mesh& skeleBoss = Aen::Resource::CreateMesh("SkeleBoss");
	skeleBoss.Load(AEN_RESOURCE_DIR("Models/Skeleton/skBoss_test.fbx"));
	Aen::Material& skeleBossMat = Aen::Resource::CreateMaterial("SkeleBossMat");
	skeleBossMat["GlowColor"] = Aen::Color::Cyan;
	m_enemy->AddComponent<Aen::MeshInstance>();
	m_enemy->GetComponent<Aen::MeshInstance>().SetMesh(skeleBoss);
	m_enemy->GetComponent<Aen::MeshInstance>().SetMaterial("SkeleBossMat");
	skeleBossMat.LoadeAndSetDiffuseMap(AEN_RESOURCE_DIR("Textures/Skeleton/SkeletonBoss_diffuse.png"));
	skeleBossMat.LoadeAndSetEmissionMap(AEN_RESOURCE_DIR("Textures/Skeleton/SkeletonBoss_glow.png"));

	mE_hurtBox->AddComponent<Aen::OBBox>();
	mp_hurtBox = &mE_hurtBox->GetComponent<Aen::OBBox>();

	mp_hurtBox->SetBoundingBox(1, 3, 1);

	m_enemy->SetPos(1, 0, 3);
	mE_hurtBox->SetParent(*m_enemy);

	//mE_hurtBox->SetPos(1, 1, 1);
	//phurtbox->SetBoundingBox(2, 7, 2);

}

Boss::~Boss()
{
	mE_hurtBox->RemoveParent();
}

void Boss::Update(const float& deltaTime, Player& player)
{
	mp_player = &player;

	for (int i = 0; i < m_pMinions.size(); i++)
	{
		m_pMinions[i]->Update(deltaTime, player);
	}

	m_deltatime = deltaTime;
	Aen::Vec3f eDir = player.GetEntity()->GetPos() - m_enemy->GetPos();
	float distance = eDir.Magnitude();
	eDir.Normalized();
	m_v = Aen::Vec3f::zero;

	m_direction = Aen::Lerp(m_direction, eDir.Normalized(), 0.03f);
	float yaw = std::atan2(m_direction.x, m_direction.z);
	mp_hurtBox->SetOrientation(0, yaw, 0);
	yaw = Aen::RadToDeg(yaw);
	m_enemy->SetRot(0.f, yaw + 180, 0.f);
	Aen::Vec3f attackPos = m_direction * 3 /*+ m_enemy->GetPos()*/;
	mE_hurtBox->SetPos(attackPos);


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

	float zero = 0;
	if (Aen::Input::KeyDown(Aen::Key::G))
		LightAttack();

	if (Aen::Input::KeyDown(Aen::Key::H))
		BigAttack();
	if (Aen::Input::KeyDown(Aen::Key::J))
		SummonSlimes(3);

	m_v += m_v += Aen::Vec3f(-m_v.x * 1.8f, -30.f, -m_v.z * 1.8f) * deltaTime;
	m_v = Aen::Clamp(m_v, -Aen::Vec3f(20.f, 20.f, 20.f), Aen::Vec3f(20.f, 20.f, 20.f));

	if (Aen::Input::KeyDown(Aen::Key::K))
		m_isEngaged = !m_isEngaged;

	if (m_isEngaged)
	{
		m_v += m_direction * 1.3;
	}

	m_enemy->GetComponent<Aen::CharacterController>().Move(m_v * m_deltatime, m_deltatime);

	UpdateAttack();
}

void Boss::LightAttack()
{
	EventData data;
	data.function = [&](float& accell, const float& attackDuration) {

		mp_hurtBox->ToggleActive(true);
		m_v = m_direction * accell;
	};
	data.accell = 4.f;
	data.duration = .3f;
	data.type = EventType::Attack;
	data.damage = LIGHTDMG;
	data.knockbackForce = LIGHTFORCE;


	m_eventQueue.emplace_back(data);
}

void Boss::BigAttack()
{
	EventData data;
	data.duration = .5f;
	data.type = EventType::Attack;
	data.damage = HEAVYDMG;
	data.knockbackForce = HEAVYFORCE;
	data.accell = .1f;
	data.function = [&](float& accell, const float& attackDuration) {

		mp_hurtBox->ToggleActive(true);
		m_v = m_direction * 4.f * accell;

	};

	m_eventQueue.emplace_back(data);
}

void Boss::GoToThrone()
{
	EventData data;
	data.duration = .5f;
	data.type = EventType::Dash;
	data.accell = .1f;
	data.function = [&](float& accell, const float& attackDuration) {

	};

	m_eventQueue.emplace_back(data);
}

void Boss::SummonSlimes(int amountOfSLimes)
{
	if (!m_areMinionsSummoned)
	{
		m_areMinionsSummoned = true;
		float bengt = 0.f;
		for (int i = 0; i < amountOfSLimes; i++)
		{
			bengt += 3.f;
			EventData data;

			data.duration = .01f;
			data.type = EventType::Wait;
			data.damage = 0;
			data.knockbackForce = 0;
			data.accell = 0.f;
			data.function = [&](float& accell, const float& attackDuration) {

				m_pMinions.emplace_back(new Rimuru({ -11.f, bengt, bengt }));


			};

			m_eventQueue.emplace_back(data);
		}
	}
	


}

void Boss::UpdateAttack()
{
	if (!m_eventQueue.empty() && m_eventQueue.front().type == EventType::Attack)
	{
		if (mp_hurtBox->Intersects(mp_player->GetEntity()->GetComponent<Aen::AABoundBox>()))
		{
			if (!m_isHurting)
			{
				EventData currentEvent(m_eventQueue.front());
				m_isHurting = true;
				mp_player->Hurt(currentEvent.damage, currentEvent.knockbackForce, m_direction.Normalized());
			}


		}
		else m_isHurting = false;
	}
	else
		mp_hurtBox->ToggleActive(false);

	if (!m_eventQueue.empty() && m_eventQueue.front().type == EventType::Summon)
	{

	}

	//if (!m_eventQueue.empty() && m_eventQueue.front().type == EventType::Wait)
		//printf("e");

}
