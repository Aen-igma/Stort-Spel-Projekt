#pragma once
#include "States.h"
#include "AenirEngine/LevelGeneration/LevelGenerator.h"
#include "AenirEngine/ThirdParty/ImGui/imgui.h"
#include "AenirEngine\Graphics\Component\EntityHandler.h"
#include"../Enemy/Enemies.h"
#include<functional>

class Gameplay : public State {
    private:
    int m_hp;
    float m_iFrames;
    const float IFRAMEMAX;
    bool m_invincible;

    float m_speed;
    float m_fSpeed;
    bool m_toggleFullScreen;

    Aen::Raycast m_ray;

    Aen::Entity* m_dLight;
    Aen::Entity* m_plane;
    Aen::Entity* m_reimube;
    Aen::Entity* m_UI;
    Aen::Entity* m_wall;

    Player m_player;
    bool m_beatBoss;

    std::deque<Enemy*> m_enemyQueue;

    public:
    Gameplay(Aen::Window& window);
    ~Gameplay() override;

    // Inherited via State
    virtual void Update(const float& deltaTime) override;
    virtual void Initialize() override;
};