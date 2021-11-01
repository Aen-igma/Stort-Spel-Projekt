#pragma once
#include "AenirEngine\AenCore.h"
#include<queue>

struct EventData {
	float duration;
	float accell;
	std::function<void(float& accell)> function;
};

class Enemy {
	public:
	Enemy();
	~Enemy();

	virtual void Update(const float& deltaTime, Aen::Entity& player) = 0;

	protected:
	Aen::Entity* m_enemy;
	std::queue<EventData> m_eventQueue;
};