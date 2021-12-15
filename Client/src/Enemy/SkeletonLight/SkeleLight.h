#pragma once
#include"../Enemy.h"

class SkeleLight : public Enemy {
public:
	SkeleLight();
	SkeleLight(const Aen::Vec3f& pos);
	~SkeleLight() override;

	Aen::Entity*& GetEntity();

	virtual void Update(const float& deltaTime, Player& player);
private:

	void CombatEvent(const float& deltaTime, const float& distance);
	void RandomIdleEvent(const float& deltaTime, const Aen::Vec2f& randDir);
	void WaitEvent();

	Aen::Entity* mp_skeleton;
	Aen::Entity* mp_healthBar;
	Aen::Entity* mp_hurtbox;

	Aen::Animator* m_animator;

	Aen::Vec3f m_Dir;
	Aen::Vec3f m_lDir;
	Aen::Vec2f m_nDir;
	Aen::Vec2f m_rDir;

	bool m_hurting;
	bool m_wait;
};
