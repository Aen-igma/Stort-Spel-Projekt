#pragma once
#include"AenirEngine.h"
#include"AenirEngine/ThirdParty/ImGui/imgui.h"
#include"AenirEngine/LevelGeneration/LevelGenerator.h"

#include<stack>

class Client : public Aen::App {
	public:
	~Client() override;
	Client() = delete;
	Client(const Aen::WindowDesc& desc, const std::wstring& windowName, const std::wstring& className);

	void Start() override;
	void Update(const float& deltaTime) override;

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
	
	Aen::Mesh* m_meshcube;
	Aen::Entity* m_sphere;

	//For accessing level generation functions
	Aen::LevelGenerator m_levelGenerator;
	Aen::Room* mptr_map;
	Aen::Entity* rooms[Aen::mapSize * Aen::mapSize];

	Aen::Mesh* m_reimubeMesh;
	Aen::Material* m_ReimuMat;
	Aen::Texture* m_ReimuTex;

	Aen::Entity* m_emiCube;

	std::stack<Aen::Entity*> m_reimubes;
	std::stack<Aen::Entity*> m_pLights;
};


Aen::App* Aen::CreateApp() {

	Aen::WindowDesc wDesc;
	wDesc.width = GetSystemMetrics(SM_CXSCREEN) + 4u;
	wDesc.height = GetSystemMetrics(SM_CYSCREEN) + 4u;
	wDesc.EXStyle = AEN_WS_EX_APPWINDOW;
	wDesc.style = AEN_WS_OVERLAPPEDWINDOW | AEN_WS_VISIBLE;
	wDesc.hMenu = 0;
	wDesc.WCStyle = 0;

	return new Client(wDesc, L"Aenir Engine", L"Main_Window");
}

