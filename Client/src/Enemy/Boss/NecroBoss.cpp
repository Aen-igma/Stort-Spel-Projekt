#include "NecroBoss.h"

Boss::Boss(float hp) :
	//m_hurtbox(&Aen::EntityHandler::CreateEntity()),
	mE_hurtBox(&Aen::EntityHandler::CreateEntity()),
	Enemy(), m_direction(0.f, 0.f, 1.f),
	m_thronePosition(m_enemy->GetPos() + Aen::Vec3f(0.f, 4.f, 0.f)),

	m_isEngaged(false), m_hp(hp), m_isHurting(false), m_areMinionsSummoned(false),
	LIGHTDMG(20.f), HEAVYDMG(50.f), LIGHTFORCE(20.f), HEAVYFORCE(100.f), BASESPEED(3.f),
	m_stage(0)
{
	m_health = 100.f;

	m_speed = BASESPEED;

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

	mp_hitbox->ToggleActive(true);

	//mE_hurtBox->SetPos(1, 1, 1);
	//phurtbox->SetBoundingBox(2, 7, 2);

}

Boss::~Boss()
{
	mE_hurtBox->RemoveParent();
	UINT minionSize = m_pMinions.size();
	for (int i = 0; i < minionSize; i++)
	{
		delete m_pMinions[i];
	}
}

void Boss::Update(const float& deltaTime, Player& player)
{
	mp_player = &player;

	if (Aen::Input::KeyDown(Aen::Key::K))
		m_isEngaged = !m_isEngaged;

	if (m_isEngaged)
	{
		//m_v = Aen::Vec3f::zero;
		for (int i = 0; i < m_pMinions.size(); i++)
		{
			m_pMinions[i]->Update(deltaTime, player);
		}


		m_deltatime = deltaTime;
		Aen::Vec3f eDir = player.GetEntity()->GetPos() - m_enemy->GetPos();
		float distance = eDir.Magnitude();
		eDir.Normalized();
		

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
				m_eventQueue.front().function(m_eventQueue.front().accell, m_eventQueue.front().duration, m_eventQueue.front().nrOfAttacks);
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
			SummonSlimes(6);
		if (Aen::Input::KeyDown(Aen::Key::L))
			GoToThrone();
		UpdateAttack();

		m_v = m_direction * m_speed;
		m_v += Aen::Vec3f(-m_v.x * 1.8f, -30.f, -m_v.z * 1.8f) * deltaTime;
		m_v = Aen::Clamp(m_v, -Aen::Vec3f(20.f, 20.f, 20.f), Aen::Vec3f(20.f, 20.f, 20.f));
		m_v += Aen::Vec3f(-m_v.x * 1.8f, -30.f, -m_v.z * 1.8f) * deltaTime;
		m_v.x;
	}
	
	mp_charCont->Move(m_v * m_deltatime, m_deltatime);
}

void Boss::SetThronePosition(Aen::Vec3f v)
{
	m_thronePosition = v;
}

void Boss::SetThronePosition(float x, float y, float z)
{
	m_thronePosition = Aen::Vec3f(x, y, z);
}

std::vector<Rimuru*>& Boss::GetEnemies()
{
	return m_pMinions;
}

void Boss::LightAttack()
{
	EventData data;
	data.function = [&](float& accell, const float& attackDuration, const int& nrOf) {

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
	data.function = [&](float& accell, const float& attackDuration, const int& nrOf) {

		mp_hurtBox->ToggleActive(true);
		m_v = m_direction * 4.f * accell;

	};

	m_eventQueue.emplace_back(data);
}

void Boss::GoToThrone()
{
	EventData data;
	data.type = EventType::Wait;
	data.function = [&](float& accell, const float& attackDuration, const int& nrOf) {
		m_enemy->SetPos(m_thronePosition);
	};

	m_eventQueue.emplace_back(data);
}

void Boss::SummonSlimes(int amountOfSLimes)
{
	if (!m_areMinionsSummoned)
	{
		m_areMinionsSummoned = true;
		float minionPos = 0.f;
		EventData data;

		data.nrOfAttacks = amountOfSLimes;
		data.duration = .01f;
		data.type = EventType::Summon;
		data.damage = amountOfSLimes;
		data.knockbackForce = 0;
		data.accell = 0.f;
		data.function = [&](float& accell, const float& attackDuration, const int& nrOf) {

			float bengt = 0.f;
			for (int i = 0; i < nrOf; i++)
			{
				m_pMinions.emplace_back(new Rimuru({ -11.f + minionPos, 3, minionPos }));
				minionPos += 3.f;
			}


		};

		m_eventQueue.emplace_back(data);
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

	//if (!m_eventQueue.empty() && m_eventQueue.front().type == EventType::Hit)
		//printf("e");

}
