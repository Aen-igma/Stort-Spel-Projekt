#include "PCH.h"
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
		this->m_maxX = 1000.0f;
		this->m_maxY = 1000.0f;
		//AABB
		m_AxisAlignedBoundBox = new AABB(this->m_minX, this->m_minY, this->m_maxX, this->m_maxY);
		//QuadTree
		m_Quadtree = new Quadtree(*m_AxisAlignedBoundBox, m_quadCap, 3);
		//Object
		//m_Object = new Object(m_AxisAlignedBoundBox);

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

				m_renderer->Render(); // VSync
				
			}
		}
		
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		Resource::Destroy();
		GCore::Concealed::Release();

		//Quadtree
		m_Quadtree->clear();
		

		delete m_app;
		delete m_renderer;
	}
}