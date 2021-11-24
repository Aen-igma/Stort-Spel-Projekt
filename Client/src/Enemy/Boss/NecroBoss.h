#pragma once
#include"../Enemy.h"

class Boss : public Enemy
{
private:
	const float LIGHTDMG;
	const float HEAVYDMG;
	const float LIGHTFORCE;
	const float HEAVYFORCE;
public:
	Boss(float hp = 10);
	virtual~Boss();
	// Inherited via Enemy
	virtual void Update(const float& deltaTime, Player& player) override;

protected:
	//Aen::Entity* m_hurtbox;

private:
	void LightAttack();
	void FleeAttack();
	void DashAttack();

	void UpdateAttack();

	Aen::Entity* mE_hurtBox;
	Aen::OBBox* mp_hurtBox;

	float m_deltatime;
	Aen::Vec3f m_direction;

	Player* mp_player;

	float m_hp;
	bool m_isEngaged;
	bool m_hurting;

	float m_knockBackForce;
	float m_attackDamage;
};

