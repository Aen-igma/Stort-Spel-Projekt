#include"AenirEngine.h"
#include"Client.h"

Client::~Client() {

}

Client::Client(const Aen::WindowDesc& desc, const std::wstring& windowName, const std::wstring& className)
	:Aen::App(desc, windowName, className), m_speed(10.f), m_fSpeed(0.15f), m_mouseSense(50.f), m_toggleCamera(true) {}

void Client::Start() {

	m_camera.AddComponent<Aen::Camera>();
	m_camera.GetComponent<Aen::Camera>().SetCameraPerspective(100.f, window.GetAspectRatio(), 0.01f, 100.f);
	m_camera.SetPos(0.f, 0.f, 0.f);

	Aen::GlobalSettings::SetMainCamera(m_camera);
	Aen::GlobalSettings::SetBGColor(Aen::Color::White);

	Aen::Mesh& pPlane = Aen::ResourceHandler::CreateMesh("plane");
	//Aen::Mesh& aniMesh = Aen::ResourceHandler::CreateMesh("ani");
	Aen::Mesh& seeUMesh = Aen::ResourceHandler::CreateMesh("seeu");
	Aen::Mesh& sphereMesh = Aen::ResourceHandler::CreateMesh("sphereMesh");

	pPlane.Load("../Resource/Test/Plane.obj");
	//aniMesh.Load("../Resource/Ani/Ani.obj");
	seeUMesh.Load("../Resource/Seeu/Seeu.obj");
	sphereMesh.Load("../Resource/Test/sphere.obj");

	Aen::Material& pMaterial1 = Aen::ResourceHandler::CreateMaterial("cubeMaterial");
	/*Aen::Material& aniClothes = Aen::ResourceHandler::CreateMaterial("aniClothes");
	Aen::Material& aniBody = Aen::ResourceHandler::CreateMaterial("aniBody");
	Aen::Material& aniHair = Aen::ResourceHandler::CreateMaterial("aniHair");
	Aen::Material& aniExp = Aen::ResourceHandler::CreateMaterial("aniExp");*/

	Aen::Material& seeU00 = Aen::ResourceHandler::CreateMaterial("seeU00");
	Aen::Material& seeU01 = Aen::ResourceHandler::CreateMaterial("seeU01");
	Aen::Material& seeU02 = Aen::ResourceHandler::CreateMaterial("seeU02");
	Aen::Material& seeU03 = Aen::ResourceHandler::CreateMaterial("seeU03");
	Aen::Material& seeU04 = Aen::ResourceHandler::CreateMaterial("seeU04");
	Aen::Material& seeU05 = Aen::ResourceHandler::CreateMaterial("seeU05");

	Aen::Material& glow = Aen::ResourceHandler::CreateMaterial("glow");
	m_mat = &glow;

	/*aniClothes.LoadeAndSetDiffuseMap("../Resource/Ani/Cloths_1.png");
	aniBody.LoadeAndSetDiffuseMap("../Resource/Ani/Body.png");
	aniHair.LoadeAndSetDiffuseMap("../Resource/Ani/Hair.png");
	aniExp.LoadeAndSetDiffuseMap("../Resource/Ani/Exp.png");*/

	seeU00.LoadeAndSetDiffuseMap("../Resource/Seeu/SeeU00.png");
	seeU01.LoadeAndSetDiffuseMap("../Resource/Seeu/SeeU01.png");
	seeU02.LoadeAndSetDiffuseMap("../Resource/Seeu/SeeU02.png");
	seeU03.LoadeAndSetDiffuseMap("../Resource/Seeu/SeeU03.png");
	seeU04.LoadeAndSetDiffuseMap("../Resource/Seeu/SeeU04.png");

	pMaterial1["SpecularColor"] = Aen::Color::White;
	pMaterial1["SpecularPower"] = 1.f;
	pMaterial1["SpecularStrength"] = 0.3f;
	pMaterial1["Roughness"] = 1.f;
	pMaterial1["BaseColor"] = Aen::Color::Gray;

	pPlane.SetMaterial(pMaterial1);
	m_plane.AddComponent<Aen::MeshInstance>();
	m_plane.GetComponent<Aen::MeshInstance>().SetMesh(pPlane);
	m_plane.SetPos(0.f, -1.f, 3.f);
	m_plane.SetScale(60.f, 1.f, 20.f);
	
	m_plane1.AddComponent<Aen::MeshInstance>();
	m_plane1.GetComponent<Aen::MeshInstance>().SetMesh(pPlane);
	m_plane1.SetPos(0.f, 19.f, 13.f);
	m_plane1.SetScale(60.f, 1.f, 40.f);
	m_plane1.Rotate(90.f, 0.f, 0.f);
	
	m_dLight.AddComponent<Aen::DirectionalLight>();
	m_dLight.GetComponent<Aen::DirectionalLight>().SetColor(Aen::Color::White);
	m_dLight.GetComponent<Aen::DirectionalLight>().SetStrength(0.6f);
	m_dLight.SetRot(55.f, 165.f, 0.f);
	
	/*AEN_ENDL;

	aniClothes["OuterEdgeThickness"] = 0.0012f;
	aniClothes["InnerEdgeThickness"] = 0.0012f;
	aniClothes["OuterEdgeColor"] = Aen::Color(0.1f, 0.16f, 0.23f, 1.f);
	aniClothes["InnerEdgeColor"] = Aen::Color(0.1f, 0.16f, 0.23f, 1.f);
	aniClothes["SpecularColor"] = Aen::Color(0.5f, 1.f, 1.f, 1.f);
	aniClothes["SpecularPower"] = 300.f;

	aniBody["OuterEdgeThickness"] = 0.f;
	aniBody["InnerEdgeThickness"] = 0.0004f;
	aniBody["OuterEdgeColor"] = Aen::Color(0.1f, 0.1f, 0.1f, 1.f);
	aniBody["InnerEdgeColor"] = Aen::Color(0.1f, 0.1f, 0.1f, 1.f);
	aniBody["SpecularColor"] = Aen::Color(1.f, 0.f, 0.0f, 0.05f);
	aniBody["SpecularPower"] = 50.f;

	aniHair["OuterEdgeThickness"] = 0.0003f;
	aniHair["InnerEdgeThickness"] = 0.0004f;
	aniHair["OuterEdgeColor"] = Aen::Color(0.f, 0.07f, 0.2f, 1.f);
	aniHair["InnerEdgeColor"] = Aen::Color(0.f, 0.07f, 0.2f, 1.f);
	aniHair["SpecularColor"] = Aen::Color::White;
	aniHair["SpecularPower"] = 100.f;

	aniExp["OuterEdgeThickness"] = 0.f;
	aniExp["InnerEdgeThickness"] = 0.f;
	aniExp["OuterEdgeColor"] = Aen::Color::Black;
	aniExp["InnerEdgeColor"] = Aen::Color::Black;
	aniExp["SpecularColor"] = Aen::Color(0.f, 0.f, 0.f, 0.f);
	aniExp["SpecularPower"] = 0.f;

	aniMesh.PrintMaterialSlots();
	aniMesh.SetMaterial("aniClothes", aniClothes);
	aniMesh.SetMaterial("aniBody", aniBody);
	aniMesh.SetMaterial("aniHair", aniHair);
	aniMesh.SetMaterial("aniExp", aniExp);

	m_Ani.AddComponent<Aen::MeshInstance>();
	m_Ani.GetComponent<Aen::MeshInstance>().SetMesh(aniMesh);
	m_Ani.SetPos(-2.f, -1.f, 3.f);
	m_Ani.SetRot(0.f, 180.f, 0.f);*/

	AEN_ENDL;

	seeU00["OuterEdgeThickness"] = 0.004f;
	seeU00["InnerEdgeThickness"] = 0.002f;
	seeU00["OuterEdgeColor"] = Aen::Color(0.08f, 0.08f, 0.1f, 1.f);
	seeU00["InnerEdgeColor"] = Aen::Color(0.08f, 0.08f, 0.1f, 1.f);
	seeU00["SpecularColor"] = Aen::Color(1.f, 0.4f, 0.f, 1.f);
	seeU00["SpecularPower"] = 0.5f;
	seeU00["SpecularStrength"] = 0.3f;
	seeU00["Roughness"] = 0.5f;
	seeU00["ShadowOffset"] = -0.3f;

	seeU01["OuterEdgeThickness"] = 0.001f;
	seeU01["InnerEdgeThickness"] = 0.003f;
	seeU01["OuterEdgeColor"] = Aen::Color(0.3f, 0.15f, 0.f, 1.f);
	seeU01["InnerEdgeColor"] = Aen::Color(0.3f, 0.15f, 0.f, 1.f);
	seeU01["SpecularColor"] = Aen::Color(1.f, 0.5f, 0.f, 1.f);
	seeU01["SpecularPower"] = 0.05f;
	seeU01["SpecularStrength"] = 0.2f;
	seeU01["Roughness"] = 1.f;
	seeU01["ShadowOffset"] = 0.4f;

	seeU02["OuterEdgeThickness"] = 0.003f;
	seeU02["InnerEdgeThickness"] = 0.003f;
	seeU02["OuterEdgeColor"] = Aen::Color(0.1f, 0.1f, 0.1f, 1.f);
	seeU02["InnerEdgeColor"] = Aen::Color(0.1f, 0.1f, 0.1f, 1.f);
	seeU02["SpecularColor"] = Aen::Color::Orange;
	seeU02["SpecularPower"] = 0.1f;
	seeU02["SpecularStrength"] = 0.1f;
	seeU02["Roughness"] = 1.f;

	seeU03["OuterEdgeThickness"] = 0.008f;
	seeU03["InnerEdgeThickness"] = 0.003f;
	seeU03["OuterEdgeColor"] = Aen::Color(0.5f, 0.1f, 0.f, 1.f);
	seeU03["InnerEdgeColor"] = Aen::Color(0.5f, 0.1f, 0.f, 1.f);
	seeU03["SpecularColor"] = Aen::Color::Orange;
	seeU03["SpecularPower"] = 0.1f;
	seeU03["SpecularStrength"] = 0.2f;
	seeU03["Roughness"] = 1.f;

	seeU04["OuterEdgeThickness"] = 0.003f;
	seeU04["InnerEdgeThickness"] = 0.003f;
	seeU04["OuterEdgeColor"] = Aen::Color(0.5f ,0.25f, 0.f, 1.f);
	seeU04["InnerEdgeColor"] = Aen::Color(0.5f, 0.25f, 0.f, 1.f);
	seeU04["SpecularColor"] = Aen::Color::Orange;
	seeU04["SpecularPower"] = 0.2f;
	seeU04["SpecularStrength"] = 0.1f;
	seeU04["Roughness"] = 1.f;

	seeU05["OuterEdgeThickness"] = 0.003f;
	seeU05["InnerEdgeThickness"] = 0.003f;
	seeU05["OuterEdgeColor"] = Aen::Color::Cyan;
	seeU05["InnerEdgeColor"] = Aen::Color::Cyan;
	seeU05["BaseColor"] = Aen::Color::Cyan;
	seeU05["SpecularColor"] = Aen::Color::White;
	seeU05["SpecularPower"] = 0.f;
	seeU05["SpecularStrength"] = 0.f;
	seeU05["Roughness"] = 0.f;

	seeUMesh.PrintMaterialSlots();
	seeUMesh.SetMaterial("seeU00", seeU00);
	seeUMesh.SetMaterial("seeU01", seeU01);
	seeUMesh.SetMaterial("seeU02", seeU02);
	seeUMesh.SetMaterial("seeU03", seeU03);
	seeUMesh.SetMaterial("seeU04", seeU04);
	seeUMesh.SetMaterial("seeU05", seeU05);

	m_SeeU.AddComponent<Aen::MeshInstance>();
	m_SeeU.GetComponent<Aen::MeshInstance>().SetMesh(seeUMesh);
	m_SeeU.SetPos(0.f, 7.7f, 2.f);
	m_SeeU.SetScale(0.06f, 0.06f, 0.06f);
	m_SeeU.SetRot(0.f, 180.f, 0.f);

	glow["OuterEdgeThickness"] = 0.003f;
	glow["InnerEdgeThickness"] = 0.0f;
	glow["OuterEdgeColor"] = Aen::Color::Black;
	glow["InnerEdgeColor"] = Aen::Color::Black;
	glow["BaseColor"] = Aen::Color(0.1f, 0.1f, 0.4f, 1.f);
	glow["SpecularColor"] = Aen::Color::White;
	glow["SpecularPower"] = 0.2f;
	glow["SpecularStrength"] = 1.f;
	glow["Roughness"] = 0.7f;

	sphereMesh.SetMaterial(glow);
	m_sphere.AddComponent<Aen::MeshInstance>();
	m_sphere.GetComponent<Aen::MeshInstance>().SetMesh(sphereMesh);
	m_sphere.SetPos(0.f, 0.f, 1.f);

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
