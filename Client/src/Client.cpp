#include"AenirEngine.h"
#include"Client.h"

Client::~Client() {

}

Client::Client(const Aen::WindowDesc& desc, const std::wstring& windowName, const std::wstring& className)
	:Aen::App(desc, windowName, className), m_speed(10.f), m_fSpeed(0.15f), m_mouseSense(50.f), m_toggleCamera(true) {}

void Client::Start() {

	m_camera.AddComponent<Aen::Camera>();
	m_camera.GetComponent<Aen::Camera>().SetCameraPerspective(100.f, window.GetAspectRatio(), 0.01f, 100.f);
	Aen::GlobalSettings::SetMainCamera(m_camera);

	Aen::Mesh& pPlane = Aen::ResourceHandler::CreateMesh("plane");
	Aen::Mesh& pObamaDuck = Aen::ResourceHandler::CreateMesh("obamaDuckMesh");
	Aen::Mesh& pMiku = Aen::ResourceHandler::CreateMesh("miku");
	Aen::Mesh& p3Material = Aen::ResourceHandler::CreateMesh("3Material");

	Aen::Material& pMaterial1 = Aen::ResourceHandler::CreateMaterial("cubeMaterial");
	Aen::Material& pObamium = Aen::ResourceHandler::CreateMaterial("obamaiumkMat");
	Aen::Material& pSubaruDuck = Aen::ResourceHandler::CreateMaterial("subaruDuckMat");

	Aen::Material& pSphere = Aen::ResourceHandler::CreateMaterial("sphere");
	Aen::Material& pNotspehereIsCube = Aen::ResourceHandler::CreateMaterial("notspehereiscube");
	Aen::Material& pTorHammer = Aen::ResourceHandler::CreateMaterial("torhammer");

	pObamaDuck.Load("../Resource/Test2/ObamaDuck.obj");
	pPlane.Load("../Resource/Test/Plane.obj");
	pMiku.Load("../Resource/Test/Miku.obj");
	p3Material.Load("../Resource/Test3/3material.obj");

	pMaterial1.LoadeAndSetDiffuseMap("../Resource/Test/Reimu.png");
	pObamium.LoadeAndSetDiffuseMap("../Resource/Test2/Obama.png");
	pSubaruDuck.LoadeAndSetDiffuseMap("../Resource/Test2/SubaruDuck.gif");

	pSphere.LoadeAndSetDiffuseMap("../Resource/Test3/cont_texture.png");
	pNotspehereIsCube.LoadeAndSetDiffuseMap("../Resource/Test3/ayyy_cropped.png");
	pTorHammer.LoadeAndSetDiffuseMap("../Resource/Test3/Star_of_Chaos.jpg");

	pMaterial1["InnerEdgeThickness"] = 0.002f;
	pMaterial1["OuterEdgeThickness"] = 0.002f;
	pMaterial1["InnerEdgeColor"] = Aen::Color::Red;
	pMaterial1["OuterEdgeColor"] = Aen::Color::Red;

	m_camera.SetPos(0.f, 0.f, 0.f);

	m_plane.AddComponent<Aen::MeshInstance>();

	pPlane.SetMaterial(pMaterial1);
	m_plane.GetComponent<Aen::MeshInstance>().SetMesh(pPlane);
	m_plane.SetPos(0.f, -1.f, 3.f);
	m_plane.SetScale(10.f, 10.f, 10.f);

	pSubaruDuck["InnerEdgeColor"] = Aen::Color::Red;
	pSubaruDuck["OuterEdgeColor"] = Aen::Color::Red;
	pObamium["InnerEdgeColor"] = Aen::Color::Blue;
	pObamium["OuterEdgeColor"] = Aen::Color::Blue;
	
	pObamaDuck.PrintMaterialSlots();
	pObamaDuck.SetMaterial("lambert4SG", pObamium);
	pObamaDuck.SetMaterial("multiMaterials:lambert2SG", pSubaruDuck);

	m_obamaDuck.AddComponent<Aen::MeshInstance>();
	m_obamaDuck.GetComponent<Aen::MeshInstance>().SetMesh(pObamaDuck);
	m_obamaDuck.SetPos(0.f, 0.f, 10.f);
	
	m_dLight.AddComponent<Aen::DirectionalLight>();
	m_dLight.GetComponent<Aen::DirectionalLight>().SetColor(Aen::Color::White);
	m_dLight.GetComponent<Aen::DirectionalLight>().SetStrength(1.f);
	m_dLight.SetRot(45.f, 135.f, 0.f);

	AEN_ENDL;
	p3Material.PrintMaterialSlots();
	p3Material.SetMaterial("sphere", pSphere);
	p3Material.SetMaterial("torhammer", pTorHammer);
	p3Material.SetMaterial("notspehereiscube", pNotspehereIsCube);

	m_3Material.AddComponent<Aen::MeshInstance>();
	m_3Material.GetComponent<Aen::MeshInstance>().SetMesh(p3Material); 
	m_3Material.SetPos(0.f, 0.f, -3.f);
	
	AEN_ENDL;
	pMiku.PrintMaterialSlots();

	m_Miku.AddComponent<Aen::MeshInstance>();
	m_Miku.GetComponent<Aen::MeshInstance>().SetMesh(pMiku);
	m_Miku.SetPos(0.f, -1.f, 3.f);
	m_Miku.SetScale(0.15f, 0.15f, 0.15f);
	m_Miku.SetRot(0.f, 180.f, 0.f);

	Aen::Input::SetMouseVisible(false);
}

void Client::Update(const float& deltaTime) {

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
		float focus = (Aen::Input::KeyPress(Aen::Key::LCONTROL)) ? m_fSpeed : 1.f;
		Aen::Input::SetMousePos(window.GetWindowPos() + (Aen::Vec2i)((Aen::Vec2f)window.GetSize() * 0.5f));
		m_camera.MoveRelative(axis.x * deltaTime * m_speed * focus, 0.f, axis.z * deltaTime * m_speed * focus);
		m_camera.Move(0.f, axis.y * deltaTime * m_speed * focus, 0.f);
		m_camera.Rotate((float)mouseAxis.y * deltaTime * m_mouseSense, (float)mouseAxis.x * deltaTime * m_mouseSense, 0.f);
	}
}
