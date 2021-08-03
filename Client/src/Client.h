#pragma once
#include"AenirEngine.h"



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
	bool m_toggleCamera;

	Aen::Entity m_camera;
	Aen::Entity m_dLight;

	Aen::Entity m_plane;
	Aen::Entity m_plane1;

	//Aen::Entity m_Ani;
	Aen::Entity m_SeeU;
	Aen::Entity m_sphere;
	Aen::Material* m_mat;
};


Aen::App* Aen::CreateApp() {

	Aen::WindowDesc wDesc;
	//wDesc.width = 720;
	//wDesc.height = 480;
	wDesc.width = 1920;
	wDesc.height = 1080;
	wDesc.EXStyle = AEN_WS_EX_APPWINDOW;
	wDesc.style = AEN_WS_OVERLAPPEDWINDOW | AEN_WS_VISIBLE;
	wDesc.hMenu = 0;
	wDesc.WCStyle = 0;
	//wDesc.LoadIcon(L"");
	//wDesc.LoadSmallIcon(L"");
	//wDesc.LoadMouseCursor(L"Heart.cur");

	return new Client(wDesc, L"Aenir Engine", L"Main_Window");
}