#pragma once
#include "Interact.h"

class Chest : public Interact
{
public:
	Chest();
	~Chest();

	Aen::Entity*& GetEntity();
	bool& GetNear();

	// Inherited via Interact
	virtual void Update(const float& deltaTime, Aen::Entity*& e) override;
	virtual Aen::Entity*& GetObjectEntity() override;
	virtual Type GetType() override;
	virtual void SetType(const Type& type) override;
private:
	Aen::Entity* m_chest;
	bool m_near;
};