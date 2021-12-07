#pragma once
#include "Interact.h"

class Chest : public Interact
{
public:
	Chest();
	~Chest();

	// Inherited via Interact
	virtual void Update(const float& deltaTime, Aen::Entity*& e) override;
	virtual Type GetType() override;
	virtual bool& GetNear() override;
	virtual Aen::Entity*& GetEntity() override;
	virtual void SetType(const Type& type) override;
private:
	Aen::Entity* m_chest;
};