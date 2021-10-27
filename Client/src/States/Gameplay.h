#pragma once
#include "States.h"
#include "../Level/LevelGenerator.h"
#include"AenirEngine/ThirdParty/ImGui/imgui.h"
#include "AenirEngine\Graphics\Component\EntityHandler.h"
#include<queue>
#include<functional>

struct EventData {
    float duration;
    float accell;
    std::function<void(float& accell)> function;
};

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
    Aen::Entity* m_reimube;

    Aen::Entity* m_target;
    float m_targetDist;
    
    std::queue<EventData> m_eventQueue;
    float m_movementSpeed;
    Aen::Vec3f m_finalDir;

public:
	Gameplay(Aen::Window& window);
	~Gameplay() override;

	// Inherited via State
	virtual void Update(const float& deltaTime) override;
	virtual void Initialize() override;
};