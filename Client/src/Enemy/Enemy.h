#pragma once
#include"../Player.h"

class Enemy {
	public:
	Enemy();
	virtual~Enemy();
	Aen::Entity*& GetEntity();

	virtual void Update(const float& deltaTime, Player& player) = 0;
	void SetISTargeted(const bool& targeted);

	protected:
	Aen::Entity* m_enemy;
	std::queue<EventData> m_eventQueue;
	bool m_targeted;
};