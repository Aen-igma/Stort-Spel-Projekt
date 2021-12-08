#pragma once
#include "Interact.h"

class Portal : public Interact
{
public:
	Portal();
	~Portal();
	
	void Update(const float& deltaTime);

	// Inherited via Interact
	virtual void Update(const float& deltaTime, Aen::Entity*& e) override;
	virtual Type GetType() override;
	virtual bool& GetNear() override;
	virtual Aen::Entity*& GetEntity() override;
	virtual void SetType(const Type& type) override;
private:
	Aen::Entity* m_exit;
	float m_scale;
};
