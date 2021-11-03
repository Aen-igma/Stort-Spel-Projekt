#pragma once
#include "AenirEngine\AenCore.h"
#include<queue>

class Enemy;

struct EventData {
	float duration;
	float accell;
	std::function<void(float& accell)> function;
};

struct TargetData {
	float distance;
	Aen::Entity* target;
};

class Player {
	public:
	Player();
	~Player();

	void Update(std::deque<Enemy*>& e,const float& deltaTime);
	Aen::Entity*& GetEntity();
	Aen::Entity*& GetHurtBox();

private:
	Aen::Entity* m_hurtbox;

	Aen::Entity* m_player;
	Aen::Entity* m_camera;
	Aen::Raycast m_ray;

	bool m_lightAttacking;
	bool m_heavyAttacking;

	float m_mouseSense;
	float m_movementSpeed;
	Aen::Vec3f m_finalDir;

	float m_attackTimer;

	const float m_LIGHTATTACKTIME;
	const float m_HEAVYATTACKTIME;
	const float m_LIGHTCHARGETIME;
	const float m_HEAVYCHARGETIME;

	bool LightAttack(std::deque<Enemy*>& e, const float deltatime);
	bool HeavyAttack(std::deque<Enemy*>& e, const float deltatime);


	std::queue<EventData> m_eventQueue;
	std::deque<TargetData> m_targets;
};