#pragma once
#include "AenirEngine\AenCore.h"
#include<queue>

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


private:
	Aen::Entity* m_hurtbox;

	Aen::Entity* m_player;
	Aen::Entity* m_camera;
	Aen::Raycast m_ray;

	float m_mouseSense;
	float m_movementSpeed;
	Aen::Vec3f m_finalDir;

	Aen::Entity* m_target;
	float m_targetDist;

	std::queue<EventData> m_eventQueue;
};