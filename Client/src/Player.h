#pragma once
#include "AenirEngine\AenCore.h"
#include<queue>

class Enemy;

enum class EventType {
	Dash,
	Attack
};

struct EventData {
	float duration;
	float accell;
	float damage;
	EventType type;
	std::function<void(float& accell, const float& attackDuration)> function;
};

struct TargetData {
	float distance;
	Enemy* target;
};

class Player {
public:
	Player();
	~Player();

	void Update(std::deque<Enemy*>& e,const float& deltaTime);
	Aen::Entity*& GetEntity();
	Aen::Entity*& GetHurtBox();

	void UpdateAttack(std::deque<Enemy*>& e, const float& deltaTime);
	void SubtractHealth(const float& damage);
	void Move(const Aen::Vec3f& dir);
	const float& GetHealth();

	const bool IsAttacking();

#ifdef _DEBUG
	bool GetImPause() const;
#endif // _DEBUG


private:

	void SwordSwing(float speed, float time, const float& deltaTime);
	void ResetSword();
	void AddEvent(EventData& event);

	float m_health;
#ifdef _DEBUG
	bool m_imPause = false;
#endif // _DEBUG


	Aen::Entity* m_hurtbox;
	Aen::Entity* m_player;
	Aen::Entity* m_camera;
	Aen::Entity* m_sword;
	Aen::Raycast m_ray;

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
};