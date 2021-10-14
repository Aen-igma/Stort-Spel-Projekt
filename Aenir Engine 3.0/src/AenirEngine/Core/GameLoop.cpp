#include"PCH.h"
#include"GameLoop.h"

namespace Aen {

	GameLoop::GameLoop()
		:m_app(nullptr), m_start(), m_end(), m_frameTime(), m_deltaTime(), m_renderer(nullptr) {}

	void GameLoop::Initialize() {
		//m_app = CreateApp();                       fix this
		int ft = (int)(((double)1 / (double)60) * (double)pow(10, 9));
		m_frameTime = std::chrono::nanoseconds{ft};

		if(!Input::Initialize())
			exit(-1);

		if(!GCore::Concealed::Initialize(m_app->m_window))
			exit(-1);

		GlobalSettings::Initialize(m_app->m_window);


		m_renderer = AEN_NEW Renderer(m_app->m_window);
		m_renderer->Initialize();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		ImGui_ImplDX11_Init(GCore::m_device.Get(), GCore::m_dContext.Get());
		ImGui_ImplWin32_Init(m_app->m_window.GetWHND());
		ImGui::StyleColorsDark();

		//top left corner and bottom right corner of entire game world
		this->m_minX = 0.0f;
		this->m_minY = 0.0f;
		this->m_maxX = 50.0f;
		this->m_maxY = 50.0f;
		this->m_quadCap = 4;
		//AABB
		DirectX::XMFLOAT3 tempHalfExtent = DirectX::XMFLOAT3(m_maxX / 2, m_maxY / 2, m_maxY / 2);
		m_WorldBox = DirectX::BoundingBox(DirectX::XMFLOAT3(0, 0, 0), tempHalfExtent);
		//QuadTree
		m_Quadtree = new Quadtree(m_WorldBox, 0, 3, m_quadCap);

		//for-loop going through all static objects in the world and inserting them one by one
		for (int i = 0; i < 10; i++)
		{
			DirectX::BoundingBox* tempBox = new DirectX::BoundingBox(DirectX::XMFLOAT3(1.f + i * 4, 1.f, 0), 
				DirectX::XMFLOAT3(1.f, 1.f, 1.f));
			//m_Quadtree->insertObject(tempBox); 
			m_Quadtree->insertObject(i, tempBox);

		}
		m_PlayerCenter = DirectX::XMFLOAT3(10, 10, 0);
		m_PlayerSizes = DirectX::XMFLOAT3(1, 1, 1);
		m_TempPlayer = DirectX::BoundingBox(m_PlayerCenter, m_PlayerSizes);


		std::cout << std::endl;
		m_app->Start();
	}

	void GameLoop::Run() {
		
		m_start = m_end = ResClock::now();
		while(Aen::WindowHandle::HandleMsg()) {
		
			m_end = ResClock::now();
			while(std::chrono::duration_cast<std::chrono::nanoseconds>(m_end - m_start) > m_frameTime) {
				m_deltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(m_end - m_start);
				m_start = ResClock::now();

				if(m_app->m_window.IsActive()) {
					Input::Update();
					m_app->Update(static_cast<float>(m_deltaTime.count()));
				}
				if (GlobalSettings::GetMainCamera())
				{
					if (Input::KeyDown(Key::B))
					{
						// when updating to never math library, replace this with .smvec
						m_PlayerCenter = DirectX::XMFLOAT3(GlobalSettings::GetMainCamera()->GetPos().x, 
							GlobalSettings::GetMainCamera()->GetPos().y, 
							GlobalSettings::GetMainCamera()->GetPos().z);
						m_TempPlayer = DirectX::BoundingBox(m_PlayerCenter, m_PlayerSizes);
						m_Quadtree->getRoot()->inside(m_TempPlayer);
					}
					if (Input::KeyDown(Key::C))
					{
						std::cout << "Camera Position: " << GlobalSettings::GetMainCamera()->GetPos().x << ", " <<
							GlobalSettings::GetMainCamera()->GetPos().y << ", " <<
							GlobalSettings::GetMainCamera()->GetPos().z << std::endl;
					}
				}
				
				m_renderer->Render(); // VSync
				
			}
		}
		
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		Resource::Destroy();
		GCore::Concealed::Release();

		//Quadtree
		delete m_Quadtree;

		delete m_app;
		delete m_renderer;
	}
}