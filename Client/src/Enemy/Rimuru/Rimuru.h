#pragma once
#include"../Enemy.h"

class Rimuru : public Enemy {
	public:
	Rimuru();
	~Rimuru();

	// Inherited via Enemy
	virtual void Update(const float& deltaTime, Aen::Entity& player) override;

	private:

};