#pragma once
#pragma once
#include "Interact.h"

class Door : public Interact
{
public:
	Door();
	~Door();

	Aen::Entity*& GetEntity();
	bool& GetNear();

	// Inherited via Interact
	virtual void Update(const float& deltaTime, Aen::Entity*& e) override;
	virtual Aen::Entity*& GetObjectEntity() override;
	virtual Type GetType() override;
	virtual void SetType(const Type& type) override;
private:
	Aen::Entity* m_door;
	bool m_near;
};