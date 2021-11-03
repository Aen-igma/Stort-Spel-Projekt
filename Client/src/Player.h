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

	void UpdateAttack(std::deque<Enemy*>& e,const float& deltaTime);

	const bool IsAttacking();

private:
	
	const float* mp_deltaTime;

	void SwordSwing(float speed, float time);
	void ResetSword();
	void AddEvent(EventData& event);

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

	//bool LightAttack(std::deque<Enemy*>& e, const float deltatime);
	//bool HeavyAttack(std::deque<Enemy*>& e, const float deltatime);


	std::deque<EventData> m_eventQueue;
	std::deque<TargetData> m_targets;
};