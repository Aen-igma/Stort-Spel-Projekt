#pragma once
#include"../Enemy.h"
class NecroBoss : public Enemy
{
public:
	NecroBoss();
	
	// Inherited via Enemy
	virtual void Update(const float& deltaTime, Player& player) override;
private:
	




};

