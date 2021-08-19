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

	Aen::Mesh& pPlane = Aen::Resource::CreateMesh("plane");
	Aen::Mesh& seeUMesh = Aen::Resource::CreateMesh("seeu");
	Aen::Mesh& sphereMesh1 = Aen::Resource::CreateMesh("sphereMesh1");
	Aen::Mesh& sphereMesh2 = Aen::Resource::CreateMesh("sphereMesh2");

	pPlane.Load(AEN_RESOURCE_DIR("Test/Plane.obj"));
	seeUMesh.Load(AEN_RESOURCE_DIR("Seeu/Seeu.obj"));
	sphereMesh1.Load(AEN_RESOURCE_DIR("Test/sphere.obj"));
	sphereMesh2.Load(AEN_RESOURCE_DIR("Test/sphere.obj"));

	Aen::Material& pMaterial1 = Aen::Resource::CreateMaterial("cubeMaterial");

	Aen::Material& seeU00 = Aen::Resource::CreateMaterial("seeU00");
	Aen::Material& seeU01 = Aen::Resource::CreateMaterial("seeU01");
	Aen::Material& seeU02 = Aen::Resource::CreateMaterial("seeU02");
	Aen::Material& seeU03 = Aen::Resource::CreateMaterial("seeU03");
	Aen::Material& seeU04 = Aen::Resource::CreateMaterial("seeU04");
	Aen::Material& seeU05 = Aen::Resource::CreateMaterial("seeU05");

	Aen::Material& seeU001 = Aen::Resource::CreateMaterial("seeU001");
	Aen::Material& seeU011 = Aen::Resource::CreateMaterial("seeU011");
	Aen::Material& seeU021 = Aen::Resource::CreateMaterial("seeU021");
	Aen::Material& seeU031 = Aen::Resource::CreateMaterial("seeU031");
	Aen::Material& seeU041 = Aen::Resource::CreateMaterial("seeU041");

	Aen::Material& red = Aen::Resource::CreateMaterial("red");
	Aen::Material& blue = Aen::Resource::CreateMaterial("blue");

	seeU00.LoadeAndSetDiffuseMap(AEN_RESOURCE_DIR("Seeu/SeeU00.png"));
	seeU01.LoadeAndSetDiffuseMap(AEN_RESOURCE_DIR("Seeu/SeeU01.png"));
	seeU02.LoadeAndSetDiffuseMap(AEN_RESOURCE_DIR("Seeu/SeeU02.png"));
	seeU03.LoadeAndSetDiffuseMap(AEN_RESOURCE_DIR("Seeu/SeeU03.png"));
	seeU04.LoadeAndSetDiffuseMap(AEN_RESOURCE_DIR("Seeu/SeeU04.png"));

	seeU001.SetDiffuseMap("SeeU00");
	seeU011.SetDiffuseMap("SeeU01");
	seeU021.SetDiffuseMap("SeeU02");
	seeU031.SetDiffuseMap("SeeU03");
	seeU041.SetDiffuseMap("SeeU04");

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

	AEN_ENDL;

	seeU00["OuterEdgeThickness"] = 0.0006f;
	seeU00["InnerEdgeThickness"] = 0.0004f;
	seeU00["OuterEdgeColor"] = Aen::Color(0.08f, 0.08f, 0.1f, 1.f);
	seeU00["InnerEdgeColor"] = Aen::Color(0.08f, 0.08f, 0.1f, 1.f);
	seeU00["SpecularColor"] = Aen::Color(1.f, 0.4f, 0.f, 1.f);
	seeU00["SpecularPower"] = 0.5f;
	seeU00["SpecularStrength"] = 0.3f;
	seeU00["Roughness"] = 0.5f;
	seeU00["ShadowOffset"] = -0.3f;
	seeU00["RimLightSize"] = 0.4f;
	seeU00["RimLightColor"] = Aen::Color::Red;
	seeU00["RimLightIntensity"] = 0.5f;

	seeU01["OuterEdgeThickness"] = 0.0003f;
	seeU01["InnerEdgeThickness"] = 0.0005f;
	seeU01["OuterEdgeColor"] = Aen::Color(0.3f, 0.15f, 0.f, 1.f);
	seeU01["InnerEdgeColor"] = Aen::Color(0.3f, 0.15f, 0.f, 1.f);
	seeU01["SpecularColor"] = Aen::Color(1.f, 0.5f, 0.f, 1.f);
	seeU01["SpecularPower"] = 0.05f;
	seeU01["SpecularStrength"] = 0.2f;
	seeU01["Roughness"] = 1.f;
	seeU01["ShadowOffset"] = 0.4f;

	seeU02["OuterEdgeThickness"] = 0.0005f;
	seeU02["InnerEdgeThickness"] = 0.0005f;
	seeU02["OuterEdgeColor"] = Aen::Color(0.1f, 0.1f, 0.1f, 1.f);
	seeU02["InnerEdgeColor"] = Aen::Color(0.1f, 0.1f, 0.1f, 1.f);
	seeU02["SpecularColor"] = Aen::Color::Orange;
	seeU02["SpecularPower"] = 0.1f;
	seeU02["SpecularStrength"] = 0.1f;
	seeU02["Roughness"] = 1.f;

	seeU03["OuterEdgeThickness"] = 0.001f;
	seeU03["InnerEdgeThickness"] = 0.0005f;
	seeU03["OuterEdgeColor"] = Aen::Color(0.5f, 0.1f, 0.f, 1.f);
	seeU03["InnerEdgeColor"] = Aen::Color(0.5f, 0.1f, 0.f, 1.f);
	seeU03["SpecularColor"] = Aen::Color::Orange;
	seeU03["SpecularPower"] = 0.1f;
	seeU03["SpecularStrength"] = 0.2f;
	seeU03["Roughness"] = 1.f;
	seeU03["RimLightSize"] = 0.45f;
	seeU03["RimLightColor"] = Aen::Color::Orange;
	seeU03["RimLightIntensity"] = 0.6f;

	seeU04["OuterEdgeThickness"] = 0.0005f;
	seeU04["InnerEdgeThickness"] = 0.0005f;
	seeU04["OuterEdgeColor"] = Aen::Color(0.3f ,0.1f, 0.f, 1.f);
	seeU04["InnerEdgeColor"] = Aen::Color(0.3f, 0.1f, 0.f, 1.f);
	seeU04["SpecularColor"] = Aen::Color::Orange;
	seeU04["SpecularPower"] = 0.2f;
	seeU04["SpecularStrength"] = 0.1f;
	seeU04["Roughness"] = 1.f;

	seeU05["OuterEdgeThickness"] = 0.0005f;
	seeU05["InnerEdgeThickness"] = 0.0005f;
	seeU05["OuterEdgeColor"] = Aen::Color::Cyan;
	seeU05["InnerEdgeColor"] = Aen::Color::Cyan;
	seeU05["BaseColor"] = Aen::Color::Cyan;
	seeU05["SpecularColor"] = Aen::Color::White;
	seeU05["SpecularPower"] = 0.f;
	seeU05["SpecularStrength"] = 0.f;
	seeU05["Roughness"] = 0.f;

	// ------------------------------------------------------

	seeU001["OuterEdgeThickness"] = 0.004f;
	seeU001["InnerEdgeThickness"] = 0.002f;
	seeU001["OuterEdgeColor"] = Aen::Color(0.08f, 0.08f, 0.1f, 1.f);
	seeU001["InnerEdgeColor"] = Aen::Color(0.08f, 0.08f, 0.1f, 1.f);
	seeU001["SpecularColor"] = Aen::Color(1.f, 0.4f, 0.f, 1.f);
	seeU001["SpecularPower"] = 0.5f;
	seeU001["SpecularStrength"] = 0.3f;
	seeU001["Roughness"] = 0.5f;
	seeU001["ShadowOffset"] = -0.3f;
	seeU001["RimLightSize"] = 0.4f;
	seeU001["RimLightColor"] = Aen::Color::Red;
	seeU001["RimLightIntensity"] = 0.5f;
		  
	seeU011["OuterEdgeThickness"] = 0.001f;
	seeU011["InnerEdgeThickness"] = 0.003f;
	seeU011["OuterEdgeColor"] = Aen::Color(0.3f, 0.15f, 0.f, 1.f);
	seeU011["InnerEdgeColor"] = Aen::Color(0.3f, 0.15f, 0.f, 1.f);
	seeU011["SpecularColor"] = Aen::Color(1.f, 0.5f, 0.f, 1.f);
	seeU011["SpecularPower"] = 0.05f;
	seeU011["SpecularStrength"] = 0.2f;
	seeU011["Roughness"] = 1.f;
	seeU011["ShadowOffset"] = 0.4f;
		  
	seeU021["OuterEdgeThickness"] = 0.003f;
	seeU021["InnerEdgeThickness"] = 0.003f;
	seeU021["OuterEdgeColor"] = Aen::Color(0.1f, 0.1f, 0.1f, 1.f);
	seeU021["InnerEdgeColor"] = Aen::Color(0.1f, 0.1f, 0.1f, 1.f);
	seeU021["SpecularColor"] = Aen::Color::Orange;
	seeU021["SpecularPower"] = 0.1f;
	seeU021["SpecularStrength"] = 0.1f;
	seeU021["Roughness"] = 1.f;
		  
	seeU031["OuterEdgeThickness"] = 0.008f;
	seeU031["InnerEdgeThickness"] = 0.003f;
	seeU031["OuterEdgeColor"] = Aen::Color(0.5f, 0.1f, 0.f, 1.f);
	seeU031["InnerEdgeColor"] = Aen::Color(0.5f, 0.1f, 0.f, 1.f);
	seeU031["SpecularColor"] = Aen::Color::Orange;
	seeU031["SpecularPower"] = 0.1f;
	seeU031["SpecularStrength"] = 0.2f;
	seeU031["Roughness"] = 1.f;
	seeU031["RimLightSize"] = 0.45f;
	seeU031["RimLightColor"] = Aen::Color::Orange;
	seeU031["RimLightIntensity"] = 0.6f;
		  
	seeU041["OuterEdgeThickness"] = 0.003f;
	seeU041["InnerEdgeThickness"] = 0.003f;
	seeU041["OuterEdgeColor"] = Aen::Color(0.3f, 0.1f, 0.f, 1.f);
	seeU041["InnerEdgeColor"] = Aen::Color(0.3f, 0.1f, 0.f, 1.f);
	seeU041["SpecularColor"] = Aen::Color::Orange;
	seeU041["SpecularPower"] = 0.2f;
	seeU041["SpecularStrength"] = 0.1f;
	seeU041["Roughness"] = 1.f;

	seeUMesh.PrintMaterialSlots();
	seeUMesh.SetMaterial("seeU00", seeU00);
	seeUMesh.SetMaterial("seeU01", seeU01);
	seeUMesh.SetMaterial("seeU02", seeU02);
	seeUMesh.SetMaterial("seeU03", seeU03);
	seeUMesh.SetMaterial("seeU04", seeU04);
	seeUMesh.SetMaterial("seeU05", seeU05);
	
	seeUMesh.SetMaterial("seeU001", seeU001);
	seeUMesh.SetMaterial("seeU011", seeU011);
	seeUMesh.SetMaterial("seeU021", seeU021);
	seeUMesh.SetMaterial("seeU031", seeU031);
	seeUMesh.SetMaterial("seeU041", seeU041);

	m_SeeU.AddComponent<Aen::MeshInstance>();
	m_SeeU.GetComponent<Aen::MeshInstance>().SetMesh(seeUMesh);
	m_SeeU.SetPos(0.f, 7.7f, 2.f);
	m_SeeU.SetScale(0.06f, 0.06f, 0.06f);
	m_SeeU.SetRot(0.f, 180.f, 0.f);

	red["OuterEdgeThickness"] = 0.005f;
	red["InnerEdgeThickness"] = 0.0f;	
	red["OuterEdgeColor"] = Aen::Color(0.2f, 0.f, 0.f, 1.f);
	red["InnerEdgeColor"] = Aen::Color(0.2f, 0.f, 0.f, 1.f);
	red["BaseColor"] = Aen::Color(0.6f, 0.1f, 0.f, 1.f);
	red["SpecularColor"] = Aen::Color::White;
	red["SpecularPower"] = 0.2f;
	red["SpecularStrength"] = 1.f;
	red["Roughness"] = 0.7f;
	red["RimLightSize"] = 0.35f;
	red["ShadowOffset"] = 0.4f;

	blue["OuterEdgeThickness"] = 0.005f;
	blue["InnerEdgeThickness"] = 0.0f;	
	blue["OuterEdgeColor"] = Aen::Color(0.f, 0.1f, 0.6f, 1.f);
	blue["InnerEdgeColor"] = Aen::Color(0.f, 0.1f, 0.6f, 1.f);
	blue["BaseColor"] = Aen::Color(0.f, 0.1f, 0.6f, 1.f);
	blue["SpecularColor"] = Aen::Color::White;
	blue["SpecularPower"] = 0.2f;
	blue["SpecularStrength"] = 1.f;
	blue["Roughness"] = 0.7f;
	blue["RimLightSize"] = 0.35f;
	blue["ShadowOffset"] = 0.4f;

	sphereMesh1.SetMaterial(red);
	m_sphere1.AddComponent<Aen::MeshInstance>();
	m_sphere1.GetComponent<Aen::MeshInstance>().SetMesh(sphereMesh1);
	m_sphere1.SetPos(3.f, 0.f, 3.f);

	sphereMesh2.SetMaterial(blue);
	m_sphere2.AddComponent<Aen::MeshInstance>();
	m_sphere2.GetComponent<Aen::MeshInstance>().SetMesh(sphereMesh2);
	m_sphere2.SetPos(5.f, 0.f, 2.f);

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
