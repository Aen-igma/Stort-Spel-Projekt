#pragma once
#include"../Player.h"

class Enemy {
	public:
	Enemy();
	~Enemy();

	virtual void Update(const float& deltaTime, Aen::Entity& player) = 0;

	protected:
	Aen::Entity* m_enemy;
	std::queue<EventData> m_eventQueue;
};