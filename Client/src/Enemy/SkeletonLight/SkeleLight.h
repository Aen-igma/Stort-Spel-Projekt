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

	void RandomCombatEvent(const float& deltaTime);
	void RandomIdleEvent(const float& deltaTime, const Aen::Vec2f& randDir);
	void WaitEvent(uint64_t randomTime);

	Aen::Entity* mp_healthBar;
	Aen::Entity* mp_skeleton;

	Aen::Vec3f m_lDir;

	bool m_toggleAttacked;
	bool m_hurting;
	bool m_wait;
};
