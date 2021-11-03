#pragma once
#include"../Enemy.h"

class Rimuru : public Enemy {
	public:
	Rimuru();
	~Rimuru() override;

	Aen::Entity*& GetEntity();

	// Inherited via Enemy
	virtual void Update(const float& deltaTime, Player& player) override;

	private:
	Aen::Entity* m_rimuru;
	Aen::Vec3f m_lDir;
	Aen::Vec3f m_Dir;
	Aen::Vec3f m_v;

};