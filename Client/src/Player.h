#pragma once
#include "AenirEngine\AenCore.h"
#include<queue>

class Enemy;
class Boss;

enum class EventType {
	Dash,
	Attack,
	Wait,
	Summon,
	Hit,
};


struct EventData {
	float duration;
	float accell;
	float damage;
	float knockbackForce;
	int nrOfAttacks;
	EventType type;
	std::function<void(float& accell, const float& attackDuration, const int& nrOfAttacks)> function;


	EventData() :duration(0.f), accell(0.f), damage(0.f), knockbackForce(1.f), nrOfAttacks(0), type(EventType::Wait), function() {}
};

struct TargetData {
	float distance;
	Enemy* target;

	TargetData() :distance(0.f), target(nullptr) {}
};

class Player {
public:
	Player();
	~Player();

	void Update(std::deque<Enemy*>& e,const float& deltaTime);
	Aen::Entity*& GetEntity();
	Aen::Entity*& GetHurtBox();
	Aen::Entity*& GetCamera();

	void UpdateAttack(std::deque<Enemy*>& e, const float& deltaTime);
	void SubtractHealth(const float& damage);
	void Move(const Aen::Vec3f& dir);
	void PotionUpdate();
	void IncreaseHealthCap();
	const float& GetHealth();
	int GetPotionNr() const;

	static void SetHealing(const bool& b);
	bool& IsHealing()const;

	void Hurt(float dmg, float force, Aen::Vec3f dir);

	const bool IsAttacking();

	Aen::AABoundBox* GetHitBoxP() const;

	void SetBossP(Boss* boss);

	int GetBossesAlive() const;
	void AddBossesAlive(int n);

private:
	Aen::Animation* m_protagIdleToRun = nullptr;
	void SwordSwing(float speed, float time, const float& deltaTime);
	void ResetSword();
	void AddEvent(EventData& event);

	float m_health;
	float m_potion;
	int m_potionCap;
	int m_nrPotion;
	float m_timer;
	static bool m_healing;

	Aen::Entity* m_playerMeshHolder;
	Aen::Entity* m_hurtbox;
	Aen::Entity* m_player;
	Aen::Entity* m_camera;
	Aen::Entity* m_sword;
	Aen::Entity* m_targetUI;
	Aen::Raycast m_ray;

	Boss* mp_boss;



	float m_mouseSense;
	float m_movementSpeed;
	Aen::Vec3f m_finalDir;

	float m_attackTimer;

	const float m_LIGHTATTACKTIME;
	const float m_HEAVYATTACKTIME;
	const float m_LIGHTCHARGETIME;
	const float m_HEAVYCHARGETIME;
	const float m_LIGHTATTACKSPEED;
	const float m_HEAVYATTACKSPEED;

	Aen::Vec3f m_v;
	std::deque<EventData> m_eventQueue;
	std::deque<TargetData> m_targets;

	Aen::CharacterController* mp_charCont;
	Aen::AABoundBox* mp_hitBox;
	Aen::OBBox* mp_hurtBox;
	Aen::Rotation* m_rot;

	int m_bossesAlive = 0;
};