#pragma once
#include "AenirEngine\AenCore.h"

enum class Type{
	Opened,
	Closed,
	Locked
};

struct Data
{
	float duration;
	Type type;
	std::function<void(float& accell, const float& duration)> function;

	Data() :duration(0.f), type(), function() {};
};

class Interact
{
public:
	Interact();
	virtual ~Interact();
	Aen::Entity*& GetEntity();

	virtual void Update(const float& deltaTime, Aen::Entity*& e) = 0;
protected:
	Aen::Entity* mp_object;
	static bool m_near;
	std::deque<Data> m_eventQueue;
};