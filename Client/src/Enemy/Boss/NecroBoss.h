#pragma once
#include"../Rimuru/Rimuru.h"

class Boss : public Enemy
{
private:
	const float LIGHTDMG;
	const float HEAVYDMG;
	const float LIGHTFORCE;
	const float HEAVYFORCE;
public:
	Boss(float hp = 100.f);
	virtual~Boss();
	// Inherited via Enemy
	virtual void Update(const float& deltaTime, Player& player) override;

protected:
	//Aen::Entity* m_hurtbox;

private:
	void LightAttack();
	void BigAttack();
	void DashAttack();
	void GoToThrone();
	void SummonSlimes(int amountOfSLimes);

	void UpdateAttack();

	Aen::Entity* mE_hurtBox;
	Aen::OBBox* mp_hurtBox;

	float m_deltatime;
	Aen::Vec3f m_direction;

	Player* mp_player;
	std::vector<Rimuru*> m_pMinions;

	Aen::Vec3f m_thronePosition; // TODO: Pair with procedual generation

	float m_hp;
	bool m_isEngaged;
	bool m_isHurting;
	bool m_isCasting;
	bool m_areMinionsSummoned;

	float m_knockBackForce;
	float m_attackDamage;

	float m_speed;
	const float BASESPEED;

	uint8_t m_stage = 0;

};

