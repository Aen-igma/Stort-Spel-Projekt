#pragma once
#include"../Enemy.h"

class Rimuru : public Enemy {
	public:
	Rimuru();
	~Rimuru() override;

	// Inherited via Enemy
	virtual void Update(const float& deltaTime, Aen::Entity& player) override;

	private:
	Aen::Entity* m_rimuru;
	Aen::Vec3f m_lDir;
	Aen::Vec3f m_Dir;
	Aen::Vec3f m_v;

};