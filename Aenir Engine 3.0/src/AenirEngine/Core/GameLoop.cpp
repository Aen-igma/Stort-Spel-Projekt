#include"PCH.h"
#include"GameLoop.h"

namespace Aen {

	GameLoop::GameLoop()
		:m_app(nullptr), /*m_start(), m_end(), m_frameTime(), m_deltaTime(),*/ m_renderer(nullptr) {}

	void GameLoop::Initialize() {
		//m_app = CreateApp();                       fix this
		int ft = (int)(((double)1 / (double)60) * (double)pow(10, 9));
		//m_frameTime = std::chrono::nanoseconds{ft};
		m_frametime = ft / pow(10, 9);

		if(!Input::Initialize())
			exit(-1);

		if(!GCore::Concealed::Initialize(m_app->m_window))
			exit(-1);

		// Initialize physX
		PhysicsHandler::Initialize(100, 981);
		
		m_renderer = AEN_NEW Renderer(m_app->m_window);
		m_renderer->Initialize();

		GlobalSettings::Initialize(m_app->m_window, m_renderer);

		m_app->Start();
	}
	
	void GameLoop::Run() {
		//m_start = m_end = 0.01fResClock::now();
		bool useVsync = GlobalSettings::GetVSync();
		double sstart = 0, deltaTime = 0/*, /*frameTime = 0, endTime = 0*/;
		while(Aen::WindowHandle::HandleMsg()) {

			//endTime = omp_get_wtime();
			//m_end = ResClock::now();
			
			//while(std::chrono::duration_cast<std::chrono::nanoseconds>(m_end - m_start) > m_frameTime) {
			//while ((endTime - sstart) > m_frametime) {
				//m_deltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(m_end - m_start);
				//m_start = ResClock::now();
				//deltaTime = (endTime - sstart);
				sstart = omp_get_wtime();

				if (m_app->m_window.IsActive()) {

					Input::Update();
					m_app->Update(static_cast<float>(deltaTime));
				}

				PhysicsHandler::Update(static_cast<float>(deltaTime));
				//PhysicsHandler::Update(static_cast<float>(m_deltaTime.count()));

				if (useVsync)
					m_renderer->Render();
		//	}

			if(!useVsync)
				m_renderer->Render();
			
			deltaTime = (omp_get_wtime() - sstart);
		}

		// Destroy imGui

		//delete Aen::GlobalSettings::GetImGuiHandler();
		
		delete m_app;
		Resource::Destroy();
		EntityHandler::Destroy();
		GCore::Concealed::Release();
		GlobalSettings::Destroy();
		PhysicsHandler::Destroy();
		delete m_renderer;
	}
}