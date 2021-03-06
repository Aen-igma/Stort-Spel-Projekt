#pragma once
#include "States.h"
#include "AenirEngine/LevelGeneration/LevelGenerator.h"
#include "AenirEngine/ThirdParty/ImGui/imgui.h"
#include "AenirEngine\Graphics\Component\EntityHandler.h"
#include"../Enemy/Enemies.h"
#include "../Actors/Chest.h"
#include "../Actors/Door.h"
#include "../Actors/Gravestone.h"
#include "../Actors/Exit.h"
#include"AenirEngine\LevelEditor\ImGuiImporter.h"
#include<functional>

class Gameplay : public State {
private:
    float m_hp;
    float m_sub;
    float m_timer;
    float m_deathTimer;
    float m_bossHP;
    float m_TransTimer;

    float m_iFrames;
    const float IFRAMEMAX;
    bool m_invincible;

    float m_speed;
    float m_fSpeed;
    float camSpeed = 0.5;
    bool m_toggleFullScreen;
    bool m_paused = false;
    bool m_BossTorch = false;
#ifdef _DEBUG
    bool m_debug = false;
    Aen::Entity* m_debugCam;
    Aen::Entity* m_debugFrustum;
#endif
    Aen::Vec2f screenSize;

    Aen::Raycast m_ray;

    Aen::Entity* m_plane;
    Aen::Entity* m_UI;


    Aen::Entity* m_wall;
    Aen::Entity* m_throne;
    Aen::Entity* m_dLight;
 
    Aen::Entity* m_bill;
    Aen::Material& m_Mat;
    Aen::UIComponent* mp_uiComp;

    Player m_player;
    Boss* m_pSkeleBoss;


    Chest m_chest;
    Door m_door;
    Gravestone m_grave;
    Portal m_exit;
    bool m_beatBoss;
    Aen::Vec3f m_bossPos;


    vector<Aen::Entity*> m_PSList;

    std::queue<EventData> m_eventQueue;

    std::deque<Enemy*> m_enemyQueue;

    Aen::LevelGenerator m_levelGenerator;
    Aen::Room* mptr_map;


    public:
    Gameplay(Aen::Window& window);
    ~Gameplay() override;

    // Inherited via State
    virtual void Update(const float& deltaTime) override;
    virtual void Initialize() override;
};