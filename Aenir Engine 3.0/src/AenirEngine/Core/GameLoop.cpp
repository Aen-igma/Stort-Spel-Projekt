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

		//QuadTree
	
		//Node a(Point(1, 1), 1);
		//Node b(Point(2, 5), 2);
		//Node c(Point(7, 6), 3);
		//m_Center.Insert(&a);
		//m_Center.Insert(&b);
		//m_Center.Insert(&c);

	

		Quadtree m_Center(Point(1, 1), Point(8, 8));
		Node a(Point(1, 1), 1);
		Node b(Point(2, 5), 2);
		Node c(Point(7, 6), 3);
		m_Center.Insert(&a);
		m_Center.Insert(&b);
		m_Center.Insert(&c);

		/*std::string word = std::to_string(m_Center->Search(Point(1, 1))->m_data);*/
	

		//Vet inte hur man ska fixa detta  får fråga senare

		/*std::cout << "Node A: " << m_Center->Search(Point(1,1))->m_data << "\n";*/

		//std::cout << "Node A: " << m_Center->Search(Point(1,1))->m_data << "\n";
		//std::cout << "Node B: " << m_Center.search(Point(2,5))->m_data << "\n";
		//std::cout << "Node C: " << m_Center.search(Point(7,6))->m_data << "\n";
		//std::cout << "None-Existing node: " << m_Center.search(Point(5, 5));
		std::cout << std::endl;



		m_renderer = AEN_NEW Renderer(m_app->m_window);
		m_renderer->Initialize();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		ImGui_ImplDX11_Init(GCore::m_device.Get(), GCore::m_dContext.Get());
		ImGui_ImplWin32_Init(m_app->m_window.GetWHND());
		ImGui::StyleColorsDark();

	

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
		delete m_app;
		delete m_renderer;
	}
}