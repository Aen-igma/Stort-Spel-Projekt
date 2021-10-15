#pragma once
#include"AenirEngine.h"
#include"AenirEngine/ThirdParty/ImGui/imgui.h"
#include "AenirEngine/ImGuiHandler.h"
#include"Level/LevelGenerator.h"


class levelBuilder : public Aen::LevelGenerator {
	private:
		//Generation storage
		std::unordered_map<std::string, Aen::Mesh*> m_meshPtrMap;
		//------------------------//																																				
		//Room* mptr_map;
		void inline initLevelBuilder();
	public:
		//void setMapPointer(Room* ptr);

		
		bool CreateRooms(Aen::Entity** storage, uint8_t x, uint8_t y);
		levelBuilder();
};


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

	Aen::Entity m_camera;
	Aen::Entity m_dLight;
	Aen::Entity m_spotLight;
	Aen::Entity m_plane;

	
	Aen::Mesh* m_meshcube;

	//Generation storage
	levelBuilder m_buildLevel; //Var name suggestions open
	Aen::Entity* rooms[Aen::mapSize * Aen::mapSize]; //Static array
	//std::vector<Aen::Mesh> roomMeshes;

	//-----//
	std::unordered_map<int, Aen::Entity*> m_reimubes;
	Aen::Mesh* m_reimubeMesh;
	Aen::Material* m_ReimuMat;
	Aen::Texture* m_ReimuTex;
	int m_reimubeCount;

	std::unordered_map<int, Aen::Entity*> m_pLights;
	int ind;
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

