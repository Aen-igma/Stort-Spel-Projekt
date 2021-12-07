#pragma once
#include "Interact.h"

class Gravestone : public Interact
{
public:
	Gravestone();
	~Gravestone();

	Aen::Entity*& GetEntity();
	bool& GetNear();

	// Inherited via Interact
	virtual void Update(const float& deltaTime, Aen::Entity*& e) override;
	virtual Type GetType() override;
	virtual void SetType(const Type& type) override;
private:
	Aen::Entity* m_door;
};