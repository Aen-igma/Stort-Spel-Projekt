#pragma once
#include"../Enemy.h"

class Rimuru : public Enemy {
	public:
	Rimuru();
	Rimuru(const Aen::Vec3f &pos);
	~Rimuru() override;

	Aen::Entity*& GetEntity();


	// Inherited via Enemy
	virtual void Update(const float& deltaTime, Player& player) override;

	private:

	void RandomCombatEvent(const float& deltaTime);
	void RandomIdleEvent(const float& deltaTime, const Aen::Vec2f& randDir);
	void WaitEvent();
	
	float m_damage = 80.f;
	Aen::Entity* m_healthBar;
	Aen::Entity* m_rimuru;
	Aen::Vec3f m_lDir;
	Aen::Vec3f m_Dir;
	bool m_toggleAttacked;
	bool m_dodge;
	bool m_hurting;
	bool m_wait;
	Aen::Vec2f m_rDir;
};