#include"AenirEngine.h"
#include"Client.h"

Client::~Client() {

}

Client::Client(const Aen::WindowDesc& desc, const std::wstring& windowName, const std::wstring& className)
	:Aen::App(desc, windowName, className), m_speed(10.f), m_mouseSense(50.f), m_toggleCamera(true) {}

void Client::Start() {

	m_camera.AddComponent<Aen::Camera>();
	m_camera.GetComponent<Aen::Camera>().SetCameraPerspective(90.f, window.GetAspectRatio(), 0.01f, 100.f);
	Aen::GlobalSettings::SetMainCamera(m_camera);

	Aen::Mesh* pCube = Aen::ResourceHandler::CreateMesh("cube");
	Aen::Mesh* pPlane = Aen::ResourceHandler::CreateMesh("plane");
	Aen::Material* pMaterial = Aen::ResourceHandler::CreateMaterial("cubeMaterial");
	Aen::Texture* pTexture = Aen::ResourceHandler::CreateTexture("Reimu");

	pCube->Load("../Resource/Test/Cube.obj");
	pPlane->Load("../Resource/Test/Plane.obj");
	pTexture->LoadTexture("../Resource/Test/Reimu.png");
	pMaterial->CreateDefault();
	pMaterial->SetDiffuseMap(*pTexture);

	m_cube.AddComponent<Aen::MeshInstance>();
	m_cube.AddComponent<Aen::MaterialInstance>();
	m_cube2.AddComponent<Aen::MeshInstance>();
	m_cube2.AddComponent<Aen::MaterialInstance>();

	m_cube.GetComponent<Aen::MeshInstance>().SetMesh(pCube);
	m_cube.GetComponent<Aen::MaterialInstance>().SetMaterial(pMaterial);
	m_cube2.GetComponent<Aen::MeshInstance>().SetMesh(pCube);
	m_cube2.GetComponent<Aen::MaterialInstance>().SetMaterial(pMaterial);

	m_camera.SetPos(0.f, 0.f, 0.f);
	m_cube.SetPos(0.f, 0.f, 3.f);
	m_cube2.SetPos(2.f, 0.f, 0.f);

	m_cube2.SetParent(m_cube);

	m_plane.AddComponent<Aen::MeshInstance>();
	m_plane.AddComponent<Aen::MaterialInstance>();

	m_plane.GetComponent<Aen::MeshInstance>().SetMesh(pPlane);
	m_plane.GetComponent<Aen::MaterialInstance>().SetMaterial(pMaterial);
	m_plane.SetPos(0.f, -1.f, 3.f);
	m_plane.SetScale(10.f, 10.f, 10.f);

	m_sLight.AddComponent<Aen::SpotLight>();
	m_sLight.GetComponent<Aen::SpotLight>().SetColor(Aen::Color::Green);
	m_sLight.GetComponent<Aen::SpotLight>().SetLightDist(0.4f, 0.02f, 0.f, 100.f);
	m_sLight.GetComponent<Aen::SpotLight>().SetStrength(1.f);
	m_sLight.GetComponent<Aen::SpotLight>().SetConeSize(20.f);
	m_sLight.SetPos(0.f, 2.f, 3.f);

	m_pLight.AddComponent<Aen::PointLight>();
	m_pLight.GetComponent<Aen::PointLight>().SetColor(Aen::Color::Red);
	m_pLight.GetComponent<Aen::PointLight>().SetLightDist(0.4f, 0.02f, 0.f, 5.f);
	m_pLight.GetComponent<Aen::PointLight>().SetStrength(1.f);
	m_pLight.SetPos(0.f, 0.f, 2.f);
	
	m_dLight.AddComponent<Aen::DirectionalLight>();
	m_dLight.GetComponent<Aen::DirectionalLight>().SetColor(Aen::Color::Cyan);
	m_dLight.GetComponent<Aen::DirectionalLight>().SetStrength(1.f);
	m_dLight.SetRot(45.f, 135.f, 0.f);

	AEN_PRINT("");

	Aen::Input::SetMouseVisible(false);
}

void Client::Update(const float& deltaTime) {

	m_cube.Rotate(0.f, 20.f * deltaTime, 0.f);

	Aen::Vec3f axis;
	axis.x = (float)Aen::Input::KeyPress(Aen::Key::D) - (float)Aen::Input::KeyPress(Aen::Key::A);
	axis.y = (float)Aen::Input::KeyPress(Aen::Key::SPACE) - (float)Aen::Input::KeyPress(Aen::Key::LSHIFT);
	axis.z = (float)Aen::Input::KeyPress(Aen::Key::W) - (float)Aen::Input::KeyPress(Aen::Key::S);

	static Aen::Vec2i mouseAxis;
	mouseAxis = Aen::Input::GetRawMouse();

	if(Aen::Input::KeyDown(Aen::Key::ESCAPE))
		window.Exit();

	if(Aen::Input::KeyDown(Aen::Key::TAB)) {
		Aen::Input::SetMouseVisible(m_toggleCamera);
		m_toggleCamera = !m_toggleCamera;
	}

	if(m_toggleCamera) {
		Aen::Input::SetMousePos(window.GetWindowPos() + (Aen::Vec2i)((Aen::Vec2f)window.GetSize() * 0.5f));
		m_camera.MoveRelative(axis.x * deltaTime * m_speed, 0.f, axis.z * deltaTime * m_speed);
		m_camera.Move(0.f, axis.y * deltaTime * m_speed, 0.f);
		m_camera.Rotate((float)mouseAxis.y * deltaTime * m_mouseSense, (float)mouseAxis.x * deltaTime * m_mouseSense, 0.f);
	}
}
