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
	std::function<void(float& accell)> function;
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

	private:
	
	void AddEvent(EventData& event);

	float m_health;

	Aen::Entity* m_hurtbox;
	Aen::Entity* m_player;
	Aen::Entity* m_camera;
	Aen::Raycast m_ray;

	float m_mouseSense;
	float m_movementSpeed;
	Aen::Vec3f m_finalDir;

	float m_attackTimer;

	const float m_LIGHTATTACKTIME;
	const float m_HEAVYATTACKTIME;
	
	Aen::Vec3f m_v;
	std::deque<EventData> m_eventQueue;
	std::deque<TargetData> m_targets;
};