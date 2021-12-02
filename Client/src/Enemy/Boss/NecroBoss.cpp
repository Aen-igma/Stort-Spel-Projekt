#include "NecroBoss.h"

Boss::Boss(const Aen::Vec3f position, float hp) :
	//m_hurtbox(&Aen::EntityHandler::CreateEntity()),
	mE_hurtBox(&Aen::EntityHandler::CreateEntity()),
	Enemy(EnemyType::BOSS), m_direction(0.f, 0.f, 1.f), 
	m_thronePosition(m_enemy->GetPos() + Aen::Vec3f(0.f, 4.f, 0.f)), bs(BossState::STATIONARY),

	m_isHurting(false), m_cantSummonSlimes(false), m_waiting(false),
	LIGHTDMG(20.f), HEAVYDMG(50.f), LIGHTFORCE(20.f), HEAVYFORCE(100.f), BASESPEED(3.f),
	m_MAXHP(hp)
{
	m_health = hp;

	m_speed = BASESPEED;

	Aen::Mesh& skeleBoss = Aen::Resource::CreateMesh("SkeleBoss");
	skeleBoss.Load(AEN_RESOURCE_DIR("Models/Skeleton/skBoss_test.fbx"));
	Aen::Material& skeleBossMat = Aen::Resource::CreateMaterial("SkeleBossMat");
	skeleBossMat["GlowColor"] = Aen::Color::Cyan;
	m_enemy->AddComponent<Aen::MeshInstance>();
	m_enemy->GetComponent<Aen::MeshInstance>().SetMesh(skeleBoss);
	m_enemy->GetComponent<Aen::MeshInstance>().SetMaterial("SkeleBossMat");
	mp_meshInst = &m_enemy->GetComponent<Aen::MeshInstance>();
	skeleBossMat.LoadeAndSetDiffuseMap(AEN_RESOURCE_DIR("Textures/Skeleton/SkeletonBoss_diffuse.png"));
	skeleBossMat.LoadeAndSetEmissionMap(AEN_RESOURCE_DIR("Textures/Skeleton/SkeletonBoss_glow.png"));

	mE_hurtBox->AddComponent<Aen::OBBox>();
	mp_hurtBox = &mE_hurtBox->GetComponent<Aen::OBBox>();

	mp_hurtBox->SetBoundingBox(2.f, 3.f, 1.f);

	m_thronePosition = position + Aen::Vec3f(0.f, 5.5f, 0.f);

	m_enemy->SetPos(position);
	mE_hurtBox->SetParent(*m_enemy);
	mE_sword = &Aen::EntityHandler::CreateEntity();
	mE_sword->AddComponent<Aen::MeshInstance>();
	Aen::Mesh& swordMesh = Aen::Resource::CreateMesh("DuckBringer");
	swordMesh.Load(AEN_MODEL_DIR("SwordOffset.fbx"));
	mE_sword->GetComponent<Aen::MeshInstance>().SetMesh(swordMesh);
	mE_sword->SetScale(3.f);
	mE_sword->SetParent(*m_enemy);


	mp_hitbox->ToggleActive(true);

	m_healthBar = &Aen::EntityHandler::CreateEntity();
	m_healthBar->AddComponent<Aen::MeshInstance>();
	m_healthBar->GetComponent<Aen::MeshInstance>().SetMesh("eBar");
	m_healthBar->GetComponent<Aen::MeshInstance>().SetMaterial("barMat");
	m_healthBar->SetRot(180, 0, 0);
	m_healthBar->SetPos(0, -100, 0);
	m_healthBar->SetScale(5.f, 0.f, 5.f);
	m_healthBar->SetRenderLayer(1);
	m_healthBar->SetParent(*m_enemy);

	SetStationary(true);
}

Boss::~Boss()
{
	mE_hurtBox->RemoveParent();
	Aen::EntityHandler::RemoveEntity(*mE_hurtBox);
	Aen::EntityHandler::RemoveEntity(*m_healthBar);
	mE_sword->RemoveParent();
	Aen::EntityHandler::RemoveEntity(*mE_sword);
}

void Boss::Update(const float& deltaTime, Player& player)
{
	//m_areMinionsSummoned = m_pMinions.size() > 0;
	mp_player = &player;
	m_cantSummonSlimes = m_pMinions.size() > 0;
	if (!m_cantSummonSlimes)
		m_waiting = false;

	Aen::Vec3f eDir = player.GetEntity()->GetPos() - m_enemy->GetPos();
	float distance = eDir.Magnitude();
	eDir.Normalized();

	if (!m_hurt)
		mp_meshInst->SetMaterial("SkeleBossMat");
	else
		mp_meshInst->SetMaterial("EnemyMaterialHurt");

	if (distance <= 10.f && bs == BossState(0))
		bs = BossState(1);

	if (bs != BossState::STATIONARY && !m_eventQueue.empty())
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
	switch (bs)
	{
	case BossState::STATIONARY:
	{
		m_v = Aen::Vec3f::zero;
		break;
	}
	case BossState::PHASE1:
	{
		m_deltatime = deltaTime;

		if(distance < 10.f ) m_attackTimer += deltaTime;
		if (m_attackTimer >= 3.f)
		{
			m_attackTimer = 0.f;
			RandomCombatEvent();
		}

		m_direction = Aen::Lerp(m_direction, eDir.Normalized(), 0.03f);
		float yaw = std::atan2(m_direction.x, m_direction.z);
		mp_hurtBox->SetOrientation(0, yaw, 0);
		yaw = Aen::RadToDeg(yaw);
		m_enemy->SetRot(0.f, yaw + 180, 0.f);
		Aen::Vec3f attackPos = m_direction * 3 /*+ m_enemy->GetPos()*/;
		mE_hurtBox->SetPos(attackPos);



		UpdateAttack();
		m_v = m_direction * m_speed;
		static float drag = 5.0f;
		m_v += Aen::Vec3f(-m_v.x * drag, -30.f, -m_v.z * drag) * deltaTime;
		m_v = Aen::Clamp(m_v, -Aen::Vec3f(20.f, 20.f, 20.f), Aen::Vec3f(20.f, 20.f, 20.f));
		m_v += Aen::Vec3f(-m_v.x * 1.8f, -30.f, -m_v.z * 1.8f) * deltaTime;
		if (m_waiting)
			m_v = Aen::Vec3f::zero;

		if (!m_slimesWereCasted && m_health <= m_MAXHP / 2)
		{
			m_slimesWereCasted = true;
			SummonSlimes(6);
			GoToThrone();
		}

#ifdef _DEBUG
		if (Aen::Input::KeyDown(Aen::Key::G))
			LightAttack(deltaTime);
		if (Aen::Input::KeyDown(Aen::Key::H))
			BigAttack(deltaTime);
		if (Aen::Input::KeyDown(Aen::Key::J))
			SummonSlimes(6);
		if (Aen::Input::KeyDown(Aen::Key::L))
			GoToThrone();
		if (Aen::Input::KeyDown(Aen::Key::P))
			Wait(2);
#endif // _DEBUG

		break;
	}
	case BossState::CASTING:
		bs = BossState::CASTING;
		break;
	case BossState::ONTHRONE:
		m_spawnTimer += deltaTime;
		if (m_pMinions.size() == 0 && m_spawnTimer > 2.f)
			bs = BossState::PHASE1;
		m_v = Aen::Vec3f::zero;
		break;
	case BossState::PHASE2:
		break;
	default:
		bs = BossState::CASTING;
		break;
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

void Boss::MakeMinionsSummonable(bool b)
{
	m_cantSummonSlimes = b;
}

std::vector<Rimuru*>& Boss::GetMinions()
{
	return m_pMinions;
}

int Boss::GetEnemiesToSummon()
{
	int minionsToSummon = m_minionsToSummon;
	if (!m_cantSummonSlimes && m_minionsToSummon != 0)
	{
		m_minionsToSummon = 0;
		m_cantSummonSlimes = true;
	}
	return minionsToSummon;
}

int Boss::GetEnemiesInVector() const
{
	return m_pMinions.size();
}

void Boss::EmplaceMinion(Rimuru* e)
{
	m_pMinions.emplace_back(e);
}

void Boss::RemoveMinion(Enemy* e)
{
	delete e;
	e = m_pMinions.back();
	m_pMinions.pop_back();
}

void Boss::RemoveMinion(uint16_t i)
{
	delete m_pMinions[i];
	m_pMinions[i] = m_pMinions.back();
	m_pMinions.pop_back();
}

void Boss::LightAttack(const float& deltaTime)
{
	EventData data;
	data.function = [&](float& accell, const float& attackDuration, const int& nrOf) {
		SwordSwing(500.f, .3f, deltaTime);
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

void Boss::BigAttack(const float& deltaTime)
{
	EventData data;
	data.duration = .5f;
	data.type = EventType::Attack;
	data.damage = HEAVYDMG;
	data.knockbackForce = HEAVYFORCE;
	data.accell = .1f;
	data.function = [&](float& accell, const float& attackDuration, const int& nrOf) {
		SwordSwing(250.f, .5f, deltaTime);
		mp_hurtBox->ToggleActive(true);
		m_v = m_direction * 4.f * accell;

	};

	m_eventQueue.emplace_back(data);
}

void Boss::GoToThrone()
{
	//EventData data;
	//data.type = EventType::Wait;
	//data.function = [&](float& accell, const float& attackDuration, const int& nrOf) {
		
	//};

	//m_eventQueue.emplace_back(data);
}

void Boss::SummonSlimes(int amountOfSLimes)
{
	//m_areMinionsSummoned = true;
	float minionPos = 0.f;
	EventData data;

	data.nrOfAttacks = amountOfSLimes;
	data.duration = .01f;
	data.type = EventType::Summon;
	data.damage = amountOfSLimes;
	data.knockbackForce = 0;
	data.accell = 0.f;
	data.function = [&](float& accell, const float& attackDuration, const int& nrOf) {

		if (!m_cantSummonSlimes)
		{
			m_minionsToSummon = nrOf;
			//for (int i = 0; i < 1; i++)
			//{
				//if (m_areMinionsSummoned)
				//m_pMinions.emplace_back(new Rimuru({ -11.f + minionPos, 3, minionPos }));
				//minionPos += 3.f;
			//}
			m_enemy->SetPos(m_enemy->GetPos() + Aen::Vec3f(0.f, 6.f, 0.f));
			bs = BossState::ONTHRONE;
		}
		else m_minionsToSummon = 0;
		m_spawnTimer = 0;

	};

	m_eventQueue.emplace_back(data);

}

void Boss::Wait(const float duration)
{
	EventData e;
	e.accell = 0.f;
	e.duration = duration;
	e.type = EventType::Wait;
	e.function = [&](float& accell, const float& attackDuration, const int& nrOf) {
		m_waiting = true;
	};
	m_eventQueue.emplace_back(e);
}

void Boss::RandomCombatEvent()
{
	int randN = LehmerInt() % 2;
	switch (randN)
	{
	case 0:
		LightAttack(m_deltatime);
		break;
	case 1:
		BigAttack(m_deltatime);
		break;
	default:
		break;
	}
}

void Boss::SwordSwing(const float& speed, const float& time, const float& deltaTime)
{
	static float timer = 0.f;
	timer += deltaTime;
	if (timer > time)
	{
		mE_sword->SetRot(0, 0 + 180.f, 0);
		timer = 0.f;
	}
	mE_sword->Rotate(-speed * deltaTime, -speed * deltaTime, 0.f);
}

void Boss::ResetSword()
{
	mE_sword->SetRot(0, 180.f, 0);
}

void Boss::UpdateAttack()
{
	if (!m_eventQueue.empty() && m_eventQueue.front().type == EventType::Attack)
	{
		m_cantSummonSlimes = false;
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
	{
		mp_hurtBox->ToggleActive(false);
		ResetSword();
	}
}
