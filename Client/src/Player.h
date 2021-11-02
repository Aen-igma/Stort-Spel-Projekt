#pragma once
#include "AenirEngine\AenCore.h"
#include<queue>

class Enemy;

struct EventData {
	float duration;
	float accell;
	std::function<void(float& accell)> function;
};

class Player {
	public:
	Player();
	~Player();

	void Update(Aen::Entity* e,const float& deltaTime);
	Aen::Entity*& GetEntity();
	Aen::Entity*& GetHurtBox();

	bool LightAttack(std::deque<Enemy*>& e, const float deltatime);

private:
	Aen::Entity* m_hurtbox;

	Aen::Entity* m_player;
	Aen::Entity* m_camera;
	Aen::Raycast m_ray;

	bool m_lightAttacking;

	float m_mouseSense;
	float m_movementSpeed;
	Aen::Vec3f m_finalDir;

	Aen::Entity* m_target;
	float m_targetDist;

	float m_attackTimer;

	const float m_LIGHTATTACKTIME;
	const float m_HEAVYATTACKTIME;


	std::queue<EventData> m_eventQueue;
};