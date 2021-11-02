#pragma once
#include"../Player.h"

class Enemy {
	public:
	Enemy();
	virtual~Enemy();
	Aen::Entity*& GetEntity();

	virtual void Update(const float& deltaTime, Player& player) = 0;

	protected:
	Aen::Entity* m_enemy;
	std::queue<EventData> m_eventQueue;
};