#pragma once
#include "States.h"
#include "../Level/LevelGenerator.h"
#include"AenirEngine/ThirdParty/ImGui/imgui.h"
#include "AenirEngine\Graphics\Component\EntityHandler.h"

class GameEnd : public State
{
private:
    float m_speed;
    float m_fSpeed;
    float m_mouseSense;
    bool m_toggleFullScreen;

public:
    GameEnd(Aen::Window& window);
    ~GameEnd()override;

    // Inherited via State
    virtual void Update(const float& deltaTime) override;
    virtual void Initialize() override;
};
