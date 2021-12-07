#pragma once
#include "Interact.h"

class Door : public Interact
{
public:
	Door();
	~Door();

	// Inherited via Interact
	virtual void Update(const float& deltaTime, Aen::Entity*& e) override;
	virtual Type GetType() override;
	virtual bool& GetNear() override;
	virtual Aen::Entity*& GetEntity() override;
	virtual void SetType(const Type& type) override;
private:
	Aen::Entity* m_door;
};