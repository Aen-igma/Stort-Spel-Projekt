#pragma once
#include "AenirEngine\AenCore.h"

enum class Type{
	Open,
	Opening,
	Closing,
	Closed,
	Locking,
	Locked
};

class Interact
{
public:
	Interact();
	virtual ~Interact();
	Aen::Entity*& GetEntity();

	virtual void Update(const float& deltaTime, Aen::Entity*& e) = 0;
	virtual Aen::Entity*& GetObjectEntity() = 0;
	virtual Type GetType() = 0;
	virtual void SetType(const Type& type) = 0;
protected:
	Aen::Entity* mp_object;
	static bool m_near;
	Type m_type;
};