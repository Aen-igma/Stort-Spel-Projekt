#pragma once
#include "States.h"
#include "../Level/LevelGenerator.h"
#include"AenirEngine/ThirdParty/ImGui/imgui.h"
#include <stack>

class Gameplay : public State
{
private:
    float m_speed;
    float m_fSpeed;
    float m_mouseSense;
    bool m_toggleFullScreen;

    Aen::Entity* m_camera;
    Aen::Entity* m_dLight;
    Aen::Entity* m_plane;
    Aen::Entity* m_player;

    Aen::Entity* rooms[mapSize * mapSize];

public:
	Gameplay(Aen::Window& window);
	~Gameplay()override;

	// Inherited via State
	virtual void Update(const float& deltaTime) override;
	virtual void Initialize() override;
};