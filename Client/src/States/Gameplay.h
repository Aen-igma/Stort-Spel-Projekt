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
    Aen::Entity* m_spotLight;
    Aen::Entity* m_plane;
    Aen::Entity* m_plane1;
    Aen::Entity* m_cube;

    //Aen::Entity* m_skele;
    Aen::Entity* m_goblin;

    Aen::Mesh* m_meshcube;
    Aen::Entity* m_sphere;

    Aen::Entity* rooms[mapSize * mapSize];

    Aen::Mesh* m_reimubeMesh;
    Aen::Material* m_ReimuMat;
    Aen::Texture* m_ReimuTex;

    Aen::Entity* m_emiCube;

    std::stack<Aen::Entity*> m_reimubes;
    std::stack<Aen::Entity*> m_pLights;

public:
	Gameplay(Aen::Window& window);
	~Gameplay()override;

	// Inherited via State
	virtual void Update(const float& deltaTime) override;
	virtual void Initialize() override;
};