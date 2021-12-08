#include"PCH.h"
#include"GameLoop.h"

namespace Aen {

	GameLoop::GameLoop()
		:m_app(nullptr), /*m_start(), m_end(), m_frameTime(), m_deltaTime(),*/ m_renderer(nullptr) {}

	void GameLoop::Initialize() {
		//m_app = CreateApp();                       fix this
		//int ft = (int)(((double)1 / (double)60) * (double)pow(10, 9));
		/*m_frameTime = std::chrono::nanoseconds{ft};
		m_frametime = ft / pow(10, 9);*/

		if(!Input::Initialize())
			exit(-1);

		if(!GCore::Concealed::Initialize(m_app->m_window))
			exit(-1);

		// Initialize physX
		PhysicsHandler::Initialize(100, 981);
		
		m_renderer = AEN_NEW Renderer(m_app->m_window);
		m_renderer->Initialize();

		GlobalSettings::Initialize(m_app->m_window, m_renderer);
		GlobalSettings::SetVSync(true);

		m_app->Start();
	}
	
	void GameLoop::Run() {
		bool useVsync = GlobalSettings::GetVSync();
		
		/*m_start = m_end = ResClock::now();
		while(Aen::WindowHandle::HandleMsg()) {

			m_end = ResClock::now();
			while(std::chrono::duration_cast<std::chrono::nanoseconds>(m_end - m_start) > m_frameTime) {
				m_deltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(m_end - m_start);
				m_start = ResClock::now();

				if(m_app->m_window.IsActive()) {
					Input::Update();
					m_app->Update(static_cast<float>(m_deltaTime.count()));
				}

				PhysicsHandler::Update(m_deltaTime.count());
			}

			ComponentHandler::UpdateAnimation();
			m_renderer->Culling();
			m_renderer->Render();
		}*/

		double sStart = 0, deltaTime = 0;
		while (Aen::WindowHandle::HandleMsg()) {

			sStart = omp_get_wtime();
			if (m_app->m_window.IsActive()) {

				Input::Update();
				m_app->Update(static_cast<float>(deltaTime));
				ComponentHandler::UpdatePS(deltaTime);
			}
			PhysicsHandler::Update(static_cast<float>(deltaTime));
	
			m_renderer->Culling();
			m_renderer->Render();

			deltaTime = (omp_get_wtime() - sStart);

			ComponentHandler::UpdateAnimation();
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

	void GameLoop::InitApp(Aen::App* app)
	{
		m_app = app;
	}
}