#pragma once
#include "States.h"
#include "AenirEngine/LevelGeneration/LevelGenerator.h"
#include"AenirEngine/ThirdParty/ImGui/imgui.h"
#include "AenirEngine\Graphics\Component\EntityHandler.h"

class Victory : public State
{
private:
    float m_speed;
    float m_fSpeed;
    float m_mouseSense;
    bool m_toggleFullScreen;

    Aen::Entity* m_UI;

public:
    Victory(Aen::Window& window);
    ~Victory()override;

    // Inherited via State
    virtual void Update(const float& deltaTime) override;
    virtual void Initialize() override;
};
