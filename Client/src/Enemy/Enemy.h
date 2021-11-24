#pragma once
#include"../Player.h"

class Enemy {
	public:
	Enemy();
	virtual~Enemy();
	Aen::Entity*& GetEntity();

	virtual void Update(const float& deltaTime, Player& player) = 0;
	void SetISTargeted(const bool& targeted);
	void SubtractHealth(const float& damage);
	void Move(const Aen::Vec3f& dir);
	const float& GetHealth();

	void Hurt(const bool& hurt);
	const bool IsHurt();
	protected:
	Aen::AABoundBox* mp_hitbox;
	Aen::Entity* m_enemy;
	std::deque<EventData> m_eventQueue;
	bool m_targeted;
	float m_health;
	bool m_hurt;
	Aen::Vec3f m_v;
};