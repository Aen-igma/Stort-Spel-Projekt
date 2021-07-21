#include"AenirEngine.h"
#include "Client.h"



Client::~Client() {

}

Client::Client(const Aen::WindowDesc& desc, const std::wstring& windowName, const std::wstring& className)
	:Aen::App(desc, windowName, className) {}

void Client::Start() {

	m_camera.AddComponent<Aen::Camera>();
	m_camera.GetComponent<Aen::Camera>().SetCameraPerspective(90.f, window.GetAspectRatio(), 0.1f, 100.f);
	Aen::GlobalSettings::SetMainCamera(m_camera);

	Aen::Mesh* pCube = Aen::MeshHandler::CreateMesh("cube");
	Aen::Material* pMaterial = Aen::MaterialHandler::CreateMaterial("cubeMaterial");
	Aen::Texture* pTexture = Aen::TextureHandler::CreateTexture("Reimu");

	pCube->Load("../Resource/Test/Cube.obj");
	pTexture->LoadTexture("../Resource/Test/Reimu.png");
	pMaterial->CreateDefault();
	pMaterial->SetDiffuseMap(*pTexture);

	m_cube.AddComponent<Aen::MeshInstance>();
	m_cube.AddComponent<Aen::MaterialInstance>();

	m_cube.GetComponent<Aen::MeshInstance>().SetMesh(pCube);
	m_cube.GetComponent<Aen::MaterialInstance>().SetMaterial(pMaterial);

	m_camera.SetPos(0.f, 0.f, 0.f);
	m_cube.SetPos(0.f, 0.f, -5.f);
}

void Client::Update(const float& deltaTime) {

}
