#pragma once
#include "States.h"
#include "AenirEngine/LevelGeneration/LevelGenerator.h"
#include "AenirEngine/ThirdParty/ImGui/imgui.h"
#include "AenirEngine\Graphics\Component\EntityHandler.h"
#include"../Enemy/Enemies.h"
#include"AenirEngine\LevelEditor\ImGuiImporter.h"
#include<functional>

class Gameplay : public State {
private:
    float m_hp;
    float m_sub;

    float m_iFrames;
    const float IFRAMEMAX;
    bool m_invincible;

    float m_speed;
    float m_fSpeed;
    bool m_toggleFullScreen;

    Aen::Raycast m_ray;

    Aen::Entity* m_dLight;
    Aen::Entity* m_plane;
    Aen::Entity* m_reimube1;

    Aen::Entity* m_UI;
    Aen::Entity* m_wall;
    Aen::Entity* m_bones;
    Aen::Entity* m_pot;
    Aen::Entity* m_torch;
    Aen::Entity* m_healingPot;
    Aen::Entity* m_skelLight;
    Aen::Entity* m_door;
    Aen::Entity* m_chest;
    Aen::Entity* m_tim;


    Player m_player;
    bool m_beatBoss;

    Aen::ImGuiImporter m_levelImporter;

    std::queue<EventData> m_eventQueue;

    std::deque<Enemy*> m_enemyQueue;

    Aen::LevelGenerator m_levelGenerator;
    Aen::Room* mptr_map;
    Aen::Entity* rooms[Aen::mapSize * Aen::mapSize];

    public:
    Gameplay(Aen::Window& window);
    ~Gameplay() override;

    // Inherited via State
    virtual void Update(const float& deltaTime) override;
    virtual void Initialize() override;
};