#pragma once
#include"AenirEngine.h"
#include"AenirEngine/ThirdParty/ImGui/imgui.h"
#include"Level/LevelGenerator.h"

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
	Aen::Entity* m_plane;
	Aen::Entity* m_plane1;

	Aen::Entity* rooms[mapSize * mapSize];

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


