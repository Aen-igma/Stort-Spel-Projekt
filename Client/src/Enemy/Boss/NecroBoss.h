#pragma once
#include"../Enemy.h"
class NecroBoss : public Enemy
{
public:
	NecroBoss();
	
	// Inherited via Enemy
	virtual void Update(const float& deltaTime, Player& player) override;
private:
	void LightAttack();
	void FleeAttack();
	void DashAttack();

	float m_deltatime;
	Aen::Vec3f m_direction;
};

