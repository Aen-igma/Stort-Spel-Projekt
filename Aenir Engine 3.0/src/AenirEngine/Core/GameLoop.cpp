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
		DirectX::XMFLOAT3 tempWorldCenter = DirectX::XMFLOAT3(m_maxX/2, m_maxY/2, 0);
		DirectX::XMFLOAT3 tempHalfExtent = DirectX::XMFLOAT3(m_maxX / 2, m_maxY / 2, m_maxY / 2);
		m_WorldBox = DirectX::BoundingBox(tempWorldCenter, tempHalfExtent);
		//m_AxisAlignedBoundBox = new AABB(this->m_minX, this->m_minY, this->m_maxX, this->m_maxY);
		//QuadTree
		m_Quadtree = new Quadtree(m_WorldBox, 0, 3, m_quadCap);
		//m_Quadtree = new Quadtree(*m_AxisAlignedBoundBox, 0, 3, m_quadCap);
		//List of all static objects in the world bellow

		m_boundingBoxes = new DirectX::BoundingBox[10];
		for (int i = 0; i < 10; i++)
		{
			m_boundingBoxes[i] = DirectX::BoundingBox(DirectX::XMFLOAT3(1.f + i*4, 1.f, 0.f), DirectX::XMFLOAT3(1.f, 1.f, 1.f));
		}
		//for-loop going through all static objects in the world and inserting them one by one
		for (int i = 0; i < 10; i++)
		{
			m_Quadtree->insertNode(&m_boundingBoxes[i]);
		}
		DirectX::XMFLOAT3 playerCenter = DirectX::XMFLOAT3(10, 10, 0);
		m_TempPlayer = DirectX::BoundingBox(playerCenter, DirectX::XMFLOAT3(1, 1, 1));


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
				if(Input::KeyPress(Key::B))
				{
					m_Quadtree->getRoot()->inside(m_TempPlayer);
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
		//m_Quadtree->clear();
		

		delete m_app;
		delete m_renderer;
	}
}