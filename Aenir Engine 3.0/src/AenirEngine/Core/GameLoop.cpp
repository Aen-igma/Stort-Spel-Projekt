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

		// Initialize physX
		PhysXService::SetInstance(&m_PhysX);
		PhysXService::GetInstance()->InitPhysics(100, 981);

		// Initialize Quadtree
		m_WorldBox = DirectX::BoundingBox(DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(25.f, 25.f, 25.f));
		m_Quadtree = new Quadtree(m_WorldBox, 0, 3, 4);

		m_Quadtree->Initialize(); 

		
		GlobalSettings::Initialize(m_app->m_window);

		m_renderer = AEN_NEW Renderer(m_app->m_window);
		m_renderer->Initialize();

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

				//m_Quadtree->Update(m_QuadObjectsToRender);
				m_Quadtree->Update();

				PhysXService::GetInstance()->RunPhysics(m_deltaTime.count());
				m_renderer->Render(); // VSync
			}
		}

		// Destroy imGui
		delete Aen::GlobalSettings::GetImGuiHandler();

		PhysXService::GetInstance()->ClosePhysics();
		
		Resource::Destroy();
		GCore::Concealed::Release();
		delete m_app;
		delete m_renderer;
	}
}